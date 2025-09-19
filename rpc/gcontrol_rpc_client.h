#ifndef __GCONTROL_RPC_CLIENT_H__
#define __GCONTROL_RPC_CLIENT_H__

#include "base_rpc_client.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Atom
{

class GcontrolRpcClient : public BaseRpcClient
{
public:
    GcontrolRpcClient(const std::string& ip = "192.168.8.234", int port = 51235) : BaseRpcClient(ip, port)
    {
    }
    RpcErrorCode GetDevState(int32_t& devstate);
    RpcErrorCode SetVolume(int32_t volume, bool& result);
    RpcErrorCode GetVolume(int32_t& volume);
    RpcErrorCode PlayAudio(bool& result);
    RpcErrorCode StopAudio(bool& result);
    RpcErrorCode PauseAudio(bool& result);
    RpcErrorCode ContinueAudio(bool& result);

    // UDP 客户端
    bool InitUdpSender(const std::string& ip, uint16_t port = 51236);
    bool SendStartLoad();
    bool SendStopLoad();
    bool SendAudioData(const uint8_t* data, size_t len);
    void CloseUdpSender();
    bool ReceiveUdpAck();

    RpcErrorCode Pc2SwitchTTS(bool flag, bool& result);
    RpcErrorCode Pc2PlayTTS(const std::string text, bool& result);
    RpcErrorCode Pc2SwitchASR(bool flag, bool& result);
    RpcErrorCode Pc2PlayASR(const std::string language, uint32_t timeout, std::string& result);
    RpcErrorCode Pc2PlayDify(const std::string text, std::string& result);
    RpcErrorCode Pc1MicTTS(const std::string wavPath, const std::string text, bool& result);
    RpcErrorCode Pc1MicServer(bool flag, bool& result);
    RpcErrorCode Pc1MicIVW(const uint32_t timeout, std::string& result);
    RpcErrorCode Pc1MicESR(const uint32_t timeout, std::string& result);

private:
    int m_udpSockFd = -1;
    sockaddr_in m_udpServAddr{};
    bool m_udpInitialized = false;
};

}    // namespace Atom

#endif    // __GCONTROL_RPC_CLIENT_H__