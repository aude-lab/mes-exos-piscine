A = first
B = second
C = $(A)_and_$(B)

target_$(A): dep_$(B)
	echo "Target: $(A)"
	echo "Combined: $(C)"
	echo "Single char: $B"

dep_$(B):
	echo "Dependency: $(B)"

$(C)_rule:
	echo "Complex target: $(C)"

# Test variables récursives
VAR1 = hello
VAR2 = $(VAR1)_world
VAR3 = $(VAR2)_again

test_recursive:
	echo "VAR1=$(VAR1)"
	echo "VAR2=$(VAR2)" 
	echo "VAR3=$(VAR3)"

# Test dans les noms de variables
PREFIX = MY_
$(PREFIX)VAR = special_value

test_complex:
	echo "$(PREFIX)VAR = $($(PREFIX)VAR)"
