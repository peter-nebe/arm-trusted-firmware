The following log outputs show the initialization on the Raspberry Pi 4. You can see how control passes from the ARM Trusted Firmware to the OP-TEE OS and back again.
```
VERBOSE: rpi4: copy optee-os image (512000 bytes) from 0x20000 to 0x10100000
VERBOSE: rpi4: optee-os entry: 0x10100000
VERBOSE: rpi4: dtb: 0x2eff2f00
VERBOSE: rpi4: kernel entry: 0
VERBOSE: rpi4: Preparing to boot 64-bit Linux kernel
VERBOSE: Trusted SRAM seen by this BL image: 0x1000 - 0x1a000
VERBOSE: Code region: 0x1000 - 0xd000
VERBOSE: Read-only data region: 0xd000 - 0xf000
VERBOSE: Coherent region: 0x19000 - 0x1a000
mmap:
 VA:0x0  PA:0x0  size:0x1000  attr:0x9  granularity:0x40000000
 VA:0x1000  PA:0x1000  size:0xc000  attr:0x2  granularity:0x40000000
 VA:0xd000  PA:0xd000  size:0x2000  attr:0x42  granularity:0x40000000
 VA:0x19000  PA:0x19000  size:0x1000  attr:0x8  granularity:0x40000000
 VA:0x1000  PA:0x1000  size:0x19000  attr:0xa  granularity:0x40000000
 VA:0x2ee00000  PA:0x2ee00000  size:0x400000  attr:0x1a  granularity:0x40000000
 VA:0xfc000000  PA:0xfc000000  size:0x4000000  attr:0x8  granularity:0x40000000

VERBOSE: Translation tables state:
VERBOSE:   Xlat regime:     EL3
VERBOSE:   Max allowed PA:  0xffffffff
VERBOSE:   Max allowed VA:  0xffffffff
VERBOSE:   Max mapped PA:   0xffffffff
VERBOSE:   Max mapped VA:   0xffffffff
VERBOSE:   Initial lookup level: 1
VERBOSE:   Entries @initial lookup level: 4
VERBOSE:   Used 3 sub-tables out of 4 (spare: 1)
  [LV1] VA:0x0 size:0x40000000
    [LV2] VA:0x0 size:0x200000
      [LV3] VA:0x0 PA:0x0 size:0x1000 NC-RW-XN-S
      [LV3] VA:0x1000 PA:0x1000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x2000 PA:0x2000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x3000 PA:0x3000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x4000 PA:0x4000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x5000 PA:0x5000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x6000 PA:0x6000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x7000 PA:0x7000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x8000 PA:0x8000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0x9000 PA:0x9000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0xa000 PA:0xa000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0xb000 PA:0xb000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0xc000 PA:0xc000 size:0x1000 MEM-RO-EXEC-S
      [LV3] VA:0xd000 PA:0xd000 size:0x1000 MEM-RO-XN-S
      [LV3] VA:0xe000 PA:0xe000 size:0x1000 MEM-RO-XN-S
      [LV3] VA:0xf000 PA:0xf000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x10000 PA:0x10000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x11000 PA:0x11000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x12000 PA:0x12000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x13000 PA:0x13000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x14000 PA:0x14000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x15000 PA:0x15000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x16000 PA:0x16000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x17000 PA:0x17000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x18000 PA:0x18000 size:0x1000 MEM-RW-XN-S
      [LV3] VA:0x19000 PA:0x19000 size:0x1000 DEV-RW-XN-S
      [LV3] VA:0x1a000 size:0x1000
      [LV3] (485 invalid descriptors omitted)
    [LV2] VA:0x200000 size:0x200000
    [LV2] (373 invalid descriptors omitted)
    [LV2] VA:0x2ee00000 PA:0x2ee00000 size:0x200000 MEM-RW-XN-NS
    [LV2] VA:0x2f000000 PA:0x2f000000 size:0x200000 MEM-RW-XN-NS
    [LV2] VA:0x2f200000 size:0x200000
    [LV2] (134 invalid descriptors omitted)
  [LV1] VA:0x40000000 size:0x40000000
  [LV1] (1 invalid descriptors omitted)
  [LV1] VA:0xc0000000 size:0x40000000
    [LV2] VA:0xc0000000 size:0x200000
    [LV2] (479 invalid descriptors omitted)
    [LV2] VA:0xfc000000 PA:0xfc000000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfc200000 PA:0xfc200000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfc400000 PA:0xfc400000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfc600000 PA:0xfc600000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfc800000 PA:0xfc800000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfca00000 PA:0xfca00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfcc00000 PA:0xfcc00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfce00000 PA:0xfce00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfd000000 PA:0xfd000000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfd200000 PA:0xfd200000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfd400000 PA:0xfd400000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfd600000 PA:0xfd600000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfd800000 PA:0xfd800000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfda00000 PA:0xfda00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfdc00000 PA:0xfdc00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfde00000 PA:0xfde00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfe000000 PA:0xfe000000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfe200000 PA:0xfe200000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfe400000 PA:0xfe400000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfe600000 PA:0xfe600000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfe800000 PA:0xfe800000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfea00000 PA:0xfea00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfec00000 PA:0xfec00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xfee00000 PA:0xfee00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xff000000 PA:0xff000000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xff200000 PA:0xff200000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xff400000 PA:0xff400000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xff600000 PA:0xff600000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xff800000 PA:0xff800000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xffa00000 PA:0xffa00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xffc00000 PA:0xffc00000 size:0x200000 DEV-RW-XN-S
    [LV2] VA:0xffe00000 PA:0xffe00000 size:0x200000 DEV-RW-XN-S
NOTICE:  BL31: v2.8(debug):3d2da6f5d-dirty
NOTICE:  BL31: Built : 18:00:25, Mar  6 2023
INFO:    Changed device tree to advertise PSCI.
INFO:    ARM GICv2 driver initialized
INFO:    BL31: Initializing runtime services
INFO:    BL31: cortex_a72: CPU workaround for 859971 was applied
WARNING: BL31: cortex_a72: CPU workaround for 1319367 was missing!
INFO:    BL31: cortex_a72: CPU workaround for cve_2017_5715 was applied
INFO:    BL31: cortex_a72: CPU workaround for cve_2018_3639 was applied
INFO:    BL31: cortex_a72: CPU workaround for cve_2022_23960 was applied
INFO:    BL31: Initializing BL32
D/TC:0   console_init:48 done
D/TC:0   get_aslr_seed:1566 Cannot find /secure-chosen
D/TC:0   plat_get_aslr_seed:114 Warning: no ASLR seed
D/TC:0   add_phys_mem:635 ROUNDDOWN(0xfe215040, CORE_MMU_PGDIR_SIZE) type IO_NSEC 0xfe200000 size 0x00200000
D/TC:0   add_phys_mem:635 TEE_SHMEM_START type NSEC_SHM 0x08000000 size 0x00400000
D/TC:0   add_phys_mem:635 TA_RAM_START type TA_RAM 0x10800000 size 0x00800000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RW_PA type TEE_RAM_RW 0x10166000 size 0x0069a000
D/TC:0   add_phys_mem:635 VCORE_UNPG_RX_PA type TEE_RAM_RX 0x10100000 size 0x00066000
D/TC:0   add_va_space:675 type RES_VASPACE size 0x00a00000
D/TC:0   add_va_space:675 type SHM_VASPACE size 0x02000000
D/TC:0   dump_mmap_table:800 type SHM_VASPACE  va 0x00000000..0x01ffffff pa 0x00000000..0x01ffffff size 0x02000000 (pgdir)
D/TC:0   dump_mmap_table:800 type RES_VASPACE  va 0x00000000..0x009fffff pa 0x00000000..0x009fffff size 0x00a00000 (pgdir)
D/TC:0   dump_mmap_table:800 type NSEC_SHM     va 0x08000000..0x083fffff pa 0x08000000..0x083fffff size 0x00400000 (pgdir)
D/TC:0   dump_mmap_table:800 type TEE_RAM_RX   va 0x10100000..0x10165fff pa 0x10100000..0x10165fff size 0x00066000 (smallpg)
D/TC:0   dump_mmap_table:800 type TEE_RAM_RW   va 0x10166000..0x107fffff pa 0x10166000..0x107fffff size 0x0069a000 (smallpg)
D/TC:0   dump_mmap_table:800 type TA_RAM       va 0x10800000..0x10ffffff pa 0x10800000..0x10ffffff size 0x00800000 (pgdir)
D/TC:0   dump_mmap_table:800 type IO_NSEC      va 0xfe200000..0xfe3fffff pa 0xfe200000..0xfe3fffff size 0x00200000 (pgdir)
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 1 / 8
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 2 / 8
D/TC:0   core_mmu_xlat_table_alloc:526 xlat tables used 3 / 8
F/TC:0   checkpoint:54 .
D/TC:0   console_init:48 done
F/TC:0   checkpoint1:58 checkpoint 1
F/TC:0 0 init_primary:1369 .
F/TC:0 0 init_runtime:613 .
F/TC:0 0 gen_malloc_add_pool:876 .
F/TC:0 0 gen_malloc_add_pool:878 .
F/TC:0 0 gen_malloc_add_pool:881 .
F/TC:0 0 gen_malloc_add_pool:883 .
F/TC:0 0 init_runtime:615 .
I/TC: 
F/TC:0 0 init_primary:1371 .
D/TC:0 0 select_vector_wa_spectre_v2:624 SMCCC_ARCH_WORKAROUND_1 (0x80008000) available
D/TC:0 0 select_vector_wa_spectre_v2:626 SMC Workaround for CVE-2017-5715 used
F/TC:0 0 checkpoint2:62 checkpoint 2
I/TC: Non-secure external DT found
I/TC: OP-TEE version: 706768be-dev (gcc version 10.3.1 20210621 (GNU Toolchain for the A-profile Architecture 10.3-2021.07 (arm-10.29))) #386 Mon Mar  6 16:36:55 UTC 2023 aarch64
I/TC: WARNING: This OP-TEE configuration might be insecure!
I/TC: WARNING: Please check https://optee.readthedocs.io/en/latest/architecture/porting_guidelines.html
I/TC: Primary CPU initializing
D/TC:0 0 boot_init_primary_late:1422 Executing at offset 0 with virtual load address 0x10100000
D/TC:0 0 call_initcalls:40 level 1 register_time_source()
D/TC:0 0 call_initcalls:40 level 1 teecore_init_pub_ram()
D/TC:0 0 call_initcalls:40 level 2 probe_dt_drivers_early()
D/TC:0 0 call_initcalls:40 level 3 check_ta_store()
D/TC:0 0 check_ta_store:417 TA store: "Secure Storage TA"
D/TC:0 0 check_ta_store:417 TA store: "REE"
D/TC:0 0 call_initcalls:40 level 3 verify_pseudo_tas_conformance()
D/TC:0 0 call_initcalls:40 level 3 tee_cryp_init()
D/TC:0 0 call_initcalls:40 level 4 tee_fs_init_key_manager()
D/TC:0 0 call_initcalls:40 level 5 probe_dt_drivers()
D/TC:0 0 call_initcalls:40 level 6 mobj_init()
D/TC:0 0 call_initcalls:40 level 6 default_mobj_init()
D/TC:0 0 call_initcalls:40 level 6 ftmn_boot_tests()
D/TC:0 0 ftmn_boot_tests:198 Calling simple_call()
D/TC:0 0 ftmn_boot_tests:198 Return from simple_call()
D/TC:0 0 ftmn_boot_tests:199 Calling two_level_call()
D/TC:0 0 ftmn_boot_tests:199 Return from two_level_call()
D/TC:0 0 ftmn_boot_tests:200 Calling chained_calls()
D/TC:0 0 ftmn_boot_tests:200 Return from chained_calls()
D/TC:0 0 ftmn_boot_tests:202 *************************************************
D/TC:0 0 ftmn_boot_tests:203 **************  Tests complete  *****************
D/TC:0 0 ftmn_boot_tests:204 *************************************************
D/TC:0 0 call_initcalls:40 level 7 release_probe_lists()
D/TC:0 0 call_finalcalls:59 level 1 release_external_dt()
I/TC: Primary CPU switching to normal world boot
F/TC:0 0 checkpoint:54 .
F/TC:0   checkpoint3:66 checkpoint 3
ASSERT: bl31/bl31_main.c:246
BACKTRACE: START: assert
0: EL3: 0x2a08
1: EL3: 0x1210
2: EL3: 0x316c
3: EL3: 0x2e04
4: EL3: 0x1128
BACKTRACE: END: assert
```
A Linux kernel has not yet been loaded on purpose. Therefore, the assertion at the end fails.

The solution in this branch is temporary and has the following **limitations**:
- It only works on the Raspberry Pi 4.
- The OP-TEE OS image is mandatory and must not be larger than 500 KiB.

This solution is published **without any guarantee**. Use it at your own risk.

More information at https://github.com/peter-nebe/optee_os
