#ifndef BRIDGE_H
#define BRIDGE_H

#include <iostream>
#include <algorithm>
#include <eigen3/unsupported/Eigen/EulerAngles>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "lower_cmd.h"
#include "lower_state.h"
#include "hands_cmd.h"
#include "hands_state.h"
#include "upper_cmd.h"
#include "upper_state.h"
#include "set_fsm_id.h"
#include "main_nodes_state.h"
#include "clear_errors.h"
#include "amr_cmd.h"
#include "amr_state.h"
#include "enable_motors.h"
#include "emergency_state.h"
#include "remote_control.h"
#include "common/data_buffer.h"
#include "common/joystick.h"
#include "common/motor_command.h"
#include "common/yaml_parser.h"
#include "dds/dds.h"
// #include "log/logger.h"

#define MAX_WRITE_TOPICS 7
#define MAX_READ_TOPICS 7
namespace Atom
{
    class Bridge
    {
    public:
        Bridge();
        ~Bridge();

        /* Command */
        void SetNewestLegCommand(const LegCommand &motor_command);
        void SetNewestHandCommand(const HandCommand &hand_command);
        void SetNewestArmCommand(const ArmCommand &arm_command);
        void SetNewestAmrCommand(const dobot_atom_msg_dds__AMRCommand_ &amr_command);
        void SetNewestFsmCommand(const int &fsm_id, const std::string &trajectory_file_name = "");
        void SetLiftMotorsEnabled(bool enabled);
        bool GetLiftMotorsEnabled();
        void ClearErrors();  // 清除错误

        /* State */
        const std::shared_ptr<const BaseState> GetNewestBaseStatePtr() { return base_state_buffer_.GetData(); }
        const std::shared_ptr<const JointState> GetNewestLegJointStatePtr() { return joint_state_buffer_.GetData(); }
        const std::shared_ptr<const ArmJointState> GetNewestArmJointStatePtr() { return arm_state_buffer_.GetData(); }
        const std::shared_ptr<const DexterousHandState> GetNewestHandJointStatePtr() { return hand_state_buffer_.GetData(); }
        const std::shared_ptr<const RemoteCommand> GetNewestRemoteCommandPtr() { return remote_command_buffer_.GetData(); }
        const std::shared_ptr<const MainNodesStateStruct> GetNewestMainNodeStatePtr() { return main_node_state_buffer_.GetData(); }
        const std::shared_ptr<const AtomRobotType> GetRobotTypePtr() { return robot_type_buffer_.GetData(); }
        const std::shared_ptr<const BmsStateStruct> GetNewestBmsStatePtr() { return bms_state_buffer_.GetData(); }
        const std::shared_ptr<const dobot_atom_msg_dds__AMRState_> GetNewestAmrStatePtr() { return amr_state_buffer_.GetData(); }
        const std::shared_ptr<const dobot_atom_msg_dds__EmergencyState_> GetNewestEmergencyStatePtr() { return emergency_state_buffer_.GetData(); }

        const std::shared_ptr<const LegCommand> GetLegCommandPtr() { return motor_command_.GetData(); }
        const std::shared_ptr<const ArmCommand> GetArmCommandPtr() { return arm_command_.GetData(); }
        const std::shared_ptr<const Remote_Velocity> GetRemoteVelCommandPtr() { return remote_velocity_buffer_.GetData(); }

    private:
        void ListenOperation(void);
        void PublishOperation(void);
        void DexterousHandsHandler(const void *message);
        void HandStateRecorder(const dobot_atom_msg_dds__HandsState_ &dds_low_state);
        void EmergencyStateHandler(const void *message);

