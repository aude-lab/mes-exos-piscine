#!/bin/bash

echo "============= TESTING STARTING ================================"


echo "Test 1: 1+1"
echo "1+1" | ./evalexpr
echo "Expected: 2"
echo "Result: $(echo "1+1" | ./evalexpr)"
echo

echo "Test 2: 5-2"
echo "5-2" | ./evalexpr
echo "Expected: 3"
echo "Result: $(echo "5-2" | ./evalexpr)"
echo

echo "Test 3: 3*4"
echo "3*4" | ./evalexpr
echo "Expected: 12"
echo "Result: $(echo "3*4" | ./evalexpr)"
echo

echo "Test 4: 8/2"
echo "8/2" | ./evalexpr
echo "Expected: 4"
echo "Result: $(echo "8/2" | ./evalexpr)"
echo

echo "Test 5: 10%3"
echo "10%3" | ./evalexpr
echo "Expected: 1"
echo "Result: $(echo "10%3" | ./evalexpr)"
echo

echo "Test 6: 2^3"
echo "2^3" | ./evalexpr
echo "Expected: 8"
echo "Result: $(echo "2^3" | ./evalexpr)"
echo

echo "Test 7: 2+3*4"
echo "2+3*4" | ./evalexpr
echo "Expected: 14"
echo "Result: $(echo "2+3*4" | ./evalexpr)"
echo

echo "Test 8: 10-8/2"
echo "10-8/2" | ./evalexpr
echo "Expected: 6"
echo "Result: $(echo "10-8/2" | ./evalexpr)"
echo

echo "Test 9: (1+2)"
echo "(1+2)" | ./evalexpr
echo "Expected: 3"
echo "Result: $(echo "(1+2)" | ./evalexpr)"
echo

echo "Test 10: (2+3)*4"
echo "(2+3)*4" | ./evalexpr
echo "Expected: 20"
echo "Result: $(echo "(2+3)*4" | ./evalexpr)"
echo

echo "Test 11: ((1+2)*3)"
echo "((1+2)*3)" | ./evalexpr
echo "Expected: 9"
echo "Result: $(echo "((1+2)*3)" | ./evalexpr)"
echo

echo "Test 12: 2*(3+4)-5"
echo "2*(3+4)-5" | ./evalexpr
echo "Expected: 9"
echo "Result: $(echo "2*(3+4)-5" | ./evalexpr)"
echo

echo "Test 13: 3+4*2/(1-5)"
echo "3+4*2/(1-5)" | ./evalexpr
echo "Expected: 1"
echo "Result: $(echo "3+4*2/(1-5)" | ./evalexpr)"
echo

echo "Test 14: 2^3*4"
echo "2^3*4" | ./evalexpr
echo "Expected: 32"
echo "Result: $(echo "2^3*4" | ./evalexpr)"
echo

echo "Test 15: 2*3^2"
echo "2*3^2" | ./evalexpr
echo "Expected: 18"
echo "Result: $(echo "2*3^2" | ./evalexpr)"
echo

echo "Test 16: -5+3"
echo "-5+3" | ./evalexpr
echo "Expected: -2"
echo "Result: $(echo "-5+3" | ./evalexpr)"
echo

echo "Test 17: +2*-3"
echo "+2*-3" | ./evalexpr
echo "Expected: -6"
echo "Result: $(echo "+2*-3" | ./evalexpr)"
echo

echo "Test 18: --5"
echo "--5" | ./evalexpr
echo "Expected: 5"
echo "Result: $(echo "--5" | ./evalexpr)"
echo

echo "Test 19: 3--+++4"
echo "3--+++4" | ./evalexpr
echo "Expected: 7"
echo "Result: $(echo "3--+++4" | ./evalexpr)"
echo

echo "Test 20: 86*--1"
echo "86*--1" | ./evalexpr
echo "Expected: 86"
echo "Result: $(echo "86*--1" | ./evalexpr)"
echo

