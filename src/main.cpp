#include "stomp/dsp/Gain.hpp"

#include <array>
#include <iostream>

int main()
{
    constexpr double sampleRate = 48000.0;

    std::array<float, 5> input{
        0.0f,
        0.25f,
        0.5f,
        -0.5f,
        -1.0f
    };

    std::array<float, 5> output{};

    stomp::dsp::Gain gain(0.5f);

    gain.prepare(sampleRate, input.size());

    gain.process(
        input.data(),
        output.data(),
        input.size()
    );

    std::cout << "stomp-engine v0.1\n";
    std::cout << "Gain: " << gain.getGain() << "\n\n";

    for (std::size_t i = 0; i < input.size(); ++i) {
        std::cout
            << input[i]
            << " -> "
            << output[i]
            << '\n';
    }

    return 0;
}
