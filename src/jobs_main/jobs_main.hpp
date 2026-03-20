#ifndef _JOBS_INIT_HPP_
#define _JOBS_INIT_HPP_

int download_init(std::string manifest);

bool getDownloadStatus();

int download_deinit();

int extract_init();

bool getExtractStatus();

int extract_deinit();

#endif
