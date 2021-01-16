/*
 * File functions
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

#if !defined( _LIBREGF_FILE_H )
#define _LIBREGF_FILE_H

#include <common.h>
#include <types.h>

#include "libregf_extern.h"
#include "libregf_dirty_vector.h"
#include "libregf_file_header.h"
#include "libregf_hive_bins_list.h"
#include "libregf_io_handle.h"
#include "libregf_libbfio.h"
#include "libregf_libcerror.h"
#include "libregf_libcthreads.h"
#include "libregf_libfcache.h"
#include "libregf_libfdata.h"
#include "libregf_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libregf_internal_file libregf_internal_file_t;

struct libregf_internal_file
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libregf_io_handle_t *io_handle;

	/* The file header
	 */
	libregf_file_header_t *file_header;

	/* The dirty vector
	 */
	libregf_dirty_vector_t *dirty_vector;

	/* The hive bins list
	 */
	libregf_hive_bins_list_t *hive_bins_list;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* Value to indicate if abort was signalled
	 */
	int abort;

#if defined( HAVE_LIBREGF_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBREGF_EXTERN \
int libregf_file_initialize(
     libregf_file_t **file,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_free(
     libregf_file_t **file,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_signal_abort(
     libregf_file_t *file,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_open(
     libregf_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBREGF_EXTERN \
int libregf_file_open_wide(
     libregf_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBREGF_EXTERN \
int libregf_file_open_file_io_handle(
     libregf_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_close(
     libregf_file_t *file,
     libcerror_error_t **error );

int libregf_internal_file_open_read(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libregf_internal_file_read_hive_bins(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libregf_internal_file_read_dirty_vector(
     libregf_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_is_corrupted(
     libregf_file_t *file,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_ascii_codepage(
     libregf_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_set_ascii_codepage(
     libregf_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_format_version(
     libregf_file_t *file,
     uint32_t *major_version,
     uint32_t *minor_version,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_type(
     libregf_file_t *file,
     uint32_t *file_type,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_root_key(
     libregf_file_t *file,
     libregf_key_t **key,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_key_by_utf8_path(
     libregf_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libregf_key_t **key,
     libcerror_error_t **error );

LIBREGF_EXTERN \
int libregf_file_get_key_by_utf16_path(
     libregf_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libregf_key_t **key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBREGF_FILE_H ) */

