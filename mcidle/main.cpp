#include <iostream>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/tuple/tuple.hpp>

#include <bitset>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <vector>
#include <memory>

#include <networking/TCPSocket.hpp>
#include <networking/connection/Connection.hpp>
#include <networking/connection/SConnection.hpp>

#include <networking/ByteBuffer.hpp>
#include <networking/types/VarInt.hpp>

#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>

#include <networking/packet/serverbound/EncryptionResponse.hpp>
#include <networking/encryption/AesCtx.hpp>
#include <networking/protocol/Protocol_1_12_2.hpp>

#include <util/Yggdrasil.hpp>

#include <zlib.h>

#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include <curl/curl.h>

#include <networking/packet/clientbound/KeepAlive.hpp>
#include <networking/packet/serverbound/KeepAlive.hpp>

#include <MCIdle.hpp>

#include <boost/thread.hpp>

#include <networking/Proxy.hpp>

#include <common/Rsa.hpp>

std::unordered_map<std::string, std::vector<s32>> chunks;

int main(int argc, char* argv[]) 
{
    auto protocol = std::make_shared<mcidle::Protocol_1_12_2_CB>(340);
    std::string serverName = "localhost";
    bool ONLINE_MODE = false;

    mcidle::util::Yggdrasil yg;

	std::string accessTkn = "eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJjYjRkNjNhOGMwMjQ4NGMxODJkZWE5MzRkNmQ2ZjM1OSIsInlnZ3QiOiIwNWFkOWEwYTE5ZjU0ZTJjYmFhYzQ0MWZjYmY3MDE2YSIsInNwciI6ImNjNzgwODNkN2E3ZTQwY2E5YWJiN2VkZmQyZTAxMzgzIiwiaXNzIjoiWWdnZHJhc2lsLUF1dGgiLCJleHAiOjE1OTEyNTkzMTQsImlhdCI6MTU5MTA4NjUxNH0.pUndDszGrD3iTL73Yx6NkqFCqzZZebGOR4Psjm8FbGg";

    if (ONLINE_MODE)
    {
        try {
            if (yg.Authenticate("qubard@gmail.com", ""))
            {
                std::cout << yg.AccessToken() << "\n";
                std::cout << yg.ClientToken() << "\n";
            }
        }
        catch (mcidle::util::YggdrasilException e) {
            printf("Fail to auth\n");
            return -1;
        }
        //yg.SetAccessToken("eyJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJjYjRkNjNhOGMwMjQ4NGMxODJkZWE5MzRkNmQ2ZjM1OSIsInlnZ3QiOiI5NmQ1MjE5NGExNjc0ZTY3ODQwMWQyZGM1MWFjYzZiNCIsInNwciI6ImNjNzgwODNkN2E3ZTQwY2E5YWJiN2VkZmQyZTAxMzgzIiwiaXNzIjoiWWdnZHJhc2lsLUF1dGgiLCJleHAiOjE1OTU2OTUzMTAsImlhdCI6MTU5NTUyMjUxMH0.BV5X5SNCzmxRWWufeDlrRGfvma7kQTUj94EJdjA2R70");
        //yg.SetProfileId("cc78083d7a7e40ca9abb7edfd2e01383");

        std::cout << yg.AccessToken() << "\n";
        std::cout << "profile id:" << yg.ProfileId() << "\n";
    }

    mcidle::MCIdle mc(false, "localhost", protocol, nullptr, yg);

    std::cout << "Started in " << (ONLINE_MODE ? "ONLINE" : "OFFLINE") << " mode!\n";

    // Bug: if a packet isn't sent and a readpacket call fails everything fails
    if (!mc.Start()) printf("Failed to start mcidle.\n");

    return 0;
}
