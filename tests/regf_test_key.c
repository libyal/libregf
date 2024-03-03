/*
 * Library key type test program
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
#include "../libregf/libregf_key.h"

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libregf_io_handle_t *io_handle  = NULL;
	libregf_key_t *key              = NULL;
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

/* TODO pass file IO handle */
	result = libregf_key_initialize(
	          &key,
	          io_handle,
	          NULL,
	          0,
	          NULL,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libregf_key_free(
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libregf_key_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          0,
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

	key = (libregf_key_t *) 0x12345678UL;

	result = libregf_key_initialize(
	          &key,
	          io_handle,
	          NULL,
	          0,
	          NULL,
	          &error );

	key = NULL;

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libregf_key_initialize(
	          &key,
	          NULL,
	          NULL,
	          0,
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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libregf_key_initialize with malloc failing
		 */
		regf_test_malloc_attempts_before_fail = test_number;

		result = libregf_key_initialize(
		          &key,
		          io_handle,
		          NULL,
		          0,
		          NULL,
		          &error );

		if( regf_test_malloc_attempts_before_fail != -1 )
		{
			regf_test_malloc_attempts_before_fail = -1;

			if( key != NULL )
			{
				libregf_key_free(
				 &key,
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
			 "key",
			 key );

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
		/* Test libregf_key_initialize with memset failing
		 */
		regf_test_memset_attempts_before_fail = test_number;

		result = libregf_key_initialize(
		          &key,
		          io_handle,
		          NULL,
		          0,
		          NULL,
		          &error );

		if( regf_test_memset_attempts_before_fail != -1 )
		{
			regf_test_memset_attempts_before_fail = -1;

			if( key != NULL )
			{
				libregf_key_free(
				 &key,
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
			 "key",
			 key );

			REGF_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
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
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
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

/* Tests the libregf_key_free function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libregf_key_free(
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

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* Tests the libregf_key_is_corrupted function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_is_corrupted(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_is_corrupted(
	          key,
	          &error );

	REGF_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
*/

	/* Test error cases
	 */
	result = libregf_key_is_corrupted(
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

/* Tests the libregf_key_get_offset function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_offset(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	off64_t offset           = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_offset(
	          key,
	          &offset,
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
	result = libregf_key_get_offset(
	          NULL,
	          &offset,
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

	result = libregf_key_get_offset(
	          key,
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

/* Tests the libregf_key_get_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t string_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_name_size(
	          key,
	          &string_size,
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
	result = libregf_key_get_name_size(
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

	result = libregf_key_get_name_size(
	          key,
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

/* Tests the libregf_key_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_utf8_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_utf8_name_size(
	          key,
	          &utf8_string_size,
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
	result = libregf_key_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libregf_key_get_utf8_name_size(
	          key,
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

/* Tests the libregf_key_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_utf16_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_utf16_name_size(
	          key,
	          &utf16_string_size,
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
	result = libregf_key_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libregf_key_get_utf16_name_size(
	          key,
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

/* Tests the libregf_key_get_class_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_class_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t string_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_class_name_size(
	          key,
	          &string_size,
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
	result = libregf_key_get_class_name_size(
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

	result = libregf_key_get_class_name_size(
	          key,
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

/* Tests the libregf_key_get_utf8_class_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_utf8_class_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_utf8_class_name_size(
	          key,
	          &utf8_string_size,
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
	result = libregf_key_get_utf8_class_name_size(
	          NULL,
	          &utf8_string_size,
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

	result = libregf_key_get_utf8_class_name_size(
	          key,
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

/* Tests the libregf_key_get_utf16_class_name_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_utf16_class_name_size(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_utf16_class_name_size(
	          key,
	          &utf16_string_size,
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
	result = libregf_key_get_utf16_class_name_size(
	          NULL,
	          &utf16_string_size,
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

	result = libregf_key_get_utf16_class_name_size(
	          key,
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

/* Tests the libregf_key_get_last_written_time function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_last_written_time(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	uint64_t filetime        = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_last_written_time(
	          key,
	          &filetime,
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
	result = libregf_key_get_last_written_time(
	          NULL,
	          &filetime,
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

	result = libregf_key_get_last_written_time(
	          key,
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

/* Tests the libregf_key_get_security_descriptor_size function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_security_descriptor_size(
     libregf_key_t *key )
{
	libcerror_error_t *error        = NULL;
	size_t security_descriptor_size = 0;
	int result                      = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_security_descriptor_size(
	          key,
	          &security_descriptor_size,
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
	result = libregf_key_get_security_descriptor_size(
	          NULL,
	          &security_descriptor_size,
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

	result = libregf_key_get_security_descriptor_size(
	          key,
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

/* Tests the libregf_key_get_number_of_values function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_number_of_values(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	int number_of_values     = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_number_of_values(
	          key,
	          &number_of_values,
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
	result = libregf_key_get_number_of_values(
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

	result = libregf_key_get_number_of_values(
	          key,
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

/* Tests the libregf_key_get_number_of_sub_keys function
 * Returns 1 if successful or 0 if not
 */
int regf_test_key_get_number_of_sub_keys(
     libregf_key_t *key )
{
	libcerror_error_t *error = NULL;
	int number_of_sub_keys   = 0;
	int result               = 0;

	/* Test regular cases
	 */
/* TODO: implement
	result = libregf_key_get_number_of_sub_keys(
	          key,
	          &number_of_sub_keys,
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
	result = libregf_key_get_number_of_sub_keys(
	          NULL,
	          &number_of_sub_keys,
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

	result = libregf_key_get_number_of_sub_keys(
	          key,
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
	libregf_key_t *key             = NULL;
	int result                     = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_UNREFERENCED_PARAMETER( argc )
	REGF_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

/* TODO
	REGF_TEST_RUN(
	 "libregf_key_initialize",
	 regf_test_key_initialize );
*/

#endif /* defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT ) */

	REGF_TEST_RUN(
	 "libregf_key_free",
	 regf_test_key_free );

#if defined( __GNUC__ ) && !defined( LIBREGF_DLL_IMPORT )

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

/* TODO: implement */

	/* Run tests
	 */
	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_is_corrupted",
	 regf_test_key_is_corrupted,
	 key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_offset",
	 regf_test_key_get_offset,
	 key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_name_size",
	 regf_test_key_get_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_utf8_name_size",
	 regf_test_key_get_utf8_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_utf8_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_utf16_name_size",
	 regf_test_key_get_utf16_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_utf16_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_class_name_size",
	 regf_test_key_get_class_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_class_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_utf8_class_name_size",
	 regf_test_key_get_utf8_class_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_utf8_class_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_utf16_class_name_size",
	 regf_test_key_get_utf16_class_name_size,
	 key );

	/* TODO: add tests for libregf_key_get_utf16_class_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_last_written_time",
	 regf_test_key_get_last_written_time,
	 key );

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_security_descriptor_size",
	 regf_test_key_get_security_descriptor_size,
	 key );

	/* TODO: add tests for libregf_key_get_security_descriptor */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_number_of_values",
	 regf_test_key_get_number_of_values,
	 key );

	/* TODO: add tests for libregf_key_get_value_by_index */

	/* TODO: add tests for libregf_key_get_value_by_utf8_name */

	/* TODO: add tests for libregf_key_get_value_by_utf16_name */

	REGF_TEST_RUN_WITH_ARGS(
	 "libregf_key_get_number_of_sub_keys",
	 regf_test_key_get_number_of_sub_keys,
	 key );

	/* TODO: add tests for libregf_key_get_sub_key_by_index */

	/* TODO: add tests for libregf_key_get_sub_key_by_utf8_name */

	/* TODO: add tests for libregf_key_get_sub_key_by_utf8_path */

	/* TODO: add tests for libregf_key_get_sub_key_by_utf16_name */

	/* TODO: add tests for libregf_key_get_sub_key_by_utf16_path */

	/* Clean up
	 */
/* TODO: implement
	result = libregf_key_free(
	          &key,
	          &error );

	REGF_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	REGF_TEST_ASSERT_IS_NULL(
	 "key",
	 key );

	REGF_TEST_ASSERT_IS_NULL(
	 "error",
	 error );
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
	if( key != NULL )
	{
		libregf_key_free(
		 &key,
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

