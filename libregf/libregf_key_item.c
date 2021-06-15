/*
 * Key item functions
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

#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_hive_bins_list.h"
#include "libregf_io_handle.h"
#include "libregf_key_descriptor.h"
#include "libregf_key_item.h"
#include "libregf_libbfio.h"
#include "libregf_libcdata.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfcache.h"
#include "libregf_libfdata.h"
#include "libregf_libuna.h"
#include "libregf_named_key.h"
#include "libregf_security_key.h"
#include "libregf_sub_key_list.h"
#include "libregf_unused.h"
#include "libregf_value_item.h"

#include "regf_cell_values.h"

/* Creates key item
 * Make sure the key_item is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_initialize(
     libregf_key_item_t **key_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_initialize";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( *key_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item value already set.",
		 function );

		return( -1 );
	}
	*key_item = memory_allocate_structure(
	             libregf_key_item_t );

	if( *key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key item.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_item,
	     0,
	     sizeof( libregf_key_item_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key item.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *key_item )->sub_key_descriptors ),
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
	if( *key_item != NULL )
	{
		memory_free(
		 *key_item );

		*key_item = NULL;
	}
	return( -1 );
}

/* Frees key item
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_free(
     libregf_key_item_t **key_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_free";
	int result            = 1;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( *key_item != NULL )
	{
		if( ( *key_item )->named_key != NULL )
		{
			if( libregf_named_key_free(
			     &( ( *key_item )->named_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free the named key.",
				 function );

				result = -1;
			}
		}
		if( ( *key_item )->class_name != NULL )
		{
			memory_free(
			 ( *key_item )->class_name );
		}
		if( ( *key_item )->security_descriptor != NULL )
		{
			memory_free(
			 ( *key_item )->security_descriptor );
		}
		if( ( *key_item )->values_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *key_item )->values_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free the values data list.",
				 function );

				result = -1;
			}
		}
		if( ( *key_item )->values_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *key_item )->values_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free values cache.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *key_item )->sub_key_descriptors ),
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
		 *key_item );

		*key_item = NULL;
	}
	return( result );
}

/* Reads a key item
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_read(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     off64_t key_offset,
     uint32_t named_key_hash,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_read";
	int hive_bin_index    = 0;
	int result            = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->named_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item - named key value already set.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_initialize(
	     &( key_item->named_key ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create named key.",
		 function );

		goto on_error;
	}
	if( libregf_key_item_read_named_key(
	     key_item->named_key,
	     file_io_handle,
	     hive_bins_list,
	     key_offset,
	     named_key_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read named key at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 key_offset,
		 key_offset );

		goto on_error;
	}
	if( libregf_key_item_read_class_name(
	     key_item,
	     file_io_handle,
	     hive_bins_list,
	     key_item->named_key->class_name_offset,
	     key_item->named_key->class_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read class name at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 key_item->named_key->class_name_offset,
		 key_item->named_key->class_name_offset );

		goto on_error;
	}
	if( key_item->named_key->security_key_offset != 0xffffffffUL )
	{
		if( libregf_key_item_read_security_key(
		     key_item,
		     file_io_handle,
		     hive_bins_list,
		     key_item->named_key->security_key_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read security key at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 key_item->named_key->security_key_offset,
			 key_item->named_key->security_key_offset );

			goto on_error;
		}
	}
	if( key_item->named_key->number_of_sub_keys > 0 )
	{
		result = libregf_key_item_read_sub_keys_list(
		          key_item->sub_key_descriptors,
		          file_io_handle,
		          hive_bins_list,
		          (off64_t) key_item->named_key->sub_keys_list_offset,
		          0,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sub keys list at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 key_item->named_key->sub_keys_list_offset,
			 key_item->named_key->sub_keys_list_offset );

			goto on_error;
		}
		else if( result == 0 )
		{
			key_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
		}
	}
/* TODO clone function */
	if( libfdata_list_initialize(
	     &( key_item->values_list ),
	     (intptr_t *) hive_bins_list,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libregf_value_item_read_element_data,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values data list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( key_item->values_cache ),
	     LIBREGF_MAXIMUM_CACHE_ENTRIES_VALUES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values cache.",
		 function );

		goto on_error;
	}
	result = libregf_hive_bins_list_get_index_at_offset(
	          hive_bins_list,
	          (off64_t) key_item->named_key->values_list_offset,
	          &hive_bin_index,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if values list offset is valid.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		key_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
	}
	else
	{
		if( libregf_key_item_read_values_list(
		     key_item,
		     file_io_handle,
		     hive_bins_list,
		     key_item->named_key->values_list_offset,
		     key_item->named_key->number_of_values,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values list at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 key_item->named_key->values_list_offset,
			 key_item->named_key->values_list_offset );

			goto on_error;
		}
	}
	/* The values and sub keys are read on demand
	 */
	return( 1 );

