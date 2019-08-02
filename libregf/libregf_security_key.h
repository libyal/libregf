/*
 * Security key functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBREGF_SECURITY_KEY_H )
#define _LIBREGF_SECURITY_KEY_H

#include <common.h>
#include <types.h>

#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_security_key libregf_security_key_t;

struct libregf_security_key
{
	/* The security descriptor
	 */
	uint8_t *security_descriptor;

	/* The security descriptor size
	 */
	size_t security_descriptor_size;
};

int libregf_security_key_initialize(
     libregf_security_key_t **security_key,
     libcerror_error_t **error );

int libregf_security_key_free(
     libregf_security_key_t **security_key,
     libcerror_error_t **error );

int libregf_security_key_read_data(
     libregf_security_key_t *security_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_SECURITY_KEY_H ) */
