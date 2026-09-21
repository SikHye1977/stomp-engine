#pragma once

#include <algorithm>
#include <atomic>
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
            throw std::invalid_argument(
                "Parameter ID cannot be empty"
            );
        }

        if (name_.empty()) {
            throw std::invalid_argument(
                "Parameter name cannot be empty"
            );
        }

        if (minValue_ > maxValue_) {
            throw std::invalid_argument(
                "Parameter minimum cannot be greater than maximum"
            );
        }

        if (
            defaultValue_ < minValue_ ||
            defaultValue_ > maxValue_
        ) {
            throw std::invalid_argument(
                "Parameter default value must be within range"
            );
        }
    }

    void setValue(float value) noexcept
    {
        value_.store(
            std::clamp(value, minValue_, maxValue_),
            std::memory_order_relaxed
        );
    }

    float getValue() const noexcept
    {
        return value_.load(
            std::memory_order_relaxed
        );
    }

    void reset() noexcept
    {
        value_.store(
            defaultValue_,
            std::memory_order_relaxed
        );
    }

    bool isValueLockFree() const noexcept
    {
        return value_.is_lock_free();
    }

    const std::string& getId() const noexcept
    {
        return id_;
    }

    const std::string& getName() const noexcept
    {
        return name_;
    }

    float getMinValue() const noexcept
    {
        return minValue_;
    }

    float getMaxValue() const noexcept
    {
        return maxValue_;
    }

    float getDefaultValue() const noexcept
    {
        return defaultValue_;
    }

private:
    std::string id_;
    std::string name_;

    float minValue_;
    float maxValue_;
    float defaultValue_;

    std::atomic<float> value_;
};

} // namespace stomp::dsp
