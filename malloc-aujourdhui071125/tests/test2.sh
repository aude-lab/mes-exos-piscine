#!/bin/sh

#set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
LIBRARY="$PROJECT_ROOT/libmalloc.so"
TESTS_PASSED=0
TESTS_FAILED=0

print_success() 
{ 
	echo -e "${GREEN}PASS $1${NC}"
	TESTS_PASSED=$((TESTS_PASSED + 1))
}

print_failure() 
{ 
	echo -e "${RED}FAIL $1${NC}"
       	TESTS_PASSED=$((TESTS_FAILED + 1)) 
}

print_info() 
{ 
	echo -e "${BLUE}INFO $1${NC}"
}

check_library() 
{
    if [ ! -f "$LIBRARY" ]; then
        echo -e "${RED}Erreur: $LIBRARY non trouvée${NC}"
        exit 1
    fi
    echo "Youpi Library trouvée!"
}

run_with_malloc() 
{
    LD_LIBRARY_PATH="$PROJECT_ROOT" LD_PRELOAD="$LIBRARY" "$@"
}

assert_exit_success() 
{
    local exit_code=$?
    local test_name="$1"
    
    if [ $exit_code -eq 0 ]; then
        print_success "$test_name"
        return 0
    else
        print_failure "$test_name (exit code: $exit_code)"
        return 1
    fi
}

cleanup() 
{
    rm -f test_* *.o 2>/dev/null
}

#PARTIE-TEST

test_malloc_basic() 
{
    print_info "Test malloc basique"
    
    cat > test_malloc_basic.c << 'EOF'
#include "src/malloc.h"
#include <stdio.h>

int main() 
{
    printf("  Début test malloc basique...\n");
    void *ptr = malloc(100);
    printf("  malloc(100) = %p\n", ptr);
    
    if (ptr == NULL) 
    {
        printf("  ÉCHEC: malloc NULL\n");
        return 1;
    }
    
    printf("  SUCCÈS: allocation OK\n");
    free(ptr);
    printf("  free OK\n");
    return 0;
}
EOF

    gcc test_malloc_basic.c -o test_malloc_basic
    run_with_malloc ./test_malloc_basic
    local result=$?
    rm -f test_malloc_basic test_malloc_basic.c
    return $result
}

test_malloc_zero() 
{
    print_info "Test malloc(0)"
    
    cat > test_malloc_zero.c << 'EOF'
#include "src/malloc.h"
#include <stdio.h>

int main() 
{
    printf("  Début test malloc(0)...\n");
    void *ptr = malloc(0);
    printf("  malloc(0) = %p\n", ptr);
    
    if (ptr != NULL) 
    {
        free(ptr);
        printf("  free sur pointeur non-NULL\n");
    }
    printf("  Test malloc(0) terminé\n");
    return 0;
}
EOF

    gcc test_malloc_zero.c -o test_malloc_zero
    run_with_malloc ./test_malloc_zero
    local result=$?
    rm -f test_malloc_zero test_malloc_zero.c
    return $result
}

test_free_null() 
{
    print_info "Test free(NULL)"
    
    cat > test_free_null.c << 'EOF'
#include "src/malloc.h"
#include <stdio.h>

int main() 
{
    printf("  Début test free(NULL)...\n");
    free(NULL);
    printf("  free(NULL) OK\n");
    return 0;
}
EOF

    gcc test_free_null.c -o test_free_null
    run_with_malloc ./test_free_null
    local result=$?
    rm -f test_free_null test_free_null.c
    return $result
}

