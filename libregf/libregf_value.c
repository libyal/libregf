/*
 * Value functions
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
#include <memory.h>
#include <types.h>

#include "libregf_definitions.h"
#include "libregf_io_handle.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_multi_string.h"
#include "libregf_value.h"
#include "libregf_value_item.h"

/* Creates a value
 * Make sure the value value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_value_initialize(
     libregf_value_t **value,
     libregf_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libregf_value_item_t *value_item,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_initialize";

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
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value value already set.",
		 function );

		return( -1 );
	}
	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	internal_value = memory_allocate_structure(
	                  libregf_internal_value_t );

	if( internal_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_value,
	     0,
	     sizeof( libregf_internal_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal value.",
		 function );

		memory_free(
		 internal_value );

		return( -1 );
	}
	if( libregf_value_item_clone(
	     &( internal_value->value_item ),
	     value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone value item.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_value->read_write_lock ),
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
	internal_value->file_io_handle = file_io_handle;
	internal_value->io_handle      = io_handle;
	internal_value->file_offset    = file_offset;

	*value = (libregf_value_t *) internal_value;

	return( 1 );

on_error:
	if( internal_value != NULL )
	{
		if( internal_value->value_item != NULL )
		{
			libregf_value_item_free(
			 &( internal_value->value_item ),
			 NULL );
		}
		memory_free(
		 internal_value );
	}
	return( -1 );
}

/* Frees a value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_free(
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_free";
	int result                               = 1;

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
	if( *value != NULL )
	{
		internal_value = (libregf_internal_value_t *) *value;
		*value         = NULL;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_value->read_write_lock ),
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
		if( libregf_value_item_free(
		     &( internal_value->value_item ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value item.",
			 function );

			result = -1;
		}
		/* The io_handle and file_io_handle references are freed elsewhere
		 */
		memory_free(
		 internal_value );
	}
	return( result );
}

/* Determine if the value corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libregf_value_is_corrupted(
     libregf_value_t *value,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_is_corrupted";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	result = libregf_value_item_is_corrupted(
	          internal_value->value_item,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value is corrupted.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the offset of the value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_offset(
     libregf_value_t *value,
     off64_t *offset,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_offset";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	*offset = internal_value->file_offset;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the value name size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_name_size(
     libregf_value_t *value,
     size_t *name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_name_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_name_size(
	     internal_value->value_item,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the value name
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_name(
     libregf_value_t *value,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_name";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_name(
	     internal_value->value_item,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the UTF-8 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_utf8_name_size(
     libregf_value_t *value,
     size_t *utf8_name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_utf8_name_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_utf8_name_size(
	     internal_value->value_item,
	     utf8_name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the UTF-8 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_utf8_name(
     libregf_value_t *value,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_utf8_name";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_utf8_name(
	     internal_value->value_item,
	     utf8_name,
	     utf8_name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the UTF-16 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_utf16_name_size(
     libregf_value_t *value,
     size_t *utf16_name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_utf16_name_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_utf16_name_size(
	     internal_value->value_item,
	     utf16_name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the UTF-16 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_utf16_name(
     libregf_value_t *value,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_utf16_name";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_utf16_name(
	     internal_value->value_item,
	     utf16_name,
	     utf16_name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_type(
     libregf_value_t *value,
     uint32_t *value_type,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_type";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_value_type(
	     internal_value->value_item,
	     value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the value data size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_data_size(
     libregf_value_t *value,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_data_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_value->read_write_lock,
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
	if( libregf_value_item_get_data_size(
	     internal_value->value_item,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_value->read_write_lock,
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

/* Retrieves the value data
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_data(
     libregf_value_t *value,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	uint8_t *data                            = NULL;
	static char *function                    = "libregf_value_get_value_data";
	size_t data_size                         = 0;
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_data(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		result = -1;
	}
	else if( value_data_size < data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		result = -1;
	}
	else if( memory_copy(
	          value_data,
	          data,
	          data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the 32-bit value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_32bit(
     libregf_value_t *value,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_32bit";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_32bit(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 32-bit value.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the 64-bit value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_64bit(
     libregf_value_t *value,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_64bit";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_64bit(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve 64-bit value.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_utf8_string_size(
     libregf_value_t *value,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_utf8_string_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_utf8_string_size(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_utf8_string(
     libregf_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_utf8_string";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_utf8_string(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 string size at a specific value from the referenced value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_utf16_string_size(
     libregf_value_t *value,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_utf16_string_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_utf16_string_size(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_utf16_string(
     libregf_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_utf16_string";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_utf16_string(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the binary data size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_binary_data_size(
     libregf_value_t *value,
     size_t *size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_binary_data_size";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_binary_data_size(
	     internal_value->value_item,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve binary data size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the binary data value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_binary_data(
     libregf_value_t *value,
     uint8_t *binary_data,
     size_t size,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_binary_data";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_binary_data(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     binary_data,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve binary data.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the multi string value
 * Creates a new multi string
 * Returns 1 if successful or -1 on error
 */
int libregf_value_get_value_multi_string(
     libregf_value_t *value,
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error )
{
	libregf_internal_value_t *internal_value = NULL;
	static char *function                    = "libregf_value_get_value_multi_string";
	int result                               = 1;

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
	internal_value = (libregf_internal_value_t *) value;

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
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libregf_value_item_get_value_multi_string(
	     internal_value->value_item,
	     internal_value->file_io_handle,
	     multi_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve multi string.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_value->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		libregf_multi_string_free(
		 multi_string,
		 NULL );

		return( -1 );
	}
#endif
	return( result );
}

