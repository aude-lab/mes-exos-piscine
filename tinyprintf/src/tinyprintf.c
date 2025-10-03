#include "tinyprintf.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
static int len_base(const char *base)
{
    int lenbase = 0;
    while (base[lenbase] != '\0')
    {
        lenbase += 1;
    }
    return lenbase;
}
static int numberdig(unsigned int n, const char *base)
{
    int baselen = len_base(base);
    int digits = 0;
    while (n > 0)
    {
        digits++;
        n /= baselen;
    }
    return digits;
}
static char *my_itoabase(unsigned int n, char *s, const char *base)
{
    int lenbase = len_base(base);
    int index = 0;
    if (n == 0)
    {
        s[index] = base[0];
        s[index + 1] = '\0';
        return s;
    }
    /*if (n < 0 && lenbase == 10)
    {
        // s[index] = '-';
        n = -n;
        // index += 1;
    }*/
    int digits = numberdig(n, base);
    for (int i = digits - 1; i >= 0; i--)
    {
        int chiffre = n % lenbase;
        s[index + i] = base[chiffre];
        n /= lenbase;
    }
    index += digits;
    s[index] = '\0';
    return s;
}
static int print_entier_decimal(int value)
{
    /*int nombrecarac = 0;
    if (value == -2147483648)
    {
        value = value + 1000;
    }
    if (value < 0)
    {
        putchar('-');
        nombrecarac += 1;
        value = -1 * value;
    }
    else
    {
        value = +value;
    }
    int i = 0;
    int m = numberdig(value, "0123456789");
    char *s = malloc(m + 2);
    char *str = my_itoabase(value, s, "0123456789");
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
        nombrecarac++;
    }
    free(s);
    return nombrecarac;*/

	if (value == -2147483648)
    {
        const char *min_str = "-2147483648";
        for (int i = 0; min_str[i] != '\0'; i++)
        {
            putchar(min_str[i]);
        }
        return 11;
    }

    int count = 0;

    if (value < 0)
    {
        putchar('-');
        count++;
        value = -value;
    }

    if (value == 0)
    {
        putchar('0');
        return count + 1;
    }

    char buffer[32];
    int index = 0;
    int n = value;  

    while (n > 0)
    {
        buffer[index++] = (n % 10) + '0';
        n /= 10;
    }

    for (int i = index - 1; i >= 0; i--)
    {
        putchar(buffer[i]);
        count++;
    }

    return count;
}




static int print_entier_octal(int value)
{
    int nombrecarac = 0;
    int m = numberdig(value, "01234567");
    char *s = malloc(m + 2);
    char *str = my_itoabase(value, s, "01234567");
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
        nombrecarac++;
    }
    free(s);
    return nombrecarac;
}



static int print_entier_hexa(unsigned int value)
{
    if (value == 0)
    {
        putchar('0');
        return 1;
    }
    int nombrecarac = 0;
    int m = numberdig(value, "0123456789abcdef");
    char *s = malloc(m + 2);
    char *str = my_itoabase(value, s, "0123456789abcdef");
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
        nombrecarac++;
    }
    free(s);
    return nombrecarac;
}



static int print_unsigned(unsigned int value)
{
    /*unsigned int c;
    int nombrecarac = 0;
    if (value < 0)
        c = value;
    else
        c = value;
    if (c == 0)
    {
        putchar('0');
        nombrecarac++;
        return nombrecarac;
    }
    else
    {
        char str[64];
        int i = 0;
        while (c > 0)
        {
            str[i] = c % 10 + '0';
            c = c / 10;
            i++;
            nombrecarac++;
        }
        while (i >= 0)
        {
            putchar(str[i]);
            i--;
        }
        return nombrecarac;
    }*/
	if (value == 0)
    {
        putchar('0');
        return 1;
    }

    char buffer[32];
    int index = 0;
    unsigned int n = value;

    while (n > 0)
    {
        buffer[index++] = (n % 10) + '0';
        n /= 10;
    }

    int count = index;

    for (int i = index - 1; i >= 0; i--)
    {
        putchar(buffer[i]);
    }

    return count;
}

static int print_str(const char *str)
{
    int nombrecarac = 0;
    if (str == NULL)
    {
        const char *nullstr = "(null)";
        int d = len_base(nullstr);
        for (int i = 0; i < d; i++)
        {
            putchar(nullstr[i]);
            nombrecarac += 1;
        }
    }
    else
    {
        int k = len_base(str);
        for (int i = 0; i < k; i++)
        {
            putchar(str[i]);
            nombrecarac += 1;
        }
    }
    return nombrecarac;
}

static int default_case(const char f)
{
    int nombrecaractere = 0;
    if (f == '%')
    {
        putchar(f);
        nombrecaractere += 1;
    }
    else
    {
        putchar('%');
        putchar(f);
        nombrecaractere += 2;
    }
    return nombrecaractere;
}





int tinyprintf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int i = 0;
    int nombrecaractere = 0;
    
    
    while (format[i] != '\0')
    {
        if (format[i] == '%')
        {
            i++;

            switch (format[i])
            {
            case 'd': {
                nombrecaractere += print_entier_decimal(va_arg(args, int));
                break;
            }
            case 'x': {
                nombrecaractere += print_entier_hexa(va_arg(args, unsigned int));
                break;
            }
            case 'o': {
                nombrecaractere += print_entier_octal(va_arg(args, unsigned int));
                break;
            }
            case 'c': {
                nombrecaractere += 1;
                putchar(va_arg(args, int));
                break;
            }
            case 's': {
                nombrecaractere += print_str(va_arg(args, const char *));
                break;
            }
            case 'u': {
                nombrecaractere += print_unsigned(va_arg(args, unsigned int));
                break;
            }
            default: {

                nombrecaractere += default_case(format[i]);
            }
            }
            i++;
        }
        else
        {
            putchar(format[i]);
            nombrecaractere += 1;
            i++;
        }
    }
    return nombrecaractere;
}










/*int main(void)
{
    int t = tinyprintf("No deadc0de allowed");
    printf("%d\n", t);
    // tinyprintf("%c%c is %s... %d too.", '4', '2', "the answer", '*');
    // tinyprintf("%%s", "in your head");
    // int d = tinyprintf("Good morning ACU! %t Tinyprintf is cool", 12);
    // printf("%d\n", d);
}



int main(void) {
    tinyprintf("I love positive numbers like: %u", 12);
    return 0;
}*/
