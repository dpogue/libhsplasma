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

#ifndef _PLONESHOTCALLBACKS_H
#define _PLONESHOTCALLBACKS_H

#include "Util/hsTArray.hpp"
#include "PRP/KeyedObject/plKey.h"
#include "ResManager/plResManager.h"

class PLASMA_DLL plOneShotCallbacks {
public:
    class PLASMA_DLL plOneShotCallback {
    public:
        plString fMarker;
        plKey fReceiver;
        short fUser;

        plOneShotCallback();
    };

protected:
    hsTArray<plOneShotCallback> fCallbacks;

public:
    void read(hsStream* S, plResManager* mgr);
    void write(hsStream* S, plResManager* mgr);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag, plResManager* mgr);
};

#endif
