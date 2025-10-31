#include "simple_fnmatch.h"

int simple_fnmatch(const char *pattern, const char *string)
{
    if (*pattern == '\0' && *string == '\0')
        return 0;

    if (*pattern == '*')
    {
        const char *p = pattern + 1;
        while (*p == '*')
            p++;

        if (*p == '\0')
            return 0;

        for (const char *s = string; *s != '\0'; s++)
            if (simple_fnmatch(p, s) == 0)
                return 0;
        return FNM_NOMATCH;
    }

    if (*pattern == '\\')
    {
	    if (*(pattern + 1) == '\0')
        	return FNM_NOMATCH;  

    	    if (*(pattern + 1) == *string)
        	return simple_fnmatch(pattern + 2, string + 1);
    	    return FNM_NOMATCH;
    }

    if (*pattern == '?' && *string != '\0')
        return simple_fnmatch(pattern + 1, string + 1);

    if (*pattern == *string && *pattern != '\0')
        return simple_fnmatch(pattern + 1, string + 1);

    return FNM_NOMATCH;
}
