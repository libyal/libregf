/*
 * Data block key functions
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

#if !defined( _LIBREGF_DATA_BLOCK_KEY_H )
#define _LIBREGF_DATA_BLOCK_KEY_H

#include <common.h>
#include <types.h>

#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_data_block_key libregf_data_block_key_t;

struct libregf_data_block_key
{
	/* The number of segments
	 */
	uint16_t number_of_segments;

	/* The data block list offset
	 */
	uint32_t data_block_list_offset;
};

int libregf_data_block_key_initialize(
     libregf_data_block_key_t **data_block_key,
     libcerror_error_t **error );

int libregf_data_block_key_free(
     libregf_data_block_key_t **data_block_key,
     libcerror_error_t **error );

int libregf_data_block_key_read_data(
     libregf_data_block_key_t *data_block_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_DATA_BLOCK_KEY_H ) */

