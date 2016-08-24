#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "idmefv2.h"

#define FLOAT_TOLERANCE 0.0001

#ifndef ABS
# define ABS(x) (((x) < 0) ? -(x) : (x))
#endif

#ifndef MAX
# define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif


static double reldif(double a, double b)
{
        double c = ABS(a);
        double d = ABS(b);

        d = MAX(c, d);

        return d == 0.0 ? 0.0 : ABS(a - b) / d;
}


int main(void)
{
        char *res;
        char *resd;
        idmefv2_message_t *idmefv2;

        assert(idmefv2_message_new(&idmefv2) == 0);

        assert(idmefv2_message_set_string(idmefv2, "alert.classification.text", "Random value") == 0);
        assert(idmefv2_message_get_string(idmefv2, "alert.classification.text", &res) > 0);
        assert(strcmp("Random value", res) == 0);
        free(res);

        assert(idmefv2_message_set_string(idmefv2, "alert.assessment.impact.severity", "high") == 0);
        assert(idmefv2_message_get_string(idmefv2, "alert.assessment.impact.severity", &res) > 0);
        assert(strcmp("high", res) == 0);
        free(res);

        assert(idmefv2_message_set_string(idmefv2, "alert.assessment.impact.severity", "Random value") < 0);
        printf("successful test \n");
        exit(0);
}
