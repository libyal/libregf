/*
 * Security key functions
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

#include "libregf_io_handle.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfwnt.h"
#include "libregf_security_key.h"

#include "regf_cell_values.h"

/* Creates a security key
 * Make sure the value security_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_security_key_initialize(
     libregf_security_key_t **security_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_security_key_initialize";

	if( security_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security key.",
		 function );

		return( -1 );
	}
	if( *security_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security key value already set.",
		 function );

		return( -1 );
	}
	*security_key = memory_allocate_structure(
	                 libregf_security_key_t );

	if( *security_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create security key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *security_key,
	     0,
	     sizeof( libregf_security_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear security key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *security_key != NULL )
	{
		memory_free(
		 *security_key );

		*security_key = NULL;
	}
	return( -1 );
}

/* Frees a security key
 * Returns 1 if successful or -1 on error
 */
int libregf_security_key_free(
     libregf_security_key_t **security_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_security_key_free";

	if( security_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security key.",
		 function );

		return( -1 );
	}
	if( *security_key != NULL )
	{
		if( ( *security_key )->security_descriptor != NULL )
		{
			memory_free(
			 ( *security_key )->security_descriptor );
		}
		memory_free(
		 *security_key );

		*security_key = NULL;
	}
	return( 1 );
}

/* Reads a security key
 * Returns 1 if successful or -1 on error
 */
int libregf_security_key_read_data(
     libregf_security_key_t *security_key,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function             = "libregf_security_key_read_data";
	size_t data_offset                = 0;
	size_t security_key_data_size     = 0;
	uint32_t security_descriptor_size = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit              = 0;
	uint16_t value_16bit              = 0;
#endif

	if( security_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security key.",
		 function );

		return( -1 );
	}
	if( security_key->security_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid security key - security descriptor value already set.",
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
	security_key_data_size = sizeof( regf_security_key_t );

	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		security_key_data_size += 4;
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
	if( ( data_size < security_key_data_size )
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: security key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		data_offset += 4;
	}
	/* Check if the signature matches that of a security key: "sk"
	 */
	if( ( data[ data_offset ] != (uint8_t) 's' )
	 || ( data[ data_offset + 1 ] != (uint8_t) 'k' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported security key signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_security_key_t *) &( data[ data_offset ] ) )->security_descriptor_size,
	 security_descriptor_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( io_handle->major_version == 1 )
		 && ( io_handle->minor_version <= 1 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 data,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown0\t\t\t\t: 0x%08" PRIx32 " (%" PRIi32 ")\n",
			 function,
			 value_32bit,
			 (int32_t) value_32bit );
		}
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->signature[ 0 ],
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->signature[ 1 ] );

		byte_stream_copy_to_uint16_little_endian(
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
		 function,
		 value_16bit,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->previous_security_key_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: previous security key offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->next_security_key_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: next security key offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_security_key_t *) &( data[ data_offset ] ) )->reference_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: reference count\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: security descriptor size\t\t: %" PRIu32 "\n",
		 function,
		 security_descriptor_size );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += sizeof( regf_security_key_t );

	if( security_descriptor_size > ( data_size - data_offset ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid security descriptor size value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: security descriptor data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ data_offset ] ),
		 security_descriptor_size,
		 0 );
	}
#endif
	if( security_descriptor_size > 0 )
	{
		if( security_descriptor_size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid security descriptor size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		security_key->security_descriptor = (uint8_t *) memory_allocate(
		                                                 sizeof( uint8_t ) * security_descriptor_size );

		if( security_key->security_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create security descriptor.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     security_key->security_descriptor,
		     &( data[ data_offset ] ),
		     security_descriptor_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy security descriptor data.",
			 function );

			goto on_error;
		}
		security_key->security_descriptor_size = security_descriptor_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libregf_debug_print_security_descriptor_value(
			     security_key->security_descriptor,
			     security_key->security_descriptor_size,
			     LIBFWNT_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print security descriptor value.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	data_offset += security_descriptor_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 data_size - data_offset,
			 0 );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( security_key->security_descriptor != NULL )
	{
		memory_free(
		 security_key->security_descriptor );

		security_key->security_descriptor = NULL;
	}
	security_key->security_descriptor_size = 0;

	return( -1 );
}

