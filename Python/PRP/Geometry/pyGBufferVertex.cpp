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

#include <PyPlasma.h>
#include <PRP/Geometry/plGBufferGroup.h>
#include "pyGBufferGroup.h"
#include "Math/pyGeometry3.h"

extern "C" {

static void pyGBufferVertex_dealloc(pyGBufferVertex* self) {
    delete self->fThis;
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static int pyGBufferVertex___init__(pyGBufferVertex* self, PyObject* args, PyObject* kwds) {
    if (!PyArg_ParseTuple(args, ""))
        return -1;
    return 0;
}

static PyObject* pyGBufferVertex_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyGBufferVertex* self = (pyGBufferVertex*)type->tp_alloc(type, 0);
    if (self != NULL)
        self->fThis = new plGBufferVertex();
    return (PyObject*)self;
}

static PyObject* pyGBufferVertex_getPos(pyGBufferVertex* self, void*) {
    return pyVector3_FromVector3(self->fThis->fPos);
}

static PyObject* pyGBufferVertex_getNormal(pyGBufferVertex* self, void*) {
    return pyVector3_FromVector3(self->fThis->fNormal);
}

static PyObject* pyGBufferVertex_getSkin(pyGBufferVertex* self, void*) {
    return PyInt_FromLong(self->fThis->fSkinIdx);
}

static PyObject* pyGBufferVertex_getColor(pyGBufferVertex* self, void*) {
    return PyInt_FromLong(self->fThis->fColor);
}

static PyObject* pyGBufferVertex_getWeights(pyGBufferVertex* self, void*) {
    PyObject* list = PyList_New(3);
    PyList_SET_ITEM(list, 0, PyFloat_FromDouble(self->fThis->fSkinWeights[0]));
    PyList_SET_ITEM(list, 1, PyFloat_FromDouble(self->fThis->fSkinWeights[1]));
    PyList_SET_ITEM(list, 2, PyFloat_FromDouble(self->fThis->fSkinWeights[2]));
    return list;
}

static PyObject* pyGBufferVertex_getUVWs(pyGBufferVertex* self, void*) {
    PyObject* list = PyList_New(10);
    for (size_t i=0; i<10; i++)
        PyList_SET_ITEM(list, i, pyVector3_FromVector3(self->fThis->fUVWs[i]));
    return list;
}

static int pyGBufferVertex_setPos(pyGBufferVertex* self, PyObject* value, void*) {
    if (value == NULL || !pyVector3_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "pos should be an hsVector3");
        return -1;
    }
    self->fThis->fPos = *((pyVector3*)value)->fThis;
    return 0;
}

static int pyGBufferVertex_setNormal(pyGBufferVertex* self, PyObject* value, void*) {
    if (value == NULL || !pyVector3_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "normal should be an hsVector3");
        return -1;
    }
    self->fThis->fNormal = *((pyVector3*)value)->fThis;
    return 0;
}

static int pyGBufferVertex_setSkin(pyGBufferVertex* self, PyObject* value, void*) {
    if (value == NULL || !PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "skinIdx should be an int");
        return -1;
    }
    self->fThis->fSkinIdx = PyInt_AsLong(value);
    return 0;
}

static int pyGBufferVertex_setColor(pyGBufferVertex* self, PyObject* value, void*) {
    if (value == NULL || !PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "color should be an int");
        return -1;
    }
    self->fThis->fColor = PyInt_AsLong(value);
    return 0;
}

static int pyGBufferVertex_setWeights(pyGBufferVertex* self, PyObject* value, void*) {
    if (value != NULL && !PyList_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "weights should be a list of up to 3 floats");
        return -1;
    }
    int size = (value == NULL) ? 0 : PyList_Size(value);
    if (size > 3) {
        PyErr_SetString(PyExc_RuntimeError, "weights should be a list of up to 3 floats");
        return -1;
    }
    for (int i=0; i<size; i++) {
        if (!PyFloat_Check(PyList_GetItem(value, i))) {
            PyErr_SetString(PyExc_TypeError, "weights should be a list of up to 3 floats");
            return -1;
        }
        self->fThis->fSkinWeights[i] = PyFloat_AsDouble(PyList_GetItem(value, i));
    }
    for (int i=size; i<3; i++)
        self->fThis->fSkinWeights[i] = 0.0f;
    return 0;
}

