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
#include "common/data_buffer.h"
#include "common/joystick.h"
#include "common/motor_command.h"
#include "common/dexterous_hand.h"
#include "common/yaml_parser.h"
#include "dds/dds.h"
// #include "log/logger.h"

#define MAX_WRITE_TOPICS 5
#define MAX_READ_TOPICS 4
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
        void SetNewestFsmCommand(const int &fsm_id);
        void ClearErrors();  // 清除错误

        /* State */
        const std::shared_ptr<const BaseState> GetNewestBaseStatePtr() { return base_state_buffer_.GetData(); }
        const std::shared_ptr<const JointState> GetNewestJointStatePtr() { return joint_state_buffer_.GetData(); }
        const std::shared_ptr<const ArmJointState> GetNewestArmStatePtr() { return arm_state_buffer_.GetData(); }
        const std::shared_ptr<const DexterousHandState> GetNewestHandStatePtr() { return hand_state_buffer_.GetData(); }
        const std::shared_ptr<const RemoteCommand> GetNewestRemoteCommandPtr() { return remote_command_buffer_.GetData(); }
        const std::shared_ptr<const MainNodesStateStruct> GetNewestMainNodeStatePtr() { return main_node_state_buffer_.GetData(); }
        const std::shared_ptr<const BmsStateStruct> GetNewestBmsStatePtr() { return bms_state_buffer_.GetData(); }

        const std::shared_ptr<const LegCommand> GetLegCommandPtr() { return motor_command_.GetData(); }
        const std::shared_ptr<const ArmCommand> GetArmCommandPtr() { return arm_command_.GetData(); }

    private:
        void ListenOperation(void);
        void PublishOperation(void);
        void DexterousHandsHandler(const void *message);
        void HandStateRecorder(const dobot_atom_msg_dds__HandsState_ &dds_low_state);

        DataBuffer<BaseState> base_state_buffer_;
        DataBuffer<JointState> joint_state_buffer_;
        DataBuffer<BmsStateStruct> bms_state_buffer_;
        DataBuffer<ArmJointState> arm_state_buffer_;
        DataBuffer<DexterousHandState> hand_state_buffer_;
        DataBuffer<RemoteCommand> remote_command_buffer_;
        DataBuffer<MainNodesStateStruct> main_node_state_buffer_;
        DataBuffer<LegCommand> motor_command_;
        DataBuffer<HandCommand> hand_command_;
        DataBuffer<ArmCommand> arm_command_;
        DataBuffer<dobot_atom_msg_dds__LowerState_> loco_sdk_buffer_;
        DataBuffer<dobot_atom_msg_dds__UpperState_> arm_sdk_buffer_;
        DataBuffer<int> fsm_id_;
        void LowStateHandler(const void *message);
        void LocoSDKHandler(const void *message);
        void ArmStateHandler(const void *message);
        void MainNodesStateHandler(const void *message);
        void SendNewestLegCommand();
        void SendNewestHandCommand();
        void SendNewestArmCommand();
        // other parameters from yaml
        void ReadParameters();
        bool send_motor_command_;
        bool send_hand_command_;
        bool send_arm_command_;

        void LowCmd2Dds(const PackOneLowCommand &pack_one_motor_command, dobot_atom_msg_dds__LowerCmd_ &dds_motor_command);
        void UpCmd2Dds(const PackOneUpCommand &pack_one_low_command, dobot_atom_msg_dds__UpperCmd_ &dds_motor_command);
        void JointStateRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void ArmStateRecorder(const dobot_atom_msg_dds__UpperState_ &dds_upper_state);
        void MainNodesState(const dobot_atom_msg_dds__MainNodesState_ &dds_main_node_state);
        void BaseStateRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void JoystickRecorder(const dobot_atom_msg_dds__LowerState_ &dds_low_state);
        void JoystickCombiner(xRockerBtnDataStruct &remote_key_data, xRockerBtnDataStruct &sdk_key_data);

        enum { ALG_REC_LEG_COND = 1, DEXTEROUS_HANDS = 2, ALG_REC_ARMS_COND = 3, MAIN_NODES_STATE = 4 };

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
    };
}  // namespace Atom

#endif  // BRIDGE_H
