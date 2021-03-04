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
#include <cstdlib>

std::unordered_map<std::string, std::vector<s32>> chunks;

const std::string loadVariable(const char* name)
{
    if(auto env = std::getenv(name))
        return std::string(env);
    return "";
}

int main(int argc, char* argv[]) 
{
    auto protocol = std::make_shared<mcidle::Protocol_1_12_2_CB>(340);
    std::string serverAddr = "connect.2b2t.org.";
    bool ONLINE_MODE = true;

    mcidle::util::Yggdrasil yg;

	std::string accessTkn = "";

    auto USER = loadVariable("MCIDLE_USER");
    auto PASS = loadVariable("MCIDLE_PASS");

    if (ONLINE_MODE)
    {
        try {
            if (yg.Authenticate(USER, PASS))
            {
                std::cout << "AccessToken:" << yg.AccessToken() << "\n";
                std::cout << "ClientToken:" << yg.ClientToken() << "\n";
            } else {
                printf("Fail to auth\n");
                return -1;
            }
        }
        catch (mcidle::util::YggdrasilException e) {
            printf("Fail to auth\n");
            return -1;
        }

        std::cout << yg.AccessToken() << "\n";
        std::cout << "profile id:" << yg.ProfileId() << "\n";
    }

    mcidle::MCIdle mc(ONLINE_MODE, serverAddr, 25565, protocol, nullptr, yg);

    std::cout << "Started in " << (ONLINE_MODE ? "ONLINE" : "OFFLINE") << " mode!\n";

    // Bug: if a packet isn't sent and a readpacket call fails everything fails
    if (!mc.Start()) printf("Failed to start mcidle.\n");

    return 0;
}
