/*
 * Input/Output (IO) handle
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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libregf_checksum.h"
#include "libregf_codepage.h"
#include "libregf_data_type.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_hive_bin.h"
#include "libregf_io_handle.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfdatetime.h"
#include "libregf_unused.h"

#include "regf_file_header.h"

const char *regf_file_signature = "regf";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_io_handle_initialize(
     libregf_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libregf_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libregf_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libregf_io_handle_free(
     libregf_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears an IO handle
 * Returns 1 if successful or -1 on error
 */
int libregf_io_handle_clear(
     libregf_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libregf_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;

	return( 1 );
}

/* Reads the segment data into the buffer
 * Callback function for the value item data stream
 * Returns the number of bytes read or -1 on error
 */
ssize_t libregf_io_handle_read_segment_data(
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
	static char *function = "libregf_io_handle_read_segment_data";
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
off64_t libregf_io_handle_seek_segment_offset(
         intptr_t *data_handle LIBREGF_ATTRIBUTE_UNUSED,
         libbfio_handle_t *file_io_handle,
         int segment_index LIBREGF_ATTRIBUTE_UNUSED,
         int segment_file_index LIBREGF_ATTRIBUTE_UNUSED,
         off64_t segment_offset,
         libcerror_error_t **error )
{
	static char *function = "libregf_io_handle_seek_segment_offset";

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

