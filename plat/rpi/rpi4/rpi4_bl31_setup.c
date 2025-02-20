/*
 * Copyright (c) 2015-2019, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>

#include <libfdt.h>

#include <platform_def.h>
#include <arch_helpers.h>
#include <common/bl_common.h>
#include <lib/mmio.h>
#include <lib/xlat_tables/xlat_mmu_helpers.h>
#include <lib/xlat_tables/xlat_tables_defs.h>
#include <lib/xlat_tables/xlat_tables_v2.h>
#include <plat/common/platform.h>
#include <common/fdt_fixup.h>
#include <common/fdt_wrappers.h>
#include <libfdt.h>

#include <drivers/arm/gicv2.h>

#include <rpi_shared.h>

/*
 * Fields at the beginning of armstub8.bin.
 * While building the BL31 image, we put the stub magic into the binary.
 * The GPU firmware detects this at boot time, clears that field as a
 * confirmation and puts the kernel and DT address in the following words.
 */
extern uint32_t stub_magic;
extern uint32_t dtb_ptr32;
extern uint32_t kernel_entry32;

static const gicv2_driver_data_t rpi4_gic_data = {
	.gicd_base = RPI4_GIC_GICD_BASE,
	.gicc_base = RPI4_GIC_GICC_BASE,
};

/*
 * To be filled by the code below. At the moment BL32 is not supported.
 * In the future these might be passed down from BL2.
 */
static entry_point_info_t bl32_image_ep_info;
static entry_point_info_t bl33_image_ep_info;

/*******************************************************************************
 * Return a pointer to the 'entry_point_info' structure of the next image for
 * the security state specified. BL33 corresponds to the non-secure image type
 * while BL32 corresponds to the secure image type. A NULL pointer is returned
 * if the image does not exist.
 ******************************************************************************/
entry_point_info_t *bl31_plat_get_next_image_ep_info(uint32_t type)
{
	entry_point_info_t *next_image_info;

	assert(sec_state_is_valid(type) != 0);

	next_image_info = (type == NON_SECURE)
			? &bl33_image_ep_info : &bl32_image_ep_info;

	/* None of the images can have 0x0 as the entrypoint. */
	if (next_image_info->pc) {
		return next_image_info;
	} else {
		return NULL;
	}
}

uintptr_t plat_get_ns_image_entrypoint(void)
{
#ifdef PRELOADED_BL33_BASE
	return PRELOADED_BL33_BASE;
#else
	/* Cleared by the GPU if kernel address is valid. */
	if (stub_magic == 0)
		return kernel_entry32;

	WARN("Stub magic failure, using default kernel address 0x80000\n");
	return 0x80000;
#endif
}

static uintptr_t rpi4_get_dtb_address(void)
{
#ifdef RPI3_PRELOADED_DTB_BASE
	return RPI3_PRELOADED_DTB_BASE;
#else
	/* Cleared by the GPU if DTB address is valid. */
	if (stub_magic == 0)
		return dtb_ptr32;

	WARN("Stub magic failure, DTB address unknown\n");
	return 0;
#endif
}

static void ldelay(register_t delay)
{
	__asm__ volatile (
		"1:\tcbz %0, 2f\n\t"
		"sub %0, %0, #1\n\t"
		"b 1b\n"
		"2:"
		: "=&r" (delay) : "0" (delay)
	);
}

/*******************************************************************************
 * Perform any BL31 early platform setup. Here is an opportunity to copy
 * parameters passed by the calling EL (S-EL1 in BL2 & EL3 in BL1) before
 * they are lost (potentially). This needs to be done before the MMU is
 * initialized so that the memory layout can be used while creating page
 * tables. BL2 has flushed this information to memory, so we are guaranteed
 * to pick up good data.
 ******************************************************************************/
void bl31_early_platform_setup2(u_register_t arg0, u_register_t arg1,
				u_register_t arg2, u_register_t arg3)

