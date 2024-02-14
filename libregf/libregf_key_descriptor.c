/*
 * Key descriptor functions
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
#include <memory.h>
#include <types.h>

#include "libregf_key_descriptor.h"
#include "libregf_libcerror.h"

/* Creates a key descriptor
 * Make sure the value key_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_key_descriptor_initialize(
     libregf_key_descriptor_t **key_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_descriptor_initialize";

	if( key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key descriptor.",
		 function );

		return( -1 );
	}
	if( *key_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key descriptor value already set.",
		 function );

		return( -1 );
	}
	*key_descriptor = memory_allocate_structure(
	                   libregf_key_descriptor_t );

	if( *key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key descriptor.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_descriptor,
	     0,
	     sizeof( libregf_key_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key descriptor.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_descriptor != NULL )
	{
		memory_free(
		 *key_descriptor );

		*key_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a key descriptor
 * Returns 1 if successful or -1 on error
 */
int libregf_key_descriptor_free(
     libregf_key_descriptor_t **key_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_descriptor_free";

	if( key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key descriptor.",
		 function );

		return( -1 );
	}
	if( *key_descriptor != NULL )
	{
		memory_free(
		 *key_descriptor );

		*key_descriptor = NULL;
	}
	return( 1 );
}

