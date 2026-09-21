#include "stomp/dsp/Gain.hpp"

namespace stomp::dsp {

Gain::Gain(float gain)
    : gain_(gain)
{
}

void Gain::prepare(double, std::size_t)
{
    // Gain은 현재 sample rate나 block size에
    // 의존하지 않으므로 할 일이 없다.
}

void Gain::process(
    const float* input,
    float* output,
    std::size_t numFrames
)
{
    for (std::size_t i = 0; i < numFrames; ++i) {
        output[i] = input[i] * gain_;
    }
}

void Gain::reset()
{
    // Gain은 내부 상태를 가지지 않는다.
}

void Gain::setGain(float gain)
{
    gain_ = gain;
}

float Gain::getGain() const
{
    return gain_;
}

} // namespace stomp::dsp