        DataBuffer<BaseState> base_state_buffer_;
        DataBuffer<JointState> joint_state_buffer_;
        DataBuffer<BmsStateStruct> bms_state_buffer_;
        DataBuffer<ArmJointState> arm_state_buffer_;
        DataBuffer<Remote_Velocity> remote_velocity_buffer_;
        DataBuffer<DexterousHandState> hand_state_buffer_;
        DataBuffer<RemoteCommand> remote_command_buffer_;
        DataBuffer<MainNodesStateStruct> main_node_state_buffer_;
        DataBuffer<LegCommand> motor_command_;
        DataBuffer<HandCommand> hand_command_;
        DataBuffer<ArmCommand> arm_command_;
        dobot_atom_msg_dds__HandsCmd_ last_hand_command_;  // 上一次下发的hand命令，用于NaN防护
        PackOneLowCommand last_low_command_;               // 上一次下发的下肢命令，用于NaN防护
        PackOneUpCommand last_up_command_;                 // 上一次下发的上肢命令，用于NaN防护
        DataBuffer<dobot_atom_msg_dds__AMRCommand_> amr_command_;
        DataBuffer<dobot_atom_msg_dds__AMRState_> amr_state_buffer_;
        DataBuffer<dobot_atom_msg_dds__LowerState_> loco_sdk_buffer_;
        DataBuffer<dobot_atom_msg_dds__UpperState_> arm_sdk_buffer_;
        DataBuffer<dobot_atom_msg_dds__EmergencyState_> emergency_state_buffer_;
        DataBuffer<dobot_atom_msg_dds__SetFsmId_> fsm_id_;  // 缓存状态机ID和轨迹文件名称
        DataBuffer<AtomRobotType> robot_type_buffer_;
        void LowStateHandler(const void *message);
        void LocoSDKHandler(const void *message);
        void ArmStateHandler(const void *message);
        void AmrStateHandler(const void *message);
        void RemoteVelHandler(const void *message);
        void MainNodesStateHandler(const void *message);
        void SendNewestLegCommand();
        void SendNewestHandCommand();
        void SendNewestArmCommand();
        void SendNewestAmrCommand();
        // other parameters from yaml
        void ReadParameters();
        bool send_motor_command_;
        bool send_hand_command_;
        bool send_arm_command_;
        bool send_amr_command_;

        void LowCmd2Dds(const PackOneLowCommand &pack_one_motor_command, dobot_atom_msg_dds__LowerCmd_ &dds_motor_command);
        void UpCmd2Dds(const PackOneUpCommand &pack_one_low_command, dobot_atom_msg_dds__UpperCmd_ &dds_motor_command);
        void JointStateRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void ArmStateRecorder(const dobot_atom_msg_dds__UpperState_ &dds_upper_state);
        void MainNodesState(const dobot_atom_msg_dds__MainNodesState_ &dds_main_node_state);
        void BaseStateRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void JoystickRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void JoystickCombiner(xRockerBtnDataStruct &remote_key_data, xRockerBtnDataStruct &sdk_key_data);
 
        enum { ALG_REC_LEG_COND = 1, DEXTEROUS_HANDS = 2, ALG_REC_ARMS_COND = 3, MAIN_NODES_STATE = 4, AMR_STATE_COND = 5, REMOTE_VEL = 6, EMERGENCY_STATE_COND = 7};

        std::thread leg_command_thread_;
        std::atomic<bool> leg_command_running_;
        std::chrono::microseconds loop_duration_{2000};  // 2ms

        // dds parameters
        dds_return_t rc;
        dds_entity_t participant;
        dds_entity_t topic;
        dds_entity_t writer_leg;
        dds_entity_t writer_hand;
        dds_entity_t writer_arm;
        dds_entity_t writer_fsm;
        dds_entity_t writer_clear_errors_;
        dds_entity_t writer_amr_;
        dds_entity_t writer_enable_motors_;
        dds_qos_t *qos;
        dds_entity_t publisher;
        dds_entity_t reader;
        void *samples[1];
        dds_sample_info_t infos[1];
        // thread
        std::thread listenerThread_;
        bool listenerStarted_ = false;
        std::mutex mutex_;
        bool isInitialized = false;
        bool isLegInitialized = false;
        bool isArmInitialized = false;
        std::atomic<bool> running_{true};
        std::condition_variable cv;
        bool all_topics_initialized_printed_ = false;  // 标记是否已打印所有话题初始化完成
    };
}  // namespace Atom

#endif  // BRIDGE_H
