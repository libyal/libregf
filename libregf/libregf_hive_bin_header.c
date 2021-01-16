/*
 * Hive bin header functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libregf_debug.h"
#include "libregf_hive_bin_header.h"
#include "libregf_io_handle.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfdatetime.h"

#include "regf_hive_bin.h"

const char *regf_hive_bin_signature = "hbin";

/* Creates hive bin header
 * Make sure the value hive_bin_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_header_initialize(
     libregf_hive_bin_header_t **hive_bin_header,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_header_initialize";

	if( hive_bin_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin header.",
		 function );

		return( -1 );
	}
	if( *hive_bin_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid hive bin header value already set.",
		 function );

		return( -1 );
	}
	*hive_bin_header = memory_allocate_structure(
	                    libregf_hive_bin_header_t );

	if( *hive_bin_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create hive bin header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *hive_bin_header,
	     0,
	     sizeof( libregf_hive_bin_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hive bin header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *hive_bin_header != NULL )
	{
		memory_free(
		 *hive_bin_header );

		*hive_bin_header = NULL;
	}
	return( -1 );
}

/* Frees hive bin header
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_header_free(
     libregf_hive_bin_header_t **hive_bin_header,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_header_free";

	if( hive_bin_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin header.",
		 function );

		return( -1 );
	}
	if( *hive_bin_header != NULL )
	{
		memory_free(
		 *hive_bin_header );

		*hive_bin_header = NULL;
	}
	return( 1 );
}

/* Reads the hive bin header data
 * Returns 1 if successful, 0 if no hive bin signature was found or -1 on error
 */
int libregf_hive_bin_header_read_data(
     libregf_hive_bin_header_t *hive_bin_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_header_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#endif

	if( hive_bin_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin header.",
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
	if( data_size < sizeof( regf_hive_bin_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: hive bin header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( regf_hive_bin_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (regf_hive_bin_header_t *) data )->signature,
	     regf_hive_bin_signature,
	     4 ) != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_hive_bin_header_t *) data )->hive_bin_offset,
	 hive_bin_header->offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_hive_bin_header_t *) data )->size,
	 hive_bin_header->size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (regf_hive_bin_header_t *) data )->signature[ 0 ],
		 ( (regf_hive_bin_header_t *) data )->signature[ 1 ],
		 ( (regf_hive_bin_header_t *) data )->signature[ 2 ],
		 ( (regf_hive_bin_header_t *) data )->signature[ 3 ] );

		libcnotify_printf(
		 "%s: hive bin offset\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 hive_bin_header->offset,
		 hive_bin_header->offset );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 " bytes\n",
		 function,
		 hive_bin_header->size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_hive_bin_header_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_hive_bin_header_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		if( libregf_debug_print_filetime_value(
		     function,
		     "unknown time\t\t\t\t",
		     ( (regf_hive_bin_header_t *) data )->unknown_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print filetime value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_hive_bin_header_t *) data )->unknown_spare,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown spare\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads the hive bin header
 * Returns 1 if successful, 0 if no hive bin signature was found or -1 on error
 */
int libregf_hive_bin_header_read_file_io_handle(
     libregf_hive_bin_header_t *hive_bin_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t hive_bin_header_data[ sizeof( regf_hive_bin_header_t ) ];

	static char *function = "libregf_hive_bin_header_read_file_io_handle";
	ssize_t read_count    = 0;
	int result            = 0;

	if( hive_bin_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading hive bin header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              hive_bin_header_data,
	              sizeof( regf_hive_bin_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( regf_hive_bin_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hive bin header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          hive_bin_header_data,
	          sizeof( regf_hive_bin_header_t ),
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hive bin header.",
		 function );

		return( -1 );
	}
	return( result );
}