on_error:
	if( key_item->values_cache != NULL )
	{
		libfcache_cache_free(
		 &( key_item->values_cache ),
		 NULL );
	}
	if( key_item->values_list != NULL )
	{
		libfdata_list_free(
		 &( key_item->values_list ),
		 NULL );
	}
	if( key_item->security_descriptor != NULL )
	{
		memory_free(
		 key_item->security_descriptor );

		key_item->security_descriptor = NULL;
	}
	key_item->class_name_size = 0;

	if( key_item->class_name != NULL )
	{
		memory_free(
		 key_item->class_name );

		key_item->class_name = NULL;
	}
	key_item->class_name_size = 0;

	if( key_item->named_key != NULL )
	{
		libregf_named_key_free(
		 &( key_item->named_key ),
		 NULL );
	}
	libcdata_array_empty(
	 key_item->sub_key_descriptors,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libregf_key_descriptor_free,
	 NULL );

	return( -1 );
}

/* Reads a named key
 * Returns the number of bytes read if successful or -1 on error
 */
int libregf_key_item_read_named_key(
     libregf_named_key_t *named_key,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     off64_t named_key_offset,
     uint32_t named_key_hash,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	static char *function                  = "libregf_key_item_read_named_key";

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
	if( ( named_key_offset == 0 )
	 || ( named_key_offset >= (off64_t) 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid named key offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading named key at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 named_key_offset,
		 named_key_offset );
	}
#endif
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     (uint32_t) named_key_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin cell at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 named_key_offset,
		 named_key_offset );

		return( -1 );
	}
	if( libregf_named_key_read_data(
	     named_key,
	     hive_bins_list->io_handle,
	     hive_bin_cell->data,
	     hive_bin_cell->size,
	     named_key_hash,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read named key at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 named_key_offset,
		 named_key_offset );

		return( -1 );
	}
	return( 1 );
}

