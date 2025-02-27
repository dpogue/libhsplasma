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

#ifndef _PLJPEG_H
#define _PLJPEG_H

#include "PlasmaDefs.h"
#include "Debug/hsExceptions.hpp"

class hsJPEGException : public hsException
{
public:
    inline hsJPEGException(const char* file, unsigned long line,
                           const char* message = nullptr) HS_NOEXCEPT
        : hsException(ST_LITERAL("libJPEG error"), file, line)
    {
        if (message != nullptr)
            fWhat += ST_LITERAL(": ") + message;
    }
};


class hsStream;
class plMipmap;

class HSPLASMA_EXPORT plJPEG
{
public:
    /* Read JPEG file from stream into buffer as bitmap data. */
    static void DecompressJPEG(hsStream* S, void* buf, size_t size);

    /* Read JPEG file from stream directly into a plMipmap. */
    static plMipmap* DecompressJPEG(hsStream* S);

    /* Write JPEG file to stream from bitmap data buffer. */
    static void CompressJPEG(hsStream* S, void* buf, size_t size,
                             uint32_t width, uint32_t height, uint32_t bpp);

private:
    plJPEG() = delete;
};

#endif
