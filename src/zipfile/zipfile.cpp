#include "zipfile.hpp"
#include "common/common_typedefs.hpp"

#include <zlib-ng.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string getFileName(const std::string &str)
{
	int lastdim = 0;
	for (int i = str.length() - 1; i >= 0; i--)
	{
		if (str[i] == '/' || str[i] == '\\')
		{
			lastdim = i + 1;
			break;
		}
	}
	return str.substr(lastdim);
}

// class RawInflate
// {
// 	std::string rawData;
// 	std::string nextIn;
// 	std::string nextOut;

// 	zng_stream strm;

// 	RawInflate()
// 	{
// 		strm.zalloc = Z_NULL;
// 		strm.zfree = Z_NULL;
// 		strm.opaque = Z_NULL;
// 		strm.avail_in = 0;
// 		strm.next_in = Z_NULL;
// 		int32_t ret = zng_inflateInit2(&strm, -15);
// 	}

// 	int inflate()
// 	{
// 		// strm.avail_in = lfh->compressedSize;
// 		// strm.next_in = (uint8_t *)compressedData.data();
// 		// strm.avail_out = lfh->uncompressedSize;
// 		// strm.next_out = (uint8_t *)uncompressedData.data();

// 		// ret = zng_inflate(&strm, Z_NO_FLUSH);
// 		// std::cout << "inflate result: " << ret << "\n";
// 		// zng_inflateEnd(&strm);
// 	}

// 	int inflateWrite()
// 	{
// 	}
// };

class ZipFileChunk
{
	LocalFileHeader lfh;
	std::string fileName;
	std::string extraField;
	std::string compressedData;

	int readHeader()
	{
		// char rdbuf[sizeof(LocalFileHeader)];
		// std::cout << sizeof(LocalFileHeader) << "\n";

		// std::cout << "**** local file header ****\n";

		// memset(rdbuf, 0, sizeof(LocalFileHeader));

		// fin.read(rdbuf, sizeof(LocalFileHeader));

		// LocalFileHeader *lfh = (LocalFileHeader *)rdbuf;
		// // std::cout << lfh->signature << "\n";
		// // std::cout << lfh->version << "\n";
		// std::cout << "method: " << lfh->method << "\n";
		// std::cout << lfh->compressedSize << "\n";
		// std::cout << lfh->uncompressedSize << "\n";
		// std::cout << lfh->fileNameLength << "\n";
		// std::cout << lfh->extraFieldLength << "\n";

		// std::string fileName(lfh->fileNameLength, '\0');
		// fin.read(fileName.data(), lfh->fileNameLength);
		// std::string extraField(lfh->extraFieldLength, '\0');
		// fin.read(extraField.data(), lfh->extraFieldLength);

		// std::cout << fileName << "\n";
		// std::cout << extraField << "\n";
	}
};

int32_t rawInflate(char *in, uint32_t insize, char *out, uint32_t outsize)
{
	zng_stream strm;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	int32_t ret = zng_inflateInit2(&strm, -15);

	strm.avail_in = insize;
	strm.next_in = (uint8_t *)in;
	strm.avail_out = outsize;
	strm.next_out = (uint8_t *)out;

	ret = zng_inflate(&strm, Z_NO_FLUSH);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		return ret;
	}
	ret = zng_inflateEnd(&strm);
	return ret;
}

int32_t rawInflate(MemChunk inchk, MemChunk outchk)
{
	zng_stream strm;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	int32_t ret = zng_inflateInit2(&strm, -15);

	strm.avail_in = inchk.size;
	strm.next_in = (uint8_t *)inchk.data;
	strm.avail_out = outchk.size;
	strm.next_out = (uint8_t *)outchk.data;

	ret = zng_inflate(&strm, Z_NO_FLUSH);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		return ret;
	}
	ret = zng_inflateEnd(&strm);
	return ret;
}

int32_t rawInflate(MemChunk inchk, const std::string &outFileName)
{
	zng_stream strm;

	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	int32_t ret = zng_inflateInit2(&strm, -15);

	strm.avail_in = inchk.size;
	strm.next_in = (uint8_t *)inchk.data;

	constexpr uint32_t CHUNKSIZE = 131072;
	uint8_t out[CHUNKSIZE];

	std::fstream fout(outFileName, std::ios::out | std::ios::binary);

	do
	{
		strm.avail_out = CHUNKSIZE;
		strm.next_out = out;

		ret = zng_inflate(&strm, Z_NO_FLUSH);
		if (ret == Z_STREAM_ERROR)
		{
			return -1;
		}
		switch (ret)
		{
		case Z_NEED_DICT:
			ret = Z_DATA_ERROR; /* and fall through */
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			zng_inflateEnd(&strm);
			return ret;
		}

		uint32_t writesize = CHUNKSIZE - strm.avail_out;
		fout.write((char *)out, writesize);

	} while (strm.avail_out == 0);

	ret = zng_inflate(&strm, Z_NO_FLUSH);
	if (ret != Z_OK && ret != Z_STREAM_END)
	{
		return ret;
	}
	ret = zng_inflateEnd(&strm);
	return ret;
}

int rawStore(MemChunk inchk, const std::string &outFileName)
{
	std::fstream fout(outFileName, std::ios::out | std::ios::binary);
	fout.write(inchk.data, inchk.size);
	if (fout.fail())
	{
		return -1;
	}
	return 0;
}
