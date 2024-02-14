/*
 * Dirty vector functions
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

#if !defined( _LIBREGF_DIRTY_VECTOR_H )
#define _LIBREGF_DIRTY_VECTOR_H

#include <common.h>
#include <types.h>

#include "libregf_libbfio.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_dirty_vector libregf_dirty_vector_t;

struct libregf_dirty_vector
{
	/* The major format version
	 */
	uint32_t major_format_version;
};

int libregf_dirty_vector_initialize(
     libregf_dirty_vector_t **dirty_vector,
     libcerror_error_t **error );

int libregf_dirty_vector_free(
     libregf_dirty_vector_t **dirty_vector,
     libcerror_error_t **error );

int libregf_dirty_vector_read_data(
     libregf_dirty_vector_t *dirty_vector,
     const uint8_t *data,
     size_t data_size,
     size_t dirty_page_bitmap_size,
     libcerror_error_t **error );

int libregf_dirty_vector_read_file_io_handle(
     libregf_dirty_vector_t *dirty_vector,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t hive_bins_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_DIRTY_VECTOR_H ) */

