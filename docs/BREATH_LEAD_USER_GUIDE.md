# Breath Lead - User Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Quick Start](#quick-start)
3. [The 5 Knobs](#the-5-knobs)
4. [Performance Controls](#performance-controls)
5. [Presets](#presets)
6. [Sound Design Tips](#sound-design-tips)
7. [Technical Details](#technical-details)
8. [FAQ](#faq)

---

## Introduction

**Breath Lead** is a monophonic expressive lead instrument that responds to breath-like gestures. It sits between soft synths and wind/vocal instruments, designed for playability, mix-fit, and expressiveness.

### What Makes It Special

- **Not oscillator-centric**: Pitch is implied by resonance, not shouted
- **Breath-like response**: Responds to small gestures, feels alive
- **Golden Init Patch**: Sounds musical in 5 seconds with one finger
- **Never gimmicky**: No FM, no chaos, no fast random modulation
- **Mix-ready**: Good headroom, clean output, no harshness

### Ideal For

- Expressive lead lines
- Atmospheric pads (mono)
- Wind instrument emulations
- Vocal-like textures
- Contoured solos

---

## Quick Start

### 5-Second Test

1. Open Breath Lead
2. Play a single note (any note)
3. **It should sound musical immediately**

The **Golden Init Patch** is tuned to sound good without any adjustment. That's the point.

### First Sounds to Try

```
Note: A4 (440 Hz)
Velocity: Medium (about 80)
Result: Clear, breathy lead with subtle warmth
```

**Move your mod wheel**: Air pressure increases in real-time
**Add aftertouch**: Brightness increases, resistance tightens
**Bend pitch**: ±2 semitones (expressive, not synthy)

---

## The 5 Knobs

Breath Lead has **5 primary parameters** - no hidden menus, no complex modulation. Just these:

### 1. Air 🌬️
**Overall breath intensity**

- **0.0**: Silent (no breath = no sound)
- **0.5**: Medium breath (default)
- **1.0**: Full breath (powerful)

**What it does**: Controls how much air is flowing through the virtual instrument. More air = more output level, more breath noise, more presence.

**When to use**:
- Low air: Intimate passages, background textures
- Mid air: Balanced lead lines (most common)
- High air: Solos, projecting parts, powerful moments

### 2. Tone 🎨
**Dark ↔ Bright (spectral tilt)**

- **0.0**: Dark, warm, muted
- **0.6**: Slightly bright (default)
- **1.0**: Bright, clear, present

**What it does**: Filters the high frequencies. Dark sounds like a flute in lower register; bright sounds like a trumpet or brass.

**When to use**:
- Dark: Background parts, warmth, vintage feel
- Mid: Balanced, versatile (most common)
- Bright: Lead lines, cutting through mix, presence

### 3. Formant 🗣️
**Vowel/resonance shape**

- **0.0**: Narrow resonance (focused)
- **0.5**: Neutral vowel (default)
- **1.0**: Wide resonance (open)

**What it does**: Changes the "vowel" of the sound. Like saying "oooo" vs "aaaa". Narrow is tight and focused; wide is open and spacious.

**When to use**:
- Low: Focused, controlled, centered
- Mid: Neutral, natural (most common)
- High: Open, resonant, airy

### 4. Resistance 💪
**How "tight" the airflow feels**

- **0.0**: Loose (free, easy)
- **0.4**: Medium-tight (default)
- **1.0**: Tight (controlled, firm)

**What it does**: Simulates how much back-pressure the instrument has. Loose is easy to blow through; tight requires more air pressure.

**When to use**:
- Low: Airy, open, free-blowing
- Mid: Balanced resistance (most common)
- High: Tight, focused, punchy

### 5. Vibrato 🎵
**Vibrato depth**

- **0.0**: No vibrato (default)
- **0.5**: Medium vibrato
- **1.0**: Wide vibrato

**What it does**: Adds pitch oscillation. Subtle warmth at low settings; expressive wobble at high settings. Rate is fixed around 5-6 Hz (natural vibrato).

**When to use**:
- Low: Pure tone, steady notes
- Mid: Gentle warmth, classic lead
- High: Expressive solos, dramatic moments

---

## Performance Controls

Breath Lead responds to standard MIDI controllers for expressive performance:

### Velocity → Attack Intensity
- **Low velocity (1-40)**: Soft attack, gentle
- **Medium velocity (41-80)**: Balanced attack
- **High velocity (81-127)**: Fast attack, pronounced

**Use**: Play with dynamics for natural phrasing

### Mod Wheel → Air Pressure
- **Low (0-31)**: Decrease air
- **Center (32-95)**: Normal air
- **High (96-127)**: Increase air

**Use**: Real-time breath control, swells, crescendos

### Channel Aftertouch → Resistance + Tone
- **Low (0-31)**: Normal resistance, normal tone
- **Medium (32-95)**: Tighter resistance, brighter
- **High (96-127)**: Tightest resistance, brightest

**Use**: Add brightness and intensity to held notes

### Pitch Bend → ±2 Semitones
- **Down (0-4095)**: Pitch down to -2 semitones
- **Center (4096)**: No bend
- **Up (4097-8191)**: Pitch up to +2 semitones

**Use**: Expressive pitch slides, not extreme bends

---

## Presets

Breath Lead comes with **21 presets across 7 categories**:

### Init Patches (1)
- **Golden Init Patch** - Default, sounds good immediately

### Breath Intensity (3)
- **Soft Breath** - Gentle, intimate
- **Medium Breath** - Balanced, versatile
- **Full Breath** - Powerful, projecting

### Tone Colors (3)
- **Dark** - Warm, muted, intimate
- **Bright** - Clear, present, cutting
- **Neutral** - Balanced, natural

### Character (3)
- **Airy** - Breath noise prominent, spacious
- **Focused** - Tight, controlled, centered
- **Open** - Loose, resonant, free

### Performance (3)
- **Subtle Vibrato** - Gentle oscillation, warmth
- **Wide Vibrato** - Expressive wobble, classic
- **No Vibrato** - Pure tone, steady

### Musical Styles (5)
- **Flute-like** - Woodwind character, airy
- **Brassy** - Trumpet-like, bright
- **Vocal Ooh** - Vowel Ooh, warm
- **Vocal Aah** - Vowel Aah, open
- **Synth Lead** - Electronic, controlled

### Special Effects (3)
- **Whisper** - Noisy, intimate
- **Punchy** - Fast attack, percussive
- **Evanescent** - Delicate, fading

### Installing Presets

**macOS**:
```bash
# Copy to user presets directory
cp -r presets/* ~/Library/Audio/Presets/SchillingerEcosystem/BreathLead/
```

**DAW**: Most DAWs will scan the user presets directory automatically.

---

## Sound Design Tips

### Getting That "Breathy" Sound

1. Start with **Soft Breath** preset
2. Increase **Air** to 0.7-0.8
3. Decrease **Tone** to 0.3-0.4 (darker)
4. Decrease **Formant** to 0.2-0.3 (narrow)
5. Decrease **Resistance** to 0.1-0.2 (loose)

Result: Intimate, whispery texture with lots of air noise

### Getting That "Brassy" Sound

1. Start with **Brassy** preset
2. Increase **Air** to 0.7-0.9
3. Increase **Tone** to 0.8-1.0 (bright)
4. Set **Formant** to 0.4-0.6 (mid vowel)
5. Increase **Resistance** to 0.6-0.8 (tight)
6. Add **Vibrato** to 0.3-0.5

Result: Trumpet-like lead with punchy attacks

### Getting That "Vocal" Sound

1. Start with **Vocal Aah** preset
2. Set **Air** to 0.5-0.6
3. Set **Tone** to 0.5-0.7
4. Adjust **Formant** for vowel (0.3 = ooh, 0.6 = aah)
5. Set **Resistance** to 0.4-0.5
6. Add **Vibrato** to 0.5-0.7

Result: Vocal-like lead with formant resonance

### Layering with Other Sounds

Breath Lead works great layered:

- **With piano**: Adds breath texture to acoustic piano
- **With strings**: Adds wind character to string pads
- **With synths**: Adds organic feel to electronic sounds
- **With guitars**: Adds atmospheric texture

**Tip**: Keep Breath Lead lower in the mix for textural layers

---

## Technical Details

### Sound Generation

Breath Lead is **not oscillator-based**. It uses:

1. **Noise excitation**: Filtered noise (white → pink blend)
2. **Formant filter**: Bandpass filter defines pitch
3. **Air envelope**: Pressure-based envelope (slow attack/release)
4. **Spectral tilt**: Dark ↔ bright filtering
5. **Soft saturation**: Tape-like warmth
6. **Vibrato**: Slow oscillation (5-6 Hz)

### Envelope Times

- **Attack**: 10-60ms (velocity-controlled)
- **Release**: 80-200ms (exponential decay)
- **Vibrato rate**: ~5-6 Hz (fixed)
- **Pitch drift**: ±5 cents (subtle)

### CPU Usage

- **Lightweight**: Simple DSP, low CPU
- **Sample rate**: Works at any rate (44.1kHz, 48kHz, 96kHz)
- **Oversampling**: None (keeps it simple)

### Output Characteristics

- **Mono source**: Single voice, dual output
- **Dynamic range**: ~40 dB from silence to max
- **Headroom**: +20 dB before clipping
- **Frequency response**: Controlled by Tone knob

---

## FAQ

### Q: Why no oscillator? 🤔

**A**: Oscillators are too "shouty" for breath-like sounds. By using filtered noise and formant resonance, we get pitch that's **implied** rather than **announced**. This feels more like a wind instrument.

### Q: Can I play chords? 🎹

**A**: No, Breath Lead is **monophonic** by design. It's for expressive single-note lines, not pads or chords. Use multiple instances if you need layered leads.

### Q: Why only 5 knobs? 🎛️

**A**: **Intentionally not clever**. The goal is playability, not complexity. These 5 knobs cover all the essential characters. More knobs would just confuse the playability.

### Q: Where's the LFO? 🔄

**A**: There is no LFO. Vibrato is a fixed-rate oscillation (5-6 Hz). This keeps it simple and musical. If you need complex modulation, use your DAW's MIDI controllers.

### Q: Can I save my own presets? 💾

**A**: Yes! Save presets from your DAW's preset menu. Or manually adjust the 5 knobs and save the state as a preset.

### Q: Why no effects (reverb, delay)? 🎛️

**A**: Breath Lead is a **sound source**, not an effects processor. Add reverb/delay in your DAW for production. This keeps the plugin focused and CPU-light.

### Q: What's the "Golden Init Patch"? ✨

**A**: It's the default preset. We spent time tuning it so it sounds musical immediately. The spec said: "If this instrument feels good with one finger, sounds musical at init, responds predictably - it will succeed." That's the Golden Init Patch.

### Q: Can I use this for live performance? 🎤

**A**: Absolutely! Breath Lead is designed for real-time performance:
- **Mod wheel**: Real-time breath control
- **Aftertouch**: Add brightness to held notes
- **Velocity**: Play with dynamics
- **Pitch bend**: Expressive slides

Pair with a wind controller (ROLI Seaboard, Yamaha BC3) for ultimate expression.

### Q: Why does Air=0.0 produce silence? 🔇

**A**: Because no breath = no sound. Just like a real wind instrument. If you want silence, just decrease Air to zero. No need for a gate.

### Q: What DAWs are supported? 🖥️

**A**: Breath Lead supports:
- **Standalone**: macOS app
- **AU**: Logic Pro, GarageBand, AU-compatible hosts
- **VST3**: Ableton Live, Cubase, Reaper, Bitwig, etc.

### Q: Can I automate the parameters? 🎚️

**A**: Yes! All 5 knobs are automatable in your DAW. Use automation for swells, filter sweeps, and evolving textures.

---

## Version History

### v1.0.0 (Current)
- Initial release
- 21 presets
- 5 primary knobs
- MIDI controller support
- DSP-validated (all tests passing)

---

## Support & Community

### Report Issues
Found a bug? Have a feature request?
- GitHub: https://github.com/bretbouchard/white_room_box
- Issues: Report bugs with system info and DAW details

### Preset Sharing
Created a great preset? Share it!
- Export preset XML
- Share on GitHub community
- Tag with #BreathLead

### Tutorials & Demos
Want to learn more?
- Watch demo videos (coming soon)
- Read sound design tutorials (coming soon)
- Join community discussions (coming soon)

---

## License

Breath Lead is part of the White Room ecosystem.
- **Plugin**: Free and open source
- **Presets**: Free to use and modify
- **Documentation**: Creative Commons BY-SA 4.0

---

**Enjoy making music with Breath Lead! 🎵**

*Remember: The Golden Init Patch sounds musical in 5 seconds. Just play.* ✨
