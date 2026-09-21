#include "stomp/dsp/Gain.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

namespace {

bool nearlyEqual(float a, float b, float epsilon = 1e-6f)
{
    return std::fabs(a - b) < epsilon;
}

}

int main()
{
    std::array<float, 5> input{
        0.0f,
        0.25f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, 5> output{};

    stomp::dsp::Gain gain(0.5f);

    gain.prepare(48000.0, input.size());

    // Effect ON
    gain.process(
        input.data(),
        output.data(),
        input.size()
    );

    assert(nearlyEqual(output[1], 0.125f));
    assert(nearlyEqual(output[2], 0.25f));
    assert(nearlyEqual(output[4], -0.5f));

    // Effect bypass
    gain.setBypassed(true);

    assert(gain.isBypassed());

    gain.process(
        input.data(),
        output.data(),
        input.size()
    );

    for (std::size_t i = 0; i < input.size(); ++i) {
        assert(nearlyEqual(output[i], input[i]));
    }

    // Effect ON again
    gain.setBypassed(false);

    assert(!gain.isBypassed());

    gain.process(
        input.data(),
        output.data(),
        input.size()
    );

    assert(nearlyEqual(output[2], 0.25f));

    std::cout << "BypassTest passed\n";

    return 0;
}
