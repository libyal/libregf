/*
 * Named key functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_io_handle.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfdatetime.h"
#include "libregf_libuna.h"
#include "libregf_named_key.h"
#include "libregf_unused.h"

#include "regf_cell_values.h"

/* Creates a named key
 * Make sure the value named_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_initialize(
     libregf_named_key_t **named_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_initialize";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( *named_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid named key value already set.",
		 function );

		return( -1 );
	}
	*named_key = memory_allocate_structure(
	              libregf_named_key_t );

	if( *named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create named key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *named_key,
	     0,
	     sizeof( libregf_named_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear named key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *named_key != NULL )
	{
		memory_free(
		 *named_key );

		*named_key = NULL;
	}
	return( -1 );
}

/* Frees a named key
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_free(
     libregf_named_key_t **named_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_free";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( *named_key != NULL )
	{
		if( ( *named_key )->name != NULL )
		{
			memory_free(
			 ( *named_key )->name );
		}
		memory_free(
		 *named_key );

		*named_key = NULL;
	}
	return( 1 );
}

/* Reads a named key
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_read_data(
     libregf_named_key_t *named_key,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     uint32_t named_key_hash LIBREGF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function                        = "libregf_named_key_read_data";
	libuna_unicode_character_t unicode_character = 0;
	size_t data_offset                           = 0;
	size_t name_index                            = 0;
	size_t named_key_data_size                   = 0;
	int result                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                         = 0;
#endif

	LIBREGF_UNREFERENCED_PARAMETER( named_key_hash )

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( named_key->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid named key - name value already set.",
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
	named_key_data_size = sizeof( regf_named_key_t );

	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		named_key_data_size += 4;
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
	if( ( data_size < named_key_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds (%zd < %zd).",
		 function, data_size, named_key_data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: named key data:\n",
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
	/* Check if the signature matches that of a named key: "nk"
	 */
	if( ( data[ data_offset ] != (uint8_t) 'n' )
	 || ( data[ data_offset + 1 ] != (uint8_t) 'k' ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported named key signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->flags,
	 named_key->flags );

	byte_stream_copy_to_uint64_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->last_written_time,
	 named_key->last_written_time );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->number_of_sub_keys,
	 named_key->number_of_sub_keys );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->sub_keys_list_offset,
	 named_key->sub_keys_list_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->number_of_values,
	 named_key->number_of_values );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->values_list_offset,
	 named_key->values_list_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->security_key_offset,
	 named_key->security_key_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->class_name_offset,
	 named_key->class_name_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->key_name_size,
	 named_key->name_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_named_key_t *) &( data[ data_offset ] ) )->class_name_size,
	 named_key->class_name_size );

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
			 "%s: unknown0\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIi32 ")\n",
			 function,
			 value_32bit,
			 (int32_t) value_32bit );
		}
		libcnotify_printf(
		 "%s: signature\t\t\t\t\t: %c%c\n",
		 function,
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->signature[ 0 ],
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 named_key->flags );
		libregf_debug_print_named_key_flags(
		 named_key->flags );

		if( libregf_debug_print_filetime_value(
		     function,
		     "last written time\t\t\t\t",
		     ( (regf_named_key_t *) &( data[ data_offset ] ) )->last_written_time,
		     8,
		     LIBFDATETIME_ENDIAN_LITTLE,
		     LIBFDATETIME_STRING_FORMAT_TYPE_CTIME | LIBFDATETIME_STRING_FORMAT_FLAG_DATE_TIME_NANO_SECONDS,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print FILETIME value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->parent_key_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: parent key offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of sub keys\t\t\t\t: %" PRIu32 "\n",
		 function,
		 named_key->number_of_sub_keys );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->number_of_volatile_sub_keys,
		 value_32bit );
		libcnotify_printf(
		 "%s: number of volatile sub keys\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: sub keys list offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 named_key->sub_keys_list_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->volatile_sub_keys_list_offset,
		 value_32bit );
		libcnotify_printf(
		 "%s: volatile sub keys list offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of values\t\t\t\t: %" PRIu32 "\n",
		 function,
		 named_key->number_of_values );

		libcnotify_printf(
		 "%s: values list offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 named_key->values_list_offset );

		libcnotify_printf(
		 "%s: security key offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 named_key->security_key_offset );

		libcnotify_printf(
		 "%s: class name offset\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 named_key->class_name_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->largest_sub_key_name_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: largest sub key name size\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->largest_sub_key_class_name_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: largest sub key class name size\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->largest_value_name_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: largest value name size\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->largest_value_data_size,
		 value_32bit );
		libcnotify_printf(
		 "%s: largest value data size\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (regf_named_key_t *) &( data[ data_offset ] ) )->unknown6,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown6\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "%s: key name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 named_key->name_size );

		libcnotify_printf(
		 "%s: class name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 named_key->class_name_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += sizeof( regf_named_key_t );

	if( ( named_key->name_size == 0 )
	 || ( named_key->name_size > ( data_size - data_offset ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key name size value out of bounds.",
		 function );

		goto on_error;
	}
	named_key->name = (uint8_t *) memory_allocate(
	                               sizeof( uint8_t ) * (size_t) named_key->name_size );

	if( named_key->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key name.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     named_key->name,
	     &( data[ data_offset ] ),
	     (size_t) named_key->name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy key name.",
		 function );

		goto on_error;
	}
	named_key->name_hash = 0;

	while( name_index < (size_t) named_key->name_size )
	{
		if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
		{
			result = libuna_unicode_character_copy_from_byte_stream(
				  &unicode_character,
				  named_key->name,
				  (size_t) named_key->name_size,
				  &name_index,
				  io_handle->ascii_codepage,
				  error );
		}
		else
		{
			result = libuna_unicode_character_copy_from_utf16_stream(
				  &unicode_character,
				  named_key->name,
				  (size_t) named_key->name_size,
				  &name_index,
				  LIBUNA_ENDIAN_LITTLE,
				  error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy key name to Unicode character.",
			 function );

			goto on_error;
		}
		named_key->name_hash *= 37;
		named_key->name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
		{
			if( libregf_debug_print_string_value(
			     function,
			     "key name\t\t\t\t\t",
			     named_key->name,
			     (size_t) named_key->name_size,
			     io_handle->ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print string value.",
				 function );

				goto on_error;
			}
		}
		else
		{
			if( libregf_debug_print_utf16_string_value(
			     function,
			     "key name\t\t\t\t\t",
			     named_key->name,
			     (size_t) named_key->name_size,
			     LIBUNA_ENDIAN_LITTLE,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print UTF-16 string value.",
				 function );

				goto on_error;
			}
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key name string size.",
			 function );

			goto on_error;
		}
		libcnotify_printf(
		 "%s: key name hash\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 named_key->name_hash );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += named_key->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( named_key_hash != 0 )
		 && ( named_key_hash != named_key->name_hash ) )
		{
			libcnotify_printf(
			 "%s: mismatch in name hash ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
			 function,
			 named_key_hash,
			 named_key->name_hash );
		}
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
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( named_key->name != NULL )
	{
		memory_free(
		 named_key->name );

		named_key->name = NULL;
	}
	named_key->name_size = 0;

	return( -1 );
}

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_name_size(
     libregf_named_key_t *named_key,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_name_size";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
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
	*name_size = named_key->name_size;

	return( 1 );
}

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_name(
     libregf_named_key_t *named_key,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_name";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
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
	if( name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size < named_key->name_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid name size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     name,
	     named_key->name,
	     named_key->name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_utf8_name_size(
     libregf_named_key_t *named_key,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_utf8_name_size";
	int result            = 1;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf8_string_size_from_byte_stream(
			  named_key->name,
			  (size_t) named_key->name_size,
			  ascii_codepage,
			  utf8_string_size,
			  error );
	}
	else
	{
		result = libuna_utf8_string_size_from_utf16_stream(
			  named_key->name,
			  (size_t) named_key->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf8_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_utf8_name(
     libregf_named_key_t *named_key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_utf8_name";
	int result            = 1;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf8_string_copy_from_byte_stream(
			  utf8_string,
			  utf8_string_size,
			  named_key->name,
			  (size_t) named_key->name_size,
			  ascii_codepage,
			  error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_string,
			  utf8_string_size,
			  named_key->name,
			  (size_t) named_key->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_utf16_name_size(
     libregf_named_key_t *named_key,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_utf16_name_size";
	int result            = 1;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf16_string_size_from_byte_stream(
			  named_key->name,
			  (size_t) named_key->name_size,
			  ascii_codepage,
			  utf16_string_size,
			  error );
	}
	else
	{
		result = libuna_utf16_string_size_from_utf16_stream(
			  named_key->name,
			  (size_t) named_key->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  utf16_string_size,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_utf16_name(
     libregf_named_key_t *named_key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_utf16_name";
	int result            = 1;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf16_string_copy_from_byte_stream(
			  utf16_string,
			  utf16_string_size,
			  named_key->name,
			  (size_t) named_key->name_size,
			  ascii_codepage,
			  error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_string,
			  utf16_string_size,
			  named_key->name,
			  (size_t) named_key->name_size,
			  LIBUNA_ENDIAN_LITTLE,
			  error );
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares the key name with UTF-8 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_named_key_compare_name_with_utf8_string(
     libregf_named_key_t *named_key,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libregf_named_key_compare_name_with_utf8_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;
	int character_compare_result                = 0;
	int result                                  = 0;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( named_key->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid named key - missing name.",
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
	/* Do a full compare if there no name hash was provided or the name hash matches
	 */
	if( ( name_hash == 0 )
	 || ( named_key->name_hash == 0 )
	 || ( named_key->name_hash == name_hash ) )
	{
		while( name_index < (size_t) named_key->name_size )
		{
			if( utf8_string_index >= utf8_string_length )
			{
				break;
			}
			if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
					  &name_character,
					  named_key->name,
					  (size_t) named_key->name_size,
					  &name_index,
					  ascii_codepage,
					  error );
			}
			else
			{
				result = libuna_unicode_character_copy_from_utf16_stream(
					  &name_character,
					  named_key->name,
					  (size_t) named_key->name_size,
					  &name_index,
					  LIBUNA_ENDIAN_LITTLE,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy key name to Unicode character.",
				 function );

				return( -1 );
			}
			if( libuna_unicode_character_copy_from_utf8(
			     &string_character,
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
			character_compare_result = towupper( (wint_t) name_character ) == towupper( (wint_t) string_character );

			if( character_compare_result == 0 )
			{
				break;
			}
		}
		if( ( character_compare_result != 0 )
		 && ( name_index == (size_t) named_key->name_size )
		 && ( utf8_string_index == utf8_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Compares the key name with UTF-16 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_named_key_compare_name_with_utf16_string(
     libregf_named_key_t *named_key,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libregf_named_key_compare_name_with_utf16_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;
	int character_compare_result                = 0;
	int result                                  = 0;

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( named_key->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid named key - missing name.",
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
	/* Do a full compare if there no name hash was provided or the name hash matches
	 */
	if( ( name_hash == 0 )
	 || ( named_key->name_hash == 0 )
	 || ( named_key->name_hash == name_hash ) )
	{
		while( name_index < (size_t) named_key->name_size )
		{
			if( utf16_string_index >= utf16_string_length )
			{
				break;
			}
			if( ( named_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
					  &name_character,
					  named_key->name,
					  (size_t) named_key->name_size,
					  &name_index,
					  ascii_codepage,
					  error );
			}
			else
			{
				result = libuna_unicode_character_copy_from_utf16_stream(
					  &name_character,
					  named_key->name,
					  (size_t) named_key->name_size,
					  &name_index,
					  LIBUNA_ENDIAN_LITTLE,
					  error );
			}
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy key name to Unicode character.",
				 function );

				return( -1 );
			}
			if( libuna_unicode_character_copy_from_utf16(
			     &string_character,
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
			character_compare_result = towupper( (wint_t) name_character ) == towupper( (wint_t) string_character );

			if( character_compare_result == 0 )
			{
				break;
			}
		}
		if( ( character_compare_result != 0 )
		 && ( name_index == (size_t) named_key->name_size )
		 && ( utf16_string_index == utf16_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Retrieves the 64-bit FILETIME value of the last written date and time
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_last_written_time(
     libregf_named_key_t *named_key,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_last_written_time";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( filetime == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filetime.",
		 function );

		return( -1 );
	}
	*filetime = named_key->last_written_time;

	return( 1 );
}

/* Retrieves the number of sub keys
 * Returns 1 if successful or -1 on error
 */
int libregf_named_key_get_number_of_sub_keys(
     libregf_named_key_t *named_key,
     uint32_t *number_of_sub_keys,
     libcerror_error_t **error )
{
	static char *function = "libregf_named_key_get_number_of_sub_keys";

	if( named_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid named key.",
		 function );

		return( -1 );
	}
	if( number_of_sub_keys == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub keys.",
		 function );

		return( -1 );
	}
	*number_of_sub_keys = named_key->number_of_sub_keys;

	return( 1 );
}

