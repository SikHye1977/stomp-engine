#include "stomp/dsp/Effect.hpp"
#include "stomp/dsp/Gain.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace {

bool nearlyEqual(float a, float b, float epsilon = 1e-6f)
{
    return std::fabs(a - b) < epsilon;
}

}

int main()
{
    stomp::dsp::Gain gain(0.5f);

    // Effect metadata
    assert(gain.getId() == "gain");
    assert(gain.getName() == "Gain");

    // Parameter registry
    assert(gain.getParameterCount() == 1);

    auto& parameter = gain.getParameter("gain");

    assert(parameter.getId() == "gain");
    assert(parameter.getName() == "Gain");

    assert(nearlyEqual(parameter.getValue(), 0.5f));

    // Generic parameter access must affect the actual DSP parameter.
    parameter.setValue(1.5f);

    assert(nearlyEqual(gain.getGain(), 1.5f));

    // Existing Gain API must affect the same Parameter.
    gain.setGain(0.75f);

    assert(nearlyEqual(
        gain.getParameter("gain").getValue(),
        0.75f
    ));

    // Unknown parameter
    bool threw = false;

    try {
        gain.getParameter("does-not-exist");
    } catch (const std::out_of_range&) {
        threw = true;
    }

    assert(threw);

    // Reset should restore parameter default.
    gain.reset();

    assert(nearlyEqual(gain.getGain(), 1.0f));

    std::cout << "EffectParameterTest passed\n";

    return 0;
}
