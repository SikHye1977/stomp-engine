#include "stomp/dsp/EffectChain.hpp"

#include <algorithm>
#include <stdexcept>

namespace stomp::dsp {

void EffectChain::addEffect(std::unique_ptr<Effect> effect)
{
    if (!effect) {
        throw std::invalid_argument(
            "Effect cannot be null"
        );
    }

    for (const auto& existing : effects_) {
        if (existing->getId() == effect->getId()) {
            throw std::invalid_argument(
                "Duplicate effect ID: " + effect->getId()
            );
        }
    }

    effects_.push_back(std::move(effect));
}

Effect& EffectChain::getEffect(const std::string& id)
{
    for (auto& effect : effects_) {
        if (effect->getId() == id) {
            return *effect;
        }
    }

    throw std::out_of_range(
        "Effect not found: " + id
    );
}

const Effect& EffectChain::getEffect(
    const std::string& id
) const
{
    for (const auto& effect : effects_) {
        if (effect->getId() == id) {
            return *effect;
        }
    }

    throw std::out_of_range(
        "Effect not found: " + id
    );
}

std::size_t EffectChain::getEffectCount() const
{
    return effects_.size();
}

void EffectChain::prepare(
    double sampleRate,
    std::size_t blockSize
)
{
    bufferA_.resize(blockSize);
    bufferB_.resize(blockSize);

    for (auto& effect : effects_) {
        effect->prepare(sampleRate, blockSize);
    }
}

void EffectChain::process(
    const float* input,
    float* output,
    std::size_t numFrames
)
{
    if (numFrames > bufferA_.size()) {
        throw std::runtime_error(
            "EffectChain received a block larger than prepared block size"
        );
    }

    if (effects_.empty()) {
        std::copy(
            input,
            input + numFrames,
            output
        );

        return;
    }

    const float* currentInput = input;
    float* currentOutput = bufferA_.data();

    for (std::size_t i = 0; i < effects_.size(); ++i) {

        const bool isLast =
            (i == effects_.size() - 1);

        if (isLast) {
            currentOutput = output;
        }

        effects_[i]->process(
            currentInput,
            currentOutput,
            numFrames
        );

        currentInput = currentOutput;

        if (!isLast) {
            currentOutput =
                (currentOutput == bufferA_.data())
                    ? bufferB_.data()
                    : bufferA_.data();
        }
    }
}

void EffectChain::reset()
{
    for (auto& effect : effects_) {
        effect->reset();
    }
}

std::size_t EffectChain::size() const
{
    return getEffectCount();
}

} // namespace stomp::dsp
