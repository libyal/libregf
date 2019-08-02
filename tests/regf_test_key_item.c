/*
 * Library key_item type test program
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

#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_hive_bins_list.h"
#include "../libregf/libregf_io_handle.h"
#include "../libregf/libregf_key_item.h"

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_key_item_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_key_item_t *key_item    = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_key_item_initialize(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_key_item_free(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_key_item_initialize(
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

	key_item = (libregf_key_item_t *) 0x12345678UL;

	result = libregf_key_item_initialize(
	          &key_item,
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

	key_item = NULL;

#if defined( HAVE_REGF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libregf_key_item_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_key_item_initialize(
		          &key_item,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( key_item != NULL )
			{
				libregf_key_item_free(
				 &key_item,
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
			 "key_item",
			 key_item );

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
		/* Test libregf_key_item_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_key_item_initialize(
		          &key_item,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( key_item != NULL )
			{
				libregf_key_item_free(
				 &key_item,
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
			 "key_item",
			 key_item );

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
	if( key_item != NULL )
	{
		libregf_key_item_free(
		 &key_item,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_key_item_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_key_item_free(
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

/* Tests the libregf_key_item_read_named_key function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_read_named_key(
     void )
{
	libcerror_error_t *error                 = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libregf_key_item_t *key_item             = NULL;
	int result                               = 0;

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

	result = libregf_hive_bins_list_initialize(
	          &hive_bins_list,
	          io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bins_list",
	 hive_bins_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_key_item_initialize(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
/* TODO implement */

	result = libregf_key_item_read_named_key(
	          NULL,
	          NULL,
	          NULL,
	          hive_bins_list,
	          32,
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

	key_item->name = (uint8_t *) 0x12345678UL;

	result = libregf_key_item_read_named_key(
	          key_item,
	          NULL,
	          NULL,
	          hive_bins_list,
	          32,
	          64,
	          &error );

	key_item->name = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_key_item_read_named_key(
	          key_item,
	          NULL,
	          NULL,
	          hive_bins_list,
	          32,
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

	/* Clean up
	 */
	result = libregf_key_item_free(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_hive_bins_list_free(
	          &hive_bins_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bins_list",
	 hive_bins_list );

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
	if( key_item != NULL )
	{
		libregf_key_item_free(
		 &key_item,
		 NULL );
	}
	if( hive_bins_list != NULL )
	{
		libregf_hive_bins_list_free(
		 &hive_bins_list,
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

/* Tests the libregf_key_item_read_class_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_read_class_name(
     void )
{
	libcerror_error_t *error                 = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libregf_key_item_t *key_item             = NULL;
	int result                               = 0;

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

	result = libregf_hive_bins_list_initialize(
	          &hive_bins_list,
	          io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bins_list",
	 hive_bins_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_key_item_initialize(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
/* TODO implement */

	result = libregf_key_item_read_class_name(
	          NULL,
	          NULL,
	          hive_bins_list,
	          32,
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

	key_item->class_name = (uint8_t *) 0x12345678UL;

	result = libregf_key_item_read_class_name(
	          key_item,
	          NULL,
	          hive_bins_list,
	          32,
	          64,
	          &error );

	key_item->class_name = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#ifdef TODO
	result = libregf_key_item_read_class_name(
	          key_item,
	          NULL,
	          hive_bins_list,
	          32,
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
#endif

	/* Clean up
	 */
	result = libregf_key_item_free(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_hive_bins_list_free(
	          &hive_bins_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bins_list",
	 hive_bins_list );

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
	if( key_item != NULL )
	{
		libregf_key_item_free(
		 &key_item,
		 NULL );
	}
	if( hive_bins_list != NULL )
	{
		libregf_hive_bins_list_free(
		 &hive_bins_list,
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

/* Tests the libregf_key_item_read_security_key function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_read_security_key(
     void )
{
	libcerror_error_t *error                 = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libregf_key_item_t *key_item             = NULL;
	int result                               = 0;

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

	result = libregf_hive_bins_list_initialize(
	          &hive_bins_list,
	          io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bins_list",
	 hive_bins_list );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_key_item_initialize(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
/* TODO implement */

	result = libregf_key_item_read_security_key(
	          NULL,
	          NULL,
	          hive_bins_list,
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

	key_item->security_descriptor = (uint8_t *) 0x12345678UL;

	result = libregf_key_item_read_security_key(
	          key_item,
	          NULL,
	          hive_bins_list,
	          32,
	          &error );

	key_item->security_descriptor = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_key_item_read_security_key(
	          key_item,
	          NULL,
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

	result = libregf_key_item_read_security_key(
	          key_item,
	          NULL,
	          hive_bins_list,
	          0xffffffffUL,
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
	result = libregf_key_item_free(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_hive_bins_list_free(
	          &hive_bins_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bins_list",
	 hive_bins_list );

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
	if( key_item != NULL )
	{
		libregf_key_item_free(
		 &key_item,
		 NULL );
	}
	if( hive_bins_list != NULL )
	{
		libregf_hive_bins_list_free(
		 &hive_bins_list,
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

/* Tests the libregf_key_item_get_number_of_values function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_get_number_of_values(
     libregf_key_item_t *key_item )
{
	libcerror_error_t *error = NULL;
	int number_of_values     = 0;
	int result               = 0;

	/* Test regular cases
	 */
#ifdef TODO
	result = libregf_key_item_get_number_of_values(
	          key_item,
	          &number_of_values,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
#endif

	/* Test error cases
	 */
	result = libregf_key_item_get_number_of_values(
	          NULL,
	          &number_of_values,
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

	result = libregf_key_item_get_number_of_values(
	          key_item,
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

/* Tests the libregf_key_item_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_compare_name_with_utf8_string(
     libregf_key_item_t *key_item )
{
	uint8_t utf8_string[ 5 ] = {
		't', 'e', 's', 't', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libregf_key_item_compare_name_with_utf8_string(
	          NULL,
	          0,
	          utf8_string,
	          4,
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

	result = libregf_key_item_compare_name_with_utf8_string(
	          key_item,
	          0,
	          NULL,
	          4,
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

	result = libregf_key_item_compare_name_with_utf8_string(
	          key_item,
	          0,
	          utf8_string,
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

/* Tests the libregf_key_item_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_item_compare_name_with_utf16_string(
     libregf_key_item_t *key_item )
{
	uint16_t utf16_string[ 5 ] = {
		't', 'e', 's', 't', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libregf_key_item_compare_name_with_utf16_string(
	          NULL,
	          0,
	          utf16_string,
	          4,
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

	result = libregf_key_item_compare_name_with_utf16_string(
	          key_item,
	          0,
	          NULL,
	          4,
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

	result = libregf_key_item_compare_name_with_utf16_string(
	          key_item,
	          0,
	          utf16_string,
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

	libcerror_error_t *error     = NULL;
	libregf_key_item_t *key_item = NULL;
	int result                   = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_key_item_initialize",
	 regf_test_key_item_initialize );

	REGF_TEST_RUN(
	 "libregf_key_item_free",
	 regf_test_key_item_free );

	REGF_TEST_RUN(
	 "libregf_key_item_read_named_key",
	 regf_test_key_item_read_named_key );

	REGF_TEST_RUN(
	 "libregf_key_item_read_class_name",
	 regf_test_key_item_read_class_name );

	REGF_TEST_RUN(
	 "libregf_key_item_read_security_key",
	 regf_test_key_item_read_security_key );

	/* TODO: add tests for libregf_key_item_read_values_list */

	/* TODO: add tests for libregf_key_item_read_node_data */

	/* TODO: add tests for libregf_key_item_read_sub_keys_list */

	/* TODO: add tests for libregf_key_item_read_sub_nodes */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libregf_key_item_initialize(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key_item",
	 key_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_item_get_number_of_values",
	 regf_test_key_item_get_number_of_values,
	 key_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_item_compare_name_with_utf8_string",
	 regf_test_key_item_compare_name_with_utf8_string,
	 key_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_item_compare_name_with_utf16_string",
	 regf_test_key_item_compare_name_with_utf16_string,
	 key_item );

	/* Clean up
	 */
	result = libregf_key_item_free(
	          &key_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key_item",
	 key_item );

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
	if( key_item != NULL )
	{
		libregf_key_item_free(
		 &key_item,
		 NULL );
	}
#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

