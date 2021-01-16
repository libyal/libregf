/*
 * Value key functions
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libregf_data_type.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_io_handle.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libuna.h"
#include "libregf_value_key.h"
#include "libregf_unused.h"

#include "regf_cell_values.h"

/* Creates a value key
 * Make sure the value value_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_initialize(
     libregf_value_key_t **value_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_initialize";

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( *value_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value key value already set.",
		 function );

		return( -1 );
	}
	*value_key = memory_allocate_structure(
	              libregf_value_key_t );

	if( *value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *value_key,
	     0,
	     sizeof( libregf_value_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_key != NULL )
	{
		memory_free(
		 *value_key );

		*value_key = NULL;
	}
	return( -1 );
}

/* Frees a value key
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_free(
     libregf_value_key_t **value_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_free";

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( *value_key != NULL )
	{
		if( ( *value_key )->name != NULL )
		{
			memory_free(
			 ( *value_key )->name );
		}
		if( ( *value_key )->data != NULL )
		{
			memory_free(
			 ( *value_key )->data );
		}
		memory_free(
		 *value_key );

		*value_key = NULL;
	}
	return( 1 );
}

/* Clones (duplicates) the value key
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_clone(
     libregf_value_key_t **destination_value_key,
     libregf_value_key_t *source_value_key,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_clone";

	if( destination_value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination value key.",
		 function );

		return( -1 );
	}
	if( *destination_value_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination value key already set.",
		 function );

		return( -1 );
	}
	if( source_value_key == NULL )
	{
		*destination_value_key = NULL;

		return( 1 );
	}
	if( libregf_value_key_initialize(
	     destination_value_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination value key.",
		 function );

		goto on_error;
	}
	if( source_value_key->name != NULL )
	{
		if( source_value_key->name_size == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid source value key - invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
		( *destination_value_key )->name = (uint8_t *) memory_allocate(
		                                                sizeof( uint8_t ) * (size_t) source_value_key->name_size );

		if( ( *destination_value_key )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_value_key )->name,
		     source_value_key->name,
		     (size_t) source_value_key->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value name.",
			 function );

			goto on_error;
		}
		( *destination_value_key )->name_size = source_value_key->name_size;
		( *destination_value_key )->name_hash = source_value_key->name_hash;
	}
	if( source_value_key->data != NULL )
	{
		if( ( source_value_key->data_size == 0 )
		 || ( source_value_key->data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid source value key - invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		( *destination_value_key )->data = (uint8_t *) memory_allocate(
		                                                sizeof( uint8_t ) * (size_t) source_value_key->data_size );

		if( ( *destination_value_key )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_value_key )->data,
		     source_value_key->data,
		     (size_t) source_value_key->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data.",
			 function );

			goto on_error;
		}
		( *destination_value_key )->data_size = source_value_key->data_size;
	}
	( *destination_value_key )->value_type  = source_value_key->value_type;
	( *destination_value_key )->flags       = source_value_key->flags;
	( *destination_value_key )->data_offset = source_value_key->data_offset;
	( *destination_value_key )->data_in_key = source_value_key->data_in_key;

	return( 1 );

on_error:
	libregf_value_key_free(
	 destination_value_key,
	 NULL );

	return( -1 );
}

/* Reads a value key
 * Returns 1 if successful, 0 if the signature does not match or -1 on error
 */
