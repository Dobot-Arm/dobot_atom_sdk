#ifndef COMMON_MOTOR_COMMAND_
#define COMMON_MOTOR_COMMAND_

#include <eigen3/Eigen/Core>

#include "robot_state.h"

namespace Atom
{
    struct LegCommand {
        Eigen::Matrix<float, Atom::kLegDofs, 1> q_ref = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();        // angle
        Eigen::Matrix<float, Atom::kLegDofs, 1> dq_ref = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();       // velocity
        Eigen::Matrix<float, Atom::kLegDofs, 1> kp = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();           // position stiffness coefficients
        Eigen::Matrix<float, Atom::kLegDofs, 1> kd = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();           // velocity stiffness coefficients
        Eigen::Matrix<float, Atom::kLegDofs, 1> tau_forward = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();  // forward torque
    };

    struct HandCommand {
        Eigen::Matrix<float, Atom::kNumHands, 1> q_ref = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
        Eigen::Matrix<float, Atom::kNumHands, 1> dq_ref = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
        Eigen::Matrix<float, Atom::kNumHands, 1> kp = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
        Eigen::Matrix<float, Atom::kNumHands, 1> kd = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
        Eigen::Matrix<float, Atom::kNumHands, 1> tau_forward = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
    };

    struct ArmCommand {
        Eigen::Matrix<float, Atom::kArmDofs, 1> q_ref = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> dq_ref = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> kp = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> kd = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> tau_forward = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
    };

#pragma pack(1)

    struct PackOneBmsCmd {
        uint8_t off = 0x00;
        std::array<uint8_t, 3> reserve;
    };

    // motor control
    struct PackOneMotorCommand {
        // desired working mode
        uint8_t mode = 0x01;
        // desired angle (unit: radian)
        float q = 0;
        // desired velocity (unit: radian/second)
        float dq = 0;
        // desired output torque (unit: N.m)
        float tau = 0;
        // desired position stiffness (unit: N.m/rad )
        float kp = 0;
        // desired velocity stiffness (unit: N.m/(rad/s) )
        float kd = 0;
        std::array<uint32_t, 3> reserve = {0};
    };

    // low level control
    struct PackOneLowCommand {
        uint8_t head[2];
        uint8_t level_flag;
        uint8_t frame_reserve;
        uint32_t sn[2];
        uint32_t version[2];
        uint16_t bandwidth;
        // struct PackOneMotorCommand motor_cmd[20];
        std::array<PackOneMotorCommand, 12> motor_command;
        std::array<uint8_t, 40> wireless_remote;
        struct PackOneMotorCommand bms_cmd;
        // uint8_t wireless_remote[40];
        uint8_t led[12];
        uint8_t fan[2];
        uint8_t gpio;
        uint32_t reserve;
        uint32_t crc;
    };

    struct PackOneUpCommand {
        uint8_t head[2];
        uint8_t level_flag;
        uint8_t frame_reserve;
        uint32_t sn[2];
        uint32_t version[2];
        uint16_t bandwidth;
        // struct PackOneMotorCommand motor_cmd[20];
        std::array<PackOneMotorCommand, 17> motor_command;
        std::array<uint8_t, 40> wireless_remote;
        struct PackOneMotorCommand bms_cmd;
        // uint8_t wireless_remote[40];
        uint8_t led[12];
        uint8_t fan[2];
        uint8_t gpio;
        uint32_t reserve;
        uint32_t crc;
    };

#pragma pack()

}  // namespace Atom

#endif  // COMMON_MOTOR_COMMAND_
