/*
 * OSS-Fuzz target for libregf file type
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
#define FUZZER_LOOKUP_STRING_LIMIT ( (size_t) 0x4000 )

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

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libregf_file_t *file             = NULL;
	libregf_key_t *key               = NULL;
	uint16_t *utf16_lookup_string    = NULL;
	size_t codepage_index            = 0;
	size_t index                     = 0;
	size_t utf8_lookup_string_length  = 0;
	size_t utf16_lookup_string_length = 0;
	uint32_t file_type               = 0;
	uint32_t major_version           = 0;
	uint32_t minor_version           = 0;
	int ascii_codepage               = 0;

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
	libregf_file_get_type(
	 file,
	 &file_type,
	 NULL );

	libregf_file_get_ascii_codepage(
	 file,
	 &ascii_codepage,
	 NULL );

	libregf_file_get_format_version(
	 file,
	 &major_version,
	 &minor_version,
	 NULL );

	libregf_file_is_corrupted(
	 file,
	 NULL );

	/* Cycles through the supported ASCII codepages to exercise the
	 * codepage handling in the library
	 */
	for( codepage_index = 0;
	     codepage_index < value_number_of_codepages;
	     codepage_index++ )
	{
		libregf_file_set_ascii_codepage(
		 file,
		 value_codepages[ codepage_index ],
		 NULL );

		libregf_file_get_ascii_codepage(
		 file,
		 &ascii_codepage,
		 NULL );
	}
	/* Sets an unsupported codepage to exercise the error path
	 */
	libregf_file_set_ascii_codepage(
	 file,
	 65001,
	 NULL );
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
	/* Retrieves a key using a path derived from the fuzz data.
	 * The data is not necessarily valid UTF-8 or UTF-16
	 * which exercises the validation and conversion error paths
	 * in the library
	 */
	if( utf8_lookup_string_length > 0 )
	{
		libregf_file_get_key_by_utf8_path(
		 file,
		 data,
		 utf8_lookup_string_length,
		 &key,
		 NULL );

		if( key != NULL )
		{
			libregf_key_free(
			 &key,
			 NULL );
		}
		key = NULL;
	}
	if( ( utf16_lookup_string != NULL )
	 && ( utf16_lookup_string_length > 0 ) )
	{
		libregf_file_get_key_by_utf16_path(
		 file,
		 utf16_lookup_string,
		 utf16_lookup_string_length,
		 &key,
		 NULL );

		if( key != NULL )
		{
			libregf_key_free(
			 &key,
			 NULL );
		}
		key = NULL;
	}
#if defined( LIBREGF_HAVE_BFIO )
	libregf_check_file_signature_file_io_handle(
	 file_io_handle,
	 NULL );
#endif
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
