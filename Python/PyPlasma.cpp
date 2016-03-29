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

#include "PyPlasma.h"
#include <unordered_set>

PyObject* PyString_FromPlasmaString(const ST::string& str) {
    return PyString_FromString(str.c_str());
}

PyObject* PyUnicode_FromPlasmaString(const ST::string& str) {
    return PyUnicode_DecodeUTF8(str.c_str(), str.size(), NULL);
}

ST::String PyAnyString_AsPlasmaString(PyObject* str) {
    if (PyUnicode_Check(str)) {
        PyObject* utfStr = PyUnicode_AsUTF8String(str);
        ST::string plstr = PyBytes_AsString(utfStr);
        Py_XDECREF(utfStr);
        return plstr;
    } else {
        return PyBytes_AsString(str);
    }
}

int PyType_CheckAndReady(PyTypeObject* type)
{
    static std::unordered_set<PyTypeObject*> init_bases;
    if (type->tp_base != NULL && init_bases.find(type->tp_base) == init_bases.end()) {
        fputs(plString::Format("ERROR: Base %s for type %s is not initialized\n",
                               type->tp_base->tp_name, type->tp_name).cstr(),
              stderr);
        fputs(plString::Format("Classes derived from %s WILL NOT WORK CORRECTLY\n",
                               type->tp_base->tp_name).cstr(), stderr);
    }
    int result = PyType_Ready(type);
    if (result == 0)
        init_bases.insert(type);
    else
        fputs(plString::Format("WARN: Failed to ready %s", type->tp_name).cstr(), stderr);
    return result;
}
