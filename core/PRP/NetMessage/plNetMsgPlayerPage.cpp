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

#include "plNetMsgPlayerPage.h"

plNetMsgPlayerPage::plNetMsgPlayerPage() : fUnload(0) { }

void plNetMsgPlayerPage::read(hsStream* S, plResManager* mgr) {
    plNetMessage::read(S, mgr);

    fUnload = S->readByte();
    fUoid.read(S);
}

void plNetMsgPlayerPage::write(hsStream* S, plResManager* mgr) {
    plNetMessage::write(S, mgr);

    S->writeByte(fUnload);
    fUoid.write(S);
}

void plNetMsgPlayerPage::IPrcWrite(pfPrcHelper* prc) {
    plNetMessage::IPrcWrite(prc);

    prc->startTag("PlayerPageParams");
    prc->writeParam("Unload", fUnload);
    prc->endTag();
    fUoid.prcWrite(prc);
    prc->closeTag();
}

void plNetMsgPlayerPage::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "PlayerPageParams") {
        fUnload = tag->getParam("Unload", "0").toUint();
        if (tag->hasChildren())
            fUoid.prcParse(tag->getFirstChild());
    } else {
        plNetMessage::IPrcParse(tag, mgr);
    }
}
