#ifndef __ALGS_RPC_CLIENT_H__
#define __ALGS_RPC_CLIENT_H__

#include "rpc/base_rpc_client.h"

namespace Atom
{

    class AlgsRpcClient : public BaseRpcClient
    {
    public:
        AlgsRpcClient(const std::string &ip = "192.168.8.234", int port = 51234) : BaseRpcClient(ip, port) {}
        RpcErrorCode SwitchUpperLimbControl(bool is_on);
        RpcErrorCode GetFsmId(int32_t &fsm_id);
        RpcErrorCode SetFsmId(int32_t fsm_id);
        RpcErrorCode SetVel(float vx, float vy, float vyaw, float duration = 1.0);  // vx：前后运动速度，向前为正，单位m/s；vy：左右运动速度，向左为正，单位m/s；
                                                                                    // vyaw:旋转速度，向逆时针为正，单位rad / s；duration：速度指令持续时间，单位s。
    };

}  // namespace Atom

#endif  // __ALGS_RPC_CLIENT_H__
