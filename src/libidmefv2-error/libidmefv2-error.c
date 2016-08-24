#include "config.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <gettext.h>
#include <errno.h>

#include "libidmefv2-log.h"
#include "libidmefv2-error.h"

#include "code-to-errno.h"
#include "code-from-errno.h"


#define LIBIDMEFV2_ERROR_CODE_DIM     65536
#define LIBIDMEFV2_ERROR_SOURCE_DIM     256

#define LIBIDMEFV2_ERROR_SOURCE_SHIFT    23
#define LIBIDMEFV2_ERROR_VERBOSE_SHIFT   22

#define LIBIDMEFV2_ERROR_CODE_MASK       (LIBIDMEFV2_ERROR_CODE_DIM - 1)
#define LIBIDMEFV2_ERROR_SOURCE_MASK     (LIBIDMEFV2_ERROR_SOURCE_DIM - 1)
#define LIBIDMEFV2_ERROR_VERBOSE_MASK    (1)


/**
 * libidmefv2_error_make:
 * @source: Error source.
 * @code: Error code.
 *
 * Create a new #libidmefv2_error_t error using @source and @code.
 *
 * Returns: The created #libidmefv2_error_t integer.
 */
libidmefv2_error_t libidmefv2_error_make(libidmefv2_error_source_t source, libidmefv2_error_code_t code)
{
        return (code == LIBIDMEFV2_ERROR_NO_ERROR) ? code : -((source << LIBIDMEFV2_ERROR_SOURCE_SHIFT) | code);
}


/**
 * libidmefv2_error_make_from_errno:
 * @source: Error source.
 * @err: errno value.
 *
 * Create a new #libidmefv2_error_t error using @source and @errno.
 *
 * Returns: The created #libidmefv2_error_t integer.
 */
libidmefv2_error_t libidmefv2_error_make_from_errno(libidmefv2_error_source_t source, int err)
{
        libidmefv2_error_code_t code = libidmefv2_error_code_from_errno(err);
        return libidmefv2_error_make(source, code);
}



/**
 * libidmefv2_error_verbose_make_v:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @ap: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libidmefv2_error_t integer.
 */
libidmefv2_error_t libidmefv2_error_verbose_make_v(libidmefv2_error_source_t source,
                                             libidmefv2_error_code_t code, const char *fmt, va_list ap)
{
        int ret;

        ret = libidmefv2_error_make(source, code);
        ret = -ret;
        ret |= (1 << LIBIDMEFV2_ERROR_VERBOSE_SHIFT);

        return -ret;
}



/**
 * libidmefv2_error_verbose_make:
 * @source: Error source.
 * @code: Error code.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Create a new error using @source and @code, using the detailed error message
 * specified within @fmt.
 *
 * Returns: The created #libidmefv2_error_t integer.
 */
libidmefv2_error_t libidmefv2_error_verbose_make(libidmefv2_error_source_t source,
                                           libidmefv2_error_code_t code, const char *fmt, ...)
{
        int ret;
        va_list ap;

        va_start(ap, fmt);
        ret = libidmefv2_error_verbose_make_v(source, code, fmt, ap);
        va_end(ap);

        return ret;
}


/**
 * libidmefv2_error_get_code:
 * @error: A #libidmefv2_error_t return value.
 *
 * Returns: the #libidmefv2_code_t code contained within the @libidmefv2_error_t integer.
 */
libidmefv2_error_code_t libidmefv2_error_get_code(libidmefv2_error_t error)
{
        error = -error;
        return (libidmefv2_error_code_t) (error & LIBIDMEFV2_ERROR_CODE_MASK);
}


/**
 * libidmefv2_error_get_source:
 * @error: A #libidmefv2_error_t return value.
 *
 * Returns: the #libidmefv2_source_t source contained within the @libidmefv2_error_t integer.
 */
libidmefv2_error_source_t libidmefv2_error_get_source(libidmefv2_error_t error)
{
        error = -error;
        return (libidmefv2_error_source_t) ((error >> LIBIDMEFV2_ERROR_SOURCE_SHIFT) & LIBIDMEFV2_ERROR_SOURCE_MASK);
}


/**
 * libidmefv2_error_is_verbose:
 * @error: A #libidmefv2_error_t return value.
 *
 * Returns: #LIBIDMEFV2_BOOL_TRUE if there is a detailed message for this error, #LIBIDMEFV2_BOOL_FALSE otherwise.
 */
libidmefv2_bool_t libidmefv2_error_is_verbose(libidmefv2_error_t error)
{
        error = -error;
        return ((error >> LIBIDMEFV2_ERROR_VERBOSE_SHIFT) & LIBIDMEFV2_ERROR_VERBOSE_MASK) ? LIBIDMEFV2_BOOL_TRUE : LIBIDMEFV2_BOOL_FALSE;
}


/**
 * libidmefv2_error_code_from_errno:
 * @err: errno value.
 *
 * Returns: the #libidmefv2_error_code_t value corresponding to @err.
 */
libidmefv2_error_code_t libidmefv2_error_code_from_errno(int err)
{
        int idx;

        if ( ! err )
                return LIBIDMEFV2_ERROR_NO_ERROR;

        idx = errno_to_idx(err);
        if ( idx < 0 )
                return LIBIDMEFV2_ERROR_UNKNOWN_ERRNO;

        return LIBIDMEFV2_ERROR_SYSTEM_ERROR | err_code_from_index[idx];
}


/**
 * libidmefv2_error_code_to_errno:
 * @code: Error code.
 *
 * Returns: the errno value corresponding to @code.
 */
int libidmefv2_error_code_to_errno(libidmefv2_error_code_t code)
{
        if ( ! (code & LIBIDMEFV2_ERROR_SYSTEM_ERROR) )
                return 0;

        code &= ~LIBIDMEFV2_ERROR_SYSTEM_ERROR;

        if ( code < sizeof(err_code_to_errno) / sizeof(err_code_to_errno[0]) )
                return err_code_to_errno[code];
        else
                return 0;
}



/**
 * libidmefv2_perror:
 * @error: A #libidmefv2_error_t return value.
 * @fmt: Format string.
 * @...: Argument list.
 *
 * Print the error to stderr, or to syslog() in case stderr is unavailable.
 */
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
