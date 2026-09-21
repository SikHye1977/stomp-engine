#include "stomp/dsp/EffectChain.hpp"
#include "stomp/dsp/Gain.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

namespace {

bool nearlyEqual(float a, float b, float epsilon = 1e-6f)
{
    return std::fabs(a - b) < epsilon;
}

}

int main()
{
    stomp::dsp::EffectChain chain;

    chain.addEffect(
        std::make_unique<stomp::dsp::Gain>(0.5f)
    );

    chain.addEffect(
        std::make_unique<stomp::dsp::Gain>(2.0f)
    );

    chain.prepare(48000.0, 5);

    std::array<float, 5> input{
        0.0f,
        0.25f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, 5> output{};

    chain.process(
        input.data(),
        output.data(),
        input.size()
    );

    assert(chain.size() == 2);

    for (std::size_t i = 0; i < input.size(); ++i) {
        assert(nearlyEqual(input[i], output[i]));
    }

    std::cout << "EffectChainTest passed\n";

    return 0;
}
