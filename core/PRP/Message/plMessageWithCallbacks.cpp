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

#include "plMessageWithCallbacks.h"

plMessageWithCallbacks::~plMessageWithCallbacks() {
    for (size_t i=0; i<fCallbacks.getSize(); i++)
        delete fCallbacks[i];
}

void plMessageWithCallbacks::read(hsStream* S, plResManager* mgr) {
    plMessage::read(S, mgr);

    clearCallbacks();
    fCallbacks.setSizeNull(S->readInt());
    for (size_t i=0; i<fCallbacks.getSize(); i++) {
        fCallbacks[i] = plMessage::Convert(mgr->ReadCreatable(S));
        if (fCallbacks[i] == NULL)
            throw hsNotImplementedException(__FILE__, __LINE__, "Callback Message");
    }
}

void plMessageWithCallbacks::write(hsStream* S, plResManager* mgr) {
    plMessage::write(S, mgr);

    S->writeInt(fCallbacks.getSize());
    for (size_t i=0; i<fCallbacks.getSize(); i++)
        mgr->WriteCreatable(S, fCallbacks[i]);
}

void plMessageWithCallbacks::IPrcWrite(pfPrcHelper* prc) {
    plMessage::IPrcWrite(prc);

    prc->writeSimpleTag("Callbacks");
    for (size_t i=0; i<fCallbacks.getSize(); i++)
        fCallbacks[i]->prcWrite(prc);
    prc->closeTag();
}

void plMessageWithCallbacks::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "Callbacks") {
        clearCallbacks();
        fCallbacks.setSizeNull(tag->countChildren());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fCallbacks.getSize(); i++) {
            fCallbacks[i] = plMessage::Convert(mgr->prcParseCreatable(child));
            child = child->getNextSibling();
        }
    } else {
        plMessage::IPrcParse(tag, mgr);
    }
}

void plMessageWithCallbacks::delCallback(size_t idx) {
    delete fCallbacks[idx];
    fCallbacks.remove(idx);
}

void plMessageWithCallbacks::clearCallbacks() {
    for (size_t i=0; i<fCallbacks.getSize(); i++)
        delete fCallbacks[i];
    fCallbacks.clear();
}
