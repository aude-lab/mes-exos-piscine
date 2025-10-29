#!/bin/bash

###########################################
#        MY MINIMAKE TESTSUITE            #
###########################################


#----------------- COLOR -----------------#
RED="\e[0;31m"
BRED="\e[1;31m"
GRN="\e[0;32m"
BGRN="\e[1;32m"
YEL="\e[0;33m"
BYEL="\e[1;33m"
BLU="\e[0;34m"
BBLU="\e[1;34m"
PUR="\e[0;35m"
BPUR="\e[1;35m"
CYA="\e[0;36m"
BCYA="\e[1;36m"
WHI="\e[0;37m"
BWHI="\e[1;37m"
GRE="\e[2;37m"

#---------------- GLOBAL VAR --------------#

TEST_DIR="test_makefiles"
TOTAL_TESTS=0
PASSED_TESTS=0




tit_wrap()
{
	echo -e "$@$WHI"
}

print_result()
{
	if [ $1 -eq 0 ]; then
		tit_wrap " $GRN PASS"
		PASSED_TESTS=$((PASSED_TESTS + 1))

	else
		tit_wrap " $RED FAIL"
	fi

	TOTAL_TESTS=$((TOTAL_TESTS + 1))
	echo
}


#----------------- TEST FUNCTIONS -----------------#
run_test() {
    local category="$1"
    local test_file="$2"
    local args="$3"
    local expected_output="$4"
    local description="$5"

    tit_wrap "$BLUTesting: $description"
    tit_wrap "  File: $GRE$test_file"
    tit_wrap "  Args: $GRE$args"

    ./minimake $args -f "tests/$TEST_DIR/$category/$test_file" > /tmp/actual_output 2>&1

    if echo "$expected_output" | diff -q /tmp/actual_output - > /dev/null 2>&1; then
        print_result 0

     else
        tit_wrap "  Expected:$GRN"
        echo "$expected_output"
        tit_wrap "$WHI  Got:$RED"
        cat /tmp/actual_output
        tit_wrap "$WHI"
        print_result 1
    fi
    
    rm -f /tmp/actual_output
}


#------DIFFERENTS TEST GROUPS -----------------#




run_basic_tests()
{
	tit_wrap "$PUR=== BASIC TESTS ==="
	run_test "basic" "simple.mk" "all" "Hello World" "Simple rule execution"
    	 run_test "basic" "help.mk" "-h" "Use: minimake [options] [targets]
Options:
-f file    Using file as makefile
-h         Showing this help message
-p         Pretty-printing the makefile" "Help option"

	run_test "basic" "pretty_print.mk" "-p" "# variables
'VAR1' = 'value1'
'VAR2' = 'value2'
# rules
(rule1): [dep1]
    '@echo \"Building rule1\"'
(dep1):
    '@echo \"Dependency 1\"'" "Pretty print option"
}





#--------- MAIN CALL -----------------------

main() 
{
    tit_wrap "$BGRN=== MINIMAKE TEST SUITE ==="
    echo

    if [ ! -f "./minimake" ]; then
        tit_wrap "$REDError: minimake not found. Run 'make' first."
        exit 1
    fi

    run_basic_tests

    tit_wrap "$BGRN=== RESULTS ==="
    echo -e "$GRN$PASSED_TESTS$WHI/$TOTAL_TESTS tests passed"

    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        tit_wrap "$BGRN All tests passed!"
        exit 0
    else
        tit_wrap "$BRED Some tests failed"
        exit 1
    fi
}

main
