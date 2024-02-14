/*
 * Hive Bin functions
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
#include <memory.h>
#include <types.h>

#include "libregf_debug.h"
#include "libregf_definitions.h"
#include "libregf_hive_bin.h"
#include "libregf_hive_bin_cell.h"
#include "libregf_hive_bin_header.h"
#include "libregf_libbfio.h"
#include "libregf_libcdata.h"
#include "libregf_libcerror.h"
#include "libregf_libcnotify.h"
#include "libregf_libfdatetime.h"

#include "regf_hive_bin.h"

/* Creates a hive bin
 * Make sure the value hive_bin is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_initialize(
     libregf_hive_bin_t **hive_bin,
     uint32_t offset,
     uint32_t size,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_initialize";

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( *hive_bin != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid hive bin value already set.",
		 function );

		return( -1 );
	}
	*hive_bin = memory_allocate_structure(
	             libregf_hive_bin_t );

	if( *hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create hive bin.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *hive_bin,
	     0,
	     sizeof( libregf_hive_bin_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear hive bin.",
		 function );

		memory_free(
		 *hive_bin );

		*hive_bin = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *hive_bin )->cells_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create cells array.",
		 function );

		goto on_error;
	}
	( *hive_bin )->offset = offset;
	( *hive_bin )->size   = size;

	return( 1 );

on_error:
	if( *hive_bin != NULL )
	{
		memory_free(
		 *hive_bin );

		*hive_bin = NULL;
	}
	return( -1 );
}

/* Frees a hive bin
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_free(
     libregf_hive_bin_t **hive_bin,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_free";
	int result            = 1;

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( *hive_bin != NULL )
	{
		if( libcdata_array_free(
		     &( ( *hive_bin )->cells_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libregf_hive_bin_cell_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the hive bin cells array.",
			 function );

			result = -1;
		}
		if( ( *hive_bin )->data != NULL )
		{
			memory_free(
			 ( *hive_bin )->data );
		}
		memory_free(
		 *hive_bin );

		*hive_bin = NULL;
	}
	return( result );
}

/* Reads a hive bin and determines its cells
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_read_cells_data(
     libregf_hive_bin_t *hive_bin,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libregf_hive_bin_cell_t *hive_bin_cell = NULL;
	static char *function                  = "libregf_hive_bin_read_cells_data";
	size_t data_offset                     = 0;
	uint32_t cell_size                     = 0;
	uint32_t file_offset                   = 0;
	uint8_t flags                          = 0;
	int cell_index                         = 0;
	int entry_index                        = 0;

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
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
	if( ( data_size < 4 )
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
		 "%s: hive bin cells:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
/* TODO empty cells array */
	file_offset = hive_bin->offset + sizeof( regf_hive_bin_header_t );

	while( data_offset < data_size )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: hive bin cell: %03d offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 cell_index,
			 file_offset );
		}
#endif
		if( data_offset >= ( data_size - 4 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cell size value exceeds hive bin size.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint32_little_endian(
		 &( data[ data_offset ] ),
		 cell_size );

		data_offset += 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: hive bin cell: %03d size\t\t: 0x%08" PRIx32 " (%" PRIi32 ")\n",
			 function,
			 cell_index,
			 cell_size,
			 (int32_t) cell_size );
		}
#endif
		flags = 0;

		if( cell_size == 0x80000000UL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cell size value out of bounds.",
			 function );

			goto on_error;
		}
		if( (int32_t) cell_size < 0 )
		{
			cell_size = (uint32_t) ( -1 * (int32_t) cell_size );
		}
		else
		{
			flags |= LIBREGF_HIVE_BIN_CELL_FLAG_UNALLOCATED;
		}
		if( cell_size < 4 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cell size value out of bounds.",
			 function );

			goto on_error;
		}
		if( ( cell_size % 8 ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cell size value should be multitude of 8.",
			 function );

			goto on_error;
		}
		/* Remove the size of the cell size value
		 */
		cell_size -= 4;

		if( cell_size > ( data_size - data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid cell size value exceeds hive bin size.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: hive bin cell: %03d data:\n",
			 function,
			 cell_index );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 cell_size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
		if( libregf_hive_bin_cell_initialize(
		     &hive_bin_cell,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create hive bin cell.",
			 function );

			goto on_error;
		}
		hive_bin_cell->offset = (uint32_t) file_offset;
		hive_bin_cell->data   = &( data[ data_offset ] );
		hive_bin_cell->size   = cell_size;
		hive_bin_cell->flags |= flags;

		data_offset += cell_size;
		file_offset += 4 + cell_size;

		if( libcdata_array_append_entry(
		     hive_bin->cells_array,
		     &entry_index,
		     (intptr_t *) hive_bin_cell,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append hive bin cell: %d.",
			 function,
			 cell_index );

			goto on_error;
		}
		hive_bin_cell = NULL;

		cell_index++;
	}
