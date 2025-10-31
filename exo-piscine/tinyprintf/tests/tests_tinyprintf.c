#include <criterion/criterion.h>
#include <criterion/criterion.h>
#include "../src/tinyprintf.h"

Test(tinyprintf, test_str)
{
    int res = tinyprintf("%s\n", "Hello world!");

    cr_expect_eq(res, 13, "Expected 13 to be printed, but got %d", res);
}

Test(tinyprintf, test_str_int)
{
    int res = tinyprintf("%s [%d] %s\n", "Hello", 22, "world!");

    cr_expect_eq(res, 18, "Expected 18 to be printed, but got %d", res);
}

Test(tinyprintf, test_null)
{
    int res = tinyprintf("%s\n", NULL);

    cr_expect_eq(res, 7, "Expected 7 to be printed, but got %d", res);
}

Test(tinyprintf, test_pos_int)
{
    int res = tinyprintf("%d\n", 82);

    cr_expect_eq(res, 3, "Expected 3 to be printed, but got %d", res);
}

Test(tinyprintf, test_neg_lint)
{
    int res = tinyprintf("%d\n", -2147483648);

    cr_expect_eq(res, 12, "Expected 12 to be printed, but got %d", res);
}

Test(tinyprintf, test_octal)
{
    int res = tinyprintf("%o\n", 22);

    cr_expect_eq(res, 3, "Expected 3 to be printed, but got %d", res);
}

Test(tinyprintf, test_hexadecimal)
{
    int res = tinyprintf("%x\n", 22);

    cr_expect_eq(res, 3, "Expected 3 to be printed, but got %d", res);
}

Test(tinyprintf, test_unsigned_pos)
{
    int res = tinyprintf("%u\n", 22);

    cr_expect_eq(res, 3, "Expected 3 to be printed, but got %d", res);
}

Test(tinyprintf, test_undigned_neg)
{
    int res = tinyprintf("%u\n", -22);

    cr_expect_eq(res, 11, "Expected 11 to be printed, but got %d", res);
}

Test(tinyprintf, test_percentage)
{
    int res = tinyprintf("%%s\n", "in the EPITA");

    cr_expect_eq(res, 3, "Expected 3 to be printed, but got %d", res);
}

Test(tinyprintf, test_unknown)
{
    int res = tinyprintf("Good morning ACU! %t Tinyprintf is cool\n", 12);

    cr_expect_eq(res, 40, "Expected 40 to be printed, but got %d", res);
}

Test(tinyprintf, test_tricky)
{
    int res =
        tinyprintf("%c%c is %s... %d too.\n", '4', '2', "the answer", '*');

    cr_expect_eq(res, 28, "Expected 28 to be printed, but got %d", res);
}

Test(tinyprintf, test_charactere)
{
    int res = tinyprintf("%c\n", 'a');

    cr_expect_eq(res, 2, "Expected 2 to be printed, but got %d", res);
}

Test(tinyprintf, order_test) {
    int res = tinyprintf("I love positive numbers like: %u", 12);
    cr_expect_eq(res, 32, "Expected 32 characters, got %d", res);
}



Test(tinyprintf, detect_int_min_bug) {
	int res = tinyprintf("Test: %d", '*');
	cr_expect_eq(res, 8, "Expected 8 characters ('Test: 42'), got %d", res);
}
