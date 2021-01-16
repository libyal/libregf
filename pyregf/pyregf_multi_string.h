/*
 * Python object definition of the sequence and iterator object of a multi string
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

#if !defined( _PYREGF_MULTI_STRING_H )
#define _PYREGF_MULTI_STRING_H

#include <common.h>
#include <types.h>

#include "pyregf_libregf.h"
#include "pyregf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyregf_multi_string pyregf_multi_string_t;

struct pyregf_multi_string
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libregf multi string
	 */
	libregf_multi_string_t *multi_string;

	/* The parent object
	 */
	PyObject *parent_object;

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyregf_multi_string_type_object;

PyObject *pyregf_multi_string_new(
           libregf_multi_string_t *multi_string,
           PyObject *parent_object );

int pyregf_multi_string_init(
     pyregf_multi_string_t *sequence_object );

void pyregf_multi_string_free(
      pyregf_multi_string_t *sequence_object );

Py_ssize_t pyregf_multi_string_len(
            pyregf_multi_string_t *sequence_object );

PyObject *pyregf_multi_string_getitem(
           pyregf_multi_string_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyregf_multi_string_iter(
           pyregf_multi_string_t *sequence_object );

PyObject *pyregf_multi_string_iternext(
           pyregf_multi_string_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYREGF_MULTI_STRING_H ) */

