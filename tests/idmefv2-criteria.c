#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include "idmefv2.h"

static void test_criteria(idmefv2_message_t *idmefv2, const char *criteria_str, int expect_create, int expect_match)
{
        idmefv2_criteria_t *criteria;

        if ( expect_create < 0 ) {
                assert(idmefv2_criteria_new_from_string(&criteria, criteria_str) < 0);
                return;
        } else
                assert(idmefv2_criteria_new_from_string(&criteria, criteria_str) == 0);

        assert(idmefv2_criteria_match(criteria, idmefv2) == expect_match);
        idmefv2_criteria_destroy(criteria);

}

int main(void)
{
        idmefv2_time_t *ctime;
        idmefv2_alert_t *alert;
        idmefv2_message_t *idmefv2;
        idmefv2_classification_t *classification;
        libidmefv2_string_t *str;

        assert(libidmefv2_string_new_ref(&str, "A") == 0);

        assert(idmefv2_message_new(&idmefv2) == 0);
        assert(idmefv2_message_new_alert(idmefv2, &alert) == 0);
        assert(idmefv2_alert_new_classification(alert, &classification) == 0);
        idmefv2_classification_set_text(classification, str);

        idmefv2_message_set_string(idmefv2, "alert.analyzer(0).name", "A");
        idmefv2_message_set_string(idmefv2, "alert.analyzer(1).name", "B");
        idmefv2_message_set_string(idmefv2, "alert.analyzer(1).ostype", "C");
        idmefv2_message_set_string(idmefv2, "alert.source(1).ident", "I");
        idmefv2_message_set_string(idmefv2, "alert.analyzer(1).process.arg(0)", "a0");
        idmefv2_message_set_string(idmefv2, "alert.analyzer(1).process.arg(2)", "a2");

        test_criteria(idmefv2, "alert", 0, 1);
        test_criteria(idmefv2, "heartbeat", 0, 0);
        test_criteria(idmefv2, "alert || heartbeat", 0, 1);
        test_criteria(idmefv2, "alert.classification.txt == A", -1, -1);
        test_criteria(idmefv2, "alert.classification.text = (A || B || C || D) || heartbeat", 0, 1);
        test_criteria(idmefv2, "(alert.classification.text == A || heartbeat", -1, -1);
        libidmefv2_string_set_ref(str, "My String");
        test_criteria(idmefv2, "alert.classification.text != 'My String'", 0, 0);
        test_criteria(idmefv2, "alert.classification.text != 'random'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text == 'My String'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text <> 'My'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text <> 'my'", 0, 0);
        test_criteria(idmefv2, "alert.classification.text <>* 'my'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text ~ 'My String'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text ~ 'My (String|Check)'", 0, 1);
        test_criteria(idmefv2, "alert.classification.text ~ 'my'", 0, 0);
        test_criteria(idmefv2, "alert.classification.text ~* 'my'", 0, 1);
        idmefv2_alert_new_create_time(alert, &ctime);
        assert(idmefv2_time_set_from_string(ctime, "2015-05-03 1:59:08") == 0);

        /*
         * Regular time operator check
         */
        test_criteria(idmefv2, "alert.create_time == '2015-05-03 1:59:08'", 0, 1);
        test_criteria(idmefv2, "alert.create_time != '2015-05-03 1:59:08'", 0, 0);
        test_criteria(idmefv2, "alert.create_time < '2015-05-03 1:59:08'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > '2015-05-03 1:59:08'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= '2015-05-03 1:59:08'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= '2015-05-03 1:59:08'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == '2015-05-03 1:59:07'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != '2015-05-03 1:59:07'", 0, 1);
        test_criteria(idmefv2, "alert.create_time < '2015-05-03 1:59:07'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > '2015-05-03 1:59:07'", 0, 1);
        test_criteria(idmefv2, "alert.create_time <= '2015-05-03 1:59:07'", 0, 0);
        test_criteria(idmefv2, "alert.create_time >= '2015-05-03 1:59:07'", 0, 1);

        test_criteria(idmefv2, "alert.create_time < '2015-05-03 1:59:09'", 0, 1);
        test_criteria(idmefv2, "alert.create_time > '2015-05-03 1:59:09'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= '2015-05-03 1:59:09'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= '2015-05-03 1:59:09'", 0, 0);
        /*
         * Broken down time check
         */
        assert(idmefv2_time_set_from_string(ctime, "2015-05-04 00:00:00+00:00") == 0);
        test_criteria(idmefv2, "alert.create_time == 'month:may mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != 'month:may mday:3'", 0, 1);
        test_criteria(idmefv2, "alert.create_time < 'month:may mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > 'month:may mday:3'", 0, 1);
        test_criteria(idmefv2, "alert.create_time <= 'month:may mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time >= 'month:may mday:3'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == 'month:may mday:4'", 0, 1);
        test_criteria(idmefv2, "alert.create_time != 'month:may mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time < 'month:may mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > 'month:may mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= 'month:may mday:4'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= 'month:may mday:4'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == 'month:may mday:5'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != 'month:may mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time < 'month:may mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time > 'month:may mday:5'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= 'month:may mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= 'month:may mday:5'", 0, 0);
        /*
         * Broken down time special wday/yday fields
         */
        test_criteria(idmefv2, "alert.create_time == 'wday:monday'", 0, 1);
        test_criteria(idmefv2, "alert.create_time != 'wday:monday'", 0, 0);
        test_criteria(idmefv2, "alert.create_time == 'wday:tuesday'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != 'wday:tuesday'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == 'wday:monday mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != 'wday:monday mday:3'", 0, 1);
        test_criteria(idmefv2, "alert.create_time < 'wday:monday mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > 'wday:monday mday:3'", 0, 1);
        test_criteria(idmefv2, "alert.create_time <= 'wday:monday mday:3'", 0, 0);
        test_criteria(idmefv2, "alert.create_time >= 'wday:monday mday:3'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == 'wday:monday mday:4'", 0, 1);
        test_criteria(idmefv2, "alert.create_time != 'wday:monday mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time < 'wday:monday mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time > 'wday:monday mday:4'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= 'wday:monday mday:4'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= 'wday:monday mday:4'", 0, 1);

        test_criteria(idmefv2, "alert.create_time == 'wday:monday mday:5'", 0, 0);
        test_criteria(idmefv2, "alert.create_time != 'wday:monday mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time < 'wday:monday mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time > 'wday:monday mday:5'", 0, 0);
        test_criteria(idmefv2, "alert.create_time <= 'wday:monday mday:5'", 0, 1);
        test_criteria(idmefv2, "alert.create_time >= 'wday:monday mday:5'", 0, 0);
        /*
         * Test on listed object without specific index
         */
        test_criteria(idmefv2, "alert.analyzer(*).name == 'A'", 0, 1);
        test_criteria(idmefv2, "alert.analyzer(*).name != 'A'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).name == 'NOT EXIST'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).ostype == 'C'", 0, 1);
        test_criteria(idmefv2, "alert.analyzer(*).ostype != 'C'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).ostype == 'NOT EXIST'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).ostype != 'NOT EXIST'", 0, 1);
        test_criteria(idmefv2, "alert.analyzer(*).class", 0, 0);
        test_criteria(idmefv2, "! alert.analyzer(*).class", 0, 1);
        test_criteria(idmefv2, "alert.analyzer(*).name", 0, 1);
        test_criteria(idmefv2, "! alert.analyzer(*).name", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).ostype", 0, 1);
        test_criteria(idmefv2, "! alert.analyzer(*).ostype", 0, 0);
        test_criteria(idmefv2, "Alert.source", 0, 0);
        test_criteria(idmefv2, "! Alert.source", 0, 1);

        test_criteria(idmefv2, "Alert.analyzer", 0, 1);
        test_criteria(idmefv2, "! Alert.analyzer", 0, 0);

        test_criteria(idmefv2, "Alert.analyzer(*).process.arg", 0, 1);
        test_criteria(idmefv2, "! Alert.analyzer(*).process.arg", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).process.arg == 'a0'", 0, 1);
        test_criteria(idmefv2, "alert.analyzer(*).process.arg != 'a0'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).process.arg == 'NOT EXIST'", 0, 0);
        test_criteria(idmefv2, "alert.analyzer(*).process.arg != 'NOT EXIST'", 0, 1);
        idmefv2_message_destroy(idmefv2);
        exit(0);
}
