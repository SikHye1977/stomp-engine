#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>

namespace stomp::dsp {

class Parameter {
public:
    Parameter(
        std::string id,
        std::string name,
        float minValue,
        float maxValue,
        float defaultValue
    )
        : id_(std::move(id)),
          name_(std::move(name)),
          minValue_(minValue),
          maxValue_(maxValue),
          defaultValue_(defaultValue),
          value_(defaultValue)
    {
        if (id_.empty()) {
            throw std::invalid_argument("Parameter ID cannot be empty");
        }

        if (name_.empty()) {
            throw std::invalid_argument("Parameter name cannot be empty");
        }

        if (minValue_ > maxValue_) {
            throw std::invalid_argument(
                "Parameter minimum cannot be greater than maximum"
            );
        }

        if (defaultValue_ < minValue_ || defaultValue_ > maxValue_) {
            throw std::invalid_argument(
                "Parameter default value must be within range"
            );
        }
    }

    void setValue(float value)
    {
        value_ = std::clamp(value, minValue_, maxValue_);
    }

    float getValue() const
    {
        return value_;
    }

    void reset()
    {
        value_ = defaultValue_;
    }

    const std::string& getId() const
    {
        return id_;
    }

    const std::string& getName() const
    {
        return name_;
    }

    float getMinValue() const
    {
        return minValue_;
    }

    float getMaxValue() const
    {
        return maxValue_;
    }

    float getDefaultValue() const
    {
        return defaultValue_;
    }

private:
    std::string id_;
    std::string name_;

    float minValue_;
    float maxValue_;
    float defaultValue_;
    float value_;
};

} // namespace stomp::dsp