int libregf_value_key_read_data(
     libregf_value_key_t *value_key,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     uint32_t value_key_hash LIBREGF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function                        = "libregf_value_key_read_data";
	libuna_unicode_character_t unicode_character = 0;
	size_t data_offset                           = 0;
	size_t name_index                            = 0;
	size_t value_key_data_size                   = 0;
	uint32_t value_data_offset                   = 0;
	uint8_t correct_string_size                  = 0;
	int result                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                         = 0;
	uint16_t value_16bit                         = 0;
#endif

	LIBREGF_UNREFERENCED_PARAMETER( value_key_hash )

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_key->name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value key - name value already set.",
		 function );

		return( -1 );
	}
	if( value_key->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value key - data value already set.",
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
	value_key_data_size = sizeof( regf_value_key_t );

	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		value_key_data_size += 4;
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
	if( ( data_size < value_key_data_size )
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
		 "%s: value key data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		data_offset += 4;
	}
	/* Check if the signature matches that of a value key: "vk"
	 */
	if( ( data[ data_offset ] != (uint8_t) 'v' )
	 || ( data[ data_offset + 1 ] != (uint8_t) 'k' ) )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_value_key_t *) &( data[ data_offset ] ) )->value_name_size,
	 value_key->name_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) &( data[ data_offset ] ) )->data_size,
	 value_key->data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) &( data[ data_offset ] ) )->data_offset,
	 value_data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) &( data[ data_offset ] ) )->value_type,
	 value_key->value_type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_value_key_t *) &( data[ data_offset ] ) )->flags,
	 value_key->flags );

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
		 ( (regf_value_key_t *) &( data[ data_offset ] ) )->signature[ 0 ],
		 ( (regf_value_key_t *) &( data[ data_offset ] ) )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: value name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_key->name_size );

		libcnotify_printf(
		 "%s: value data size\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_key->data_size,
		 value_key->data_size & 0x5fffffffUL );

		if( ( value_key->data_size & 0x80000000UL ) == 0 )
		{
			libcnotify_printf(
			 "%s: value data offset\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_data_offset );
		}
		else
		{
			libcnotify_printf(
			 "%s: value data:\n",
			 function );
			libcnotify_print_data(
			 ( (regf_value_key_t *) &( data[ data_offset ] ) )->data_offset,
			 4,
			 0 );
		}
		libcnotify_printf(
		 "%s: value type\t\t\t\t\t: 0x%08" PRIx32 " (%s) %s\n",
		 function,
		 value_key->value_type,
		 libregf_data_type_get_identifier(
		  value_key->value_type ),
		 libregf_data_type_get_description(
		  value_key->value_type ) );

		libcnotify_printf(
		 "%s: flags\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_key->flags );
		libregf_debug_print_value_key_flags(
		 value_key->flags );

		byte_stream_copy_to_uint16_little_endian(
		 ( (regf_value_key_t *) &( data[ data_offset ] ) )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
		 function,
		 value_16bit,
		 value_16bit );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( ( value_key->data_size & 0x80000000UL ) != 0 )
	{
		value_key->data_in_key = 1;
	}
/* TODO should this bitmask be 0x5fffffffUL ? */
	value_key->data_size &= 0x7fffffffUL;

	if( value_key->data_in_key == 0 )
	{
		value_key->data_offset = value_data_offset;
	}
	else
	{
/* TODO mark value as corrupted and handle issue */
		if( value_key->data_size > 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			goto on_error;
		}
		if( value_key->data_size > 0 )
		{
			if( ( value_key->value_type == LIBREGF_VALUE_TYPE_STRING )
			 || ( value_key->value_type == LIBREGF_VALUE_TYPE_EXPANDABLE_STRING ) )
			{
				if( ( value_key->data_size == 1 )
				 || ( value_key->data_size == 3 ) )
				{
					correct_string_size = 1;

					value_key->data_size += 1;
				}
			}
			if( value_key->data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid value key - invalid data size value exceeds maximum allocation size.",
				 function );

				goto on_error;
			}
			value_key->data = (uint8_t *) memory_allocate(
			                               sizeof( uint8_t ) * value_key->data_size );

			if( value_key->data == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create data.",
				 function );

				goto on_error;
			}
			value_key->data_size = (size_t) value_key->data_size;

			if( memory_copy(
			     value_key->data,
			     &( ( ( (regf_value_key_t *) &( data[ data_offset ] ) )->data_offset )[ 4 - value_key->data_size ] ),
			     value_key->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy data.",
				 function );

				goto on_error;
			}
			if( correct_string_size != 0 )
			{
				/* Make sure remnant data in the string is zero-ed out
				 */
				value_key->data[ value_key->data_size - 1 ] = 0;
			}
		}
	}
	data_offset += sizeof( regf_value_key_t );

	if( value_key->name_size > 0 )
	{
/* TODO mark value as corrupted and handle issue */
		if( value_key->name_size > ( data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value name size value out of bounds.",
			 function );

			goto on_error;
		}
		value_key->name = (uint8_t *) memory_allocate(
		                               sizeof( uint8_t ) * (size_t) value_key->name_size );

		if( value_key->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     value_key->name,
		     &( data[ data_offset ] ),
		     (size_t) value_key->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value name.",
			 function );

			goto on_error;
		}
		value_key->name_hash = 0;

		while( name_index < (size_t) value_key->name_size )
		{
			if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
					  &unicode_character,
					  value_key->name,
					  (size_t) value_key->name_size,
					  &name_index,
					  io_handle->ascii_codepage,
					  error );
			}
			else
			{
				result = libuna_unicode_character_copy_from_utf16_stream(
					  &unicode_character,
					  value_key->name,
					  (size_t) value_key->name_size,
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
				 "%s: unable to copy value name to Unicode character.",
				 function );

				goto on_error;
			}
			value_key->name_hash *= 37;
			value_key->name_hash += (uint32_t) towupper( (wint_t) unicode_character );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				if( libregf_debug_print_string_value(
				     function,
				     "value name\t\t\t\t\t",
				     value_key->name,
				     (size_t) value_key->name_size,
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
				     "value name\t\t\t\t\t",
				     value_key->name,
				     (size_t) value_key->name_size,
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
				 "%s: unable to retrieve value name string size.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: value name hash\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_key->name_hash );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value name\t\t\t\t\t: (default)\n",
		 function );

		libcnotify_printf(
		 "%s: value name hash\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_key->name_hash );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += value_key->name_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( value_key_hash != 0 )
		 && ( value_key_hash != value_key->name_hash ) )
		{
			libcnotify_printf(
			 "%s: mismatch in name hash ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
			 function,
			 value_key_hash,
			 value_key->name_hash );
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
	if( value_key->data != NULL )
	{
		memory_free(
		 value_key->data );

		value_key->data = NULL;
	}
	value_key->data_size = 0;

	if( value_key->name != NULL )
	{
		memory_free(
		 value_key->name );

		value_key->name = NULL;
	}
	value_key->name_size = 0;

	return( -1 );
}

/* Retrieves the value name size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_name_size(
     libregf_value_key_t *value_key,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_name_size";

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
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
	*name_size = value_key->name_size;

	return( 1 );
}

/* Retrieves the value name
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_name(
     libregf_value_key_t *value_key,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_name";

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
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
	if( name_size < value_key->name_size )
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
	     value_key->name,
	     value_key->name_size ) == NULL )
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

/* Retrieves the UTF-8 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_utf8_name_size(
     libregf_value_key_t *value_key,
     size_t *utf8_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_utf8_name_size";
	int result            = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_key->name == NULL )
	{
		if( utf8_name_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 name size.",
			 function );

			return( -1 );
		}
		*utf8_name_size = 0;
	}
	else
	{
		if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
		{
			result = libuna_utf8_string_size_from_byte_stream(
				  value_key->name,
				  (size_t) value_key->name_size,
				  ascii_codepage,
				  utf8_name_size,
				  error );
		}
		else
		{
			result = libuna_utf8_string_size_from_utf16_stream(
				  value_key->name,
				  (size_t) value_key->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  utf8_name_size,
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
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_utf8_name(
     libregf_value_key_t *value_key,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_utf8_name";
	int result            = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_key->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value key - missing name.",
		 function );

		return( -1 );
	}
	if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf8_string_copy_from_byte_stream(
			  utf8_name,
			  utf8_name_size,
			  value_key->name,
			  (size_t) value_key->name_size,
			  ascii_codepage,
			  error );
	}
	else
	{
		result = libuna_utf8_string_copy_from_utf16_stream(
			  utf8_name,
			  utf8_name_size,
			  value_key->name,
			  (size_t) value_key->name_size,
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

/* Retrieves the UTF-16 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_utf16_name_size(
     libregf_value_key_t *value_key,
     size_t *utf16_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_utf16_name_size";
	int result            = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_key->name == NULL )
	{
		if( utf16_name_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 name size.",
			 function );

			return( -1 );
		}
		*utf16_name_size = 0;
	}
	else
	{
		if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
		{
			result = libuna_utf16_string_size_from_byte_stream(
				  value_key->name,
				  (size_t) value_key->name_size,
				  ascii_codepage,
				  utf16_name_size,
				  error );
		}
		else
		{
			result = libuna_utf16_string_size_from_utf16_stream(
				  value_key->name,
				  (size_t) value_key->name_size,
				  LIBUNA_ENDIAN_LITTLE,
				  utf16_name_size,
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
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_utf16_name(
     libregf_value_key_t *value_key,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_utf16_name";
	int result            = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_key->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value key - missing name.",
		 function );

		return( -1 );
	}
	if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
	{
		result = libuna_utf16_string_copy_from_byte_stream(
			  utf16_name,
			  utf16_name_size,
			  value_key->name,
			  (size_t) value_key->name_size,
			  ascii_codepage,
			  error );
	}
	else
	{
		result = libuna_utf16_string_copy_from_utf16_stream(
			  utf16_name,
			  utf16_name_size,
			  value_key->name,
			  (size_t) value_key->name_size,
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

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
int libregf_value_key_get_value_type(
     libregf_value_key_t *value_key,
     uint32_t *value_type,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_key_get_value_type";

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type.",
		 function );

		return( -1 );
	}
	*value_type = value_key->value_type;

	return( 1 );
}

/* Compares the value name with UTF-8 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_value_key_compare_name_with_utf8_string(
     libregf_value_key_t *value_key,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libregf_value_key_compare_name_with_utf8_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;
	int result                                  = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
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
	/* Check if we are looking for the nameless default value
	 */
	if( value_key->name == NULL )
	{
		if( ( utf8_string == NULL )
		 && ( utf8_string_length == 0 ) )
		{
			return( 1 );
		}
	}
	else if( ( name_hash == 0 )
	      || ( value_key->name_hash == 0 )
	      || ( value_key->name_hash == name_hash ) )
	{
		/* Do a full compare if there no name hash was provided or the name hash matches
		 */
		while( name_index < (size_t) value_key->name_size )
		{
			if( utf8_string_index >= utf8_string_length )
			{
				break;
			}
			if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
					  &name_character,
					  value_key->name,
					  (size_t) value_key->name_size,
					  &name_index,
					  ascii_codepage,
					  error );
			}
			else
			{
				result = libuna_unicode_character_copy_from_utf16_stream(
					  &name_character,
					  value_key->name,
					  (size_t) value_key->name_size,
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
				 "%s: unable to copy value name to Unicode character.",
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
			if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
			{
				break;
			}
		}
		if( ( name_index == (size_t) value_key->name_size )
		 && ( utf8_string_index == utf8_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Compares the value name with UTF-16 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_value_key_compare_name_with_utf16_string(
     libregf_value_key_t *value_key,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libregf_value_key_compare_name_with_utf16_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;
	int result                                  = 0;

	if( value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value key.",
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
	/* Check if we are looking for the nameless default value
	 */
	if( value_key->name == NULL )
	{
		if( ( utf16_string == NULL )
		 && ( utf16_string_length == 0 ) )
		{
			return( 1 );
		}
	}
	else if( ( name_hash == 0 )
	      || ( value_key->name_hash == 0 )
	      || ( value_key->name_hash == name_hash ) )
	{
		/* Do a full compare if there no name hash was provided or the name hash matches
		 */
		while( name_index < (size_t) value_key->name_size )
		{
			if( utf16_string_index >= utf16_string_length )
			{
				break;
			}
			if( ( value_key->flags & LIBREGF_VALUE_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
				          &name_character,
				          value_key->name,
				          (size_t) value_key->name_size,
				          &name_index,
				          ascii_codepage,
				          error );
			}
			else
			{
				result = libuna_unicode_character_copy_from_utf16_stream(
				          &name_character,
				          value_key->name,
				          (size_t) value_key->name_size,
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
				 "%s: unable to copy value name to Unicode character.",
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
			if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
			{
				break;
			}
		}
		if( ( name_index == (size_t) value_key->name_size )
		 && ( utf16_string_index == utf16_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

