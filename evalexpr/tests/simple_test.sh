#!/bin/bash

echo "=== Tests basiques ==="

test_rpn() {
    input="$1"
    expected="$2"
    result=$(echo "$input" | ./evalexpr -rpn 2>/dev/null)

    if [ "$result" = "$expected" ]; then
        echo "'$input' = $expected"
    else
        echo "'$input' = $result (expected: $expected)"
    fi
}

test_rpn "1 1 +" "2"
test_rpn "2 3 *" "6"
test_rpn "10 5 -" "5"
test_rpn "8 2 /" "4"
test_rpn "2 3 ^" "8" 

echo "=== Tests terminés ==="
