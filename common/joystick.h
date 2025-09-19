#ifndef COMMON_JOYSTICK_
#define COMMON_JOYSTICK_

#include <stdint.h>
#include <eigen3/Eigen/Core>
// 16b
typedef union {
    struct {
        uint8_t R1 : 1;
        uint8_t L1 : 1;
        uint8_t start : 1;
        uint8_t select : 1;
        uint8_t R2 : 1;
        uint8_t L2 : 1;
        uint8_t F1 : 1;
        uint8_t F2 : 1;
        uint8_t A : 1;
        uint8_t B : 1;
        uint8_t X : 1;
        uint8_t Y : 1;
        uint8_t up : 1;
        uint8_t right : 1;
        uint8_t down : 1;
        uint8_t left : 1;
    } components;
    uint16_t value;
} xKeySwitchUnion;

// 40 Byte (now used 24B)
typedef struct {
    uint8_t head[2];
    xKeySwitchUnion btn;
    float lx;
    float rx;
    float ry;
    float ly;
    float L2;

    uint8_t idle[16];
} xRockerBtnDataStruct;

struct RemoteCommand {
    double pos_incre = 0;  //位置递增

    Eigen::Vector2f lin_vel = Eigen::Vector2f::Zero();
    float yaw_vel = 0.0;

    bool button_START_ = false;
    bool button_SELECT_ = false;

    bool button_L1A_ = false;
    bool button_L1B_ = false;
    bool button_L1X_ = false;
    bool button_L1Y_ = false;

    bool button_L2A_ = false;
    bool button_L2B_ = false;
    bool button_L2X_ = false;
    bool button_L2Y_ = false;

    bool button_R1A_ = false;
    bool button_R1B_ = false;
    bool button_R1X_ = false;
    bool button_R1Y_ = false;

    bool button_R2A_ = false;
    bool button_R2B_ = false;
    bool button_R2X_ = false;
    bool button_R2Y_ = false;

    bool button_L1U_ = false;
    bool button_L1D_ = false;
    bool button_L1L_ = false;
    bool button_L1R_ = false;

    bool button_L2U_ = false;
    bool button_L2D_ = false;
    bool button_L2L_ = false;
    bool button_L2R_ = false;

    bool button_R1U_ = false;
    bool button_R1D_ = false;
    bool button_R1R_ = false;
    bool button_R1L_ = false;

    bool button_R2U_ = false;
    bool button_R2D_ = false;
    bool button_R2L_ = false;
    bool button_R2R_ = false;

    bool button_L2R2_ = false;

    bool button_UP_ = false;
    bool button_DOWN_ = false;
    bool button_LEFT_ = false;
    bool button_RIGHT_ = false;

    bool button_L1_ = false;
    bool button_L2_ = false;
    bool button_R1_ = false;
    bool button_R2_ = false;

    bool button_A_ = false;
    bool button_B_ = false;
    bool button_X_ = false;
    bool button_Y_ = false;
};

#endif  // COMMON_JOYSTICK_
