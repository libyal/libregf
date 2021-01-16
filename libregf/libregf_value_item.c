/*
 * Value item functions
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
#include <memory.h>
#include <types.h>

#include "libregf_data_block_key.h"
#include "libregf_data_block_stream.h"
#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_hive_bin_cell.h"
#include "libregf_hive_bins_list.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfcache.h"
#include "libregf_libfdata.h"
#include "libregf_libuna.h"
#include "libregf_multi_string.h"
#include "libregf_unused.h"
#include "libregf_value_item.h"
#include "libregf_value_key.h"

#include "regf_cell_values.h"

/* Creates value item
 * Make sure the value value_item is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_initialize(
     libregf_value_item_t **value_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_initialize";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( *value_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value item value already set.",
		 function );

		return( -1 );
	}
	*value_item = memory_allocate_structure(
	               libregf_value_item_t );

	if( *value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value item.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *value_item,
	     0,
	     sizeof( libregf_value_item_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value item.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_item != NULL )
	{
		memory_free(
		 *value_item );

		*value_item = NULL;
	}
	return( -1 );
}

/* Frees value item
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_free(
     libregf_value_item_t **value_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_free";
	int result            = 1;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( *value_item != NULL )
	{
		if( ( *value_item )->value_key != NULL )
		{
			if( libregf_value_key_free(
			     &( ( *value_item )->value_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value key.",
				 function );

				result = -1;
			}
		}
		if( ( *value_item )->data_buffer != NULL )
		{
			memory_free(
			 ( *value_item )->data_buffer );
		}
		if( ( *value_item )->data_stream != NULL )
		{
			if( libfdata_stream_free(
			     &( ( *value_item )->data_stream ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data stream.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *value_item );

		*value_item = NULL;
	}
	return( result );
}

/* Clones (duplicates) the value item
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_clone(
     libregf_value_item_t **destination_value_item,
     libregf_value_item_t *source_value_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_clone";

	if( destination_value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination value item.",
		 function );

		return( -1 );
	}
	if( *destination_value_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: destination value item already set.",
		 function );

		return( -1 );
	}
	if( source_value_item == NULL )
	{
		*destination_value_item = NULL;

		return( 1 );
	}
	if( libregf_value_item_initialize(
	     destination_value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination value item.",
		 function );

		goto on_error;
	}
	if( libregf_value_key_clone(
	     &( ( *destination_value_item )->value_key ),
	     source_value_item->value_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to clone value key.",
		 function );

		goto on_error;
	}
	if( source_value_item->data_buffer != NULL )
	{
		if( ( source_value_item->data_buffer_size == 0 )
		 || ( source_value_item->data_buffer_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid source value item - invalid data buffer size value out of bounds.",
			 function );

			goto on_error;
		}
		( *destination_value_item )->data_buffer = (uint8_t *) memory_allocate(
		                                                        sizeof( uint8_t ) * source_value_item->data_buffer_size );

		if( ( *destination_value_item )->data_buffer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data buffer.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     ( *destination_value_item )->data_buffer,
		     source_value_item->data_buffer,
		     (size_t) source_value_item->data_buffer_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data buffer.",
			 function );

			goto on_error;
		}
		( *destination_value_item )->data_buffer_size = source_value_item->data_buffer_size;
	}
	if( source_value_item->data_stream != NULL )
	{
		if( libfdata_stream_clone(
		     &( ( *destination_value_item )->data_stream ),
		     source_value_item->data_stream,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone data stream.",
			 function );

			goto on_error;
		}
	}
	( *destination_value_item )->data_type  = source_value_item->data_type;
	( *destination_value_item )->item_flags = source_value_item->item_flags;

	return( 1 );

on_error:
	libregf_value_item_free(
	 destination_value_item,
	 NULL );

	return( -1 );
}

/* Reads a value key
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_read_value_key(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t value_key_offset,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	static char *function                  = "libregf_value_item_read_value_key";
	size_t value_key_data_size             = 0;
	int hive_bin_index                     = 0;
	int result                             = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( value_item->value_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value item - value key value already set.",
		 function );

		return( -1 );
	}
	if( hive_bins_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bins list.",
		 function );

		return( -1 );
	}
	if( hive_bins_list->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bins list - missing IO handle.",
		 function );

		return( -1 );
	}
	value_key_data_size = sizeof( regf_value_key_t );

	if( ( hive_bins_list->io_handle->major_version == 1 )
	 && ( hive_bins_list->io_handle->minor_version <= 1 ) )
	{
		value_key_data_size += 4;
	}
	if( ( value_key_offset == 0 )
	 || ( value_key_offset == 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid value key offset.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     value_key_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 value_key_offset,
		 value_key_offset );

		goto on_error;
	}
	if( hive_bin_cell->size < value_key_data_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: invalid hive bin cell size value too small.\n",
			 function );
		}
#endif
		value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

		return( 1 );
	}
	if( libregf_value_key_initialize(
	     &( value_item->value_key ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value key.",
		 function );

		goto on_error;
	}
/* TODO pass value name hash */
	result = libregf_value_key_read_data(
	          value_item->value_key,
	          hive_bins_list->io_handle,
	          hive_bin_cell->data,
	          hive_bin_cell->size,
	          0,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value key at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 value_key_offset,
		 value_key_offset );

		goto on_error;
	}
	else if( result == 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unsupported value key signature.\n",
			 function );
		}
