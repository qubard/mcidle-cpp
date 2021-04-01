#include <iostream>
#include <string>

#include <boost/algorithm/hex.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/tuple/tuple.hpp>

#include <bitset>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <memory>
#include <vector>

#include <networking/TCPSocket.hpp>
#include <networking/connection/Connection.hpp>
#include <networking/connection/SConnection.hpp>

#include <networking/ByteBuffer.hpp>
#include <networking/types/VarInt.hpp>

#include <networking/packet/serverbound/Handshake.hpp>
#include <networking/packet/serverbound/LoginStart.hpp>

#include <networking/encryption/AesCtx.hpp>
#include <networking/packet/serverbound/EncryptionResponse.hpp>
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

const std::string loadVariable(const char *name)
{
    if (auto env = std::getenv(name))
        return std::string(env);
    return "";
}

bool IsNumber(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

int main(int argc, char *argv[])
{
    auto protocol = std::make_shared<mcidle::Protocol_1_12_2_CB>(340);
    bool ONLINE_MODE = loadVariable("MC_ONLINE") != "";

    mcidle::util::Yggdrasil yg;

    std::string accessTkn = "";

    auto USER = loadVariable("MC_USER");
    auto PASS = loadVariable("MC_PASS");
    auto IP = loadVariable("MC_IP");
    auto PORT = loadVariable("MC_PORT");

    if (!IsNumber(PORT))
    {
        PORT = "25565";
    }

    std::cout << USER << ":" << PASS << ":" << IP << ":" << PORT << "\n";

    if (ONLINE_MODE)
    {
        try
        {
            if (yg.Authenticate(USER, PASS))
            {
                std::cout << "AccessToken:" << yg.AccessToken() << "\n";
                std::cout << "ClientToken:" << yg.ClientToken() << "\n";
            }
            else
            {
                printf("Fail to auth\n");
                return -1;
            }
        }
        catch (mcidle::util::YggdrasilException e)
        {
            printf("Fail to auth\n");
            return -1;
        }

        std::cout << yg.AccessToken() << "\n";
        std::cout << "profile id:" << yg.ProfileId() << "\n";
    }

    mcidle::MCIdle mc(ONLINE_MODE, IP, PORT, protocol, nullptr, yg);

    std::cout << "Started in " << (ONLINE_MODE ? "ONLINE" : "OFFLINE") << " mode!\n";

    // Bug: if a packet isn't sent and a readpacket call fails everything fails
    if (!mc.Start())
        printf("Failed to start mcidle.\n");

    return 0;
}
