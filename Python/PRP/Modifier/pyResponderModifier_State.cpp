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

#include "pyResponderModifier.h"

#include <PRP/Modifier/plResponderModifier.h>
#include "PRP/Message/pyMessage.h"

extern "C" {

PY_PLASMA_DEALLOC(ResponderModifier_State)
PY_PLASMA_EMPTY_INIT(ResponderModifier_State)
PY_PLASMA_NEW(ResponderModifier_State, plResponderModifier::plResponderState)

PY_METHOD_VA(ResponderModifier_State, addCommand,
    "Params: msg, waitOn\n"
    "Add a command to the Responder state")
{
    pyMessage* msg;
    int waitOn;
    if (!PyArg_ParseTuple(args, "Oi", &msg, &waitOn)) {
        PyErr_SetString(PyExc_TypeError, "addCommand expects plMessage, int");
        return NULL;
    }
    if (!pyMessage_Check((PyObject*)msg)) {
        PyErr_SetString(PyExc_TypeError, "addCommand expects plMessage, int");
        return NULL;
    }
    self->fThis->addCommand(msg->fThis, waitOn);
    msg->fPyOwned = false;
    Py_RETURN_NONE;
}

PY_METHOD_VA(ResponderModifier_State, delCommand,
    "Params: idx\n"
    "Delete a command from the Responder state")
{
    int idx;
    if (!PyArg_ParseTuple(args, "i", &idx)) {
        PyErr_SetString(PyExc_TypeError, "delCommand expects an int");
        return NULL;
    }
    self->fThis->delCommand(idx);
    Py_RETURN_NONE;
}

PY_METHOD_NOARGS(ResponderModifier_State, clearCommands,
    "Delete all commands from the Responder state")
{
    self->fThis->clearCommands();
    Py_RETURN_NONE;
}

static PyObject* pyResponderModifier_State_getCommands(pyResponderModifier_State* self, void*) {
    PyObject* list = PyList_New(self->fThis->fCmds.size());
    for (size_t i=0; i<self->fThis->fCmds.size(); i++)
        PyList_SET_ITEM(list, i, pyResponderModifier_Cmd_FromResponderModifier_Cmd(self->fThis->fCmds[i]));
    return list;
}

static PyObject* pyResponderModifier_State_getWaits(pyResponderModifier_State* self, void*) {
    PyObject* dict = PyDict_New();
    std::map<int8_t, int8_t>::iterator wp = self->fThis->fWaitToCmd.begin();
    for ( ; wp != self->fThis->fWaitToCmd.end(); wp++)
        PyDict_SetItem(dict, pyPlasma_convert(wp->first), pyPlasma_convert(wp->second));
    return dict;
}

static int pyResponderModifier_State_setCommands(pyResponderModifier_State* self, PyObject* value, void*) {
    PyErr_SetString(PyExc_TypeError, "To add commands, use addCommand");
    return -1;
}

static int pyResponderModifier_State_setWaits(pyResponderModifier_State* self, PyObject* value, void*) {
    if (value == NULL) {
        self->fThis->fWaitToCmd.clear();
        return 0;
    }
    if (!PyDict_Check(value)) {
        PyErr_SetString(PyExc_TypeError, "waitToCmd should be a dict { int => int }");
        return -1;
    }

    PyObject *dkey, *dvalue;
    Py_ssize_t pos = 0;
    self->fThis->fWaitToCmd.clear();

    while (PyDict_Next(value, &pos, &dkey, &dvalue)) {
        if (!PyInt_Check(dkey) || !PyInt_Check(dvalue)) {
            PyErr_SetString(PyExc_TypeError, "waitToCmd should be a dict { int => int }");
            return -1;
        }
        self->fThis->fWaitToCmd[PyInt_AsLong(dkey)] = PyInt_AsLong(dvalue);
    }
    return 0;
}

static PyMethodDef pyResponderModifier_State_Methods[] = {
    pyResponderModifier_State_addCommand_method,
    pyResponderModifier_State_delCommand_method,
    pyResponderModifier_State_clearCommands_method,
    PY_METHOD_TERMINATOR
};

PY_PROPERTY_MEMBER(int8_t, ResponderModifier_State, numCallbacks, fNumCallbacks)
PY_PROPERTY_MEMBER(int8_t, ResponderModifier_State, switchToState, fSwitchToState)

static PyGetSetDef pyResponderModifier_State_GetSet[] = {
    { _pycs("commands"), (getter)pyResponderModifier_State_getCommands,
        (setter)pyResponderModifier_State_setCommands, NULL, NULL },
    pyResponderModifier_State_numCallbacks_getset,
    pyResponderModifier_State_switchToState_getset,
    { _pycs("waitToCmd"), (getter)pyResponderModifier_State_getWaits,
        (setter)pyResponderModifier_State_setWaits, NULL, NULL },
    PY_GETSET_TERMINATOR
};

PyTypeObject pyResponderModifier_State_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "PyHSPlasma.plResponderModifier_State", /* tp_name */
    sizeof(pyResponderModifier_State),  /* tp_basicsize */
    0,                                  /* tp_itemsize */

    pyResponderModifier_State_dealloc,  /* tp_dealloc */
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
    "plResponderModifier::plResponderState wrapper", /* tp_doc */

    NULL,                               /* tp_traverse */
    NULL,                               /* tp_clear */
    NULL,                               /* tp_richcompare */
    0,                                  /* tp_weaklistoffset */
    NULL,                               /* tp_iter */
    NULL,                               /* tp_iternext */

    pyResponderModifier_State_Methods,  /* tp_methods */
    NULL,                               /* tp_members */
    pyResponderModifier_State_GetSet,   /* tp_getset */
    NULL,                               /* tp_base */
    NULL,                               /* tp_dict */
    NULL,                               /* tp_descr_get */
    NULL,                               /* tp_descr_set */
    0,                                  /* tp_dictoffset */

    pyResponderModifier_State___init__, /* tp_init */
    NULL,                               /* tp_alloc */
    pyResponderModifier_State_new,      /* tp_new */
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

PyObject* Init_pyResponderModifier_State_Type() {
    if (PyType_Ready(&pyResponderModifier_State_Type) < 0)
        return NULL;

    Py_INCREF(&pyResponderModifier_State_Type);
    return (PyObject*)&pyResponderModifier_State_Type;
}

PY_PLASMA_IFC_METHODS(ResponderModifier_State, plResponderModifier::plResponderState)

}
