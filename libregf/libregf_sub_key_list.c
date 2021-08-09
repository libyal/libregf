/*
 * Sub key list functions
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
#include <memory.h>
#include <types.h>

#include "libregf_key_descriptor.h"
#include "libregf_io_handle.h"
#include "libregf_libcdata.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_sub_key_list.h"

#include "regf_cell_values.h"

/* Creates a sub key list
 * Make sure the value sub_key_list is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_sub_key_list_initialize(
     libregf_sub_key_list_t **sub_key_list,
     libcerror_error_t **error )
{
	static char *function = "libregf_sub_key_list_initialize";

	if( sub_key_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key list.",
		 function );

		return( -1 );
	}
	if( *sub_key_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub key list value already set.",
		 function );

		return( -1 );
	}
	*sub_key_list = memory_allocate_structure(
	                 libregf_sub_key_list_t );

	if( *sub_key_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create sub key list.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *sub_key_list,
	     0,
	     sizeof( libregf_sub_key_list_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear sub key list.",
		 function );

		memory_free(
		 *sub_key_list );

		*sub_key_list = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *sub_key_list )->sub_key_descriptors ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub key descriptors.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *sub_key_list != NULL )
	{
		memory_free(
		 *sub_key_list );

		*sub_key_list = NULL;
	}
	return( -1 );
}

/* Frees a sub key list
 * Returns 1 if successful or -1 on error
 */
int libregf_sub_key_list_free(
     libregf_sub_key_list_t **sub_key_list,
     libcerror_error_t **error )
{
	static char *function = "libregf_sub_key_list_free";
	int result            = 1;

	if( sub_key_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key list.",
		 function );

		return( -1 );
	}
	if( *sub_key_list != NULL )
	{
		if( libcdata_array_free(
		     &( ( *sub_key_list )->sub_key_descriptors ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libregf_key_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key descriptors.",
			 function );

			result = -1;
		}
		memory_free(
		 *sub_key_list );

		*sub_key_list = NULL;
	}
	return( result );
}

/* Reads a sub key list
 * Returns 1 if successful or -1 on error
 */
int libregf_sub_key_list_read_data(
     libregf_sub_key_list_t *sub_key_list,
     libregf_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libregf_sub_key_list_read_data";
	size_t data_offset                           = 0;
	size_t sub_key_list_data_size                = 0;
	uint16_t element_index                       = 0;
	uint16_t number_of_elements                  = 0;
	uint8_t element_data_size                    = 0;
	int entry_index                              = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                         = 0;
#endif

	if( sub_key_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key list.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	sub_key_list_data_size = sizeof( regf_sub_key_list_t );

	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		sub_key_list_data_size += 4;
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sub_key_list_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: sub key list data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( ( io_handle->major_version == 1 )
	 && ( io_handle->minor_version <= 1 ) )
	{
		data_offset += 4;
	}
	/* Check if the signature matches that of a sub key list: "lf", "lh", "li" or "ri"
	 */
	if( ( data[ data_offset ] == (uint8_t) 'r' )
	 && ( data[ data_offset + 1 ] == (uint8_t) 'i' ) )
	{
		element_data_size           = 4;
		sub_key_list->at_leaf_level = 0;
	}
	else if( ( data[ data_offset ] == (uint8_t) 'l' )
	      && ( data[ data_offset + 1 ] == (uint8_t) 'i' ) )
	{
		element_data_size           = 4;
		sub_key_list->at_leaf_level = 1;
	}
	else if( ( data[ data_offset ] == (uint8_t) 'l' )
	      && ( ( data[ data_offset + 1 ] == (uint8_t) 'f' )
	        || ( data[ data_offset + 1 ] == (uint8_t) 'h' ) ) )
	{
		element_data_size           = 8;
		sub_key_list->at_leaf_level = 1;
	}
	else
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported sub key list signature.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint16_little_endian(
	 ( (regf_sub_key_list_t *) &( data[ data_offset ] ) )->number_of_elements,
	 number_of_elements );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( ( io_handle->major_version == 1 )
		 && ( io_handle->minor_version <= 1 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 data,
			 value_32bit );
			libcnotify_printf(
			 "%s: unknown0\t\t\t\t\t: 0x%08" PRIx32 " (%" PRIi32 ")\n",
			 function,
			 value_32bit,
			 (int32_t) value_32bit );
		}
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c\n",
		 function,
		 ( (regf_sub_key_list_t *) &( data[ data_offset ] ) )->signature[ 0 ],
		 ( (regf_sub_key_list_t *) &( data[ data_offset ] ) )->signature[ 1 ] );

		libcnotify_printf(
		 "%s: number of elements\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_elements );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset += sizeof( regf_sub_key_list_t );

	if( (size_t) number_of_elements > ( ( data_size - data_offset ) / element_data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of elements of elements value out of bounds.",
		 function );

		goto on_error;
	}
	for( element_index = 0;
	     element_index < number_of_elements;
	     element_index++ )
	{
		if( libregf_key_descriptor_initialize(
		     &sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub key descriptor.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_offset ] ),
		 sub_key_descriptor->key_offset );

		data_offset += 4;

		if( element_data_size == 8 )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( data[ data_offset ] ),
			 sub_key_descriptor->hash_value );

			data_offset += 4;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: element: %03" PRIu16 " offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 element_index,
			 sub_key_descriptor->key_offset );

			if( element_data_size == 8 )
			{
				libcnotify_printf(
				 "%s: element: %03" PRIu16 " hash\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 element_index,
				 sub_key_descriptor->hash_value );
			}
		}
#endif
		if( libcdata_array_append_entry(
		     sub_key_list->sub_key_descriptors,
		     &entry_index,
		     (intptr_t *) sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub key descriptor to array.",
			 function );

			goto on_error;
		}
		sub_key_descriptor = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_size )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ]),
			 data_size - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( sub_key_descriptor != NULL )
	{
		libregf_key_descriptor_free(
		 &sub_key_descriptor,
		 NULL );
	}
	return( -1 );
}

