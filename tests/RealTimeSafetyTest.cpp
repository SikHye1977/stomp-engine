#include "stomp/dsp/EffectChain.hpp"
#include "stomp/dsp/Gain.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

namespace {

bool nearlyEqual(
    float a,
    float b,
    float epsilon = 1e-6f
)
{
    return std::fabs(a - b) < epsilon;
}

}

int main()
{
    stomp::dsp::Gain gain;

    // These must be lock-free on the target platform
    // before we rely on them in the audio callback.
    assert(gain.isBypassLockFree());

    assert(
        gain
            .getParameter("gain")
            .isValueLockFree()
    );

    stomp::dsp::EffectChain chain;

    chain.addEffect(
        std::make_unique<stomp::dsp::Gain>(
            0.5f
        )
    );

    // Prepare for four samples.
    chain.prepare(
        48000.0,
        4
    );

    // But deliberately send eight samples.
    std::array<float, 8> input{
        0.0f,
        0.1f,
        0.2f,
        0.3f,
        0.4f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, 8> output{};

    // This must not throw.
    chain.process(
        input.data(),
        output.data(),
        input.size()
    );

    // Oversized blocks use safe pass-through.
    for (std::size_t i = 0; i < input.size(); ++i) {
        assert(
            nearlyEqual(
                input[i],
                output[i]
            )
        );
    }

    std::cout
        << "RealTimeSafetyTest passed\n";

    return 0;
}
