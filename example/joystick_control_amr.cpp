#include <unistd.h>
#include <csignal>
#include "common/bridge.h"
#include "common/joystick.h"
#include "common/robot_state.h"
#include "amr_cmd.h"
#include "amr_state.h"

volatile sig_atomic_t g_running = 1;

void sigint_handler(int sig) { g_running = 0; }

int main(int argc, char **argv)
{
    // Set the path to the CycloneDDS configuration file.
    // This is necessary to ensure that the DDS communication is properly configured.
#ifdef _SET_CYCLONEDDS_URI
    const char *config_file = "../config/cyclonedds.xml";
    if (setenv("CYCLONEDDS_URI", config_file, 1) != 0) {
        std::cout << "setenv CYCLONEDDS_URI failed" << std::endl;
    } else {
        std::cout << "CYCLONEDDS_URI set to config_file: " << config_file << std::endl;
        std::cout << "Please refer to the configuration file description to ensure that the network interface name in the configuration file is consistent with the network card name connected to the "
                     "robot."
                  << std::endl;
    }
#endif

    Atom::Bridge bridge;
    sleep(1);
    
    printf("bridge init done\n");
    dobot_atom_msg_dds__AMRCommand_ amr_cmd;
    memset(&amr_cmd, 0, sizeof(amr_cmd));
    amr_cmd.command_type = dobot_atom_msg_dds__AMRCommandType::dobot_atom_msg_dds__START_REMOTE;
    bridge.SetNewestAmrCommand(amr_cmd);
    usleep(30000);

    printf("Start remote control AMR\n");
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);
    const double max_linear_vel = 0.6; // m/s
    const double max_angular_vel = 1; // rad/s
    while (g_running) {
        auto rc_ptr = bridge.GetNewestRemoteCommandPtr();
        if (rc_ptr) {
            amr_cmd.command_type = dobot_atom_msg_dds__AMRCommandType::dobot_atom_msg_dds__REMOTE_CONTROL;
            amr_cmd.linear_vel = rc_ptr->lin_vel.x() * max_linear_vel;
            amr_cmd.angular_vel = rc_ptr->yaw_vel * max_angular_vel;
            bridge.SetNewestAmrCommand(amr_cmd);

            printf("Sending AMR command: linear_vel=%.2f, angular_vel=%.2f\n", amr_cmd.linear_vel, amr_cmd.angular_vel);
        }

        auto amr_state_ptr = bridge.GetNewestAmrStatePtr();
        if (amr_state_ptr) { printf("AMR state: x=%.2f, y=%.2f, yaw=%.2f\n", amr_state_ptr->position[0], amr_state_ptr->position[1], amr_state_ptr->position[2]); }

        usleep(20000);  // 50Hz
    }

    amr_cmd.command_type = dobot_atom_msg_dds__AMRCommandType::dobot_atom_msg_dds__STOP_REMOTE;
    amr_cmd.linear_vel = 0;
    amr_cmd.angular_vel = 0;
    bridge.SetNewestAmrCommand(amr_cmd);
    printf("Stop remote control AMR\n");

    return 0;
}
