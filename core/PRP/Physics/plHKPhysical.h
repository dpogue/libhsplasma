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

/* The enumerations in this file are internal use ONLY.
 * Do NOT include this file in your own code!
 */
#include "plPhysical.h"

class PLASMA_DLL plHKSimDefs {
public:
    enum Bounds {
        kBoxBounds = 1, kSphereBounds, kHullBounds, kProxyBounds,
        kExplicitBounds, kNumBounds, kBoundsMax = 0xFF
    };

    enum Group {
        kGroupLOSOnly   =       0x0, // This seems most accurate?

        kGroupWTFAhnonay =      0x2, // ONLY seen on TreePlane in Sphere02
        kGroupWTFTeledahn =     0x4, // CollideGroup Slave Cave kickables

        kGroupClickable =   0x20000, //Av
        kGroupCreatures =   0x40000, //Cr

        kGroupAnimated  =  0x800000, //Ani
        kGroupDynamic   = 0x1000000, //DS
        kGroupStatic    = 0x2000000, //SS
        kGroupDetector  = 0x4000000, //Det
        kGroupAvatar    = 0x8000000
    };

    static unsigned int fromGroup(unsigned int group) {
        if (group == kGroupLOSOnly)
            return plSimDefs::kGroupLOSOnly;
        else if ((group & kGroupDynamic) != 0)
            return plSimDefs::kGroupDynamic;
        else if ((group & kGroupStatic) != 0)
            return plSimDefs::kGroupStatic;
        else if ((group & kGroupDetector) != 0)
            return plSimDefs::kGroupDetector;
        else if ((group & kGroupAvatar) != 0)
            return plSimDefs::kGroupAvatar;

        return plSimDefs::kGroupStatic;
    }

    static unsigned int toGroup(unsigned int group) {
        if (group == plSimDefs::kGroupStatic)
            return kGroupStatic;
        else if (group == plSimDefs::kGroupAvatar)
            return kGroupAvatar;
        else if (group == plSimDefs::kGroupDynamic)
            return kGroupDynamic;
        else if (group == plSimDefs::kGroupDetector)
            return kGroupDetector;
        else if (group == plSimDefs::kGroupLOSOnly)
            return kGroupLOSOnly;

        return kGroupStatic;
    }

    static unsigned int getBitshiftGroup(unsigned int group) {
        unsigned int retGroup = 0;

        if ((group & kGroupDynamic) != 0)
            retGroup |= (1 << plSimDefs::kGroupDynamic);
        if ((group & kGroupStatic) != 0)
            retGroup |= (1 << plSimDefs::kGroupStatic);
        if ((group & kGroupDetector) != 0)
            retGroup |= (1 << plSimDefs::kGroupDetector);
        if ((group & kGroupAvatar) != 0)
            retGroup |= (1 << plSimDefs::kGroupAvatar);

        return retGroup;
    }

    static unsigned int setBitshiftGroup(unsigned int group) {
        unsigned int retGroup = 0;

        for (size_t i=0; i<plSimDefs::kGroupMax; i++) {
            if ((group & (1u << i)) == (1u << i)) {
                retGroup |= toGroup(i);
            }
        }

        return retGroup;
    }

    static void fixGroups(plGenericPhysical* physical, unsigned int* memGroup,
                          unsigned int* repGroup, unsigned int* colGroup) {
        plSceneObject* so = plSceneObject::Convert(physical->getObject()->getObj());
        if (so == NULL)
            return;
        hsTArray<plKey> mods = so->getModifiers();

        for (size_t i = 0; i < mods.getSize(); i++) {
            if (mods[i]->getType() == kPickingDetector) {
                *colGroup |= plHKSimDefs::kGroupClickable;
            } else if (mods[i]->getType() == kATCAnim) {
                *memGroup |= plHKSimDefs::kGroupAnimated;
            }
        }
    }
};
