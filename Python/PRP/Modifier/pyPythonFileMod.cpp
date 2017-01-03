/* This file is part of HSPlasma.
 *
 * HSPlasma is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * HSPlasma is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HSPlasma.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pyPythonFileMod.h"

#include <PRP/Modifier/plPythonFileMod.h>
#include "pyModifier.h"
#include "PRP/KeyedObject/pyKey.h"
#include "PRP/pyCreatable.h"

extern "C" {

PY_PLASMA_NEW(PythonFileMod, plPythonFileMod)

PY_METHOD_NOARGS(PythonFileMod, clearReceivers,
    "Remove all receivers from the Python File Mod")
{
    self->fThis->clearReceivers();
    Py_RETURN_NONE;
}

PY_METHOD_NOARGS(PythonFileMod, clearParameters,
    "Remove all parameters from the Python File Mod")
{
    self->fThis->clearParameters();
    Py_RETURN_NONE;
}

PY_METHOD_VA(PythonFileMod, addReceiver,
    "Params: key\n"
    "Add a receiver to the Python File Mod")
{
    pyKey* key;
    if (!PyArg_ParseTuple(args, "O", &key)) {
        PyErr_SetString(PyExc_TypeError, "addReceiver expects a plKey");
        return NULL;
    }
    if (!pyKey_Check((PyObject*)key)) {
        PyErr_SetString(PyExc_TypeError, "addReceiver expects a plKey");
        return NULL;
    }
    self->fThis->addReceiver(*key->fThis);
    Py_RETURN_NONE;
}

PY_METHOD_VA(PythonFileMod, addParameter,
    "Params: parameter\n"
    "Add a parameter to the Python File Mod")
{
    pyPythonParameter* param;
    if (!PyArg_ParseTuple(args, "O", &param)) {
        PyErr_SetString(PyExc_TypeError, "addParameter expects a plPythonParameter");
        return NULL;
    }
    if (!pyPythonParameter_Check((PyObject*)param)) {
        PyErr_SetString(PyExc_TypeError, "addParameter expects a plPythonParameter");
        return NULL;
    }
    self->fThis->addParameter(*param->fThis);
    Py_RETURN_NONE;
}

static PyObject* pyPythonFileMod_getReceivers(pyPythonFileMod* self, void*) {
    PyObject* list = PyList_New(self->fThis->getReceivers().size());
    for (size_t i=0; i<self->fThis->getReceivers().size(); i++)
        PyList_SET_ITEM(list, i, pyKey_FromKey(self->fThis->getReceivers()[i]));
    return list;
}

static PyObject* pyPythonFileMod_getParameters(pyPythonFileMod* self, void*) {
    PyObject* list = PyList_New(self->fThis->getParameters().size());
    for (size_t i=0; i<self->fThis->getParameters().size(); i++)
        PyList_SET_ITEM(list, i, pyPythonParameter_FromPythonParameter(self->fThis->getParameters()[i]));
    return list;
}

static int pyPythonFileMod_setReceivers(pyPythonFileMod* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_RuntimeError, "to add receivers, use addReceiver");
    return -1;
}

static int pyPythonFileMod_setParameters(pyPythonFileMod* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_RuntimeError, "to add parameters, use addParameter");
    return -1;
}

static PyMethodDef pyPythonFileMod_Methods[] = {
    pyPythonFileMod_clearReceivers_method,
    pyPythonFileMod_clearParameters_method,
    pyPythonFileMod_addReceiver_method,
    pyPythonFileMod_addParameter_method,
    PY_METHOD_TERMINATOR
};

PY_PROPERTY(plString, PythonFileMod, filename, getFilename, setFilename)

static PyGetSetDef pyPythonFileMod_GetSet[] = {
    pyPythonFileMod_filename_getset,
    { _pycs("receivers"), (getter)pyPythonFileMod_getReceivers,
        (setter)pyPythonFileMod_setReceivers, NULL, NULL },
    { _pycs("parameters"), (getter)pyPythonFileMod_getParameters,
        (setter)pyPythonFileMod_setParameters, NULL, NULL },
    PY_GETSET_TERMINATOR
};

PyTypeObject pyPythonFileMod_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyHSPlasma.plPythonFileMod",       /* tp_name */
    sizeof(pyPythonFileMod),            /* tp_basicsize */
    0,                                  /* tp_itemsize */

    NULL,                               /* tp_dealloc */
    NULL,                               /* tp_print */
    NULL,                               /* tp_getattr */
    NULL,                               /* tp_setattr */
    NULL,                               /* tp_compare */
    NULL,                               /* tp_repr */
    NULL,                               /* tp_as_number */
    NULL,                               /* tp_as_sequence */
    NULL,                               /* tp_as_mapping */
    NULL,                               /* tp_hash */
    NULL,                               /* tp_call */
    NULL,                               /* tp_str */
    NULL,                               /* tp_getattro */
    NULL,                               /* tp_setattro */
    NULL,                               /* tp_as_buffer */

    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "plPythonFileMod wrapper",          /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyPythonFileMod_Methods,            /* tp_methods */
    NULL,                               /* tp_members */
    pyPythonFileMod_GetSet,             /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyPythonFileMod_new,                /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */

    NULL,                               /* tp_del */
    TP_VERSION_TAG_INIT                 /* tp_version_tag */
    TP_FINALIZE_INIT                    /* tp_finalize */
};

PyObject* Init_pyPythonFileMod_Type() {
    pyPythonFileMod_Type.tp_base = &pyMultiModifier_Type;
    if (PyType_Ready(&pyPythonFileMod_Type) < 0)
        return NULL;

    Py_INCREF(&pyPythonFileMod_Type);
    return (PyObject*)&pyPythonFileMod_Type;
}

PY_PLASMA_IFC_METHODS(PythonFileMod, plPythonFileMod)

}