/* Reads a class name
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_read_class_name_data(
     libregf_key_item_t *key_item,
     const uint8_t *data,
     size_t data_size,
     uint16_t class_name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_read_class_name_data";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->class_name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item - class name value already set.",
		 function );

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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	if( ( class_name_size == 0 )
	 || ( (size_t) class_name_size > data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid class name size value out of bounds.",
		 function );

		goto on_error;
	}
	key_item->class_name = (uint8_t *) memory_allocate(
	                                    sizeof( uint8_t ) * (size_t) class_name_size );

	if( key_item->class_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create class name.",
		 function );

		goto on_error;
	}
	if( memory_copy(
	     key_item->class_name,
	     data,
	     (size_t) class_name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to class name.",
		 function );

		goto on_error;
	}
	key_item->class_name_size = class_name_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( libregf_debug_print_utf16_string_value(
		     function,
		     "class name\t\t\t",
		     key_item->class_name,
		     (size_t) key_item->class_name_size,
		     LIBUNA_ENDIAN_LITTLE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print UTF-16 string value.",
			 function );

			goto on_error;
		}
		if( (size_t) class_name_size < data_size )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 &( data[ (size_t) class_name_size ] ),
			 data_size - (size_t) class_name_size,
			 0 );
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
	if( key_item->class_name != NULL )
	{
		memory_free(
		 key_item->class_name );

		key_item->class_name = NULL;
	}
	key_item->class_name_size = 0;

	return( -1 );
}

/* Reads a class name
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_read_class_name(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t class_name_offset,
     uint16_t class_name_size,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	static char *function                  = "libregf_key_item_read_class_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->class_name != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item - class name value already set.",
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
	if( class_name_offset == 0xffffffffUL )
	{
		return( 1 );
	}
	if( ( class_name_offset == 0 )
	 && ( class_name_size == 0 ) )
	{
		return( 1 );
	}
	if( class_name_offset == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid class name offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading class name at offset: %" PRIi64 " (0x%08" PRIx64 ").\n",
		 function,
		 class_name_offset,
		 class_name_offset );
	}
#endif
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     class_name_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 class_name_offset,
		 class_name_offset );

		return( -1 );
	}
	if( libregf_key_item_read_class_name_data(
	     key_item,
	     hive_bin_cell->data,
	     hive_bin_cell->size,
	     class_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read class name at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 class_name_offset,
		 class_name_offset );

		return( -1 );
	}
	return( 1 );
}

/* Reads a security key
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_read_security_key(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t security_key_offset,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	libregf_security_key_t *security_key   = NULL;
	static char *function                  = "libregf_key_item_read_security_key";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->security_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item - security descriptor value already set.",
		 function );

		return( -1 );
	}
	if( ( security_key_offset == 0 )
	 || ( security_key_offset == 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid security key offset.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     security_key_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 security_key_offset,
		 security_key_offset );

		goto on_error;
	}
	if( libregf_security_key_initialize(
	     &security_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create security key.",
		 function );

		goto on_error;
	}
	if( libregf_security_key_read_data(
	     security_key,
	     hive_bins_list->io_handle,
	     hive_bin_cell->data,
	     hive_bin_cell->size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read security key at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 security_key_offset,
		 security_key_offset );

		goto on_error;
	}
	key_item->security_descriptor      = security_key->security_descriptor;
	key_item->security_descriptor_size = security_key->security_descriptor_size;

	security_key->security_descriptor      = NULL;
	security_key->security_descriptor_size = 0;

	if( libregf_security_key_free(
	     &security_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free security key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( security_key != NULL )
	{
		libregf_security_key_free(
		 &security_key,
		 NULL );
	}
	return( -1 );
}

/* Reads a values list
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_read_values_list(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t values_list_offset,
     uint32_t number_of_values_list_elements,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	static char *function                  = "libregf_key_item_read_values_list";
	size_t data_offset                     = 0;
	size_t hive_bin_cell_size              = 0;
	uint32_t values_list_element_index     = 0;
	uint32_t values_list_element_offset    = 0;
	int element_index                      = 0;
	int hive_bin_index                     = 0;
	int result                             = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
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
	if( number_of_values_list_elements == 0 )
	{
		return( 1 );
	}
	if( ( values_list_offset == 0 )
	 || ( values_list_offset == 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid values list offset.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     values_list_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIu32 " (0x%08" PRIx32 ").",
		 function,
		 values_list_offset ,
		 values_list_offset );

		return( -1 );
	}
	hive_bin_cell_size = hive_bin_cell->size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 hive_bin_cell->data,
		 hive_bin_cell_size,
		 0 );
	}
#endif
	if( ( hive_bins_list->io_handle->major_version == 1 )
	 && ( hive_bins_list->io_handle->minor_version <= 1 ) )
	{
		data_offset += 4;
	}
	if( ( data_offset > hive_bin_cell_size )
	 || ( number_of_values_list_elements > ( ( hive_bin_cell_size - data_offset ) / 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid cell size value too small to contain number of values.",
		 function );

		return( -1 );
	}
	for( values_list_element_index = 0;
	     values_list_element_index < number_of_values_list_elements;
	     values_list_element_index++ )
	{
		byte_stream_copy_to_uint32_little_endian(
		 &( hive_bin_cell->data[ data_offset ] ),
		 values_list_element_offset );

		data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: element: %03" PRIu32 " offset\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 values_list_element_index,
			 values_list_element_offset );
		}
#endif
		result = libregf_hive_bins_list_get_index_at_offset(
		          hive_bins_list,
		          (off64_t) values_list_element_offset,
		          &hive_bin_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if values list element offset is valid.",
			 function );

			return( -1 );
		}
		else if( result == 0 )
		{
			key_item->item_flags |= LIBREGF_ITEM_FLAG_IS_CORRUPTED;
		}
		else
		{
			if( libfdata_list_append_element(
			     key_item->values_list,
			     &element_index,
			     0,
			     (off64_t) values_list_element_offset,
			     0,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
				 "%s: unable to set value list element: %" PRIu32 " in list.",
				 function,
				 values_list_element_index );

				return( -1 );
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < hive_bin_cell_size )
		{
			libcnotify_printf(
			 "%s: padding:\n",
			 function );
			libcnotify_print_data(
			 &( hive_bin_cell->data[ data_offset ] ),
			 hive_bin_cell_size - data_offset,
			 0 );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

/* Reads a sub keys list
 * Returns 1 if successful, 0 if not or -1 on error
 */
