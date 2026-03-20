#include "common/common_typedefs.hpp"
#include "data_queue/data_queue.hpp"
#include "zipfile/zipfile.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <cstring>
#include <filesystem>
#include <thread>

static bool runningFlag;
std::thread extractThread;

int extract_main()
{
	uint64_t chkread = 0;
	MemChunk incompleteChunk;
	MemChunk nextchk;
	incompleteChunk.data = nullptr;
	incompleteChunk.size = 0;
	while (runningFlag)
	{
		if (!dlBuffer.read_available())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		nextchk = dlBuffer.front();
		dlBuffer.pop();

		if (nextchk.data == nullptr && nextchk.size == 0)
		{
			spdlog::info("Dummy exit");
			runningFlag = false;
			break;
		}

		uint64_t curpos = 0;
		while (curpos < nextchk.size)
		{
			// incomplete header
			if (chkread < sizeof(LocalFileHeader))
			{
				uint64_t needsize = std::min(sizeof(LocalFileHeader) - chkread, nextchk.size - curpos);
				incompleteChunk.data = (char *)realloc(incompleteChunk.data, sizeof(LocalFileHeader));
				incompleteChunk.size = sizeof(LocalFileHeader);
				memcpy(incompleteChunk.data + chkread, nextchk.data + curpos, needsize);
				curpos += needsize;
				chkread += needsize;
			}
			// complete header
			else
			{
				LocalFileHeader *lfh = (LocalFileHeader *)incompleteChunk.data;
				if (lfh->signature != 0x04034b50UL)
				{
					runningFlag = false;
					break;
				}
				uint64_t fullsize = lfh->fileNameLength + lfh->extraFieldLength + lfh->compressedSize + sizeof(LocalFileHeader);
				// complete compressed data
				if (chkread == fullsize)
				{
					std::string fileName(incompleteChunk.data + sizeof(LocalFileHeader), incompleteChunk.data + sizeof(LocalFileHeader) + lfh->fileNameLength);

					// directory
					if (fileName.back() == '/' || fileName.back() == '\\')
					{
						std::filesystem::create_directory(fileName);
					}
					// store
					else if (lfh->method == 0)
					{
						if (lfh->compressedSize != lfh->uncompressedSize)
						{
							// unmatched size
						}
						else
						{
							MemChunk rawStoreChunk;
							rawStoreChunk.data = incompleteChunk.data + sizeof(LocalFileHeader) + lfh->fileNameLength + lfh->extraFieldLength;
							rawStoreChunk.size = lfh->compressedSize;
							int retStore = rawStore(rawStoreChunk, fileName);
							if (retStore != 0)
							{
								spdlog::error("{} failed. Method: Store", fileName);
							}
							else
							{
								spdlog::info("{} extracted", fileName);
							}
						}
					}
					// deflate
					else if (lfh->method == 8)
					{
						MemChunk rawDeflateChunk;
						rawDeflateChunk.data = incompleteChunk.data + sizeof(LocalFileHeader) + lfh->fileNameLength + lfh->extraFieldLength;
						rawDeflateChunk.size = lfh->compressedSize;
						int retInflate = rawInflate(rawDeflateChunk, fileName);
						if (retInflate != 0)
						{
							spdlog::error("{} failed. Method: Deflate", fileName);
						}
						else
						{
							spdlog::info("{} extracted", fileName);
						}
					}
					chkread = 0;
				}
				// incomplete compressed data
				else
				{
					incompleteChunk.data = (char *)realloc(incompleteChunk.data, fullsize);
					incompleteChunk.size = fullsize;
					uint64_t copysize = std::min(fullsize - chkread, nextchk.size - curpos);
					memcpy(incompleteChunk.data + chkread, nextchk.data + curpos, copysize);
					curpos += copysize;
					chkread += copysize;
				}
			}
		}

		free(nextchk.data);
		nextchk.data = nullptr;
		nextchk.size = 0;
	}
	if (incompleteChunk.data != nullptr)
	{
		free(incompleteChunk.data);
		incompleteChunk.data = nullptr;
		incompleteChunk.size = 0;
	}
	if (nextchk.data != nullptr)
	{
		free(nextchk.data);
		nextchk.data = nullptr;
		nextchk.size = 0;
	}

	while (runningFlag)
	{
		spdlog::info("Clear buffer");
		if (!dlBuffer.read_available())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			continue;
		}
		MemChunk nextchk = dlBuffer.front();
		dlBuffer.pop();
		if (nextchk.data != nullptr)
		{
			free(nextchk.data);
			nextchk.data = nullptr;
			nextchk.size = 0;
		}
		else
		{
			spdlog::info("Dummy exit");
			runningFlag = false;
			break;
		}
	}

	return 0;
}

int extract_init()
{
	runningFlag = true;
	extractThread = std::thread(extract_main);
	return 0;
}

bool getExtractStatus()
{
	return runningFlag;
}

int extract_deinit()
{
	runningFlag = false;
	extractThread.join();
	return 0;
}
