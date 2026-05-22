#ifndef __ALGS_RPC_CLIENT_H__
#define __ALGS_RPC_CLIENT_H__

#include "rpc/base_rpc_client.h"
#include <array>
#include <map>
#include <string>

namespace Atom
{
    class AlgsRpcClient : public BaseRpcClient
    {
    public:
        AlgsRpcClient(const std::string &ip = "192.168.8.234", int port = 51234) : BaseRpcClient(ip, port) {}
        RpcErrorCode SwitchUpperLimbControl(bool is_on);
        RpcErrorCode GetFsmId(int32_t &fsm_id);
        RpcErrorCode SetFsmId(int32_t fsm_id);
        // 轮式没有左右运动的自由度，vy设置不生效
        RpcErrorCode SetVel(float vx, float vy, float vyaw, float duration = 1.0);  // vx：前后运动速度，向前为正，单位m/s；vy：左右运动速度，向左为正，单位m/s；
                                                                                    // vyaw:旋转速度，向逆时针为正，单位rad / s；duration：速度指令持续时间，单位s。
        RpcErrorCode TriggerTrajectoryRecording(bool is_on, std::string *file_path = nullptr);
        RpcErrorCode ReloadArmSkillsData();
        RpcErrorCode SendActionMapping(const std::map<std::string, std::string> &action_mapping);  // 发送动作映射数据，格式：按键名称 -> 技能名称
        RpcErrorCode executeAction(const std::string &file_path);
        RpcErrorCode SetControlMode(int32_t control_mode);  // 设置控制模式：0-手柄控制，1-APP控制，2-第三方控制
        RpcErrorCode SetPolicyControl(bool mode);           // 设置运动模式，true表示运动模式，false表示非运动模式
    };

}  // namespace Atom

#endif  // __ALGS_RPC_CLIENT_H__
