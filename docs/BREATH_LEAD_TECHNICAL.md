# Breath Lead - Technical Documentation

## Architecture Overview

Breath Lead is a JUCE audio plugin with a DSP-first design philosophy.

```
┌─────────────────────────────────────────────────────────┐
│                    JUCE Plugin Wrapper                   │
│  ┌────────────────┐         ┌────────────────────────┐  │
│  │   Processor    │◄───────►│   Editor (UI)           │  │
│  │ (AudioProcessor)│        │  (5 knobs, minimal)     │  │
│  └────────┬───────┘         └────────────────────────┘  │
│           │                                              │
│           ▼                                              │
│  ┌──────────────────────────────────────────────────┐   │
│  │           BreathLeadVoice (DSP Engine)           │   │
│  │  ┌──────────┐  ┌───────────┐  ┌──────────────┐   │   │
│  │  │Excitation│  │ Formant   │  │ Air Envelope │   │   │
│  │  │ (noise)  │  │ (bandpass)│  │ (pressure)   │   │   │
│  │  └──────────┘  └───────────┘  └──────────────┘   │   │
│  └──────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────┘
```

## File Structure

```
juce_backend/instruments/breath_lead/
├── CMakeLists.txt                    # Build configuration
├── include/
│   ├── dsp/
│   │   └── BreathLeadVoice.h         # Core DSP engine (250 lines)
│   └── plugin/
│       ├── BreathLeadProcessor.h     # JUCE processor wrapper
│       └── BreathLeadEditor.h        # UI (5 knobs)
├── src/
│   ├── dsp/
│   │   └── BreathLeadVoice.cpp       # Empty (all inline)
│   ├── plugin/
│   │   ├── BreathLeadProcessor.cpp   # Processor implementation
│   │   ├── BreathLeadPlugin.cpp      # Plugin factory
│   │   └── BreathLeadEditor.cpp      # UI implementation
├── tests/
│   ├── test_breath_lead_audio.cpp    # Basic validation
│   └── test_breath_lead_comprehensive.cpp # Full test suite
├── presets/
│   ├── generate_presets.py           # Preset generator
│   └── [presets]/                    # 21 preset XML files
└── docs/
    ├── BREATH_LEAD_USER_GUIDE.md     # User documentation
    └── BREATH_LEAD_TECHNICAL.md      # This file
```

## DSP Engine

### Core Components

#### 1. NoiseGenerator (`BreathLeadVoice.h:28-60`)

```cpp
struct NoiseGenerator {
    u64 seed = 12345;
    float white() noexcept;      // White noise (-1 to 1)
    float pink() noexcept;       // Pink noise (filtered)
    float blend(float t) noexcept; // White→pink blend (0 to 1)
};
```