int libregf_key_item_read_sub_keys_list(
     libcdata_array_t *sub_key_descriptors,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     off64_t sub_keys_list_offset,
     int recursion_depth,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell       = NULL;
	libregf_key_descriptor_t *sub_key_descriptor = NULL;
	libregf_sub_key_list_t *sub_key_list         = NULL;
	static char *function                        = "libregf_key_item_read_sub_keys_list";
	int corruption_detected                      = 0;
	int entry_index                              = 0;
	int hive_bin_index                           = 0;
	int number_of_sub_key_descriptors            = 0;
	int result                                   = 0;
	int sub_key_descriptor_index                 = 0;

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
	if( ( sub_keys_list_offset == 0 )
	 || ( sub_keys_list_offset == (off64_t) 0xffffffffUL ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid sub keys list offset.",
		 function );

		return( -1 );
	}
	if( ( recursion_depth < 0 )
	 || ( recursion_depth > LIBREGF_MAXIMUM_SUB_KEY_RECURSION_DEPTH ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid recursion depth value out of bounds.",
		 function );

		return( -1 );
	}
	if( libregf_hive_bins_list_get_cell_at_offset(
	     hive_bins_list,
	     file_io_handle,
	     (uint32_t) sub_keys_list_offset,
	     &hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 sub_keys_list_offset,
		 sub_keys_list_offset );

		goto on_error;
	}
	if( libregf_sub_key_list_initialize(
	     &sub_key_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub key list.",
		 function );

		goto on_error;
	}
	if( libregf_sub_key_list_read_data(
	     sub_key_list,
	     hive_bins_list->io_handle,
	     hive_bin_cell->data,
	     hive_bin_cell->size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read sub key list at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 sub_keys_list_offset,
		 sub_keys_list_offset );

		goto on_error;
	}
	if( libcdata_array_get_number_of_entries(
	     sub_key_list->sub_key_descriptors,
	     &number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from sub key descriptors array.",
		 function );

		goto on_error;
	}
	for( sub_key_descriptor_index = 0;
	     sub_key_descriptor_index < number_of_sub_key_descriptors;
	     sub_key_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     sub_key_list->sub_key_descriptors,
		     sub_key_descriptor_index,
		     (intptr_t **) &sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key descriptor: %d from array.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor: %d.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		result = libregf_hive_bins_list_get_index_at_offset(
		          hive_bins_list,
		          (off64_t) sub_key_descriptor->key_offset,
		          &hive_bin_index,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if sub keys list element: %d offset is valid.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( sub_key_list->at_leaf_level != 0 )
			{
				if( libcdata_array_set_entry_by_index(
				     sub_key_list->sub_key_descriptors,
				     sub_key_descriptor_index,
				     NULL,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set sub key descriptor: %d in array.",
					 function,
					 sub_key_descriptor_index );

					goto on_error;
				}
				if( libcdata_array_append_entry(
				     sub_key_descriptors,
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

					libregf_key_descriptor_free(
					 &sub_key_descriptor,
					 NULL );

					goto on_error;
				}
				sub_key_descriptor = NULL;
			}
			else
			{
				result = libregf_key_item_read_sub_keys_list(
					  sub_key_descriptors,
					  file_io_handle,
					  hive_bins_list,
				          (off64_t) sub_key_descriptor->key_offset,
				          recursion_depth + 1,
					  error );

				if( result == -1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_READ_FAILED,
					 "%s: unable to read sub keys list at offset: %" PRIu32 " (0x%08" PRIx32 ").",
					 function,
					 sub_key_descriptor->key_offset,
					 sub_key_descriptor->key_offset );

					goto on_error;
				}
			}
		}
		if( result == 0 )
		{
			corruption_detected = 1;
		}
	}
	if( libregf_sub_key_list_free(
	     &sub_key_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub key list.",
		 function );

		goto on_error;
	}
	if( corruption_detected != 0 )
	{
		return( 0 );
	}
	return( 1 );

on_error:
	if( sub_key_list != NULL )
	{
		libregf_sub_key_list_free(
		 &sub_key_list,
		 NULL );
	}
	libcdata_array_empty(
	 sub_key_descriptors,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libregf_key_descriptor_free,
	 NULL );

	return( -1 );
}

