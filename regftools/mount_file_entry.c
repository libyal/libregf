/*
 * Mount file entry
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_SYS_STAT_H )
#include <sys/stat.h>
#endif

#include "mount_file_entry.h"
#include "mount_file_system.h"
#include "regftools_libcerror.h"
#include "regftools_libregf.h"

#if !defined( S_IFDIR )
#define S_IFDIR 0x4000
#endif

#if !defined( S_IFREG )
#define S_IFREG 0x8000
#endif

/* Creates a file entry
 * Make sure the value file_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_initialize(
     mount_file_entry_t **file_entry,
     mount_file_system_t *file_system,
     const system_character_t *name,
     size_t name_length,
     int type,
     libregf_key_t *regf_key,
     libregf_value_t *regf_value,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_initialize";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file entry value already set.",
		 function );

		return( -1 );
	}
	if( file_system == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file system.",
		 function );

		return( -1 );
	}
	if( name_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name length value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( type != MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
	 && ( type != MOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( type != MOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( type != MOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported type.",
		 function );

		return( -1 );
	}
	*file_entry = memory_allocate_structure(
	               mount_file_entry_t );

	if( *file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_entry,
	     0,
	     sizeof( mount_file_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file entry.",
		 function );

		memory_free(
		 *file_entry );

		*file_entry = NULL;

		return( -1 );
	}
	( *file_entry )->file_system = file_system;

	if( name != NULL )
	{
		( *file_entry )->name = system_string_allocate(
		                         name_length + 1 );

		if( ( *file_entry )->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name string.",
			 function );

			goto on_error;
		}
		if( name_length > 0 )
		{
			if( system_string_copy(
			     ( *file_entry )->name,
			     name,
			     name_length ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy name.",
				 function );

				goto on_error;
			}
		}
		( *file_entry )->name[ name_length ] = 0;

		( *file_entry )->name_size = name_length + 1;
	}
	( *file_entry )->type       = type;
	( *file_entry )->regf_key   = regf_key;
	( *file_entry )->regf_value = regf_value;

	return( 1 );

on_error:
	if( *file_entry != NULL )
	{
		if( ( *file_entry )->name != NULL )
		{
			memory_free(
			 ( *file_entry )->name );
		}
		memory_free(
		 *file_entry );

		*file_entry = NULL;
	}
	return( -1 );
}

/* Frees a file entry
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_free(
     mount_file_entry_t **file_entry,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_free";
	int result            = 1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( *file_entry != NULL )
	{
		if( ( *file_entry )->name != NULL )
		{
			memory_free(
			 ( *file_entry )->name );
		}
		if( ( *file_entry )->regf_key != NULL )
		{
			if( ( ( *file_entry )->type != MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
			 && ( ( *file_entry )->type != MOUNT_FILE_ENTRY_TYPE_VALUE ) )
			{
				if( libregf_key_free(
				     &( ( *file_entry )->regf_key ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free key.",
					 function );

					result = -1;
				}
			}
		}
		if( ( *file_entry )->regf_value != NULL )
		{
			if( libregf_value_free(
			     &( ( *file_entry )->regf_value ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -1;
			}
		}
		if( ( *file_entry )->value_data != NULL )
		{
			memory_free(
			 ( *file_entry )->value_data );
		}
		memory_free(
		 *file_entry );

		*file_entry = NULL;
	}
	return( result );
}

/* Retrieves the parent file entry
 * Returns 1 if successful, 0 if no such file entry or -1 on error
 */
int mount_file_entry_get_parent_file_entry(
     mount_file_entry_t *file_entry,
     mount_file_entry_t **parent_file_entry,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_parent_file_entry";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( parent_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid parent file entry.",
		 function );

		return( -1 );
	}
	if( *parent_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid parent file entry value already set.",
		 function );

		return( -1 );
	}
/* TODO implement get_parent function */

	return( 1 );
}