test_calloc_basic() 
{
    print_info "Test calloc basique"
    
    cat > test_calloc_basic.c << 'EOF'
#include "src/malloc.h"
#include <stdio.h>

int main() 
{
    printf("  Début test calloc...\n");
    int *ptr = calloc(10, sizeof(int));
    printf("  calloc(10, sizeof(int)) = %p\n", ptr);
    
    if (ptr == NULL) 
    {
        printf("  ÉCHEC: calloc NULL\n");
        return 1;
    }
    
    
    printf("  Vérification initialisation à zéro...\n");
    for (int i = 0; i < 10; i++) 
    {
        if (ptr[i] != 0) 
        {
            printf("  ÉCHEC: calloc pas initialisé à zéro à l'index %d\n", i);
            free(ptr);
            return 1;
        }
    }
    
    printf("  SUCCÈS: calloc OK\n");
    free(ptr);
    return 0;
}
EOF

    gcc test_calloc_basic.c -o test_calloc_basic
    run_with_malloc ./test_calloc_basic
    local result=$?
    rm -f test_calloc_basic test_calloc_basic.c
    return $result
}

test_realloc_basic() 
{
    print_info "Test realloc basique"
    
    cat > test_realloc_basic.c << 'EOF'
#include "src/malloc.h"
#include <stdio.h>

int main() 
{
    printf("  Début test realloc...\n");
    

    int *ptr = malloc(5 * sizeof(int));
    printf("  malloc(5*sizeof(int)) = %p\n", ptr);
    
    if (ptr == NULL) 
    {
        printf("  ÉCHEC: malloc initial NULL\n");
        return 1;
    }
    
    
    for (int i = 0; i < 5; i++) 
    {
        ptr[i] = i * 10;
    }
    
    
    printf("  Appel realloc...\n");
    int *new_ptr = realloc(ptr, 10 * sizeof(int));
    printf("  realloc résultat = %p\n", new_ptr);
    
    if (new_ptr == NULL) 
    {
        printf("  ÉCHEC: realloc NULL\n");
        free(ptr);
        return 1;
    }
    
    
    printf("  Vérification données...\n");
    for (int i = 0; i < 5; i++) 
    {
        if (new_ptr[i] != i * 10) 
        {
            printf("  ÉCHEC: données perdues à l'index %d\n", i);
            free(new_ptr);
            return 1;
        }
    }
    
    printf("  SUCCÈS: realloc OK\n");
    free(new_ptr);
    return 0;
}
EOF

    gcc test_realloc_basic.c -o test_realloc_basic
    run_with_malloc ./test_realloc_basic
    local result=$?
    rm -f test_realloc_basic test_realloc_basic.c
    return $result
}


echo -e "${YELLOW}=== TEST SUITE FOR MALLOC ===${NC}"

check_library
cleanup

echo -e "\n=== TESTS MALLOC ==="
test_malloc_basic && assert_exit_success "Malloc basique" || assert_exit_success "Malloc basique"

echo "--- Test suivant ---"
test_malloc_zero && assert_exit_success "Malloc zero" || assert_exit_success "Malloc zero"

echo -e "\n=== TESTS FREE ==="
test_free_null && assert_exit_success "Free NULL" || assert_exit_success "Free NULL"

echo -e "\n=== TESTS CALLOC ==="  
test_calloc_basic && assert_exit_success "Calloc basique" || assert_exit_success "Calloc basique"

echo -e "\n=== TESTS REALLOC ==="
test_realloc_basic && assert_exit_success "Realloc basique" || assert_exit_success "Realloc basique"

echo -e "\n=== TEST ACU PROVIDED ==="
echo
LD_PRELOAD=./libmalloc.so ls
echo
LD_PRELOAD=./libmalloc.so ./speed; echo $?
echo
LD_PRELOAD=./libmalloc.so ./corruptionproof; echo $?

echo
cleanup
echo
echo "=== RÉSUMÉ ==="
echo -e "${GREEN}Tests passés(hors tests acu): $TESTS_PASSED${NC}"
echo -e "${RED}Tests échoués: $TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}Geniaaall Tous les tests, hors les tests acu, sont passés !${NC}"
    exit 0
else
    echo -e "${RED}Zuut Certains tests ont échoué${NC}"
    exit 1
fi
