/*
 * Key functions
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libregf_definitions.h"
#include "libregf_io_handle.h"
#include "libregf_key.h"
#include "libregf_key_item.h"
#include "libregf_key_tree.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcthreads.h"
#include "libregf_libfdata.h"
#include "libregf_libuna.h"
#include "libregf_value.h"
#include "libregf_value_item.h"

/* Creates a key
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_key_initialize(
     libregf_key_t **key,
     libregf_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t key_offset,
     libregf_hive_bins_list_t *hive_bins_list,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_initialize";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key value already set.",
		 function );

		return( -1 );
	}
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
	internal_key = memory_allocate_structure(
	                libregf_internal_key_t );

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_key,
	     0,
	     sizeof( libregf_internal_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal key.",
		 function );

		memory_free(
		 internal_key );

		return( -1 );
	}
	if( libregf_key_item_initialize(
	     &( internal_key->key_item ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key item.",
		 function );

		goto on_error;
	}
	if( libregf_key_item_read(
	     internal_key->key_item,
	     file_io_handle,
	     hive_bins_list,
	     key_offset,
	     (uint32_t) 0, /* TODO pass hash or key descriptor */
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key item at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 key_offset,
		 key_offset );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_key->read_write_lock ),
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
	internal_key->file_io_handle = file_io_handle;
	internal_key->io_handle      = io_handle;
	internal_key->key_offset     = key_offset;
	internal_key->hive_bins_list = hive_bins_list;

	*key = (libregf_key_t *) internal_key;

	return( 1 );

on_error:
	if( internal_key != NULL )
	{
		if( internal_key->key_item != NULL )
		{
			libregf_key_item_free(
			 &( internal_key->key_item ),
			 NULL );
		}
		memory_free(
		 internal_key );
	}
	return( -1 );
}

/* Frees a key
 * Returns 1 if successful or -1 on error
 */
int libregf_key_free(
     libregf_key_t **key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_free";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		internal_key = (libregf_internal_key_t *) *key;
		*key         = NULL;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_key->read_write_lock ),
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
		/* The io_handle, file_io_handle and hive_bins_list references are freed elsewhere
		 */
		if( libregf_key_item_free(
		     &( internal_key->key_item ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key item.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_key );
	}
	return( result );
}

