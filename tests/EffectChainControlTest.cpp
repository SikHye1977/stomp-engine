#include "stomp/dsp/EffectChain.hpp"
#include "stomp/dsp/Gain.hpp"
#include "stomp/dsp/Overdrive.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>

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
    stomp::dsp::EffectChain chain;

    chain.addEffect(
        std::make_unique<stomp::dsp::Gain>()
    );

    chain.addEffect(
        std::make_unique<stomp::dsp::Overdrive>()
    );

    // Effect count
    assert(chain.getEffectCount() == 2);
    assert(chain.size() == 2);

    // Generic lookup
    auto& gain =
        chain.getEffect("gain");

    auto& overdrive =
        chain.getEffect("overdrive");

    assert(gain.getId() == "gain");
    assert(overdrive.getId() == "overdrive");

    // Generic parameter control
    gain
        .getParameter("gain")
        .setValue(1.5f);

    assert(
        nearlyEqual(
            gain.getParameter("gain").getValue(),
            1.5f
        )
    );

    overdrive
        .getParameter("drive")
        .setValue(0.8f);

    assert(
        nearlyEqual(
            overdrive.getParameter("drive").getValue(),
            0.8f
        )
    );

    // Generic bypass control
    assert(!overdrive.isBypassed());

    overdrive.setBypassed(true);

    assert(overdrive.isBypassed());

    overdrive.setBypassed(false);

    assert(!overdrive.isBypassed());

    // Unknown effect lookup
    bool missingEffectThrew = false;

    try {
        chain.getEffect("delay");
    } catch (const std::out_of_range&) {
        missingEffectThrew = true;
    }

    assert(missingEffectThrew);

    // Duplicate effect ID must be rejected.
    bool duplicateThrew = false;

    try {
        chain.addEffect(
            std::make_unique<stomp::dsp::Gain>()
        );
    } catch (const std::invalid_argument&) {
        duplicateThrew = true;
    }

    assert(duplicateThrew);

    // Null effects must still be rejected.
    bool nullThrew = false;

    try {
        chain.addEffect(nullptr);
    } catch (const std::invalid_argument&) {
        nullThrew = true;
    }

    assert(nullThrew);

    std::cout
        << "EffectChainControlTest passed\n";

    return 0;
}
