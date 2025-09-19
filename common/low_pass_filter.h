#ifndef COMMON_LOW_PASS_FILTER_
#define COMMON_LOW_PASS_FILTER_

#include <eigen3/Eigen/Core>
#include "common/constants.h"

namespace Atom
{
#include <vector>

    template <typename T>
    class LowPassFilter
    {
    public:
        LowPassFilter(double sample_rate, double cutoff_frequency)
        {
            sample_rate_ = sample_rate;
            cutoff_frequency_ = cutoff_frequency;
            alpha_ = CalculateAlpha();
            initialized_ = false;
        }

        T Update(T input)
        {
            if (!initialized_) {
                output_ = input;
                initialized_ = true;
            } else {
                output_ = alpha_ * input + (1 - alpha_) * output_;
            }
            return output_;
        }

        void SetCutoffFrequency(double cutoff_frequency)
        {
            cutoff_frequency_ = cutoff_frequency;
            alpha_ = CalculateAlpha();
        }

        void Reset() { initialized_ = false; }

    private:
        double sample_rate_;
        double cutoff_frequency_;
        double alpha_;
        T output_;
        bool initialized_;

        double CalculateAlpha()
        {
            double dt = 1.0 / sample_rate_;
            double rc = 1.0 / (math::k2Pi * cutoff_frequency_);
            return dt / (dt + rc);
        }
    };

}  // namespace Atom

#endif  // COMMON_LOW_PASS_FILTER_
