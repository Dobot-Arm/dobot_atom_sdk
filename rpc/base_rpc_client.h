#ifndef __BASE_RPC_CLIENT_H__
#define __BASE_RPC_CLIENT_H__

#include <iostream>
#include <string>
#include "nlohmann/json.hpp"

namespace Atom
{

    // Define the error code enumeration
    enum RpcErrorCode {
        SUCCESS = 0,
        SOCKET_CREATION_FAILED = 1,
        CONNECT_FAILED = 2,
        NO_RESPONSE = 3,
        JSON_PARSE_ERROR = 4,
        UNEXPECTED_RESPONSE = 5,
        SERVER_INTERNAL_ERROR = 6,
        FSM_TRANSITION_FAILED = 100,
        SWITCH_UPPER_LIMB_CONTROL_FAILED = 101,
        SET_VEL_FAILED = 102
    };

    class BaseRpcClient
    {
    public:
        BaseRpcClient(const std::string &ip, int port) : ip_(ip), port_(port) {}
        virtual ~BaseRpcClient() = default;

    protected:
        RpcErrorCode SendRequest(const std::string &method, const nlohmann::json &params, nlohmann::json &response);

    protected:
        std::string ip_;
        int port_;
    };

}  // namespace Atom

#endif  // __BASE_RPC_CLIENT_H__
