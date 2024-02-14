/*
 * Value key functions
 *
 * Copyright (C) 2009-2024, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBREGF_VALUE_KEY_H )
#define _LIBREGF_VALUE_KEY_H

#include <common.h>
#include <types.h>

#include "libregf_io_handle.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_value_key libregf_value_key_t;

struct libregf_value_key
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

	/* The value type
	 */
	uint32_t value_type;

	/* The flags
	 */
	uint16_t flags;

	/* The value data offset
	 */
	uint32_t data_offset;

	/* The value data
	 */
	uint8_t *data;

	/* The value data size
	 */
	uint32_t data_size;

	/* Value to indicate the value data is stored in the key
	 */
	uint8_t data_in_key;
};

int libregf_value_key_initialize(
     libregf_value_key_t **value_key,
     libcerror_error_t **error );

int libregf_value_key_free(
     libregf_value_key_t **value_key,
     libcerror_error_t **error );

int libregf_value_key_clone(
     libregf_value_key_t **destination_value_key,
     libregf_value_key_t *source_value_key,
     libcerror_error_t **error );

int libregf_value_key_read_data(
     libregf_value_key_t *value_key,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     uint32_t value_key_hash,
     libcerror_error_t **error );

int libregf_value_key_get_name_size(
     libregf_value_key_t *value_key,
     size_t *name_size,
     libcerror_error_t **error );

int libregf_value_key_get_name(
     libregf_value_key_t *value_key,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libregf_value_key_get_utf8_name_size(
     libregf_value_key_t *value_key,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_get_utf8_name(
     libregf_value_key_t *value_key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_get_utf16_name_size(
     libregf_value_key_t *value_key,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_get_utf16_name(
     libregf_value_key_t *value_key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_compare_name_with_utf8_string(
     libregf_value_key_t *value_key,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_compare_name_with_utf16_string(
     libregf_value_key_t *value_key,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libregf_value_key_get_value_type(
     libregf_value_key_t *value_key,
     uint32_t *value_type,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_VALUE_KEY_H ) */