/* Determines if the key is corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libregf_key_is_corrupted(
     libregf_key_t *key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_is_corrupted";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_is_corrupted(
	          internal_key->key_item,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if key item is corruped.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the offset of the key
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_offset(
     libregf_key_t *key,
     off64_t *offset,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_offset";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
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
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	/* The offset is relative from the start of the hive bins list
	 * and points to the start of the corresponding hive bin cell
	 */
	*offset = (off64_t) internal_key->key_offset + internal_key->io_handle->hive_bins_list_offset + 4;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_name_size(
     libregf_key_t *key,
     size_t *name_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_name_size(
	     internal_key->key_item,
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
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_name(
     libregf_key_t *key,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_name(
	     internal_key->key_item,
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
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_utf8_name_size(
     libregf_key_t *key,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf8_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_utf8_name_size(
	     internal_key->key_item,
	     utf8_string_size,
	     internal_key->io_handle->ascii_codepage,
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
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_utf8_name(
     libregf_key_t *key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf8_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_utf8_name(
	     internal_key->key_item,
	     utf8_string,
	     utf8_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_utf16_name_size(
     libregf_key_t *key,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf16_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_utf16_name_size(
	     internal_key->key_item,
	     utf16_string_size,
	     internal_key->io_handle->ascii_codepage,
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
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_utf16_name(
     libregf_key_t *key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_value_get_utf16_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_utf16_name(
	     internal_key->key_item,
	     utf16_string,
	     utf16_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the class name size
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_class_name_size(
     libregf_key_t *key,
     size_t *class_name_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_class_name_size";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_class_name_size(
	          internal_key->key_item,
	          class_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve class name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the class name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_class_name(
     libregf_key_t *key,
     uint8_t *class_name,
     size_t class_name_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_class_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_class_name(
	          internal_key->key_item,
	          class_name,
	          class_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve class name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-8 string size of the class name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_utf8_class_name_size(
     libregf_key_t *key,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf8_class_name_size";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_utf8_class_name_size(
	          internal_key->key_item,
	          utf8_string_size,
	          internal_key->io_handle->ascii_codepage,
	          error );

	if( result == -1 )
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
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-8 string value of the class name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_utf8_class_name(
     libregf_key_t *key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf8_class_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_utf8_class_name(
	          internal_key->key_item,
	          utf8_string,
	          utf8_string_size,
	          internal_key->io_handle->ascii_codepage,
	          error );

	if( result == -1 )
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
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-16 string size of the class name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_utf16_class_name_size(
     libregf_key_t *key,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_utf16_class_name_size";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_utf16_class_name_size(
	          internal_key->key_item,
	          utf16_string_size,
	          internal_key->io_handle->ascii_codepage,
	          error );

	if( result == -1 )
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
	     internal_key->read_write_lock,
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

/* Retrieves the UTF-16 string value of the class name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_utf16_class_name(
     libregf_key_t *key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_value_get_utf16_class_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_utf16_class_name(
	          internal_key->key_item,
	          utf16_string,
	          utf16_string_size,
	          internal_key->io_handle->ascii_codepage,
	          error );

	if( result == -1 )
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
	     internal_key->read_write_lock,
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

/* Retrieves the 64-bit FILETIME value of the last written date and time
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_last_written_time(
     libregf_key_t *key,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_last_written_time";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_last_written_time(
	     internal_key->key_item,
	     filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve last written time.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the security descriptor size
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_security_descriptor_size(
     libregf_key_t *key,
     size_t *security_descriptor_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_security_descriptor_size";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_security_descriptor_size(
	          internal_key->key_item,
	          security_descriptor_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the security descriptor
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_security_descriptor(
     libregf_key_t *key,
     uint8_t *security_descriptor,
     size_t security_descriptor_size,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_security_descriptor";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_item_get_security_descriptor(
	          internal_key->key_item,
	          security_descriptor,
	          security_descriptor_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve security descriptor.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_number_of_values(
     libregf_key_t *key,
     int *number_of_values,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_number_of_values";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_number_of_values(
	     internal_key->key_item,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the value
 * Creates a new value
 * Returns 1 if successful or -1 on error
 */
int libregf_internal_key_get_value(
     libregf_internal_key_t *internal_key,
     int value_index,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libfdata_list_element_t *values_list_element = NULL;
	libregf_value_item_t *value_item             = NULL;
	static char *function                        = "libregf_internal_key_get_value";
	size64_t size                                = 0;
	off64_t offset                               = 0;
	uint32_t flags                               = 0;
	int file_index                               = 0;

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( internal_key->key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing key item.",
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
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_list_element_by_index(
	     internal_key->key_item->values_list,
	     value_index,
	     &values_list_element,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 value_index );

		return( -1 );
	}
	if( libfdata_list_element_get_data_range(
	     values_list_element,
	     &file_index,
	     &offset,
	     &size,
	     &flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data range.",
		 function );

		return( -1 );
	}
	/* The offset is relative from the start of the hive bins list
	 * and points to the start of the corresponding hive bin cell
	 */
	offset += internal_key->io_handle->hive_bins_list_offset + 4;

	if( libfdata_list_element_get_element_value(
	     values_list_element,
	     (intptr_t *) internal_key->file_io_handle,
	     (libfdata_cache_t *) internal_key->key_item->values_cache,
	     (intptr_t **) &value_item,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_initialize(
	     value,
	     internal_key->io_handle,
	     internal_key->file_io_handle,
	     offset,
	     value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value
 * Creates a new value
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_value(
     libregf_key_t *key,
     int value_index,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_value";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

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
		 "%s: value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_internal_key_get_value(
	     internal_key,
	     value_index,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 value_index );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the value
 * Creates a new value
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_value_by_index(
     libregf_key_t *key,
     int value_index,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_value_by_index";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

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
		 "%s: value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_internal_key_get_value(
	     internal_key,
	     value_index,
	     value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d.",
		 function,
		 value_index );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the value for the specific UTF-8 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Creates a new value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_internal_key_get_value_by_utf8_name(
     libregf_internal_key_t *internal_key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libfdata_list_element_t *values_list_element = NULL;
	libregf_value_item_t *value_item             = NULL;
	static char *function                        = "libregf_internal_key_get_value_by_utf8_name";
	libuna_unicode_character_t unicode_character = 0;
	size64_t size                                = 0;
	size_t utf8_string_index                     = 0;
	off64_t offset                               = 0;
	uint32_t flags                               = 0;
	uint32_t name_hash                           = 0;
	int file_index                               = 0;
	int number_of_values                         = 0;
	int result                                   = 0;
	int value_index                              = 0;

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_key->key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing key item.",
		 function );

		return( -1 );
	}
	if( ( utf8_string == NULL )
	 && ( utf8_string_length != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
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
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
	if( libregf_key_item_get_number_of_values(
	     internal_key->key_item,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	if( number_of_values == 0 )
	{
		return( 0 );
	}
	while( utf8_string_index < utf8_string_length )
	{
		if( libuna_unicode_character_copy_from_utf8(
		     &unicode_character,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-8 string to Unicode character.",
			 function );

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libfdata_list_get_list_element_by_index(
		     internal_key->key_item->values_list,
		     value_index,
		     &values_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value item: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( libfdata_list_element_get_element_value(
		     values_list_element,
		     (intptr_t *) internal_key->file_io_handle,
		     (libfdata_cache_t *) internal_key->key_item->values_cache,
		     (intptr_t **) &value_item,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value item: %d values.",
			 function,
			 value_index );

			return( -1 );
		}
		result = libregf_value_item_compare_name_with_utf8_string(
			  value_item,
			  name_hash,
			  utf8_string,
			  utf8_string_length,
			  internal_key->io_handle->ascii_codepage,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare value name with UTF-8 string.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			break;
		}
	}
	if( value_index >= number_of_values )
	{
		return( 0 );
	}
	if( libfdata_list_element_get_data_range(
	     values_list_element,
	     &file_index,
	     &offset,
	     &size,
	     &flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data range.",
		 function );

		return( -1 );
	}
	/* The offset is relative from the start of the hive bins list
	 * and points to the start of the corresponding hive bin cell
	 */
	offset += internal_key->io_handle->hive_bins_list_offset + 4;

	if( libregf_value_initialize(
	     value,
	     internal_key->io_handle,
	     internal_key->file_io_handle,
	     offset,
	     value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value for the specific UTF-8 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Creates a new value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_value_by_utf8_name(
     libregf_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_value_by_utf8_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_internal_key_get_value_by_utf8_name(
	          internal_key,
	          utf8_string,
	          utf8_string_length,
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-8 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the value for the specific UTF-16 encoded name
 * To retrieve the default value specify string as NULL and its length as 0
 * Creates a new value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_internal_key_get_value_by_utf16_name(
     libregf_internal_key_t *internal_key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libfdata_list_element_t *values_list_element = NULL;
	libregf_value_item_t *value_item             = NULL;
	static char *function                        = "libregf_internal_key_get_value_by_utf16_name";
	libuna_unicode_character_t unicode_character = 0;
	size64_t size                                = 0;
	size_t utf16_string_index                    = 0;
	off64_t offset                               = 0;
	uint32_t flags                               = 0;
	uint32_t name_hash                           = 0;
	int file_index                               = 0;
	int number_of_values                         = 0;
	int result                                   = 0;
	int value_index                              = 0;

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_key->key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing key item.",
		 function );

		return( -1 );
	}
	if( ( utf16_string == NULL )
	 && ( utf16_string_length != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
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
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
	if( libregf_key_item_get_number_of_values(
	     internal_key->key_item,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	if( number_of_values == 0 )
	{
		return( 0 );
	}
	while( utf16_string_index < utf16_string_length )
	{
		if( libuna_unicode_character_copy_from_utf16(
		     &unicode_character,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 string to Unicode character.",
			 function );

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libfdata_list_get_list_element_by_index(
		     internal_key->key_item->values_list,
		     value_index,
		     &values_list_element,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value item: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( libfdata_list_element_get_element_value(
		     values_list_element,
		     (intptr_t *) internal_key->file_io_handle,
		     (libfdata_cache_t *) internal_key->key_item->values_cache,
		     (intptr_t **) &value_item,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value item: %d values.",
			 function,
			 value_index );

			return( -1 );
		}
		result = libregf_value_item_compare_name_with_utf16_string(
			  value_item,
			  name_hash,
			  utf16_string,
			  utf16_string_length,
			  internal_key->io_handle->ascii_codepage,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare value name with UTF-16 string.",
			 function );

			return( -1 );
		}
		else if( result != 0 )
		{
			break;
		}
	}
	if( value_index >= number_of_values )
	{
		return( 0 );
	}
	if( libfdata_list_element_get_data_range(
	     values_list_element,
	     &file_index,
	     &offset,
	     &size,
	     &flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data range.",
		 function );

		return( -1 );
	}
	/* The offset is relative from the start of the hive bins list
	 * and points to the start of the corresponding hive bin cell
	 */
	offset += internal_key->io_handle->hive_bins_list_offset + 4;

	if( libregf_value_initialize(
	     value,
	     internal_key->io_handle,
	     internal_key->file_io_handle,
	     offset,
	     value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value for the specific UTF-16 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Creates a new value
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_get_value_by_utf16_name(
     libregf_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_value_t **value,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_value_by_utf16_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_internal_key_get_value_by_utf16_name(
	          internal_key,
	          utf16_string,
	          utf16_string_length,
	          value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the number of sub keys
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_number_of_sub_keys(
     libregf_key_t *key,
     int *number_of_sub_keys,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_number_of_sub_keys";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_number_of_sub_key_descriptors(
	     internal_key->key_item,
	     number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub key descriptors.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves a specific sub key
 * Creates a new key
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_sub_key(
     libregf_key_t *key,
     int sub_key_index,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key         = NULL;
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libregf_key_get_sub_key";
	int result                                   = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_sub_key_descriptor_by_index(
	     internal_key->key_item,
             sub_key_index,
	     &sub_key_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %d descriptor.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( sub_key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub key: %d descriptor.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( libregf_key_initialize(
	          sub_key,
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          sub_key_descriptor->key_offset,
	          internal_key->hive_bins_list,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub key: %d.",
		 function,
		 sub_key_index );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves a specific sub key
 * Creates a new key
 * Returns 1 if successful or -1 on error
 */
int libregf_key_get_sub_key_by_index(
     libregf_key_t *key,
     int sub_key_index,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key         = NULL;
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libregf_key_get_sub_key_by_index";
	int result                                   = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	if( libregf_key_item_get_sub_key_descriptor_by_index(
	     internal_key->key_item,
             sub_key_index,
	     &sub_key_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %d descriptor.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( sub_key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub key: %d descriptor.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( libregf_key_initialize(
	          sub_key,
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          sub_key_descriptor->key_offset,
	          internal_key->hive_bins_list,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub key: %d.",
		 function,
		 sub_key_index );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the sub key for the specific UTF-8 encoded name
 * Creates a new key
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libregf_internal_key_get_sub_key_by_utf8_name(
     libregf_internal_key_t *internal_key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libregf_internal_key_get_sub_key_by_utf8_name";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf8_string_index                     = 0;
	uint32_t name_hash                           = 0;
	int result                                   = 0;

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
	while( utf8_string_index < utf8_string_length )
	{
		if( libuna_unicode_character_copy_from_utf8(
		     &unicode_character,
		     utf8_string,
		     utf8_string_length,
		     &utf8_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-8 string to Unicode character.",
			 function );

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	result = libregf_key_item_get_sub_key_descriptor_by_utf8_name(
	          internal_key->key_item,
	          internal_key->file_io_handle,
	          internal_key->hive_bins_list,
	          name_hash,
	          utf8_string,
	          utf8_string_length,
	          &sub_key_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor by UTF-8 name.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor.",
			 function );

			return( -1 );
		}
		if( libregf_key_initialize(
		     sub_key,
		     internal_key->io_handle,
		     internal_key->file_io_handle,
		     sub_key_descriptor->key_offset,
		     internal_key->hive_bins_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub key.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the sub key for the specific UTF-8 encoded name
 * Creates a new key
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libregf_key_get_sub_key_by_utf8_name(
     libregf_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_sub_key_by_utf8_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_internal_key_get_sub_key_by_utf8_name(
	          internal_key,
	          utf8_string,
	          utf8_string_length,
	          sub_key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-8 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the sub key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libregf_key_get_sub_key_by_utf8_path(
     libregf_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_sub_key_by_utf8_path";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_tree_get_sub_key_by_utf8_path(
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          internal_key->hive_bins_list,
	          internal_key->key_offset,
	          utf8_string,
	          utf8_string_length,
	          sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-8 path.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the sub key for the specific UTF-16 encoded name
 * Creates a new key
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libregf_internal_key_get_sub_key_by_utf16_name(
     libregf_internal_key_t *internal_key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libregf_internal_key_get_value_by_utf16_name";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf16_string_index                    = 0;
	uint32_t name_hash                           = 0;
	int result                                   = 0;

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
	while( utf16_string_index < utf16_string_length )
	{
		if( libuna_unicode_character_copy_from_utf16(
		     &unicode_character,
		     utf16_string,
		     utf16_string_length,
		     &utf16_string_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy UTF-16 string to Unicode character.",
			 function );

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	result = libregf_key_item_get_sub_key_descriptor_by_utf16_name(
	          internal_key->key_item,
	          internal_key->file_io_handle,
	          internal_key->hive_bins_list,
	          name_hash,
	          utf16_string,
	          utf16_string_length,
	          &sub_key_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key values by UTF-16 name.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor.",
			 function );

			return( -1 );
		}
		if( libregf_key_initialize(
		     sub_key,
		     internal_key->io_handle,
		     internal_key->file_io_handle,
		     sub_key_descriptor->key_offset,
		     internal_key->hive_bins_list,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub key.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the sub key for the specific UTF-16 encoded name
 * Creates a new key
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libregf_key_get_sub_key_by_utf16_name(
     libregf_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_sub_key_by_utf16_name";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_internal_key_get_sub_key_by_utf16_name(
	          internal_key,
	          utf16_string,
	          utf16_string_length,
	          sub_key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

/* Retrieves the sub key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libregf_key_get_sub_key_by_utf16_path(
     libregf_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **sub_key,
     libcerror_error_t **error )
{
	libregf_internal_key_t *internal_key = NULL;
	static char *function                = "libregf_key_get_sub_key_by_utf16_path";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libregf_internal_key_t *) key;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
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
	result = libregf_key_tree_get_sub_key_by_utf16_path(
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          internal_key->hive_bins_list,
	          internal_key->key_offset,
	          utf16_string,
	          utf16_string_length,
	          sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-16 path.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
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

