#pragma once

#include "stomp/dsp/Parameter.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace stomp::dsp {

class Effect {
public:
    Effect(std::string id, std::string name)
        : id_(std::move(id)),
          name_(std::move(name))
    {
        if (id_.empty()) {
            throw std::invalid_argument("Effect ID cannot be empty");
        }

        if (name_.empty()) {
            throw std::invalid_argument("Effect name cannot be empty");
        }
    }

    virtual ~Effect() = default;

    virtual void prepare(double sampleRate, std::size_t blockSize) = 0;

    void process(
        const float* input,
        float* output,
        std::size_t numFrames
    )
    {
        if (bypassed_) {
            std::copy(input, input + numFrames, output);
            return;
        }

        processBlock(input, output, numFrames);
    }

    virtual void reset()
    {
        for (auto& parameter : parameters_) {
            parameter->reset();
        }
    }

    void setBypassed(bool bypassed)
    {
        bypassed_ = bypassed;
    }

    bool isBypassed() const
    {
        return bypassed_;
    }

    const std::string& getId() const
    {
        return id_;
    }

    const std::string& getName() const
    {
        return name_;
    }

    Parameter& getParameter(const std::string& id)
    {
        for (auto& parameter : parameters_) {
            if (parameter->getId() == id) {
                return *parameter;
            }
        }

        throw std::out_of_range(
            "Parameter not found: " + id
        );
    }

    const Parameter& getParameter(const std::string& id) const
    {
        for (const auto& parameter : parameters_) {
            if (parameter->getId() == id) {
                return *parameter;
            }
        }

        throw std::out_of_range(
            "Parameter not found: " + id
        );
    }

    std::size_t getParameterCount() const
    {
        return parameters_.size();
    }

protected:
    Parameter& addParameter(
        std::string id,
        std::string name,
        float minValue,
        float maxValue,
        float defaultValue
    )
    {
        for (const auto& parameter : parameters_) {
            if (parameter->getId() == id) {
                throw std::invalid_argument(
                    "Duplicate parameter ID: " + id
                );
            }
        }

        auto parameter = std::make_unique<Parameter>(
            std::move(id),
            std::move(name),
            minValue,
            maxValue,
            defaultValue
        );

        Parameter& reference = *parameter;

        parameters_.push_back(std::move(parameter));

        return reference;
    }

    virtual void processBlock(
        const float* input,
        float* output,
        std::size_t numFrames
    ) = 0;

private:
    std::string id_;
    std::string name_;

    bool bypassed_ = false;

    std::vector<std::unique_ptr<Parameter>> parameters_;
};

} // namespace stomp::dsp
