/*
 * Named key functions
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

#if !defined( _LIBREGF_NAMED_KEY_H )
#define _LIBREGF_NAMED_KEY_H

#include <common.h>
#include <types.h>

#include "libregf_io_handle.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_named_key libregf_named_key_t;

struct libregf_named_key
{
	/* The name hash
	 */
	uint32_t name_hash;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The last written time
	 */
	uint64_t last_written_time;

	/* The flags
	 */
	uint16_t flags;

	/* The number of sub keys
	 */
	uint32_t number_of_sub_keys;

	/* The sub keys list offset
	 */
	uint32_t sub_keys_list_offset;

	/* The number of values
	 */
	uint32_t number_of_values;

	/* The values list offset
	 */
	uint32_t values_list_offset;

	/* The security key offset
	 */
	uint32_t security_key_offset;

	/* The class name offset
	 */
	uint32_t class_name_offset;

	/* The class name size
	 */
	uint16_t class_name_size;
};

int libregf_named_key_initialize(
     libregf_named_key_t **named_key,
     libcerror_error_t **error );

int libregf_named_key_free(
     libregf_named_key_t **named_key,
     libcerror_error_t **error );

int libregf_named_key_read_data(
     libregf_named_key_t *named_key,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     uint32_t named_key_hash,
     libcerror_error_t **error );

int libregf_named_key_get_name_size(
     libregf_named_key_t *named_key,
     size_t *name_size,
     libcerror_error_t **error );

int libregf_named_key_get_name(
     libregf_named_key_t *named_key,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libregf_named_key_get_utf8_name_size(
     libregf_named_key_t *named_key,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_get_utf8_name(
     libregf_named_key_t *named_key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_get_utf16_name_size(
     libregf_named_key_t *named_key,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_get_utf16_name(
     libregf_named_key_t *named_key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_compare_name_with_utf8_string(
     libregf_named_key_t *named_key,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_compare_name_with_utf16_string(
     libregf_named_key_t *named_key,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_named_key_get_last_written_time(
     libregf_named_key_t *named_key,
     uint64_t *filetime,
     libcerror_error_t **error );

int libregf_named_key_get_number_of_sub_keys(
     libregf_named_key_t *named_key,
     uint32_t *number_of_sub_keys,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_NAMED_KEY_H ) */

