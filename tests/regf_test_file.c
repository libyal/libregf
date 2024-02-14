/*
 * Library file type test program
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
#include <file_stream.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "regf_test_functions.h"
#include "regf_test_getopt.h"
#include "regf_test_libbfio.h"
#include "regf_test_libcerror.h"
#include "regf_test_libregf.h"
#include "regf_test_macros.h"
#include "regf_test_memory.h"
#include "regf_test_unused.h"

#include "../libregf/libregf_file.h"

#if !defined( LIBREGF_HAVE_BFIO )

LIBREGF_EXTERN \
int libregf_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_open_file_io_handle(
     libregf_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libregf_error_t **error );

#endif /* !defined( LIBREGF_HAVE_BFIO ) */

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make regf_test_file generate verbose output
#define REGF_TEST_FILE_VERBOSE
 */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int regf_test_file_open_source(
     libregf_file_t **file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "regf_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libregf_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	result = libregf_file_open_file_io_handle(
	          *file,
	          file_io_handle,
	          LIBREGF_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libregf_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int regf_test_file_close_source(
     libregf_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "regf_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libregf_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libregf_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libregf_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_file_t *file            = NULL;
	int result                      = 0;

#if defined( HAVE_REGF_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libregf_file_initialize(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_file_free(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_file_initialize(
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

	file = (libregf_file_t *) 0x12345678UL;

	result = libregf_file_initialize(
	          &file,
	          &error );

	file = NULL;

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
		/* Test libregf_file_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_file_initialize(
		          &file,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libregf_file_free(
				 &file,
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
			 "file",
			 file );

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
		/* Test libregf_file_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_file_initialize(
		          &file,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libregf_file_free(
				 &file,
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
			 "file",
			 file );

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
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_file_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_file_free(
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

/* Tests the libregf_file_open function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libregf_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = regf_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_file_initialize(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libregf_file_open(
	          file,
	          narrow_source,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_open(
	          NULL,
	          narrow_source,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open(
	          file,
	          NULL,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open(
	          file,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libregf_file_open(
	          file,
	          narrow_source,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_free(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libregf_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libregf_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = regf_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_file_initialize(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libregf_file_open_wide(
	          file,
	          wide_source,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_open_wide(
	          NULL,
	          wide_source,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open_wide(
	          file,
	          NULL,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open_wide(
	          file,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libregf_file_open_wide(
	          file,
	          wide_source,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_free(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libregf_file_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libregf_file_t *file             = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        REGF_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libregf_file_initialize(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libregf_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open_file_io_handle(
	          file,
	          NULL,
	          LIBREGF_OPEN_READ,
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

	result = libregf_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libregf_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBREGF_OPEN_READ,
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
	result = libregf_file_free(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
         "file_io_handle",
         file_io_handle );

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
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_file_close function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_file_close(
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

/* Tests the libregf_file_open and libregf_file_close functions
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libregf_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libregf_file_initialize(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_file_open_wide(
	          file,
	          source,
	          LIBREGF_OPEN_READ,
	          &error );
#else
	result = libregf_file_open(
	          file,
	          source,
	          LIBREGF_OPEN_READ,
	          &error );
#endif

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_file_close(
	          file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libregf_file_open_wide(
	          file,
	          source,
	          LIBREGF_OPEN_READ,
	          &error );
#else
	result = libregf_file_open(
	          file,
	          source,
	          LIBREGF_OPEN_READ,
	          &error );
#endif

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_file_close(
	          file,
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
	result = libregf_file_free(
	          &file,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libregf_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_signal_abort(
     libregf_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_file_signal_abort(
	          file,
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
	result = libregf_file_signal_abort(
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

/* Tests the libregf_file_is_corrupted function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_is_corrupted(
     libregf_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_file_is_corrupted(
	          file,
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
	result = libregf_file_is_corrupted(
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

/* Tests the libregf_file_get_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_ascii_codepage(
     libregf_file_t *file )
{
	libcerror_error_t *error  = NULL;
	int ascii_codepage        = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_ascii_codepage(
	          file,
	          &ascii_codepage,
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
	result = libregf_file_get_ascii_codepage(
	          NULL,
	          &ascii_codepage,
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

	result = libregf_file_get_ascii_codepage(
	          file,
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

/* Tests the libregf_file_set_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_set_ascii_codepage(
     libregf_file_t *file )
{
	int supported_codepages[ 15 ] = {
		LIBREGF_CODEPAGE_ASCII,
		LIBREGF_CODEPAGE_WINDOWS_874,
		LIBREGF_CODEPAGE_WINDOWS_932,
		LIBREGF_CODEPAGE_WINDOWS_936,
		LIBREGF_CODEPAGE_WINDOWS_949,
		LIBREGF_CODEPAGE_WINDOWS_950,
		LIBREGF_CODEPAGE_WINDOWS_1250,
		LIBREGF_CODEPAGE_WINDOWS_1251,
		LIBREGF_CODEPAGE_WINDOWS_1252,
		LIBREGF_CODEPAGE_WINDOWS_1253,
		LIBREGF_CODEPAGE_WINDOWS_1254,
		LIBREGF_CODEPAGE_WINDOWS_1255,
		LIBREGF_CODEPAGE_WINDOWS_1256,
		LIBREGF_CODEPAGE_WINDOWS_1257,
		LIBREGF_CODEPAGE_WINDOWS_1258 };

	int unsupported_codepages[ 17 ] = {
		LIBREGF_CODEPAGE_ISO_8859_1,
		LIBREGF_CODEPAGE_ISO_8859_2,
		LIBREGF_CODEPAGE_ISO_8859_3,
		LIBREGF_CODEPAGE_ISO_8859_4,
		LIBREGF_CODEPAGE_ISO_8859_5,
		LIBREGF_CODEPAGE_ISO_8859_6,
		LIBREGF_CODEPAGE_ISO_8859_7,
		LIBREGF_CODEPAGE_ISO_8859_8,
		LIBREGF_CODEPAGE_ISO_8859_9,
		LIBREGF_CODEPAGE_ISO_8859_10,
		LIBREGF_CODEPAGE_ISO_8859_11,
		LIBREGF_CODEPAGE_ISO_8859_13,
		LIBREGF_CODEPAGE_ISO_8859_14,
		LIBREGF_CODEPAGE_ISO_8859_15,
		LIBREGF_CODEPAGE_ISO_8859_16,
		LIBREGF_CODEPAGE_KOI8_R,
		LIBREGF_CODEPAGE_KOI8_U };

	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int index                = 0;
	int result               = 0;

	/* Test set ASCII codepage
	 */
	for( index = 0;
	     index < 15;
	     index++ )
	{
		codepage = supported_codepages[ index ];

		result = libregf_file_set_ascii_codepage(
		          file,
		          codepage,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libregf_file_set_ascii_codepage(
	          NULL,
	          LIBREGF_CODEPAGE_ASCII,
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

	for( index = 0;
	     index < 17;
	     index++ )
	{
		codepage = unsupported_codepages[ index ];

		result = libregf_file_set_ascii_codepage(
		          file,
		          codepage,
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
	result = libregf_file_set_ascii_codepage(
	          file,
	          LIBREGF_CODEPAGE_WINDOWS_1252,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

/* Tests the libregf_file_get_format_version function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_format_version(
     libregf_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t major_version   = 0;
	uint32_t minor_version   = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_format_version(
	          file,
	          &major_version,
	          &minor_version,
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
	result = libregf_file_get_format_version(
	          NULL,
	          &major_version,
	          &minor_version,
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

	result = libregf_file_get_format_version(
	          file,
	          NULL,
	          &minor_version,
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

	result = libregf_file_get_format_version(
	          file,
	          &major_version,
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

/* Tests the libregf_file_get_type function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_type(
     libregf_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_type(
	          file,
	          &type,
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
	result = libregf_file_get_type(
	          NULL,
	          &type,
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

	result = libregf_file_get_type(
	          file,
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

/* Tests the libregf_file_get_root_key function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_root_key(
     libregf_file_t *file )
{
	libcerror_error_t *error = NULL;
	libregf_key_t *root_key  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_root_key(
	          file,
	          &root_key,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( result != 0 )
	{
		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "root_key",
		 root_key );

		result = libregf_key_free(
		          &root_key,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libregf_file_get_root_key(
	          NULL,
	          &root_key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "root_key",
	 root_key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_root_key(
	          file,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "root_key",
	 root_key );

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

/* Tests the libregf_file_get_key_by_utf8_path function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_key_by_utf8_path(
     libregf_file_t *file )
{
	uint8_t utf8_key_path[ 7 ] = { 'S', 'e', 'l', 'e', 'c', 't', 0 };
	libcerror_error_t *error   = NULL;
	libregf_key_t *key         = 0;
	int result                 = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_key_by_utf8_path(
	          file,
	          utf8_key_path,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( result != 0 )
	{
		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "key",
		 key );

		result = libregf_key_free(
		          &key,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libregf_file_get_key_by_utf8_path(
	          NULL,
	          utf8_key_path,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf8_path(
	          file,
	          NULL,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf8_path(
	          file,
	          utf8_key_path,
	          (size_t) SSIZE_MAX + 1,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf8_path(
	          file,
	          utf8_key_path,
	          6,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

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

/* Tests the libregf_file_get_key_by_utf16_path function
 * Returns 1 if successful or 0 if not
 */
int regf_test_file_get_key_by_utf16_path(
     libregf_file_t *file )
{
	uint16_t utf16_key_path[ 7 ] = { 'S', 'e', 'l', 'e', 'c', 't', 0 };
	libcerror_error_t *error     = NULL;
	libregf_key_t *key           = 0;
	int result                   = 0;

	/* Test regular cases
	 */
	result = libregf_file_get_key_by_utf16_path(
	          file,
	          utf16_key_path,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	if( result != 0 )
	{
		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "key",
		 key );

		result = libregf_key_free(
		          &key,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libregf_file_get_key_by_utf16_path(
	          NULL,
	          utf16_key_path,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf16_path(
	          file,
	          NULL,
	          6,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf16_path(
	          file,
	          utf16_key_path,
	          (size_t) SSIZE_MAX + 1,
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_file_get_key_by_utf16_path(
	          file,
	          utf16_key_path,
	          6,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

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

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libregf_file_t *file             = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = regf_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( REGF_TEST_FILE_VERBOSE )
	libregf_notify_set_verbose(
	 1 );
	libregf_notify_set_stream(
	 stderr,
	 NULL );
#endif

	REGF_TEST_RUN(
	 "libregf_file_initialize",
	 regf_test_file_initialize );

	REGF_TEST_RUN(
	 "libregf_file_free",
	 regf_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        REGF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libregf_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		REGF_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_open",
		 regf_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_open_wide",
		 regf_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_open_file_io_handle",
		 regf_test_file_open_file_io_handle,
		 source );

		REGF_TEST_RUN(
		 "libregf_file_close",
		 regf_test_file_close );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_open_close",
		 regf_test_file_open_close,
		 source );

		/* Initialize file for tests
		 */
		result = regf_test_file_open_source(
		          &file,
		          file_io_handle,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NOT_NULL(
		 "file",
		 file );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_signal_abort",
		 regf_test_file_signal_abort,
		 file );

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

		/* TODO: add tests for libregf_file_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_is_corrupted",
		 regf_test_file_is_corrupted,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_ascii_codepage",
		 regf_test_file_get_ascii_codepage,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_set_ascii_codepage",
		 regf_test_file_set_ascii_codepage,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_format_version",
		 regf_test_file_get_format_version,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_type",
		 regf_test_file_get_type,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_root_key",
		 regf_test_file_get_root_key,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_key_by_utf8_path",
		 regf_test_file_get_key_by_utf8_path,
		 file );

		REGF_TEST_RUN_WITH_ARGS(
		 "libregf_file_get_key_by_utf16_path",
		 regf_test_file_get_key_by_utf16_path,
		 file );

		/* Clean up
		 */
		result = regf_test_file_close_source(
		          &file,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		REGF_TEST_ASSERT_IS_NULL(
		 "file",
		 file );

		REGF_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		REGF_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		REGF_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        REGF_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libregf_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

