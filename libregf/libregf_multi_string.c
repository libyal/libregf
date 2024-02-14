/*
 * Multi string functions
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
#include <memory.h>
#include <types.h>

#include "libregf_definitions.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libuna.h"
#include "libregf_multi_string.h"
#include "libregf_types.h"

/* Creates multi string
 * Make sure the value multi_string is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_initialize(
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_initialize";

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	if( *multi_string != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid multi string value already set.",
		 function );

		return( -1 );
	}
	internal_multi_string = memory_allocate_structure(
	                         libregf_internal_multi_string_t );

	if( internal_multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create multi string.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_multi_string,
	     0,
	     sizeof( libregf_internal_multi_string_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear multi string.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_multi_string->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	*multi_string = (libregf_multi_string_t *) internal_multi_string;

	return( 1 );

on_error:
	if( internal_multi_string != NULL )
	{
		memory_free(
		 internal_multi_string );
	}
	return( -1 );
}

/* Frees multi string
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_free(
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_free";
	int result                                             = 1;

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	if( *multi_string != NULL )
	{
		internal_multi_string = (libregf_internal_multi_string_t *) *multi_string;
		*multi_string         = NULL;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_multi_string->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		if( internal_multi_string->string_sizes != NULL )
		{
			memory_free(
			 internal_multi_string->string_sizes );
		}
		if( internal_multi_string->strings != NULL )
		{
			memory_free(
			 internal_multi_string->strings );
		}
		if( internal_multi_string->data != NULL )
		{
			memory_free(
			 internal_multi_string->data );
		}
		memory_free(
		 internal_multi_string );
	}
	return( result );
}

/* Reads the multi string data
 * Returns 1 if successful or -1 on error
 */
int libregf_internal_multi_string_read_data(
     libregf_internal_multi_string_t *internal_multi_string,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	uint8_t *string_start   = NULL;
	static char *function   = "libregf_multi_string_get_number_of_strings";
	size_t data_offset      = 0;
	size_t string_size      = 0;
	int found_end_of_string = 0;
	int number_of_strings   = 0;
	int string_index        = 0;

	if( internal_multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	if( internal_multi_string->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid multi string - data already set.",
		 function );

		return( -1 );
	}
	if( internal_multi_string->strings != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid multi string - strings already set.",
		 function );

		return( -1 );
	}
	if( internal_multi_string->string_sizes != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid multi string - string sizes already set.",
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
	if( ( data_size < 2 )
	 || ( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: multi string data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	data_offset = 0;

	while( data_offset < data_size )
	{
		found_end_of_string = 0;
		string_size         = 0;

		while( data_offset <= ( data_size - 2 ) )
		{
			string_size += 2;

			if( ( data[ data_offset ] == 0 )
			 && ( data[ data_offset + 1 ] == 0 ) )
			{
				data_offset        += 2;
				found_end_of_string = 1;

				break;
			}
			data_offset += 2;
		}
		if( found_end_of_string == 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: multi string: %d missing end-of-string character.\n",
				 function,
				 number_of_strings );
			}
#endif
			internal_multi_string->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

			break;
		}
		if( string_size == 2 )
		{
			break;
		}
		number_of_strings++;
	}
	if( ( data[ data_offset - 2 ] != 0 )
	 || ( data[ data_offset - 1 ] != 0 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: multi string without terminating empty string.\n",
			 function );
		}
#endif
		internal_multi_string->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
	}
	internal_multi_string->data = (uint8_t *) memory_allocate(
	                                           sizeof( uint8_t ) * data_size );

	if( internal_multi_string->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     internal_multi_string->data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		goto on_error;
	}
	internal_multi_string->data_size = data_size;

	if( number_of_strings > 0 )
	{
		internal_multi_string->strings = (uint8_t **) memory_allocate(
		                                               sizeof( uint8_t * ) * number_of_strings );

		if( internal_multi_string->strings == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create strings.",
			 function );

			goto on_error;
		}
		internal_multi_string->string_sizes = (size_t *) memory_allocate(
		                                                  sizeof( size_t ) * number_of_strings );

		if( internal_multi_string->string_sizes == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create string sizes.",
			 function );

			goto on_error;
		}
		data_offset = 0;

		while( data_offset < data_size )
		{
			string_start = &( ( internal_multi_string->data )[ data_offset ] );
			string_size  = 0;

			while( data_offset <= ( data_size - 2 ) )
			{
				string_size += 2;

				if( ( data[ data_offset ] == 0 )
				 && ( data[ data_offset + 1 ] == 0 ) )
				{
					data_offset += 2;

					break;
				}
				data_offset += 2;
			}
			if( ( string_index < 0 )
			 || ( string_index >= number_of_strings ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid string index value out of bounds.",
				 function );

				goto on_error;
			}
			internal_multi_string->strings[ string_index ]      = string_start;
			internal_multi_string->string_sizes[ string_index ] = string_size;

			string_index++;

			if( string_index >= number_of_strings )
			{
				break;
			}
		}
	}
	internal_multi_string->number_of_strings = number_of_strings;

	return( 1 );

