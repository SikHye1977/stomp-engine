#pragma once

#include "stomp/dsp/Effect.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace stomp::dsp {

class EffectChain {
public:
    void addEffect(std::unique_ptr<Effect> effect);

    Effect& getEffect(const std::string& id);
    const Effect& getEffect(const std::string& id) const;

    std::size_t getEffectCount() const;

    void prepare(double sampleRate, std::size_t blockSize);

    void process(
        const float* input,
        float* output,
        std::size_t numFrames
    );

    void reset();

    // 기존 코드와의 호환성을 위해 유지한다.
    std::size_t size() const;

private:
    std::vector<std::unique_ptr<Effect>> effects_;

    std::vector<float> bufferA_;
    std::vector<float> bufferB_;
};

} // namespace stomp::dsp
