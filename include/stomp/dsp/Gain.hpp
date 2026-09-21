#pragma once

#include "stomp/dsp/Effect.hpp"
#include "stomp/dsp/Parameter.hpp"

namespace stomp::dsp {

class Gain final : public Effect {
public:
    explicit Gain(float gain = 1.0f);

    void prepare(double sampleRate, std::size_t blockSize) override;

    void reset() override;

    void setGain(float gain);
    float getGain() const;

    Parameter& getGainParameter();
    const Parameter& getGainParameter() const;

protected:
    void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) override;

private:
    Parameter gain_;
};

} // namespace stomp::dsp
