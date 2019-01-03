/*
 * Python object definition of the sequence and iterator object of keys
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

#include "pyregf_key.h"
#include "pyregf_keys.h"
#include "pyregf_libcerror.h"
#include "pyregf_libregf.h"
#include "pyregf_python.h"

PySequenceMethods pyregf_keys_sequence_methods = {
	/* sq_length */
	(lenfunc) pyregf_keys_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyregf_keys_getitem,
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

PyTypeObject pyregf_keys_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyregf._keys",
	/* tp_basicsize */
	sizeof( pyregf_keys_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyregf_keys_free,
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
	&pyregf_keys_sequence_methods,
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
	"pyregf internal sequence and iterator object of keys",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyregf_keys_iter,
	/* tp_iternext */
	(iternextfunc) pyregf_keys_iternext,
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
	(initproc) pyregf_keys_init,
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

/* Creates a new keys object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyregf_keys_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pyregf_keys_t *keys_object = NULL;
	static char *function      = "pyregf_keys_new";

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
	/* Make sure the keys values are initialized
	 */
	keys_object = PyObject_New(
	               struct pyregf_keys,
	               &pyregf_keys_type_object );

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create keys object.",
		 function );

		goto on_error;
	}
	if( pyregf_keys_init(
	     keys_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize keys object.",
		 function );

		goto on_error;
	}
	keys_object->parent_object     = parent_object;
	keys_object->get_item_by_index = get_item_by_index;
	keys_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) keys_object->parent_object );

	return( (PyObject *) keys_object );

on_error:
	if( keys_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) keys_object );
	}
	return( NULL );
}

/* Intializes a keys object
 * Returns 0 if successful or -1 on error
 */
int pyregf_keys_init(
     pyregf_keys_t *keys_object )
{
	static char *function = "pyregf_keys_init";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return( -1 );
	}
	/* Make sure the keys values are initialized
	 */
	keys_object->parent_object     = NULL;
	keys_object->get_item_by_index = NULL;
	keys_object->current_index     = 0;
	keys_object->number_of_items   = 0;

	return( 0 );
}

/* Frees a keys object
 */
void pyregf_keys_free(
      pyregf_keys_t *keys_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyregf_keys_free";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           keys_object );

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
	if( keys_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) keys_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) keys_object );
}

/* The keys len() function
 */
Py_ssize_t pyregf_keys_len(
            pyregf_keys_t *keys_object )
{
	static char *function = "pyregf_keys_len";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) keys_object->number_of_items );
}

/* The keys getitem() function
 */
PyObject *pyregf_keys_getitem(
           pyregf_keys_t *keys_object,
           Py_ssize_t item_index )
{
	PyObject *key_object  = NULL;
	static char *function = "pyregf_keys_getitem";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return( NULL );
	}
	if( keys_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( keys_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) keys_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	key_object = keys_object->get_item_by_index(
	              keys_object->parent_object,
	              (int) item_index );

	return( key_object );
}

/* The keys iter() function
 */
PyObject *pyregf_keys_iter(
           pyregf_keys_t *keys_object )
{
	static char *function = "pyregf_keys_iter";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) keys_object );

	return( (PyObject *) keys_object );
}

/* The keys iternext() function
 */
PyObject *pyregf_keys_iternext(
           pyregf_keys_t *keys_object )
{
	PyObject *key_object  = NULL;
	static char *function = "pyregf_keys_iternext";

	if( keys_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object.",
		 function );

		return( NULL );
	}
	if( keys_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( keys_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object - invalid current index.",
		 function );

		return( NULL );
	}
	if( keys_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( keys_object->current_index >= keys_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	key_object = keys_object->get_item_by_index(
	              keys_object->parent_object,
	              keys_object->current_index );

	if( key_object != NULL )
	{
		keys_object->current_index++;
	}
	return( key_object );
}

