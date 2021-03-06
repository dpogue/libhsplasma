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

#ifndef _PFGUIMISC_H
#define _PFGUIMISC_H

#include "pfGUIControlMod.h"

class PLASMA_DLL pfGUIClickMapCtrl : public virtual pfGUIControlMod {
    CREATABLE(pfGUIClickMapCtrl, kGUIClickMapCtrl, pfGUIControlMod)

public:
    enum ClickMapFlags { kReportDragging = kDerivedFlagsStart, kReportHovering };

public:
    inline pfGUIClickMapCtrl();
};


class PLASMA_DLL pfGUIDragBarCtrl : public virtual pfGUIControlMod {
    CREATABLE(pfGUIDragBarCtrl, kGUIDragBarCtrl, pfGUIControlMod)
};


class PLASMA_DLL pfGUIDraggableMod : public virtual pfGUIControlMod {
    CREATABLE(pfGUIDraggableMod, kGUIDraggableMod, pfGUIControlMod)

public:
    enum DraggableFlags {
        kReportDragging = kDerivedFlagsStart, kHideCursorWhileDragging,
        kAlwaysSnapBackToStart
    };

public:
    inline pfGUIDraggableMod();
};


class PLASMA_DLL pfGUIEditBoxMod : public virtual pfGUIControlMod {
    CREATABLE(pfGUIEditBoxMod, kGUIEditBoxMod, pfGUIControlMod)
};


/* pfGUIClickMapCtrl */
pfGUIClickMapCtrl::pfGUIClickMapCtrl() {
    fFlags.setName(kReportDragging, "kReportDragging");
    fFlags.setName(kReportHovering, "kReportHovering");
}


/* pfGUIDraggableMod */
pfGUIDraggableMod::pfGUIDraggableMod() {
    fFlags.setName(kReportDragging, "kReportDragging");
    fFlags.setName(kHideCursorWhileDragging, "kHideCursorWhileDragging");
    fFlags.setName(kAlwaysSnapBackToStart, "kAlwaysSnapBackToStart");
}

#endif
