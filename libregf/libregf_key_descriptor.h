/*
 * Key descriptor functions
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

#if !defined( _LIBREGF_KEY_DESCRIPTOR_H )
#define _LIBREGF_KEY_DESCRIPTOR_H

#include <common.h>
#include <types.h>

#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_key_descriptor libregf_key_descriptor_t;

struct libregf_key_descriptor
{
	/* The key offset
	 */
	uint32_t key_offset;

	/* The hash value
	 */
	uint32_t hash_value;
};

int libregf_key_descriptor_initialize(
     libregf_key_descriptor_t **key_descriptor,
     libcerror_error_t **error );

int libregf_key_descriptor_free(
     libregf_key_descriptor_t **key_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_KEY_DESCRIPTOR_H ) */

