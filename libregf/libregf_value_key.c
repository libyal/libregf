/*
 * Value key functions
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libregf_data_type.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
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
		memory_free(
		 *value_key );

		*value_key = NULL;
	}
	return( 1 );
}

/* Reads a value key
 * Returns 1 if successful, 0 if the signature does not match or -1 on error
 */
int libregf_value_key_read_data(
     libregf_value_key_t *value_key,
     const uint8_t *data,
     size_t data_size,
     uint32_t value_key_hash LIBREGF_ATTRIBUTE_UNUSED,
     int ascii_codepage LIBREGF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function                        = "libregf_value_key_read_data";
	libuna_unicode_character_t unicode_character = 0;
	size_t data_offset                           = 0;
	size_t name_index                            = 0;
	int result                                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit                         = 0;
#endif

	LIBREGF_UNREFERENCED_PARAMETER( value_key_hash )
	LIBREGF_UNREFERENCED_PARAMETER( ascii_codepage )

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
	if( data_size < sizeof( regf_value_key_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
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
	/* Check if the cell signature matches that of a value key: "vk"
	 */
	if( ( data[ 0 ] != (uint8_t) 'v' )
	 || ( data[ 1 ] != (uint8_t) 'k' ) )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_value_key_t *) data )->value_name_size,
	 value_key->name_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) data )->data_size,
	 value_key->data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) data )->data_offset,
	 value_key->data_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (regf_value_key_t *) data )->data_type,
	 value_key->type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_value_key_t *) data )->flags,
	 value_key->flags );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ( (regf_value_key_t *) data )->signature[ 0 ],
		 ( (regf_value_key_t *) data )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: value name size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_key->name_size );

		libcnotify_printf(
		 "%s: value data size\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_key->data_size,
		 value_key->data_size & 0x5fffffffUL );

		libcnotify_printf(
		 "%s: value data offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_key->data_offset );

		libcnotify_printf(
		 "%s: data type\t\t\t\t: 0x%08" PRIx32 " (%s) %s\n",
		 function,
		 value_key->type,
		 libregf_data_type_get_identifier(
		  value_key->type ),
		 libregf_data_type_get_description(
		  value_key->type ) );

		libcnotify_printf(
		 "%s: flags\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_key->flags );
		libregf_debug_print_value_key_flags(
		 value_key->flags );

		byte_stream_copy_to_uint16_little_endian(
		 ( (regf_value_key_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
		 function,
		 value_16bit,
		 value_16bit );
	}
#endif
	data_offset = sizeof( regf_value_key_t );

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
			if( ( value_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				result = libuna_unicode_character_copy_from_byte_stream(
					  &unicode_character,
					  value_key->name,
					  (size_t) value_key->name_size,
					  &name_index,
					  ascii_codepage,
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
			if( ( value_key->flags & LIBREGF_NAMED_KEY_FLAG_NAME_IS_ASCII ) != 0 )
			{
				if( libregf_debug_print_string_value(
				     function,
				     "value name\t\t\t\t",
				     value_key->name,
				     (size_t) value_key->name_size,
				     ascii_codepage,
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
				     "value name\t\t\t\t",
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
			 "%s: value name hash\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_key->name_hash );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: value name\t\t\t\t: (default)\n",
		 function );

		libcnotify_printf(
		 "%s: value name hash\t\t\t: 0x%08" PRIx32 "\n",
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
	if( value_key->name != NULL )
	{
		memory_free(
		 value_key->name );

		value_key->name = NULL;
	}
	value_key->name_size = 0;

	return( -1 );
}

