/*
 * Debug functions
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the named key flags
 * Returns 1 if successful or -1 on error
 */
void libregf_debug_print_named_key_flags(
      uint16_t named_key_flags )
{
	if( ( named_key_flags & 0x0001 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs volatile\n" );
	}
	if( ( named_key_flags & 0x0002 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs mount point\n" );
	}
	if( ( named_key_flags & 0x0004 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs root key\n" );
	}
	if( ( named_key_flags & 0x0008 ) != 0 )
	{
		libcnotify_printf(
		 "\tCannot be deleted\n" );
	}
	if( ( named_key_flags & 0x0010 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs symbolic link key\n" );
	}
	if( ( named_key_flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		libcnotify_printf(
		 "\tKey name is an ASCII string\n" );
	}
	if( ( named_key_flags & 0x0040 ) != 0 )
	{
		libcnotify_printf(
		 "\tIs predefined handle\n" );
	}

	libcnotify_printf(
	 "\n" );
}

/* Prints the value key flags
 * Returns 1 if successful or -1 on error
 */
void libregf_debug_print_value_key_flags(
      uint16_t value_key_flags )
{
	if( ( value_key_flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		libcnotify_printf(
		 "\tValue name is an ASCII string (VALUE_COMP_NAME)\n" );
	}
	libcnotify_printf(
	 "\n" );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libregf_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int number_of_offsets = 0;
	int offset_iterator   = 0;

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
	if( libbfio_handle_get_number_of_offsets_read(
	     file_io_handle,
	     &number_of_offsets,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of offsets read.",
		 function );

		return( -1 );
	}
	libcnotify_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < number_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 ( offset_iterator + 1 ) );

			return( -1 );
		}
		libcnotify_printf(
		 "%08" PRIi64 " ( 0x%08" PRIx64 " ) - %08" PRIi64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + (off64_t) size,
		 offset + (off64_t) size,
		 size );
	}
	libcnotify_printf(
	 "\n" );

	return( 1 );
}

#endif

