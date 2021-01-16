/*
 * Data block stream functions
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

#if !defined( _LIBREGF_DATA_BLOCK_STREAM_H )
#define _LIBREGF_DATA_BLOCK_STREAM_H

#include <common.h>
#include <types.h>

#include "libregf_libbfio.h"
#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

ssize_t libregf_data_block_stream_read_segment_data(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags,
         uint8_t read_flags,
         libcerror_error_t **error );

off64_t libregf_data_block_stream_seek_segment_offset(
         intptr_t *data_handle,
         libbfio_handle_t *file_io_handle,
         int segment_index,
         int segment_file_index,
         off64_t segment_offset,
         libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_DATA_BLOCK_STREAM_H ) */

