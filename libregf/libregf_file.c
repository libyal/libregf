/*
 * File functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libregf_codepage.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_file.h"
#include "libregf_file_header.h"
#include "libregf_hive_bin.h"
#include "libregf_hive_bins_list.h"
#include "libregf_io_handle.h"
#include "libregf_key.h"
#include "libregf_key_item.h"
#include "libregf_key_tree.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfcache.h"
#include "libregf_libfdata.h"
#include "libregf_libuna.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_file_initialize(
     libregf_file_t **file,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libregf_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libregf_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 internal_file );

		return( -1 );
	}
	if( libregf_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_file->read_write_lock ),
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
	*file = (libregf_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		if( internal_file->io_handle != NULL )
		{
			libregf_io_handle_free(
			 &( internal_file->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libregf_file_free(
     libregf_file_t **file,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_free";
	int result                             = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libregf_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libregf_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_file->read_write_lock ),
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
		if( libregf_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals the libregf file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libregf_file_signal_abort(
     libregf_file_t *file,
     libcerror_error_t **error )
{
	static char *function = "libregf_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	( (libregf_internal_file_t *) file )->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libregf_file_open(
     libregf_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_open";
	size_t filename_length                 = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
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
	if( ( ( access_flags & LIBREGF_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libregf_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_file->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libregf_file_open_wide(
     libregf_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle       = NULL;
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_open_wide";
	size_t filename_length                 = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
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
	if( ( ( access_flags & LIBREGF_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set track offsets read in file IO handle.",
		 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set filename in file IO handle.",
		 function );

		goto on_error;
	}
	if( libregf_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_file->file_io_handle_created_in_library = 1;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_file->file_io_handle_created_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libregf_file_open_file_io_handle(
     libregf_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file   = NULL;
	static char *function                    = "libregf_file_open_file_io_handle";
	uint8_t file_io_handle_opened_in_library = 0;
	int bfio_access_flags                    = 0;
	int file_io_handle_is_open               = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file IO handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBREGF_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBREGF_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBREGF_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		file_io_handle_opened_in_library = 1;
	}
	if( libregf_internal_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		goto on_error;
	}
#endif
	internal_file->file_io_handle                   = file_io_handle;
	internal_file->file_io_handle_opened_in_library = file_io_handle_opened_in_library;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		internal_file->file_io_handle                   = NULL;
		internal_file->file_io_handle_opened_in_library = 0;

		goto on_error;
	}
#endif
	return( 1 );

on_error:
	if( file_io_handle_opened_in_library != 0 )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );
	}
	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libregf_file_close(
     libregf_file_t *file,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_close";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libregf_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file IO handle.",
			 function );

			result = -1;
		}
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libregf_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( internal_file->file_header != NULL )
	{
		if( libregf_file_header_free(
		     &( internal_file->file_header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file header.",
			 function );

			result = -1;
		}
	}
	if( internal_file->dirty_vector != NULL )
	{
		if( libregf_dirty_vector_free(
		     &( internal_file->dirty_vector ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free dirty vector.",
			 function );

			result = -1;
		}
	}
	if( internal_file->hive_bins_list != NULL )
	{
		if( libregf_hive_bins_list_free(
		     &( internal_file->hive_bins_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free hive bins list.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
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

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libregf_internal_file_open_read(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_internal_file_open_read";
	size64_t file_size    = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - file header value already set.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libregf_file_header_initialize(
	     &( internal_file->file_header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( libregf_file_header_read_file_io_handle(
	     internal_file->file_header,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
	internal_file->io_handle->major_version = internal_file->file_header->major_format_version;
	internal_file->io_handle->minor_version = internal_file->file_header->minor_format_version;

	if( ( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_REGISTRY )
	 && ( file_size > 4096 ) )
	{
/* TODO print data between header and hive bins list offset ? */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading hive bins:\n" );
		}
#endif
		if( libregf_internal_file_read_hive_bins(
		     internal_file,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read file header.",
			 function );

			goto on_error;
		}
	}
	else if( ( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_TRANSACTION_LOG1 )
	      || ( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_TRANSACTION_LOG2 ) )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading dirty vector:\n" );
		}
