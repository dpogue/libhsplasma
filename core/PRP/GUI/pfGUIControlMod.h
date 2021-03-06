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

#ifndef _PFGUICONTROLMOD_H
#define _PFGUICONTROLMOD_H

#include "PRP/Modifier/plModifier.h"
#include "Sys/hsColor.h"
#include "Math/hsGeometry3.h"
#include "PRP/Region/hsBounds.h"
#include "Math/hsMatrix44.h"
#include "pfGUIControlHandlers.h"

class PLASMA_DLL pfGUIColorScheme {
public:
    enum FontFlags {
        kFontBold = 0x01,
        kFontItalic = 0x02,
        kFontShadowed = 0x04
    };

private:
    hsColorRGBA fForeColor, fBackColor, fSelForeColor, fSelBackColor;
    int fTransparent;
    plString fFontFace;
    unsigned char fFontSize, fFontFlags;

public:
    pfGUIColorScheme();

    void read(hsStream* S);
    void write(hsStream* S);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag);

private:
    void IReset();

public:
    hsColorRGBA getForeColor() const { return fForeColor; }
    hsColorRGBA getBackColor() const { return fBackColor; }
    hsColorRGBA getSelForeColor() const { return fSelForeColor; }
    hsColorRGBA getSelBackColor() const { return fSelBackColor; }
    int getTransparent() const { return fTransparent; }
    plString getFontFace() const { return fFontFace; }
    unsigned char getFontSize() const { return fFontSize; }
    unsigned char getFontFlags() const { return fFontFlags; }

    void setForeColor(const hsColorRGBA& color) { fForeColor = color; }
    void setBackColor(const hsColorRGBA& color) { fBackColor = color; }
    void setSelForeColor(const hsColorRGBA& color) { fSelForeColor = color; }
    void setSelBackColor(const hsColorRGBA& color) { fSelBackColor = color; }
    void setTransparent(int trans) { fTransparent = trans; }
    void setFontFace(const plString& face) { fFontFace = face; }
    void setFontSize(unsigned char size) { fFontSize = size; }
    void setFontFlags(unsigned char flags) { fFontFlags = flags; }
};


class PLASMA_DLL pfGUIControlMod : public virtual plSingleModifier {
    CREATABLE(pfGUIControlMod, kGUIControlMod, plSingleModifier)

public:
    enum {
        kWantsInterest, kInheritProcFromDlg, kIntangible, kXparentBgnd,
        kScaleTextWithResolution, kTakesSpecialKeys, kHasProxy,
        kBetterHitTesting, kDerivedFlagsStart = 0x20
    };

protected:
    unsigned int fTagID;
    bool fVisible;
    pfGUICtrlProcWriteableObject* fHandler;
    plKey fDynTextMap, fDynTextLayer;
    pfGUIColorScheme* fColorScheme;
    hsTArray<int> fSoundIndices;
    plKey fProxy, fSkin;

public:
    pfGUIControlMod();
    virtual ~pfGUIControlMod();

    virtual void read(hsStream* S, plResManager* mgr);
    virtual void write(hsStream* S, plResManager* mgr);

protected:
    virtual void IPrcWrite(pfPrcHelper* prc);
    virtual void IPrcParse(const pfPrcTag* tag, plResManager* mgr);

public:
    const hsTArray<int>& getSoundIndices() const { return fSoundIndices; }
    hsTArray<int>& getSoundIndices() { return fSoundIndices; }
    void setSoundIndices(const hsTArray<int>& indices) { fSoundIndices = indices; }
    void clearSoundIndices() { fSoundIndices.clear(); }

    unsigned int getTagID() const { return fTagID; }
    bool isVisible() const { return fVisible; }
    pfGUICtrlProcWriteableObject* getHandler() const { return fHandler; }
    plKey getDynTextMap() const { return fDynTextMap; }
    plKey getDynTextLayer() const { return fDynTextLayer; }
    pfGUIColorScheme* getColorScheme() const { return fColorScheme; }
    plKey getProxy() const { return fProxy; }
    plKey getSkin() const { return fSkin; }

    void setTagID(unsigned int id) { fTagID = id; }
    void setVisible(bool visible) { fVisible = visible; }
    void setDynTextMap(plKey map) { fDynTextMap = map; }
    void setDynTextLayer(plKey layer) { fDynTextLayer = layer; }
    void setProxy(plKey proxy) { fProxy = proxy; }
    void setSkin(plKey skin) { fSkin = skin; }
    void setHandler(pfGUICtrlProcWriteableObject* handler);
    void setColorScheme(pfGUIColorScheme* scheme);
};

#endif
