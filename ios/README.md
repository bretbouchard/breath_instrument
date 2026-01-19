# AUv3 (iOS) Build Instructions

## Overview

AUv3 format for iOS requires a separate Xcode project due to iOS-specific requirements and code signing.

## Requirements

- macOS with Xcode 15+
- iOS Developer Account (for code signing)
- iOS Device or Simulator for testing
- JUCE 8.0.12+

## Build Instructions

### Method 1: Using JUCE Projucer

1. **Open JUCE Projucer**:
   ```bash
   open /Applications/JUCE/Projucer
   ```

2. **Create New iOS AUv3 Project**:
   - File → New Project
   - Select "Audio Unit (v3)" template
   - Set project name: "BreathLead"
   - Select iOS platform
   - Set company identifier: "com.schillingerEcosystem"

3. **Configure Project**:
   ```
   Project Settings:
   - Plugin Name: BreathLead
   - Bundle ID: com.schillingerEcosystem.breathlead
   - Plugin Type: AUv3 Instrument
   - Needs MIDI Input: ✅
   - Is Synthesizer: ✅
   ```

4. **Add Source Files**:
   ```
   Add to project:
   - include/dsp/BreathLeadVoice.h
   - src/dsp/BreathLeadVoice.cpp
   - include/plugin/BreathLeadProcessor.h
   - src/plugin/BreathLeadProcessor.cpp
   - src/plugin/BreathLeadPlugin.cpp
   ```

5. **Build for iOS**:
   - Select target: iOS (Device or Simulator)
   - Product → Build
   - Output: build/ios/BreathLead.app

### Method 2: Manual Xcode Project

Create an Xcode project with the following structure:

```
BreathLeadiOS/
├── BreathLeadiOS.xcodeproj/
├── BreathLeadiOS/
│   ├── AudioUnit.swift (AUv3 entry point)
│   ├── BreathLeadProcessor.h
│   ├── BreathLeadProcessor.cpp
│   ├── BreathLeadVoice.h
│   ├── BreathLeadVoice.cpp
│   └── Info.plist
```

### Method 3: Export from Existing Build

If you have macOS builds, you can export to iOS:

1. **Open macOS build in Xcode**:
   ```bash
   open build/BreathLead_artefacts/Release/BreathLead.xcodeproj
   ```

2. **Enable iOS Target**:
   - Project → Target → General → Supported Platforms
   - Add "iOS" to platforms list
   - Set iOS deployment target: 13.0+

3. **Code Signing**:
   - Project → Target → Signing & Capabilities
   - Select development team
   - Enable automatic signing

4. **Build for iOS**:
   - Product → Destination → iOS Device
   - Product → Build

## Installation on iOS Device

### Method 1: Direct Deployment

```bash
# Install to connected iOS device
xcodebuild -project BreathLeadiOS.xcodeproj \
  -scheme BreathLeadiOS \
  -configuration Release \
  -sdk iphoneos \
  -destination 'platform=iOS,name=iPhone' \
  install
```

### Method 2: TestFlight Distribution

1. Archive the app:
   ```bash
   xcodebuild archive \
     -project BreathLeadiOS.xcodeproj \
     -scheme BreathLeadiOS \
     -archive build/BreathLeadiOS.xcarchive
   ```

2. Upload to App Store Connect:
   ```bash
   xcodebuild -exportArchive \
     -archivePath build/BreathLeadiOS.xcarchive \
     -exportPath build/export \
     -exportOptionsPlist ExportOptions.plist
   ```

## Testing in iOS DAWs

### Compatible iOS DAWs:
- **GarageBand iOS** - Full AUv3 support
- **Logic Pro for iPad** - Full AUv3 support
- **Cubasis** - AUv3 support
- **AUM (Audio Mixer)** - AUv3 host

### Testing Steps:

1. **Install Plugin**:
   - Connect iOS device to macOS
   - Build and install via Xcode
   - Or install via TestFlight

2. **Load in DAW**:
   - Open GarageBand or Logic Pro for iPad
   - Create new track → Instrument → AU Instruments
   - Select "BreathLead" from list

3. **Test Features**:
   - ✅ MIDI keyboard input
   - ✅ 5 parameter knobs (Air, Tone, Formant, Resistance, Vibrato)
   - ✅ Pitch bend response
   - ✅ Mod wheel control
   - ✅ Aftertouch response
   - ✅ Preset loading

## Known Limitations

### iOS-Specific Constraints:

1. **CPU Limit**: iOS apps have strict CPU limits
   - Solution: Optimize DSP code for mobile
   - Current DSP is already efficient

2. **Memory Constraints**: iOS has limited memory
   - Solution: Use singleton pattern for DSP engine
   - Already implemented in current code

3. **Code Signing Required**:
   - Development builds require Apple Developer account
   - Distribution requires App Store approval

4. **No Standalone App**:
   - AUv3 plugins must be hosted by DAWs
   - Cannot run independently on iOS

## Status

- ✅ **macOS AU**: Built and working
- ⚠️ **iOS AUv3**: Requires Xcode project setup (documented)
- ✅ **DSP Engine**: Platform-agnostic
- ✅ **MIDI Support**: Fully compatible with iOS MIDI APIs

## Next Steps

1. Create Xcode project using Projucer
2. Test in iOS Simulator
3. Deploy to physical device
4. Test in GarageBand iOS
5. Submit to App Store (optional)

---

**Last Updated**: January 19, 2026
**Status**: Documentation Complete
**Build Method**: Requires Xcode project creation
