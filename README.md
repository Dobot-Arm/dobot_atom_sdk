English | [中文](README_zh-CN.md)

## 📋 Content

- [Dobot Atom SDK User Manual](#dobot-atom-sdk-user-manual)
  - [**🖥** Build Environment](#-Build Environment)
  - [📦 Compilation](#-compilation)
  - [🔎 Package Structure Analysis](#-package-structure-analysis)
  - [🔧 Configuration](#-configuration)
    - [1. Network Connection](#1-network-connection)
    - [2. DDS Configuration](#2-dds-configuration)
  - [🎮 Sample Program Execution](#-sample-program-execution)
    - [1. Run the High-Level RPC Sample](#1-run-the-high-level-rpc-sample)
    - [2. Introduction to Main Functions in the RPC Sample](#2-introduction-to-main-functions-in-the-rpc-sample)
    - [3. Run the Low-Level DDS Sample](#3-run-the-low-level-dds-sample)
    - [4. Introduction to Main Functions in the DDS Sample](#4-introduction-to-main-functions-in-the-dds-sample)
    - [5. Run the Joint Recording Sample](#5-run-the-joint-recording-sample)
    - [6. Run the Joystick Control Sample](#6-run-the-joystick-control-sample)

# Dobot Atom SDK User Manual

This manual provides a comprehensive guide for the Dobot Atom Robot SDK, covering **compilation, package structure analysis, configuration, and sample program execution**.

## **🖥** Build Environment

- **OS:** Ubuntu 20.04 LTS
- **Compiler:** GCC 9.4.0

## 📦 Compilation

It is assumed that the working directory is `/home/atom/workspace`, and users can modify this path according to their actual situation.

```bash
cd /home/atom/workspace/dobot_atom_sdk
mkdir build
cd build
cmake ..
make
```

**Successful Compilation:**

Executable files such as `rpc_test` (high-level RPC sample), `bridge_test` (low-level DDS sample), and `record_joint` (joint recording sample) will be generated in the `build` directory.

## 🔎 Package Structure Analysis

| **Directory Path**   | **Core Content & Function**                                                                                                                                                                                                                                              |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `dobot_atom_sdk/common`  | Common header files:<br />Support the basic functions of all sample programs.                                                                                                                                                                                                  |
| `dobot_atom_sdk/config`  | Configuration file directory:<br />- `cyclonedds.xml`: DDS communication configuration; <br />- `controlParams.json`: Joint PID control parameters; <br />- `joint_angles.txt`: Joint trajectory file (for low-level DDS samples).                                       |
| `dobot_atom_sdk/example` | Sample code directory:<br />- `rpc_test.cpp`: High-level RPC control (FSM switching, speed control); <br />- `bridge_test.cpp`: Low-level DDS control (joint trajectory tracking); <br />- `record_joint.cpp`: Joint angle recording (generates joint trajectory files). |
| `dobot_atom_sdk/idl`     | DDS Interface Definition Files (IDL):<br />e.g. , `bms_cmd.h` (Battery Management System header file), `main_nodes_state.h` (Robot node status header file).                                                                                                              |
| `dobot_atom_sdk/rpc`     | RPC client interface directory:<br />- `base_rpc_client.h`: Basic RPC communication logic; <br />- `algs_rpc_client.h`: Motion control RPC interface; <br />- `gcontrol_rpc_client.h`: Voice/device control RPC interface;                                               |

## 🔧 Configuration

### 1. Network Connection

Connect one end of an Ethernet cable to the robot and the other end to your computer. Open the terminal and execute the ping command:

```bash
ping 192.168.8.234
```

If the following information appears, the connection is successful:

```bash
64 bytes from 192.168.8.234: icmp_seq=1 ttl=64 time=0.567 ms
64 bytes from 192.168.8.234: icmp_seq=2 ttl=64 time=0.612 ms
```

### 2. DDS Configuration

Enter `ip addr show` in the terminal to query the network card name. As shown in the figure below, the network card name is `enp8s0`.

![ip_addr_show.jpg](./images/ip_addr_show.jpg)

Open the `dobot_atom_sdk/config/cyclonedds.xml` file and replace `eth0` with the queried network card name `enp8s0`.

## 🎮 Sample Program Execution

In the `dobot_atom_sdk/build` folder, `rpc_test` is the high-level RPC sample, and `bridge_test` is the low-level DDS sample. For detailed introductions to the samples, please refer to the *Software Service Interface* document.

> ⚠️ Warning: The robot will move when running the following samples. Ensure the environment is safe before execution.

### 1. Run the High-Level RPC Sample

**Function**:

Control robot FSM state switching (e.g., Standby → Walking) and speed control (moving forward, backward, left, right) through command-line interaction.

```bash
cd build
./rpc_test
```

**Interactive Operations**:

Enter 1 → Enter FSM state control: First displays the current FSM ID, then enter the target ID;

Enter 2 → Enter speed control: You must first switch to an FSM state that supports speed control (e.g., 301 or 302), otherwise it will prompt “SetVel failed. Please enter fsm 301 or 302.”

### 2. Introduction to main functions in the sample program

```c++
// Set whether to enable upper-limb control: is_on = True enables upper-limb control, is_on = False disables upper-limb control.
rpc.SwitchUpperLimbControl(bool is_on); 

// Used to get the state machine state
rpc.GetFsmId(int32_t &fsm_id); 

// Used to set the state machine state
rpc.SetFsmId(int32_t fsm_id); 

// Send speed command to the robot, vx: forward/backward movement speed, positive for forward, unit m/s; vy: left/right movement speed, positive for left, unit m/s;
// vyaw: rotation speed, positive for counterclockwise, unit rad/s; duration: duration of the speed command, unit s.
rpc.SetVel(float vx, float vy, float vyaw, float duration = 1.0);  
```

### 3. Run the Low-Level DDS Sample

> ⚠️ Warning: Ensure the robot has been switched to debug mode before running the following sample.

**Function**:

Read the preset trajectory `config/joint_angles.txt` and control the robot joints to track the preset trajectory via DDS communication.

```bash
cd build
./bridge_test
```

### 4. Introduction to main functions in the sample program

Main functions of the Atom::Bridge class for data acquisition:

```c++
// Used to get lower limb joint status (including busVoltage, q, dq, tau) (The wheeled lifting structure joints are J1, J2, and J3 in order from the chassis, corresponding to the first three joints in sequence)
bridge.GetNewestLegJointStatePtr()  

// Used to get upper limb joint status (including busVoltage, q, dq, tau)
bridge.GetNewestArmJointStatePtr()  

// Used to get dexterous hand status (only including q)
bridge.GetNewestHandJointStatePtr()  

// Used to get IMU status (including qua, rpy, vel, w)
bridge.GetNewestBaseStatePtr()  

// Used to get handle commands (such as button_A_, button_X_, lin_vel, yaw_vel, etc.)
bridge.GetNewestRemoteCommandPtr()  


// Function usage, taking getting dexterous hand joint positions as an example:
bridge.GetNewestHandStatePtr()->q; 
```

Main functions of the Atom::Bridge class for sending motion commands:

```c++
// Used to send lower limb motion commands (The wheeled lifting structure joints are J1, J2, and J3 in order from the chassis, corresponding to the first three joints in sequence)
bridge.SetNewestLegCommand(const LegCommand &motor_command)  

// Used to send upper limb motion commands
bridge.SetNewestArmCommand(const ArmCommand &arm_command)  

// Used to send dexterous hand motion commands
bridge.SetNewestHandCommand(const HandCommand &hand_command)  

// Used to set the state machine state
bridge.SetNewestFsmCommand(const int &fsm_id)  


// Function usage, taking issuing lower limb control commands as an example:
Atom::LegCommand leg_command;               //Create a new leg_command of LegCommand structure, see motor_command.h for structure definition
leg_command.q_ref = 0.5;                     //Set target joint position，here we set 0.5 as an example
leg_command.dq_ref.setZero();               //Set joint angular velocity
leg_command.tau_forward.setZero();          //Set feedforward torque
bridge.SetNewestLegCommand(leg_command);    //Issue lower limb control commands
```

Main functions of the Atom::Bridge class for chassis control:

```c++
// Used to get chassis status (including position, device_status, navigation_status, etc.)
bridge.GetNewestAmrStatePtr()

// Used to send chassis motion commands
bridge.SetNewestAmrCommand(const dobot_atom_msg_dds__AMRCommand_ &amr_command)


// Function usage, taking sending chassis control commands as an example:
dobot_atom_msg_dds__AMRCommand_ amr_cmd;             // Create a new amr_cmd of AMRCommand_ structure, see amr_cmd.idl for structure definition
amr_cmd.command_type = dobot_atom_msg_dds__AMRCommandType::dobot_atom_msg_dds__REMOTE_CONTROL; // Set command type to remote control
amr_cmd.linear_vel = 0.5;                            // Set target linear velocity, here we set 0.5 as an example
amr_cmd.angular_vel = 0.2;                           // Set target angular velocity, here we set 0.2 as an example
bridge.SetNewestAmrCommand(amr_cmd);                 // Issue chassis control commands
```

### 5. Run the Joint Recording Sample

**Function**:

Collect robot joint angles, save them to `joint_angles.txt`, and automatically append a “reverse trajectory” (to restore joints to the initial position). The generated file can be used by `bridge_test`.

```bash
cd build
./record_joint
```

**Recording Process**:

No trajectory will be recorded within 5 seconds after the program starts; you can manually adjust the robot joints to the initial position;

Recording starts after 5 seconds, and the terminal will display the current recording time;

After recording is completed, `joint_angles.txt` will be generated in the `build` directory. You can copy it to the `config` directory to replace the original file for use by `bridge_test`.

### 6. Run the Joystick Control Sample

**Function**:

Use the remote control joystick to control the movement of the chassis.

```bash
cd build
./joystick_control_amr
```

**Execution**:

After running the program, you can use the joysticks on the remote control to control the linear and angular velocity of the chassis. The program will print the sent commands and the current position of the chassis.