{
	/*
	 * LOCAL_CONTROL:
	 * Bit 9 clear: Increment by 1 (vs. 2).
	 * Bit 8 clear: Timer source is 19.2MHz crystal (vs. APB).
	 */
	mmio_write_32(RPI4_LOCAL_CONTROL_BASE_ADDRESS, 0);

	/* LOCAL_PRESCALER; divide-by (0x80000000 / register_val) == 1 */
	mmio_write_32(RPI4_LOCAL_CONTROL_PRESCALER, 0x80000000);

	/* Early GPU firmware revisions need a little break here. */
	ldelay(100000);

	/* Initialize the console to provide early debug support. */
	rpi3_console_init();

	/*
	 * Copy the OP-TEE OS image to the entry address.
	 * Unfortunately, this is still a quick and dirty hack that bypasses any FIP.
	 * Restrictions:
	 * - The OP-TEE OS image is mandatory and must be attached to the bl31 (directly, not as FIP).
	 * - It must not be larger than 500 KiB.
	 */
	const size_t KiB = 1024;
	const size_t opteeOsSize = 500 * KiB;
	const void *const opteeOsLoadAdr = (const void*)(128 * KiB);
	void *const opteeOsEntry = (void*)0x10100000;
	VERBOSE("rpi4: copy optee-os image (%lu bytes) from %p to %p\n", opteeOsSize, opteeOsLoadAdr, opteeOsEntry);
	memcpy(opteeOsEntry, opteeOsLoadAdr, opteeOsSize);

	/* Initialize the OP-TEE OS image info. */
	bl32_image_ep_info.pc = (uintptr_t)opteeOsEntry;
	bl32_image_ep_info.args.arg2 = rpi4_get_dtb_address();
	SET_SECURITY_STATE(bl32_image_ep_info.h.attr, SECURE);
	VERBOSE("rpi4: optee-os entry: %p\n", (void*)bl32_image_ep_info.pc);
	VERBOSE("rpi4: dtb: %p\n", (void*)bl32_image_ep_info.args.arg2);

	/* Initialize the Linux kernel image info. */
	bl33_image_ep_info.pc = plat_get_ns_image_entrypoint();
	bl33_image_ep_info.spsr = rpi3_get_spsr_for_bl33_entry();
	SET_SECURITY_STATE(bl33_image_ep_info.h.attr, NON_SECURE);
	VERBOSE("rpi4: kernel entry: %p\n", (void*)bl33_image_ep_info.pc);

#if RPI3_DIRECT_LINUX_BOOT
# if RPI3_BL33_IN_AARCH32
	/*
	 * According to the file ``Documentation/arm/Booting`` of the Linux
	 * kernel tree, Linux expects:
	 * r0 = 0
	 * r1 = machine type number, optional in DT-only platforms (~0 if so)
	 * r2 = Physical address of the device tree blob
	 */
	VERBOSE("rpi4: Preparing to boot 32-bit Linux kernel\n");
	bl33_image_ep_info.args.arg0 = 0U;
	bl33_image_ep_info.args.arg1 = ~0U;
	bl33_image_ep_info.args.arg2 = rpi4_get_dtb_address();
# else
	/*
	 * According to the file ``Documentation/arm64/booting.txt`` of the
	 * Linux kernel tree, Linux expects the physical address of the device
	 * tree blob (DTB) in x0, while x1-x3 are reserved for future use and
	 * must be 0.
	 */
	VERBOSE("rpi4: Preparing to boot 64-bit Linux kernel\n");
	bl33_image_ep_info.args.arg0 = rpi4_get_dtb_address();
	bl33_image_ep_info.args.arg1 = 0ULL;
	bl33_image_ep_info.args.arg2 = 0ULL;
	bl33_image_ep_info.args.arg3 = 0ULL;
# endif /* RPI3_BL33_IN_AARCH32 */
#endif /* RPI3_DIRECT_LINUX_BOOT */
}

