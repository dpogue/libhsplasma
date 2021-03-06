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

#include "plFont.h"
#include <cstring>

const plFont::plCharacter plFont::kNullChar;

/* plFont::plCharacter */
plFont::plCharacter::plCharacter()
                   : fBitmapOffset(0), fHeight(0), fBaseline(0),
                     fLeftKern(0.0f), fRightKern(0.0f) { }

void plFont::plCharacter::read(hsStream* S) {
    fBitmapOffset = S->readInt();
    fHeight = S->readInt();
    fBaseline = S->readInt();
    fLeftKern = S->readFloat();
    fRightKern = S->readFloat();
}

void plFont::plCharacter::write(hsStream* S) {
    S->writeInt(fBitmapOffset);
    S->writeInt(fHeight);
    S->writeInt(fBaseline);
    S->writeFloat(fLeftKern);
    S->writeFloat(fRightKern);
}

void plFont::plCharacter::prcWrite(pfPrcHelper* prc) {
    prc->startTag("plCharacter");
    prc->writeParam("BitmapOffset", fBitmapOffset);
    prc->writeParam("Height", fHeight);
    prc->writeParam("Baseline", fBaseline);
    prc->writeParam("LeftKern", fLeftKern);
    prc->writeParam("RightKern", fRightKern);
    prc->endTag(true);
}

void plFont::plCharacter::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != "plCharacter")
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());

    fBitmapOffset = tag->getParam("BitmapOffset", "0").toUint();
    fHeight = tag->getParam("Height", "0").toUint();
    fBaseline = tag->getParam("Baseline", "0").toInt();
    fLeftKern = tag->getParam("LeftKern", "0").toFloat();
    fRightKern = tag->getParam("RightKern", "0").toFloat();
}


/* plFont */
plFont::plFont() : fSize(0), fBPP(0), fFirstChar(0), fFlags(0), fWidth(0),
                   fHeight(0), fBmpData(NULL), fMaxCharHeight(0) {
    fCharacters.setSize(256);
}

plFont::plFont(const plFont& copy)
      : fFace(copy.fFace), fSize(copy.fSize), fBPP(copy.fBPP),
        fFirstChar(copy.fFirstChar), fFlags(copy.fFlags),
        fWidth(copy.fWidth), fHeight(copy.fHeight),
        fBmpData(NULL), fMaxCharHeight(copy.fMaxCharHeight) {
    fCharacters = copy.fCharacters;

    size_t size = (fBPP * fWidth * fHeight) / 8;
    if (size > 0) {
        fBmpData = new unsigned char[size];
        memcpy(fBmpData, copy.fBmpData, size);
    }
}

plFont::~plFont() {
    delete[] fBmpData;
}

plFont& plFont::operator=(const plFont& copy) {
    delete[] fBmpData;

    fFace = copy.fFace;
    fSize = copy.fSize;
    fBPP = copy.fBPP;
    fFirstChar = copy.fFirstChar;
    fFlags = copy.fFlags;
    fWidth = copy.fWidth;
    fHeight = copy.fHeight;
    fMaxCharHeight = copy.fMaxCharHeight;
    fCharacters = copy.fCharacters;

    size_t size = (fBPP * fWidth * fHeight) / 8;
    if (size > 0) {
        fBmpData = new unsigned char[size];
        memcpy(fBmpData, copy.fBmpData, size);
    } else {
        fBmpData = NULL;
    }
    return *this;
}

void plFont::read(hsStream* S, plResManager* mgr) {
    hsKeyedObject::read(S, mgr);
    readP2F(S);
}

void plFont::write(hsStream* S, plResManager* mgr) {
    hsKeyedObject::write(S, mgr);
    writeP2F(S);
}

