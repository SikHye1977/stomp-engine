#include "stomp/dsp/EffectChain.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

namespace {

bool nearlyEqual(float a, float b, float epsilon = 1e-6f)
{
    return std::fabs(a - b) < epsilon;
}

class TestGain final : public stomp::dsp::Effect {
public:
    TestGain(
        std::string id,
        float gain
    )
        : Effect(std::move(id), "Test Gain"),
          gain_(gain)
    {
    }

    void prepare(double, std::size_t) override
    {
    }

    void reset() override
    {
        Effect::reset();
    }

protected:
    void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) override
    {
        for (std::size_t i = 0; i < numFrames; ++i) {
            output[i] = input[i] * gain_;
        }
    }

private:
    float gain_;
};

}

int main()
{
    stomp::dsp::EffectChain chain;

    chain.addEffect(
        std::make_unique<TestGain>(
            "gain-half",
            0.5f
        )
    );

    chain.addEffect(
        std::make_unique<TestGain>(
            "gain-double",
            2.0f
        )
    );

    assert(chain.size() == 2);

    constexpr std::size_t blockSize = 5;

    chain.prepare(
        48000.0,
        blockSize
    );

    std::array<float, blockSize> input{
        0.0f,
        0.25f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, blockSize> output{};

    chain.process(
        input.data(),
        output.data(),
        input.size()
    );

    // 0.5x followed by 2.0x = 1.0x
    for (std::size_t i = 0; i < input.size(); ++i) {
        assert(
            nearlyEqual(
                output[i],
                input[i]
            )
        );
    }

    std::cout << "EffectChainTest passed\n";

    return 0;
}
