#include "data_queue/data_queue.hpp"
#include "filedl/filedl.hpp"
#include "jobs_main/jobs_main.hpp"

#include <curl/curl.h>

#include <spdlog/spdlog.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		spdlog::info("[manifest url]");
		return 0;
	}

	spdlog::info("Manifest url: {}", argv[1]);

	filedl_init();
	MemChunk manifestChunk;
	manifestChunk.data = nullptr;
	manifestChunk.size = 0;
	int res = filedl_download(std::string(argv[1]), manifestChunk);
	filedl_deinit();

	if (res != 0)
	{
		return 0;
	}

	std::string manifeststr(manifestChunk.data, manifestChunk.data + manifestChunk.size);

	download_init(manifeststr);
	extract_init();

	while (getDownloadStatus() || getExtractStatus())
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	download_deinit();
	extract_deinit();

	return 0;
}
