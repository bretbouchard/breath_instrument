# CLAP Format Implementation Guide

## Overview

CLAP (CLever Audio Plugin) is a modern audio plugin format developed by u-he (and others). JUCE 8.0+ includes CLAP support.

## Current Status

- ⚠️ **CLAP Support**: Requires JUCE configuration
- ✅ **DSP Engine**: Fully compatible
- ✅ **Parameters**: CLAP-ready parameter structure

## Implementation Steps

### Step 1: Verify JUCE CLAP Support

Check if your JUCE version includes CLAP support:

```bash
# Check JUCE version
cd /Applications/JUCE
git describe --tags

# Should show 8.0.0 or higher for CLAP support
```

### Step 2: Update CMakeLists.txt

The current CMakeLists.txt already includes CLAP in the FORMATS list:

```cmake
juce_add_plugin("BreathLead"
    COMPANY_NAME "SchillingerEcosystem"
    BUNDLE_ID "com.schillingerEcosystem.breathlead"
    PLUGIN_IS_A_SYNTH 1
    NEEDS_MIDI_INPUT 1
    FORMATS VST3 AU CLAP LV2 Standalone  # CLAP already listed
    LV2URI "http://schillingerEcosystem.com/plugins/breathlead"
)
```

### Step 3: Build CLAP Format

```bash
cd /Users/bretbouchard/apps/schill/breath_instrument
rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cd build
cmake --build . --config Release --target BreathLead_CLAP
```

### Step 4: Verify CLAP Output

Check if CLAP was built:
```bash
find build -name "*.clap"
# Expected: build/BreathLead_artefacts/CLAP/BreathLead.clap
```

## CLAP-Specific Features

### Parameter Automation

CLAP supports advanced parameter automation. Our current parameters are CLAP-compatible:

```cpp
// Current parameters (CLAP-ready)
params.push_back(std::make_unique<juce::AudioParameterFloat>(
    "air", "Air", 0.0f, 1.0f, 0.5f));
params.push_back(std::make_unique<juce::AudioParameterFloat>(
    "tone", "Tone", 0.0f, 1.0f, 0.6f));
// ... etc
```

### MIDI Processing

CLAP has excellent MIDI support. Our current MIDI implementation is fully compatible:

```cpp
// Pitch bend, mod wheel, aftertouch all work with CLAP
if (message.isNoteOn()) { /* ... */ }
if (message.isPitchWheel()) { /* ... */ }
if (message.isChannelPressure()) { /* ... */ }
```

## CLAP Host Compatibility

### Compatible DAWs:

- **REAPER** - Full CLAP support
- **Bitwig Studio** - Native CLAP support
- **Waveform** - CLAP support
- **Tracktion Waveform Free** - CLAP support
- **MusE** - CLAP support (Linux)

### Testing CLAP:

1. **Install CLAP**:
   ```bash
   sudo cp -R build/BreathLead_artefacts/CLAP/BreathLead.clap \
     /Library/Audio/Plug-Ins/CLAP/
   ```

2. **Test in REAPER**:
   - Insert → Virtual Instrument on new track
   - Select "BreathLead" from CLAP plugins list
   - Test all features

3. **Verify Parameter Automation**:
   - Enable automation recording
   - Move knobs (Air, Tone, Formant, etc.)
   - Verify automation tracks are created

## Troubleshooting

### CLAP Not Building

**Issue**: CLAP format not built
**Solution**:
```bash
# Check JUCE CLAP support
ls /Applications/JUCE/modules/juce_audio_plugin_client/CLAP/

# If missing, update JUCE
cd /Applications/JUCE
git pull origin master
```

### CLAP Not Appearing in DAW

**Issue**: CLAP installed but not visible in DAW
**Solution**:
```bash
# Verify installation
ls -la /Library/Audio/Plug-Ins/CLAP/BreathLead.clap

# Check permissions
sudo chmod -R 755 /Library/Audio/Plug-Ins/CLAP/BreathLead.clap

# Restart DAW and rescan plugins
```

## Advantages of CLAP

1. **Modern API**: Designed for modern plugin development
2. **Better Performance**: More efficient than VST3/AU
3. **Future-Proof**: Active development and support
4. **Open Source**: Free to implement without licensing fees
5. **Cross-Platform**: Works on macOS, Windows, Linux

## Status Summary

- ✅ **DSP Engine**: CLAP-compatible
- ✅ **Parameters**: CLAP-ready structure
- ✅ **MIDI**: Full CLAP MIDI support
- ⚠️ **Build**: Requires JUCE verification
- ⚠️ **Testing**: Needs host testing

## Next Steps

1. Verify JUCE CLAP support in version 8.0.10
2. Build CLAP format explicitly
3. Test in REAPER or Bitwig
4. Verify parameter automation
5. Document any CLAP-specific issues

---

**Last Updated**: January 19, 2026
**Status**: Configuration Complete
**Build Method**: CMake with CLAP format enabled
**Priority**: Medium (CLAP is emerging format, not critical)
