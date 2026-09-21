#pragma once

#include "stomp/dsp/EffectChain.hpp"

#include <jack/jack.h>

#include <atomic>
#include <cstddef>
#include <string>

namespace stomp::audio {

class JackAudioBackend {
public:
    explicit JackAudioBackend(
        stomp::dsp::EffectChain& effectChain
    );

    ~JackAudioBackend();

    JackAudioBackend(
        const JackAudioBackend&
    ) = delete;

    JackAudioBackend& operator=(
        const JackAudioBackend&
    ) = delete;

    bool open(
        const std::string& clientName
    );

    bool start();

    void stop();

    bool isRunning() const noexcept;

    double getSampleRate() const noexcept;
    std::size_t getBufferSize() const noexcept;

private:
    static int processCallback(
        jack_nframes_t numFrames,
        void* argument
    );

    static void shutdownCallback(
        void* argument
    );

    int process(
        jack_nframes_t numFrames
    ) noexcept;

    void close() noexcept;

    stomp::dsp::EffectChain& effectChain_;

    jack_client_t* client_ = nullptr;
    jack_port_t* inputPort_ = nullptr;
    jack_port_t* outputPort_ = nullptr;

    double sampleRate_ = 0.0;
    std::size_t bufferSize_ = 0;

    std::atomic<bool> running_{false};
};

} // namespace stomp::audio
