#include "common/bridge.h"
#include "common/timer.h"

#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
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

    // Initialize the bridge for communication with the robot.
    Atom::Bridge bridge;

    // Open a file to store the recorded joint angles.
    std::ofstream file("joint_angles.txt");
    if (!file.is_open()) {
        std::cerr << "Failed to open joint_angles.txt" << std::endl;
        return 1;
    }

    // Define timing parameters for the recording loop.
    const float init_duration = 5.0;  // Start recording after this many seconds.
    const float end_duration = 20.0;  // Stop recording after this many seconds.
    const float control_dt = 0.01;    // Time step for each recording.
    const int s2us = 1e6;             // Conversion factor from seconds to microseconds.
    double time = 0.0;
    Timer timer;
    std::vector<Eigen::VectorXf> q_ref;
    constexpr int kRecordDofs = Atom::kLegDofs + Atom::kArmDofs;

    // Main loop to record joint angles.
    while (1) {
        // Get the latest joint states for the arm and legs.
        const std::shared_ptr<const Atom::ArmJointState> ajs_tmp_ptr = bridge.GetNewestArmStatePtr();
        const std::shared_ptr<const Atom::JointState> js_tmp_ptr = bridge.GetNewestJointStatePtr();
        if (nullptr == ajs_tmp_ptr || nullptr == js_tmp_ptr) {
            std::cerr << "Failed to get joint state ptr" << std::endl;
            return 1;
        }
        timer.Tic();
        time += control_dt;

        // Start recording after the initial duration and before the end duration.
        if (time > init_duration && time < end_duration) {
            std::cout << "start time: " << init_duration << ", end time: " << end_duration << ", current time: " << time << std::endl;
            Eigen::VectorXf q_current(kRecordDofs);
            // Combine leg and arm joint angles into a single vector.
            for (size_t i = 0; i < kRecordDofs; ++i) {
                if (i < Atom::kLegDofs) {
                    q_current[i] = js_tmp_ptr->q[i];
                } else {
                    q_current[i] = ajs_tmp_ptr->q[i - Atom::kLegDofs];
                }
                file << q_current[i];
                if (i < kRecordDofs) file << " ";
            }
            q_ref.push_back(q_current);
            file << "\n";
        }
        timer.Toc();
        // Wait for the next recording cycle.
        timer.usDelay(control_dt * s2us - timer.usDuration());
        if (time > end_duration) { break; }
    }

    // Append the recorded trajectory in reverse order to the file.
    // This creates a motion that returns to the starting position.
    for (auto it = q_ref.rbegin(); it != q_ref.rend(); ++it) {
        for (size_t i = 0; i < kRecordDofs; ++i) {
            file << (*it)[i];
            if (i < kRecordDofs) file << " ";
        }
        file << "\n";
    }
    file.close();
    std::cout << "Done ==========" << std::endl;
    return 0;
}