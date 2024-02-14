/*
 * Mount handle
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "mount_file_entry.h"
#include "mount_file_system.h"
#include "mount_handle.h"
#include "regftools_libcerror.h"
#include "regftools_libclocale.h"
#include "regftools_libcpath.h"
#include "regftools_libregf.h"

/* Creates a mount handle
 * Make sure the value mount_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_handle_initialize(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_initialize";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid mount handle value already set.",
		 function );

		return( -1 );
	}
	*mount_handle = memory_allocate_structure(
	                 mount_handle_t );

	if( *mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create mount handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *mount_handle,
	     0,
	     sizeof( mount_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear mount handle.",
		 function );

		goto on_error;
	}
	if( mount_file_system_initialize(
	     &( ( *mount_handle )->file_system ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file system.",
		 function );

		goto on_error;
	}
	( *mount_handle )->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *mount_handle != NULL )
	{
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( -1 );
}

/* Frees a mount handle
 * Returns 1 if successful or -1 on error
 */
int mount_handle_free(
     mount_handle_t **mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_free";
	int result            = 1;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( *mount_handle != NULL )
	{
		if( mount_file_system_free(
		     &( ( *mount_handle )->file_system ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file system.",
			 function );

			result = -1;
		}
		memory_free(
		 *mount_handle );

		*mount_handle = NULL;
	}
	return( result );
}

/* Signals the mount handle to abort
 * Returns 1 if successful or -1 on error
 */
int mount_handle_signal_abort(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	static char *function = "mount_handle_signal_abort";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_signal_abort(
	     mount_handle->file_system,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to signal file system to abort.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int mount_handle_set_ascii_codepage(
     mount_handle_t *mount_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "mount_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( mount_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( mount_handle->ascii_codepage ),
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

/* Opens the mount handle
 * Returns 1 if successful, 0 if not or -1 on error
 */
int mount_handle_open(
     mount_handle_t *mount_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	libregf_file_t *regf_file = NULL;
	static char *function     = "mount_handle_open";
	int result                = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( libregf_file_initialize(
	     &regf_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_file_open_wide(
	          regf_file,
	          filename,
	          LIBREGF_OPEN_READ,
	          error );
#else
	result = libregf_file_open(
	          regf_file,
	          filename,
	          LIBREGF_OPEN_READ,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	if( mount_file_system_set_file(
	     mount_handle->file_system,
	     regf_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file in file system.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( regf_file != NULL )
	{
		libregf_file_free(
		 &regf_file,
		 NULL );
	}
	return( -1 );
}

/* Closes the mount handle
 * Returns the 0 if succesful or -1 on error
 */
int mount_handle_close(
     mount_handle_t *mount_handle,
     libcerror_error_t **error )
{
	libregf_file_t *regf_file = NULL;
	static char *function     = "mount_handle_close";

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_file(
	     mount_handle->file_system,
	     &regf_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file from file system.",
		 function );

		goto on_error;
	}
	if( mount_file_system_set_file(
	     mount_handle->file_system,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file in file system.",
		 function );

		regf_file = NULL;

		goto on_error;
	}
	if( libregf_file_close(
	     regf_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		goto on_error;
	}
	if( libregf_file_free(
	     &regf_file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		goto on_error;
	}
	return( 0 );

on_error:
	if( regf_file != NULL )
	{
		libregf_file_free(
		 &regf_file,
		 NULL );
	}
	return( -1 );
}

/* Retrieves a file entry for a specific path
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_handle_get_file_entry_by_path(
     mount_handle_t *mount_handle,
     const system_character_t *path,
     mount_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	libregf_key_t *regf_key            = NULL;
	libregf_value_t *regf_value        = NULL;
	const system_character_t *filename = NULL;
	static char *function              = "mount_handle_get_file_entry_by_path";
	size_t filename_length             = 0;
	size_t key_path_length             = 0;
	size_t last_path_index             = 0;
	size_t path_index                  = 0;
	size_t path_length                 = 0;
	int file_entry_type                = MOUNT_FILE_ENTRY_TYPE_UNKNOWN;
	int result                         = 0;

	if( mount_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid mount handle.",
		 function );

		return( -1 );
	}
	if( path == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		return( -1 );
	}
	path_length = system_string_length(
	               path );

	if( path_length == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid path length value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( path_length >= 2 )
	 && ( path[ path_length - 1 ] == LIBCPATH_SEPARATOR ) )
	{
		path_length -= 1;
	}
	path_index = path_length;

	while( path_index > 0 )
	{
		if( path[ path_index ] == LIBCPATH_SEPARATOR )
		{
			break;
		}
		path_index--;
	}
	/* Ignore the name of the root item
	 */
	if( path_length == 0 )
	{
		filename        = _SYSTEM_STRING( "" );
		filename_length = 0;
	}
	else
	{
		filename        = &( path[ path_index + 1 ] );
		filename_length = path_length - ( path_index + 1 );
	}
	if( filename_length == 11 )
	{
#if defined( WINAPI )
		result = system_string_compare_no_case(
		          filename,
		          _SYSTEM_STRING( "(classname)" ),
		          11 );
#else
		result = system_string_compare(
		          filename,
		          _SYSTEM_STRING( "(classname)" ),
		          11 );
#endif
		if( result == 0 )
		{
			file_entry_type = MOUNT_FILE_ENTRY_TYPE_CLASS_NAME;
			key_path_length = path_index + 1;
		}
	}
	else if( filename_length == 8 )
	{
#if defined( WINAPI )
		result = system_string_compare_no_case(
		          filename,
		          _SYSTEM_STRING( "(values)" ),
		          8 );
#else
		result = system_string_compare(
		          filename,
		          _SYSTEM_STRING( "(values)" ),
		          8 );
#endif
		if( result == 0 )
		{
			file_entry_type = MOUNT_FILE_ENTRY_TYPE_VALUES;
			key_path_length = path_index + 1;
		}
	}
	if( ( file_entry_type == MOUNT_FILE_ENTRY_TYPE_UNKNOWN )
	 && ( path_index > 0 )
	 && ( path_length > 10 ) )
	{
		last_path_index = path_index;
		path_index     -= 1;

		while( path_index > 0 )
		{
			if( path[ path_index ] == LIBCPATH_SEPARATOR )
			{
				break;
			}
			path_index--;
		}
		if( ( last_path_index - ( path_index + 1 ) ) == 8 )
		{
#if defined( WINAPI )
			result = system_string_compare_no_case(
				  &( path[ path_index + 1 ] ),
				  _SYSTEM_STRING( "(values)" ),
				  8 );
#else
			result = system_string_compare(
				  &( path[ path_index + 1 ] ),
				  _SYSTEM_STRING( "(values)" ),
				  8 );
#endif
			if( result == 0 )
			{
				file_entry_type = MOUNT_FILE_ENTRY_TYPE_VALUE;
				key_path_length = path_index + 1;
			}
		}
	}
	if( file_entry_type == MOUNT_FILE_ENTRY_TYPE_UNKNOWN )
	{
		file_entry_type = MOUNT_FILE_ENTRY_TYPE_KEY;
		key_path_length = path_length;
	}
	result = mount_file_system_get_key_by_path(
	          mount_handle->file_system,
	          path,
	          key_path_length,
	          &regf_key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( file_entry_type == MOUNT_FILE_ENTRY_TYPE_VALUE )
		{
			result = mount_file_system_get_value_by_filename(
			          mount_handle->file_system,
			          regf_key,
			          filename,
			          filename_length,
			          &regf_value,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value.",
				 function );

				goto on_error;
			}
		}
		if( result != 0 )
		{
			if( mount_file_entry_initialize(
			     file_entry,
			     mount_handle->file_system,
			     filename,
			     filename_length,
			     file_entry_type,
			     regf_key,
			     regf_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to initialize file entry.",
				 function );

				goto on_error;
			}
		}
	}
	return( result );

on_error:
	if( regf_value != NULL )
	{
		libregf_value_free(
		 &regf_value,
		 NULL );
	}
	if( regf_key != NULL )
	{
		libregf_key_free(
		 &regf_key,
		 NULL );
	}
	return( -1 );
}

