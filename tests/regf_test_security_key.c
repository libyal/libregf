/*
 * Library security_key type test program
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
#include "../libregf/libregf_security_key.h"

uint8_t regf_test_security_key_data1[ 180 ] = {
	0x73, 0x6b, 0x00, 0x00, 0x50, 0x4b, 0x04, 0x00, 0xa8, 0x2c, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00,
	0xa0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x14, 0x88, 0x88, 0x00, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00,
	0x14, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x6c, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x13, 0x24, 0x00, 0x3f, 0x00, 0x0f, 0x00,
	0x01, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x15, 0x00, 0x00, 0x00, 0x8e, 0x64, 0x79, 0x0d,
	0x67, 0x54, 0xde, 0xb1, 0x63, 0x5e, 0x9a, 0x47, 0x53, 0x04, 0x00, 0x00, 0x00, 0x13, 0x14, 0x00,
	0x3f, 0x00, 0x0f, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x12, 0x00, 0x00, 0x00,
	0x00, 0x13, 0x18, 0x00, 0x3f, 0x00, 0x0f, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
	0x20, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x13, 0x14, 0x00, 0x19, 0x00, 0x02, 0x00,
	0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0c, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x05, 0x12, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05,
	0x12, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_security_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_security_key_initialize(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_security_key_t *security_key = NULL;
	int result                           = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests      = 1;
	int number_of_memset_fail_tests      = 1;
	int test_number                      = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_security_key_initialize(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "security_key",
	 security_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_security_key_free(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "security_key",
	 security_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_security_key_initialize(
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

	security_key = (libregf_security_key_t *) 0x12345678UL;

	result = libregf_security_key_initialize(
	          &security_key,
	          &error );

	security_key = NULL;

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
		/* Test libregf_security_key_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_security_key_initialize(
		          &security_key,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( security_key != NULL )
			{
				libregf_security_key_free(
				 &security_key,
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
			 "security_key",
			 security_key );

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
		/* Test libregf_security_key_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_security_key_initialize(
		          &security_key,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( security_key != NULL )
			{
				libregf_security_key_free(
				 &security_key,
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
			 "security_key",
			 security_key );

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
	if( security_key != NULL )
	{
		libregf_security_key_free(
		 &security_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_security_key_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_security_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_security_key_free(
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

/* Tests the libregf_security_key_read_data function
 * Returns 1 if successful or 0 if not
 */
int regf_test_security_key_read_data(
     void )
{
	libcerror_error_t *error             = NULL;
	libregf_io_handle_t *io_handle       = NULL;
	libregf_security_key_t *security_key = NULL;
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

	result = libregf_security_key_initialize(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "security_key",
	 security_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	REGF_TEST_ASSERT_EQUAL_SIZE(
	 "security_key->security_descriptor_size",
	 security_key->security_descriptor_size,
	 (size_t) 160 );

	/* Test error cases
	 */
	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
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
	result = libregf_security_key_free(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "security_key",
	 security_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize test
	 */
	result = libregf_security_key_initialize(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "security_key",
	 security_key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_security_key_read_data(
	          NULL,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
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

	result = libregf_security_key_read_data(
	          security_key,
	          NULL,
	          regf_test_security_key_data1,
	          180,
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

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          NULL,
	          180,
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

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
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

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
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

	/* Test libregf_security_key_read_data with malloc failing
	 */
	regf_test_malloc_attempts_before_fail = 0;

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
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

	/* Test libregf_security_key_read_data with memcpy failing
	 */
	regf_test_memcpy_attempts_before_fail = 0;

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
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
	 &( regf_test_security_key_data1[ 0 ] ),
	 0xffff );

	result = libregf_security_key_read_data(
	          security_key,
	          io_handle,
	          regf_test_security_key_data1,
	          180,
	          &error );

	byte_stream_copy_from_uint16_little_endian(
	 &( regf_test_security_key_data1[ 0 ] ),
	 0x6b73 );

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
	result = libregf_security_key_free(
	          &security_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "security_key",
	 security_key );

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
	if( security_key != NULL )
	{
		libregf_security_key_free(
		 &security_key,
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
	 "libregf_security_key_initialize",
	 regf_test_security_key_initialize );

	REGF_TEST_RUN(
	 "libregf_security_key_free",
	 regf_test_security_key_free );

	REGF_TEST_RUN(
	 "libregf_security_key_read_data",
	 regf_test_security_key_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */
}

