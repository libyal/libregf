/*
 * Report handle
 *
 * Copyright (C) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _REPORT_HANDLE_H )
#define _REPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include "regftools_libcstring.h"
#include "regftools_libregf.h"
#include "registry_file.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct report_handle report_handle_t;

struct report_handle
{
	/* The input file
	 */
	registry_file_t *input_file;

	/* The base key
	 */
	libregf_key_t *base_key;

	/* The base key name
	 */
	libcstring_system_character_t *base_key_name;

	/* The base key name
	 */
	size_t base_key_name_size;

	/* The nofication output stream
	 */
	FILE *notify_stream;

	/* The ASCII codepage
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int report_handle_initialize(
     report_handle_t **report_handle,
     libcerror_error_t **error );

int report_handle_free(
     report_handle_t **report_handle,
     libcerror_error_t **error );

int report_handle_signal_abort(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_set_ascii_codepage(
     report_handle_t *report_handle,
     const libcstring_system_character_t *string,
     libcerror_error_t **error );

int report_handle_print_data(
     report_handle_t *report_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int report_handle_open_input(
     report_handle_t *report_handle,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error );

int report_handle_close_input(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_get_key_name(
     libregf_key_t *key,
     libcstring_system_character_t **name,
     size_t *name_size,
     libcerror_error_t **error );

int report_handle_get_value_name(
     libregf_value_t *value,
     libcstring_system_character_t **name,
     size_t *name_size,
     libcerror_error_t **error );

int report_handle_get_value_data(
     libregf_value_t *value,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int report_handle_get_value_data_string(
     libregf_value_t *value,
     libcstring_system_character_t **data_string,
     size_t *data_string_size,
     libcerror_error_t **error );

int report_handle_get_value_binary_data(
     libregf_value_t *value,
     uint8_t **binary_data,
     size_t *binary_data_size,
     libcerror_error_t **error );

int report_handle_analyze_key_with_posix_time_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcerror_error_t **error );

int report_handle_analyze_key_with_string_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcstring_system_character_t *value_name,
     size_t value_name_length,
     libcerror_error_t **error );

int report_handle_analyze_key_with_string_values(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_key_path_with_string_values(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error );

int report_handle_analyze_autorun(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_list_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_mrulist_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_mrulist_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_shell_item_list_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_list_mrulistex_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_list_mrulistex_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_string_and_shell_item_list_mrulistex_sub_keys_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcerror_error_t **error );

int report_handle_analyze_bagmru_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_bagmru_key_path(
     report_handle_t *report_handle,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libregf_error_t **error );

int report_handle_analyze_bagmru(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_mru_keys(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_explorer(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_sam_f_value(
     report_handle_t *report_handle,
     libregf_value_t *value,
     libcerror_error_t **error );

int report_handle_analyze_sam_users_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_sam(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_recent_docs_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_recent_docs_sub_key(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_recent_docs(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_product_info(
     report_handle_t *report_handle,
     libcerror_error_t **error );

int report_handle_analyze_digital_product_id_value(
     report_handle_t *report_handle,
     libregf_key_t *key,
     libcerror_error_t **error );

int report_handle_analyze_file(
     report_handle_t *report_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