#endif
		value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

		return( 1 );
	}
/* TODO
		if( value_item->name_size > hive_bin_cell->size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid value name size value out of bounds.\n",
				 function );
			}
#endif
			value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

			return( 1 );
		}
*/

	if( value_item->value_key->data_in_key != 0 )
	{
/* TODO
		if( value_key->data_size > 4 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: invalid data size value out of bounds.\n",
				 function );
			}
#endif
			value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

			return( 1 );
		}
*/
		value_item->data_buffer      = value_item->value_key->data;
		value_item->data_buffer_size = value_item->value_key->data_size;
		value_item->data_type        = LIBREGF_VALUE_ITEM_DATA_TYPE_BUFFER;

		value_item->value_key->data      = NULL;
		value_item->value_key->data_size = 0;
	}
	else
	{
		result = libregf_hive_bins_list_get_index_at_offset(
		          hive_bins_list,
		          (off64_t) value_item->value_key->data_offset,
		          &hive_bin_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine hive bin value index for data offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 value_item->value_key->data_offset,
			 value_item->value_key->data_offset );

			goto on_error;
		}
		else if( result == 0 )
		{
			value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
		}
		else
		{
			if( libregf_value_item_read_value_data(
			     value_item,
			     file_io_handle,
			     hive_bins_list,
			     value_item->value_key->data_offset,
			     value_item->value_key->data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read value data at offset: %" PRIu32 " (0x%08" PRIx32 ").",
				 function,
				 value_item->value_key->data_offset,
				 value_item->value_key->data_offset );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( value_item->data_buffer != NULL )
	{
		memory_free(
		 value_item->data_buffer );

		value_item->data_buffer = NULL;
	}
	value_item->data_buffer_size = 0;

	if( value_item->value_key != NULL )
	{
		libregf_value_key_free(
		 &( value_item->value_key ),
		 NULL );
	}
	return( -1 );
}

/* Reads value data
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_read_value_data(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t value_data_offset,
     uint32_t value_data_size,
     libcerror_error_t **error )
{
	libregf_data_block_key_t *data_block_key = NULL;
	libregf_hive_bin_cell_t *hive_bin_cell   = NULL;
	static char *function                    = "libregf_value_item_read_value_data";
	size_t data_offset                       = 0;
	size_t utf16_string_size                 = 0;
	uint32_t calculated_value_data_size      = 0;
	uint32_t value_type                      = 0;
	int hive_bin_index                       = 0;
	int result                               = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( hive_bins_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bins list.",
		 function );

		return( -1 );
	}
	if( hive_bins_list->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bins list - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( value_data_offset == 0 )
	 || ( value_data_offset == 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid value data offset.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     value_data_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIu32 ".",
		 function,
		 value_data_offset );

		goto on_error;
	}
	/* As of version 1.5 large value data is stored in a data block
	 */
	if( ( value_data_size > 16344 )
	 && ( hive_bins_list->io_handle->major_version >= 1 )
	 && ( hive_bins_list->io_handle->minor_version >= 5 ) )
	{
		if( libregf_data_block_key_initialize(
		     &data_block_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data block key.",
			 function );

			goto on_error;
		}
		result = libregf_data_block_key_read_data(
		          data_block_key,
		          hive_bin_cell->data,
		          hive_bin_cell->size,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block key at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 value_data_offset,
			 value_data_offset );

			goto on_error;
		}
		result = libregf_hive_bins_list_get_index_at_offset(
		          hive_bins_list,
		          (off64_t) data_block_key->data_block_list_offset,
		          &hive_bin_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if data block list offset is valid.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
		}
		else
		{
			if( libregf_value_item_read_data_block_list(
			     value_item,
			     file_io_handle,
			     hive_bins_list,
			     data_block_key->data_block_list_offset,
			     data_block_key->number_of_segments,
			     value_data_size,
			     error ) == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read data block list at offset: %" PRIu32 ".",
				 function,
				 data_block_key->data_block_list_offset );

				goto on_error;
			}
/* TODO: Check if number of segments stored in the data block key matches the calculated */
		}
		if( libregf_data_block_key_free(
		     &data_block_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data block key.",
			 function );

			goto on_error;
		}
	}
	else
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data:\n",
			 function );
			libcnotify_print_data(
			 hive_bin_cell->data,
			 hive_bin_cell->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( ( hive_bins_list->io_handle->major_version == 1 )
		 && ( hive_bins_list->io_handle->minor_version <= 1 ) )
		{
			data_offset = 4;
		}
		if( value_data_size > hive_bin_cell->size )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: value data size: %" PRIu32 " exceeds hive bin cell size: %" PRIzd ".\n",
				 function,
				 value_data_size,
				 hive_bin_cell->size );
			}
