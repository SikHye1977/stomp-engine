#pragma once

#include "stomp/dsp/Effect.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace stomp::dsp {

class EffectChain {
public:
    void addEffect(std::unique_ptr<Effect> effect);

    void prepare(double sampleRate, std::size_t blockSize);

    void process(
        const float* input,
        float* output,
        std::size_t numFrames
    );

    void reset();

    std::size_t size() const;

private:
    std::vector<std::unique_ptr<Effect>> effects_;

    std::vector<float> bufferA_;
    std::vector<float> bufferB_;
};

} // namespace stomp::dsp
