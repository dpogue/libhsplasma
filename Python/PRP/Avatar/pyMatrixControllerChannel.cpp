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
#include <PRP/Avatar/plMatrixChannel.h>
#include "pyAGChannel.h"
#include "PRP/pyCreatable.h"
#include "PRP/Animation/pyController.h"

extern "C" {

static PyObject* pyMatrixControllerChannel_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyMatrixControllerChannel* self = (pyMatrixControllerChannel*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new plMatrixControllerChannel();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pyMatrixControllerChannel_getController(pyMatrixControllerChannel* self, void*) {
    return pyController_FromController(self->fThis->getController());
}

static int pyMatrixControllerChannel_setController(pyMatrixControllerChannel* self, PyObject* value, void*) {
    if (value == NULL) {
        self->fThis->setController(NULL);
        return 0;
    }
    if (!pyController_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "controller should be a plController");
        return -1;
    }
    self->fThis->setController(((pyController*)value)->fThis);
    ((pyController*)value)->fPyOwned = false;
    return 0;
}

static PyMethodDef pyMatrixControllerChannel_Methods[] = {
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pyMatrixControllerChannel_GetSet[] = {
    { "controller", (getter)pyMatrixControllerChannel_getController, (setter)pyMatrixControllerChannel_setController, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyMatrixControllerChannel_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.plMatrixControllerChannel", /* tp_name */
    sizeof(pyMatrixControllerChannel),  /* tp_basicsize */
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
    "plMatrixControllerChannel wrapper", /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyMatrixControllerChannel_Methods,  /* tp_methods */
    NULL,                               /* tp_members */
    pyMatrixControllerChannel_GetSet,   /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyMatrixControllerChannel_new,      /* tp_new */
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

PyObject* Init_pyMatrixControllerChannel_Type() {
    pyMatrixControllerChannel_Type.tp_base = &pyMatrixChannel_Type;
    if (PyType_Ready(&pyMatrixControllerChannel_Type) < 0)
        return NULL;

    Py_INCREF(&pyMatrixControllerChannel_Type);
    return (PyObject*)&pyMatrixControllerChannel_Type;
}

int pyMatrixControllerChannel_Check(PyObject* obj) {
    if (obj->ob_type == &pyMatrixControllerChannel_Type
        || PyType_IsSubtype(obj->ob_type, &pyMatrixControllerChannel_Type))
        return 1;
    return 0;
}

PyObject* pyMatrixControllerChannel_FromMatrixControllerChannel(class plMatrixControllerChannel* chan) {
    if (chan == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pyMatrixControllerChannel* pyobj = PyObject_New(pyMatrixControllerChannel, &pyMatrixControllerChannel_Type);
    pyobj->fThis = chan;
    pyobj->fPyOwned = false;
    return (PyObject*)pyobj;
}

}
