/*
 * Report handle
 *
 * Copyright (C) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "regftools_libcerror.h"
#include "regftools_libclocale.h"
#include "regftools_libcstring.h"
#include "regftools_libfdatetime.h"
#include "regftools_libfguid.h"
#include "regftools_libfwsi.h"
#include "regftools_libregf.h"
#include "regftools_libuna.h"
#include "registry_file.h"
#include "report_handle.h"
#include "shell_items.h"

/* Creates a report handle
 * Make sure the value report_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int report_handle_initialize(
     report_handle_t **report_handle,
     libcerror_error_t **error )
{
	static char *function = "report_handle_initialize";

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( *report_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid report handle value already set.",
		 function );

		return( -1 );
	}
	*report_handle = memory_allocate_structure(
	                  report_handle_t );

	if( *report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create report handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *report_handle,
	     0,
	     sizeof( report_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear report handle.",
		 function );

		goto on_error;
	}
	if( registry_file_initialize(
	     &( ( *report_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create input file.",
		 function );

		goto on_error;
	}
	( *report_handle )->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;
	( *report_handle )->notify_stream  = stdout;

	return( 1 );

on_error:
	if( *report_handle != NULL )
	{
		memory_free(
		 *report_handle );

		*report_handle = NULL;
	}
	return( -1 );
}

/* Frees a report handle
 * Returns 1 if successful or -1 on error
 */
int report_handle_free(
     report_handle_t **report_handle,
     libcerror_error_t **error )
{
	static char *function = "report_handle_free";
	int result            = 1;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( *report_handle != NULL )
	{
		if( registry_file_free(
		     &( ( *report_handle )->input_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file.",
			 function );

			result = -1;
		}
		memory_free(
		 *report_handle );

		*report_handle = NULL;
	}
	return( result );
}

/* Signals the report handle to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int report_handle_signal_abort(
     report_handle_t *report_handle,
     libcerror_error_t **error )
{
	static char *function = "report_handle_signal_abort";

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	report_handle->abort = 1;

	if( report_handle->input_file != NULL )
	{
		if( registry_file_signal_abort(
		     report_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int report_handle_set_ascii_codepage(
     report_handle_t *report_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "report_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = libcstring_system_string_length(
	                 string );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( report_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( report_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Prints the data to the notify stream
 * Returns the number of printed characters if successful or -1 on error
 */
int report_handle_print_data(
     report_handle_t *report_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_print_data";
	size_t byte_iterator  = 0;
	size_t data_iterator  = 0;
	int print_count       = 0;
	int total_print_count = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		return( 0 );
	}
	while( data_iterator < data_size )
	{
		while( byte_iterator < data_size )
		{
			if( byte_iterator % 16 == 0 )
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       "%.8" PRIzx ": ",
					       byte_iterator );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
			print_count = fprintf(
				       report_handle->notify_stream,
				       "%.2" PRIx8 " ",
				       data[ byte_iterator++ ] );

			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		while( byte_iterator % 16 != 0 )
		{
			byte_iterator++;

			print_count = fprintf(
				       report_handle->notify_stream,
				       "   " );

			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			if( ( byte_iterator % 8 == 0 )
			 && ( byte_iterator % 16 != 0 ) )
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		print_count = fprintf(
			       report_handle->notify_stream,
			       "  " );

		if( print_count <= -1 )
		{
			return( -1 );
		}
		total_print_count += print_count;

		byte_iterator = data_iterator;

		while( byte_iterator < data_size )
		{
			if( ( data[ byte_iterator ] >= 0x20 )
			 && ( data[ byte_iterator ] <= 0x7e ) )
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       "%c",
					       (char) data[ byte_iterator ] );
			}
			else
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       "." );
			}
			if( print_count <= -1 )
			{
				return( -1 );
			}
			total_print_count += print_count;

			byte_iterator++;

			if( byte_iterator % 16 == 0 )
			{
				break;
			}
			else if( byte_iterator % 8 == 0 )
			{
				print_count = fprintf(
					       report_handle->notify_stream,
					       " " );

				if( print_count <= -1 )
				{
					return( -1 );
				}
				total_print_count += print_count;
			}
		}
		print_count = fprintf(
			       report_handle->notify_stream,
			       "\n" );

		if( print_count <= -1 )
		{
			return( -1 );
		}
		total_print_count += print_count;

		data_iterator = byte_iterator;
	}
	print_count = fprintf(
		       report_handle->notify_stream,
		       "\n" );

	if( print_count <= -1 )
	{
		return( -1 );
	}
	total_print_count += print_count;

	return( total_print_count );
}

/* Opens the input file
 * Returns 1 if successful or -1 on error
 */
