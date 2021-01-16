/*
 * Library multi_string type test program
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

#include "../libregf/libregf_multi_string.h"

/* Define to make regf_test_multi_string generate verbose output
 */
#define REGF_TEST_MULTI_STRING_VERBOSE

uint8_t regf_test_multi_string_data1[ 712 ] = {
	0x53, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x20, 0x00, 0x42, 0x00,
	0x75, 0x00, 0x73, 0x00, 0x20, 0x00, 0x45, 0x00, 0x78, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6e, 0x00,
	0x64, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00, 0x53, 0x00, 0x43, 0x00, 0x53, 0x00, 0x49, 0x00,
	0x20, 0x00, 0x6d, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x69, 0x00, 0x70, 0x00, 0x6f, 0x00, 0x72, 0x00,
	0x74, 0x00, 0x00, 0x00, 0x70, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x74, 0x00, 0x00, 0x00, 0x50, 0x00,
	0x72, 0x00, 0x69, 0x00, 0x6d, 0x00, 0x61, 0x00, 0x72, 0x00, 0x79, 0x00, 0x20, 0x00, 0x64, 0x00,
	0x69, 0x00, 0x73, 0x00, 0x6b, 0x00, 0x00, 0x00, 0x53, 0x00, 0x43, 0x00, 0x53, 0x00, 0x49, 0x00,
	0x20, 0x00, 0x63, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00, 0x53, 0x00,
	0x43, 0x00, 0x53, 0x00, 0x49, 0x00, 0x20, 0x00, 0x43, 0x00, 0x44, 0x00, 0x52, 0x00, 0x4f, 0x00,
	0x4d, 0x00, 0x20, 0x00, 0x63, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00,
	0x66, 0x00, 0x69, 0x00, 0x6c, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x00, 0x00, 0x62, 0x00,
	0x6f, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x20, 0x00, 0x66, 0x00, 0x69, 0x00, 0x6c, 0x00, 0x65, 0x00,
	0x20, 0x00, 0x73, 0x00, 0x79, 0x00, 0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00,
	0x42, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00, 0x00, 0x50, 0x00, 0x6f, 0x00, 0x69, 0x00,
	0x6e, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00, 0x50, 0x00, 0x6f, 0x00, 0x72, 0x00,
	0x74, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x65, 0x00, 0x79, 0x00, 0x62, 0x00, 0x6f, 0x00, 0x61, 0x00,
	0x72, 0x00, 0x64, 0x00, 0x20, 0x00, 0x50, 0x00, 0x6f, 0x00, 0x72, 0x00, 0x74, 0x00, 0x00, 0x00,
	0x50, 0x00, 0x6f, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x20, 0x00,
	0x43, 0x00, 0x6c, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00, 0x4b, 0x00, 0x65, 0x00,
	0x79, 0x00, 0x62, 0x00, 0x6f, 0x00, 0x61, 0x00, 0x72, 0x00, 0x64, 0x00, 0x20, 0x00, 0x43, 0x00,
	0x6c, 0x00, 0x61, 0x00, 0x73, 0x00, 0x73, 0x00, 0x00, 0x00, 0x56, 0x00, 0x69, 0x00, 0x64, 0x00,
	0x65, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x49, 0x00, 0x6e, 0x00, 0x69, 0x00, 0x74, 0x00, 0x00, 0x00,
	0x56, 0x00, 0x69, 0x00, 0x64, 0x00, 0x65, 0x00, 0x6f, 0x00, 0x00, 0x00, 0x56, 0x00, 0x69, 0x00,
	0x64, 0x00, 0x65, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x53, 0x00, 0x61, 0x00, 0x76, 0x00, 0x65, 0x00,
	0x00, 0x00, 0x66, 0x00, 0x69, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x20, 0x00, 0x73, 0x00, 0x79, 0x00,
	0x73, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x45, 0x00, 0x76, 0x00, 0x65, 0x00,
	0x6e, 0x00, 0x74, 0x00, 0x20, 0x00, 0x6c, 0x00, 0x6f, 0x00, 0x67, 0x00, 0x00, 0x00, 0x53, 0x00,
	0x74, 0x00, 0x72, 0x00, 0x65, 0x00, 0x61, 0x00, 0x6d, 0x00, 0x73, 0x00, 0x20, 0x00, 0x44, 0x00,
	0x72, 0x00, 0x69, 0x00, 0x76, 0x00, 0x65, 0x00, 0x72, 0x00, 0x73, 0x00, 0x00, 0x00, 0x50, 0x00,
	0x4e, 0x00, 0x50, 0x00, 0x5f, 0x00, 0x54, 0x00, 0x44, 0x00, 0x49, 0x00, 0x00, 0x00, 0x4e, 0x00,
	0x44, 0x00, 0x49, 0x00, 0x53, 0x00, 0x00, 0x00, 0x54, 0x00, 0x44, 0x00, 0x49, 0x00, 0x00, 0x00,
	0x4e, 0x00, 0x65, 0x00, 0x74, 0x00, 0x42, 0x00, 0x49, 0x00, 0x4f, 0x00, 0x53, 0x00, 0x47, 0x00,
	0x72, 0x00, 0x6f, 0x00, 0x75, 0x00, 0x70, 0x00, 0x00, 0x00, 0x53, 0x00, 0x70, 0x00, 0x6f, 0x00,
	0x6f, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x72, 0x00, 0x47, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x75, 0x00,
	0x70, 0x00, 0x00, 0x00, 0x4e, 0x00, 0x65, 0x00, 0x74, 0x00, 0x44, 0x00, 0x44, 0x00, 0x45, 0x00,
	0x47, 0x00, 0x72, 0x00, 0x6f, 0x00, 0x75, 0x00, 0x70, 0x00, 0x00, 0x00, 0x50, 0x00, 0x61, 0x00,
	0x72, 0x00, 0x61, 0x00, 0x6c, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x6c, 0x00, 0x20, 0x00, 0x61, 0x00,
	0x72, 0x00, 0x62, 0x00, 0x69, 0x00, 0x74, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 0x00, 0x6f, 0x00,
	0x72, 0x00, 0x00, 0x00, 0x65, 0x00, 0x78, 0x00, 0x74, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x64, 0x00,
	0x65, 0x00, 0x64, 0x00, 0x20, 0x00, 0x62, 0x00, 0x61, 0x00, 0x73, 0x00, 0x65, 0x00, 0x00, 0x00,
	0x52, 0x00, 0x65, 0x00, 0x6d, 0x00, 0x6f, 0x00, 0x74, 0x00, 0x65, 0x00, 0x56, 0x00, 0x61, 0x00,
	0x6c, 0x00, 0x69, 0x00, 0x64, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00, 0x6f, 0x00, 0x6e, 0x00,
	0x00, 0x00, 0x50, 0x00, 0x43, 0x00, 0x49, 0x00, 0x20, 0x00, 0x43, 0x00, 0x6f, 0x00, 0x6e, 0x00,
	0x66, 0x00, 0x69, 0x00, 0x67, 0x00, 0x75, 0x00, 0x72, 0x00, 0x61, 0x00, 0x74, 0x00, 0x69, 0x00,
	0x6f, 0x00, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00 };

