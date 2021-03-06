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

#include "plGBufferGroup.h"

/* plGBufferCell */
plGBufferCell::plGBufferCell() : fVtxStart(0), fColorStart(0), fLength(0) { }

plGBufferCell::plGBufferCell(const plGBufferCell& init)
             : fVtxStart(init.fVtxStart), fColorStart(init.fColorStart),
               fLength(init.fLength) { }

void plGBufferCell::read(hsStream* S) {
    fVtxStart = S->readInt();
    fColorStart = S->readInt();
    fLength = S->readInt();
}

void plGBufferCell::write(hsStream* S) {
    S->writeInt(fVtxStart);
    S->writeInt(fColorStart);
    S->writeInt(fLength);
}

void plGBufferCell::prcWrite(pfPrcHelper* prc) {
    prc->startTag("plGBufferCell");
    prc->writeParam("VertexStart", fVtxStart);
    prc->writeParam("ColorStart", fColorStart);
    prc->writeParam("Length", fLength);
    prc->endTag(true);
}

void plGBufferCell::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != "plGBufferCell")
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());

    fVtxStart = tag->getParam("VertexStart", "0").toUint();
    fColorStart = tag->getParam("ColorStart", "0").toUint();
    fLength = tag->getParam("Length", "0").toUint();
}


/* plGBufferTriangle */
plGBufferTriangle::plGBufferTriangle()
                 : fIndex1(0), fIndex2(0), fIndex3(0), fSpanIndex(0) { }

plGBufferTriangle::plGBufferTriangle(const plGBufferTriangle& init)
                 : fIndex1(init.fIndex1), fIndex2(init.fIndex2),
                   fIndex3(init.fIndex3), fSpanIndex(init.fSpanIndex),
                   fCenter(init.fCenter) { }

void plGBufferTriangle::read(hsStream* S) {
    fIndex1 = S->readShort();
    fIndex2 = S->readShort();
    fIndex3 = S->readShort();
    fSpanIndex = S->readShort();
    fCenter.read(S);
}

void plGBufferTriangle::write(hsStream* S) {
    S->writeShort(fIndex1);
    S->writeShort(fIndex2);
    S->writeShort(fIndex3);
    S->writeShort(fSpanIndex);
    fCenter.write(S);
}

void plGBufferTriangle::prcWrite(pfPrcHelper* prc) {
    prc->writeSimpleTag("plGBufferTriangle");
      prc->startTag("Indices");
      prc->writeParam("Index1", fIndex1);
      prc->writeParam("Index2", fIndex2);
      prc->writeParam("Index3", fIndex3);
      prc->writeParam("SpanIndex", fSpanIndex);
      prc->endTag(true);
      prc->writeSimpleTag("Center");
      fCenter.prcWrite(prc);
      prc->closeTag();
    prc->closeTag();
}

void plGBufferTriangle::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != "plGBufferTriangle")
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());

    const pfPrcTag* child = tag->getFirstChild();
    while (child != NULL) {
        if (child->getName() == "Indices") {
            fIndex1 = child->getParam("Index1", "0").toUint();
            fIndex2 = child->getParam("Index2", "0").toUint();
            fIndex3 = child->getParam("Index3", "0").toUint();
            fSpanIndex = child->getParam("SpanIndex", "0").toUint();
        } else if (child->getName() == "Center") {
            if (child->hasChildren())
                fCenter.prcParse(child->getFirstChild());
        } else {
            throw pfPrcTagException(__FILE__, __LINE__, child->getName());
        }
        child = child->getNextSibling();
    }
}


/* plGBufferVertex */
plGBufferVertex::plGBufferVertex() : fSkinIdx(0), fColor(0) {
    fSkinWeights[0] = 0.0f;
    fSkinWeights[1] = 0.0f;
    fSkinWeights[2] = 0.0f;
}

plGBufferVertex::plGBufferVertex(const plGBufferVertex& init)
               : fPos(init.fPos), fNormal(init.fNormal), fSkinIdx(init.fSkinIdx),
                 fColor(init.fColor) {
    memcpy(fSkinWeights, init.fSkinWeights, 3 * sizeof(float));
    for (size_t i=0; i<10; i++)
        fUVWs[i] = init.fUVWs[i];
}