int report_handle_open_input(
     report_handle_t *report_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "report_handle_open_input";

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( registry_file_set_ascii_codepage(
	     report_handle->input_file,
	     report_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in input file.",
		 function );

		return( -1 );
	}
	if( registry_file_open(
	     report_handle->input_file,
	     filename,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input file
 * Returns the 0 if succesful or -1 on error
 */
int report_handle_close_input(
     report_handle_t *report_handle,
     libcerror_error_t **error )
{
	static char *function = "report_handle_close_input";

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( registry_file_close(
	     report_handle->input_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Retrieves and allocates the name of a key
 * Returns 1 if successful or -1 on error
 */
int report_handle_get_key_name(
     libregf_key_t *key,
     libcstring_system_character_t **name,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_get_key_name";
	int result            = 0;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_key_get_utf16_name_size(
		  key,
		  name_size,
		  error );
#else
	result = libregf_key_get_utf8_name_size(
		  key,
		  name_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key name size.",
		 function );

		goto on_error;
	}
	if( *name_size > 0 )
	{
		if( ( *name_size > (size_t) SSIZE_MAX )
		 || ( ( sizeof( libcstring_system_character_t ) * *name_size ) > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid name size key exceeds maximum.",
			 function );

			goto on_error;
		}
		*name = libcstring_system_string_allocate(
			 *name_size );

		if( *name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_utf16_name(
			  key,
			  (uint16_t *) *name,
			  *name_size,
			  error );
#else
		result = libregf_key_get_utf8_name(
			  key,
			  (uint8_t *) *name,
			  *name_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key name.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *name != NULL )
	{
		memory_free(
		 *name );

		*name = NULL;
	}
	*name_size = 0;

	return( -1 );
}

/* Retrieves and allocates the name of a value
 * Returns 1 if successful or -1 on error
 */
int report_handle_get_value_name(
     libregf_value_t *value,
     libcstring_system_character_t **name,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_get_value_name";
	int result            = 0;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( name_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_value_get_utf16_name_size(
		  value,
		  name_size,
		  error );
#else
	result = libregf_value_get_utf8_name_size(
		  value,
		  name_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value name size.",
		 function );

		goto on_error;
	}
	if( *name_size > 0 )
	{
		if( ( *name_size > (size_t) SSIZE_MAX )
		 || ( ( sizeof( libcstring_system_character_t ) * *name_size ) > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid name size value exceeds maximum.",
			 function );

			goto on_error;
		}
		*name = libcstring_system_string_allocate(
			 *name_size );

		if( *name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name(
			  value,
			  (uint16_t *) *name,
			  *name_size,
			  error );
#else
		result = libregf_value_get_utf8_name(
			  value,
			  (uint8_t *) *name,
			  *name_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *name != NULL )
	{
		memory_free(
		 *name );

		*name = NULL;
	}
	*name_size = 0;

	return( -1 );
}

/* Retrieves and allocates the data of a value
 * Returns 1 if successful or -1 on error
 */
int report_handle_get_value_data(
     libregf_value_t *value,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_get_value_data";
	int result            = 0;

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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	result = libregf_value_get_value_data_size(
		  value,
		  data_size,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data size.",
		 function );

		goto on_error;
	}
	*data = (uint8_t *) memory_allocate(
	                     sizeof( uint8_t ) * *data_size );

	if( *data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	result = libregf_value_get_value_data(
		  value,
		  *data,
		  *data_size,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data != NULL )
	{
		memory_free(
		 *data );

		*data = NULL;
	}
	*data_size = 0;

	return( -1 );
}

/* Retrieves and allocates the data string of a value
 * Returns 1 if successful or -1 on error
 */
int report_handle_get_value_data_string(
     libregf_value_t *value,
     libcstring_system_character_t **data_string,
     size_t *data_string_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_get_value_data_string";
	int result            = 0;

	if( data_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data string.",
		 function );

		return( -1 );
	}
	if( data_string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data string size.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_value_get_value_utf16_string_size(
		  value,
		  data_string_size,
		  error );
#else
	result = libregf_value_get_value_utf8_string_size(
		  value,
		  data_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value string size.",
		 function );

		goto on_error;
	}
	if( *data_string_size > 0 )
	{
		if( ( *data_string_size > (size_t) SSIZE_MAX )
		 || ( ( sizeof( libcstring_system_character_t ) * *data_string_size ) > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid data string size value exceeds maximum.",
			 function );

			goto on_error;
		}
		*data_string = libcstring_system_string_allocate(
				*data_string_size );

		if( *data_string == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data string.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_value_utf16_string(
			  value,
			  (uint16_t *) *data_string,
			  *data_string_size,
			  error );
#else
		result = libregf_value_get_value_utf8_string(
			  value,
			  (uint8_t *) *data_string,
			  *data_string_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value string.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *data_string != NULL )
	{
		memory_free(
		 *data_string );

		*data_string = NULL;
	}
	*data_string_size = 0;

	return( -1 );
}

/* Retrieves and allocates the binary data of a value
 * Returns 1 if successful or -1 on error
 */
int report_handle_get_value_binary_data(
     libregf_value_t *value,
     uint8_t **binary_data,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	static char *function = "report_handle_get_value_binary_data";
	int result            = 0;

	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( binary_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data size.",
		 function );

		return( -1 );
	}
	result = libregf_value_get_value_binary_data_size(
		  value,
		  binary_data_size,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve binary data size.",
		 function );

		goto on_error;
	}
	*binary_data = (uint8_t *) memory_allocate(
			            sizeof( uint8_t ) * *binary_data_size );

	if( *binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create binary data.",
		 function );

		goto on_error;
	}
	result = libregf_value_get_value_binary_data(
		  value,
		  *binary_data,
		  *binary_data_size,
		  error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve binary data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *binary_data != NULL )
	{
		memory_free(
		 *binary_data );

		*binary_data = NULL;
	}
	*binary_data_size = 0;

	return( -1 );
}

/* Analyzes a key containing a certain POSIX time value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int report_handle_analyze_key_with_posix_time_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcerror_error_t **error )
{
	libcstring_system_character_t date_string[ 32 ];

	libfdatetime_posix_time_t *posix_time = NULL;
	libregf_value_t *value                = NULL;
	static char *function                 = "report_handle_analyze_key_with_posix_time_value";
	uint32_t value_32bit                  = 0;
	int result                            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_key_get_value_by_utf16_name(
		  key,
		  (uint16_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#else
	result = libregf_key_get_value_by_utf8_name(
		  key,
		  (uint8_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 value_name );

		goto on_error;
	}
	else if( result != 0 )
	{
		result = libregf_value_get_value_32bit(
			  value,
			  &value_32bit,
			  error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve 32-bit value.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_initialize(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create POSIX time.",
			 function );

			goto on_error;
		}
		if( libfdatetime_posix_time_copy_from_32bit(
		     posix_time,
		     value_32bit,
		     LIBFDATETIME_POSIX_TIME_VALUE_TYPE_SECONDS_32BIT_SIGNED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time from 32-bit value.",
			 function );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libfdatetime_posix_time_copy_to_utf16_string(
			  posix_time,
			  (uint16_t *) date_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#else
		result = libfdatetime_posix_time_copy_to_utf8_string(
			  posix_time,
			  (uint8_t *) date_string,
			  32,
			  LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to copy POSIX time to string.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_name );
		fprintf(
		 report_handle->notify_stream,
		 "\tPOSIX time\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 date_string );

		if( libfdatetime_posix_time_free(
		     &posix_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free POSIX time.",
			 function );

			goto on_error;
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( posix_time != NULL )
	{
		libfdatetime_posix_time_free(
		 &posix_time,
		 NULL );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key containing a certain string value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int report_handle_analyze_key_with_string_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	libregf_value_t *value                      = NULL;
	static char *function                       = "report_handle_analyze_key_with_string_value";
	size_t value_string_size                    = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_key_get_value_by_utf16_name(
		  key,
		  (uint16_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#else
	result = libregf_key_get_value_by_utf8_name(
		  key,
		  (uint8_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 value_name );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( report_handle_get_value_data_string(
		     value,
		     &value_string,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data string.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_name );
		fprintf(
		 report_handle->notify_stream,
		 "\tString\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key containing string values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_key_with_string_values(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_name   = NULL;
	libcstring_system_character_t *value_string = NULL;
	libregf_value_t *value                      = NULL;
	static char *function                       = "report_handle_analyze_key_with_string_values";
	size_t value_name_size                      = 0;
	size_t value_string_size                    = 0;
	int number_of_values                        = 0;
	int value_index                             = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( report_handle_get_value_name(
		     value,
		     &value_name,
		     &value_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name.",
			 function );

			goto on_error;
		}
		if( report_handle_get_value_data_string(
		     value,
		     &value_string,
		     &value_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data string.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_name );
		fprintf(
		 report_handle->notify_stream,
		 "\tString\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 value_string );

		memory_free(
		 value_string );

		value_string = NULL;

		memory_free(
		 value_name );

		value_name = NULL;

		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( value_name != NULL )
	{
		memory_free(
		 value_name );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key containing string values at the sprecified path
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int report_handle_analyze_key_path_with_string_values(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_key_path_with_string_values";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_key_with_string_values(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	else
	{
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( result );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the autorun (Run, RunOnce)
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_autorun(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *key_path = NULL;
	static char *function                   = "report_handle_analyze_autorun";
	size_t key_path_length                  = 0;
	int result                              = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Microsoft\\Windows\\CurrentVersion\\Run" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_key_path_with_string_values(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Microsoft\\Windows\\CurrentVersion\\RunOnce" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_key_path_with_string_values(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Run" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_key_path_with_string_values(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_key_path_with_string_values(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	return( 1 );
}

/* Analyzes a value that contains a shell item
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error )
{
	libfwsi_item_t *shell_item = NULL;
	uint8_t *binary_data       = NULL;
	static char *function      = "report_handle_analyze_shell_item_value";
	size_t binary_data_size    = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_initialize(
	     &shell_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create shell item.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_copy_from_byte_stream(
	     shell_item,
	     binary_data,
	     binary_data_size,
	     report_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to shell item.",
		 function );

		goto on_error;
	}
	if( shell_items_item_fprint(
	     shell_item,
	     0,
	     report_handle->notify_stream,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print shell item.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_free(
	     &shell_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free shell item.",
		 function );

		goto on_error;
	}
	memory_free(
	 binary_data );

	return( 1 );

on_error:
	if( shell_item != NULL )
	{
		libfwsi_item_free(
		 &shell_item,
		 NULL );
	}
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	return( -1 );
}

/* Analyzes a value that contains a shell item list
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error )
{
	libfwsi_item_list_t *shell_item_list = NULL;
	uint8_t *binary_data                 = NULL;
	static char *function                = "report_handle_analyze_shell_item_list_value";
	size_t binary_data_size              = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_list_initialize(
	     &shell_item_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create shell item list.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_list_copy_from_byte_stream(
	     shell_item_list,
	     binary_data,
	     binary_data_size,
	     report_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to shell item list.",
		 function );

		goto on_error;
	}
	if( shell_items_item_list_fprint(
	     shell_item_list,
	     report_handle->notify_stream,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print shell item list.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_list_free(
	     &shell_item_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free shell item list.",
		 function );

		goto on_error;
	}
	memory_free(
	 binary_data );

	return( 1 );

on_error:
	if( shell_item_list != NULL )
	{
		libfwsi_item_list_free(
		 &shell_item_list,
		 NULL );
	}
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	return( -1 );
}

/* Analyzes a value that contains a string and shell item
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	libfwsi_item_t *shell_item                  = NULL;
	uint8_t *binary_data                        = NULL;
	static char *function                       = "report_handle_analyze_string_and_shell_item_value";
	size_t binary_data_offset                   = 0;
	size_t binary_data_size                     = 0;
	size_t value_string_size                    = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	while( ( binary_data_offset + 2 ) < binary_data_size )
	{
		if( ( binary_data[ binary_data_offset ] == 0 )
		 && ( binary_data[ binary_data_offset + 1 ] == 0 ) )
		{
			break;
		}
		binary_data_offset += 2;
	}
	binary_data_offset += 2;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value name size.",
		 function );

		goto on_error;
	}
	value_string = libcstring_system_string_allocate(
	                value_string_size );

	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value name string.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) value_string,
		  value_string_size,
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) value_string,
		  value_string_size,
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value name.",
		 function );

		goto on_error;
	}
	fprintf(
	 report_handle->notify_stream,
	 "\tString\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 value_string );

	memory_free(
	 value_string );

	value_string = NULL;

	if( libfwsi_item_initialize(
	     &shell_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create shell item.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_copy_from_byte_stream(
	     shell_item,
	     &( binary_data[ binary_data_offset ] ),
	     binary_data_size - binary_data_offset,
	     report_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to shell item.",
		 function );

		goto on_error;
	}
	if( shell_items_item_fprint(
	     shell_item,
	     0,
	     report_handle->notify_stream,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print shell item.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_free(
	     &shell_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free shell item.",
		 function );

		goto on_error;
	}
	memory_free(
	 binary_data );

	return( 1 );

on_error:
	if( shell_item != NULL )
	{
		libfwsi_item_free(
		 &shell_item,
		 NULL );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	return( -1 );
}

/* Analyzes a value that contains a string and shell item list
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_list_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_string = NULL;
	libfwsi_item_list_t *shell_item_list        = NULL;
	uint8_t *binary_data                        = NULL;
	static char *function                       = "report_handle_analyze_string_and_shell_item_list_value";
	size_t binary_data_offset                   = 0;
	size_t binary_data_size                     = 0;
	size_t value_string_size                    = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	while( ( binary_data_offset + 2 ) < binary_data_size )
	{
		if( ( binary_data[ binary_data_offset ] == 0 )
		 && ( binary_data[ binary_data_offset + 1 ] == 0 ) )
		{
			break;
		}
		binary_data_offset += 2;
	}
	binary_data_offset += 2;

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_size_from_utf16_stream(
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#else
	result = libuna_utf8_string_size_from_utf16_stream(
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  &value_string_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value name size.",
		 function );

		goto on_error;
	}
	value_string = libcstring_system_string_allocate(
	                value_string_size );

	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value name string.",
		 function );

		goto on_error;
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libuna_utf16_string_copy_from_utf16_stream(
		  (libuna_utf16_character_t *) value_string,
		  value_string_size,
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#else
	result = libuna_utf8_string_copy_from_utf16_stream(
		  (libuna_utf8_character_t *) value_string,
		  value_string_size,
		  binary_data,
		  binary_data_offset,
		  LIBUNA_ENDIAN_LITTLE,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value name.",
		 function );

		goto on_error;
	}
	fprintf(
	 report_handle->notify_stream,
	 "\tString\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
	 value_string );

	memory_free(
	 value_string );

	value_string = NULL;

	if( libfwsi_item_list_initialize(
	     &shell_item_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create shell item list.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_list_copy_from_byte_stream(
	     shell_item_list,
	     &( binary_data[ binary_data_offset ] ),
	     binary_data_size - binary_data_offset,
	     report_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy byte stream to shell item list.",
		 function );

		goto on_error;
	}
	if( shell_items_item_list_fprint(
	     shell_item_list,
	     report_handle->notify_stream,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print shell item list.",
		 function );

		goto on_error;
	}
	if( libfwsi_item_list_free(
	     &shell_item_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free shell item list.",
		 function );

		goto on_error;
	}
	memory_free(
	 binary_data );

	return( 1 );

on_error:
	if( shell_item_list != NULL )
	{
		libfwsi_item_list_free(
		 &shell_item_list,
		 NULL );
	}
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	return( -1 );
}

/* Analyzes a key that contains a MRUList and shell item list values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_mrulist_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libcstring_system_character_t name[ 2 ];

	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_shell_item_list_mrulist_key";
	size_t name_size       = 0;
	int number_of_values   = 0;
	int result             = 0;
	int value_index        = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
/* TODO check for the MRUList value */
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name_size(
		          value,
		          &name_size,
		          error );
#else
		result = libregf_value_get_utf8_name_size(
		          value,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_utf16_name(
			          value,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_value_get_utf8_name(
			          value,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			/* A-Z is not a continous range on an EBCDIC based system
			 * it consists of the ranges: A-I, J-R, S-Z
			 */
			if( ( ( name[ 0 ] >= (libcstring_system_character_t) 'a' )
			  &&  ( name[ 0 ] <= (libcstring_system_character_t) 'i' ) )
			 || ( ( name[ 0 ] >= (libcstring_system_character_t) 'j' )
			  &&  ( name[ 0 ] <= (libcstring_system_character_t) 'r' ) )
			 || ( ( name[ 0 ] >= (libcstring_system_character_t) 's' )
			  &&  ( name[ 0 ] <= (libcstring_system_character_t) 'z' ) ) )
			{
				fprintf(
				 report_handle->notify_stream,
				 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 name );

				if( report_handle_analyze_shell_item_list_value(
				     report_handle,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM ".",
					 function,
					 name );

					goto on_error;
				}
			}
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key path that contains a MRUList and shell item list values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_mrulist_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_shell_item_list_mrulist_key_path";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_shell_item_list_mrulist_key(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key that contains a MRUListEx and shell item list values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	/* The largest string of a 32-bit unsigned integer is: 4294967295 (10 characters)
	 */
	libcstring_system_character_t name[ 11 ];

	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_shell_item_list_mrulistex_key";
	size_t name_size       = 0;
	int number_of_values   = 0;
	int result             = 0;
	int name_index         = 0;
	int value_index        = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
/* TODO check for the MRUListEx value */
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name_size(
		          value,
		          &name_size,
		          error );
#else
		result = libregf_value_get_utf8_name_size(
		          value,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_utf16_name(
			          value,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_value_get_utf8_name(
			          value,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			result = 1;

			for( name_index = 0;
			     name_index < 11;
			     name_index++ )
			{
				if( name[ name_index ] == 0 )
				{
					break;
				}
				if( ( name[ name_index ] < (libcstring_system_character_t) '0' )
				 || ( name[ name_index ] > (libcstring_system_character_t) '9' ) )
				{
					result = 0;

					break;
				}
			}
			if( result != 0 )
			{
				fprintf(
				 report_handle->notify_stream,
				 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 name );

				if( report_handle_analyze_shell_item_list_value(
				     report_handle,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM ".",
					 function,
					 name );

					goto on_error;
				}
			}
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key path that contains a MRUListEx and shell item list values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_shell_item_list_mrulistex_key_path";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_shell_item_list_mrulistex_key(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the sub keys of a path that contains a MRUListEx and shell item list values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_shell_item_list_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libcstring_system_character_t *sub_key_name = NULL;
	libregf_key_t *key                          = NULL;
	libregf_key_t *sub_key                      = NULL;
	static char *function                       = "report_handle_analyze_shell_item_list_mrulistex_sub_keys_path";
	size_t sub_key_name_size                    = 0;
	int number_of_sub_keys                      = 0;
	int sub_key_index                           = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libregf_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libregf_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( report_handle_get_key_name(
			     sub_key,
			     &sub_key_name,
			     &sub_key_name_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d name.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM "\n",
			 key_path,
			 sub_key_name );

			if( report_handle_analyze_shell_item_list_mrulistex_key(
			     report_handle,
			     sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 key_path,
				 sub_key_name );

				goto on_error;
			}
			memory_free(
			 sub_key_name );

			sub_key_name = NULL;

			if( libregf_key_free(
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "\n" );
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key_name != NULL )
	{
		memory_free(
		 sub_key_name );
	}
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key that contains a MRUListEx and "string and shell item" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	/* The largest string of a 32-bit unsigned integer is: 4294967295 (10 characters)
	 */
	libcstring_system_character_t name[ 11 ];

	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_string_and_shell_item_mrulistex_key";
	size_t name_size       = 0;
	int number_of_values   = 0;
	int result             = 0;
	int name_index         = 0;
	int value_index        = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
/* TODO check for the MRUListEx value */
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name_size(
		          value,
		          &name_size,
		          error );
#else
		result = libregf_value_get_utf8_name_size(
		          value,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_utf16_name(
			          value,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_value_get_utf8_name(
			          value,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			result = 1;

			for( name_index = 0;
			     name_index < 11;
			     name_index++ )
			{
				if( name[ name_index ] == 0 )
				{
					break;
				}
				if( ( name[ name_index ] < (libcstring_system_character_t) '0' )
				 || ( name[ name_index ] > (libcstring_system_character_t) '9' ) )
				{
					result = 0;

					break;
				}
			}
			if( result != 0 )
			{
				fprintf(
				 report_handle->notify_stream,
				 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 name );

				if( report_handle_analyze_string_and_shell_item_value(
				     report_handle,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM ".",
					 function,
					 name );

					goto on_error;
				}
			}
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key path that contains a MRUListEx and "string and shell item" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_string_and_shell_item_mrulistex_key_path";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_string_and_shell_item_mrulistex_key(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the sub keys of a path that contains a MRUListEx and "string and shell item" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libcstring_system_character_t *sub_key_name = NULL;
	libregf_key_t *key                          = NULL;
	libregf_key_t *sub_key                      = NULL;
	static char *function                       = "report_handle_analyze_string_and_shell_item_mrulistex_sub_keys_path";
	size_t sub_key_name_size                    = 0;
	int number_of_sub_keys                      = 0;
	int sub_key_index                           = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libregf_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libregf_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( report_handle_get_key_name(
			     sub_key,
			     &sub_key_name,
			     &sub_key_name_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d name.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM "\n",
			 key_path,
			 sub_key_name );

			if( report_handle_analyze_string_and_shell_item_mrulistex_key(
			     report_handle,
			     sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 key_path,
				 sub_key_name );

				goto on_error;
			}
			memory_free(
			 sub_key_name );

			sub_key_name = NULL;

			if( libregf_key_free(
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "\n" );
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key_name != NULL )
	{
		memory_free(
		 sub_key_name );
	}
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key that contains a MRUListEx and "string and shell item list" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_list_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	/* The largest string of a 32-bit unsigned integer is: 4294967295 (10 characters)
	 */
	libcstring_system_character_t name[ 11 ];

	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_string_and_shell_item_list_mrulistex_key";
	size_t name_size       = 0;
	int number_of_values   = 0;
	int result             = 0;
	int name_index         = 0;
	int value_index        = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
/* TODO check for the MRUListEx value */
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name_size(
		          value,
		          &name_size,
		          error );
#else
		result = libregf_value_get_utf8_name_size(
		          value,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_utf16_name(
			          value,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_value_get_utf8_name(
			          value,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			result = 1;

			for( name_index = 0;
			     name_index < 11;
			     name_index++ )
			{
				if( name[ name_index ] == 0 )
				{
					break;
				}
				if( ( name[ name_index ] < (libcstring_system_character_t) '0' )
				 || ( name[ name_index ] > (libcstring_system_character_t) '9' ) )
				{
					result = 0;

					break;
				}
			}
			if( result != 0 )
			{
				fprintf(
				 report_handle->notify_stream,
				 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
				 name );

				if( report_handle_analyze_string_and_shell_item_list_value(
				     report_handle,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM ".",
					 function,
					 name );

					goto on_error;
				}
			}
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a key path that contains a MRUListEx and "string and shell item list" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_list_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_string_and_shell_item_list_mrulistex_key_path";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_string_and_shell_item_list_mrulistex_key(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the sub keys of a path that contains a MRUListEx and "string and shell item list" values
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_string_and_shell_item_list_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libcstring_system_character_t *sub_key_name = NULL;
	libregf_key_t *key                          = NULL;
	libregf_key_t *sub_key                      = NULL;
	static char *function                       = "report_handle_analyze_string_and_shell_item_list_mrulistex_sub_keys_path";
	size_t sub_key_name_size                    = 0;
	int number_of_sub_keys                      = 0;
	int sub_key_index                           = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libregf_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libregf_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( report_handle_get_key_name(
			     sub_key,
			     &sub_key_name,
			     &sub_key_name_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d name.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM "\n",
			 key_path,
			 sub_key_name );

			if( report_handle_analyze_string_and_shell_item_list_mrulistex_key(
			     report_handle,
			     sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM "\\%" PRIs_LIBCSTRING_SYSTEM ".",
				 function,
				 key_path,
				 sub_key_name );

				goto on_error;
			}
			memory_free(
			 sub_key_name );

			sub_key_name = NULL;

			if( libregf_key_free(
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "\n" );
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key_name != NULL )
	{
		memory_free(
		 sub_key_name );
	}
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a BagMRU key
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_bagmru_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libcstring_system_character_t name[ 2 ];

	libregf_key_t *sub_key = NULL;
	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_bagmru_key";
	size_t name_size       = 0;
	int number_of_sub_keys = 0;
	int number_of_values   = 0;
	int result             = 0;
	int sub_key_index      = 0;
	int value_index        = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_value_get_utf16_name_size(
		          value,
		          &name_size,
		          error );
#else
		result = libregf_value_get_utf8_name_size(
		          value,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_value_get_utf16_name(
			          value,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_value_get_utf8_name(
			          value,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			if( ( name[ 0 ] >= (libcstring_system_character_t) '0' )
			 && ( name[ 0 ] <= (libcstring_system_character_t) '9' ) )
			{
				fprintf(
				 report_handle->notify_stream,
				 "(value: %d) %" PRIs_LIBCSTRING_SYSTEM "\n",
				 value_index,
				 name );

				if( report_handle_analyze_shell_item_value(
				     report_handle,
				     value,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM ".",
					 function,
					 name );

					goto on_error;
				}
			}
		}
		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	if( libregf_key_get_number_of_sub_keys(
	     key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		goto on_error;
	}
	for( sub_key_index = 0;
	     sub_key_index < number_of_sub_keys;
	     sub_key_index++ )
	{
		if( libregf_key_get_sub_key(
		     key,
		     sub_key_index,
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %d.",
			 function,
			 sub_key_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_utf16_name_size(
		          sub_key,
		          &name_size,
		          error );
#else
		result = libregf_key_get_utf8_name_size(
		          sub_key,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %d name size.",
			 function,
			 sub_key_index );

			goto on_error;
		}
		if( name_size == 2 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libregf_key_get_utf16_name(
			          sub_key,
			          (uint16_t *) name,
			          name_size,
			          error );
#else
			result = libregf_key_get_utf8_name(
			          sub_key,
			          (uint8_t *) name,
			          name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d name.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( ( name[ 0 ] >= (uint8_t) '0' )
			 && ( name[ 0 ] <= (uint8_t) '9' ) )
			{
				fprintf(
				 report_handle->notify_stream,
				 "(key:) %" PRIs_LIBCSTRING_SYSTEM "\n",
				 name );

				if( report_handle_analyze_bagmru_key(
				     report_handle,
				     sub_key,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GENERIC,
					 "%s: unable to analyze BagMRU sub key: %d.",
					 function,
					 sub_key_index );

					goto on_error;
				}
			}
		}
		if( libregf_key_free(
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key: %d.",
			 function,
			 sub_key_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a BagMRU key path
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int report_handle_analyze_bagmru_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error )
{
	libregf_key_t *key    = NULL;
	static char *function = "report_handle_analyze_bagmru_key_path";
	int result            = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( key_path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key path.",
		 function );

		return( -1 );
	}
	if( key_path_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid key path length value exceeds maximum.",
		 function );

		return( -1 );
	}
	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by name.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 key_path );

		if( report_handle_analyze_bagmru_key(
		     report_handle,
		     key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the BagMRU
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_bagmru(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *key_path = NULL;
	static char *function                   = "report_handle_analyze_bagmru";
	size_t key_path_length                  = 0;
	int result                              = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
/* TODO */

	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\Shell\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\ShellNoRoam\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Local Settings\\Software\\Microsoft\\Windows\\ShellNoRoam\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Wow6432Node\\Local Settings\\Software\\Microsoft\\Windows\\Shell\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Wow6432Node\\Local Settings\\Software\\Microsoft\\Windows\\ShellNoRoam\\BagMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_bagmru_key_path(
	          report_handle,
	          key_path,
	          key_path_length,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	return( 1 );
}

/* Analyzes the MRU keys
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_mru_keys(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *key_path = NULL;
	static char *function                   = "report_handle_analyze_mru_keys";
	size_t key_path_length                  = 0;
	int result                              = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	/* MRUList keys with shell item list values
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\DesktopStreamMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_shell_item_list_mrulist_key_path(
		  report_handle,
		  key_path,
		  key_path_length,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	/* MRUListEx keys with shell item list values
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\OpenSavePidlMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_shell_item_list_mrulistex_sub_keys_path(
		  report_handle,
		  key_path,
		  key_path_length,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StreamMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_shell_item_list_mrulistex_key_path(
		  report_handle,
		  key_path,
		  key_path_length,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	/* MRUListEx keys with "string and shell item" values
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_string_and_shell_item_mrulistex_sub_keys_path(
		  report_handle,
		  key_path,
		  key_path_length,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	/* MRUListEx keys with "string and shell item list" values
	 */
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\ComDlg32\\LastVisitedPidlMRU" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = report_handle_analyze_string_and_shell_item_list_mrulistex_key_path(
		  report_handle,
		  key_path,
		  key_path_length,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	return( 1 );
}

/* Analyzes the Windows Explorer settings
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_explorer(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *key_path = NULL;
	static char *function                   = "report_handle_analyze_explorer";
	size_t key_path_length                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	key_path = _LIBCSTRING_SYSTEM_STRING( "\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Browser Helper Objects" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

/* TODO contains a key with sub keys and a value "name"
	if( report_handle_analyze_key_path_with_string_values(
	     report_handle,
	     key_path,
	     key_path_length,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
*/
	return( 1 );
}

/* Analyzes a SAM F value
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_sam_f_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error )
{
	uint8_t *binary_data    = NULL;
	static char *function   = "report_handle_analyze_sam_f_value";
	size_t binary_data_size = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	if( report_handle_print_data(
	     report_handle,
	     binary_data,
	     binary_data_size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print value binary data.",
		 function );

		return( -1 );
	}
/* TODO something with data */

	memory_free(
	 binary_data );

	return( 1 );

on_error:
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	return( -1 );
}

/* Analyzes a SAM Users key
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_sam_users_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libregf_value_t *value = NULL;
	static char *function  = "report_handle_analyze_sam_users_key";
	int result             = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	result = libregf_key_get_value_by_utf8_name(
	          key,
	          (uint8_t *) "F",
	          1,
	          &value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve F value.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( report_handle_analyze_sam_f_value(
		     report_handle,
		     value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze F value.",
			 function );

			goto on_error;
		}
	}
	if( libregf_value_free(
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free F value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the SAM
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_sam(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *key_path = NULL;
	libregf_key_t *key                      = NULL;
	libregf_key_t *sub_key                  = NULL;
	static char *function                   = "report_handle_analyze_sam";
	size_t key_path_length                  = 0;
	int number_of_sub_keys                  = 0;
	int result                              = 0;
	int sub_key_index                       = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
/* TODO dynamically handle user keys */
	key_path = _LIBCSTRING_SYSTEM_STRING( "SAM\\Domains\\Account\\Users" );

	key_path_length = libcstring_system_string_length(
	                   key_path );

	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          key_path,
	          key_path_length,
	          &key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by path: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( libregf_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libregf_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
/* TODO get name and ignore Names */
			if( report_handle_analyze_sam_users_key(
			     report_handle,
			     sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to analyze SAM users key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( libregf_key_free(
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
		}
		if( libregf_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a Windows Explorer RecentDocs key
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_recent_docs_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libcstring_system_character_t name[ 32 ];

	libregf_key_t *sub_key = NULL;
	static char *function  = "report_handle_analyze_recent_docs_key";
	size_t name_size       = 0;
	int number_of_sub_keys = 0;
	int result             = 0;
	int sub_key_index      = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_sub_keys(
	     key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		goto on_error;
	}
	for( sub_key_index = 0;
	     sub_key_index < number_of_sub_keys;
	     sub_key_index++ )
	{
		if( libregf_key_get_sub_key(
		     key,
		     sub_key_index,
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %d.",
			 function,
			 sub_key_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_utf16_name_size(
		          sub_key,
		          &name_size,
		          error );
#else
		result = libregf_key_get_utf8_name_size(
		          sub_key,
		          &name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %d name size.",
			 function,
			 sub_key_index );

			goto on_error;
		}
		if( name_size > 32 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: sub key: %d name size value out of bounds.",
			 function,
			 sub_key_index );

			goto on_error;
		}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libregf_key_get_utf16_name(
		          sub_key,
		          (uint16_t *) name,
		          name_size,
		          error );
#else
		result = libregf_key_get_utf8_name(
		          sub_key,
		          (uint8_t *) name,
		          name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key: %d name.",
			 function,
			 sub_key_index );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\tType\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 name );

		if( report_handle_analyze_recent_docs_sub_key(
		     report_handle,
		     sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze RecentDocs sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 name );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );

		if( libregf_key_free(
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key: %d.",
			 function,
			 sub_key_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes a Windows Explorer RecentDocs sub key
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_recent_docs_sub_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libcstring_system_character_t *value_name   = NULL;
	libcstring_system_character_t *value_string = NULL;
	libregf_value_t *value                      = NULL;
	static char *function                       = "report_handle_analyze_recent_docs_sub_key";
	size_t value_name_size                      = 0;
	size_t value_string_size                    = 0;
	int number_of_values                        = 0;
	int value_index                             = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libregf_key_get_value(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
		if( report_handle_get_value_name(
		     value,
		     &value_name,
		     &value_name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name.",
			 function );

			goto on_error;
		}
		if( value_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value name value.",
			 function );

			goto on_error;
		}
/* TODO check if this code belongs here */
		if( libcstring_system_string_compare(
		     value_name,
		     _LIBCSTRING_SYSTEM_STRING( "MRUListEx" ),
		     9 ) != 0 )
		{
			if( report_handle_get_value_data_string(
			     value,
			     &value_string,
			     &value_string_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data string.",
				 function );

				goto on_error;
			}
			fprintf(
			 report_handle->notify_stream,
			 "Value\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 value_name );
			fprintf(
			 report_handle->notify_stream,
			 "\tString\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
		memory_free(
		 value_name );

		value_name = NULL;

		if( libregf_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
	if( value_name != NULL )
	{
		memory_free(
		 value_name );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the Windows Explorer RecentDocs
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_recent_docs(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *sub_key_path = NULL;
	libregf_key_t *sub_key                      = NULL;
	static char *function                       = "report_handle_analyze_recent_docs";
	size_t sub_key_path_length                  = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	sub_key_path = _LIBCSTRING_SYSTEM_STRING( "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\RecentDocs" );

	sub_key_path_length = libcstring_system_string_length(
	                       sub_key_path );

	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          sub_key_path,
	          sub_key_path_length,
	          &sub_key,
	          error );

	if( result == - 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve RecentDocs key by path.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Windows Explorer RecentDocs: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 sub_key_path );

		if( report_handle_analyze_recent_docs_key(
		     report_handle,
		     sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze RecentDocs key.",
			 function );

			goto on_error;
		}
		if( libregf_key_free(
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free users sub key.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the Windows product info
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_product_info(
     report_handle_t *report_handle,
     libregf_error_t **error )
{
	libcstring_system_character_t *sub_key_path = NULL;
	libcstring_system_character_t *value_name   = NULL;
	libregf_key_t *sub_key                      = NULL;
	static char *function                       = "report_handle_analyze_product_info";
	size_t sub_key_path_length                  = 0;
	size_t value_name_length                    = 0;
	int result                                  = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	sub_key_path = _LIBCSTRING_SYSTEM_STRING( "\\Microsoft\\Windows NT\\CurrentVersion" );

	sub_key_path_length = libcstring_system_string_length(
	                       sub_key_path );

	result = registry_file_get_key_by_path(
	          report_handle->input_file,
	          sub_key_path,
	          sub_key_path_length,
	          &sub_key,
	          error );

	if( result == - 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key by path: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 sub_key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		fprintf(
		 report_handle->notify_stream,
		 "Windows Product Information:\n" );

		fprintf(
		 report_handle->notify_stream,
		 "Key\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
		 sub_key_path );

		value_name = _LIBCSTRING_SYSTEM_STRING( "ProductName" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "CSDVersion" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "CurrentVersion" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "CurrentBuildNumber" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "CurrentType" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "ProductId" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "RegisteredOwner" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "RegisteredOrganization" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "PathName" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "SystemRoot" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_string_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		value_name = _LIBCSTRING_SYSTEM_STRING( "InstallDate" );

		value_name_length = libcstring_system_string_length(
		                     value_name );

		result = report_handle_analyze_key_with_posix_time_value(
		          report_handle,
		          sub_key,
		          value_name,
		          value_name_length,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: %" PRIs_LIBCSTRING_SYSTEM " in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 value_name,
			 sub_key_path );

			goto on_error;
		}
		result = report_handle_analyze_digital_product_id_value(
		          report_handle,
		          sub_key,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to analyze value: DigitalProductId in sub key: %" PRIs_LIBCSTRING_SYSTEM ".",
			 function,
			 sub_key_path );

			goto on_error;
		}
		if( libregf_key_free(
		     &sub_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key.",
			 function );

			goto on_error;
		}
		fprintf(
		 report_handle->notify_stream,
		 "\n" );
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libregf_key_free(
		 &sub_key,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the DigitalProductId value in the product information
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_digital_product_id_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error )
{
	uint8_t value_15byte[ 15 ];
	char product_key_string[ 30 ];

	char substition_table[ 24 ] = {
		'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K',
		'M', 'P', 'Q', 'R', 'T', 'V', 'W', 'X',
		'Y', '2', '3', '4', '6', '7', '8', '9' };

	libcstring_system_character_t *value_name = NULL;
	libregf_value_t *value                    = NULL;
	uint8_t *binary_data                      = NULL;
	static char *function                     = "report_handle_analyze_digital_product_id_value";
	size_t binary_data_size                   = 0;
	size_t value_name_length                  = 0;
	uint16_t value_16bit                      = 0;
	uint8_t remainder                         = 0;
	int8_t byte_index                         = 0;
	int8_t string_index                       = 0;
	int result                                = 0;

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	value_name = _LIBCSTRING_SYSTEM_STRING( "DigitalProductId" );

	value_name_length = libcstring_system_string_length(
	                     value_name );

#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_key_get_value_by_utf16_name(
		  key,
		  (uint16_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#else
	result = libregf_key_get_value_by_utf8_name(
		  key,
		  (uint8_t *) value_name,
		  value_name_length,
		  &value,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 value_name );

		goto on_error;
	}
	if( report_handle_get_value_binary_data(
	     value,
	     &binary_data,
	     &binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value binary data.",
		 function );

		goto on_error;
	}
	fprintf(
	 report_handle->notify_stream,
	 "%" PRIs_LIBCSTRING_SYSTEM ":\n",
	 value_name );

/* TODO */
	if( report_handle_print_data(
	     report_handle,
	     binary_data,
	     binary_data_size,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print binary data.",
		 function );

		goto on_error;
	}
	if( binary_data_size == 0xa4 )
	{
/* TODO no longer applies to WinXP SP3 */
		if( memory_copy(
		     value_15byte,
		     &( binary_data[ 0x34 ] ),
		     15 ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy binary data to 15-byte value.",
			 function );

			goto on_error;
		}
		for( string_index = 28;
		     string_index >= 0;
		     string_index-- )
		{
			if( ( string_index % 6 ) == 5 )
			{
				product_key_string[ string_index ] = '-';
			}
			else
			{
				/* Divide the 15-byte value by 24 */
				remainder = 0;

				for( byte_index = ( 15 - 1 );
				     byte_index >= 0;
				     byte_index-- )
				{
					value_16bit   = remainder;
					value_16bit <<= 8;
					value_16bit  |= value_15byte[ byte_index ];

					value_15byte[ byte_index ] = (uint8_t) ( value_16bit / 24 );

					remainder = (uint8_t) ( value_16bit % 24 );
				}
				product_key_string[ string_index ] = substition_table[ remainder ];
			}
		}
		product_key_string[ 29 ] = 0;

		fprintf(
		 report_handle->notify_stream,
		 "%s\n",
		 product_key_string );
	}
	memory_free(
	 binary_data );

	binary_data = NULL;

	if( libregf_value_free(
	     &value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free value: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 value_name );

		goto on_error;
	}
	return( 1 );

on_error:
	if( binary_data != NULL )
	{
		memory_free(
		 binary_data );
	}
	if( value != NULL )
	{
		libregf_value_free(
		 &value,
		 NULL );
	}
	return( -1 );
}

/* Analyzes the file
 * Returns 1 if successful or -1 on error
 */
int report_handle_analyze_file(
     report_handle_t *report_handle,
     libcerror_error_t **error )
{
	static char *function = "report_handle_analyze_file";

	if( report_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid report handle.",
		 function );

		return( -1 );
	}
	if( report_handle_analyze_sam(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze SAM.",
		 function );

		goto on_error;
	}
/* TODO
fuse/Microsoft/Windows\ NT/CurrentVersion/Windows/LoadAppInit_DLLs
fuse/Microsoft/Windows\ NT/CurrentVersion/Windows/AppInit_DLLs

If exists
Microsoft\Command Processor\Auto Run


fuse/Microsoft/Windows/CurrentVersion/Explorer/FolderDescriptions/

\Microsoft\Windows\CurrentVersion\Explorer\UserAssist\
\Microsoft\Windows\CurrentVersion\Explorer\RunMRU

*/

	if( report_handle_analyze_product_info(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze product info.",
		 function );

		goto on_error;
	}
/* TODO
* Network settings (XP)
* /CurrentControlSet/Services/Tcpip/Parameters/DataBasePath
*/

	if( report_handle_analyze_autorun(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze autorun.",
		 function );

		goto on_error;
	}
	if( report_handle_analyze_explorer(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze explorer.",
		 function );

		goto on_error;
	}
/* TODO
	if( report_handle_analyze_recent_docs(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze recent docs.",
		 function );

		goto on_error;
	}
*/
	if( report_handle_analyze_bagmru(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze BagMRU.",
		 function );

		goto on_error;
	}
	if( report_handle_analyze_mru_keys(
	     report_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to analyze MRU keys.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	return( -1 );
}

