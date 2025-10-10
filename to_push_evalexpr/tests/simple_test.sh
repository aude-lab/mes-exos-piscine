#!/bin/bash

echo "=== debut Tests ==="

echo "Test 1: 1 1 +"
echo "1 1 +" | ./evalexpr -rpn

echo "Test 2: 2 3 *" 
echo "2 3 *" | ./evalexpr -rpn

echo "Test 3: 10 5 -"
echo "10 5 -" | ./evalexpr -rpn


echo "1. Test: 1+1 (standard)"
echo "1+1" | ./evalexpr
echo

echo "2. Test: 5-2 (standard)"
echo "5-2" | ./evalexpr
echo

echo "3. Test: 3*4 (standard)"
echo "3*4" | ./evalexpr
echo

echo "4. Test: 8/2 (standard)"
echo "8/2" | ./evalexpr
echo

echo "5. Test: 2+3*4 (priorité standard)"
echo "2+3*4" | ./evalexpr
echo

echo "6. Test: 10-8/2 (priorité standard)"
echo "10-8/2" | ./evalexpr
echo

echo "7. Test: 10-3-2 (associativité standard)"
echo "10-3-2" | ./evalexpr
echo

echo "8. Test: (1+2) (parenthèses standard)"
echo "(1+2)" | ./evalexpr
echo

echo "9. Test: (2+3)*4 (parenthèses standard)"
echo "(2+3)*4" | ./evalexpr
echo

echo "10. Test: ((1+2)*3) (parenthèses imbriquées standard)"
echo "((1+2)*3)" | ./evalexpr
echo

echo "11. Test: 2*(3+4)-5 (complexe standard)"
echo "2*(3+4)-5" | ./evalexpr
echo

echo "12. Test: espaces multiples (standard)"
echo "   1   +   2   " | ./evalexpr
echo

echo "13. Test: espaces avec parenthèses (standard)"
echo " ( 1 + 2 ) * 3 " | ./evalexpr
echo

echo "14. Test: 3+4*2/(1-5) (complexe standard)"
echo "3+4*2/(1-5)" | ./evalexpr
echo

echo "15. Test: 10-2*3+4 (complexe standard)"
echo "10-2*3+4" | ./evalexpr
echo

echo "16. Test: 1 1 + (RPN)"
echo "1 1 +" | ./evalexpr -rpn
echo

echo "17. Test: 5 2 - (RPN)"
echo "5 2 -" | ./evalexpr -rpn
echo

echo "18. Test: 3 4 * (RPN)"
echo "3 4 *" | ./evalexpr -rpn
echo

echo "19. Test: 8 2 / (RPN)"
echo "8 2 /" | ./evalexpr -rpn
echo


echo "20. Test: 5 1 2 + 4 * + 3 - (RPN complexe)"
echo "5 1 2 + 4 * + 3 -" | ./evalexpr -rpn
echo

echo "21. Test: espaces RPN"
echo "  2   3   *   4   +  " | ./evalexpr -rpn
echo

echo "22. Test: erreur caractère invalide"
echo "1 + a" | ./evalexpr
echo "Code: $?"
echo

echo "23. Test: erreur parenthèses en RPN"
echo "(1+2)" | ./evalexpr -rpn
echo "Code: $?"
echo

echo "24. Test: erreur parenthèse non fermée"
echo "(1+2" | ./evalexpr
echo "Code: $?"
echo

echo "25. Test: mauvais argument"
./evalexpr --invalid 2>/dev/null
echo "Code: $?"
echo


echo "=== Fin des tests ==="
