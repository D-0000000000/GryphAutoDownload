#include "md5calc.hpp"
#include <cctype>
#include <cstdlib>
#include <openssl/evp.h>
#include <openssl/md5.h>

MD5Calc::MD5Calc()
{
	ctx = EVP_MD_CTX_new();
	memset(digest, 0, sizeof(digest));
	mdlen = 0;
	EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
}

MD5Calc::~MD5Calc()
{
	EVP_MD_CTX_free(ctx);
}

void MD5Calc::reinit()
{
	if (ctx != nullptr)
	{
		EVP_MD_CTX_free(ctx);
	}
	ctx = EVP_MD_CTX_new();
	memset(digest, 0, sizeof(digest));
	mdlen = 0;
	EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
	return;
}

void MD5Calc::md5Digest(void *rawData, uint64_t size)
{

	EVP_DigestUpdate(ctx, rawData, size);

	EVP_DigestFinal(ctx, digest, &mdlen);
	return;
}

bool MD5Calc::checkDigest(uint8_t *dig, uint32_t dig_len)
{
	return memcmp(dig, digest, dig_len) == 0;
}

bool MD5Calc::checkDigest(std::string dig, uint32_t dig_len)
{
	uint8_t cdig[EVP_MAX_MD_SIZE];
	if (dig.length() & 1)
	{
		return false;
	}
	for (int i = 0; i < dig.length(); i += 2)
	{
		char digstr[3];
		digstr[0] = dig[i];
		digstr[1] = dig[i + 1];
		digstr[2] = 0;
		cdig[i >> 1] = strtol(digstr, NULL, 16);
	}
	return memcmp(cdig, digest, dig_len) == 0;
}