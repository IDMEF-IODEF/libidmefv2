#include <stdio.h>
#include <stdarg.h>

#include "libidmefv2-log.h"
#include "libidmefv2-error.h"


void libidmefv2_perror(libidmefv2_error_t error, const char *fmt, ...)
{
	va_list ap;
	char buf[1024];
        
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

        if ( libidmefv2_error_get_source(error) )
                libidmefv2_log(LIBIDMEFV2_LOG_WARN, "%s: %s: %s.\n", libidmefv2_strsource(error), buf, libidmefv2_strerror(error));
        else
                libidmefv2_log(LIBIDMEFV2_LOG_WARN, "%s: %s.\n", buf, libidmefv2_strerror(error));
}
