#include "common/bridge.h"
#include "common/timer.h"
#include "nlohmann/json.hpp"

#include <fstream>
#include <iostream>
#include <string>

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

    // Read PID parameters from the JSON file.
    std::ifstream jsonfile("../config/controlParams.json");
    if (!jsonfile.is_open()) {
        std::cerr << "Unable to open PID file!" << std::endl;
        return 1;
    }
    nlohmann::json controlParams;
    jsonfile >> controlParams;

    // Read the joint angle trajectory from a text file.
    // This trajectory defines the desired motion of the robot's joints.
    std::ifstream file("../config/joint_angles.txt");
    if (!file.is_open()) {
        std::cerr << "Unable to open Angle track file" << std::endl;
        return 1;
    }
    std::vector<std::vector<double>> q_val = {};
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<double> row;
        double value;
        while (iss >> value) { row.push_back(value); }
        q_val.push_back(row);
    }
    file.close();
    if (q_val.empty() || q_val[0].size() < static_cast<size_t>(Atom::kLegDofs + Atom::kArmDofs)) {  // Check if q_val has enough data
        std::cerr << "Insufficient data in joint_angles.txt" << std::endl;
        return 1;
    }

    // Initialize the bridge for communication and the command structures for legs, hands, and arms.
    Atom::Bridge bridge;
    Atom::LegCommand leg_command;
    Atom::HandCommand hand_command;
    Atom::ArmCommand arm_command;

    // Define timing parameters for the control loop.
    // The control loop will run at a frequency of 1/control_dt.
    const float init_duration = 5.0;  // time unit: s
    const float control_dt = 0.01;    // time unit: s
    const int s2us = 1e6;
    float time = 0.0;
    Timer timer;

    // Set the initial position for the robot's arms and legs.
    // The robot will move to this position at the beginning of the program.
    Eigen::Vector<float, Atom::kArmDofs> q_arm_init;
    Eigen::Vector<float, Atom::kLegDofs> q_leg_init;
    q_leg_init.setZero();
    q_arm_init.setZero();
    for (int i = 0; i < Atom::kLegDofs; ++i) { q_leg_init[i] = q_val[0][i]; }
    for (int i = 0; i < Atom::kArmDofs; ++i) { q_arm_init[i] = q_val[0][i + Atom::kLegDofs]; }
    Eigen::Vector<float, Atom::kLegDofs> q_leg_ref;
    Eigen::Vector<float, Atom::kArmDofs> q_arm_ref;

    int row = 0;

    // Main control loop.
    while (1) {
        // Get the current joint states from the robot.
        const std::shared_ptr<const Atom::JointState> js_tmp_ptr = bridge.GetNewestLegJointStatePtr();      // leg
        const std::shared_ptr<const Atom::ArmJointState> ajs_tmp_ptr = bridge.GetNewestArmJointStatePtr();  // arm
        timer.Tic();

        if (js_tmp_ptr && ajs_tmp_ptr) {
            time += control_dt;
            if (time < init_duration) {
                // Interpolate to the initial position over a specified duration.
                // This ensures a smooth start for the robot's motion.
                q_leg_ref = (q_leg_init - js_tmp_ptr->q) * time / init_duration + js_tmp_ptr->q;
                q_arm_ref = (q_arm_init - ajs_tmp_ptr->q) * time / init_duration + ajs_tmp_ptr->q;
            } else {
                // Follow the predefined trajectory from the file.
                for (int i = 0; i < Atom::kLegDofs; ++i) { q_leg_ref[i] = q_val[row][i]; }
                for (int i = 0; i < Atom::kArmDofs; ++i) { q_arm_ref[i] = q_val[row][i + Atom::kLegDofs]; }
                row++;
                if (row == q_val.size() - 1) { row = 0; }  // Loop back to the beginning of the trajectory.
            }
            // Set the PID gains for the legs from the configuration file.
            for (int j = 0; j < Atom::kLegDofs; j++) {
                leg_command.kp(j) = controlParams[j]["kp"].get<double>();
                leg_command.kd(j) = controlParams[j]["kd"].get<double>();
            }
            // Set the PID gains for the arms from the configuration file.
            for (int j = 0; j < Atom::kArmDofs; j++) {
                arm_command.kp(j) = controlParams[j + Atom::kLegDofs]["kp"].get<double>();
                arm_command.kd(j) = controlParams[j + Atom::kLegDofs]["kd"].get<double>();
            }

            // Set and send the commands to the robot.
            leg_command.q_ref = q_leg_ref;
            leg_command.dq_ref.setZero();
            leg_command.tau_forward.setZero();
            bridge.SetNewestLegCommand(leg_command);

            hand_command.q_ref.setZero();
            hand_command.dq_ref.setZero();
            bridge.SetNewestHandCommand(hand_command);

            arm_command.q_ref = q_arm_ref;
            arm_command.dq_ref.setZero();
            arm_command.tau_forward.setZero();
            bridge.SetNewestArmCommand(arm_command);

            // Output the target joint angles for the arm for debugging purposes.
            std::cout << "motor_state_q: [";
            for (size_t i = 0; i < arm_command.q_ref.size(); ++i) {
                if (i > 0) { std::cout << ", "; }
                std::cout << arm_command.q_ref[i];
            }
            std::cout << "]" << std::endl;
        }

        // Wait for the next control cycle.
        timer.Toc();
        timer.usDelay(control_dt * s2us - timer.usDuration());
    }

    return 0;
}