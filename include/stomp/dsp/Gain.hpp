#pragma once

#include "stomp/dsp/Effect.hpp"

namespace stomp::dsp {

class Gain final : public Effect {
public:
    explicit Gain(float gain = 1.0f);

    void prepare(double sampleRate, std::size_t blockSize) override;

    void reset() override;

    void setGain(float gain);
    float getGain() const;

protected:
    void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) override;

private:
    float gain_;
};

} // namespace stomp::dsp
