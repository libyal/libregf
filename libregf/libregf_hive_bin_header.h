/*
 * Hive bin header functions
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

#if !defined( _LIBREGF_HIVE_BIN_HEADER_H )
#define _LIBREGF_HIVE_BIN_HEADER_H

#include <common.h>
#include <types.h>

#include "libregf_libbfio.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_hive_bin_header libregf_hive_bin_header_t;

struct libregf_hive_bin_header
{
	/* The offset of the hive bin
	 */
	uint32_t offset;

	/* The size of the hive bin
	 */
	uint32_t size;
};

int libregf_hive_bin_header_initialize(
     libregf_hive_bin_header_t **hive_bin_header,
     libcerror_error_t **error );

int libregf_hive_bin_header_free(
     libregf_hive_bin_header_t **hive_bin_header,
     libcerror_error_t **error );

int libregf_hive_bin_header_read_data(
     libregf_hive_bin_header_t *hive_bin_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libregf_hive_bin_header_read_file_io_handle(
     libregf_hive_bin_header_t *hive_bin_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_HIVE_BIN_HEADER_H ) */

