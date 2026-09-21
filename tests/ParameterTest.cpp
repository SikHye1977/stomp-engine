#include "stomp/dsp/Parameter.hpp"

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
    stomp::dsp::Parameter parameter(
        "gain",
        "Gain",
        0.0f,
        2.0f,
        1.0f
    );

    // Metadata
    assert(parameter.getId() == "gain");
    assert(parameter.getName() == "Gain");

    assert(nearlyEqual(parameter.getMinValue(), 0.0f));
    assert(nearlyEqual(parameter.getMaxValue(), 2.0f));
    assert(nearlyEqual(parameter.getDefaultValue(), 1.0f));

    // Default value
    assert(nearlyEqual(parameter.getValue(), 1.0f));

    // Normal value
    parameter.setValue(1.5f);
    assert(nearlyEqual(parameter.getValue(), 1.5f));

    // Upper clamp
    parameter.setValue(10.0f);
    assert(nearlyEqual(parameter.getValue(), 2.0f));

    // Lower clamp
    parameter.setValue(-10.0f);
    assert(nearlyEqual(parameter.getValue(), 0.0f));

    // Reset
    parameter.reset();
    assert(nearlyEqual(parameter.getValue(), 1.0f));

    // Invalid range
    bool threw = false;

    try {
        stomp::dsp::Parameter invalid(
            "test",
            "Test",
            2.0f,
            1.0f,
            1.5f
        );
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);

    std::cout << "ParameterTest passed\n";

    return 0;
}
