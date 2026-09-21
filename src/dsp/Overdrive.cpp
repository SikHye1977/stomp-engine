#include "stomp/dsp/Overdrive.hpp"

#include <algorithm>
#include <cmath>

namespace stomp::dsp {

namespace {

constexpr float kPi = 3.14159265358979323846f;

}

Overdrive::Overdrive(
    float drive,
    float tone,
    float level
)
    : Effect("overdrive", "Overdrive"),
      drive_(
          addParameter(
              "drive",
              "Drive",
              0.0f,
              1.0f,
              0.5f
          )
      ),
      tone_(
          addParameter(
              "tone",
              "Tone",
              0.0f,
              1.0f,
              0.5f
          )
      ),
      level_(
          addParameter(
              "level",
              "Level",
              0.0f,
              1.0f,
              0.5f
          )
      )
{
    drive_.setValue(drive);
    tone_.setValue(tone);
    level_.setValue(level);

    updateToneCoefficient();
}

void Overdrive::prepare(
    double sampleRate,
    std::size_t
)
{
    sampleRate_ = sampleRate;

    updateToneCoefficient();

    toneState_ = 0.0f;
}

void Overdrive::reset()
{
    Effect::reset();

    toneState_ = 0.0f;

    updateToneCoefficient();
}

void Overdrive::setDrive(float value)
{
    drive_.setValue(value);
}

void Overdrive::setTone(float value)
{
    tone_.setValue(value);

    updateToneCoefficient();
}

void Overdrive::setLevel(float value)
{
    level_.setValue(value);
}

float Overdrive::getDrive() const
{
    return drive_.getValue();
}

float Overdrive::getTone() const
{
    return tone_.getValue();
}

float Overdrive::getLevel() const
{
    return level_.getValue();
}

void Overdrive::updateToneCoefficient()
{
    // Tone 0.0 -> approximately 500 Hz
    // Tone 1.0 -> approximately 8000 Hz
    const float cutoff =
        500.0f + tone_.getValue() * 7500.0f;

    const float safeCutoff = std::min(
        cutoff,
        static_cast<float>(sampleRate_ * 0.45)
    );

    toneCoefficient_ =
        1.0f -
        std::exp(
            -2.0f *
            kPi *
            safeCutoff /
            static_cast<float>(sampleRate_)
        );
}

void Overdrive::processBlock(
    const float* input,
    float* output,
    std::size_t numFrames
)
{
    const float driveGain =
        1.0f + drive_.getValue() * 19.0f;

    // 0.5 = unity gain.
    const float outputGain =
        level_.getValue() * 2.0f;

    for (std::size_t i = 0; i < numFrames; ++i) {

        // Pre-gain
        const float driven =
            input[i] * driveGain;

        // Soft clipping
        const float clipped =
            std::tanh(driven);

        // Simple one-pole low-pass tone control
        toneState_ +=
            toneCoefficient_ *
            (clipped - toneState_);

        // Output level
        output[i] =
            toneState_ * outputGain;
    }
}

} // namespace stomp::dsp