/* plGBufferGroup */
plGBufferGroup::plGBufferGroup(unsigned char fmt)
              : fGBuffStorageType(kStoreUncompressed) {
    setFormat(fmt);
}

plGBufferGroup::~plGBufferGroup() {
    for (size_t i=0; i<fVertBuffStorage.getSize(); i++)
        delete[] fVertBuffStorage[i];
    for (size_t i=0; i<fIdxBuffStorage.getSize(); i++)
        delete[] fIdxBuffStorage[i];
    for (size_t i=0; i<fCompGBuffStorage.getSize(); i++)
        delete[] fCompGBuffStorage[i];
}

unsigned int plGBufferGroup::ICalcVertexSize(unsigned int& lStride) {
    lStride = (getNumUVs() + 2) * 12;
    unsigned int numSkinWeights = getSkinWeights();
    if (numSkinWeights > 0) {
        lStride += numSkinWeights * sizeof(float);
        if (fFormat & kSkinIndices)
            lStride += sizeof(int);
    }
    return lStride + 8;
}

bool plGBufferGroup::INeedVertRecompression(PlasmaVer ver) const {
    if ((fGBuffStorageType & kStoreIsDirty) != 0)
        return true;
    if (ver.isHexIsle())
        return (fGBuffStorageType & kStoreCompTypeMask) != kStoreCompV3;
    else if (ver.isLive())
        return (fGBuffStorageType & kStoreCompTypeMask) != kStoreCompV2;
    else
        return (fGBuffStorageType & kStoreCompTypeMask) != kStoreCompV1;
}

void plGBufferGroup::read(hsStream* S) {
    if (S->getVer().isHexIsle()) {
        fFormat = S->readInt();
        S->readByte();
        S->readByte();
        S->readBool();
        S->readBool();
        S->readInt();
        S->readByte();
        // Don't know what the format parameters are, so better to just crash
        throw hsNotImplementedException(__FILE__, __LINE__, "HexIsle GBufferGroup");
    } else {
        fFormat = S->readByte();
        S->readInt();
        fStride = ICalcVertexSize(fLiteStride);
    }

    clearVertices();
    clearIndices();
    clearCells();

    if (!S->getVer().isUniversal() && (fFormat & kEncoded) != 0) {
        if (S->getVer().isHexIsle())
            fGBuffStorageType = kStoreCompV3;
        else if (S->getVer().isLive())
            fGBuffStorageType = kStoreCompV2;
        else
            fGBuffStorageType = kStoreCompV1;
    } else {
        fGBuffStorageType = kStoreUncompressed;
    }

    plVertCoder coder;
    size_t count = S->readInt();
    fVertBuffSizes.setSize(count);
    fVertBuffStorage.setSize(count);
    fCompGBuffSizes.setSize(count);
    fCompGBuffStorage.setSize(count);
    for (size_t i=0; i<count; i++) {
        unsigned char* vData;
        unsigned int vtxCount = 0, vtxSize = 0;
        if (fFormat & kEncoded) {
            vtxCount = S->readShort();
            vtxSize = vtxCount * fStride;
            fVertBuffSizes[i] = vtxSize;
            vData = new unsigned char[vtxSize];
            fVertBuffStorage[i] = vData;
            uint32_t compStart = S->pos();
            coder.read(S, vData, fFormat, vtxCount);

            // Store the compressed buffer too, so we don't have to
            // recompress anything later
            size_t compSize = S->pos() - compStart;
            fCompGBuffSizes[i] = compSize;
            fCompGBuffStorage[i] = new unsigned char[compSize];
            S->seek(compStart);
            S->read(compSize, fCompGBuffStorage[i]);
        } else {
            vtxSize = S->readInt();
            fVertBuffSizes[i] = vtxSize;
            vData = new unsigned char[vtxSize];
            S->read(vtxSize, vData);
            fVertBuffStorage[i] = vData;
            uint32_t colourcount = S->readInt();
            if (colourcount > 0) {
                unsigned char* cData = new unsigned char[colourcount*8];
                S->read(colourcount * 8, cData);
            }
            fCompGBuffSizes[i] = 0;
            fCompGBuffStorage[i] = NULL;
        }
    }

    count = S->readInt();
    fIdxBuffCounts.setSize(count);
    fIdxBuffStorage.setSize(count);
    for (size_t i=0; i<count; i++) {
        unsigned int idxCount = S->readInt();
        fIdxBuffCounts[i] = idxCount;
        unsigned short* iData = new unsigned short[idxCount];
        S->readShorts(idxCount, iData);
        fIdxBuffStorage[i] = iData;
    }

    fCells.setSize(fVertBuffStorage.getSize());
    for (size_t i=0; i<fVertBuffStorage.getSize(); i++) {
        size_t cellCount = S->readInt();
        fCells[i].setSize(cellCount);
        for (size_t j=0; j<cellCount; j++)
            fCells[i][j].read(S);
    }
}

