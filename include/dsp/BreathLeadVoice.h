/*
  BreathLeadVoice.h - Expressive monophonic breath lead

  Sound model: Air → resistance → resonance → tone → body
  NOT oscillator-centric synthesis. Pitch is implied, not shouted.

  Key principles:
  - Responds to small gestures
  - Feels alive at low complexity
  - Sounds good at default
  - Never feels brittle, buzzy, or gimmicky
*/

#pragma once

#include <cmath>
#include <algorithm>
#include <random>
#include <cstdint>

namespace breath {

// Type alias
using u64 = uint64_t;

// -----------------------------------------------------------------------------
// Noise generator (white → pink blend)
// -----------------------------------------------------------------------------
struct NoiseGenerator {
    u64 seed = 12345;

    float white() noexcept {
        seed = seed * 6364136223846793005ULL + 1442695040888963407ULL;
        return float((seed >> 32) & 0xFFFFFF) / 16777216.f * 2.f - 1.f;
    }

    // Pink noise approximation (multiple octaves)
    float pink() noexcept {
        static float b[8] = {0};
        static int idx = 0;

        const float w = white();
        b[idx] = 0.99886f * b[idx] + w;
        b[idx] = 0.99332f * b[(idx + 3) & 7] + w;
        b[idx] = 0.96900f * b[(idx + 5) & 7] + w;
        b[idx] = 0.86650f * b[(idx + 7) & 7] + w;
        b[idx] = 0.55000f * b[(idx + 1) & 7] + w;

        const float sum = b[idx] + b[(idx + 3) & 7] + b[(idx + 5) & 7] +
                           b[(idx + 7) & 7] + b[(idx + 1) & 7];
        idx = (idx + 1) & 7;

        return sum * 0.2f; // Normalize
    }

    // Blend between white and pink
    float blend(float t) noexcept {
        t = std::clamp(t, 0.f, 1.f);
        return white() * (1.f - t) + pink() * t;
    }
};

// -----------------------------------------------------------------------------
// Bandpass filter (formant core)
// -----------------------------------------------------------------------------
struct BandpassFilter {
    float s1 = 0.f, s2 = 0.f;
    float f = 0.1f;
    float q = 0.5f;

    void setFrequency(float freq, float sampleRate) noexcept {
        // Exponential mapping for musical response
        f = freq / sampleRate;
        f = std::clamp(f, 0.001f, 0.4f);
    }

    void setQ(float qVal) noexcept {
        q = std::clamp(qVal, 0.5f, 10.f);
    }

    float process(float in) noexcept {
        // State variable filter (bandpass output)
        s1 += f * (in - s1 - q * s2);
        s2 += f * s1;

        // Dampen to prevent explosion
        s1 *= 0.999f;
        s2 *= 0.999f;

        return s2 * q * 2.f; // Bandpass output
    }
};

// -----------------------------------------------------------------------------
// Soft saturation (tape-like)
// -----------------------------------------------------------------------------
inline float soft_saturate(float x) noexcept {
    // Tanh is good, but this is warmer
    const float ax = std::abs(x);
    if (ax < 1.f) {
        return x;
    } else if (ax < 2.f) {
        return std::copysign(1.f + (ax - 1.f) * 0.5f, x);
    } else {
        return std::copysign(1.5f, x);
    }
}

// -----------------------------------------------------------------------------
// Excitation stage
// -----------------------------------------------------------------------------
struct Excitation {
    NoiseGenerator noise;
    float phase = 0.f;
    float sineLevel = 0.1f; // -20 dB (very subtle)

    float process(float noiseBlend, float freq, float sampleRate) noexcept {
        // Filtered noise (primary)
        const float n = noise.blend(noiseBlend) * 0.5f;

        // Tiny sine for pitch stability (secondary, very quiet)
        phase += freq / sampleRate;
        if (phase > 1.f) phase -= 1.f;
        const float s = std::sin(phase * 6.28318f) * sineLevel;

        return n + s;
    }
};

// -----------------------------------------------------------------------------
// Air envelope (pressure-based, slow attack)
// -----------------------------------------------------------------------------
struct AirEnvelope {
    float level = 0.f;
    float target = 0.f;

