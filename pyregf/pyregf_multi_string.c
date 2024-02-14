/*
 * Python object definition of the sequence and iterator object of a multi string
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
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyregf_error.h"
#include "pyregf_libcerror.h"
#include "pyregf_libregf.h"
#include "pyregf_multi_string.h"
#include "pyregf_python.h"

PySequenceMethods pyregf_multi_string_sequence_methods = {
	/* sq_length */
	(lenfunc) pyregf_multi_string_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyregf_multi_string_getitem,
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

PyTypeObject pyregf_multi_string_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyregf.multi_string",
	/* tp_basicsize */
	sizeof( pyregf_multi_string_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyregf_multi_string_free,
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
	&pyregf_multi_string_sequence_methods,
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
	"pyregf sequence and iterator object of multi_string",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyregf_multi_string_iter,
	/* tp_iternext */
	(iternextfunc) pyregf_multi_string_iternext,
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
	(initproc) pyregf_multi_string_init,
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

/* Creates a new multi_string sequence and iterator object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyregf_multi_string_new(
           libregf_multi_string_t *multi_string,
           PyObject *parent_object )
{
	libcerror_error_t *error               = NULL;
	pyregf_multi_string_t *sequence_object = NULL;
	static char *function                  = "pyregf_multi_string_new";
	int result                             = 0;

	if( multi_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid multi string.",
		 function );

		return( NULL );
	}
	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	/* Make sure the multi_string multi_string are initialized
	 */
	sequence_object = PyObject_New(
	                   struct pyregf_multi_string,
	                   &pyregf_multi_string_type_object );

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		goto on_error;
	}
	sequence_object->multi_string    = multi_string;
	sequence_object->parent_object   = parent_object;
	sequence_object->current_index   = 0;
	sequence_object->number_of_items = 0;

	Py_IncRef(
	 (PyObject *) sequence_object->parent_object );

	Py_BEGIN_ALLOW_THREADS

	result = libregf_multi_string_get_number_of_strings(
	          sequence_object->multi_string,
	          &( sequence_object->number_of_items ),
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyregf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of strings.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	return( (PyObject *) sequence_object );

on_error:
	if( sequence_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) sequence_object );
	}
	return( NULL );
}

/* Initializes a multi_string sequence and iterator object
 * Returns 0 if successful or -1 on error
 */
int pyregf_multi_string_init(
     pyregf_multi_string_t *sequence_object )
{
	static char *function = "pyregf_multi_string_init";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( -1 );
	}
	/* Make sure the multi_string multi_string are initialized
	 */
	sequence_object->multi_string    = NULL;
	sequence_object->parent_object   = NULL;
	sequence_object->current_index   = 0;
	sequence_object->number_of_items = 0;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of multi string not supported.",
	 function );

	return( -1 );
}

/* Frees a multi_string sequence object
 */
void pyregf_multi_string_free(
      pyregf_multi_string_t *sequence_object )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyregf_multi_string_free";
	int result                  = 0;

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           sequence_object );

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
	if( sequence_object->multi_string != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libregf_multi_string_free(
		          &( sequence_object->multi_string ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyregf_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libregf multi string.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( sequence_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) sequence_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) sequence_object );
}

/* The multi_string len() function
 */
Py_ssize_t pyregf_multi_string_len(
            pyregf_multi_string_t *sequence_object )
{
	static char *function = "pyregf_multi_string_len";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) sequence_object->number_of_items );
}

/* The multi_string getitem() function
 */
PyObject *pyregf_multi_string_getitem(
           pyregf_multi_string_t *sequence_object,
           Py_ssize_t item_index )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	uint8_t *utf8_string     = NULL;
	static char *function    = "pyregf_multi_string_getitem";
	size_t utf8_string_size  = 0;
	int result               = 0;

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) sequence_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libregf_multi_string_get_utf8_string_size(
	          sequence_object->multi_string,
	          (int) item_index,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyregf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	utf8_string = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libregf_multi_string_get_utf8_string(
	          sequence_object->multi_string,
	          (int) item_index,
	          utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyregf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) utf8_string,
			 (Py_ssize_t) utf8_string_size - 1,
			 NULL );

	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* The multi_string iter() function
 */
PyObject *pyregf_multi_string_iter(
           pyregf_multi_string_t *sequence_object )
{
	static char *function = "pyregf_multi_string_iter";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) sequence_object );

	return( (PyObject *) sequence_object );
}

/* The multi_string iternext() function
 */
PyObject *pyregf_multi_string_iternext(
           pyregf_multi_string_t *sequence_object )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	uint8_t *utf8_string     = NULL;
	static char *function    = "pyregf_multi_string_iternext";
	size_t utf8_string_size  = 0;
	int result               = 0;

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	if( sequence_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - invalid current index.",
		 function );

		return( NULL );
	}
	if( sequence_object->current_index >= sequence_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libregf_multi_string_get_utf8_string_size(
	          sequence_object->multi_string,
	          sequence_object->current_index,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyregf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	utf8_string = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libregf_multi_string_get_utf8_string(
	          sequence_object->multi_string,
	          sequence_object->current_index,
	          utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyregf_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) utf8_string,
			 (Py_ssize_t) utf8_string_size - 1,
			 NULL );

	PyMem_Free(
	 utf8_string );

	if( string_object != NULL )
	{
		sequence_object->current_index++;
	}
	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