void plGBufferGroup::write(hsStream* S) {
    fFormat |= kEncoded;
    unsigned int totalSize = 0;
    for (size_t i=0; i<fVertBuffSizes.getSize(); i++)
        totalSize += fVertBuffSizes[i];
    for (size_t i=0; i<fIdxBuffCounts.getSize(); i++)
        totalSize += fIdxBuffCounts[i] * sizeof(short);
    S->writeByte(fFormat);
    S->writeInt(totalSize);

    plVertCoder coder;
    S->writeInt(fVertBuffStorage.getSize());
    for (size_t i=0; i<fVertBuffStorage.getSize(); i++) {
        unsigned int count = fVertBuffSizes[i] / fStride;
        S->writeShort(count);
        if (S->getVer().isUniversal()) {
            S->writeInt(fVertBuffSizes[i]);
            S->write(fVertBuffSizes[i], fVertBuffStorage[i]);
        } else if (INeedVertRecompression(S->getVer()) || fCompGBuffStorage[i] == NULL) {
            coder.write(S, fVertBuffStorage[i], fFormat, fStride, count);
        } else {
            S->write(fCompGBuffSizes[i], fCompGBuffStorage[i]);
        }
    }

    S->writeInt(fIdxBuffStorage.getSize());
    for (size_t i=0; i<fIdxBuffCounts.getSize(); i++) {
        S->writeInt(fIdxBuffCounts[i]);
        S->writeShorts(fIdxBuffCounts[i], fIdxBuffStorage[i]);
    }

    for (size_t i=0; i<fVertBuffStorage.getSize(); i++) {
        S->writeInt(fCells[i].getSize());
        for (size_t j=0; j<fCells[i].getSize(); j++)
            fCells[i][j].write(S);
    }
}

void plGBufferGroup::prcWrite(pfPrcHelper* prc) {
    prc->startTag("plGBufferGroup");
    prc->writeParamHex("Format", fFormat);
    prc->endTag();

    if (!prc->isExcluded(pfPrcHelper::kExcludeVertexData)) {
        for (size_t grp=0; grp<fVertBuffStorage.getSize(); grp++) {
            prc->writeSimpleTag("VertexGroup");
            hsTArray<plGBufferVertex> verts = getVertices(grp);
            for (size_t i=0; i<verts.getSize(); i++) {
                prc->writeSimpleTag("Vertex");

                prc->writeSimpleTag("Position");
                verts[i].fPos.prcWrite(prc);
                prc->closeTag();

                prc->startTag("SkinWeights");
                if (fFormat & kSkinIndices)
                    prc->writeParam("SkinIndex", verts[i].fSkinIdx);
                prc->endTagNoBreak();
                for (size_t j=0; j<(size_t)((fFormat & kSkinWeightMask) >> 4); j++)
                    prc->getStream()->writeStr(plString::Format("%g ", verts[i].fSkinWeights[j]));
                prc->closeTagNoBreak();

                prc->writeSimpleTag("Normal");
                verts[i].fNormal.prcWrite(prc);
                prc->closeTag();
                prc->startTag("Color");
                prc->writeParamHex("value", verts[i].fColor);
                prc->endTag(true);

                prc->writeSimpleTag("UVWMaps");
                for (size_t j=0; j<(size_t)(fFormat & kUVCountMask); j++)
                    verts[i].fUVWs[j].prcWrite(prc);
                prc->closeTag();

                prc->closeTag();  // Vertex
            }
            prc->closeTag();  // VertexGroup
        }

        for (size_t i=0; i<fIdxBuffStorage.getSize(); i++) {
            prc->writeSimpleTag("IndexGroup");
            for (size_t j=0; j<fIdxBuffCounts[i]; j += 3) {
                prc->writeTagNoBreak("Triangle");
                prc->getStream()->writeStr(plString::Format("%d %d %d",
                                           fIdxBuffStorage[i][j],
                                           fIdxBuffStorage[i][j+1],
                                           fIdxBuffStorage[i][j+2]));
                prc->closeTagNoBreak();
            }
            prc->closeTag();
        }
        for (size_t i=0; i<fVertBuffStorage.getSize(); i++) {
            prc->writeSimpleTag("CellGroup");
            for (size_t j=0; j<fCells[i].getSize(); j++)
                fCells[i][j].prcWrite(prc);
            prc->closeTag();
        }
    } else {
        prc->writeComment("Vertex data excluded");
    }
    prc->closeTag(); // plGBufferGroup
}

