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

#ifndef _PLGBUFFERGROUP_H
#define _PLGBUFFERGROUP_H

#include "Math/hsGeometry3.h"
#include "Util/hsTArray.hpp"
#include "Util/hsTList.hpp"
#include "hsGDeviceRef.h"
#include "plVertCoder.h"

class PLASMA_DLL plGBufferCell {
public:
    unsigned int fVtxStart, fColorStart, fLength;

public:
    plGBufferCell();
    plGBufferCell(const plGBufferCell& init);

    void read(hsStream* S);
    void write(hsStream* S);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag);
};


class PLASMA_DLL plGBufferTriangle {
public:
    unsigned short fIndex1, fIndex2, fIndex3, fSpanIndex;
    hsVector3 fCenter;

public:
    plGBufferTriangle();
    plGBufferTriangle(const plGBufferTriangle& init);

    void read(hsStream* S);
    void write(hsStream* S);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag);
};


class PLASMA_DLL plGBufferVertex {
public:
    hsVector3 fPos, fNormal;
    int fSkinIdx;
    float fSkinWeights[3];
    unsigned int fColor;
    hsVector3 fUVWs[10];

public:
    plGBufferVertex();
    plGBufferVertex(const plGBufferVertex& init);
};


class PLASMA_DLL plGBufferGroup {
public:
    enum Formats {
        kUVCountMask = 0xF,
        kSkinNoWeights = 0x0,
        kSkin1Weight = 0x10,
        kSkin2Weights = 0x20,
        kSkin3Weights = 0x30,
        kSkinWeightMask = 0x30,
        kSkinIndices = 0x40,
        kEncoded = 0x80
    };

    enum GeometryStorage {
        kStoreUncompressed = 0,
        kStoreCompV1 = 0x1,
        kStoreCompV2 = 0x2,
        kStoreCompV3 = 0x3,
        kStoreCompTypeMask = 0x3,
        kStoreIsDirty = 0x4
    };

protected:
    unsigned int fFormat, fStride, fLiteStride, fGBuffStorageType;
    hsTArray<unsigned int> fVertBuffSizes, fIdxBuffCounts, fCompGBuffSizes;
    hsTArray<unsigned char*> fVertBuffStorage;
    hsTArray<unsigned short*> fIdxBuffStorage;
    hsTArray<hsTArray<plGBufferCell> > fCells;
    hsTArray<unsigned char*> fCompGBuffStorage;

    unsigned int ICalcVertexSize(unsigned int& lStride);
    bool INeedVertRecompression(PlasmaVer ver) const;

public:
    plGBufferGroup(unsigned char fmt);
    ~plGBufferGroup();

    void read(hsStream* S);
    void write(hsStream* S);
    void prcWrite(pfPrcHelper* prc);
    void prcParse(const pfPrcTag* tag);

    hsTArray<plGBufferVertex> getVertices(size_t idx, size_t start = 0, size_t count = (size_t)-1) const;
    hsTArray<unsigned short> getIndices(size_t idx, size_t start = 0, size_t count = (size_t)-1, size_t offset = 0) const;
    hsTArray<plGBufferCell> getCells(size_t idx) const;

    unsigned char getFormat() const { return fFormat; }
    size_t getSkinWeights() const { return (fFormat & kSkinWeightMask) >> 4; }
    size_t getNumUVs() const { return (fFormat & kUVCountMask); }
    bool getHasSkinIndices() const { return (fFormat & kSkinIndices) != 0; }

    void addVertices(const hsTArray<plGBufferVertex>& verts);
    void addIndices(const hsTArray<unsigned short>& indices);
    void addCells(const hsTArray<plGBufferCell>& cells);
    void setFormat(unsigned char format);
    void setSkinWeights(size_t skinWeights);
    void setNumUVs(size_t numUVs);
    void setHasSkinIndices(bool hasSI);

    void delVertices(size_t idx);
    void delIndices(size_t idx);
    void delCells(size_t idx);
    void clearVertices();
    void clearIndices();
    void clearCells();

    size_t getNumVertBuffers() const { return fVertBuffStorage.getSize(); }
    size_t getNumIdxBuffers() const { return fIdxBuffStorage.getSize(); }
    const unsigned char* getVertBufferStorage(size_t idx) const { return fVertBuffStorage[idx]; }
    const unsigned short* getIdxBufferStorage(size_t idx) const { return fIdxBuffStorage[idx]; }
    unsigned char* getMutableVertBuffer(size_t idx) { return fVertBuffStorage[idx]; }
    unsigned short* getMutableIdxBuffer(size_t idx) { return fIdxBuffStorage[idx]; }
    size_t getVertBufferSize(size_t idx) const { return fVertBuffSizes[idx]; }
    size_t getIdxBufferCount(size_t idx) const { return fIdxBuffCounts[idx]; }
    unsigned int getStride() const { return fStride; }
};

#endif