#endif
			value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

			if( hive_bin_cell->size > (uint32_t) INT32_MAX )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid hive bin cell size value out of bounds.",
				 function );

				goto on_error;
			}
			value_data_size = (uint32_t) hive_bin_cell->size;
		}
		if( libregf_value_key_get_value_type(
		     value_item->value_key,
		     &value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value type.",
			 function );

			goto on_error;
		}
		if( ( value_type == LIBREGF_VALUE_TYPE_STRING )
		 || ( value_type == LIBREGF_VALUE_TYPE_EXPANDABLE_STRING ) )
		{
			calculated_value_data_size = data_offset;

			while( ( calculated_value_data_size + 2 ) <= (uint32_t) hive_bin_cell->size )
			{
				if( ( hive_bin_cell->data[ calculated_value_data_size ] == 0 )
				 && ( hive_bin_cell->data[ calculated_value_data_size + 1 ] == 0 ) )
				{
					calculated_value_data_size += 2;

					break;
				}
				calculated_value_data_size += 2;
			}
			calculated_value_data_size -= data_offset;

			if( calculated_value_data_size != value_data_size )
			{
				/* Check if the calculated string is sane
				 */
				result = libuna_utf16_string_size_from_utf16_stream(
					  &( hive_bin_cell->data[ data_offset ] ),
					  calculated_value_data_size,
					  LIBUNA_ENDIAN_LITTLE,
					  &utf16_string_size,
					  NULL );

				if( result != 1 )
				{
					calculated_value_data_size = value_data_size;
				}
			}
			if( calculated_value_data_size != value_data_size )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: mismatch in value data size (calculated: %" PRIu32 ", stored: %" PRIu32 ").\n",
					 function,
					 calculated_value_data_size,
					 value_data_size );
				}
#endif
				value_data_size = calculated_value_data_size;
			}
		}
/* TODO what about a maximum of 1 MiB ? */

		value_item->data_type = LIBREGF_VALUE_ITEM_DATA_TYPE_BUFFER;

		if( value_data_size > 0 )
		{
			if( value_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid value data size value exceeds maximum allocation size.",
				 function );

				goto on_error;
			}
			value_item->data_buffer = (uint8_t *) memory_allocate(
			                                       sizeof( uint8_t ) * (size_t) value_data_size );

			if( value_item->data_buffer == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create data buffer.",
				 function );

				goto on_error;
			}
			value_item->data_buffer_size = (size_t) value_data_size;

			if( memory_copy(
			     value_item->data_buffer,
			     &( hive_bin_cell->data[ data_offset ] ),
			     value_data_size  ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy data buffer.",
				 function );

				goto on_error;
			}
			data_offset += value_data_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_offset < hive_bin_cell->size )
			{
				libcnotify_printf(
				 "%s: padding:\n",
				 function );
				libcnotify_print_data(
				 &( hive_bin_cell->data[ data_offset ] ),
				 hive_bin_cell->size - data_offset,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
	}
	return( 1 );

on_error:
	if( value_item->data_buffer != NULL )
	{
		memory_free(
		 value_item->data_buffer );

		value_item->data_buffer = NULL;
	}
	if( data_block_key != NULL )
	{
		libregf_data_block_key_free(
		 &data_block_key,
		 NULL );
	}
	return( -1 );
}

