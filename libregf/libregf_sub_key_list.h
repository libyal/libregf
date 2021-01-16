/*
 * Sub key list functions
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

#if !defined( _LIBREGF_SUB_KEY_LIST_H )
#define _LIBREGF_SUB_KEY_LIST_H

#include <common.h>
#include <types.h>

#include "libregf_io_handle.h"
#include "libregf_libcdata.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_sub_key_list libregf_sub_key_list_t;

struct libregf_sub_key_list
{
	/* The sub key descriptors 
	 */
	libcdata_array_t *sub_key_descriptors;

	/* Value to indicates the sub key list is at leaf level
	 */
	uint8_t at_leaf_level;
};

int libregf_sub_key_list_initialize(
     libregf_sub_key_list_t **sub_key_list,
     libcerror_error_t **error );

int libregf_sub_key_list_free(
     libregf_sub_key_list_t **sub_key_list,
     libcerror_error_t **error );

int libregf_sub_key_list_read_data(
     libregf_sub_key_list_t *sub_key_list,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_SUB_KEY_LIST_H ) */

