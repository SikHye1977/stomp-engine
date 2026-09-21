#include "stomp/audio/JackAudioBackend.hpp"

#include <jack/jack.h>

namespace stomp::audio {

JackAudioBackend::JackAudioBackend(
    stomp::dsp::EffectChain& effectChain
)
    : effectChain_(effectChain)
{
}

JackAudioBackend::~JackAudioBackend()
{
    close();
}

bool JackAudioBackend::open(
    const std::string& clientName
)
{
    if (client_ != nullptr) {
        return false;
    }

    jack_status_t status{};

    client_ = jack_client_open(
        clientName.c_str(),
        JackNoStartServer,
        &status
    );

    if (client_ == nullptr) {
        return false;
    }

    inputPort_ = jack_port_register(
        client_,
        "input",
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsInput,
        0
    );

    outputPort_ = jack_port_register(
        client_,
        "output",
        JACK_DEFAULT_AUDIO_TYPE,
        JackPortIsOutput,
        0
    );

    if (
        inputPort_ == nullptr ||
        outputPort_ == nullptr
    ) {
        close();
        return false;
    }

    sampleRate_ =
        static_cast<double>(
            jack_get_sample_rate(client_)
        );

    bufferSize_ =
        static_cast<std::size_t>(
            jack_get_buffer_size(client_)
        );

    effectChain_.prepare(
        sampleRate_,
        bufferSize_
    );

    if (
        jack_set_process_callback(
            client_,
            &JackAudioBackend::processCallback,
            this
        ) != 0
    ) {
        close();
        return false;
    }

    jack_on_shutdown(
        client_,
        &JackAudioBackend::shutdownCallback,
        this
    );

    return true;
}

bool JackAudioBackend::start()
{
    if (
        client_ == nullptr ||
        running_.load(std::memory_order_relaxed)
    ) {
        return false;
    }

    if (jack_activate(client_) != 0) {
        return false;
    }

    running_.store(
        true,
        std::memory_order_relaxed
    );

    return true;
}

void JackAudioBackend::stop()
{
    if (client_ == nullptr) {
        return;
    }

    if (
        running_.exchange(
            false,
            std::memory_order_relaxed
        )
    ) {
        jack_deactivate(client_);
    }
}

bool JackAudioBackend::isRunning() const noexcept
{
    return running_.load(
        std::memory_order_relaxed
    );
}

double JackAudioBackend::getSampleRate() const noexcept
{
    return sampleRate_;
}

std::size_t
JackAudioBackend::getBufferSize() const noexcept
{
    return bufferSize_;
}

int JackAudioBackend::processCallback(
    jack_nframes_t numFrames,
    void* argument
)
{
    auto* backend =
        static_cast<JackAudioBackend*>(
            argument
        );

    return backend->process(numFrames);
}

void JackAudioBackend::shutdownCallback(
    void* argument
)
{
    auto* backend =
        static_cast<JackAudioBackend*>(
            argument
        );

    backend->running_.store(
        false,
        std::memory_order_relaxed
    );

    backend->client_ = nullptr;
    backend->inputPort_ = nullptr;
    backend->outputPort_ = nullptr;
}

int JackAudioBackend::process(
    jack_nframes_t numFrames
) noexcept
{
    if (
        inputPort_ == nullptr ||
        outputPort_ == nullptr
    ) {
        return 0;
    }

    const auto* input =
        static_cast<const float*>(
            jack_port_get_buffer(
                inputPort_,
                numFrames
            )
        );

    auto* output =
        static_cast<float*>(
            jack_port_get_buffer(
                outputPort_,
                numFrames
            )
        );

    if (
        input == nullptr ||
        output == nullptr
    ) {
        return 0;
    }

    effectChain_.process(
        input,
        output,
        static_cast<std::size_t>(
            numFrames
        )
    );

    return 0;
}

void JackAudioBackend::close() noexcept
{
    stop();

    if (client_ != nullptr) {
        jack_client_close(client_);
        client_ = nullptr;
    }

    inputPort_ = nullptr;
    outputPort_ = nullptr;
}

} // namespace stomp::audio
