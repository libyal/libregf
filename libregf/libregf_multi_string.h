/*
 * Multi string functions
 *
 * Copyright (C) 2009-2021, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined( _LIBREGF_MULTI_STRING_H )
#define _LIBREGF_MULTI_STRING_H

#include <common.h>
#include <types.h>

#include "libregf_extern.h"
#include "libregf_libcerror.h"
#include "libregf_libcthreads.h"
#include "libregf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_internal_multi_string libregf_internal_multi_string_t;

struct libregf_internal_multi_string
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The number of strings
	 */
	int number_of_strings;

	/* The strings
	 */
	uint8_t **strings;

	/* The string sizes
	 */
	size_t *string_sizes;

	/* Various item flags
	 */
	uint8_t item_flags;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libregf_multi_string_initialize(
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_free(
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error );

int libregf_internal_multi_string_read_data(
     libregf_internal_multi_string_t *internal_multi_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_get_number_of_strings(
     libregf_multi_string_t *multi_string,
     int *number_of_strings,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_get_utf8_string_size(
     libregf_multi_string_t *multi_string,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_get_utf8_string(
     libregf_multi_string_t *multi_string,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_get_utf16_string_size(
     libregf_multi_string_t *multi_string,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_multi_string_get_utf16_string(
     libregf_multi_string_t *multi_string,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_MULTI_STRING_H ) */

