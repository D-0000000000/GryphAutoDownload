#ifndef _COMMON_TYPEDEFS_HPP_
#define _COMMON_TYPEDEFS_HPP_

#define DEBUG

#include <cstdint>

struct MemChunk
{
	uint64_t size;
	char *data;
};

#endif
