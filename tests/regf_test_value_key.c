/*
 * Library value_key type test program
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
#include "../libregf/libregf_value_key.h"

/* Value key with name and data stored outside key
 */
uint8_t regf_test_value_key_data1[ 36 ] = {
	0x76, 0x6b, 0x0c, 0x00, 0x22, 0x00, 0x00, 0x00, 0x30, 0xdc, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x44, 0x69, 0x73, 0x70, 0x46, 0x69, 0x6c, 0x65, 0x4e, 0x61, 0x6d, 0x65,
	0x00, 0x00, 0x00, 0x00 };

/* Value key with name and data stored inside key
 */
uint8_t regf_test_value_key_data2[ 28 ] = {
	0x76, 0x6b, 0x02, 0x00, 0x04, 0x00, 0x00, 0x80, 0x30, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x4f, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_value_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_value_key_t *value_key  = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_key_free(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_initialize(
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

	value_key = (libregf_value_key_t *) 0x12345678UL;

	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	value_key = NULL;

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
		/* Test libregf_value_key_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_value_key_initialize(
		          &value_key,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( value_key != NULL )
			{
				libregf_value_key_free(
				 &value_key,
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
			 "value_key",
			 value_key );

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
		/* Test libregf_value_key_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_value_key_initialize(
		          &value_key,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( value_key != NULL )
			{
				libregf_value_key_free(
				 &value_key,
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
			 "value_key",
			 value_key );

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
	if( value_key != NULL )
	{
		libregf_value_key_free(
		 &value_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_value_key_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_value_key_free(
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

/* Tests the libregf_value_key_clone function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_clone(
     void )
{
	libregf_value_key_t *destination_value_key = NULL;
	libregf_value_key_t *source_value_key      = NULL;
	libcerror_error_t *error                   = NULL;
	int result                                 = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests            = 3;
	int test_number                            = 0;

#if defined( OPTIMIZATION_DISABLED )
	int number_of_memcpy_fail_tests            = 2;
#endif
#endif

	/* Initialize test
	 */
	result = libregf_value_key_initialize(
	          &source_value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "source_value_key",
	 source_value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_key_clone on initialized value_key
	 */
	result = libregf_value_key_clone(
	          &destination_value_key,
	          source_value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "destination_value_key",
	 destination_value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_key_free(
	          &destination_value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_key",
	 destination_value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test libregf_value_key_clone on non-initialized value_key
	 */
	result = libregf_value_key_clone(
	          &destination_value_key,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_key",
	 destination_value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_clone(
	          NULL,
	          source_value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_key",
	 destination_value_key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	destination_value_key = (libregf_value_key_t *) 0x12345678UL;

	result = libregf_value_key_clone(
	          &destination_value_key,
	          source_value_key,
	          &error );

	destination_value_key = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "destination_value_key",
	 destination_value_key );

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
		/* Test libregf_value_key_clone with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_value_key_clone(
		          &destination_value_key,
		          source_value_key,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( destination_value_key != NULL )
			{
				libregf_value_key_free(
				 &destination_value_key,
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
			 "destination_value_key",
			 destination_value_key );

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
		/* Test libregf_value_key_clone with memcpy failing
		 */
		regf_test_memcpy_attempts_before_fail = 0;

		result = libregf_value_key_clone(
		          &destination_value_key,
		          source_value_key,
		          &error );

		if( regf_test_memcpy_attempts_before_fail != -1 )
		{
			regf_test_memcpy_attempts_before_fail = -1;

			if( destination_value_key != NULL )
			{
				libregf_value_key_free(
				 &destination_value_key,
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
			 "destination_value_key",
			 destination_value_key );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */
#endif /* defined( HAVE_REGF_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libregf_value_key_free(
	          &source_value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "source_value_key",
	 source_value_key );

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
	if( destination_value_key != NULL )
	{
		libregf_value_key_free(
		 &destination_value_key,
		 NULL );
	}
	if( source_value_key != NULL )
	{
		libregf_value_key_free(
		 &source_value_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_value_key_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_read_data(
     void )
{
	libcerror_error_t *error       = NULL;
	libregf_io_handle_t *io_handle = NULL;
	libregf_value_key_t *value_key = NULL;
	uint8_t *name_backup           = NULL;
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

	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
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
	 "value_key->name_size",
	 value_key->name_size,
	 12 );

	/* Clean up
	 */
	result = libregf_value_key_free(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data2,
	          28,
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
	 "value_key->name_size",
	 value_key->name_size,
	 2 );

	/* Test error cases
	 */
	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data2,
	          28,
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

	name_backup = value_key->name;

	value_key->name = NULL;

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data2,
	          28,
	          0,
	          &error );

	value_key->name = name_backup;

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
	result = libregf_value_key_free(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_read_data(
	          NULL,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
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

	result = libregf_value_key_read_data(
	          value_key,
	          NULL,
	          regf_test_value_key_data1,
	          36,
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

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          NULL,
	          36,
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

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
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

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
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

	/* Test libregf_value_key_read_data with malloc failing
	 */
	regf_test_malloc_attempts_before_fail = 0;

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
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

	/* Test libregf_value_key_read_data with memcpy failing
	 */
	regf_test_memcpy_attempts_before_fail = 0;

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
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

	/* Test regular case where signature is invalid
	 */
	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_value_key_data1[ 0 ] ),
	 0xffff );

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
	          0,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_value_key_data1[ 0 ] ),
	 0x6b76 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libregf_value_key_free(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_key",
	 value_key );

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
	if( value_key != NULL )
	{
		libregf_value_key_free(
		 &value_key,
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

/* Tests the libregf_value_key_get_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_name_size(
     libregf_value_key_t *value_key )
{
	libcerror_error_t *error = NULL;
	size_t string_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_name_size(
	          value_key,
	          &string_size,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "string_size",
	 string_size,
	 (size_t) 12 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_get_name_size(
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

	result = libregf_value_key_get_name_size(
	          value_key,
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

/* Tests the libregf_value_key_get_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_name(
     libregf_value_key_t *value_key )
{
	uint8_t string[ 32 ];

	uint8_t expected_string[ 13 ] = {
		'D', 'i', 's', 'p', 'F', 'i', 'l', 'e', 'N', 'a', 'm', 'e', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_name(
	          value_key,
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
	          12 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_key_get_name(
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

	result = libregf_value_key_get_name(
	          value_key,
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

/* Tests the libregf_value_key_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_utf8_name_size(
     libregf_value_key_t *value_key )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_utf8_name_size(
	          value_key,
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
	 (size_t) 13 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_get_utf8_name_size(
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

	result = libregf_value_key_get_utf8_name_size(
	          value_key,
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

/* Tests the libregf_value_key_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_utf8_name(
     libregf_value_key_t *value_key )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 13 ] = {
		'D', 'i', 's', 'p', 'F', 'i', 'l', 'e', 'N', 'a', 'm', 'e', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_utf8_name(
	          value_key,
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
	          13 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_key_get_utf8_name(
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

	result = libregf_value_key_get_utf8_name(
	          value_key,
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

/* Tests the libregf_value_key_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_utf16_name_size(
     libregf_value_key_t *value_key )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_utf16_name_size(
	          value_key,
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
	 (size_t) 13 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_value_key_get_utf16_name_size(
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

	result = libregf_value_key_get_utf16_name_size(
	          value_key,
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

/* Tests the libregf_value_key_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_utf16_name(
     libregf_value_key_t *value_key )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 13 ] = {
		'D', 'i', 's', 'p', 'F', 'i', 'l', 'e', 'N', 'a', 'm', 'e', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_utf16_name(
	          value_key,
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
	          13 );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libregf_value_key_get_utf16_name(
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

	result = libregf_value_key_get_utf16_name(
	          value_key,
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

/* Tests the libregf_value_key_compare_name_with_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_compare_name_with_utf8_string(
     libregf_value_key_t *value_key )
{
	uint8_t utf8_string1[ 13 ] = {
		'D', 'i', 's', 'p', 'F', 'i', 'l', 'e', 'N', 'a', 'm', 'e', 0 };

	uint8_t utf8_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_compare_name_with_utf8_string(
	          value_key,
	          0,
	          utf8_string1,
	          12,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_key_compare_name_with_utf8_string(
	          value_key,
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
	result = libregf_value_key_compare_name_with_utf8_string(
	          NULL,
	          0,
	          utf8_string1,
	          12,
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

	result = libregf_value_key_compare_name_with_utf8_string(
	          value_key,
	          0,
	          NULL,
	          12,
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

	result = libregf_value_key_compare_name_with_utf8_string(
	          value_key,
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

/* Tests the libregf_value_key_compare_name_with_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_compare_name_with_utf16_string(
     libregf_value_key_t *value_key )
{
	uint16_t utf16_string1[ 13 ] = {
		'D', 'i', 's', 'p', 'F', 'i', 'l', 'e', 'N', 'a', 'm', 'e', 0 };

	uint16_t utf16_string2[ 6 ] = {
		'B', 'o', 'g', 'u', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_compare_name_with_utf16_string(
	          value_key,
	          0,
	          utf16_string1,
	          12,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_key_compare_name_with_utf16_string(
	          value_key,
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
	result = libregf_value_key_compare_name_with_utf16_string(
	          NULL,
	          0,
	          utf16_string1,
	          12,
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

	result = libregf_value_key_compare_name_with_utf16_string(
	          value_key,
	          0,
	          NULL,
	          12,
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

	result = libregf_value_key_compare_name_with_utf16_string(
	          value_key,
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

/* Tests the libregf_value_key_get_value_type function
 * Returns 1 if successful or 0 if not
 */
int regf_test_value_key_get_value_type(
     libregf_value_key_t *value_key )
{
	libcerror_error_t *error = NULL;
	uint32_t value_type      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_value_key_get_value_type(
	          value_key,
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
	result = libregf_value_key_get_value_type(
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

	result = libregf_value_key_get_value_type(
	          value_key,
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
	libregf_value_key_t *value_key = NULL;
	int result                     = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

	REGF_TEST_RUN(
	 "libregf_value_key_initialize",
	 regf_test_value_key_initialize );

	REGF_TEST_RUN(
	 "libregf_value_key_free",
	 regf_test_value_key_free );

	REGF_TEST_RUN(
	 "libregf_value_key_clone",
	 regf_test_value_key_clone );

	REGF_TEST_RUN(
	 "libregf_value_key_read_data",
	 regf_test_value_key_read_data );

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

	result = libregf_value_key_initialize(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "value_key",
	 value_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_value_key_read_data(
	          value_key,
	          io_handle,
	          regf_test_value_key_data1,
	          36,
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
	 "libregf_value_key_get_name_size",
	 regf_test_value_key_get_name_size,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_name",
	 regf_test_value_key_get_name,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_utf8_name_size",
	 regf_test_value_key_get_utf8_name_size,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_utf8_name",
	 regf_test_value_key_get_utf8_name,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_utf16_name_size",
	 regf_test_value_key_get_utf16_name_size,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_utf16_name",
	 regf_test_value_key_get_utf16_name,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_compare_name_with_utf8_string",
	 regf_test_value_key_compare_name_with_utf8_string,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_compare_name_with_utf16_string",
	 regf_test_value_key_compare_name_with_utf16_string,
	 value_key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_value_key_get_value_type",
	 regf_test_value_key_get_value_type,
	 value_key );

	/* Clean up
	 */
	result = libregf_value_key_free(
	          &value_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "value_key",
	 value_key );

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
	if( value_key != NULL )
	{
		libregf_value_key_free(
		 &value_key,
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