/* Retrieves the creation date and time
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_creation_time(
     mount_file_entry_t *file_entry,
     uint64_t *creation_time,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_creation_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_mounted_timestamp(
	     file_entry->file_system,
	     creation_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve mounted timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the access date and time
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_access_time(
     mount_file_entry_t *file_entry,
     uint64_t *access_time,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_access_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_mounted_timestamp(
	     file_entry->file_system,
	     access_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve mounted timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the modification date and time
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_modification_time(
     mount_file_entry_t *file_entry,
     uint64_t *modification_time,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_modification_time";
	uint64_t filetime     = 0;

#if !defined( WINAPI )
	int64_t posix_time    = 0;
#endif

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( file_entry->regf_key == NULL )
	{
		if( mount_file_system_get_mounted_timestamp(
		     file_entry->file_system,
		     modification_time,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve mounted timestamp.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( modification_time == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid modification time.",
			 function );

			return( -1 );
		}
		if( libregf_key_get_last_written_time(
		     file_entry->regf_key,
		     &filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve last written time from key.",
			 function );

			return( -1 );
		}
#if defined( WINAPI )
		*modification_time = filetime;
#else
		if( filetime != 0 )
		{
			/* Convert the FILETIME timestamp into a POSIX nanoseconds timestamp
			 */
			posix_time = ( (int64_t) filetime - 116444736000000000L ) * 100;
		}
		*modification_time = (uint64_t) posix_time;
#endif
	}
	return( 1 );
}

