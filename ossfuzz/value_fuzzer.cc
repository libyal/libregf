/*
 * OSS-Fuzz target for libregf value type
 *
 * Copyright (C) 2009-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libregf.h"

#if !defined( LIBREGF_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBREGF_EXTERN \
int libregf_file_open_file_io_handle(
      libregf_file_t *file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libregf_error_t **error );

#endif /* !defined( LIBREGF_HAVE_BFIO ) */

/* Limits on the traversal and buffer sizes to bound the time a single input takes
 */
#define FUZZER_KEY_LIMIT         4
#define FUZZER_VALUE_LIMIT       8
#define FUZZER_STRING_SIZE_LIMIT ( (size_t) 0x00100000 )
#define FUZZER_LOOKUP_STRING_LIMIT ( (size_t) 0x4000 )
#define FUZZER_KEY_DEPTH_LIMIT   7

/* The ASCII codepages supported by libregf_file_set_ascii_codepage
 */
static const int value_codepages[] = {
	LIBREGF_CODEPAGE_ASCII,
	LIBREGF_CODEPAGE_WINDOWS_874,
	LIBREGF_CODEPAGE_WINDOWS_932,
	LIBREGF_CODEPAGE_WINDOWS_936,
	LIBREGF_CODEPAGE_WINDOWS_949,
	LIBREGF_CODEPAGE_WINDOWS_950,
	LIBREGF_CODEPAGE_WINDOWS_1250,
	LIBREGF_CODEPAGE_WINDOWS_1251,
	LIBREGF_CODEPAGE_WINDOWS_1252,
	LIBREGF_CODEPAGE_WINDOWS_1253,
	LIBREGF_CODEPAGE_WINDOWS_1254,
	LIBREGF_CODEPAGE_WINDOWS_1255,
	LIBREGF_CODEPAGE_WINDOWS_1256,
	LIBREGF_CODEPAGE_WINDOWS_1257,
	LIBREGF_CODEPAGE_WINDOWS_1258,
};

static const size_t value_number_of_codepages =
	  sizeof( value_codepages ) / sizeof( value_codepages[ 0 ] );

/* Decodes the value using the typed accessors
 * Returns 1 if successful or -1 on error
 */
