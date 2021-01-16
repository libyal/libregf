/*
 * Library io_handle type test program
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

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_io_handle_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_io_handle_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_io_handle_t *io_handle  = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
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

	/* Test error cases
	 */
	result = libregf_io_handle_initialize(
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

	io_handle = (libregf_io_handle_t *) 0x12345678UL;

	result = libregf_io_handle_initialize(
	          &io_handle,
	          &error );

	io_handle = NULL;

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
		/* Test libregf_io_handle_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_io_handle_initialize(
		          &io_handle,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( io_handle != NULL )
			{
				libregf_io_handle_free(
				 &io_handle,
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
			 "io_handle",
			 io_handle );

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
		/* Test libregf_io_handle_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_io_handle_initialize(
		          &io_handle,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( io_handle != NULL )
			{
				libregf_io_handle_free(
				 &io_handle,
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
			 "io_handle",
			 io_handle );

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
	if( io_handle != NULL )
	{
		libregf_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_io_handle_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_io_handle_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_io_handle_free(
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

/* Tests the libregf_io_handle_clear function
 * Returns 1 if successful or 0 if not
 */
int regf_test_io_handle_clear(
     void )
{
	libcerror_error_t *error       = NULL;
	libregf_io_handle_t *io_handle = NULL;
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

	/* Test regular cases
	 */
	result = libregf_io_handle_clear(
	          io_handle,
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
	result = libregf_io_handle_clear(
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

#if defined( HAVE_REGF_TEST_MEMORY )

	/* Test libregf_io_handle_clear with memset failing
	 */
	regf_test_memset_attempts_before_fail = 0;

	result = libregf_io_handle_clear(
	          io_handle,
	          &error );

	if( regf_test_memset_attempts_before_fail != -1 )
	{
		regf_test_memset_attempts_before_fail = -1;
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
#endif /* defined( HAVE_REGF_TEST_MEMORY ) */

	/* Clean up
	 */
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
	 "libregf_io_handle_initialize",
	 regf_test_io_handle_initialize );

	REGF_TEST_RUN(
	 "libregf_io_handle_free",
	 regf_test_io_handle_free );

	REGF_TEST_RUN(
	 "libregf_io_handle_clear",
	 regf_test_io_handle_clear );

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

