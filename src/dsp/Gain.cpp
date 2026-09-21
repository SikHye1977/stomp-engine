#include "stomp/dsp/Gain.hpp"

namespace stomp::dsp {

Gain::Gain(float gain)
    : Effect("gain", "Gain"),
      gain_(
          addParameter(
              "gain",
              "Gain",
              0.0f,
              2.0f,
              1.0f
          )
      )
{
    gain_.setValue(gain);
}

void Gain::prepare(double, std::size_t)
{
    // Gain은 sample rate나 block size에 의존하지 않는다.
}

void Gain::processBlock(
    const float* input,
    float* output,
    std::size_t numFrames
)
{
    const float gain = gain_.getValue();

    for (std::size_t i = 0; i < numFrames; ++i) {
        output[i] = input[i] * gain;
    }
}

void Gain::reset()
{
    Effect::reset();
}

void Gain::setGain(float gain)
{
    gain_.setValue(gain);
}

float Gain::getGain() const
{
    return gain_.getValue();
}

Parameter& Gain::getGainParameter()
{
    return gain_;
}

const Parameter& Gain::getGainParameter() const
{
    return gain_;
}

} // namespace stomp::dsp
