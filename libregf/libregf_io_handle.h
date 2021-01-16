/*
 * Input/Output (IO) handle
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

#if !defined( _LIBREGF_IO_HANDLE_H )
#define _LIBREGF_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libregf_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *regf_file_signature;

typedef struct libregf_io_handle libregf_io_handle_t;

struct libregf_io_handle
{
	/* The major version
	 */
	uint32_t major_version;

	/* The minor version
	 */
	uint32_t minor_version;

	/* The file type
	 */
	uint32_t file_type;

	/* The hive bins list offset
	 */
	off64_t hive_bins_list_offset;

	/* Various flags
	 */
	uint8_t flags;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;
};

int libregf_io_handle_initialize(
     libregf_io_handle_t **io_handle,
     libcerror_error_t **error );

int libregf_io_handle_free(
     libregf_io_handle_t **io_handle,
     libcerror_error_t **error );

int libregf_io_handle_clear(
     libregf_io_handle_t *io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_IO_HANDLE_H ) */

