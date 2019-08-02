/*
 * Library named_key type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

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

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	named_key = NULL;

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
	libcerror_error_t *error           = NULL;
	libregf_named_key_t *named_key = NULL;
	int result                         = 0;

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

	/* Test regular cases
	 */
	result = libregf_named_key_read_data(
	          named_key,
	          regf_test_named_key_data1,
	          140,
	          0,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
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
	          NULL,
	          regf_test_named_key_data1,
	          140,
	          0,
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

	result = libregf_named_key_read_data(
	          named_key,
	          NULL,
	          140,
	          0,
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

	result = libregf_named_key_read_data(
	          named_key,
	          regf_test_named_key_data1,
	          0,
	          0,
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

	result = libregf_named_key_read_data(
	          named_key,
	          regf_test_named_key_data1,
	          (size_t) SSIZE_MAX + 1,
	          0,
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

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_named_key_data1[ 0 ] ),
	 0xffff );

	result = libregf_named_key_read_data(
	          named_key,
	          regf_test_named_key_data1,
	          140,
	          0,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
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

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