/* Retrieves the inode change date and time
 * On Windows the timestamp is an unsigned 64-bit FILETIME timestamp
 * otherwise the timestamp is a signed 64-bit POSIX date and time value in number of nanoseconds
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_inode_change_time(
     mount_file_entry_t *file_entry,
     uint64_t *inode_change_time,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_inode_change_time";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( mount_file_system_get_mounted_timestamp(
	     file_entry->file_system,
	     inode_change_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve mounted timestamp.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the file mode
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_file_mode(
     mount_file_entry_t *file_entry,
     uint16_t *file_mode,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_file_mode";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( file_mode == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file mode.",
		 function );

		return( -1 );
	}
	if( ( file_entry->type == MOUNT_FILE_ENTRY_TYPE_KEY )
	 || ( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		*file_mode = S_IFDIR | 0555;
	}
	else
	{
		*file_mode = S_IFREG | 0444;
	}
	return( 1 );
}

/* Retrieves the size of the name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_name_size(
     mount_file_entry_t *file_entry,
     size_t *string_size,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_name_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	*string_size = file_entry->name_size;

	return( 1 );
}

/* Retrieves the name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_name(
     mount_file_entry_t *file_entry,
     system_character_t *string,
     size_t string_size,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_name";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( file_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file entry - missing name.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string_size < file_entry->name_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid string size value too small.",
		 function );

		return( -1 );
	}
	if( system_string_copy(
	     string,
	     file_entry->name,
	     file_entry->name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		return( -1 );
	}
	string[ file_entry->name_size - 1 ] = 0;

	return( 1 );
}

/* Retrieves the number of sub file entries
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_number_of_sub_file_entries(
     mount_file_entry_t *file_entry,
     int *number_of_sub_file_entries,
     libcerror_error_t **error )
{
	static char *function  = "mount_file_entry_get_number_of_sub_file_entries";
	size_t class_name_size = 0;
	int number_of_values   = 0;
	int number_of_sub_keys = 0;
	int result             = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( number_of_sub_file_entries == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of sub file entries.",
		 function );

		return( -1 );
	}
	if( ( file_entry->type == MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
	 || ( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUE ) )
	{
		*number_of_sub_file_entries = 0;

		return( 1 );
	}
	if( libregf_key_get_number_of_values(
	     file_entry->regf_key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		*number_of_sub_file_entries = number_of_values;

		return( 1 );
	}
	if( libregf_key_get_number_of_sub_keys(
	     file_entry->regf_key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		return( -1 );
	}
	if( number_of_values > 0 )
	{
		number_of_sub_keys += 1;
	}
	result = libregf_key_get_class_name_size(
	          file_entry->regf_key,
	          &class_name_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve class name size.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		number_of_sub_keys += 1;
	}
	*number_of_sub_file_entries = number_of_sub_keys;

	return( 1 );
}

/* Retrieves the sub file entry for the specific index
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_sub_file_entry_by_index(
     mount_file_entry_t *file_entry,
     int sub_file_entry_index,
     mount_file_entry_t **sub_file_entry,
     libcerror_error_t **error )
{
	libregf_key_t *sub_regf_key                  = NULL;
	libregf_value_t *regf_value                  = NULL;
	const system_character_t *classname_filename = _SYSTEM_STRING( "(classname)" );
	system_character_t *filename                 = NULL;
	const system_character_t *values_filename    = _SYSTEM_STRING( "(values)" );
	static char *function                        = "mount_file_entry_get_sub_file_entry_by_index";
	size_t class_name_size                       = 0;
	size_t filename_size                         = 0;
	int classname_sub_file_entry_index           = -1;
	int number_of_sub_keys                       = 0;
	int number_of_values                         = 0;
	int result                                   = 0;
	int sub_file_entry_type                      = MOUNT_FILE_ENTRY_TYPE_UNKNOWN;
	int values_sub_file_entry_index              = -1;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( sub_file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub file entry.",
		 function );

		return( -1 );
	}
	if( *sub_file_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid sub file entry value already set.",
		 function );

		return( -1 );
	}
	if( sub_file_entry_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub file entry index value out of bounds.",
		 function );

		return( -1 );
	}
	if( ( file_entry->type != MOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry->type != MOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported type.",
		 function );

		return( -1 );
	}
	if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUES )
	{
/* TODO refactor to libregf_key_get_sub_value_by_index */
		if( libregf_key_get_value(
		     file_entry->regf_key,
		     sub_file_entry_index,
		     &regf_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		if( mount_file_system_get_filename_from_value(
		     file_entry->file_system,
		     regf_value,
		     &filename,
		     &filename_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve filename of sub file entry: %d.",
			 function,
			 sub_file_entry_index );

			goto on_error;
		}
		sub_file_entry_type = MOUNT_FILE_ENTRY_TYPE_VALUE;
	}
	else
	{
		if( libregf_key_get_number_of_sub_keys(
		     file_entry->regf_key,
		     &number_of_sub_keys,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			goto on_error;
		}
		if( sub_file_entry_index < number_of_sub_keys )
		{
/* TODO refactor to libregf_key_get_sub_key_by_index */
			if( libregf_key_get_sub_key(
			     file_entry->regf_key,
			     sub_file_entry_index,
			     &sub_regf_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			if( mount_file_system_get_filename_from_key(
			     file_entry->file_system,
			     sub_regf_key,
			     &filename,
			     &filename_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve filename of sub file entry: %d.",
				 function,
				 sub_file_entry_index );

				goto on_error;
			}
			sub_file_entry_type = MOUNT_FILE_ENTRY_TYPE_KEY;
		}
		else
		{
			if( libregf_key_get_number_of_values(
			     file_entry->regf_key,
			     &number_of_values,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve number of values.",
				 function );

				goto on_error;
			}
			if( number_of_values > 0 )
			{
				values_sub_file_entry_index = number_of_sub_keys;
			}
			result = libregf_key_get_class_name_size(
			          file_entry->regf_key,
			          &class_name_size,
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve class name size.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				classname_sub_file_entry_index = number_of_sub_keys;

				if( number_of_values > 0 )
				{
					classname_sub_file_entry_index += 1;
				}
			}
			if( sub_file_entry_index == values_sub_file_entry_index )
			{
				filename            = (system_character_t *) values_filename;
				filename_size       = 9;
				sub_file_entry_type = MOUNT_FILE_ENTRY_TYPE_VALUES;
			}
			else if( sub_file_entry_index == classname_sub_file_entry_index )
			{
				filename            = (system_character_t *) classname_filename;
				filename_size       = 12;
				sub_file_entry_type = MOUNT_FILE_ENTRY_TYPE_CLASS_NAME;
			}
			else
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid sub file entry index value out of bounds.",
				 function );

				goto on_error;
			}
			sub_regf_key = file_entry->regf_key;
		}
	}
	if( mount_file_entry_initialize(
	     sub_file_entry,
	     file_entry->file_system,
	     filename,
	     filename_size - 1,
	     sub_file_entry_type,
	     sub_regf_key,
	     regf_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub file entry: %d.",
		 function,
		 sub_file_entry_index );

		goto on_error;
	}
	if( ( filename != NULL )
	 && ( filename != classname_filename )
	 && ( filename != values_filename ) )
	{
		memory_free(
		 filename );
	}
	return( 1 );

on_error:
	if( ( filename != NULL )
	 && ( filename != classname_filename )
	 && ( filename != values_filename ) )
	{
		memory_free(
		 filename );
	}
	if( regf_value != NULL )
	{
		libregf_value_free(
		 &regf_value,
		 NULL );
	}
	if( ( sub_regf_key != NULL )
	 && ( sub_regf_key != file_entry->regf_key ) )
	{
		libregf_key_free(
		 &sub_regf_key,
		 NULL );
	}
	return( -1 );
}

/* Reads data at a specific offset
 * Returns the number of bytes read or -1 on error
 */
ssize_t mount_file_entry_read_buffer_at_offset(
         mount_file_entry_t *file_entry,
         void *buffer,
         size_t buffer_size,
         off64_t offset,
         libcerror_error_t **error )
{
	static char *function = "mount_file_entry_read_buffer_at_offset";
	ssize_t read_count    = 0;

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( ( file_entry->type != MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
	 && ( file_entry->type != MOUNT_FILE_ENTRY_TYPE_VALUE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file entry - unsupported type.",
		 function );

		return( -1 );
	}
	if( buffer_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid buffer size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		return( -1 );
	}
	if( file_entry->value_data_size == 0 )
	{
		if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
		{
			if( libregf_key_get_class_name_size(
			     file_entry->regf_key,
			     &( file_entry->value_data_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve class name size.",
				 function );

				return( -1 );
			}
		}
		else if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUE )
		{
			if( libregf_value_get_value_data_size(
			     file_entry->regf_value,
			     &( file_entry->value_data_size ),
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
		}
	}
	if( file_entry->value_data_size == 0 )
	{
		return( 0 );
	}
	if( file_entry->value_data == NULL )
	{
		if( file_entry->value_data_size > (size_t) SSIZE_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid file entry - data size value exceeds maximum.",
			 function );

			return( -1 );
		}
		file_entry->value_data = (uint8_t *) memory_allocate(
		                                      file_entry->value_data_size );

		if( file_entry->value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			return( -1 );
		}
		if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
		{
			if( libregf_key_get_class_name(
			     file_entry->regf_key,
			     file_entry->value_data,
			     file_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve class name.",
				 function );

				memory_free(
				 file_entry->value_data );

				file_entry->value_data = NULL;

				return( -1 );
			}
		}
		else if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUE )
		{
			if( libregf_value_get_value_data(
			     file_entry->regf_value,
			     file_entry->value_data,
			     file_entry->value_data_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data.",
				 function );

				memory_free(
				 file_entry->value_data );

				file_entry->value_data = NULL;

				return( -1 );
			}
		}
	}
	if( offset < (off64_t) file_entry->value_data_size )
	{
		if( (off64_t) buffer_size > ( (off64_t) file_entry->value_data_size - offset ) )
		{
			read_count = (off64_t) file_entry->value_data_size - offset;
		}
		else
		{
			read_count = (size_t) buffer_size;
		}
		if( memory_copy(
		     buffer,
		     file_entry->value_data,
		     (size_t) read_count ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy value data.",
			 function );

			return( -1 );
		}
	}
	return( read_count );
}

/* Retrieves the size
 * Returns 1 if successful or -1 on error
 */
int mount_file_entry_get_size(
     mount_file_entry_t *file_entry,
     size64_t *size,
     libcerror_error_t **error )
{
	static char *function = "mount_file_entry_get_size";

	if( file_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file entry.",
		 function );

		return( -1 );
	}
	if( size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid size.",
		 function );

		return( -1 );
	}
	if( file_entry->value_data_size == 0 )
	{
		if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_CLASS_NAME )
		{
			if( libregf_key_get_class_name_size(
			     file_entry->regf_key,
			     &( file_entry->value_data_size ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve class name size.",
				 function );

				return( -1 );
			}
		}
		else if( file_entry->type == MOUNT_FILE_ENTRY_TYPE_VALUE )
		{
			if( libregf_value_get_value_data_size(
			     file_entry->regf_value,
			     &( file_entry->value_data_size ),
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
		}
	}
	*size = file_entry->value_data_size;

	return( 1 );
}