on_error:
	if( internal_multi_string->string_sizes != NULL )
	{
		memory_free(
		 internal_multi_string->string_sizes );

		internal_multi_string->string_sizes = NULL;
	}
	if( internal_multi_string->strings != NULL )
	{
		memory_free(
		 internal_multi_string->strings );

		internal_multi_string->strings = NULL;
	}
	internal_multi_string->number_of_strings = 0;

	if( internal_multi_string->data != NULL )
	{
		memory_free(
		 internal_multi_string->data );

		internal_multi_string->data = NULL;
	}
	internal_multi_string->data_size = 0;

	return( -1 );
}

/* Retrieves the number of strings
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_get_number_of_strings(
     libregf_multi_string_t *multi_string,
     int *number_of_strings,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_get_number_of_strings";

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	internal_multi_string = (libregf_internal_multi_string_t *) multi_string;

	if( number_of_strings == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of strings.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	*number_of_strings = internal_multi_string->number_of_strings;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the UTF-8 string size of a specific string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_get_utf8_string_size(
     libregf_multi_string_t *multi_string,
     int string_index,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_get_utf8_string_size";
	int result                                             = 1;

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	internal_multi_string = (libregf_internal_multi_string_t *) multi_string;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( string_index < 0 )
	 || ( string_index >= internal_multi_string->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		result = -1;
	}
	else if( libuna_utf8_string_size_from_utf16_stream(
	          internal_multi_string->strings[ string_index ],
	          internal_multi_string->string_sizes[ string_index ],
	          LIBUNA_ENDIAN_LITTLE,
	          utf8_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 encoded string of a specific string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_get_utf8_string(
     libregf_multi_string_t *multi_string,
     int string_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_get_utf8_string";
	int result                                             = 1;

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	internal_multi_string = (libregf_internal_multi_string_t *) multi_string;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( string_index < 0 )
	 || ( string_index >= internal_multi_string->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		result = -1;
	}
	else if( libuna_utf8_string_copy_from_utf16_stream(
	          utf8_string,
	          utf8_string_size,
	          internal_multi_string->strings[ string_index ],
	          internal_multi_string->string_sizes[ string_index ],
	          LIBUNA_ENDIAN_LITTLE,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 string size of a specific string
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_get_utf16_string_size(
     libregf_multi_string_t *multi_string,
     int string_index,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_get_utf16_string_size";
	int result                                             = 1;

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	internal_multi_string = (libregf_internal_multi_string_t *) multi_string;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( string_index < 0 )
	 || ( string_index >= internal_multi_string->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		result = -1;
	}
	else if( libuna_utf16_string_size_from_utf16_stream(
	          internal_multi_string->strings[ string_index ],
	          internal_multi_string->string_sizes[ string_index ],
	          LIBUNA_ENDIAN_LITTLE,
	          utf16_string_size,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 encoded string of a specific string
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_multi_string_get_utf16_string(
     libregf_multi_string_t *multi_string,
     int string_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_multi_string_t *internal_multi_string = NULL;
	static char *function                                  = "libregf_multi_string_get_utf16_string";
	int result                                             = 1;

	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	internal_multi_string = (libregf_internal_multi_string_t *) multi_string;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	if( ( string_index < 0 )
	 || ( string_index >= internal_multi_string->number_of_strings ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		result = -1;
	}
	else if( libuna_utf16_string_copy_from_utf16_stream(
	          utf16_string,
	          utf16_string_size,
	          internal_multi_string->strings[ string_index ],
	          internal_multi_string->string_sizes[ string_index ],
	          LIBUNA_ENDIAN_LITTLE,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_multi_string->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for reading.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