echo "Test 21: espaces simples"
echo "   1   +   2   " | ./evalexpr
echo "Expected: 3"
echo "Result: $(echo "   1   +   2   " | ./evalexpr)"
echo

echo "Test 22: espaces avec parenthèses"
echo " ( 1 + 2 ) * 3 " | ./evalexpr
echo "Expected: 9"
echo "Result: $(echo " ( 1 + 2 ) * 3 " | ./evalexpr)"
echo

echo "Test 23: espaces complexes"
echo "  2  *  (  3  +  4  )  -  5  " | ./evalexpr
echo "Expected: 9"
echo "Result: $(echo "  2  *  (  3  +  4  )  -  5  " | ./evalexpr)"
echo

echo "Test 24: 1 1 + (RPN)"
echo "1 1 +" | ./evalexpr -rpn
echo "Expected: 2"
echo "Result: $(echo "1 1 +" | ./evalexpr -rpn)"
echo

echo "Test 25: 5 2 - (RPN)"
echo "5 2 -" | ./evalexpr -rpn
echo "Expected: 3"
echo "Result: $(echo "5 2 -" | ./evalexpr -rpn)"
echo

echo "Test 26: 3 4 * (RPN)"
echo "3 4 *" | ./evalexpr -rpn
echo "Expected: 12"
echo "Result: $(echo "3 4 *" | ./evalexpr -rpn)"
echo

echo "Test 27: 8 2 / (RPN)"
echo "8 2 /" | ./evalexpr -rpn
echo "Expected: 4"
echo "Result: $(echo "8 2 /" | ./evalexpr -rpn)"
echo

echo "Test 28: 10 3 % (RPN modulo)"
echo "10 3 %" | ./evalexpr -rpn
echo "Expected: 1"
echo "Result: $(echo "10 3 %" | ./evalexpr -rpn)"
echo

echo "Test 29: 2 3 ^ (RPN puissance)"
echo "2 3 ^" | ./evalexpr -rpn
echo "Expected: 8"
echo "Result: $(echo "2 3 ^" | ./evalexpr -rpn)"
echo

echo "Test 30: 5 1 2 + 4 * + 3 - (RPN complexe)"
echo "5 1 2 + 4 * + 3 -" | ./evalexpr -rpn
echo "Expected: 14"
echo "Result: $(echo "5 1 2 + 4 * + 3 -" | ./evalexpr -rpn)"
echo

echo "Test 31: 2 3 4 * + (RPN priorité)"
echo "2 3 4 * +" | ./evalexpr -rpn
echo "Expected: 14"
echo "Result: $(echo "2 3 4 * +" | ./evalexpr -rpn)"
echo

echo "Test 32: 2 3 ^ 4 * (RPN puissance)"
echo "2 3 ^ 4 *" | ./evalexpr -rpn
echo "Expected: 32"
echo "Result: $(echo "2 3 ^ 4 *" | ./evalexpr -rpn)"
echo

echo "Test 33: espaces RPN"
echo "  2   3   *   4   +  " | ./evalexpr -rpn
echo "Expected: 10"
echo "Result: $(echo "  2   3   *   4   +  " | ./evalexpr -rpn)"
echo

echo "Test 34: erreur division par zéro"
echo "1/0" | ./evalexpr
echo "Expected: Nothing + Code: 3"
echo "Result: Nothing + Code: $(echo "1/0" | ./evalexpr 2>/dev/null; echo $?)"
echo

echo "Test 35: erreur modulo par zéro"
echo "5%0" | ./evalexpr
echo "Expected: Nothing + Code: 3"
echo "Result: Nothing + Code: $(echo "5%0" | ./evalexpr 2>/dev/null; echo $?)"
echo

echo "Test 36: mauvais argument"
./evalexpr --invalid 2>/dev/null
echo "Expected: Nothing + Code: 4"
echo "Result: Nothing + Code: $?"
echo




echo "=== END TEST ==="
