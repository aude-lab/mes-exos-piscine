#!/bin/sh

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
	echo  $@$WHI
}

print_result()
{
	if [ $1 -eq 0 ]; then
		tit_wrap "$GRN PASS"
		PASSED_TESTS=$((PASSED_TESTS + 1))

	else
		tit_wrap "$RED FAIL"
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

    tit_wrap " $BLU Testing: $description"
    tit_wrap "  File: $GRE$test_file"
    tit_wrap "  Args: $GRE$args"
    echo

    ./minimake $args -f "tests/$TEST_DIR/$category/$test_file" > /tmp/actual_output 2>&1

    if echo "$expected_output" | diff -q /tmp/actual_output - > /dev/null 2>&1; then
        print_result 0

     else
        tit_wrap $GRN EXPECTED:
	echo
        echo "$expected_output"
	echo
	echo
        tit_wrap $RED GOT:
	echo
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
	run_test "basic" "simple.mk" "all" "Hello World" "SIMPLE RULE EXECUTION"
    	run_test "basic" "help.mk" "-h" "Use: minimake [options] [targets]
Options:
  -f file    Using file as makefile
  -h         Showing this help message
  -p         Pretty-printing the makefile" "HELP OPTION"

	run_test "basic" "Makefile.syntax-test" "-p" "# variables
'SIMPLE_VAR' = 'coucou'
'SIMPLE_VAR_COMMENT' = 'the comment is gone '
'SPACES_BEFORE_TAB' = 'var_beginning    var_end'
'B' = 'B_var_beginning  B_var_end'
# rules
(sparse_rule): [depa] [depb]
    'command 1'
    'command 2'
(packed_rule): [depa] [depb]
    'command 1'
    'command 2'
(silent_rule): [depa] [depb]
    '@ command 1'
    '@command 2'
(rule_comment): [depa] [depb]
(command_space_rule): [depa] [depb]
    'echo spaces before'
    'echo spaces after        '
    'echo this is a # comment'
(simple_rule): [simple_dep]
(no_dep_rule):
(variable_rule): [beginning] [coucou] [end]
    'echo \"shouldn't be expanded: \$(SIMPLE_VAR)\"'" "PRETTY PRINT SYNTAX TEST"
}











run_variable_tests() {
    tit_wrap "$PUR=== VARIABLE TESTS ==="
    
    run_test "variables" "simple_var.mk" "all" "echo value\nvalue" "SIMPLE VARIABLE"
    run_test "variables" "recursive_var.mk" "all" "echo final value\nfinal value" "RECURSIVE VARIABLES"
}




run_subject_tests() {
    tit_wrap "$PUR=== SUBJECT EXAMPLE TESTS ==="
    
    run_test "test_sujet" "var_subject.mk" "all" "echo foo     bar\nfoo bar" "VARIABLE SUBJECT EXAMPLE"
    run_test "test_sujet" "silent.mk" "all" "a line" "SILENT COMMAND SUBJECT EXAMPLE"
    run_test "test_sujet" "recursive_subject.mk" "all" "The answer is 42 and not 2" "RECURSIVE VARIABLE EXAMPLE"
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
    run_variable_tests
    run_subject_tests

    tit_wrap "$BGRN=== RESULTS ==="
    echo "$GRN$PASSED_TESTS$WHI/$TOTAL_TESTS tests passed"

    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        tit_wrap "$BGRN All tests passed!"
        exit 0
    else
        tit_wrap "$BRED Some tests failed"
        exit 1
    fi
}

main