    // Attack: 10-60ms, Release: 80-200ms
    void setTarget(float t, float sampleRate, float attackMs = 30.f, float releaseMs = 120.f) noexcept {
        target = t;

        // Calculate time constants
        const float attackCoef = std::exp(-1.f / (attackMs * 0.001f * sampleRate));
        const float releaseCoef = std::exp(-1.f / (releaseMs * 0.001f * sampleRate));

        const float coef = (target > level) ? attackCoef : releaseCoef;

        // Smooth follow
        level += (target - level) * (1.f - coef);
    }

    float get() const noexcept {
        return level;
    }
};

// -----------------------------------------------------------------------------
// Breath Lead Voice
// -----------------------------------------------------------------------------
struct BreathLeadVoice {
    Excitation excitation;
    BandpassFilter formant;
    AirEnvelope envelope;

    float sampleRate = 48000.f;
    float freq = 440.f;

    // Parameters
    float air = 0.5f;           // Overall breath intensity
    float tone = 0.5f;          // Dark ↔ bright (spectral tilt)
    float formantParam = 0.5f;  // Vowel / resonance shape
    float resistance = 0.5f;    // How "tight" the airflow feels
    float vibratoDepth = 0.f;   // Vibrato depth

    // Internal state
    float vibratoPhase = 0.f;
    float driftPhase = 0.f;
    u64 tickCount = 0;

    void prepare(double sr) noexcept {
        sampleRate = float(sr);
        tickCount = 0;
        vibratoPhase = 0.f;
        driftPhase = 0.f;
        envelope.level = 0.f;
        envelope.target = 0.f;
    }

    void noteOn(float frequency, float velocity) noexcept {
        freq = frequency;
        // Velocity → initial air pressure
        envelope.target = velocity * air;
    }

    void noteOff() noexcept {
        envelope.target = 0.f;
    }

    void process(float& outL, float& outR) noexcept {
        tickCount++;

        // 1. Excitation (noise + tiny sine)
        const float excite = excitation.process(0.5f, freq, sampleRate);

        // 2. Air envelope
        envelope.setTarget(envelope.target, sampleRate, 30.f, 120.f);
        const float env = envelope.get();

        // 3. Slow vibrato (5-6 Hz max)
        vibratoPhase += 6.f / sampleRate;
        if (vibratoPhase > 1.f) vibratoPhase -= 1.f;
        const float vibrato = std::sin(vibratoPhase * 6.28318f) * vibratoDepth * 0.02f;

        // 4. Subtle pitch drift
        driftPhase += 0.5f / sampleRate;
        if (driftPhase > 1.f) driftPhase -= 1.f;
        const float drift = std::sin(driftPhase * 6.28318f) * 0.005f; // ±5 cents

        // 5. Formant filter (pitch-defining)
        const float pitch = freq * (1.f + vibrato + drift);
        formant.setFrequency(pitch, sampleRate);
        formant.setQ(1.f + formantParam * 4.f); // Q: 1 to 5

        const float resonated = formant.process(excite);

        // 6. Tone shaping (spectral tilt)
        // Dark: low-pass, Bright: more high-end
        // Simple tilt filter using leaky integrator
        static float tiltState = 0.f;
        const float tiltCoef = 0.95f + tone * 0.049f; // 0.95 to 0.999
        tiltState += (resonated - tiltState) * (1.f - tiltCoef);
        const float tilted = tiltState + resonated * (1.f - tiltCoef);

        // 7. Resistance (how tight the airflow feels)
        // Affects dynamics response
        const float compressed = tilted * (0.5f + resistance * 0.5f);

        // 8. Apply envelope
        const float shaped = compressed * env;

        // 9. Soft saturation (tape-like)
        const float saturated = soft_saturate(shaped * 2.f);

        // 10. Dynamics containment (soft limiter at -6 dBFS)
        const float limited = std::tanh(saturated);

        // Output (mono for now, could add slight stereo spread)
        outL = limited * 0.7f;
        outR = limited * 0.7f;
    }
};

} // namespace breath
