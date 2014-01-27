#!/bin/sh

. ./test_vars
. ./test_output_strings

# When the ex_output program is run with the STDOUT_DUMP mode, it will
# run with the normal output mode, then dump each output mode using
# srunner_print() in this order:
# CK_SILENT CK_MINIMAL CK_NORMAL CK_VERBOSE CK_ENV CK_SUBUNIT
# note though that CK_SUBUNIT does not output anything, as it is
# not fully considered an 'output mode'.
exp_silent_dump="$exp_minimal_result
$exp_normal_result
$exp_verbose_result"
exp_minimal_dump="$exp_minimal
$exp_minimal_result
$exp_normal_result
$exp_verbose_result
$exp_minimal_result"
exp_normal_dump="$exp_normal
$exp_minimal_result
$exp_normal_result
$exp_verbose_result
$exp_normal_result"
exp_verbose_dump="$exp_verbose
$exp_minimal_result
$exp_normal_result
$exp_verbose_result
$exp_verbose_result"
exp_subunit_dump="$exp_subunit

$exp_minimal_result
$exp_normal_result
$exp_verbose_result
$exp_normal_result"

act_silent=`./ex_output${EXEEXT} CK_SILENT STDOUT NORMAL  | tr -d "\r"`
act_silent_env=`CK_VERBOSITY=silent ./ex_output${EXEEXT} CK_ENV STDOUT NORMAL  | tr -d "\r"`
act_silent_dump_env=`CK_VERBOSITY=silent ./ex_output${EXEEXT} CK_ENV STDOUT_DUMP NORMAL  | tr -d "\r"`
act_minimal=`./ex_output${EXEEXT} CK_MINIMAL STDOUT NORMAL | tr -d "\r"`
act_minimal_env=`CK_VERBOSITY=minimal ./ex_output${EXEEXT} CK_ENV STDOUT NORMAL | tr -d "\r"`
act_minimal_dump_env=`CK_VERBOSITY=minimal ./ex_output${EXEEXT} CK_ENV STDOUT_DUMP NORMAL | tr -d "\r"`
act_normal=`./ex_output${EXEEXT} CK_NORMAL  STDOUT NORMAL | tr -d "\r"`
act_normal_env=`CK_VERBOSITY=normal CK_VERBOSITY='' ./ex_output${EXEEXT} CK_ENV  STDOUT NORMAL | tr -d "\r"`
act_normal_dump_env=`CK_VERBOSITY=normal CK_VERBOSITY='' ./ex_output${EXEEXT} CK_ENV  STDOUT_DUMP NORMAL | tr -d "\r"`
act_normal_env_blank=`./ex_output${EXEEXT} CK_ENV  STDOUT NORMAL | tr -d "\r"`
act_normal_env_invalid=`CK_VERBOSITY='BLARGS' ./ex_output${EXEEXT} CK_ENV  STDOUT NORMAL | tr -d "\r"`
act_verbose=`./ex_output${EXEEXT} CK_VERBOSE STDOUT NORMAL | tr -d "\r"`
act_verbose_env=`CK_VERBOSITY=verbose ./ex_output${EXEEXT} CK_ENV STDOUT NORMAL | tr -d "\r"`
act_verbose_dump_env=`CK_VERBOSITY=verbose ./ex_output${EXEEXT} CK_ENV STDOUT_DUMP NORMAL | tr -d "\r"`
if test 1 -eq $ENABLE_SUBUNIT; then
act_subunit=`./ex_output${EXEEXT} CK_SUBUNIT STDOUT NORMAL | tr -d "\r"`
act_subunit_dump_env=`CK_VERBOSITY=subunit ./ex_output${EXEEXT} CK_SUBUNIT STDOUT_DUMP NORMAL | tr -d "\r"`
fi

test_output ( ) {
    if [ "x${1}" != "x${2}" ]; then
	echo "Problem with ex_output${EXEEXT} ${3}";
	echo "Expected:";
	echo "${1}";
	echo "Got:";
	echo "${2}";
	exit 1;
    fi

}

test_output "$exp_silent"  "$act_silent"             "CK_SILENT STDOUT NORMAL";
test_output "$exp_silent"  "$act_silent_env"         "CK_ENV STDOUT NORMAL";
test_output "$exp_minimal" "$act_minimal"            "CK_MINIMAL STDOUT NORMAL";
test_output "$exp_minimal" "$act_minimal_env"        "CK_ENV STDOUT NORMAL";
test_output "$exp_normal"  "$act_normal"             "CK_NORMAL STDOUT NORMAL";
test_output "$exp_normal"  "$act_normal_env"         "CK_ENV STDOUT NORMAL";
test_output "$exp_normal"  "$act_normal_env_blank"   "CK_ENV STDOUT NORMAL";
test_output "$exp_normal"  "$act_normal_env_invalid" "CK_ENV STDOUT NORMAL";
test_output "$exp_verbose" "$act_verbose"            "CK_VERBOSE STDOUT NORMAL";
test_output "$exp_verbose" "$act_verbose_env"        "CK_ENV STDOUT NORMAL";

test_output "$exp_silent_dump"  "$act_silent_dump_env"  "CK_ENV STDOUT_DUMP NORMAL (for silent)"
test_output "$exp_minimal_dump" "$act_minimal_dump_env" "CK_ENV STDOUT_DUMP NORMAL (for minimal)"
test_output "$exp_normal_dump"  "$act_normal_dump_env"  "CK_ENV STDOUT_DUMP NORMAL (for normal)"
test_output "$exp_verbose_dump" "$act_verbose_dump_env" "CK_ENV STDOUT_DUMP NORMAL (for verbose)"

if test 1 -eq $ENABLE_SUBUNIT; then
    test_output "$exp_subunit"      "$act_subunit"          "CK_SUBUNIT STDOUT NORMAL";
    test_output "$exp_subunit_dump" "$act_subunit_dump_env" "CK_ENV STDOUT_DUMP NORMAL (for subunit)"
fi

exit 0
