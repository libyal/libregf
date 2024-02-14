/*
 * Library hive_bin_header type test program
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

#include "regf_test_functions.h"
#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_hive_bin_header.h"

uint8_t regf_test_hive_bin_header_data1[ 32 ] = {
	0x68, 0x62, 0x69, 0x6e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x12, 0xf3, 0xf1, 0xb0, 0x1c, 0xcf, 0xcf, 0x01, 0x00, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_hive_bin_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_header_initialize(
     void )
{
	libcerror_error_t *error                   = NULL;
	libregf_hive_bin_header_t *hive_bin_header = NULL;
	int result                                 = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests            = 1;
	int number_of_memset_fail_tests            = 1;
	int test_number                            = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_hive_bin_header_initialize(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bin_header",
	 hive_bin_header );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_hive_bin_header_free(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bin_header",
	 hive_bin_header );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_hive_bin_header_initialize(
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

	hive_bin_header = (libregf_hive_bin_header_t *) 0x12345678UL;

	result = libregf_hive_bin_header_initialize(
	          &hive_bin_header,
	          &error );

	hive_bin_header = NULL;

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
		/* Test libregf_hive_bin_header_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_hive_bin_header_initialize(
		          &hive_bin_header,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( hive_bin_header != NULL )
			{
				libregf_hive_bin_header_free(
				 &hive_bin_header,
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
			 "hive_bin_header",
			 hive_bin_header );

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
		/* Test libregf_hive_bin_header_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_hive_bin_header_initialize(
		          &hive_bin_header,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( hive_bin_header != NULL )
			{
				libregf_hive_bin_header_free(
				 &hive_bin_header,
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
			 "hive_bin_header",
			 hive_bin_header );

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
	if( hive_bin_header != NULL )
	{
		libregf_hive_bin_header_free(
		 &hive_bin_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_hive_bin_header_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_hive_bin_header_free(
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

/* Tests the libregf_hive_bin_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_header_read_data(
     void )
{
	libcerror_error_t *error                   = NULL;
	libregf_hive_bin_header_t *hive_bin_header = NULL;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libregf_hive_bin_header_initialize(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bin_header",
	 hive_bin_header );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          regf_test_hive_bin_header_data1,
	          32,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_UINT32(
	 "hive_bin_header->size",
	 hive_bin_header->size,
	 4096 );

	/* Test error cases
	 */
	result = libregf_hive_bin_header_read_data(
	          NULL,
	          regf_test_hive_bin_header_data1,
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

	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
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

	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          regf_test_hive_bin_header_data1,
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

	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          regf_test_hive_bin_header_data1,
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
#if defined( OPTIMIZATION_DISABLED )

	/* Test regf_test_hive_bin_header_read_data with memcpy failing
	 */
	regf_test_memcpy_attempts_before_fail = 0;

	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          regf_test_hive_bin_header_data1,
	          32,
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

	/* Test with invalid signature
	 */
	byte_stream_copy_from_uint32_little_endian(
	 &( regf_test_hive_bin_header_data1[ 0 ] ),
	 0xffffffffUL );

	result = libregf_hive_bin_header_read_data(
	          hive_bin_header,
	          regf_test_hive_bin_header_data1,
	          32,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( regf_test_hive_bin_header_data1[ 0 ] ),
	 0x6e696268UL );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libregf_hive_bin_header_free(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bin_header",
	 hive_bin_header );

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
	if( hive_bin_header != NULL )
	{
		libregf_hive_bin_header_free(
		 &hive_bin_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_hive_bin_header_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_header_read_file_io_handle(
     void )
{
	libbfio_handle_t *file_io_handle           = NULL;
	libcerror_error_t *error                   = NULL;
	libregf_hive_bin_header_t *hive_bin_header = NULL;
	int result                                 = 0;

	/* Initialize test
	 */
	result = libregf_hive_bin_header_initialize(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bin_header",
	 hive_bin_header );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_hive_bin_header_data1,
	          32,
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
	result = libregf_hive_bin_header_read_file_io_handle(
	          hive_bin_header,
	          file_io_handle,
	          0,
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
	result = libregf_hive_bin_header_read_file_io_handle(
	          NULL,
	          file_io_handle,
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

	result = libregf_hive_bin_header_read_file_io_handle(
	          hive_bin_header,
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

	/* Test data too small
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_hive_bin_header_data1,
	          8,
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

	result = libregf_hive_bin_header_read_file_io_handle(
	          hive_bin_header,
	          file_io_handle,
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

	/* Test with invalid signature
	 */
	result = regf_test_open_file_io_handle(
	          &file_io_handle,
	          regf_test_hive_bin_header_data1,
	          32,
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

	byte_stream_copy_from_uint32_little_endian(
	 &( regf_test_hive_bin_header_data1[ 0 ] ),
	 0xffffffffUL );

	result = libregf_hive_bin_header_read_file_io_handle(
	          hive_bin_header,
	          file_io_handle,
	          0,
	          &error );

	byte_stream_copy_from_uint32_little_endian(
	 &( regf_test_hive_bin_header_data1[ 0 ] ),
	 0x6e696268UL );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	result = libregf_hive_bin_header_free(
	          &hive_bin_header,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bin_header",
	 hive_bin_header );

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
	if( hive_bin_header != NULL )
	{
		libregf_hive_bin_header_free(
		 &hive_bin_header,
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
	 "libregf_hive_bin_header_initialize",
	 regf_test_hive_bin_header_initialize );

	REGF_TEST_RUN(
	 "libregf_hive_bin_header_free",
	 regf_test_hive_bin_header_free );

	REGF_TEST_RUN(
	 "libregf_hive_bin_header_read_data",
	 regf_test_hive_bin_header_read_data );

	REGF_TEST_RUN(
	 "libregf_hive_bin_header_read_file_io_handle",
	 regf_test_hive_bin_header_read_file_io_handle );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

