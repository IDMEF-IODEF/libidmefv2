#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "idmefv2.h"

#define TEST_STR "abcdefghijklmnopqrstuvwxyz"
#define MAX_LAG_SEC 3


int main(void)
{
        time_t now;
        idmefv2_time_t *ctime;
        idmefv2_alert_t *alert;
        idmefv2_message_t *idmefv2;

        assert(idmefv2_message_new(&idmefv2) == 0);
        assert(idmefv2_message_new_alert(idmefv2, &alert) == 0);

        ctime = idmefv2_alert_get_create_time(alert);
        assert(ctime != NULL);

        now = time(NULL);
        assert(now - idmefv2_time_get_sec(ctime) < MAX_LAG_SEC);
        printf("Hello \n");
        exit(0);
}
