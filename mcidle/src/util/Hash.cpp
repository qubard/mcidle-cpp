#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <util/Hash.hpp>
#include <vector>

// Source: mclib

namespace mcidle
{
namespace util
{

std::string Base64Decode(const std::string & message)
{
	BIO * bmem;
	BIO * b64;

	b64 = BIO_new(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	bmem = BIO_new_mem_buf((void *)&message[0], message.length());
	BIO_push(b64, bmem);

	std::string output;
	output.resize(message.length());
	std::size_t i = BIO_read(b64, &output[0], output.size());
	output.resize(i);
	BIO_free_all(b64);

	return output;
}

std::array<unsigned char, SHA_DIGEST_LENGTH>
Sha1TwosComplement(const unsigned char * digest)
{
	std::array<unsigned char, SHA_DIGEST_LENGTH> ret;

	bool carry = false;
	auto end = ret.end();
	auto last = end - 1;

	for (auto begin = ret.begin(); begin != end; ++begin, ++digest)
	{
		unsigned char & current = *begin;
		current = ~*digest;
		if ((begin == last) || carry)
		{
			carry = (current == std::numeric_limits<unsigned char>::max());
			if (carry)
				current = 0;
			else
				++current;
		}
	}

	return ret;
}

std::string Sha1HexDigest(const unsigned char * digest)
{
	std::string new_digest((char *)digest, SHA_DIGEST_LENGTH);
	std::stringstream ss;

	bool negative = (new_digest[0] & (1 << 7)) != 0;

	if (negative)
	{
		auto arr = Sha1TwosComplement(digest);
		for (std::size_t i = 0; i < SHA_DIGEST_LENGTH; ++i)
			new_digest[i] = arr[i];
	}

	for (std::size_t i = 0; i < SHA_DIGEST_LENGTH; ++i)
		ss << std::hex << std::setfill('0') << std::setw(2)
		   << (int)(new_digest[i] & 0xFF);

	std::string result = ss.str();
	std::size_t pos = 0;
	while (result[pos] == '0')
		pos++;
	if (result[0] == '0')
		result = result.substr(pos);

	if (negative)
		result = '-' + result;
	return result;
}

}  // namespace util
}  // namespace mcidle