/* Determines if the key item is corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libregf_key_item_is_corrupted(
     libregf_key_item_t *key_item,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_is_corrupted";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->item_flags & LIBREGF_ITEM_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_name_size(
     libregf_key_item_t *key_item,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_name_size(
	     key_item->named_key,
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

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_name(
     libregf_key_item_t *key_item,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_name(
	     key_item->named_key,
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

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_utf8_name_size(
     libregf_key_item_t *key_item,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf8_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_utf8_name_size(
	     key_item->named_key,
	     utf8_string_size,
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

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_utf8_name(
     libregf_key_item_t *key_item,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf8_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_utf8_name(
	     key_item->named_key,
	     utf8_string,
	     utf8_string_size,
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

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_utf16_name_size(
     libregf_key_item_t *key_item,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf16_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_utf16_name_size(
	     key_item->named_key,
	     utf16_string_size,
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

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_utf16_name(
     libregf_key_item_t *key_item,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf16_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_utf16_name(
	     key_item->named_key,
	     utf16_string,
	     utf16_string_size,
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

/* Retrieves the class name size
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_class_name_size(
     libregf_key_item_t *key_item,
     size_t *class_name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_class_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( class_name_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid class name size.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	*class_name_size = key_item->class_name_size;

	return( 1 );
}

/* Retrieves the class name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_class_name(
     libregf_key_item_t *key_item,
     uint8_t *class_name,
     size_t class_name_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_class_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( class_name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid class name.",
		 function );

		return( -1 );
	}
	if( class_name_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid class name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	if( class_name_size < key_item->class_name_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid class name size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     class_name,
	     key_item->class_name,
	     key_item->class_name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy class name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the class name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_utf8_class_name_size(
     libregf_key_item_t *key_item,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf8_class_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_size_from_utf16_stream(
	     key_item->class_name,
	     (size_t) key_item->class_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the class name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_utf8_class_name(
     libregf_key_item_t *key_item,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf8_class_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf8_string_copy_from_utf16_stream(
	     utf8_string,
	     utf8_string_size,
	     key_item->class_name,
	     (size_t) key_item->class_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( 1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the class name
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_utf16_class_name_size(
     libregf_key_item_t *key_item,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf16_class_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_size_from_utf16_stream(
	     key_item->class_name,
	     (size_t) key_item->class_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the class name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_utf16_class_name(
     libregf_key_item_t *key_item,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_utf16_class_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->class_name == NULL )
	 || ( key_item->class_name_size == 0 ) )
	{
		return( 0 );
	}
	if( libuna_utf16_string_copy_from_utf16_stream(
	     utf16_string,
	     utf16_string_size,
	     key_item->class_name,
	     (size_t) key_item->class_name_size,
	     LIBUNA_ENDIAN_LITTLE,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( 1 );
	}
	return( 1 );
}

/* Retrieves the 64-bit FILETIME value of the last written date and time
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_last_written_time(
     libregf_key_item_t *key_item,
     uint64_t *filetime,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_last_written_time";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libregf_named_key_get_last_written_time(
	     key_item->named_key,
	     filetime,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve last written time.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the security descriptor size
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_security_descriptor_size(
     libregf_key_item_t *key_item,
     size_t *security_descriptor_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_security_descriptor_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( security_descriptor_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor size.",
		 function );

		return( -1 );
	}
	if( ( key_item->security_descriptor == NULL )
	 || ( key_item->security_descriptor_size == 0 ) )
	{
		return( 0 );
	}
	*security_descriptor_size = key_item->security_descriptor_size;

	return( 1 );
}

/* Retrieves the security descriptor
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_security_descriptor(
     libregf_key_item_t *key_item,
     uint8_t *security_descriptor,
     size_t security_descriptor_size,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_security_descriptor";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( security_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid security descriptor.",
		 function );

		return( -1 );
	}
	if( security_descriptor_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid security descriptor size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( key_item->security_descriptor == NULL )
	 || ( key_item->security_descriptor_size == 0 ) )
	{
		return( 0 );
	}
	if( security_descriptor_size < key_item->security_descriptor_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid security descriptor size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     security_descriptor,
	     key_item->security_descriptor,
	     key_item->security_descriptor_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy security descriptor.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_number_of_values(
     libregf_key_item_t *key_item,
     int *number_of_values,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_number_of_values";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_number_of_elements(
	     key_item->values_list,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from values data list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of sub key descriptors
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_number_of_sub_key_descriptors(
     libregf_key_item_t *key_item,
     int *number_of_sub_key_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_number_of_sub_key_descriptors";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors,
	     number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub key descriptors.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific sub key descriptor
 * Returns 1 if successful or -1 on error
 */
