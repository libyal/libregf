/*
 * Library value_item type test program
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

#include "regf_test_functions.h"
#include "regf_test_libbfio.h"
#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_hive_bins_list.h"
#include "../libregf/libregf_io_handle.h"
#include "../libregf/libregf_value_item.h"

uint8_t regf_test_value_item_data1[ 4096 ] = {
	0x68, 0x62, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xe0, 0xff, 0xff, 0xff, 0x76, 0x6b, 0x02, 0x00, 0x04, 0x00, 0x00, 0x80, 0x30, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x4f, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xc0, 0x0f, 0x00, 0x00 };

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

/* Tests the libregf_value_item_clone function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_clone(
     void )
{
	libbfio_handle_t *file_io_handle             = NULL;
	libregf_hive_bins_list_t *hive_bins_list     = NULL;
	libregf_io_handle_t *io_handle               = NULL;
	libregf_value_item_t *destination_value_item = NULL;
	libregf_value_item_t *source_value_item      = NULL;
	libcerror_error_t *error                     = NULL;
	int result                                   = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests              = 5;
	int test_number                              = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests              = 4;
#endif
#endif

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

	result = libregf_hive_bins_list_append_bin(
	          hive_bins_list,
	          0,
	          4096,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_value_item_data1,
	          4096,
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

	/* Initialize test
	 */
	result = libregf_value_item_initialize(
	          &source_value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "source_value_item",
	 source_value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_item_read_value_key(
	          source_value_item,
	          file_io_handle,
	          hive_bins_list,
	          32,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_item_clone on initialized value_item
	 */
	result = libregf_value_item_clone(
	          &destination_value_item,
	          source_value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_value_item",
	 destination_value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_item_free(
	          &destination_value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_item",
	 destination_value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_item_clone on non-initialized value_item
	 */
	result = libregf_value_item_clone(
	          &destination_value_item,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_item",
	 destination_value_item );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_clone(
	          NULL,
	          source_value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_item",
	 destination_value_item );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_value_item = (libregf_value_item_t *) 0x12345678UL;

	result = libregf_value_item_clone(
	          &destination_value_item,
	          source_value_item,
	          &error );

	destination_value_item = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_item",
	 destination_value_item );

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
		/* Test libregf_value_item_clone with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_value_item_clone(
		          &destination_value_item,
		          source_value_item,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( destination_value_item != NULL )
			{
				libregf_value_item_free(
				 &destination_value_item,
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
			 "destination_value_item",
			 destination_value_item );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#if defined( OPTIMIZATION_DISABLED )

	for( test_number = 0;
	     test_number < number_of_memcpy_fail_tests;
	     test_number++ )
	{
		/* Test libregf_value_item_clone with memcpy failing
		 */
		regf_test_memcpy_attempts_before_fail = 0;

		result = libregf_value_item_clone(
		          &destination_value_item,
		          source_value_item,
		          &error );

		if( regf_test_memcpy_attempts_before_fail != -1 )
		{
			regf_test_memcpy_attempts_before_fail = -1;

			if( destination_value_item != NULL )
			{
				libregf_value_item_free(
				 &destination_value_item,
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
			 "destination_value_item",
			 destination_value_item );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_REGF_TEST_MEMORY ) */

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
	          &source_value_item,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "source_value_item",
	 source_value_item );

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
	if( destination_value_item != NULL )
	{
		libregf_value_item_free(
		 &destination_value_item,
		 NULL );
	}
	if( source_value_item != NULL )
	{
		libregf_value_item_free(
		 &source_value_item,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
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

/* Tests the libregf_value_item_read_value_key function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_read_value_key(
     void )
{
	libbfio_handle_t *file_io_handle         = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libcerror_error_t *error                 = NULL;
	libregf_value_item_t *value_item         = NULL;
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

	result = libregf_hive_bins_list_append_bin(
	          hive_bins_list,
	          0,
	          4096,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	          4096,
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
	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          32,
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
	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
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

	/* Test error cases
	 */
	result = libregf_value_item_read_value_key(
	          NULL,
	          file_io_handle,
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

	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
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

	hive_bins_list->io_handle = NULL;

	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          32,
	          &error );

	hive_bins_list->io_handle = io_handle;

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
	result = libregf_hive_bins_list_empty_cache(
	          hive_bins_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_item_read_value_key with libregf_hive_bins_list_get_cell_at_offset failing
	 */
	result = libregf_value_item_read_value_key(
	          value_item,
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

/* Tests the libregf_value_item_read_value_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_read_value_data(
     void )
{
	libbfio_handle_t *file_io_handle         = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libcerror_error_t *error                 = NULL;
	libregf_value_item_t *value_item         = NULL;
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

	result = libregf_hive_bins_list_append_bin(
	          hive_bins_list,
	          0,
	          4096,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	          4096,
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
/* TODO implement value data in regf_test_value_item_data1
	result = libregf_value_item_read_value_data(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          64,
	          16,
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
	result = libregf_value_item_read_value_data(
	          NULL,
	          file_io_handle,
	          hive_bins_list,
	          64,
	          16,
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

	result = libregf_value_item_read_value_data(
	          value_item,
	          file_io_handle,
	          NULL,
	          64,
	          16,
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

	hive_bins_list->io_handle = NULL;

	result = libregf_value_item_read_value_data(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          64,
	          16,
	          &error );

	hive_bins_list->io_handle = io_handle;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_value_item_read_value_data(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          0xffffffffUL,
	          16,
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
	result = libregf_hive_bins_list_empty_cache(
	          hive_bins_list,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_item_read_value_data with libregf_hive_bins_list_get_cell_at_offset failing
	 */
	result = libregf_value_item_read_value_data(
	          value_item,
	          NULL,
	          hive_bins_list,
	          64,
	          16,
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

/* Tests the libregf_value_item_is_corrupted function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_is_corrupted(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_is_corrupted(
	          value_item,
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
	result = libregf_value_item_is_corrupted(
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

/* Tests the libregf_value_item_get_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_name_size(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	size_t string_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_name_size(
	          value_item,
	          &string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "string_size",
	 string_size,
	 (size_t) 2 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_get_name_size(
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

	result = libregf_value_item_get_name_size(
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

/* Tests the libregf_value_item_get_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_name(
     libregf_value_item_t *value_item )
{
	uint8_t string[ 32 ];

	uint8_t expected_string[ 3 ] = {
		'O', 'n', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_name(
	          value_item,
	          string,
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
	          string,
	          expected_string,
	          2 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_item_get_name(
	          NULL,
	          string,
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

	result = libregf_value_item_get_name(
	          value_item,
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

/* Tests the libregf_value_item_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_utf8_name_size(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_utf8_name_size(
	          value_item,
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
	 (size_t) 3 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_get_utf8_name_size(
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

	result = libregf_value_item_get_utf8_name_size(
	          value_item,
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

/* Tests the libregf_value_item_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_utf8_name(
     libregf_value_item_t *value_item )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 3 ] = {
		'O', 'n', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_utf8_name(
	          value_item,
	          utf8_string,
	          32,
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
	          3 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_item_get_utf8_name(
	          NULL,
	          utf8_string,
	          32,
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

	result = libregf_value_item_get_utf8_name(
	          value_item,
	          NULL,
	          32,
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

/* Tests the libregf_value_item_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_utf16_name_size(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_utf16_name_size(
	          value_item,
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
	 (size_t) 3 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_get_utf16_name_size(
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

	result = libregf_value_item_get_utf16_name_size(
	          value_item,
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

/* Tests the libregf_value_item_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_utf16_name(
     libregf_value_item_t *value_item )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 3 ] = {
		'O', 'n', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_utf16_name(
	          value_item,
	          utf16_string,
	          32,
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
	          3 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_item_get_utf16_name(
	          NULL,
	          utf16_string,
	          32,
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

	result = libregf_value_item_get_utf16_name(
	          value_item,
	          NULL,
	          32,
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

/* Tests the libregf_value_item_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_compare_name_with_utf8_string(
     libregf_value_item_t *value_item )
{
	uint8_t utf8_string1[ 3 ] = {
		'O', 'n', 0 };

	uint8_t utf8_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_compare_name_with_utf8_string(
	          value_item,
	          0,
	          utf8_string1,
	          2,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_item_compare_name_with_utf8_string(
	          value_item,
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
	result = libregf_value_item_compare_name_with_utf8_string(
	          NULL,
	          0,
	          utf8_string1,
	          2,
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

	result = libregf_value_item_compare_name_with_utf8_string(
	          value_item,
	          0,
	          NULL,
	          2,
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

	result = libregf_value_item_compare_name_with_utf8_string(
	          value_item,
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

/* Tests the libregf_value_item_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_compare_name_with_utf16_string(
     libregf_value_item_t *value_item )
{
	uint16_t utf16_string1[ 3 ] = {
		'O', 'n', 0 };

	uint16_t utf16_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_compare_name_with_utf16_string(
	          value_item,
	          0,
	          utf16_string1,
	          2,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_item_compare_name_with_utf16_string(
	          value_item,
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
	result = libregf_value_item_compare_name_with_utf16_string(
	          NULL,
	          0,
	          utf16_string1,
	          2,
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

	result = libregf_value_item_compare_name_with_utf16_string(
	          value_item,
	          0,
	          NULL,
	          2,
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

	result = libregf_value_item_compare_name_with_utf16_string(
	          value_item,
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

/* Tests the libregf_value_item_get_value_type function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_value_type(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	uint32_t value_type      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_value_type(
	          value_item,
	          &value_type,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_UINT32(
	 "value_type",
	 value_type,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_item_get_value_type(
	          NULL,
	          &value_type,
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

	result = libregf_value_item_get_value_type(
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

/* Tests the libregf_value_item_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_data_size(
     libregf_value_item_t *value_item )
{
	libcerror_error_t *error = NULL;
	size_t data_size         = 0;
	uint8_t data_type        = 0;
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

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "data_size",
	 data_size,
	 (size_t) 4 );

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

	data_type = value_item->data_type;

	value_item->data_type = 0xff;

	result = libregf_value_item_get_data_size(
	          value_item,
	          NULL,
	          &error );

	value_item->data_type = data_type;

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

/* Tests the libregf_value_item_get_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_item_get_data(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle )
{
	libcerror_error_t *error = NULL;
	uint8_t *data            = NULL;
	size_t data_size         = 0;
	uint8_t data_type        = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_item_get_data(
	          value_item,
	          file_io_handle,
	          &data,
	          &data_size,
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
	result = libregf_value_item_get_data(
	          NULL,
	          file_io_handle,
	          &data,
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

	data_type = value_item->data_type;

	value_item->data_type = 0xff;

	result = libregf_value_item_get_data(
	          value_item,
	          file_io_handle,
	          &data,
	          &data_size,
	          &error );

	value_item->data_type = data_type;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_value_item_get_data(
	          value_item,
	          file_io_handle,
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

	result = libregf_value_item_get_data(
	          value_item,
	          file_io_handle,
	          &data,
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

	libbfio_handle_t *file_io_handle         = NULL;
	libcerror_error_t *error                 = NULL;
	libregf_hive_bins_list_t *hive_bins_list = NULL;
	libregf_io_handle_t *io_handle           = NULL;
	libregf_value_item_t *value_item         = NULL;
	int result                               = 0;

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

	REGF_TEST_RUN(
	 "libregf_value_item_clone",
	 regf_test_value_item_clone );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	REGF_TEST_RUN(
	 "libregf_value_item_read_value_key",
	 regf_test_value_item_read_value_key );

	REGF_TEST_RUN(
	 "libregf_value_item_read_value_data",
	 regf_test_value_item_read_value_data );

	/* TODO: add tests for libregf_value_item_read_data_block_list */

	/* TODO: add tests for libregf_value_item_read_element_data */

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

	result = libregf_hive_bins_list_append_bin(
	          hive_bins_list,
	          0,
	          4096,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	          4096,
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

	/* Initialize test
	 */
	result = libregf_value_item_read_value_key(
	          value_item,
	          file_io_handle,
	          hive_bins_list,
	          32,
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
	 "libregf_value_item_is_corrupted",
	 regf_test_value_item_is_corrupted,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_name_size",
	 regf_test_value_item_get_name_size,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_name",
	 regf_test_value_item_get_name,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_utf8_name_size",
	 regf_test_value_item_get_utf8_name_size,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_utf8_name",
	 regf_test_value_item_get_utf8_name,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_utf16_name_size",
	 regf_test_value_item_get_utf16_name_size,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_utf16_name",
	 regf_test_value_item_get_utf16_name,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_compare_name_with_utf8_string",
	 regf_test_value_item_compare_name_with_utf8_string,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_compare_name_with_utf16_string",
	 regf_test_value_item_compare_name_with_utf16_string,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_value_type",
	 regf_test_value_item_get_value_type,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_data_size",
	 regf_test_value_item_get_data_size,
	 value_item );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_item_get_data",
	 regf_test_value_item_get_data,
	 value_item,
	 file_io_handle );

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
#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