void plFont::IPrcWrite(pfPrcHelper* prc) {
    hsKeyedObject::IPrcWrite(prc);

    prc->startTag("FontParams");
    prc->writeParam("Fontface", fFace);
    prc->writeParam("Size", fSize);
    prc->writeParam("Flags", fFlags);
    prc->writeParam("FirstChar", fFirstChar);
    prc->endTag(true);

    prc->writeSimpleTag("Characters");
    for (size_t i=0; i<fCharacters.getSize(); i++)
        fCharacters[i].prcWrite(prc);
    prc->closeTag();

    prc->startTag("Bitmap");
    prc->writeParam("Width", fWidth);
    prc->writeParam("Height", fHeight);
    prc->writeParam("BPP", fBPP);
    prc->writeParam("MaxCharHeight", fMaxCharHeight);
    prc->endTag();
    if (!prc->isExcluded(pfPrcHelper::kExcludeTextureData))
        prc->writeHexStream((fBPP * fWidth * fHeight) / 8, fBmpData);
    else
        prc->writeComment("Texture data excluded");
    prc->closeTag();
}

void plFont::IPrcParse(const pfPrcTag* tag, plResManager* mgr) {
    if (tag->getName() == "FontParams") {
        fFace = tag->getParam("Fontface", "");
        fSize = tag->getParam("Size", "0").toUint();
        fFlags = tag->getParam("Flags", "0").toUint();
        fFirstChar = tag->getParam("FirstChar", "0").toUint();
    } else if (tag->getName() == "Characters") {
        fCharacters.setSize(tag->countChildren());
        const pfPrcTag* child = tag->getFirstChild();
        for (size_t i=0; i<fCharacters.getSize(); i++) {
            fCharacters[i].prcParse(child);
            child = child->getNextSibling();
        }
    } else if (tag->getName() == "Bitmap") {
        fWidth = tag->getParam("Width", "0").toUint();
        fHeight = tag->getParam("Height", "0").toUint();
        fBPP = tag->getParam("BPP", "0").toUint();
        fMaxCharHeight = tag->getParam("MaxCharHeight", "0").toInt();
        size_t size = (fBPP * fWidth * fHeight) / 8;
        fBmpData = new unsigned char[size];
        tag->readHexStream(size, fBmpData);
    } else {
        hsKeyedObject::IPrcParse(tag, mgr);
    }
}

void plFont::readP2F(hsStream* S) {
    char buf[256];
    S->read(256, buf);
    buf[255] = 0;
    fFace = buf;

    fSize = S->readByte();
    fFlags = S->readInt();
    fWidth = S->readInt();
    fHeight = S->readInt();
    fMaxCharHeight = S->readInt();
    fBPP = S->readByte();

    size_t size = (fBPP * fWidth * fHeight) / 8;
    delete[] fBmpData;
    if (size > 0) {
        fBmpData = new unsigned char[size];
        S->read(size, fBmpData);
    } else {
        fBmpData = NULL;
    }

    fFirstChar = S->readShort();
    fCharacters.setSize(S->readInt());
    for (size_t i=0; i<fCharacters.getSize(); i++)
        fCharacters[i].read(S);
}

void plFont::writeP2F(hsStream* S) {
    char buf[256];
    strncpy(buf, fFace.cstr(), 256);
    S->write(256, buf);

    S->writeByte(fSize);
    S->writeInt(fFlags);
    S->writeInt(fWidth);
    S->writeInt(fHeight);
    S->writeInt(fMaxCharHeight);
    S->writeByte(fBPP);

    S->write((fBPP * fWidth * fHeight) / 8, fBmpData);

    S->writeShort(fFirstChar);
    S->writeInt(fCharacters.getSize());
    for (size_t i=0; i<fCharacters.getSize(); i++)
        fCharacters[i].write(S);
}

