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

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libregf_data_block_key.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_unused.h"

#include "regf_cell_values.h"

/* Creates a data block key
 * Make sure the value data_block_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_data_block_key_initialize(
     libregf_data_block_key_t **data_block_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_data_block_key_initialize";

	if( data_block_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block key.",
		 function );

		return( -1 );
	}
	if( *data_block_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block key value already set.",
		 function );

		return( -1 );
	}
	*data_block_key = memory_allocate_structure(
	                   libregf_data_block_key_t );

	if( *data_block_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_block_key,
	     0,
	     sizeof( libregf_data_block_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data block key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_block_key != NULL )
	{
		memory_free(
		 *data_block_key );

		*data_block_key = NULL;
	}
	return( -1 );
}

/* Frees a data block key
 * Returns 1 if successful or -1 on error
 */
int libregf_data_block_key_free(
     libregf_data_block_key_t **data_block_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_data_block_key_free";

	if( data_block_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block key.",
		 function );

		return( -1 );
	}
	if( *data_block_key != NULL )
	{
		memory_free(
		 *data_block_key );

		*data_block_key = NULL;
	}
	return( 1 );
}

/* Reads a data block key
 * Returns 1 if successful or -1 on error
 */
int libregf_data_block_key_read_data(
     libregf_data_block_key_t *data_block_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_data_block_key_read_data";

	if( data_block_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block key.",
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
	if( data_size < sizeof( regf_data_block_key_t ) )
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
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data block key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	/* Check if the signature matches that of a data block key: "db"
	 */
	if( ( data[ 0 ] != (uint8_t) 'd' )
	 || ( data[ 1 ] != (uint8_t) 'b' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data block key signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_data_block_key_t *) data )->number_of_segments,
	 data_block_key->number_of_segments );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_data_block_key_t *) data )->data_block_list_offset,
	 data_block_key->data_block_list_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c\n",
		 function,
		 ( (regf_data_block_key_t *) data )->signature[ 0 ],
		 ( (regf_data_block_key_t *) data )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: number of segments\t\t\t\t: %" PRIu16 "\n",
		 function,
		 data_block_key->number_of_segments );

		libcnotify_printf(
		 "%s: data block list offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 data_block_key->data_block_list_offset );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

