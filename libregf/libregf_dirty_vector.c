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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libregf_checksum.h"
#include "libregf_debug.h"
#include "libregf_dirty_vector.h"
#include "libregf_io_handle.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfdatetime.h"

/* Creates a dirty vector
 * Make sure the value dirty_vector is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_dirty_vector_initialize(
     libregf_dirty_vector_t **dirty_vector,
     libcerror_error_t **error )
{
	static char *function = "libregf_dirty_vector_initialize";

	if( dirty_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid dirty vector.",
		 function );

		return( -1 );
	}
	if( *dirty_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid dirty vector value already set.",
		 function );

		return( -1 );
	}
	*dirty_vector = memory_allocate_structure(
	                 libregf_dirty_vector_t );

	if( *dirty_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create dirty vector.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *dirty_vector,
	     0,
	     sizeof( libregf_dirty_vector_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear dirty vector.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *dirty_vector != NULL )
	{
		memory_free(
		 *dirty_vector );

		*dirty_vector = NULL;
	}
	return( -1 );
}

/* Frees a dirty vector
 * Returns 1 if successful or -1 on error
 */
int libregf_dirty_vector_free(
     libregf_dirty_vector_t **dirty_vector,
     libcerror_error_t **error )
{
	static char *function = "libregf_dirty_vector_free";

	if( dirty_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid dirty vector.",
		 function );

		return( -1 );
	}
	if( *dirty_vector != NULL )
	{
		memory_free(
		 *dirty_vector );

		*dirty_vector = NULL;
	}
	return( 1 );
}

/* Reads the dirty vector data
 * Returns 1 if successful or -1 on error
 */
int libregf_dirty_vector_read_data(
     libregf_dirty_vector_t *dirty_vector,
     const uint8_t *data,
     size_t data_size,
     size_t dirty_page_bitmap_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_dirty_vector_read_data";

	if( dirty_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid dirty vector.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < 4 )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( dirty_page_bitmap_size > data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid dirty page bitmap size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: dirty vector data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     data,
	     "DIRT",
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid dirty vectory signature.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 data[ 0 ],
		 data[ 1 ],
		 data[ 2 ],
		 data[ 3 ] );

		libcnotify_printf(
		 "%s: dirty page bitmap:\n",
		 function );
		libcnotify_print_data(
		 &( data[ 4 ] ),
		 dirty_page_bitmap_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: padding:\n",
		 function );
		libcnotify_print_data(
		 &( data[ 4 + dirty_page_bitmap_size ] ),
		 data_size - ( 4 + dirty_page_bitmap_size ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the dirty vector
 * Returns 1 if successful or -1 on error
 */
int libregf_dirty_vector_read_file_io_handle(
     libregf_dirty_vector_t *dirty_vector,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     uint32_t hive_bins_size,
     libcerror_error_t **error )
{
	uint8_t *dirty_vector_data    = NULL;
	static char *function         = "libregf_dirty_vector_read_file_io_handle";
	size_t dirty_page_bitmap_size = 0;
	size_t dirty_vector_data_size = 0;
	ssize_t read_count            = 0;

	if( dirty_vector == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid dirty vector.",
		 function );

		return( -1 );
	}
	dirty_page_bitmap_size = (size_t) ( hive_bins_size / ( 512 * 8 ) );

	if( hive_bins_size % ( 512 * 8 ) != 0 )
	{
		dirty_page_bitmap_size += 1;
	}
	dirty_vector_data_size = dirty_page_bitmap_size + 4;

	if( ( dirty_vector_data_size % 512 ) != 0 )
	{
		dirty_vector_data_size = ( ( dirty_vector_data_size / 512 ) + 1 ) * 512;
	}
	if( dirty_vector_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid dirty vector data size value exceeds maximum allocation size.",
		 function );

		goto on_error;
	}
	dirty_vector_data = (uint8_t *) memory_allocate(
	                                 sizeof( uint8_t ) * dirty_vector_data_size );

	if( dirty_vector_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create dirty vector data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading dirty vector at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              dirty_vector_data,
	              dirty_vector_data_size,
	              file_offset,
	              error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read dirty vector data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		goto on_error;
	}
	else if( read_count == (ssize_t) dirty_vector_data_size )
	{
		/* In some transaction files the dirty vector is 512 bytes and
		 * no bit in the dirty page bitmap is set.
		 */
		if( libregf_dirty_vector_read_data(
		     dirty_vector,
		     dirty_vector_data,
		     dirty_vector_data_size,
		     dirty_page_bitmap_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read dirty vector.",
			 function );

			goto on_error;
		}
	}
	memory_free(
	 dirty_vector_data );

	return( 1 );

on_error:
	if( dirty_vector_data != NULL )
	{
		memory_free(
		 dirty_vector_data );
	}
	return( -1 );
}

