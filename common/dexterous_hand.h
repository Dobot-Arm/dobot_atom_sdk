#ifndef COMMON_DEXTEROUS_HAND_
#define COMMON_DEXTEROUS_HAND_
#include <eigen3/Eigen/Core>
#include "robot_state.h"

struct DexterousHandState {
    Eigen::Matrix<float, Atom::kNumHands, 1> q = Eigen::Matrix<float, Atom::kNumHands, 1>::Zero();
};

#endif  // COMMON_DEXTEROUS_HAND_