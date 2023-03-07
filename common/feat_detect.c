/*
 * Copyright (c) 2022, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <arch_features.h>
#include <common/debug.h>
#include <common/feat_detect.h>

static bool tainted;

/*******************************************************************************
 * This section lists the wrapper modules for each feature to evaluate the
 * feature states (FEAT_STATE_ALWAYS and FEAT_STATE_CHECK) and perform
 * necessary action as below:
 *
 * It verifies whether the FEAT_XXX (eg: FEAT_SB) is supported by the PE or not.
 * Without this check an exception would occur during context save/restore
 * routines, if the feature is enabled but not supported by PE.
 ******************************************************************************/

#define feat_detect_panic(a, b)		((a) ? (void)0 : feature_panic(b))

/*******************************************************************************
 * Function : feature_panic
 * Customised panic function with error logging mechanism to list the feature
 * not supported by the PE.
 ******************************************************************************/
static inline void feature_panic(char *feat_name)
{
	ERROR("FEAT_%s not supported by the PE\n", feat_name);
	panic();
}

/*******************************************************************************
 * Function : check_feature
 * Check for a valid combination of build time flags (ENABLE_FEAT_xxx) and
 * feature availability on the hardware. <min> is the smallest feature
 * ID field value that is required for that feature.
 * Triggers a panic later if a feature is forcefully enabled, but not
 * available on the PE. Also will panic if the hardware feature ID field
 * is larger than the maximum known and supported number, specified by <max>.
 *
 * We force inlining here to let the compiler optimise away the whole check
 * if the feature is disabled at build time (FEAT_STATE_DISABLED).
 ******************************************************************************/
static inline void __attribute((__always_inline__))
check_feature(int state, unsigned long field, const char *feat_name,
	      unsigned int min, unsigned int max)
{
	if (state == FEAT_STATE_ALWAYS && field < min) {
		ERROR("FEAT_%s not supported by the PE\n", feat_name);
		tainted = true;
	}
	if (state >= FEAT_STATE_ALWAYS && field > max) {
		ERROR("FEAT_%s is version %ld, but is only known up to version %d\n",
		      feat_name, field, max);
		tainted = true;
	}
}

/******************************************
 * Feature : FEAT_SB (Speculation Barrier)
 *****************************************/
static void read_feat_sb(void)
{
#if (ENABLE_FEAT_SB == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_0_feat_sb_present(), "SB");
#endif
}

/******************************************************
 * Feature : FEAT_CSV2_2 (Cache Speculation Variant 2)
 *****************************************************/
static void read_feat_csv2_2(void)
{
#if (ENABLE_FEAT_CSV2_2 == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_0_feat_csv2_2_present(), "CSV2_2");
#endif
}

/***********************************************
 * Feature : FEAT_PAN (Privileged Access Never)
 **********************************************/
static void read_feat_pan(void)
{
#if (ENABLE_FEAT_PAN == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_1_pan_present(), "PAN");
#endif
}

/******************************************************
 * Feature : FEAT_VHE (Virtualization Host Extensions)
 *****************************************************/
static void read_feat_vhe(void)
{
#if (ENABLE_FEAT_VHE == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_1_vhe_present(), "VHE");
#endif
}

/*******************************************************************************
 * Feature : FEAT_RAS (Reliability, Availability, and Serviceability Extension)
 ******************************************************************************/
static void read_feat_ras(void)
{
#if (RAS_EXTENSION == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_2_feat_ras_present(), "RAS");
#endif
}

/************************************************
 * Feature : FEAT_PAUTH (Pointer Authentication)
 ***********************************************/
static void read_feat_pauth(void)
{
#if (ENABLE_PAUTH == FEAT_STATE_ALWAYS) || (CTX_INCLUDE_PAUTH_REGS == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_3_pauth_present(), "PAUTH");
#endif
}

/************************************************************
 * Feature : FEAT_DIT (Data Independent Timing Instructions)
 ***********************************************************/
static void read_feat_dit(void)
{
#if (ENABLE_FEAT_DIT == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_4_feat_dit_present(), "DIT");
#endif
}

/****************************************************************************
 * Feature : FEAT_MPAM (Memory Partitioning and Monitoring (MPAM) Extension)
 ***************************************************************************/
static void read_feat_mpam(void)
{
#if (ENABLE_MPAM_FOR_LOWER_ELS == FEAT_STATE_ALWAYS)
	feat_detect_panic(get_mpam_version() != 0U, "MPAM");
#endif
}

/**************************************************************
 * Feature : FEAT_NV2 (Enhanced Nested Virtualization Support)
 *************************************************************/
static void read_feat_nv2(void)
{
#if (CTX_INCLUDE_NEVE_REGS == FEAT_STATE_ALWAYS)
	unsigned int nv = get_armv8_4_feat_nv_support();

	feat_detect_panic((nv == ID_AA64MMFR2_EL1_NV2_SUPPORTED), "NV2");
#endif
}

/***********************************
 * Feature : FEAT_SEL2 (Secure EL2)
 **********************************/
static void read_feat_sel2(void)
{
#if (ENABLE_FEAT_SEL2 == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_4_sel2_present(), "SEL2");
#endif
}

/************************************************
 * Feature : FEAT_MTE (Memory Tagging Extension)
 ***********************************************/
static void read_feat_mte(void)
{
#if (CTX_INCLUDE_MTE_REGS == FEAT_STATE_ALWAYS)
	unsigned int mte = get_armv8_5_mte_support();

	feat_detect_panic((mte != MTE_UNIMPLEMENTED), "MTE");
#endif
}

