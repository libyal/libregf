/*
 * Python object definition of the sequence and iterator object of values
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyregf_libcerror.h"
#include "pyregf_libregf.h"
#include "pyregf_python.h"
#include "pyregf_value.h"
#include "pyregf_values.h"

PySequenceMethods pyregf_values_sequence_methods = {
	/* sq_length */
	(lenfunc) pyregf_values_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyregf_values_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyregf_values_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyregf._values",
	/* tp_basicsize */
	sizeof( pyregf_values_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyregf_values_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pyregf_values_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"pyregf internal sequence and iterator object of values",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyregf_values_iter,
	/* tp_iternext */
	(iternextfunc) pyregf_values_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyregf_values_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new values object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyregf_values_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pyregf_values_t *values_object = NULL;
	static char *function          = "pyregf_values_new";

	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	if( get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get item by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the values values are initialized
	 */
	values_object = PyObject_New(
	                 struct pyregf_values,
	                 &pyregf_values_type_object );

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create values object.",
		 function );

		goto on_error;
	}
	if( pyregf_values_init(
	     values_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize values object.",
		 function );

		goto on_error;
	}
	values_object->parent_object     = parent_object;
	values_object->get_item_by_index = get_item_by_index;
	values_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) values_object->parent_object );

	return( (PyObject *) values_object );

on_error:
	if( values_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) values_object );
	}
	return( NULL );
}

/* Intializes a values object
 * Returns 0 if successful or -1 on error
 */
int pyregf_values_init(
     pyregf_values_t *values_object )
{
	static char *function = "pyregf_values_init";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return( -1 );
	}
	/* Make sure the values values are initialized
	 */
	values_object->parent_object     = NULL;
	values_object->get_item_by_index = NULL;
	values_object->current_index     = 0;
	values_object->number_of_items   = 0;

	return( 0 );
}

/* Frees a values object
 */
void pyregf_values_free(
      pyregf_values_t *values_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyregf_values_free";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           values_object );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( values_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) values_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) values_object );
}

/* The values len() function
 */
Py_ssize_t pyregf_values_len(
            pyregf_values_t *values_object )
{
	static char *function = "pyregf_values_len";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) values_object->number_of_items );
}

/* The values getitem() function
 */
PyObject *pyregf_values_getitem(
           pyregf_values_t *values_object,
           Py_ssize_t item_index )
{
	PyObject *value_object = NULL;
	static char *function  = "pyregf_values_getitem";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return( NULL );
	}
	if( values_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( values_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) values_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	value_object = values_object->get_item_by_index(
	                values_object->parent_object,
	                (int) item_index );

	return( value_object );
}

/* The values iter() function
 */
PyObject *pyregf_values_iter(
           pyregf_values_t *values_object )
{
	static char *function = "pyregf_values_iter";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) values_object );

	return( (PyObject *) values_object );
}

/* The values iternext() function
 */
PyObject *pyregf_values_iternext(
           pyregf_values_t *values_object )
{
	PyObject *value_object = NULL;
	static char *function  = "pyregf_values_iternext";

	if( values_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object.",
		 function );

		return( NULL );
	}
	if( values_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( values_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object - invalid current index.",
		 function );

		return( NULL );
	}
	if( values_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( values_object->current_index >= values_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	value_object = values_object->get_item_by_index(
	                values_object->parent_object,
	                values_object->current_index );

	if( value_object != NULL )
	{
		values_object->current_index++;
	}
	return( value_object );
}