int value_decode_fuzzer(
      libregf_value_t *value )
{
	uint8_t *name            = NULL;
	uint8_t *value_data      = NULL;
	uint8_t *utf8_string     = NULL;
	uint16_t *utf16_string   = NULL;
	libregf_multi_string_t *multi_string   = NULL;
	off64_t value_offset     = 0;
	size_t name_size         = 0;
	size_t value_data_size   = 0;
	size_t utf8_string_size  = 0;
	size_t utf16_string_size = 0;
	uint8_t value_byte       = 0;
	int number_of_strings    = 0;
	int result               = 1;

	if( value == NULL )
	{
		return( -1 );
	}
	libregf_value_get_offset(
	 value,
	 &value_offset,
	 NULL );

	libregf_value_is_corrupted(
	 value,
	 NULL );

	/* Retrieves the value using all typed accessors. The accessors
	 * return an error for an unsupported value type, which is
	 * part of what this exercises
	 */
	result = libregf_value_get_name_size(
	          value,
	          &name_size,
	          NULL );

	if( ( result == 1 )
	 && ( name_size > 0 )
	 && ( name_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		name = (uint8_t *) malloc( name_size );

		if( name != NULL )
		{
			if( libregf_value_get_name(
			     value,
			     name,
			     name_size,
			     NULL ) == 1 )
			{
				value_byte = name[ 0 ];
			}
			free( name );
		}
	}
	result = libregf_value_get_utf8_name_size(
	          value,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libregf_value_get_utf8_name(
			     value,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libregf_value_get_utf16_name_size(
	          value,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libregf_value_get_utf16_name(
			     value,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libregf_value_get_value_data_size(
	          value,
	          &value_data_size,
	          NULL );

	if( ( result == 1 )
	 && ( value_data_size > 0 )
	 && ( value_data_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		/* The value data buffer needs to be at least the size of
		 * the value data
		 */
		value_data = (uint8_t *) malloc( value_data_size );

		if( value_data != NULL )
		{
			if( libregf_value_get_value_data(
			     value,
			     value_data,
			     value_data_size,
			     NULL ) == 1 )
			{
				value_byte ^= value_data[ 0 ];
			}
			free( value_data );
		}
	}
	result = libregf_value_get_value_utf8_string_size(
	          value,
	          &utf8_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			if( libregf_value_get_value_utf8_string(
			     value,
			     utf8_string,
			     utf8_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libregf_value_get_value_utf16_string_size(
	          value,
	          &utf16_string_size,
	          NULL );

	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			if( libregf_value_get_value_utf16_string(
			     value,
			     utf16_string,
			     utf16_string_size,
			     NULL ) == 1 )
			{
				value_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	result = libregf_value_get_value_binary_data_size(
	          value,
	          &value_data_size,
	          NULL );

	if( ( result == 1 )
	 && ( value_data_size > 0 )
	 && ( value_data_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		value_data = (uint8_t *) malloc( value_data_size );

		if( value_data != NULL )
		{
			if( libregf_value_get_value_binary_data(
			     value,
			     value_data,
			     value_data_size,
			     NULL ) == 1 )
			{
				value_byte ^= value_data[ 0 ];
			}
			free( value_data );
		}
	}
	if( libregf_value_get_value_multi_string(
	     value,
	     &multi_string,
	     NULL ) == 1 )
	{
		if( libregf_multi_string_get_number_of_strings(
		     multi_string,
		     &number_of_strings,
		     NULL ) == 1 )
		{
			if( number_of_strings > 0 )
			{
				result = libregf_multi_string_get_utf8_string_size(
				          multi_string,
				          0,
				          &utf8_string_size,
				          NULL );

				if( ( result == 1 )
				 && ( utf8_string_size > 0 )
				 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
				{
					utf8_string = (uint8_t *) malloc( utf8_string_size );

					if( utf8_string != NULL )
					{
						if( libregf_multi_string_get_utf8_string(
						     multi_string,
						     0,
						     utf8_string,
						     utf8_string_size,
						     NULL ) == 1 )
						{
							value_byte ^= utf8_string[ 0 ];
						}
						free( utf8_string );
					}
				}
				result = libregf_multi_string_get_utf16_string_size(
				          multi_string,
				          0,
				          &utf16_string_size,
				          NULL );

				if( ( result == 1 )
				 && ( utf16_string_size > 0 )
				 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
				{
					utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

					if( utf16_string != NULL )
					{
						if( libregf_multi_string_get_utf16_string(
						     multi_string,
						     0,
						     utf16_string,
						     utf16_string_size,
						     NULL ) == 1 )
						{
							value_byte ^= (uint8_t) utf16_string[ 0 ];
						}
						free( utf16_string );
					}
				}
			}
		}
		libregf_multi_string_free(
		 &multi_string,
		 NULL );
	}
	else
	{
		multi_string = NULL;
	}
	/* Use the decoded values as a sink to keep them live
	 */
	if( ( value_byte ^ (uint8_t) value_offset ) != 0 )
	{
		/* No action required
		 */
	}
	return( 1 );
}

/* Walks a depth-driven path into the registry tree and decodes
 * the values of each key along the path. The sub key index at each
 * level is selected from the fuzz data (data[ 4 + level ]), which
 * steers the traversal toward different parts of the tree during
 * fuzzing
 * The parent key is not freed by this function
 * Returns 1 if successful or -1 on error
 */
int value_spine_fuzzer(
      libregf_key_t *parent_key,
      int level,
      int max_level,
      const uint8_t *data,
      size_t size )
{
	libregf_key_t *key          = NULL;
	libregf_value_t *value      = NULL;
	int number_of_sub_keys     = 0;
	int number_of_values       = 0;
	int sub_key_index          = 0;
	int value_index            = 0;
	int result                 = 1;

	if( parent_key == NULL )
	{
		return( -1 );
	}
	if( level > max_level
	 || size < ( size_t ) level + 5 )
	{
		return( 1 );
	}
	if( libregf_key_get_number_of_sub_keys(
	     parent_key,
	     &number_of_sub_keys,
	     NULL ) != 1
	   || number_of_sub_keys <= 0 )
	{
		return( 1 );
	}
	sub_key_index = ( int ) ( data[ 4 + level ] % ( size_t ) number_of_sub_keys );

	key = NULL;

	if( libregf_key_get_sub_key_by_index(
	     parent_key,
	     sub_key_index,
	     &key,
	     NULL ) != 1 )
	{
		return( 1 );
	}
	if( libregf_key_get_number_of_values(
	     key,
	     &number_of_values,
	     NULL ) == 1 )
	{
		for( value_index = 0;
		     value_index < number_of_values;
		     value_index++ )
		{
			if( value_index >= FUZZER_VALUE_LIMIT )
			{
				break;
			}
			value = NULL;

			if( libregf_key_get_value_by_index(
			     key,
			     value_index,
			     &value,
			     NULL ) != 1 )
			{
				continue;
			}
			value_decode_fuzzer(
			 value );

			libregf_value_free(
			 &value,
			 NULL );
		}
	}
	result = value_spine_fuzzer(
	             key,
	             level + 1,
	             max_level,
	             data,
	             size );

	libregf_key_free(
	 &key,
	 NULL );

	return( result );
}

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libregf_file_t *file             = NULL;
	libregf_key_t *root_key          = NULL;
	libregf_key_t *sub_key           = NULL;
	libregf_key_t *sub_sub_key       = NULL;
	libregf_value_t *value           = NULL;
	uint16_t *utf16_lookup_string    = NULL;
	size_t index                     = 0;
	size_t utf8_lookup_string_length  = 0;
	size_t utf16_lookup_string_length = 0;
	int number_of_sub_keys            = 0;
	int number_of_values              = 0;
	int sub_key_index                 = 0;
	int value_index                   = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libregf_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libregf_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBREGF_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libregf_file;
	}
	/* The codepage is selected from the fuzz data to steer the
	 * codepage conversion of value names and data
	 */
	if( size > 0 )
	{
		libregf_file_set_ascii_codepage(
		 file,
		 value_codepages[ data[ 0 ] % value_number_of_codepages ],
		 NULL );
	}
	utf8_lookup_string_length = size;

	if( utf8_lookup_string_length > FUZZER_LOOKUP_STRING_LIMIT )
	{
		utf8_lookup_string_length = FUZZER_LOOKUP_STRING_LIMIT;
	}
	utf16_lookup_string_length = ( size / 2 );

	if( utf16_lookup_string_length > FUZZER_LOOKUP_STRING_LIMIT )
	{
		utf16_lookup_string_length = FUZZER_LOOKUP_STRING_LIMIT;
	}
	if( utf16_lookup_string_length > 0 )
	{
		utf16_lookup_string = (uint16_t *) malloc( utf16_lookup_string_length * sizeof( uint16_t ) );

		if( utf16_lookup_string != NULL )
		{
			for( index = 0;
			     index < utf16_lookup_string_length;
			     index++ )
			{
				utf16_lookup_string[ index ] = (uint16_t) ( data[ index * 2 ] | ( (uint16_t) data[ index * 2 + 1 ] << 8 ) );
			}
		}
	}
	if( libregf_file_get_root_key(
	     file,
	     &root_key,
	     NULL ) == 1 )
	{
		/* Retrieves the default value
		 */
		value = NULL;

		libregf_key_get_value_by_utf8_name(
		 root_key,
		 NULL,
		 0,
		 &value,
		 NULL );

		if( value != NULL )
		{
			value_decode_fuzzer(
			 value );

			libregf_value_free(
			 &value,
			 NULL );
		}
		value = NULL;

		if( libregf_key_get_number_of_values(
		     root_key,
		     &number_of_values,
		     NULL ) == 1 )
		{
			for( value_index = 0;
			     value_index < number_of_values;
			     value_index++ )
			{
				if( value_index >= FUZZER_VALUE_LIMIT )
				{
					break;
				}
				value = NULL;

				if( libregf_key_get_value_by_index(
				     root_key,
				     value_index,
				     &value,
				     NULL ) != 1 )
				{
					continue;
				}
				value_decode_fuzzer(
				 value );

				libregf_value_free(
				 &value,
				 NULL );
			}
		}
		if( libregf_key_get_number_of_sub_keys(
		     root_key,
		     &number_of_sub_keys,
		     NULL ) == 1 )
		{
			for( sub_key_index = 0;
			     sub_key_index < number_of_sub_keys;
			     sub_key_index++ )
			{
				if( sub_key_index >= FUZZER_KEY_LIMIT )
				{
					break;
				}
				sub_key = NULL;

				if( libregf_key_get_sub_key_by_index(
				     root_key,
				     sub_key_index,
				     &sub_key,
				     NULL ) != 1 )
				{
					continue;
				}
				if( libregf_key_get_number_of_values(
				     sub_key,
				     &number_of_values,
				     NULL ) == 1 )
				{
					for( value_index = 0;
					     value_index < number_of_values;
					     value_index++ )
					{
						if( value_index >= FUZZER_VALUE_LIMIT )
						{
							break;
						}
						value = NULL;

						if( libregf_key_get_value_by_index(
						     sub_key,
						     value_index,
						     &value,
						     NULL ) != 1 )
						{
							continue;
						}
						value_decode_fuzzer(
						 value );

						libregf_value_free(
						 &value,
						 NULL );
					}
				}
				/* Decodes the values of the first sub sub key
				 */
				sub_sub_key = NULL;

				if( libregf_key_get_number_of_sub_keys(
				     sub_key,
				     &number_of_sub_keys,
				     NULL ) == 1 )
				{
					if( number_of_sub_keys > 0 )
					{
						if( libregf_key_get_sub_key_by_index(
						     sub_key,
						     0,
						     &sub_sub_key,
						     NULL ) == 1 )
						{
							if( libregf_key_get_number_of_values(
							     sub_sub_key,
							     &number_of_values,
							     NULL ) == 1 )
							{
								for( value_index = 0;
								     value_index < number_of_values;
								     value_index++ )
								{
									if( value_index >= FUZZER_VALUE_LIMIT )
									{
										break;
									}
									value = NULL;

									if( libregf_key_get_value_by_index(
									     sub_sub_key,
									     value_index,
									     &value,
									     NULL ) != 1 )
									{
										continue;
									}
									value_decode_fuzzer(
									 value );

									libregf_value_free(
									 &value,
									 NULL );
								}
							}
							libregf_key_free(
							 &sub_sub_key,
							 NULL );
						}
					}
				}
				/* Retrieves a value by name derived from the fuzz data.
				 * The data is not necessarily valid UTF-8 or UTF-16
				 * which exercises the validation and conversion error
				 * paths in the library
				 */
				if( utf8_lookup_string_length > 0 )
				{
					value = NULL;

					libregf_key_get_value_by_utf8_name(
					 sub_key,
					 data,
					 utf8_lookup_string_length,
					 &value,
					 NULL );

					if( value != NULL )
					{
						value_decode_fuzzer(
						 value );

						libregf_value_free(
						 &value,
						 NULL );
					}
					value = NULL;
				}
				if( ( utf16_lookup_string != NULL )
				 && ( utf16_lookup_string_length > 0 ) )
				{
					value = NULL;

					libregf_key_get_value_by_utf16_name(
					 sub_key,
					 utf16_lookup_string,
					 utf16_lookup_string_length,
					 &value,
					 NULL );

					if( value != NULL )
					{
						value_decode_fuzzer(
						 value );

						libregf_value_free(
						 &value,
						 NULL );
					}
					value = NULL;
				}
				libregf_key_free(
				 &sub_key,
				 NULL );
			}
		}
		/* Decodes the values of a depth-driven path into the
		 * registry tree, which exercises keys and values at
		 * deeper levels
		 */
		value_spine_fuzzer(
		 root_key,
		 1,
		 FUZZER_KEY_DEPTH_LIMIT,
		 data,
		 size );

		libregf_key_free(
		 &root_key,
		 NULL );
	}
	libregf_file_close(
	 file,
	 NULL );

on_error_libregf_file:
	free( utf16_lookup_string );
	libregf_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
