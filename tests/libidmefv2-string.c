#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "idmefv2.h"

#define TEST_COUNT 10
#define TEST_STR "abcdefghijklmnopqrstuvwxyz"


static void create_str(libidmefv2_string_t **str)
{
        size_t i;

        assert(libidmefv2_string_new(str) == 0);
        assert(libidmefv2_string_is_empty(*str) == TRUE);

        for ( i = 0; i < TEST_COUNT; i++ ) {
                assert(libidmefv2_string_cat(*str, TEST_STR) >= 0);
        }

        assert(libidmefv2_string_get_len(*str) == (TEST_COUNT * (sizeof(TEST_STR) - 1)));
}


int main(void)
{
        libidmefv2_string_t *str1, *str2;

        create_str(&str1);
        assert(libidmefv2_string_clone(str1, &str2) == 0);
        assert(libidmefv2_string_compare(str1, str2) == 0);

        assert(libidmefv2_string_cat(str2, "won't match") >= 0);
        assert(libidmefv2_string_compare(str1, str2) != 0);

        libidmefv2_string_destroy(str1);
        libidmefv2_string_destroy(str2);
        printf("successful test \n");
        exit(0);
}
