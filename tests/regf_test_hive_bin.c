/*
 * Library hive_bin type test program
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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

#include "../libregf/libregf_hive_bin.h"

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_hive_bin_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_hive_bin_t *hive_bin    = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_hive_bin_initialize(
	          &hive_bin,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bin",
	 hive_bin );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_hive_bin_free(
	          &hive_bin,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bin",
	 hive_bin );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_hive_bin_initialize(
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

	hive_bin = (libregf_hive_bin_t *) 0x12345678UL;

	result = libregf_hive_bin_initialize(
	          &hive_bin,
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

	hive_bin = NULL;

#if defined( HAVE_REGF_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libregf_hive_bin_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_hive_bin_initialize(
		          &hive_bin,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( hive_bin != NULL )
			{
				libregf_hive_bin_free(
				 &hive_bin,
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
			 "hive_bin",
			 hive_bin );

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
		/* Test libregf_hive_bin_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_hive_bin_initialize(
		          &hive_bin,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( hive_bin != NULL )
			{
				libregf_hive_bin_free(
				 &hive_bin,
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
			 "hive_bin",
			 hive_bin );

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
	if( hive_bin != NULL )
	{
		libregf_hive_bin_free(
		 &hive_bin,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_hive_bin_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_hive_bin_free(
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

/* Tests the libregf_hive_bin_get_number_of_cells function
 * Returns 1 if successful or 0 if not
 */
int regf_test_hive_bin_get_number_of_cells(
     void )
{
	libcerror_error_t *error     = NULL;
	libregf_hive_bin_t *hive_bin = NULL;
	uint16_t number_of_cells     = 0;
	int number_of_cells_is_set   = 0;
	int result                   = 0;

	/* Initialize test
	 */
	result = libregf_hive_bin_initialize(
	          &hive_bin,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "hive_bin",
	 hive_bin );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libregf_hive_bin_get_number_of_cells(
	          hive_bin,
	          &number_of_cells,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	number_of_cells_is_set = result;

	/* Test error cases
	 */
	result = libregf_hive_bin_get_number_of_cells(
	          NULL,
	          &number_of_cells,
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

	if( number_of_cells_is_set != 0 )
	{
		result = libregf_hive_bin_get_number_of_cells(
		          hive_bin,
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
	}
	/* Clean up
	 */
	result = libregf_hive_bin_free(
	          &hive_bin,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "hive_bin",
	 hive_bin );

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
	if( hive_bin != NULL )
	{
		libregf_hive_bin_free(
		 &hive_bin,
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
	 "libregf_hive_bin_initialize",
	 regf_test_hive_bin_initialize );

	REGF_TEST_RUN(
	 "libregf_hive_bin_free",
	 regf_test_hive_bin_free );

#ifdef TODO

	/* TODO: add tests for libregf_hive_bin_read_header */

	/* TODO: add tests for libregf_hive_bin_read_cells */

	REGF_TEST_RUN(
	 "libregf_hive_bin_get_number_of_cells",
	 regf_test_hive_bin_get_number_of_cells );

	/* TODO: add tests for libregf_hive_bin_get_cell */

	/* TODO: add tests for libregf_hive_bin_get_cell_at_offset */

#endif /* TODO */

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

