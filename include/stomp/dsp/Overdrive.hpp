#pragma once

#include "stomp/dsp/Effect.hpp"

namespace stomp::dsp {

class Overdrive final : public Effect {
public:
    Overdrive(
        float drive = 0.5f,
        float tone = 0.5f,
        float level = 0.5f
    );

    void prepare(
        double sampleRate,
        std::size_t blockSize
    ) override;

    void reset() override;

    void setDrive(float value);
    void setTone(float value);
    void setLevel(float value);

    float getDrive() const;
    float getTone() const;
    float getLevel() const;

protected:
    void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) override;

private:
    void updateToneCoefficient();

    Parameter& drive_;
    Parameter& tone_;
    Parameter& level_;

    double sampleRate_ = 48000.0;

    float toneState_ = 0.0f;
    float toneCoefficient_ = 0.0f;
};

} // namespace stomp::dsp
