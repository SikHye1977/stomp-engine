#pragma once

#include <cstddef>

namespace stomp::dsp {

class Effect {
public:
    virtual ~Effect() = default;

    virtual void prepare(double sampleRate, std::size_t blockSize) = 0;

    virtual void process(
        const float* input,
        float* output,
        std::size_t numFrames
    ) = 0;

    virtual void reset() = 0;
};

} // namespace stomp::dsp
