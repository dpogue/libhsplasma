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

#ifndef _PLEAXLISTENERMOD_H
#define _PLEAXLISTENERMOD_H

#include "PRP/Modifier/plModifier.h"
#include "Math/hsGeometry3.h"
#include "3rdPartyLibs/AL/EFX-Util.h"

class PLASMA_DLL plEAXListenerMod : public virtual plSingleModifier {
    CREATABLE(plEAXListenerMod, kEAXListenerMod, plSingleModifier)

protected:
    plKey fSoftRegion;
    EAXREVERBPROPERTIES fListenerProps;

public:
    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

class PLASMA_DLL plEAXEffect : public virtual plSingleModifier {
    CREATABLE(plEAXEffect, kEAXEffect, plSingleModifier)

public:
    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

class PLASMA_DLL plEAXReverbEffect : public virtual plEAXEffect {
    CREATABLE(plEAXReverbEffect, kEAXReverbEffect, plEAXEffect)

public:
    class Aperture {
    public:
        hsVector3 fX;
        hsVector3 fY;
        hsVector3 fZ;
    };

protected:
    plKey fSoftRegion;
    EAXREVERBPROPERTIES fListenerProps;
    hsTArray<Aperture> fApertures;

public:
    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);
};

#endif