void plGBufferGroup::prcParse(const pfPrcTag* tag) {
    if (tag->getName() != "plGBufferGroup")
        throw pfPrcTagException(__FILE__, __LINE__, tag->getName());
    fFormat = tag->getParam("Format", "0").toUint();
    fStride = ICalcVertexSize(fLiteStride);

    clearVertices();
    clearIndices();
    clearCells();

    fGBuffStorageType = kStoreUncompressed;

    const pfPrcTag* child = tag->getFirstChild();
    while (child != NULL) {
        if (child->getName() == "VertexGroup") {
            hsTArray<plGBufferVertex> buf;
            buf.setSize(child->countChildren());
            const pfPrcTag* vtxChild = child->getFirstChild();
            for (size_t i=0; i<buf.getSize(); i++) {
                if (vtxChild->getName() != "Vertex")
                    throw pfPrcTagException(__FILE__, __LINE__, vtxChild->getName());

                const pfPrcTag* subChild = vtxChild->getFirstChild();
                while (subChild != NULL) {
                    if (subChild->getName() == "Position") {
                        if (subChild->hasChildren())
                            buf[i].fPos.prcParse(subChild->getFirstChild());
                    } else if (subChild->getName() == "SkinWeights") {
                        buf[i].fSkinIdx = subChild->getParam("SkinIndex", "0").toInt();
                        hsTList<plString> wgtList = subChild->getContents();
                        if (wgtList.getSize() != (size_t)((fFormat & kSkinWeightMask) >> 4))
                            throw pfPrcParseException(__FILE__, __LINE__, "Incorrect Number of Skin Weights");
                        for (size_t j=0; j<(size_t)((fFormat & kSkinWeightMask) >> 4); j++)
                            buf[i].fSkinWeights[j] = wgtList.pop().toFloat();
                    } else if (subChild->getName() == "Normal") {
                        if (subChild->hasChildren())
                            buf[i].fNormal.prcParse(subChild->getFirstChild());
                    } else if (subChild->getName() == "Color") {
                        buf[i].fColor = subChild->getParam("value", "0").toUint();
                    } else if (subChild->getName() == "UVWMaps") {
                        if ((fFormat & kUVCountMask) != subChild->countChildren())
                            throw pfPrcParseException(__FILE__, __LINE__, "Incorrect Number of UVW maps");
                        const pfPrcTag* uvwChild = subChild->getFirstChild();
                        for (size_t j=0; j<(size_t)(fFormat & kUVCountMask); j++) {
                            buf[i].fUVWs[j].prcParse(uvwChild);
                            uvwChild = uvwChild->getNextSibling();
                        }
                    } else {
                        throw pfPrcTagException(__FILE__, __LINE__, subChild->getName());
                    }
                    subChild = subChild->getNextSibling();
                }
                vtxChild = vtxChild->getNextSibling();
            }
            addVertices(buf);
        } else if (child->getName() == "IndexGroup") {
            size_t idxCount = child->countChildren() * 3;
            fIdxBuffCounts.append(idxCount);
            unsigned short* idxBuff = new unsigned short[idxCount];
            fIdxBuffStorage.append(idxBuff);
            const pfPrcTag* idxChild = child->getFirstChild();
            for (size_t i=0; i<idxCount; i += 3) {
                if (idxChild->getName() != "Triangle")
                    throw pfPrcTagException(__FILE__, __LINE__, idxChild->getName());
                hsTList<plString> idxList = idxChild->getContents();
                if (idxList.getSize() != 3)
                    throw pfPrcParseException(__FILE__, __LINE__, "Triangles should have exactly 3 indices");
                idxBuff[i] = idxList.pop().toUint();
                idxBuff[i+1] = idxList.pop().toUint();
                idxBuff[i+2] = idxList.pop().toUint();
                idxChild = idxChild->getNextSibling();
            }
        } else if (child->getName() == "CellGroup") {
            fCells.append(hsTArray<plGBufferCell>());
            size_t idx = fCells.getSize() - 1;
            fCells[idx].setSize(child->countChildren());
            const pfPrcTag* cellChild = child->getFirstChild();
            for (size_t i=0; i<fCells[idx].getSize(); i++) {
                fCells[idx][i].prcParse(cellChild);
                cellChild = cellChild->getNextSibling();
            }
        } else {
            throw pfPrcTagException(__FILE__, __LINE__, child->getName());
        }
        child = child->getNextSibling();
    }
}