uint8_t regf_test_multi_string_data2[ 12 ] = {
	0x65, 0x00, 0x6e, 0x00, 0x2d, 0x00, 0x55, 0x00, 0x53, 0x00, 0x00, 0x00 };

uint8_t regf_test_multi_string_data3[ 269 ] = {
	0x31, 0x00, 0x39, 0x34, 0x39, 0x00, 0x30, 0x00, 0x7b, 0x0a, 0x20, 0x20, 0x20, 0x22, 0x44, 0x61,
	0x74, 0x61, 0x62, 0x61, 0x73, 0x65, 0x49, 0x6e, 0x73, 0x74, 0x61, 0x6e, 0x63, 0x65, 0x49, 0x64,
	0x22, 0x20, 0x3a, 0x20, 0x34, 0x37, 0x30, 0x33, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x22, 0x53, 0x65,
	0x71, 0x75, 0x65, 0x6e, 0x63, 0x65, 0x22, 0x20, 0x3a, 0x20, 0x32, 0x33, 0x37, 0x39, 0x2c, 0x0a,
	0x20, 0x20, 0x20, 0x22, 0x61, 0x63, 0x74, 0x69, 0x76, 0x69, 0x74, 0x79, 0x53, 0x74, 0x6f, 0x72,
	0x65, 0x49, 0x64, 0x22, 0x20, 0x3a, 0x20, 0x22, 0x36, 0x42, 0x35, 0x38, 0x39, 0x43, 0x37, 0x37,
	0x2d, 0x45, 0x39, 0x44, 0x37, 0x2d, 0x30, 0x32, 0x38, 0x46, 0x2d, 0x43, 0x35, 0x46, 0x37, 0x2d,
	0x43, 0x41, 0x44, 0x33, 0x33, 0x45, 0x37, 0x32, 0x35, 0x38, 0x33, 0x36, 0x22, 0x2c, 0x0a, 0x20,
	0x20, 0x20, 0x22, 0x66, 0x69, 0x6c, 0x74, 0x65, 0x72, 0x22, 0x20, 0x3a, 0x20, 0x7b, 0x0a, 0x20,
	0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x69, 0x73, 0x52, 0x65, 0x61, 0x64, 0x46, 0x69, 0x6c, 0x74,
	0x65, 0x72, 0x22, 0x20, 0x3a, 0x20, 0x30, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
	0x6f, 0x72, 0x69, 0x67, 0x69, 0x6e, 0x46, 0x69, 0x6c, 0x74, 0x65, 0x72, 0x4b, 0x65, 0x79, 0x22,
	0x20, 0x3a, 0x20, 0x30, 0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x73, 0x74, 0x61,
	0x74, 0x65, 0x46, 0x69, 0x6c, 0x74, 0x65, 0x72, 0x4b, 0x65, 0x79, 0x22, 0x20, 0x3a, 0x20, 0x30,
	0x2c, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x75, 0x73, 0x65, 0x72, 0x41, 0x63, 0x74,
	0x69, 0x6f, 0x6e, 0x53, 0x74, 0x61, 0x74, 0x65, 0x46, 0x69, 0x6c, 0x74, 0x65, 0x72, 0x22, 0x20,
	0x3a, 0x20, 0x30, 0x0a, 0x20, 0x20, 0x20, 0x7d, 0x0a, 0x7d, 0x0a, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_multi_string_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_multi_string_t *multi_string = NULL;
	int result                           = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_multi_string_initialize(
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

	multi_string = (libregf_multi_string_t *) 0x12345678UL;

	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	multi_string = NULL;

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
		/* Test libregf_multi_string_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_multi_string_initialize(
		          &multi_string,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( multi_string != NULL )
			{
				libregf_multi_string_free(
				 &multi_string,
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
			 "multi_string",
			 multi_string );

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
		/* Test libregf_multi_string_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_multi_string_initialize(
		          &multi_string,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( multi_string != NULL )
			{
				libregf_multi_string_free(
				 &multi_string,
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
			 "multi_string",
			 multi_string );

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
	if( multi_string != NULL )
	{
		libregf_multi_string_free(
		 &multi_string,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

/* Tests the libregf_multi_string_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_multi_string_free(
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

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_internal_multi_string_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_internal_multi_string_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_multi_string_t *multi_string = NULL;
	int result                           = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 3;
	int test_number                      = 0;
#endif

	/* Initialize test
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data1,
	          712,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "multi_string->number_of_strings",
	 ( (libregf_internal_multi_string_t *) multi_string )->number_of_strings,
	 29 );

	/* Clean up
	 */
	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data2,
	          12,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "multi_string->number_of_strings",
	 ( (libregf_internal_multi_string_t *) multi_string )->number_of_strings,
	 1 );

	/* Clean up
	 */
	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data3,
	          269,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "multi_string->number_of_strings",
	 ( (libregf_internal_multi_string_t *) multi_string )->number_of_strings,
	 0 );

	/* Clean up
	 */
	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_internal_multi_string_read_data(
	          NULL,
	          regf_test_multi_string_data1,
	          712,
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

	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          NULL,
	          712,
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

	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data1,
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

	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data1,
	          (size_t) SSIZE_MAX + 1,
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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test regf_test_multi_string_read_data with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_internal_multi_string_read_data(
		          (libregf_internal_multi_string_t *) multi_string,
		          regf_test_multi_string_data1,
		          712,
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
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test regf_test_multi_string_read_data with memcpy failing
	 */
	regf_test_memcpy_attempts_before_fail = 0;

	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data1,
	          712,
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

	/* Clean up
	 */
	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

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
	if( multi_string != NULL )
	{
		libregf_multi_string_free(
		 &multi_string,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_multi_string_get_number_of_strings function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_get_number_of_strings(
     libregf_multi_string_t *multi_string )
{
	libcerror_error_t *error = NULL;
	int number_of_strings    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_multi_string_get_number_of_strings(
	          multi_string,
	          &number_of_strings,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "number_of_strings",
	 number_of_strings,
	 29 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_multi_string_get_number_of_strings(
	          NULL,
	          &number_of_strings,
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

	result = libregf_multi_string_get_number_of_strings(
	          multi_string,
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

/* Tests the libregf_multi_string_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_get_utf8_string_size(
     libregf_multi_string_t *multi_string )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_multi_string_get_utf8_string_size(
	          multi_string,
	          0,
	          &utf8_string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 20 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_multi_string_get_utf8_string_size(
	          NULL,
	          0,
	          &utf8_string_size,
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

	result = libregf_multi_string_get_utf8_string_size(
	          multi_string,
	          -1,
	          &utf8_string_size,
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

	result = libregf_multi_string_get_utf8_string_size(
	          multi_string,
	          0,
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

/* Tests the libregf_multi_string_get_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_get_utf8_string(
     libregf_multi_string_t *multi_string )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 20 ] = {
		'S', 'y', 's', 't', 'e', 'm', ' ', 'B', 'u', 's', ' ', 'E', 'x', 't', 'e', 'n',
		'd', 'e', 'r', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_multi_string_get_utf8_string(
	          multi_string,
	          0,
	          utf8_string,
	          32,
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
	          20 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_multi_string_get_utf8_string(
	          NULL,
	          0,
	          utf8_string,
	          32,
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

	result = libregf_multi_string_get_utf8_string(
	          multi_string,
	          -1,
	          utf8_string,
	          32,
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

	result = libregf_multi_string_get_utf8_string(
	          multi_string,
	          0,
	          NULL,
	          32,
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

/* Tests the libregf_multi_string_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_get_utf16_string_size(
     libregf_multi_string_t *multi_string )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_multi_string_get_utf16_string_size(
	          multi_string,
	          0,
	          &utf16_string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 20 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_multi_string_get_utf16_string_size(
	          NULL,
	          0,
	          &utf16_string_size,
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

	result = libregf_multi_string_get_utf16_string_size(
	          multi_string,
	          -1,
	          &utf16_string_size,
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

	result = libregf_multi_string_get_utf16_string_size(
	          multi_string,
	          0,
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

/* Tests the libregf_multi_string_get_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_multi_string_get_utf16_string(
     libregf_multi_string_t *multi_string )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 20 ] = {
		'S', 'y', 's', 't', 'e', 'm', ' ', 'B', 'u', 's', ' ', 'E', 'x', 't', 'e', 'n',
		'd', 'e', 'r', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_multi_string_get_utf16_string(
	          multi_string,
	          0,
	          utf16_string,
	          32,
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
	          20 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_multi_string_get_utf16_string(
	          NULL,
	          0,
	          utf16_string,
	          32,
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

	result = libregf_multi_string_get_utf16_string(
	          multi_string,
	          -1,
	          utf16_string,
	          32,
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

	result = libregf_multi_string_get_utf16_string(
	          multi_string,
	          0,
	          NULL,
	          32,
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

	libcerror_error_t *error             = NULL;
	libregf_multi_string_t *multi_string = NULL;
	int result                           = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( HAVE_DEBUG_OUTPUT ) && defined( REGF_TEST_MULTI_STRING_VERBOSE )
	libregf_notify_set_verbose(
	 1 );
	libregf_notify_set_stream(
	 stderr,
	 NULL );
#endif

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_multi_string_initialize",
	 regf_test_multi_string_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_RUN(
	 "libregf_multi_string_free",
	 regf_test_multi_string_free );

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_internal_multi_string_read_data",
	 regf_test_internal_multi_string_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libregf_multi_string_initialize(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_internal_multi_string_read_data(
	          (libregf_internal_multi_string_t *) multi_string,
	          regf_test_multi_string_data1,
	          712,
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
	 "libregf_multi_string_get_number_of_strings",
	 regf_test_multi_string_get_number_of_strings,
	 multi_string );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_multi_string_get_utf8_string_size",
	 regf_test_multi_string_get_utf8_string_size,
	 multi_string );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_multi_string_get_utf8_string",
	 regf_test_multi_string_get_utf8_string,
	 multi_string );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_multi_string_get_utf16_string_size",
	 regf_test_multi_string_get_utf16_string_size,
	 multi_string );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_multi_string_get_utf16_string",
	 regf_test_multi_string_get_utf16_string,
	 multi_string );

	/* Clean up
	 */
	result = libregf_multi_string_free(
	          &multi_string,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "multi_string",
	 multi_string );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	if( multi_string != NULL )
	{
		libregf_multi_string_free(
		 &multi_string,
		 NULL );
	}
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

