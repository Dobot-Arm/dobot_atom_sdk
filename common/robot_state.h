#ifndef COMMON_BASE_STATE_
#define COMMON_BASE_STATE_

#include <eigen3/Eigen/Core>

namespace Atom
{
    constexpr int kLegDofs = 12;
    constexpr int kArmDofs = 17;
    constexpr int kNumNotUsedJoint = 1;
    constexpr int kDofs = kLegDofs + kArmDofs + kNumNotUsedJoint;

    constexpr int kNumHands = 12;  // hand

    enum JointIndex {
        // Left leg
        kLeftHipPitch = 0,
        kLeftHipRoll = 1,
        kLeftHipYaw = 2,
        kLeftKnee = 3,
        kLeftAnklePitch = 4,
        kLeftAnkleRoll = 5,
        // Right leg
        kRightHipPitch = 6,
        kRightHipRoll = 7,
        kRightHipYaw = 8,
        kRightKnee = 9,
        kRightAnklePitch = 10,
        kRightAnkleRoll = 11,
        // torso
        kWaistYaw = 12,
        // Right arm
        kLeftShoulderPitch = 13,
        kLeftShoulderRoll = 14,
        kLeftShoulderYaw = 15,
        kLeftElbowPitch = 16,
        kLeftElbowRoll = 17,
        kLeftWristPitch = 18,
        kLeftWristRoll = 19,
        // Left arm
        kRightShoulderPitch = 20,
        kRightShoulderRoll = 21,
        kRightShoulderYaw = 22,
        kRightElbowPitch = 23,
        kRightElbowRoll = 24,
        kRightWristPitch = 25,
        kRightWristRoll = 26,
        // Head
        kHeadPitch = 27,
        kHeadRoll = 28,
        // Not used dof
        kNotUsedJoint = 29,

    };

    const char *GetJointIndexString(JointIndex joint_index);
    std::ostream &operator<<(std::ostream &out, JointIndex joint_index);

    // base state of Atom iw.r.t global frame
    struct BaseState {
        Eigen::Vector3f pos = Eigen::Vector3f::Zero();
        Eigen::Matrix3f R = Eigen::Matrix3f::Identity();
        Eigen::Vector3f rpy = Eigen::Vector3f::Zero();
        Eigen::Vector3f vel = Eigen::Vector3f::Zero();
        Eigen::Vector3f w = Eigen::Vector3f::Zero();
        Eigen::Vector4f qua = Eigen::Vector4f::Zero();
    };

    struct JointState {
        Eigen::Matrix<float, Atom::kLegDofs, 1> q = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();           // angle
        Eigen::Matrix<float, Atom::kLegDofs, 1> dq = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();          // velocity
        Eigen::Matrix<float, Atom::kLegDofs, 1> tau = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();         // torque
        Eigen::Matrix<float, Atom::kLegDofs, 1> busVoltage = Eigen::Matrix<float, Atom::kLegDofs, 1>::Zero();  // busVoltage
    };

    struct ArmJointState {
        Eigen::Matrix<float, Atom::kArmDofs, 1> q = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> dq = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> tau = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
        Eigen::Matrix<float, Atom::kArmDofs, 1> busVoltage = Eigen::Matrix<float, Atom::kArmDofs, 1>::Zero();
    };

    struct MainNodesStateStruct {
        Eigen::Matrix<int, Atom::kDofs, 1> errorCode = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
        Eigen::Matrix<int, Atom::kDofs, 1> nodeState = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
        Eigen::Matrix<int, Atom::kDofs, 1> servoState = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
        Eigen::Matrix<int, Atom::kDofs, 1> pos_err_code = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
        Eigen::Matrix<int, Atom::kDofs, 1> vel_err_code = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
        Eigen::Matrix<int, Atom::kDofs, 1> torque_err_code = Eigen::Matrix<int, Atom::kDofs, 1>::Zero();
    };

    struct BmsStateStruct {
        uint16_t bms_state;                // BMS状态
        uint16_t battery_level;            // 电池电量百分比
        uint16_t battery_health;           // 电池健康度
        uint16_t pcb_board_temp;           // PCB板温度
        uint16_t cells_voltage[16];        // 16个电芯电压
        uint16_t battery_pack_io_voltage;  // 电池包放电/充电接口的电压
    };
}  // namespace Atom

#endif  // COMMON_BASE_STATE_
