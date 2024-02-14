/*
 * Python object definition of the sequence and iterator object of values
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

#if !defined( _PYREGF_VALUES_H )
#define _PYREGF_VALUES_H

#include <common.h>
#include <types.h>

#include "pyregf_libregf.h"
#include "pyregf_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyregf_values pyregf_values_t;

struct pyregf_values
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyregf_values_type_object;

PyObject *pyregf_values_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyregf_values_init(
     pyregf_values_t *sequence_object );

void pyregf_values_free(
      pyregf_values_t *sequence_object );

Py_ssize_t pyregf_values_len(
            pyregf_values_t *sequence_object );

PyObject *pyregf_values_getitem(
           pyregf_values_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyregf_values_iter(
           pyregf_values_t *sequence_object );

PyObject *pyregf_values_iternext(
           pyregf_values_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYREGF_VALUES_H ) */

