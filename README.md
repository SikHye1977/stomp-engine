cat > README.md <<'EOF'
# stomp-engine

A custom real-time guitar multi-effects engine for Raspberry Pi.

## Project Goal

Build a standalone Raspberry Pi-based guitar multi-effects processor with:

- Real-time audio DSP
- Physical rotary encoders
- Footswitch controls
- Local LCD interface
- Preset management
- Standalone operation without a PC or web interface

## Architecture

The project separates platform-independent DSP from hardware-dependent components.

- `dsp` - Audio effects and signal processing
- `audio` - Audio backend such as JACK
- `control` - Physical controls such as encoders and footswitches
- `ui` - Local display interface

## Current Status

### v0.1

- Basic CMake project
- Common `Effect` interface
- `Gain` DSP implementation
- CTest-based Gain unit test

## Build

```bash
cmake -S . -B build
cmake --build build -j
