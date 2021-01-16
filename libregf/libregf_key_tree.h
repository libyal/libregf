/*
 * Key tree functions
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

#if !defined( _LIBREGF_KEY_TREE_H )
#define _LIBREGF_KEY_TREE_H

#include <common.h>
#include <types.h>

#include "libregf_hive_bins_list.h"
#include "libregf_io_handle.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libregf_key_tree_get_sub_key_by_utf8_path(
     libregf_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t key_offset,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error );

int libregf_key_tree_get_sub_key_by_utf16_path(
     libregf_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t key_offset,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_KEY_TREE_H ) */

