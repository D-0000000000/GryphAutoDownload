#ifndef _FILE_DOWNLOAD_HPP_
#define _FILE_DOWNLOAD_HPP_

#include <common/common_typedefs.hpp>

#include <cstdint>
#include <string>

int filedl_init();

int filedl_deinit();

int filedl_download(std::string url, MemChunk &chunk);

#endif
