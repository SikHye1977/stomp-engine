#include "stomp/dsp/Overdrive.hpp"

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

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
    stomp::dsp::Overdrive overdrive;

    // Metadata
    assert(overdrive.getId() == "overdrive");
    assert(overdrive.getName() == "Overdrive");

    // Parameter registry
    assert(overdrive.getParameterCount() == 3);

    assert(
        overdrive.getParameter("drive").getId()
        == "drive"
    );

    assert(
        overdrive.getParameter("tone").getId()
        == "tone"
    );

    assert(
        overdrive.getParameter("level").getId()
        == "level"
    );

    // Generic parameter access
    overdrive
        .getParameter("drive")
        .setValue(0.8f);

    assert(
        nearlyEqual(
            overdrive.getDrive(),
            0.8f
        )
    );

    // Parameter clamping
    overdrive.setDrive(10.0f);
    assert(nearlyEqual(overdrive.getDrive(), 1.0f));

    overdrive.setTone(-10.0f);
    assert(nearlyEqual(overdrive.getTone(), 0.0f));

    overdrive.setLevel(10.0f);
    assert(nearlyEqual(overdrive.getLevel(), 1.0f));

    // DSP smoke test
    overdrive.prepare(48000.0, 5);

    std::array<float, 5> input{
        0.0f,
        0.1f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, 5> output{};

    overdrive.process(
        input.data(),
        output.data(),
        input.size()
    );

    for (float sample : output) {
        assert(std::isfinite(sample));
    }

    // Bypass must pass samples unchanged.
    overdrive.setBypassed(true);

    overdrive.process(
        input.data(),
        output.data(),
        input.size()
    );

    for (std::size_t i = 0; i < input.size(); ++i) {
        assert(
            nearlyEqual(
                input[i],
                output[i]
            )
        );
    }

    // Reset parameters to defaults.
    overdrive.setBypassed(false);
    overdrive.reset();

    assert(nearlyEqual(overdrive.getDrive(), 0.5f));
    assert(nearlyEqual(overdrive.getTone(), 0.5f));
    assert(nearlyEqual(overdrive.getLevel(), 0.5f));

    std::cout << "OverdriveTest passed\n";

    return 0;
}
