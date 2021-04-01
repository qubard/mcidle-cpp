#pragma once

#include <array>
#include <string>

namespace mcidle {
namespace util {

    std::string Base64Decode(const std::string &message);

#define SHA1_DIGEST_LENGTH 20

    /**
 * Calculates the two's complement of a sha1 digest.
 * @param digest The original sha1 digest.
 * @return an array that contains the two's complement of the digest.
 */
    std::array<unsigned char, SHA1_DIGEST_LENGTH> Sha1TwosComplement(
        const unsigned char *digest);

    /**
 * Gets the hex representation of a sha1 digest.
 * Uses two's complement for negative digests, and removes leading zeros.
 * @param digest The raw sha1 digest
 * @return a string containing the hex digest
 */
    std::string Sha1HexDigest(const unsigned char *digest);
    bool Sha1DigestTest();

}  // namespace util
}  // namespace mcidle
