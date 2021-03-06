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
#include <PRP/Geometry/plOccluder.h>
#include "pyOccluder.h"
#include "PRP/Region/pyBounds.h"
#include "PRP/pyCreatable.h"
#include "Math/pyMatrix.h"

extern "C" {

static PyObject* pyMobileOccluder_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
    pyMobileOccluder* self = (pyMobileOccluder*)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->fThis = new plMobileOccluder();
        self->fPyOwned = true;
    }
    return (PyObject*)self;
}

static PyObject* pyMobileOccluder_getLocalToWorld(pyMobileOccluder* self, void*) {
    return pyMatrix44_FromMatrix44(self->fThis->getLocalToWorld());
}

static PyObject* pyMobileOccluder_getWorldToLocal(pyMobileOccluder* self, void*) {
    return pyMatrix44_FromMatrix44(self->fThis->getWorldToLocal());
}

static PyObject* pyMobileOccluder_getLocalBounds(pyMobileOccluder* self, void*) {
    return pyBounds3Ext_FromBounds3Ext(self->fThis->getLocalBounds());
}

static int pyMobileOccluder_setLocalToWorld(pyMobileOccluder* self, PyObject* value, void*) {
    if (value == NULL || !pyMatrix44_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "localToWorld should be an hsMatrix44");
        return -1;
    }
    self->fThis->setLocalToWorld(*((pyMatrix44*)value)->fThis);
    return 0;
}

static int pyMobileOccluder_setWorldToLocal(pyMobileOccluder* self, PyObject* value, void*) {
    if (value == NULL || !pyMatrix44_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "worldToLocal should be an hsMatrix44");
        return -1;
    }
    self->fThis->setWorldToLocal(*((pyMatrix44*)value)->fThis);
    return 0;
}

static int pyMobileOccluder_setLocalBounds(pyMobileOccluder* self, PyObject* value, void*) {
    if (value == NULL || value == Py_None) {
        self->fThis->setLocalBounds(hsBounds3Ext());
        return 0;
    }
    if (!pyBounds3Ext_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "localBounds should be an hsBounds3Ext");
        return -1;
    }
    self->fThis->setLocalBounds(*((pyBounds3Ext*)value)->fThis);
    return 0;
}

static PyMethodDef pyMobileOccluder_Methods[] = {
    { NULL, NULL, 0, NULL }
};

static PyGetSetDef pyMobileOccluder_GetSet[] = {
    { "localToWorld", (getter)pyMobileOccluder_getLocalToWorld, (setter)pyMobileOccluder_setLocalToWorld, NULL, NULL },
    { "worldToLocal", (getter)pyMobileOccluder_getWorldToLocal, (setter)pyMobileOccluder_setWorldToLocal, NULL, NULL },
    { "localBounds", (getter)pyMobileOccluder_getLocalBounds, (setter)pyMobileOccluder_setLocalBounds, NULL, NULL },
    { NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyMobileOccluder_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyPlasma.plMobileOccluder",        /* tp_name */
    sizeof(pyMobileOccluder),           /* tp_basicsize */
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
    "plMobileOccluder wrapper",         /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyMobileOccluder_Methods,           /* tp_methods */
    NULL,                               /* tp_members */
    pyMobileOccluder_GetSet,            /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    NULL,                               /* tp_init */
    NULL,                               /* tp_alloc */
    pyMobileOccluder_new,               /* tp_new */
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

PyObject* Init_pyMobileOccluder_Type() {
    pyMobileOccluder_Type.tp_base = &pyOccluder_Type;
    if (PyType_Ready(&pyMobileOccluder_Type) < 0)
        return NULL;

    Py_INCREF(&pyMobileOccluder_Type);
    return (PyObject*)&pyMobileOccluder_Type;
}

int pyMobileOccluder_Check(PyObject* obj) {
    if (obj->ob_type == &pyMobileOccluder_Type
        || PyType_IsSubtype(obj->ob_type, &pyMobileOccluder_Type))
        return 1;
    return 0;
}

PyObject* pyMobileOccluder_FromMobileOccluder(class plMobileOccluder* obj) {
    if (obj == NULL) {
        Py_INCREF(Py_None);
        return Py_None;
    }
    pyMobileOccluder* pobj = PyObject_New(pyMobileOccluder, &pyMobileOccluder_Type);
    pobj->fThis = obj;
    pobj->fPyOwned = false;
    return (PyObject*)pobj;
}

}