void bl31_plat_arch_setup(void)
{
	/*
	 * Is the dtb_ptr32 pointer valid? If yes, map the DTB region.
	 * We map the 2MB region the DTB start address lives in, plus
	 * the next 2MB, to have enough room for expansion.
	 */
	if (stub_magic == 0) {
		unsigned long long dtb_region = dtb_ptr32;

		dtb_region &= ~0x1fffff;	/* Align to 2 MB. */
		mmap_add_region(dtb_region, dtb_region, 4U << 20,
				MT_MEMORY | MT_RW | MT_NS);
	}
	/*
	 * Add the first page of memory, which holds the stub magic,
	 * the kernel and the DT address.
	 * This also holds the secondary CPU's entrypoints and mailboxes.
	 */
	mmap_add_region(0, 0, 4096, MT_NON_CACHEABLE | MT_RW | MT_SECURE);

	rpi3_setup_page_tables(BL31_BASE, BL31_END - BL31_BASE,
			       BL_CODE_BASE, BL_CODE_END,
			       BL_RO_DATA_BASE, BL_RO_DATA_END
#if USE_COHERENT_MEM
			       , BL_COHERENT_RAM_BASE, BL_COHERENT_RAM_END
#endif
			      );

	enable_mmu_el3(0);
}

/*
 * Remove the FDT /memreserve/ entry that covers the region at the very
 * beginning of memory (if that exists). This is where the secondaries
 * originally spin, but we pull them out there.
 * Having overlapping /reserved-memory and /memreserve/ regions confuses
 * the Linux kernel, so we need to get rid of this one.
 */
static void remove_spintable_memreserve(void *dtb)
{
	uint64_t addr, size;
	int regions = fdt_num_mem_rsv(dtb);
	int i;

	for (i = 0; i < regions; i++) {
		if (fdt_get_mem_rsv(dtb, i, &addr, &size) != 0) {
			return;
		}
		if (size == 0U) {
			return;
		}
		/* We only look for the region at the beginning of DRAM. */
		if (addr != 0U) {
			continue;
		}
		/*
		 * Currently the region in the existing DTs is exactly 4K
		 * in size. Should this value ever change, there is probably
		 * a reason for that, so inform the user about this.
		 */
		if (size == 4096U) {
			fdt_del_mem_rsv(dtb, i);
			return;
		}
		WARN("Keeping unknown /memreserve/ region at 0, size: %" PRId64 "\n",
		     size);
	}
}

static void rpi4_prepare_dtb(void)
{
	void *dtb = (void *)rpi4_get_dtb_address();
	uint32_t gic_int_prop[3];
	int ret, offs;

	/* Return if no device tree is detected */
	if (fdt_check_header(dtb) != 0)
		return;

	ret = fdt_open_into(dtb, dtb, 0x100000);
	if (ret < 0) {
		ERROR("Invalid Device Tree at %p: error %d\n", dtb, ret);
		return;
	}

	if (dt_add_psci_node(dtb)) {
		ERROR("Failed to add PSCI Device Tree node\n");
		return;
	}

	if (dt_add_psci_cpu_enable_methods(dtb)) {
		ERROR("Failed to add PSCI cpu enable methods in Device Tree\n");
		return;
	}

	/*
	 * Remove the original reserved region (used for the spintable), and
	 * replace it with a region describing the whole of Trusted Firmware.
	 */
	remove_spintable_memreserve(dtb);
	if (fdt_add_reserved_memory(dtb, "atf@0", 0, 0x80000))
		WARN("Failed to add reserved memory nodes to DT.\n");

	offs = fdt_node_offset_by_compatible(dtb, 0, "arm,gic-400");
	gic_int_prop[0] = cpu_to_fdt32(1);		// PPI
	gic_int_prop[1] = cpu_to_fdt32(9);		// PPI #9
	gic_int_prop[2] = cpu_to_fdt32(0x0f04);		// all cores, level high
	fdt_setprop(dtb, offs, "interrupts", gic_int_prop, 12);

	offs = fdt_path_offset(dtb, "/chosen");
	fdt_setprop_string(dtb, offs, "stdout-path", "serial0");

	ret = fdt_pack(dtb);
	if (ret < 0)
		ERROR("Failed to pack Device Tree at %p: error %d\n", dtb, ret);

	clean_dcache_range((uintptr_t)dtb, fdt_blob_size(dtb));
	INFO("Changed device tree to advertise PSCI.\n");
}

void bl31_platform_setup(void)
{
	rpi4_prepare_dtb();

	/* Configure the interrupt controller */
	gicv2_driver_init(&rpi4_gic_data);
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_cpuif_enable();
}
