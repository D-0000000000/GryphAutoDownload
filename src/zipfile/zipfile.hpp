#ifndef _ZIP_FILE_HPP_
#define _ZIP_FILE_HPP_

#include "common/common_typedefs.hpp"

#include <cstdint>
#include <string>

#pragma pack(1)

struct LocalFileHeader
{
	uint32_t signature;
	uint16_t version;
	uint16_t gpflag;
	uint16_t method;
	uint16_t lastModTime;
	uint16_t lastModDate;
	uint32_t CRC32;
	uint32_t compressedSize;
	uint32_t uncompressedSize;
	uint16_t fileNameLength;
	uint16_t extraFieldLength;
};

#pragma pack()

int32_t rawInflate(MemChunk inchk, MemChunk outchk);

int32_t rawInflate(MemChunk inchk, const std::string &outFileName);

int rawStore(MemChunk inchk, const std::string &outFileName);

#endif