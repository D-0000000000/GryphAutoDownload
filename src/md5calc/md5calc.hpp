#ifndef _MD5CALC_CPP_
#define _MD5CALC_CPP_

#include <fstream>
#include <openssl/evp.h>
#include <string.h>
#include <string>

struct MD5Calc
{
	EVP_MD_CTX *ctx;
	uint8_t digest[EVP_MAX_MD_SIZE];
	uint32_t mdlen;
	uint64_t file_size;

	MD5Calc();

	~MD5Calc();

	void reinit();

	void md5Digest(void *rawData, uint64_t size);

	bool checkDigest(uint8_t *dig, uint32_t dig_len);

	bool checkDigest(std::string, uint32_t dig_len);
};

#endif