/***********************************************
 * Feature : FEAT_RNG (Random Number Generator)
 **********************************************/
static void read_feat_rng(void)
{
#if (ENABLE_FEAT_RNG == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_5_rng_present(), "RNG");
#endif
}

/****************************************************
 * Feature : FEAT_BTI (Branch Target Identification)
 ***************************************************/
static void read_feat_bti(void)
{
#if (ENABLE_BTI == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_5_bti_present(), "BTI");
#endif
}

/***********************************************
 * Feature : FEAT_AMUv1p1 (AMU Extensions v1.1)
 **********************************************/
static void read_feat_amuv1p1(void)
{
#if (ENABLE_FEAT_AMUv1p1 == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_6_feat_amuv1p1_present(), "AMUv1p1");
#endif
}

/*******************************************************
 * Feature : FEAT_ECV (Enhanced Counter Virtualization)
 ******************************************************/
static void read_feat_ecv(void)
{
#if (ENABLE_FEAT_ECV == FEAT_STATE_ALWAYS)
	unsigned int ecv = get_armv8_6_ecv_support();

	feat_detect_panic(((ecv == ID_AA64MMFR0_EL1_ECV_SUPPORTED) ||
			(ecv == ID_AA64MMFR0_EL1_ECV_SELF_SYNCH)), "ECV");
#endif
}

/***********************************************************
 * Feature : FEAT_TWED (Delayed Trapping of WFE Instruction)
 **********************************************************/
static void read_feat_twed(void)
{
#if (ENABLE_FEAT_TWED == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_armv8_6_twed_present(), "TWED");
#endif
}

/**************************************************
 * Feature : FEAT_RME (Realm Management Extension)
 *************************************************/
static void read_feat_rme(void)
{
#if (ENABLE_RME == FEAT_STATE_ALWAYS)
	feat_detect_panic((get_armv9_2_feat_rme_support() !=
			ID_AA64PFR0_FEAT_RME_NOT_SUPPORTED), "RME");
#endif
}

/******************************************************************
 * Feature : FEAT_RNG_TRAP (Trapping support for RNDR/RNDRRS)
 *****************************************************************/
static void read_feat_rng_trap(void)
{
#if (ENABLE_FEAT_RNG_TRAP == FEAT_STATE_ALWAYS)
	feat_detect_panic(is_feat_rng_trap_present(), "RNG_TRAP");
#endif
}

/***********************************************************************************
 * TF-A supports many Arm architectural features starting from arch version
 * (8.0 till 8.7+). These features are mostly enabled through build flags. This
 * mechanism helps in validating these build flags in the early boot phase
 * either in BL1 or BL31 depending on the platform and assists in identifying
 * and notifying the features which are enabled but not supported by the PE.
 *
 * It reads all the enabled features ID-registers and ensures the features
 * are supported by the PE.
 * In case if they aren't it stops booting at an early phase and logs the error
 * messages, notifying the platforms about the features that are not supported.
 *
 * Further the procedure is implemented with a tri-state approach for each feature:
 * ENABLE_FEAT_xxx = 0 : The feature is disabled statically at compile time
 * ENABLE_FEAT_xxx = 1 : The feature is enabled and must be present in hardware.
 *                       There will be panic if feature is not present at cold boot.
 * ENABLE_FEAT_xxx = 2 : The feature is enabled but dynamically enabled at runtime
 *                       depending on hardware capability.
 *
 * For better readability, state values are defined with macros, namely:
 * { FEAT_STATE_DISABLED, FEAT_STATE_ALWAYS, FEAT_STATE_CHECK }, taking values
 * { 0, 1, 2 }, respectively, as their naming.
 **********************************************************************************/
void detect_arch_features(void)
{
	tainted = false;

	/* v8.0 features */
	read_feat_sb();
	read_feat_csv2_2();

	/* v8.1 features */
	read_feat_pan();
	read_feat_vhe();

	/* v8.2 features */
	read_feat_ras();

	/* v8.3 features */
	read_feat_pauth();

	/* v8.4 features */
	read_feat_dit();
	check_feature(ENABLE_FEAT_AMUv1, read_feat_amu_id_field(),
		      "AMUv1", 1, 2);
	read_feat_mpam();
	read_feat_nv2();
	read_feat_sel2();
	check_feature(ENABLE_TRF_FOR_NS, read_feat_trf_id_field(),
		      "TRF", 1, 1);

	/* v8.5 features */
	read_feat_mte();
	read_feat_rng();
	read_feat_bti();
	read_feat_rng_trap();

	/* v8.6 features */
	read_feat_amuv1p1();
	check_feature(ENABLE_FEAT_FGT, read_feat_fgt_id_field(), "FGT", 1, 1);
	read_feat_ecv();
	read_feat_twed();

	/* v8.7 features */
	check_feature(ENABLE_FEAT_HCX, read_feat_hcx_id_field(), "HCX", 1, 1);

	/* v9.0 features */
	check_feature(ENABLE_BRBE_FOR_NS, read_feat_brbe_id_field(),
		      "BRBE", 1, 2);
	check_feature(ENABLE_TRBE_FOR_NS, read_feat_trbe_id_field(),
		      "TRBE", 1, 1);

	/* v9.2 features */
	read_feat_rme();

	if (tainted) {
		panic();
	}
}
