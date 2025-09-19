#include "rpc/algs_rpc_client.h"
#include <iostream>
#include <thread>

int main()
{
    // Initialize the RPC client for communication with the robot's algorithm server.
    Atom::AlgsRpcClient rpc;
    int sw = 0;

    while (true) {
        // Main loop to interact with the user and send commands to the robot.
        if (sw == 0) {
            // Prompt the user to select a command.
            std::cout << "Enter: 1 = FSM, 2 = SetVel." << std::endl;
            std::cin >> sw;
            if (std::cin.fail()) {
                std::cin.clear();                                                    // Clear error flags
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
                std::cout << "Invalid input. Please enter a number." << std::endl;
                continue;
            }
            std::cout << "Test Start..." << std::endl;
        }
        switch (sw) {
            case 1: { // Finite State Machine (FSM) control
                int32_t fsm_id = 0;
                rpc.GetFsmId(fsm_id);
                std::cout << "Enter number for fsm_id: ";
                std::cin >> fsm_id;
                // Set the FSM ID to change the robot's state.
                Atom::RpcErrorCode errorCode = rpc.SetFsmId(fsm_id);
                if (errorCode != Atom::RpcErrorCode::SUCCESS) { std::cout << "SetFsmId errorCode: " << errorCode << std::endl; }
                std::this_thread::sleep_for(std::chrono::seconds(2));
                break;
            }
            case 2: { // Velocity control
                int32_t fsm_id = 0;
                rpc.GetFsmId(fsm_id);
                // Check if the robot is in a state that allows velocity control.
                if (301 != fsm_id && 302 != fsm_id) {
                    std::cout << "SetVel failed. Please enter fsm 301 or 302." << std::endl;
                    sw = 0;
                    break;
                }
                const float kVel = 0.3;     // Desired velocity.
                const float duration = 2.0; // Duration of the velocity command.
                // Send the velocity command to the robot.
                Atom::RpcErrorCode errorCode = rpc.SetVel(kVel, 0.0, 0.0, duration);
                if (errorCode != Atom::RpcErrorCode::SUCCESS) { std::cout << "\033[31mSetVel failed.\033[0m" << std::endl; }
                sw = 0;
                break;
            }
            default:
                std::cout << "not support" << std::endl;
                sw = 0;
                break;
        }
    }
    return 0;
}