/* Reads a data block (segments) list cell value
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libregf_value_item_read_data_block_list(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t data_block_list_offset,
     uint16_t number_of_segments,
     uint32_t value_data_size,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	uint8_t *list_data                     = NULL;
	static char *function                  = "libregf_value_item_read_data_block_list";
	size_t data_offset                     = 0;
	size_t list_data_size                  = 0;
	uint32_t calculated_value_data_size    = 0;
	uint32_t element_offset                = 0;
	uint32_t segment_offset                = 0;
	uint32_t segment_size                  = 0;
	uint16_t element_iterator              = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( hive_bins_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bins list.",
		 function );

		return( -1 );
	}
	if( hive_bins_list->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bins list - missing IO handle.",
		 function );

		return( -1 );
	}
	if( ( data_block_list_offset == 0 )
	 || ( data_block_list_offset == 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid data block list offset.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     data_block_list_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: unable to retrieve hive bin cell at offset: %" PRIu32 ".\n",
			 function,
			 data_block_list_offset );
		}
#endif
		value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

		return( 0 );
	}
	if( hive_bin_cell == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing hive bins cell.",
		 function );

		return( -1 );
	}
	list_data_size = (size_t) hive_bin_cell->size;

	if( ( list_data_size == 0 )
	 || ( list_data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid list data size value out of bounds.",
		 function );

		goto on_error;
	}
	/* Make a local copy to prevent the hive bin cell data from being cached out
	 */
	list_data = (uint8_t *) memory_allocate(
	                         sizeof( uint8_t ) * list_data_size );

	if( list_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create list data.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     list_data,
	     hive_bin_cell->data,
	     list_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy list data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 list_data,
		 list_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( (size_t) number_of_segments > ( list_data_size / 4 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid cell size value too small to contain number of values.",
		 function );

		goto on_error;
	}
	value_item->data_type = LIBREGF_VALUE_ITEM_DATA_TYPE_BLOCK;

	if( libfdata_stream_initialize(
	     &( value_item->data_stream ),
	     NULL,
	     NULL,
	     NULL,
	     NULL,
	     (ssize_t (*)(intptr_t *, intptr_t *, int, int, uint8_t *, size_t, uint32_t, uint8_t, libcerror_error_t **)) &libregf_data_block_stream_read_segment_data,
	     NULL,
	     (off64_t (*)(intptr_t *, intptr_t *, int, int, off64_t, libcerror_error_t **)) &libregf_data_block_stream_seek_segment_offset,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value data stream.",
		 function );

		goto on_error;
	}
	if( number_of_segments > 0 )
	{
		if( libfdata_stream_resize(
		     value_item->data_stream,
		     (int) number_of_segments,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable to resize value data stream.",
			 function );

			goto on_error;
		}
		for( element_iterator = 0;
		     element_iterator < number_of_segments;
		     element_iterator++ )
		{
			byte_stream_copy_to_uint32_little_endian(
			 &( list_data[ data_offset ] ),
			 element_offset );

			data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: element: %03" PRIu16 " offset\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 element_iterator,
				 element_offset );
			}
#endif
			/* Determine the size of the segment by retrieving the corresponding hive bin cell
			 */
			if( libregf_hive_bins_list_get_cell_at_offset(
			     hive_bins_list,
			     file_io_handle,
			     element_offset,
			     &hive_bin_cell,
			     error ) != 1 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: unable to retrieve data segment: %" PRIu16 " hive bin cell at offset: %" PRIu32 ".\n",
					 function,
					 element_iterator,
					 element_offset );
				}
#endif
				value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

/* TODO return partial stream or 0-byte the missing data ? */
				libfdata_stream_free(
				 &( value_item->data_stream ),
				 NULL );

				return( 0 );
			}
			segment_offset = (uint32_t) ( hive_bins_list->io_handle->hive_bins_list_offset + 4 + element_offset );
			segment_size   = hive_bin_cell->size - 4;

			if( segment_size > 16344 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: data segment: %" PRIu16 " size: %" PRIu32 " exceeds maximum.\n",
					 function,
					 element_iterator,
					 segment_size );
				}
