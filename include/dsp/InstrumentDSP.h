/*
  ==============================================================================

    InstrumentDSP.h
    Created: January 19, 2026
    Author: Bret Bouchard

    Base class for all pure DSP instrument implementations.
    Provides common interface for instrument DSP classes.

  ==============================================================================
*/

#pragma once

#include <cstring>

namespace DSP {

//==============================================================================
// Scheduled Event Structure
//==============================================================================

struct ScheduledEvent
{
    enum Type { NoteOn, NoteOff, PitchBend, CC, AllNotesOff };

    Type type = NoteOn;
    int noteNumber = 0;
    float velocity = 0.0f;
    float value = 0.0f;
    int controllerNumber = 0;
};

//==============================================================================
// InstrumentDSP Base Class
//==============================================================================

class InstrumentDSP
{
public:
    InstrumentDSP() = default;
    virtual ~InstrumentDSP() = default;

    // Lifecycle
    virtual bool prepare(double sampleRate, int blockSize) = 0;
    virtual void reset() = 0;
    virtual void process(float** outputs, int numChannels, int numSamples) = 0;

    // Event handling
    virtual void handleEvent(const ScheduledEvent& event) = 0;

    // Parameters
    virtual float getParameter(const char* paramId) const = 0;
    virtual void setParameter(const char* paramId, float value) = 0;

    // Preset management
    virtual bool savePreset(char* jsonBuffer, int jsonBufferSize) const = 0;
    virtual bool loadPreset(const char* jsonData) = 0;

    // Voice management
    virtual int getActiveVoiceCount() const = 0;
    virtual int getMaxPolyphony() const = 0;

    // Metadata
    virtual const char* getInstrumentName() const = 0;
    virtual const char* getInstrumentVersion() const = 0;

    // Optional panic method
    virtual void panic() {}
};

} // namespace DSP
