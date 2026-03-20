#include "common/common_typedefs.hpp"
#include "data_queue/data_queue.hpp"
#include "filedl/filedl.hpp"

#include <boost/lockfree/spsc_queue.hpp>

#include <nlohmann/json.hpp>

#include <spdlog/spdlog.h>

#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>

struct DLTask
{
	std::string url;
	std::string md5;
	uint64_t fileSize;
};

std::queue<DLTask> que;
std::thread downloadThread;
static bool runningFlag;

int download_main()
{
	filedl_init();

	while (runningFlag && !que.empty())
	{
		auto front = que.front();
		que.pop();
		MemChunk chunk;
		chunk.data = nullptr;
		chunk.size = 0;
		spdlog::info("{}", front.url);
		int res = filedl_download(front.url, chunk);
		spdlog::info("Push to extract");
		while (!dlBuffer.write_available())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		dlBuffer.push(chunk);
		spdlog::info("Pushed");
	}

	filedl_deinit();
	runningFlag = false;

	while (!dlBuffer.write_available())
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	MemChunk chunk;
	chunk.data = nullptr;
	chunk.size = 0;
	dlBuffer.push(chunk);

	spdlog::info("Dummy pushed");
	spdlog::info("Download finished");

	return 0;
}

int download_init(std::string manifest)
{
	runningFlag = true;
	nlohmann::json manifestjson = nlohmann::json::parse(manifest);
	auto packs = manifestjson["pkg"]["packs"];
	for (auto &package : packs)
	{
		DLTask dlt;
		dlt.url = package["url"];
		dlt.md5 = package["md5"];
		std::string strsize = package["package_size"];
		dlt.fileSize = std::stoll(strsize);
		que.push(dlt);
	}

	downloadThread = std::thread(download_main);

	return 0;
}

bool getDownloadStatus()
{
	return runningFlag;
}

int download_deinit()
{
	runningFlag = false;
	downloadThread.join();
	return 0;
}