#endif
				value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;

				segment_size = 16344;
			}
			if( ( calculated_value_data_size + segment_size ) > value_data_size )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: mismatch in value data size (calculated: %" PRIu32 ", stored: %" PRIu32 ").\n",
					 function,
					 calculated_value_data_size + segment_size,
					 value_data_size );
				}
#endif
				segment_size = value_data_size - calculated_value_data_size;
			}
			/* Point the stream directly to the data
			 */
			if( libfdata_stream_set_segment_by_index(
			     value_item->data_stream,
			     (int) element_iterator,
			     0,
			     (off64_t) segment_offset,
			     (size64_t) segment_size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to set value data stream segment: %" PRIu16 ".",
				 function,
				 element_iterator );

				goto on_error;
			}
			calculated_value_data_size += segment_size;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < list_data_size )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 &( list_data[ data_offset ] ),
			 list_data_size - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif
	memory_free(
	 list_data );

	return( 1 );

on_error:
	if( value_item->data_stream != NULL )
	{
		libfdata_stream_free(
		 &( value_item->data_stream ),
		 NULL );
	}
	if( list_data != NULL )
	{
		memory_free(
		 list_data );
	}
	return( -1 );
}

/* Reads a value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_read_element_data(
     libregf_hive_bins_list_t *hive_bins_list,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfdata_cache_t *cache,
     int data_range_file_index LIBREGF_ATTRIBUTE_UNUSED,
     off64_t data_range_offset,
     size64_t data_range_size LIBREGF_ATTRIBUTE_UNUSED,
     uint32_t data_range_flags LIBREGF_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBREGF_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libregf_value_item_t *value_item = NULL;
	static char *function            = "libregf_value_item_read_element_data";

	LIBREGF_UNREFERENCED_PARAMETER( data_range_file_index )
	LIBREGF_UNREFERENCED_PARAMETER( data_range_size )
	LIBREGF_UNREFERENCED_PARAMETER( data_range_flags )
	LIBREGF_UNREFERENCED_PARAMETER( read_flags )

	if( hive_bins_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bins list.",
		 function );

		return( -1 );
	}
	if( hive_bins_list->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bins list - missing IO handle.",
		 function );

		return( -1 );
	}
	if( data_range_offset > (off64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data range offset value out of bounds.",
		 function );

		goto on_error;
	}
	if( libregf_value_item_initialize(
	     &value_item,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value item.",
		 function );

		goto on_error;
	}
	if( libregf_value_item_read_value_key(
	     value_item,
	     file_io_handle,
	     hive_bins_list,
	     (uint32_t) data_range_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read value key at offset: %" PRIi64 ".",
		 function,
		 data_range_offset );

		goto on_error;
	}
	if( ( value_item->item_flags & LIBREGF_ITEM_FLAG_IS_CORRUPTED ) != 0 )
	{
		hive_bins_list->io_handle->flags |= LIBREGF_IO_HANDLE_FLAG_IS_CORRUPTED;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) value_item,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libregf_value_item_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set value item as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( value_item != NULL )
	{
		libregf_value_item_free(
		 &value_item,
		 NULL );
	}
	return( -1 );
}

/* Determine if the value corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libregf_value_item_is_corrupted(
     libregf_value_item_t *value_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_is_corrupted";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( ( value_item->item_flags & LIBREGF_ITEM_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the value name size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_name_size(
     libregf_value_item_t *value_item,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_name_size";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_name_size(
	     value_item->value_key,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value name
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_name(
     libregf_value_item_t *value_item,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_name";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_name(
	     value_item->value_key,
	     name,
	     name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_utf8_name_size(
     libregf_value_item_t *value_item,
     size_t *utf8_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_utf8_name_size";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_utf8_name_size(
	     value_item->value_key,
	     utf8_name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_utf8_name(
     libregf_value_item_t *value_item,
     uint8_t *utf8_name,
     size_t utf8_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_utf8_name";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_utf8_name(
	     value_item->value_key,
	     utf8_name,
	     utf8_name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_utf16_name_size(
     libregf_value_item_t *value_item,
     size_t *utf16_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_utf16_name_size";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_utf16_name_size(
	     value_item->value_key,
	     utf16_name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_utf16_name(
     libregf_value_item_t *value_item,
     uint16_t *utf16_name,
     size_t utf16_name_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_utf16_name";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_utf16_name(
	     value_item->value_key,
	     utf16_name,
	     utf16_name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Compares the value name with UTF-8 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_value_item_compare_name_with_utf8_string(
     libregf_value_item_t *value_item,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_compare_name_with_utf8_string";
	int result            = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	result = libregf_value_key_compare_name_with_utf8_string(
	          value_item->value_key,
	          name_hash,
	          utf8_string,
	          utf8_string_length,
	          ascii_codepage,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare name with UTF-8 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Compares the value name with UTF-16 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libregf_value_item_compare_name_with_utf16_string(
     libregf_value_item_t *value_item,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_compare_name_with_utf16_string";
	int result            = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	result = libregf_value_key_compare_name_with_utf16_string(
	          value_item->value_key,
	          name_hash,
	          utf16_string,
	          utf16_string_length,
	          ascii_codepage,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to compare name with UTF-16 string.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_type(
     libregf_value_item_t *value_item,
     uint32_t *data_type,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_value_type";

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     data_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_data_size(
     libregf_value_item_t *value_item,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function    = "libregf_value_item_get_data_size";
	size64_t value_data_size = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( ( value_item->data_type != 0 )
	 && ( value_item->data_type != LIBREGF_VALUE_ITEM_DATA_TYPE_BUFFER )
	 && ( value_item->data_type != LIBREGF_VALUE_ITEM_DATA_TYPE_BLOCK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value item data type: 0x%02" PRIx8 ".",
		 function,
		 value_item->data_type );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( ( value_item->data_type == LIBREGF_VALUE_ITEM_DATA_TYPE_BLOCK )
	 && ( value_item->data_buffer == NULL ) )
	{
		if( libfdata_stream_get_size(
		     value_item->data_stream,
		     &value_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from data stream.",
			 function );

			return( -1 );
		}
		if( value_data_size > (size64_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid stream data size value out of bounds.",
			 function );

			return( -1 );
		}
	}
	else if( value_item->data_type != 0 )
	{
		value_data_size = value_item->data_buffer_size;
	}
	*data_size = (size_t) value_data_size;

	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_data(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function     = "libregf_value_item_get_data";
	size64_t stream_data_size = 0;
	ssize_t read_count        = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( ( value_item->data_type != 0 )
	 && ( value_item->data_type != LIBREGF_VALUE_ITEM_DATA_TYPE_BUFFER )
	 && ( value_item->data_type != LIBREGF_VALUE_ITEM_DATA_TYPE_BLOCK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value item data type: 0x%02" PRIx8 ".",
		 function,
		 value_item->data_type );

		return( -1 );
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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( ( value_item->data_type == LIBREGF_VALUE_ITEM_DATA_TYPE_BLOCK )
	 && ( value_item->data_buffer == NULL ) )
	{
		if( libfdata_stream_get_size(
		     value_item->data_stream,
		     &stream_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from data stream.",
			 function );

			goto on_error;
		}
		if( ( stream_data_size == 0 )
		 || ( stream_data_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid stream data size value out of bounds.",
			 function );

			goto on_error;
		}
		/* Cache the whole stream this should not happen very often "normal" files
		 */
		value_item->data_buffer = (uint8_t *) memory_allocate(
		                                       sizeof( uint8_t ) * (size_t) stream_data_size );

		if( value_item->data_buffer == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data buffer.",
			 function );

			goto on_error;
		}
		value_item->data_buffer_size = (size_t) stream_data_size;

		read_count = libfdata_stream_read_buffer_at_offset(
		              value_item->data_stream,
		              (intptr_t *) file_io_handle,
		              value_item->data_buffer,
		              value_item->data_buffer_size,
		              0,
		              0,
		              error );

		if( read_count != (ssize_t) value_item->data_buffer_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data from data stream at offset: 0 (0x00000000).",
			 function );

			goto on_error;
		}
	}
	if( value_item->data_type != 0 )
	{
		*data      = value_item->data_buffer;
		*data_size = value_item->data_buffer_size;
	}
	else
	{
		*data      = NULL;
		*data_size = 0;
	}
	return( 1 );