int libregf_key_item_get_sub_key_descriptor_by_index(
     libregf_key_item_t *key_item,
     int sub_key_descriptor_index,
     libregf_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libregf_key_item_get_sub_key_descriptor_by_index";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     key_item->sub_key_descriptors,
             sub_key_descriptor_index,
	     (intptr_t **) sub_key_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor: %d.",
		 function,
		 sub_key_descriptor_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub key descriptor for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_sub_key_descriptor_by_utf8_name(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	libregf_key_descriptor_t *safe_sub_key_descriptor = NULL;
	libregf_named_key_t *named_key                    = NULL;
	static char *function                             = "libregf_key_item_get_sub_key_descriptor_by_utf8_name";
	int number_of_sub_key_descriptors                 = 0;
	int result                                        = 0;
	int sub_key_descriptor_index                      = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
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
	*sub_key_descriptor = NULL;

/* TODO think of caching and/or optimization
 * e.g. mapping sub key descriptors to name search tree?
 */
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors,
	     &number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from sub key descriptors array.",
		 function );

		goto on_error;
	}
	for( sub_key_descriptor_index = 0;
	     sub_key_descriptor_index < number_of_sub_key_descriptors;
	     sub_key_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     key_item->sub_key_descriptors,
		     sub_key_descriptor_index,
		     (intptr_t **) &safe_sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key descriptor: %d from array.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( safe_sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor: %d.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( libregf_named_key_initialize(
		     &named_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create named key.",
			 function );

			goto on_error;
		}
		if( libregf_key_item_read_named_key(
		     named_key,
		     file_io_handle,
		     hive_bins_list,
		     safe_sub_key_descriptor->key_offset,
		     safe_sub_key_descriptor->hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read named key at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 safe_sub_key_descriptor->key_offset,
			 safe_sub_key_descriptor->key_offset );

			goto on_error;
		}
		result = libregf_named_key_compare_name_with_utf8_string(
		          named_key,
		          name_hash,
		          utf8_string,
		          utf8_string_length,
		          hive_bins_list->io_handle->ascii_codepage,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare named key with UTF-8 string.",
			 function );

			goto on_error;
		}
		if( libregf_named_key_free(
		     &named_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free named key.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( sub_key_descriptor_index >= number_of_sub_key_descriptors )
	{
		return( 0 );
	}
	*sub_key_descriptor = safe_sub_key_descriptor;

	return( 1 );

on_error:
	if( named_key != NULL )
	{
		libregf_named_key_free(
		 &named_key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the sub key descriptor for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libregf_key_item_get_sub_key_descriptor_by_utf16_name(
     libregf_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libregf_hive_bins_list_t *hive_bins_list,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	libregf_key_descriptor_t *safe_sub_key_descriptor = NULL;
	libregf_named_key_t *named_key                    = NULL;
	static char *function                             = "libregf_key_item_get_sub_key_descriptor_by_utf16_name";
	int number_of_sub_key_descriptors                 = 0;
	int result                                        = 0;
	int sub_key_descriptor_index                      = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
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
	*sub_key_descriptor = NULL;

/* TODO think of caching and/or optimization
 * e.g. mapping sub key descriptors to name search tree?
 */
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors,
	     &number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from sub key descriptors array.",
		 function );

		goto on_error;
	}
	for( sub_key_descriptor_index = 0;
	     sub_key_descriptor_index < number_of_sub_key_descriptors;
	     sub_key_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     key_item->sub_key_descriptors,
		     sub_key_descriptor_index,
		     (intptr_t **) &safe_sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key descriptor: %d from array.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( safe_sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor: %d.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( libregf_named_key_initialize(
		     &named_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create named key.",
			 function );

			goto on_error;
		}
		if( libregf_key_item_read_named_key(
		     named_key,
		     file_io_handle,
		     hive_bins_list,
		     safe_sub_key_descriptor->key_offset,
		     safe_sub_key_descriptor->hash_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read named key at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 safe_sub_key_descriptor->key_offset,
			 safe_sub_key_descriptor->key_offset );

			goto on_error;
		}
		result = libregf_named_key_compare_name_with_utf16_string(
		          named_key,
		          name_hash,
		          utf16_string,
		          utf16_string_length,
		          hive_bins_list->io_handle->ascii_codepage,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare named key with UTF-16 string.",
			 function );

			goto on_error;
		}
		if( libregf_named_key_free(
		     &named_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free named key.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( sub_key_descriptor_index >= number_of_sub_key_descriptors )
	{
		return( 0 );
	}
	*sub_key_descriptor = safe_sub_key_descriptor;

	return( 1 );

on_error:
	if( named_key != NULL )
	{
		libregf_named_key_free(
		 &named_key,
		 NULL );
	}
	return( -1 );
}