hsTArray<plGBufferVertex> plGBufferGroup::getVertices(size_t idx, size_t start, size_t count) const {
    hsTArray<plGBufferVertex> buf;

    unsigned char* cp = fVertBuffStorage[idx] + (fStride * start);
    if (count == (size_t)-1)
        count = (fVertBuffSizes[idx] / fStride) - start;
    buf.setSize(count);
    for (size_t i=0; i<count; i++) {
        buf[i].fPos.X = *(float*)cp; cp += sizeof(float);
        buf[i].fPos.Y = *(float*)cp; cp += sizeof(float);
        buf[i].fPos.Z = *(float*)cp; cp += sizeof(float);

        int weightCount = (fFormat & kSkinWeightMask) >> 4;
        if (weightCount > 0) {
            for (int j=0; j<weightCount; j++) {
                buf[i].fSkinWeights[j] = *(float*)cp;
                cp += sizeof(float);
            }
            if (fFormat & kSkinIndices) {
                buf[i].fSkinIdx = *(int*)cp;
                cp += sizeof(int);
            }
        }

        buf[i].fNormal.X = *(float*)cp; cp += sizeof(float);
        buf[i].fNormal.Y = *(float*)cp; cp += sizeof(float);
        buf[i].fNormal.Z = *(float*)cp; cp += sizeof(float);

        buf[i].fColor = *(unsigned int*)cp;
        cp += sizeof(unsigned int);

        // Zero (Color2?)
        cp += sizeof(unsigned int);

        for (size_t j=0; j<(size_t)(fFormat & kUVCountMask); j++) {
            buf[i].fUVWs[j].X = *(float*)cp; cp += sizeof(float);
            buf[i].fUVWs[j].Y = *(float*)cp; cp += sizeof(float);
            buf[i].fUVWs[j].Z = *(float*)cp; cp += sizeof(float);
        }
    }
    return buf;
}

hsTArray<unsigned short> plGBufferGroup::getIndices(size_t idx, size_t start, size_t count, size_t offset) const {
    hsTArray<unsigned short> buf;

    if (count == (size_t)-1)
        count = fIdxBuffCounts[idx] - start;
    buf.setSizeNull(count);
    for (size_t i=0; i<count; i++)
        buf[i] = fIdxBuffStorage[idx][i + start] - offset;
    return buf;
}

hsTArray<plGBufferCell> plGBufferGroup::getCells(size_t idx) const {
    return fCells[idx];
}