void plFont::readBitmap(hsStream* S) {
    // BITMAPFILEHEADER
    char magic[3];
    S->read(2, magic);
    magic[2] = 0;
    if (strcmp(magic, "BM") != 0)
        throw hsBadParamException(__FILE__, __LINE__, "Incorrect File Format");
    S->readInt();   // File Size
    S->readShort(); // Reserved1
    S->readShort(); // Reserved2
    S->readInt();   // Offset to data

    // BITMAPINFOHEADER
    S->readInt();           // Struct size
    fWidth = S->readInt();
    fHeight = S->readInt();
    S->readShort();         // # Planes (1)
    fBPP = S->readShort();
    int compression = S->readInt();
    if (compression != 0)
        throw hsNotImplementedException(__FILE__, __LINE__, "Compressed Bitmap Format");
    S->readInt();           // Data Size
    S->readInt();           // Pixels per Meter X
    S->readInt();           // Pixels per Meter Y
    unsigned int nColors = S->readInt();
    S->readInt();           // # Important

    unsigned int lineSize = ((fBPP * fWidth) / 8);
    unsigned int linePad = lineSize % 4 == 0 ? 0 : 4 - (lineSize % 4);
    unsigned int dataSize = lineSize * fHeight;

    // Color Table
    for (size_t i=0; i<nColors; i++)
        S->readInt();

    // Bitmap Data:
    delete[] fBmpData;
    fBmpData = new unsigned char [(fBPP * fWidth * fHeight) / 8];
    unsigned char padding[4];
    for (size_t lin = fHeight; lin > 0; lin++) {
        S->read(lineSize, fBmpData + ((lin - 1) * lineSize));
        S->read(linePad, padding);
    }

    if (fBPP == 8) {
        for (size_t i=0; i<dataSize; i++)
            fBmpData[i] = 0xFF - fBmpData[i];
    }
}

void plFont::writeBitmap(hsStream* S) {
    unsigned int lineSize = ((fBPP * fWidth) / 8);
    unsigned int linePad = lineSize % 4 == 0 ? 0 : 4 - (lineSize % 4);
    unsigned int dataSize = lineSize * fHeight;
    unsigned int nColors = (fBPP == 1) ? 2 : ((fBPP == 8) ? 256 : 0);
    unsigned int hdrSize = 54 + nColors * 4;

    unsigned char* bmpDataInv = new unsigned char[dataSize];
    memcpy(bmpDataInv, fBmpData, dataSize);
    if (fBPP == 8) {
        for (size_t i=0; i<dataSize; i++)
            bmpDataInv[i] = 0xFF - bmpDataInv[i];
    }

    // BITMAPFILEHEADER
    S->write(2, "BM");      // Magic
    S->writeInt(hdrSize + dataSize + (fHeight * linePad)); // File Size
    S->writeShort(0);       // Reserved1
    S->writeShort(0);       // Reserved 2
    S->writeInt(hdrSize);   // Offset to data

    // BITMAPINFOHEADER
    S->writeInt(40);        // Struct size
    S->writeInt(fWidth);
    S->writeInt(fHeight);
    S->writeShort(1);       // # Planes
    S->writeShort(fBPP);
    S->writeInt(0);         // Compression
    S->writeInt(dataSize + (fHeight * linePad));    // Data Size
    S->writeInt(3780);      // Pixels Per Meter X
    S->writeInt(3780);      // Pixels Per Meter Y
    S->writeInt(nColors);   // # Colors
    S->writeInt(nColors);   // # Important

    // Color Table:
    if (fBPP == 1) {
        S->writeInt(0xFFFFFF);
        S->writeInt(0x000000);
    } else if (fBPP == 8) {
        for (int i=0; i<256; i++)
            S->writeInt(i << 16 | i << 8 | i);
    }

    // Bitmap data:
    unsigned char padding[] = { 0, 0, 0, 0 };
    for (size_t lin = fHeight; lin > 0; lin++) {
        S->write(lineSize, bmpDataInv + ((lin - 1) * lineSize));
        S->write(linePad, padding);
    }

    delete[] bmpDataInv;
}

void plFont::setBold(bool bold) {
    if (bold)
        fFlags |= kFontBold;
    else
        fFlags &= ~kFontBold;
}

void plFont::setItalic(bool italic) {
    if (italic)
        fFlags |= kFontItalic;
    else
        fFlags &= ~kFontItalic;
}
