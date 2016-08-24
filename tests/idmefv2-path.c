#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "idmefv2.h"


static void set_value_check(idmefv2_message_t *idmefv2, const char *paths,
                            const char *str_value, libidmefv2_bool_t verify_get)
{
        idmefv2_path_t *path;
        idmefv2_value_t *value;
        libidmefv2_string_t *str;
        libidmefv2_string_t *res;

        assert(idmefv2_path_new_fast(&path, paths) == 0);
        assert(libidmefv2_string_new_ref(&str, str_value) == 0);
        assert(idmefv2_value_new_string(&value, str) == 0);

        if ( verify_get )
                assert(idmefv2_path_set(path, idmefv2, value) == 0);
        else
                assert(idmefv2_path_set(path, idmefv2, value) < 0);

        idmefv2_value_destroy(value);

        if ( ! verify_get ) {
                idmefv2_path_destroy(path);
                return;
        }

        assert(idmefv2_path_get(path, idmefv2, &value) > 0);

        assert(libidmefv2_string_new(&res) == 0);
        assert(idmefv2_value_to_string(value, res) >= 0);
        assert(strcmp(str_value, libidmefv2_string_get_string(res)) == 0);
        libidmefv2_string_destroy(res);

        idmefv2_value_destroy(value);
        idmefv2_path_destroy(path);
}

int main(void)
{
        int i, ret;
        idmefv2_value_t *value;
        idmefv2_path_t *path;
        idmefv2_message_t *idmefv2;
        struct {
                const char *path;
                int depth;
                libidmefv2_bool_t has_list;
                libidmefv2_bool_t ambiguous;
                libidmefv2_bool_t successful;
        } plist[] = {
                { "alert.classification.text", 3, FALSE, FALSE, TRUE },
                { "alert.assessment.impact.impact_type(0).name", 5, TRUE, FALSE, TRUE },
                { "alert.classification.reference(0).name", 4, TRUE, FALSE, TRUE },
                { "alert.analyzer(0).analyzerid", 3, TRUE, FALSE, TRUE },
                { "alert.invalid.path", 0, FALSE, FALSE, FALSE }
        };

        assert(idmefv2_message_new(&idmefv2) == 0);

        for ( i = 0; i < sizeof(plist) / sizeof(*plist); i++ ) {
                ret = idmefv2_path_new_fast(&path, plist[i].path);
                assert((plist[i].successful == TRUE && ret == 0) || (plist[i].successful == FALSE && ret < 0));
                 
                if ( ret < 0 )
                        continue;
                
                assert(strcmp(plist[i].path, idmefv2_path_get_name(path, -1)) == 0);
                assert(idmefv2_path_get_depth(path) == plist[i].depth);
                assert(idmefv2_path_has_lists(path) == plist[i].has_list);
                assert(idmefv2_path_is_ambiguous(path) == plist[i].ambiguous);

                if ( plist[i].ambiguous ) {
                        idmefv2_path_destroy(path);
                        continue;
                }

                /*
                 * Check whether setting NULL value work.
                 */
                ret = idmefv2_path_set(path, idmefv2, NULL);
                assert(ret == 0);
                ret = idmefv2_path_get(path, idmefv2, &value);
                assert(ret == 0); /* empty value */

                idmefv2_path_destroy(path);
        }

        set_value_check(idmefv2, "alert.classification.text", "Random value", TRUE);
        set_value_check(idmefv2, "alert.assessment.impact.severity", "high", TRUE);
        set_value_check(idmefv2, "alert.assessment.impact.severity", "Invalid enumeration", FALSE);

        idmefv2_message_destroy(idmefv2);
        printf("successful test \n");
        exit(0);
}
