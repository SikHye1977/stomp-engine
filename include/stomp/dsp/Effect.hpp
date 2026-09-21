#pragma once

#include <algorithm>
#include <cstddef>

namespace stomp::dsp {

class Effect {
public:
    virtual ~Effect() = default;

    virtual void prepare(double sampleRate, std::size_t blockSize) = 0;

    void process(
        const float* input,
        float* output,
        std::size_t numFrames
    )
    {
        if (bypassed_) {
            std::copy(input, input + numFrames, output);
            return;
        }

        processBlock(input, output, numFrames);
    }

    virtual void reset() = 0;

    void setBypassed(bool bypassed)
    {
        bypassed_ = bypassed;
    }

    bool isBypassed() const
    {
        return bypassed_;
    }

protected:
    virtual void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) = 0;

private:
    bool bypassed_ = false;
};

} // namespace stomp::dsp
