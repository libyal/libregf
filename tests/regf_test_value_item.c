/*
 * Library value_item type test program
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
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "regf_test_functions.h"
#include "regf_test_libbfio.h"
#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_value_item.h"

uint8_t regf_test_value_item_data1[ 28 ] = {
	0x76, 0x6b, 0x02, 0x00, 0x04, 0x00, 0x00, 0x80, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x4f, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_value_item_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_initialize(
     void )
{
	libcerror_error_t *error         = NULL;
	libregf_value_item_t *value_item = NULL;
	int result                       = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests  = 1;
	int number_of_memset_fail_tests  = 1;
	int test_number                  = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_value_item_initialize(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_item",
	 value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_item_free(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_item",
	 value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_initialize(
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

	value_item = (libregf_value_item_t *) 0x12345678UL;

	result = libregf_value_item_initialize(
	          &value_item,
	          &error );

	value_item = NULL;

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
		/* Test libregf_value_item_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_value_item_initialize(
		          &value_item,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( value_item != NULL )
			{
				libregf_value_item_free(
				 &value_item,
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
			 "value_item",
			 value_item );

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
		/* Test libregf_value_item_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_value_item_initialize(
		          &value_item,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( value_item != NULL )
			{
				libregf_value_item_free(
				 &value_item,
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
			 "value_item",
			 value_item );

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
	if( value_item != NULL )
	{
		libregf_value_item_free(
		 &value_item,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_value_item_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_value_item_free(
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

/* Tests the libregf_value_item_read_value_key function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_read_value_key(
     void )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libregf_value_item_t *value_item = NULL;
	int result                       = 0;

	/* Initialize test
	 */
	result = libregf_value_item_initialize(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_item",
	 value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_value_item_data1,
	          28,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement tests
	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
	          NULL,
	          0,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libregf_value_item_read_value_key(
	          NULL,
	          file_io_handle,
	          NULL,
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

	result = libregf_value_item_read_value_key(
	          value_item,
	          NULL,
	          NULL,
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

	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
	          NULL,
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

	/* Clean up file IO handle
	 */
	result = regf_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libregf_value_item_free(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_item",
	 value_item );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( value_item != NULL )
	{
		libregf_value_item_free(
		 &value_item,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_value_item_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_data_size(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	size_t data_size         = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_data_size(
	          value_item,
	          &data_size,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_get_data_size(
	          NULL,
	          &data_size,
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

	result = libregf_value_item_get_data_size(
	          value_item,
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

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libregf_value_item_t *value_item = NULL;
	int result                       = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_value_item_initialize",
	 regf_test_value_item_initialize );

	REGF_TEST_RUN(
	 "libregf_value_item_free",
	 regf_test_value_item_free );

	/* TODO add tests for libregf_value_item_clone */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	REGF_TEST_RUN(
	 "libregf_value_item_read_value_key",
	 regf_test_value_item_read_value_key );

	/* TODO: add tests for libregf_value_item_read_value_data */

	/* TODO: add tests for libregf_value_item_read_data_block_list */

	/* TODO: add tests for libregf_value_item_read_element_data */

	/* Initialize test
	 */
	result = libregf_value_item_initialize(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_item",
	 value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_value_item_data1,
	          28,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO set up value item */

	/* Run tests
	 */
	/* TODO: add tests for libregf_value_item_is_corrupted */

	/* TODO: add tests for libregf_value_item_get_name_size */

	/* TODO: add tests for libregf_value_item_get_name */

	/* TODO: add tests for libregf_value_item_get_utf8_name_size */

	/* TODO: add tests for libregf_value_item_get_utf8_name */

	/* TODO: add tests for libregf_value_item_get_utf16_name_size */

	/* TODO: add tests for libregf_value_item_get_utf16_name */

	/* TODO: add tests for libregf_value_item_compare_name_with_utf8_string */

	/* TODO: add tests for libregf_value_item_compare_name_with_utf16_string */

	/* TODO: add tests for libregf_value_item_get_value_type */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_data_size",
	 regf_test_value_item_get_data_size,
	 value_item );

	/* TODO: add tests for libregf_value_item_get_data */

	/* TODO: add tests for libregf_value_item_get_value_32bit */

	/* TODO: add tests for libregf_value_item_get_value_64bit */

	/* TODO: add tests for libregf_value_item_get_value_utf8_string_size */

	/* TODO: add tests for libregf_value_item_get_value_utf8_string */

	/* TODO: add tests for libregf_value_item_get_value_utf16_string_size */

	/* TODO: add tests for libregf_value_item_get_value_utf16_string */

	/* TODO: add tests for libregf_value_item_get_value_binary_data_size */

	/* TODO: add tests for libregf_value_item_get_value_binary_data */

	/* TODO: add tests for libregf_value_item_get_value_multi_string */

	/* Clean up file IO handle
	 */
	result = regf_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libregf_value_item_free(
	          &value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_item",
	 value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( value_item != NULL )
	{
		libregf_value_item_free(
		 &value_item,
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