on_error:
	if( value_item->data_buffer != NULL )
	{
		memory_free(
		 value_item->data_buffer );

		value_item->data_buffer = NULL;
	}
	value_item->data_buffer_size = 0;

	return( -1 );
}

/* Retrieves the 32-bit value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_32bit(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_32bit";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( value_32bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 32-bit.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( ( value_type != LIBREGF_VALUE_TYPE_INTEGER_32BIT_BIG_ENDIAN )
	 && ( value_type != LIBREGF_VALUE_TYPE_INTEGER_32BIT_LITTLE_ENDIAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported 32-bit integer value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size < 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	else if( value_data_size > 4 )
	{
		value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
	}
	if( value_type == LIBREGF_VALUE_TYPE_INTEGER_32BIT_BIG_ENDIAN )
	{
		byte_stream_copy_to_uint32_big_endian(
		 value_data,
		 *value_32bit );
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 value_data,
		 *value_32bit );
	}
	return( 1 );
}

/* Retrieves the 64-bit value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_64bit(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_64bit";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( value_64bit == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value 64-bit.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( value_type != LIBREGF_VALUE_TYPE_INTEGER_64BIT_LITTLE_ENDIAN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported 64-bit integer value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size < 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	else if( value_data_size > 8 )
	{
		value_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
	}
	byte_stream_copy_to_uint64_little_endian(
	 value_data,
	 *value_64bit );

	return( 1 );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_utf8_string_size(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_utf8_string_size";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( ( value_type != LIBREGF_VALUE_TYPE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == 0 )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_utf16_stream(
		     value_data,
		     value_data_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_utf8_string(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_utf8_string";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( ( value_type != LIBREGF_VALUE_TYPE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     value_data,
	     value_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size at a specific value from the referenced value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_utf16_string_size(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_utf16_string_size";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( ( value_type != LIBREGF_VALUE_TYPE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == 0 )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_utf16_stream(
		     value_data,
		     value_data_size,
		     LIBUNA_ENDIAN_LITTLE,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_utf16_string(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_utf16_string";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( ( value_type != LIBREGF_VALUE_TYPE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( value_type != LIBREGF_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     value_data,
	     value_data_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the binary data size
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_binary_data_size(
     libregf_value_item_t *value_item,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_value_item_get_value_binary_data_size";
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( value_type != LIBREGF_VALUE_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported binary data value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
	if( libregf_value_item_get_data_size(
	     value_item,
	     binary_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the binary data value
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_binary_data(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_binary_data";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( binary_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid binary data.",
		 function );

		return( -1 );
	}
	if( binary_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid binary data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		return( -1 );
	}
	if( value_type != LIBREGF_VALUE_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported binary data value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		return( -1 );
	}
/* TODO optimize to read directly from block or stream */
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value data.",
		 function );

		return( -1 );
	}
	if( binary_data_size < value_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: binary data is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     binary_data,
	     value_data,
	     value_data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the multi string value
 * Creates a new multi string
 * Returns 1 if successful or -1 on error
 */
int libregf_value_item_get_value_multi_string(
     libregf_value_item_t *value_item,
     libbfio_handle_t *file_io_handle,
     libregf_multi_string_t **multi_string,
     libcerror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libregf_value_item_get_value_multi_string";
	size_t value_data_size = 0;
	uint32_t value_type    = 0;

	if( value_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value item.",
		 function );

		return( -1 );
	}
	if( multi_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi string.",
		 function );

		return( -1 );
	}
	if( libregf_value_key_get_value_type(
	     value_item->value_key,
	     &value_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value type.",
		 function );

		goto on_error;
	}
	if( value_type != LIBREGF_VALUE_TYPE_MULTI_VALUE_STRING )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 value_type );

		goto on_error;
	}
	if( libregf_value_item_get_data(
	     value_item,
	     file_io_handle,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		goto on_error;
	}
	if( libregf_multi_string_initialize(
	     multi_string,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create multi string.",
		 function );

		goto on_error;
	}
	if( libregf_internal_multi_string_read_data(
	     (libregf_internal_multi_string_t *) *multi_string,
	     value_data,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read multi string from value data.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *multi_string != NULL )
	{
		libregf_multi_string_free(
		 multi_string,
		 NULL );
	}
	return( -1 );
}

