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

#include <common.h>
#include <types.h>

#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_unused.h"

/* Reads the segment data into the buffer
 * Callback function for the value item data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libregf_data_block_stream_read_segment_data(
         intptr_t *data_handle LIBREGF_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBREGF_ATTRIBUTE_UNUSED,
         int segment_file_index LIBREGF_ATTRIBUTE_UNUSED,
         uint8_t *segment_data,
         size_t segment_data_size,
         uint32_t segment_flags LIBREGF_ATTRIBUTE_UNUSED,
         uint8_t read_flags LIBREGF_ATTRIBUTE_UNUSED,
         libcerror_error_t **error )
{
	static char *function = "libregf_data_block_stream_read_segment_data";
	ssize_t read_count    = 0;

	LIBREGF_UNREFERENCED_PARAMETER( data_handle )
	LIBREGF_UNREFERENCED_PARAMETER( segment_index )
	LIBREGF_UNREFERENCED_PARAMETER( segment_file_index )
	LIBREGF_UNREFERENCED_PARAMETER( segment_flags )
	LIBREGF_UNREFERENCED_PARAMETER( read_flags )

	read_count = libbfio_handle_read_buffer(
		      file_io_handle,
		      segment_data,
		      segment_data_size,
		      error );

	if( read_count != (ssize_t) segment_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read segment data.",
		 function );

		return( -1 );
	}
	return( read_count );
}

/* Seeks a certain segment offset
 * Callback function for the value item data stream
 * Returns the offset or -1 on error
 */
off64_t libregf_data_block_stream_seek_segment_offset(
         intptr_t *data_handle LIBREGF_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBREGF_ATTRIBUTE_UNUSED,
         int segment_file_index LIBREGF_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libregf_data_block_stream_seek_segment_offset";

	LIBREGF_UNREFERENCED_PARAMETER( data_handle )
	LIBREGF_UNREFERENCED_PARAMETER( segment_index )
	LIBREGF_UNREFERENCED_PARAMETER( segment_file_index )

	segment_offset = libbfio_handle_seek_offset(
	                  file_io_handle,
	                  segment_offset,
	                  SEEK_SET,
	                  error );

	if( segment_offset == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to seek segment offset.",
		 function );

		return( -1 );
	}
	return( segment_offset );
}