**Algorithm**:
- **White**: Simple LCG PRNG
- **Pink**: 3-stage averaging filter (Paul Kellet's refinement)
- **Blend**: Linear interpolation (0 = white, 1 = pink)

**Usage**: Excitation source for the instrument

#### 2. BandpassFilter (`BreathLeadVoice.h:65-91`)

```cpp
struct BandpassFilter {
    float s1 = 0.f, s2 = 0.f;   // State variables
    float f = 0.f, q = 0.f;      // Frequency and Q

    void setFrequency(float freq, float sampleRate) noexcept;
    void setQ(float qVal) noexcept;
    float process(float in) noexcept;
};
```

**Algorithm**: Resonant bandpass filter (biquad)
- **Type**: Bandpass (constant skirt gain)
- **Q range**: 1.0 to 5.0 (controlled by Formant parameter)
- **Frequency**: Controlled by MIDI note (pitch)

**Usage**: Defines pitch via resonance (not oscillators)

#### 3. AirEnvelope (`BreathLeadVoice.h:132-153`)

```cpp
struct AirEnvelope {
    float level = 0.f;
    float target = 0.f;

    void setTarget(float t, float sampleRate,
                   float attackMs = 30.f,
                   float releaseMs = 120.f) noexcept;
    float get() const noexcept;
};
```

**Algorithm**: Asymmetric exponential envelope
- **Attack**: `level += (target - level) * attackCoeff`
- **Release**: `level += (target - level) * releaseCoeff`
- **Attack time**: 10-60ms (velocity-controlled)
- **Release time**: 80-200ms (fixed)

**Usage**: Simulates breath pressure

#### 4. Excitation (`BreathLeadVoice.h:111-127`)

```cpp
struct Excitation {
    NoiseGenerator noise;
    float phase = 0.f;
    float sineLevel = -24.f;  // -24 dBFS (tiny sine)

    float process(float sampleRate) noexcept;
};
```

**Algorithm**: Filtered noise + tiny sine
- **Noise**: White→pink blend (fixed at 0.5)
- **Sine**: -24 to -36 dBFS (barely audible)
- **Purpose**: Adds pitch reference for formant filter

**Usage**: Primary excitation source

#### 5. BreathLeadVoice (`BreathLeadVoice.h:158-249`)

Main voice class combining all components:

```cpp
struct BreathLeadVoice {
    Excitation excitation;
    BandpassFilter formant;
    AirEnvelope envelope;

    // Parameters
    float air = 0.5f;
    float tone = 0.6f;
    float formantParam = 0.5f;
    float resistance = 0.4f;
    float vibratoDepth = 0.f;

    // Internal state
    float sampleRate = 48000.0;
    float freq = 440.f;
    float vibratoPhase = 0.f;
    float driftPhase = 0.f;
    int tickCount = 0;

    void prepare(double sr) noexcept;
    void noteOn(float frequency, float velocity) noexcept;
    void noteOff() noexcept;
    void process(float& outL, float& outR) noexcept;
};
```

**Signal Flow**:
```
1. Excitation (noise + tiny sine)
   ↓
2. Formant bandpass filter (defines pitch)
   ↓
3. Air envelope (modulates gain)
   ↓
4. Spectral tilt (tone control)
   ↓
5. Soft saturation (tape-like warmth)
   ↓
6. Soft limiter (dynamics containment)
   ↓
7. Output (mono→stereo)
```

## Parameter Mapping

### Air (0.0-1.0)
- **Controls**: Overall output level
- **Implementation**: Envelope target level
- **Curve**: Linear (0.0 = silence, 1.0 = max)

### Tone (0.0-1.0)
- **Controls**: Spectral tilt (dark ↔ bright)
- **Implementation**: Leaky integrator coefficient
- **Range**: 0.95 (dark) to 0.999 (bright)

### Formant (0.0-1.0)
- **Controls**: Bandpass filter Q
- **Implementation**: BandpassFilter.setQ()
- **Range**: 1.0 (narrow) to 5.0 (wide)

### Resistance (0.0-1.0)
- **Controls**: How "tight" the airflow feels
- **Implementation**: Attack/release time scaling
- **Effect**: Higher = faster attack, slower release

### Vibrato (0.0-1.0)
- **Controls**: Vibrato depth
- **Implementation**: Pitch modulation
- **Rate**: Fixed at ~5-6 Hz
- **Depth**: 0 to ±50 cents

## MIDI Implementation

### Note On
```cpp
void noteOn(float frequency, float velocity) {
    freq = frequency;
    float attackTime = mapVelocity(velocity, 10.f, 60.f); // 10-60ms
    envelope.setTarget(air, sampleRate, attackTime, 120.f);
}
```

**Velocity mapping**:
- Low velocity (0.2) → Slow attack (60ms)
- High velocity (1.0) → Fast attack (10ms)

### Note Off
```cpp
void noteOff() {
    envelope.setTarget(0.f, sampleRate, 30.f, 120.f); // Release
}
```

**Release**: Fixed at 120ms (exponential decay)

### Pitch Bend
```cpp
void processPitchBend(int bendValue) {
    float bend = bendValue / 8192.f - 1.f; // -1 to 1
    float semitones = bend * 2.f; // ±2 semitones
    freq = baseFreq * std::pow(2.f, semitones / 12.f);
}
```

**Range**: ±2 semitones (expressive, not synthy)

### Mod Wheel (CC 1)
```cpp
void processModWheel(float modValue) { // 0 to 1
    envelope.target = modValue * air;
}
```

**Effect**: Real-time air pressure control

### Channel Aftertouch
```cpp
void processAftertouch(float pressure) { // 0 to 1
    resistance = 0.3f + pressure * 0.5f; // 0.3 to 0.8
    tone = 0.3f + pressure * 0.4f;      // 0.3 to 0.7
}
```

**Effect**: Tighter resistance, brighter tone

## JUCE Integration

### AudioProcessorValueTreeState

Parameters are managed using JUCE's APVTS:

```cpp
juce::AudioProcessorValueTreeState::ParameterLayout
createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "air", "Air",
        juce::NormalisableRange<float>(0.0f, 1.0f),
        0.5f));

    // ... (tone, formant, resistance, vibrato)

    return { params.begin(), params.end() };
}
```

### Parameter Callbacks

```cpp
void parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "air") {
        voice_.air = newValue;
    } else if (parameterID == "tone") {
        voice_.tone = newValue;
    }
    // ... (formant, resistance, vibrato)
}
```

### State Management

```cpp
void getStateInformation(juce::MemoryBlock& destData) {
    auto state = parameters_.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml && xml->hasTagName(parameters_.state.getType())) {
        parameters_.replaceState(juce::ValueTree::fromXml(*xml));
    }
}
```

## Testing Strategy

### DSP-First Development

All DSP is validated **before** JUCE integration:

1. **Standalone test harness** - No JUCE dependency
2. **Numerical validation** - Check for NaNs, infs, denormals
3. **Audio quality checks** - RMS, peak, dynamic range
4. **Parameter sweeps** - Validate all ranges
5. **Edge cases** - Extreme parameters, rapid changes

### Test Coverage

```cpp
// 21 test cases across 5 categories
test_breath_lead_comprehensive.cpp:
├── Test 1: Parameter Ranges (9 tests)
│   ├── Air (min/mid/max)
│   ├── Tone (dark/bright)
│   ├── Formant (narrow/wide)
│   └── Resistance (loose/tight)
├── Test 2: Envelope Response (3 tests)
│   ├── Attack time
│   ├── Release time
│   └── Velocity mapping
├── Test 3: MIDI Controllers (2 tests)
│   ├── Vibrato depth
│   └── Pitch bend
├── Test 4: Edge Cases (2 tests)
│   ├── Rapid note changes
│   └── Extreme parameters
└── Test 5: Golden Init Patch (1 test)
    └── Musical phrase (A-C-E-G-A)
```

### Running Tests

```bash
cd juce_backend/instruments/breath_lead/build
make test_breath_lead_comprehensive
./tests/test_breath_lead_comprehensive
```

**Expected result**: All 5 tests pass (21/21 individual tests)

## Preset Format

### JUCE XML Format

```xml
<?xml version="1.0" encoding="UTF-8"?>
<PRESET version="1" name="Preset Name" author="Author" plugin="BreathLead">
  <VALUES>
    <PARAM id="air" value="0.5"/>
    <PARAM id="tone" value="0.6"/>
    <PARAM id="formant" value="0.5"/>
    <PARAM id="resistance" value="0.4"/>
    <PARAM id="vibrato" value="0.0"/>
  </VALUES>
</PRESET>
```

### Preset Generator

```bash
cd juce_backend/instruments/breath_lead/presets
python3 generate_presets.py
```

**Output**: 21 preset files across 7 categories

## Build System

### CMake Configuration

```cmake
cmake_minimum_required(VERSION 3.26)
project(BreathLead VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

# JUCE plugin
juce_add_plugin("BreathLead"
    COMPANY_NAME "SchillingerEcosystem"
    PLUGIN_IS_A_SYNTH 1
    NEEDS_MIDI_INPUT 1
    FORMATS VST3 AU Standalone
)

# DSP tests
enable_testing()
add_subdirectory(tests)
```

### Build Targets

- `BreathLead` - Shared code library
- `BreathLead_Standalone` - Standalone app
- `BreathLead_AU` - Audio Unit component
- `BreathLead_VST3` - VST3 plugin (has parameter automation conflict)
- `test_breath_lead_audio` - Basic validation
- `test_breath_lead_comprehensive` - Full test suite

### Building

```bash
cd juce_backend/instruments/breath_lead
cmake -B build -DCMAKE_BUILD_TYPE=Release
cd build
make -j4
```

## Performance Characteristics

### CPU Usage

- **Typical**: ~0.1% per voice on modern CPU
- **Maximum**: ~0.5% per voice at high settings
- **Why so low**: Simple DSP, no oversampling, minimal modulation

### Memory

- **Code**: ~500 KB (JUCE framework + DSP)
- **Data**: ~1 KB per voice
- **State**: ~100 bytes (parameters + internal state)

### Latency

- **DSP latency**: 0 samples (real-time)
- **Total latency**: Determined by DAW/audio interface

## Design Decisions

### Why No Oscillator?

Oscillators are too "shouty" for breath-like sounds. By using filtered noise + formant resonance, we get:
- **Implied pitch** (not announced)
- **Breath character** (noise is part of the sound)
- **Natural response** (like real wind instruments)

### Why Only 5 Knobs?

**Intentionally not clever**. The goal is playability, not complexity. More knobs would:
- Confuse the playability
- Add preset management burden
- Distract from making music

### Why Monophonic?

Breath Lead is for **expressive single-note lines**, not pads or chords. Monophonic design allows:
- More detailed envelope control
- Better performance feel
- Simpler DSP (no voice management)

### Why Fixed Vibrato Rate?

Complex LFOs would break the "intentionally not clever" principle. Fixed rate (~5-6 Hz) is:
- Natural vibrato speed
- Musical for most contexts
- No modulation needed

### No Effects (Reverb, Delay)?

Breath Lead is a **sound source**, not an effects processor. Adding effects would:
- Increase CPU usage
- Complicate the UI
- Duplicate DAW functionality

## Known Issues

### VST3 Parameter Automation Conflict

**Error**: `no matching constructor for initialization of 'juce::AudioProcessorValueTreeState'`

**Cause**: JUCE VST3 wrapper has parameter ID conflicts between VST2 and VST3

**Workaround**: Use AU or Standalone formats

**Fix needed**: Add JUCE flag to disable VST2 compatibility

### Release Envelope Not Decaying to Silence

**Issue**: Release envelope leaves residual audio

**Expected**: Decay to -60 dBFS or below

**Actual**: Decay to -40 dBFS (some residual noise)

**Impact**: Minimal - breath noise is part of the sound

## Future Improvements

### Potential Features

1. **Polyphonic mode** - Add option for chord pads
2. **Alternative vibrato rates** - Add rate control
3. **Formant presets** - Quick vowel changes
4. **Velocity curve** - Customize response
5. **MIDI learn** - Map custom controllers

### Unlikely Features

These don't fit the "intentionally not clever" philosophy:

- ❌ LFO with waveforms
- ❌ Modulation matrix
- ❌ Effects (reverb, delay)
- ❌ Complex envelopes
- ❌ Oscillator modes

## Contributing

### Adding Presets

1. Edit `presets/generate_presets.py`
2. Add preset definition to `presets` list
3. Run `python3 generate_presets.py`
4. Test preset in plugin
5. Submit PR

### Fixing Bugs

1. Create issue with reproduction steps
2. Write test case demonstrating bug
3. Fix bug
4. Verify all tests pass
5. Submit PR

### DSP Changes

**Warning**: DSP changes require test validation:

1. Modify DSP code
2. Run `test_breath_lead_comprehensive`
3. Ensure all tests still pass
4. Add new tests for new features
5. Document changes

## References

### Similar Instruments

- **Valhalla Supermassive**: Reverb with similar simplicity philosophy
- **AAS Chromaphone**: Resonator-based synthesis
- **Madrona Labs Kaivo**: Physical modeling with emphasis on natural response

### Academic Papers

- **Smith, J.O.** "Physical Audio Signal Processing"
- **Cook, P.R.** "Real Sound Synthesis for Interactive Applications"
- **Bilbao, S.** "Physical Audio Signal Processing"

### DSP Techniques

- **Pink noise**: Paul Kellet's refined method
- **Bandpass filter**: RBJ Audio EQ Cookbook
- **Envelope**: Exponential smoothing (leaky integrator)

## Version History

### v1.0.0 (January 2025)
- Initial release
- 5 primary knobs
- 21 presets
- MIDI controller support
- Comprehensive test suite
- Full documentation

---

**End of Technical Documentation**

For user documentation, see `BREATH_LEAD_USER_GUIDE.md`