static int pyGBufferVertex_setUVWs(pyGBufferVertex* self, PyObject* value, void*) {
    if (value != NULL && !PyList_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "UVWs should be a list of up to 10 hsVector3s");
        return -1;
    }
    int size = (value == NULL) ? 0 : PyList_Size(value);
    if (size > 10) {
        PyErr_SetString(PyExc_RuntimeError, "UVWs should be a list of up to 10 hsVector3s");
        return -1;
    }
    for (int i=0; i<size; i++) {
        if (!pyVector3_Check(PyList_GetItem(value, i))) {
            PyErr_SetString(PyExc_TypeError, "UVWs should be a list of up to 10 hsVector3s");
            return -1;
        }
        self->fThis->fUVWs[i] = *((pyVector3*)PyList_GetItem(value, i))->fThis;
    }
    for (int i=size; i<10; i++)
        self->fThis->fUVWs[i] = hsVector3(0.0f, 0.0f, 0.0f);
    return 0;
}

static PyGetSetDef pyGBufferVertex_GetSet[] = {
    { "pos", (getter)pyGBufferVertex_getPos, (setter)pyGBufferVertex_setPos, NULL, NULL },
    { "normal", (getter)pyGBufferVertex_getNormal, (setter)pyGBufferVertex_setNormal, NULL, NULL },
    { "skinIdx", (getter)pyGBufferVertex_getSkin, (setter)pyGBufferVertex_setSkin, NULL, NULL },
    { "color", (getter)pyGBufferVertex_getColor, (setter)pyGBufferVertex_setColor, NULL, NULL },
    { "skinWeights", (getter)pyGBufferVertex_getWeights, (setter)pyGBufferVertex_setWeights, NULL, NULL },
    { "UVWs", (getter)pyGBufferVertex_getUVWs, (setter)pyGBufferVertex_setUVWs, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyGBufferVertex_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.plGBufferVertex",         /* tp_name */
    sizeof(pyGBufferVertex),            /* tp_basicsize */
    0,                                  /* tp_itemsize */

    (destructor)pyGBufferVertex_dealloc, /* tp_dealloc */
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

    Py_TPFLAGS_DEFAULT,                 /* tp_flags */
    "plGBufferVertex wrapper",          /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    NULL,                               /* tp_methods */
    NULL,                               /* tp_members */
    pyGBufferVertex_GetSet,             /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    (initproc)pyGBufferVertex___init__, /* tp_init */
    NULL,                               /* tp_alloc */
    pyGBufferVertex_new,                /* tp_new */
    NULL,                               /* tp_free */
    NULL,                               /* tp_is_gc */

    NULL,                               /* tp_bases */
    NULL,                               /* tp_mro */
    NULL,                               /* tp_cache */
    NULL,                               /* tp_subclasses */
    NULL,                               /* tp_weaklist */

    NULL,                               /* tp_del */
    TP_VERSION_TAG_INIT                 /* tp_version_tag */
};

PyObject* Init_pyGBufferVertex_Type() {
    if (PyType_Ready(&pyGBufferVertex_Type) < 0)
        return NULL;

    Py_INCREF(&pyGBufferVertex_Type);
    return (PyObject*)&pyGBufferVertex_Type;
}

int pyGBufferVertex_Check(PyObject* obj) {
    if (obj->ob_type == &pyGBufferVertex_Type
        || PyType_IsSubtype(obj->ob_type, &pyGBufferVertex_Type))
        return 1;
    return 0;
}

PyObject* pyGBufferVertex_FromGBufferVertex(const plGBufferVertex& vert) {
    pyGBufferVertex* pyvert = PyObject_New(pyGBufferVertex, &pyGBufferVertex_Type);
    pyvert->fThis = new plGBufferVertex(vert);
    return (PyObject*)pyvert;
}

}
