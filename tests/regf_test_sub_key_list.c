/*
 * Library sub_key_list type test program
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

#include "../libregf/libregf_io_handle.h"
#include "../libregf/libregf_sub_key_list.h"

uint8_t regf_test_sub_key_list_data1[ 68 ] = {
	0x6c, 0x68, 0x08, 0x00, 0x78, 0x01, 0x00, 0x00, 0xa2, 0xa9, 0x3b, 0x8f, 0x98, 0x3d, 0x22, 0x00,
	0xe5, 0x6c, 0xdd, 0xe9, 0x30, 0xb1, 0x24, 0x00, 0xf0, 0x56, 0x8b, 0x0a, 0x98, 0xdb, 0x24, 0x00,
	0x2b, 0x07, 0x7a, 0xfc, 0x70, 0xb4, 0x24, 0x00, 0x0f, 0xc2, 0x01, 0x00, 0xe8, 0xde, 0x24, 0x00,
	0xa0, 0x24, 0x00, 0x5f, 0x28, 0xb6, 0x24, 0x00, 0x81, 0xb8, 0x7c, 0x09, 0x20, 0xc0, 0x24, 0x00,
	0x10, 0xdd, 0x01, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_sub_key_list_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_sub_key_list_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_sub_key_list_t *sub_key_list = NULL;
	int result                           = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_sub_key_list_initialize(
	          &sub_key_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "sub_key_list",
	 sub_key_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_sub_key_list_free(
	          &sub_key_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "sub_key_list",
	 sub_key_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_sub_key_list_initialize(
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

	sub_key_list = (libregf_sub_key_list_t *) 0x12345678UL;

	result = libregf_sub_key_list_initialize(
	          &sub_key_list,
	          &error );

	sub_key_list = NULL;

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
		/* Test libregf_sub_key_list_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_sub_key_list_initialize(
		          &sub_key_list,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( sub_key_list != NULL )
			{
				libregf_sub_key_list_free(
				 &sub_key_list,
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
			 "sub_key_list",
			 sub_key_list );

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
		/* Test libregf_sub_key_list_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_sub_key_list_initialize(
		          &sub_key_list,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( sub_key_list != NULL )
			{
				libregf_sub_key_list_free(
				 &sub_key_list,
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
			 "sub_key_list",
			 sub_key_list );

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
	if( sub_key_list != NULL )
	{
		libregf_sub_key_list_free(
		 &sub_key_list,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_sub_key_list_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_sub_key_list_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_sub_key_list_free(
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

/* Tests the libregf_sub_key_list_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_sub_key_list_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_io_handle_t *io_handle       = NULL;
	libregf_sub_key_list_t *sub_key_list = NULL;
	int result                           = 0;

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

	result = libregf_sub_key_list_initialize(
	          &sub_key_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "sub_key_list",
	 sub_key_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          io_handle,
	          regf_test_sub_key_list_data1,
	          68,
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
	result = libregf_sub_key_list_read_data(
	          NULL,
	          io_handle,
	          regf_test_sub_key_list_data1,
	          68,
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

	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          NULL,
	          regf_test_sub_key_list_data1,
	          68,
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

	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          io_handle,
	          NULL,
	          68,
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

	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          io_handle,
	          regf_test_sub_key_list_data1,
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

	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          io_handle,
	          regf_test_sub_key_list_data1,
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

	/* Test error case where signature is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_sub_key_list_data1[ 0 ] ),
	 0xffff );

	result = libregf_sub_key_list_read_data(
	          sub_key_list,
	          io_handle,
	          regf_test_sub_key_list_data1,
	          68,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_sub_key_list_data1[ 0 ] ),
	 0x686c );

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
	result = libregf_sub_key_list_free(
	          &sub_key_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "sub_key_list",
	 sub_key_list );

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
	if( sub_key_list != NULL )
	{
		libregf_sub_key_list_free(
		 &sub_key_list,
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
	 "libregf_sub_key_list_initialize",
	 regf_test_sub_key_list_initialize );

	REGF_TEST_RUN(
	 "libregf_sub_key_list_free",
	 regf_test_sub_key_list_free );

	REGF_TEST_RUN(
	 "libregf_sub_key_list_read_data",
	 regf_test_sub_key_list_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */
}