#endif
		if( libregf_internal_file_read_dirty_vector(
		     internal_file,
		     file_io_handle,
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
	return( 1 );

on_error:
	if( internal_file->file_header != NULL )
	{
		libregf_file_header_free(
		 &( internal_file->file_header ),
		 NULL );
	}
	return( -1 );
}

/* Reads the hive bins
 * Returns 1 if successful or -1 on error
 */
int libregf_internal_file_read_hive_bins(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_internal_file_read_hive_bins";
	int result            = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( internal_file->hive_bins_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - hive bins list value already set.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->hive_bins_list_offset = 4096;

	if( libregf_hive_bins_list_initialize(
	     &( internal_file->hive_bins_list ),
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create hive bins list.",
		 function );

		goto on_error;
	}
	result = libregf_hive_bins_list_read(
		  internal_file->hive_bins_list,
		  file_io_handle,
		  internal_file->io_handle->hive_bins_list_offset,
		  internal_file->file_header->hive_bins_size,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hive bins.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( internal_file->hive_bins_list != NULL )
	{
		libregf_hive_bins_list_free(
		 &( internal_file->hive_bins_list ),
		 NULL );
	}
	return( -1 );
}

/* Reads the dirty vector
 * Returns 1 if successful or -1 on error
 */
int libregf_internal_file_read_dirty_vector(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_internal_file_read_dirty_vector";

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( internal_file->dirty_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - dirtry vector value already set.",
		 function );

		return( -1 );
	}
	if( libregf_dirty_vector_initialize(
	     &( internal_file->dirty_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create dirty vector.",
		 function );

		goto on_error;
	}
	if( libregf_dirty_vector_read_file_io_handle(
	     internal_file->dirty_vector,
	     file_io_handle,
	     512,
	     internal_file->file_header->hive_bins_size,
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
	return( 1 );

on_error:
	if( internal_file->dirty_vector != NULL )
	{
		libregf_dirty_vector_free(
		 &( internal_file->dirty_vector ),
		 NULL );
	}
	return( -1 );
}

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libregf_file_is_corrupted(
     libregf_file_t *file,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_is_corrupted";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
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
	if( ( internal_file->io_handle->flags & LIBREGF_IO_HANDLE_FLAG_IS_CORRUPTED ) != 0 )
	{
		result = 1;
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
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

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libregf_file_get_ascii_codepage(
     libregf_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ascii_codepage == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
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
	*ascii_codepage = internal_file->io_handle->ascii_codepage;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
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

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libregf_file_set_ascii_codepage(
     libregf_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_set_ascii_codepage";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( ascii_codepage != LIBREGF_CODEPAGE_ASCII )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_874 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_932 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_936 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_949 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_950 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1250 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1251 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1252 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1253 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1254 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1255 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1256 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1257 )
	 && ( ascii_codepage != LIBREGF_CODEPAGE_WINDOWS_1258 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
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
	internal_file->io_handle->ascii_codepage = ascii_codepage;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
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
	return( 1 );
}

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
int libregf_file_get_format_version(
     libregf_file_t *file,
     uint32_t *major_version,
     uint32_t *minor_version,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_format_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( major_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid major format version.",
		 function );

		return( -1 );
	}
	if( minor_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid minor format version.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
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
	*major_version = internal_file->file_header->major_format_version;
	*minor_version = internal_file->file_header->minor_format_version;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
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

/* Retrieves the type
 * Returns 1 if successful or -1 on error
 */
int libregf_file_get_type(
     libregf_file_t *file,
     uint32_t *file_type,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_type";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
	if( file_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_read(
	     internal_file->read_write_lock,
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
	*file_type = internal_file->file_header->file_type;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_read(
	     internal_file->read_write_lock,
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

/* Retrieves the root key
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libregf_file_get_root_key(
     libregf_file_t *file,
     libregf_key_t **key,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_root_key";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
		 function );

		return( -1 );
	}
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
		 "%s: key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
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
	if( ( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_REGISTRY )
	 && ( internal_file->file_header->root_key_offset != 0 ) )
	{
		result = libregf_key_initialize(
		          key,
		          internal_file->io_handle,
		          internal_file->file_io_handle,
		          internal_file->file_header->root_key_offset,
		          internal_file->hive_bins_list,
		          error );

		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create root key.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
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

/* Retrieves the key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libregf_file_get_key_by_utf8_path(
     libregf_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **key,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_key_by_utf8_path";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
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
		 "%s: key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
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
	if( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_REGISTRY )
	{
		result = libregf_key_tree_get_sub_key_by_utf8_path(
		          internal_file->io_handle,
		          internal_file->file_io_handle,
		          internal_file->hive_bins_list,
		          internal_file->file_header->root_key_offset,
		          utf8_string,
		          utf8_string_length,
		          key,
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
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
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

/* Retrieves the key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libregf_file_get_key_by_utf16_path(
     libregf_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **key,
     libcerror_error_t **error )
{
	libregf_internal_file_t *internal_file = NULL;
	static char *function                  = "libregf_file_get_key_by_utf16_path";
	int result                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libregf_internal_file_t *) file;

	if( internal_file->file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file header.",
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
		 "%s: key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_file->read_write_lock,
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
	if( internal_file->file_header->file_type == LIBREGF_FILE_TYPE_REGISTRY )
	{
		result = libregf_key_tree_get_sub_key_by_utf16_path(
		          internal_file->io_handle,
		          internal_file->file_io_handle,
		          internal_file->hive_bins_list,
		          internal_file->file_header->root_key_offset,
		          utf16_string,
		          utf16_string_length,
		          key,
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
	}
#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_file->read_write_lock,
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