void plGBufferGroup::addVertices(const hsTArray<plGBufferVertex>& verts) {
    size_t vtxSize = verts.getSize() * fStride;
    fVertBuffSizes.append(vtxSize);
    fVertBuffStorage.append(new unsigned char[vtxSize]);
    fCompGBuffSizes.append(0);
    fCompGBuffStorage.append(NULL);
    size_t idx = fVertBuffStorage.getSize() - 1;

    unsigned char* cp = fVertBuffStorage[idx];
    for (size_t i=0; i<(fVertBuffSizes[idx] / fStride); i++) {
        *(float*)cp = verts[i].fPos.X; cp += sizeof(float);
        *(float*)cp = verts[i].fPos.Y; cp += sizeof(float);
        *(float*)cp = verts[i].fPos.Z; cp += sizeof(float);

        int weightCount = (fFormat & kSkinWeightMask) >> 4;
        if (weightCount > 0) {
            for (int j=0; j<weightCount; j++) {
                *(float*)cp = verts[i].fSkinWeights[j];
                cp += sizeof(float);
            }
            if (fFormat & kSkinIndices) {
                *(int*)cp = verts[i].fSkinIdx;
                cp += sizeof(int);
            }
        }

        *(float*)cp = verts[i].fNormal.X; cp += sizeof(float);
        *(float*)cp = verts[i].fNormal.Y; cp += sizeof(float);
        *(float*)cp = verts[i].fNormal.Z; cp += sizeof(float);

        *(unsigned int*)cp = verts[i].fColor;
        cp += sizeof(unsigned int);

        // Zero (Color2?)
        *(unsigned int*)cp = 0;
        cp += sizeof(unsigned int);

        for (size_t j=0; j<(size_t)(fFormat & kUVCountMask); j++) {
            *(float*)cp = verts[i].fUVWs[j].X; cp += sizeof(float);
            *(float*)cp = verts[i].fUVWs[j].Y; cp += sizeof(float);
            *(float*)cp = verts[i].fUVWs[j].Z; cp += sizeof(float);
        }
    }
}

void plGBufferGroup::addIndices(const hsTArray<unsigned short>& indices) {
    fIdxBuffCounts.append(indices.getSize());
    fIdxBuffStorage.append(new unsigned short[indices.getSize()]);
    size_t idx = fIdxBuffStorage.getSize() - 1;

    for (size_t i=0; i<fIdxBuffCounts[idx]; i++)
        fIdxBuffStorage[idx][i] = indices[i];
}

void plGBufferGroup::addCells(const hsTArray<plGBufferCell>& cells) {
    fCells.append(cells);
}

void plGBufferGroup::setFormat(unsigned char format) {
    fFormat = format;
    fStride = ICalcVertexSize(fLiteStride);
    fGBuffStorageType |= kStoreIsDirty;
}

void plGBufferGroup::setSkinWeights(size_t skinWeights) {
    fFormat &= ~kSkinWeightMask;
    fFormat |= (skinWeights << 4) & kSkinWeightMask;
    fGBuffStorageType |= kStoreIsDirty;
}

void plGBufferGroup::setNumUVs(size_t numUVs) {
    fFormat &= ~kUVCountMask;
    fFormat |= numUVs & kUVCountMask;
    fGBuffStorageType |= kStoreIsDirty;
}

void plGBufferGroup::setHasSkinIndices(bool hasSI) {
    if (hasSI)
        fFormat |= kSkinIndices;
    else
        fFormat &= ~kSkinIndices;
    fGBuffStorageType |= kStoreIsDirty;
}

void plGBufferGroup::delVertices(size_t idx) {
    delete[] fVertBuffStorage[idx];
    delete[] fCompGBuffStorage[idx];
    fVertBuffStorage.remove(idx);
    fCompGBuffStorage.remove(idx);
    fVertBuffSizes.remove(idx);
    fCompGBuffSizes.remove(idx);
}

void plGBufferGroup::delIndices(size_t idx) {
    delete[] fIdxBuffStorage[idx];
    fIdxBuffStorage.remove(idx);
    fIdxBuffCounts.remove(idx);
}

void plGBufferGroup::delCells(size_t idx) {
    fCells.remove(idx);
}

void plGBufferGroup::clearVertices() {
    for (size_t i=0; i<fVertBuffStorage.getSize(); i++)
        delete[] fVertBuffStorage[i];
    for (size_t i=0; i<fCompGBuffStorage.getSize(); i++)
        delete[] fCompGBuffStorage[i];
    fVertBuffStorage.clear();
    fVertBuffSizes.clear();
    fCompGBuffStorage.clear();
    fCompGBuffSizes.clear();
}

void plGBufferGroup::clearIndices() {
    for (size_t i=0; i<fIdxBuffStorage.getSize(); i++)
        delete[] fIdxBuffStorage[i];
    fIdxBuffStorage.clear();
    fIdxBuffCounts.clear();
}

void plGBufferGroup::clearCells() {
    fCells.clear();
}
