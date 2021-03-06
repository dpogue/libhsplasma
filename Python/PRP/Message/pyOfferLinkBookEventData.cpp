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
#include <PRP/Message/proEventData.h>
#include "pyEventData.h"
#include "PRP/KeyedObject/pyKey.h"

extern "C" {

static PyObject* pyOfferLinkBookEventData_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyOfferLinkBookEventData* self = (pyOfferLinkBookEventData*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new proOfferLinkBookEventData();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pyOfferLinkBookEventData_getOfferer(pyOfferLinkBookEventData* self, void*) {
    return pyKey_FromKey(self->fThis->getOfferer());
}

static PyObject* pyOfferLinkBookEventData_getTargetAge(pyOfferLinkBookEventData* self, void*) {
    return PyInt_FromLong(self->fThis->getTargetAge());
}

static PyObject* pyOfferLinkBookEventData_getOfferee(pyOfferLinkBookEventData* self, void*) {
    return PyInt_FromLong(self->fThis->getOfferee());
}

static int pyOfferLinkBookEventData_setOfferer(pyOfferLinkBookEventData* self, PyObject* value, void*) {
    if (value == NULL || value == Py_None) {
        self->fThis->setOfferer(plKey());
        return 0;
    } else if (pyKey_Check(value)) {
        self->fThis->setOfferer(*((pyKey*)value)->fThis);
        return 0;
    } else {
        PyErr_SetString(PyExc_TypeError, "offerer should be a plKey");
        return -1;
    }
}

static int pyOfferLinkBookEventData_setTargetAge(pyOfferLinkBookEventData* self, PyObject* value, void*) {
    if (value == NULL || !PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "targetAge should be an int");
        return -1;
    }
    self->fThis->setTargetAge(PyInt_AsLong(value));
    return 0;
}

static int pyOfferLinkBookEventData_setOfferee(pyOfferLinkBookEventData* self, PyObject* value, void*) {
    if (value == NULL || !PyInt_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "offeree should be an int");
        return -1;
    }
    self->fThis->setOfferee(PyInt_AsLong(value));
    return 0;
}

static PyMethodDef pyOfferLinkBookEventData_Methods[] = {
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pyOfferLinkBookEventData_GetSet[] = {
    { "offerer", (getter)pyOfferLinkBookEventData_getOfferer, (setter)pyOfferLinkBookEventData_setOfferer, NULL, NULL },
    { "targetAge", (getter)pyOfferLinkBookEventData_getTargetAge, (setter)pyOfferLinkBookEventData_setTargetAge, NULL, NULL },
    { "offeree", (getter)pyOfferLinkBookEventData_getOfferee, (setter)pyOfferLinkBookEventData_setOfferee, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyOfferLinkBookEventData_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.proOfferLinkBookEventData", /* tp_name */
    sizeof(pyOfferLinkBookEventData),   /* tp_basicsize */
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
    "proOfferLinkBookEventData wrapper", /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyOfferLinkBookEventData_Methods,   /* tp_methods */
    NULL,                               /* tp_members */
    pyOfferLinkBookEventData_GetSet,    /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyOfferLinkBookEventData_new,       /* tp_new */
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

PyObject* Init_pyOfferLinkBookEventData_Type() {
    pyOfferLinkBookEventData_Type.tp_base = &pyEventData_Type;
    if (PyType_Ready(&pyOfferLinkBookEventData_Type) < 0)
        return NULL;

    Py_INCREF(&pyOfferLinkBookEventData_Type);
    return (PyObject*)&pyOfferLinkBookEventData_Type;
}

int pyOfferLinkBookEventData_Check(PyObject* obj) {
    if (obj->ob_type == &pyOfferLinkBookEventData_Type
        || PyType_IsSubtype(obj->ob_type, &pyOfferLinkBookEventData_Type))
        return 1;
    return 0;
}

PyObject* pyOfferLinkBookEventData_FromOfferLinkBookEventData(proOfferLinkBookEventData* evt) {
    if (evt == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pyOfferLinkBookEventData* pyobj = PyObject_New(pyOfferLinkBookEventData, &pyOfferLinkBookEventData_Type);
    pyobj->fThis = evt;
    pyobj->fPyOwned = false;
    return (PyObject*)pyobj;
}

}
