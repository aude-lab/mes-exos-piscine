#include "lookup_table.h"

void apply_lut(unsigned char mat[4][4], const unsigned char lut[256])
{
    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            mat[i][j] = lut[mat[i][j]];
        }
    }
}
