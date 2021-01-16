/*
 * Library named_key type test program
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_io_handle.h"
#include "../libregf/libregf_named_key.h"

uint8_t regf_test_named_key_data1[ 140 ] = {
	0x6e, 0x6b, 0x2c, 0x00, 0x0d, 0x3f, 0x8a, 0x46, 0x68, 0x22, 0xd2, 0x01, 0x02, 0x00, 0x00, 0x00,
	0x68, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xd8, 0x14, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xa8, 0x2c, 0x00, 0x00,
	0xff, 0xff, 0xff, 0xff, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x33, 0x00, 0x39, 0x00, 0x00, 0x00, 0x43, 0x73, 0x69, 0x54,
	0x6f, 0x6f, 0x6c, 0x2d, 0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x48, 0x69, 0x76, 0x65, 0x2d, 0x7b,
	0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x2d, 0x30, 0x30, 0x30, 0x30, 0x2d, 0x30, 0x30,
	0x30, 0x30, 0x2d, 0x30, 0x30, 0x30, 0x30, 0x2d, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30,
	0x30, 0x30, 0x30, 0x30, 0x7d, 0x00, 0x39, 0x00, 0x31, 0x00, 0x45, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_named_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_named_key_t *named_key  = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_named_key_initialize(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_named_key_free(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_initialize(
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	named_key = (libregf_named_key_t *) 0x12345678UL;

	result = libregf_named_key_initialize(
	          &named_key,
	          &error );

	named_key = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_REGF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libregf_named_key_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_named_key_initialize(
		          &named_key,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( named_key != NULL )
			{
				libregf_named_key_free(
				 &named_key,
				 NULL );
			}
		}
		else
		{
			REGF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			REGF_TEST_ASSERT_IS_NULL(
			 "named_key",
			 named_key );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libregf_named_key_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_named_key_initialize(
		          &named_key,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( named_key != NULL )
			{
				libregf_named_key_free(
				 &named_key,
				 NULL );
			}
		}
		else
		{
			REGF_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			REGF_TEST_ASSERT_IS_NULL(
			 "named_key",
			 named_key );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_REGF_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( named_key != NULL )
	{
		libregf_named_key_free(
		 &named_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_named_key_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_named_key_free(
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_read_data(
     void )
{
	libcerror_error_t *error       = NULL;
	libregf_io_handle_t *io_handle = NULL;
	libregf_named_key_t *named_key = NULL;
	int result                     = 0;

	/* Initialize test
	 */
	result = libregf_io_handle_initialize(
	          &io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->major_version  = 1;
	io_handle->minor_version  = 5;
	io_handle->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;

	result = libregf_named_key_initialize(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_UINT32(
	 "named_key->name_size",
	 named_key->name_size,
	 57 );

	/* Test error cases
	 */
	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libregf_named_key_free(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_named_key_initialize(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_read_data(
	          NULL,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_read_data(
	          named_key,
	          NULL,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          NULL,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          0,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          (size_t) SSIZE_MAX + 1,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_REGF_TEST_MEMORY )

	/* Test libregf_named_key_read_data with malloc failing
	 */
	regf_test_malloc_attempts_before_fail = 0;

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	if( regf_test_malloc_attempts_before_fail != -1 )
	{
		regf_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libregf_named_key_read_data with memcpy failing
	 */
	regf_test_memcpy_attempts_before_fail = 0;

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	if( regf_test_memcpy_attempts_before_fail != -1 )
	{
		regf_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_REGF_TEST_MEMORY ) */

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_named_key_data1[ 0 ] ),
	 0xffff );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_named_key_data1[ 0 ] ),
	 0x6b6e );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test error case where name size is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_named_key_data1[ 72 ] ),
	 0x0000 );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_named_key_data1[ 72 ] ),
	 0x0039 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Clean up
	 */
	result = libregf_named_key_free(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_io_handle_free(
	          &io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( named_key != NULL )
	{
		libregf_named_key_free(
		 &named_key,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libregf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_name_size(
     libregf_named_key_t *named_key )
{
	libcerror_error_t *error = NULL;
	size_t string_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_name_size(
	          named_key,
	          &string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "string_size",
	 string_size,
	 (size_t) 57 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_get_name_size(
	          NULL,
	          &string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_name_size(
	          named_key,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_name(
     libregf_named_key_t *named_key )
{
	uint8_t string[ 64 ];

	uint8_t expected_string[ 58 ] = {
	        'C', 's', 'i', 'T', 'o', 'o', 'l', '-', 'C', 'r', 'e', 'a', 't', 'e', 'H', 'i',
	        'v', 'e', '-', '{', '0', '0', '0', '0', '0', '0', '0', '0', '-', '0', '0', '0',
	        '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0',
	        '0', '0', '0', '0', '0', '0', '0', '0', '}', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_name(
	          named_key,
	          string,
	          64,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          string,
	          expected_string,
	          57 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_named_key_get_name(
	          NULL,
	          string,
	          64,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_name(
	          named_key,
	          NULL,
	          64,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_utf8_name_size(
     libregf_named_key_t *named_key )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_utf8_name_size(
	          named_key,
	          &utf8_string_size,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 58 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_utf8_name_size(
	          named_key,
	          NULL,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_utf8_name(
     libregf_named_key_t *named_key )
{
	uint8_t utf8_string[ 64 ];

	uint8_t expected_utf8_string[ 58 ] = {
	        'C', 's', 'i', 'T', 'o', 'o', 'l', '-', 'C', 'r', 'e', 'a', 't', 'e', 'H', 'i',
	        'v', 'e', '-', '{', '0', '0', '0', '0', '0', '0', '0', '0', '-', '0', '0', '0',
	        '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0',
	        '0', '0', '0', '0', '0', '0', '0', '0', '}', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_utf8_name(
	          named_key,
	          utf8_string,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf8_string,
	          expected_utf8_string,
	          58 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_named_key_get_utf8_name(
	          NULL,
	          utf8_string,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_utf8_name(
	          named_key,
	          NULL,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_utf16_name_size(
     libregf_named_key_t *named_key )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_utf16_name_size(
	          named_key,
	          &utf16_string_size,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 58 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_utf16_name_size(
	          named_key,
	          NULL,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_utf16_name(
     libregf_named_key_t *named_key )
{
	uint16_t utf16_string[ 64 ];

	uint16_t expected_utf16_string[ 58 ] = {
	        'C', 's', 'i', 'T', 'o', 'o', 'l', '-', 'C', 'r', 'e', 'a', 't', 'e', 'H', 'i',
	        'v', 'e', '-', '{', '0', '0', '0', '0', '0', '0', '0', '0', '-', '0', '0', '0',
	        '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0',
	        '0', '0', '0', '0', '0', '0', '0', '0', '}', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_utf16_name(
	          named_key,
	          utf16_string,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf16_string,
	          expected_utf16_string,
	          58 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_named_key_get_utf16_name(
	          NULL,
	          utf16_string,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_utf16_name(
	          named_key,
	          NULL,
	          64,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_compare_name_with_utf8_string(
     libregf_named_key_t *named_key )
{
	uint8_t utf8_string1[ 58 ] = {
	        'C', 's', 'i', 'T', 'o', 'o', 'l', '-', 'C', 'r', 'e', 'a', 't', 'e', 'H', 'i',
	        'v', 'e', '-', '{', '0', '0', '0', '0', '0', '0', '0', '0', '-', '0', '0', '0',
	        '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0',
	        '0', '0', '0', '0', '0', '0', '0', '0', '}', 0 };

	uint8_t utf8_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_compare_name_with_utf8_string(
	          named_key,
	          0,
	          utf8_string1,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_named_key_compare_name_with_utf8_string(
	          named_key,
	          0,
	          utf8_string2,
	          6,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_compare_name_with_utf8_string(
	          NULL,
	          0,
	          utf8_string1,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_compare_name_with_utf8_string(
	          named_key,
	          0,
	          NULL,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_compare_name_with_utf8_string(
	          named_key,
	          0,
	          utf8_string1,
	          (size_t) SSIZE_MAX + 1,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_compare_name_with_utf16_string(
     libregf_named_key_t *named_key )
{
	uint16_t utf16_string1[ 58 ] = {
	        'C', 's', 'i', 'T', 'o', 'o', 'l', '-', 'C', 'r', 'e', 'a', 't', 'e', 'H', 'i',
	        'v', 'e', '-', '{', '0', '0', '0', '0', '0', '0', '0', '0', '-', '0', '0', '0',
	        '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0', '-', '0', '0', '0', '0',
	        '0', '0', '0', '0', '0', '0', '0', '0', '}', 0 };

	uint16_t utf16_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_compare_name_with_utf16_string(
	          named_key,
	          0,
	          utf16_string1,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_named_key_compare_name_with_utf16_string(
	          named_key,
	          0,
	          utf16_string2,
	          6,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_compare_name_with_utf16_string(
	          NULL,
	          0,
	          utf16_string1,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_compare_name_with_utf16_string(
	          named_key,
	          0,
	          NULL,
	          57,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_compare_name_with_utf16_string(
	          named_key,
	          0,
	          utf16_string1,
	          (size_t) SSIZE_MAX + 1,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libregf_named_key_get_last_written_time function
 * Returns 1 if successful or 0 if not
 */
int regf_test_named_key_get_last_written_time(
     libregf_named_key_t *named_key )
{
	libcerror_error_t *error = NULL;
	uint64_t filetime        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_named_key_get_last_written_time(
	          named_key,
	          &filetime,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_named_key_get_last_written_time(
	          NULL,
	          &filetime,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_named_key_get_last_written_time(
	          named_key,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc REGF_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] REGF_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc REGF_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] REGF_TEST_ATTRIBUTE_UNUSED )
#endif
{
#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	libcerror_error_t *error       = NULL;
	libregf_io_handle_t *io_handle = NULL;
	libregf_named_key_t *named_key = NULL;
	int result                     = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_named_key_initialize",
	 regf_test_named_key_initialize );

	REGF_TEST_RUN(
	 "libregf_named_key_free",
	 regf_test_named_key_free );

	REGF_TEST_RUN(
	 "libregf_named_key_read_data",
	 regf_test_named_key_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libregf_io_handle_initialize(
	          &io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->major_version  = 1;
	io_handle->minor_version  = 5;
	io_handle->ascii_codepage = LIBREGF_CODEPAGE_WINDOWS_1252;

	result = libregf_named_key_initialize(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_named_key_read_data(
	          named_key,
	          io_handle,
	          regf_test_named_key_data1,
	          140,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_name_size",
	 regf_test_named_key_get_name_size,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_name",
	 regf_test_named_key_get_name,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_utf8_name_size",
	 regf_test_named_key_get_utf8_name_size,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_utf8_name",
	 regf_test_named_key_get_utf8_name,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_utf16_name_size",
	 regf_test_named_key_get_utf16_name_size,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_utf16_name",
	 regf_test_named_key_get_utf16_name,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_compare_name_with_utf8_string",
	 regf_test_named_key_compare_name_with_utf8_string,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_compare_name_with_utf16_string",
	 regf_test_named_key_compare_name_with_utf16_string,
	 named_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_named_key_get_last_written_time",
	 regf_test_named_key_get_last_written_time,
	 named_key );

	/* Clean up
	 */
	result = libregf_named_key_free(
	          &named_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "named_key",
	 named_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_io_handle_free(
	          &io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( named_key != NULL )
	{
		libregf_named_key_free(
		 &named_key,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libregf_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