/* TODO check cell offset == bin size */

	return( 1 );

on_error:
	if( hive_bin_cell != NULL )
	{
		libregf_hive_bin_cell_free(
		 &hive_bin_cell,
		 NULL );
	}
/* TODO empty cells array */
	return( -1 );
}

/* Reads a hive bin and determines its cells
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_read_cells_file_io_handle(
     libregf_hive_bin_t *hive_bin,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_read_cells_file_io_handle";
	ssize_t read_count    = 0;

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( hive_bin->cells_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bin - missing cells array.",
		 function );

		return( -1 );
	}
	if( hive_bin->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid hive bin - data already set.",
		 function );

		return( -1 );
	}
	if( ( hive_bin->size == 0 )
	 || ( hive_bin->size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid hive bin - size value out of bounds.",
		 function );

		return( -1 );
	}
	hive_bin->data_size = (size_t) hive_bin->size - sizeof( regf_hive_bin_header_t );

	hive_bin->data = (uint8_t *) memory_allocate(
	                              sizeof( uint8_t ) * hive_bin->data_size );
	
	if( hive_bin->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create hive cells data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              hive_bin->data,
	              hive_bin->data_size,
	              error );

	if( read_count != (ssize_t) hive_bin->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hive bin cells data.",
		 function );

		goto on_error;
	}
	if( libregf_hive_bin_read_cells_data(
	     hive_bin,
	     hive_bin->data,
	     hive_bin->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read hive bin cells.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( hive_bin->data != NULL )
	{
		memory_free(
		 hive_bin->data );

		hive_bin->data = NULL;
	}
	hive_bin->data_size = 0;

	return( -1 );
}

/* Retrieves the number of hive bin cells
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_get_number_of_cells(
     libregf_hive_bin_t *hive_bin,
     uint16_t *number_of_cells,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_get_number_of_cells";
	int number_of_entries = 0;

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( number_of_cells == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of cells.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     hive_bin->cells_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	if( number_of_entries > (int) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of entries value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_cells = (uint16_t) number_of_entries;

	return( 1 );
}

/* Retrieves the hive bin cell at the index
 * Returns 1 if successful or -1 on error
 */
int libregf_hive_bin_get_cell(
     libregf_hive_bin_t *hive_bin,
     uint16_t cell_index,
     libregf_hive_bin_cell_t **hive_bin_cell,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_get_cell";

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     hive_bin->cells_array,
	     (int) cell_index,
	     (intptr_t **) hive_bin_cell,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve hive bin cell: %" PRIu16 ".",
		 function,
		 cell_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the hive bin cell at the offset
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libregf_hive_bin_get_cell_at_offset(
     libregf_hive_bin_t *hive_bin,
     uint32_t cell_offset,
     libregf_hive_bin_cell_t **hive_bin_cell,
     libcerror_error_t **error )
{
	static char *function = "libregf_hive_bin_get_cell_at_offset";
	int entry_index       = 0;
	int number_of_entries = 0;

	if( hive_bin == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin.",
		 function );

		return( -1 );
	}
	if( hive_bin->cells_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid hive bin - missing cells array.",
		 function );

		return( -1 );
	}
	if( hive_bin_cell == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid hive bin cell.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     hive_bin->cells_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve the number of hive bin cells.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     hive_bin->cells_array,
		     entry_index,
		     (intptr_t **) hive_bin_cell,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve hive bin cell: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( ( *hive_bin_cell )->offset == cell_offset )
		{
			return( 1 );
		}
	}
	*hive_bin_cell = NULL;

	return( 0 );
}

