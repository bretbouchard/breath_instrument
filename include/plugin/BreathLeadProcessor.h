/*
  BreathLeadProcessor.h - JUCE AudioProcessor wrapper

  Monophonic expressive lead that responds to breath-like gestures.
  Air → resistance → resonance → tone → body
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../dsp/BreathLeadVoice.h"

class BreathLeadProcessor  : public juce::AudioProcessor,
                              public juce::AudioProcessorValueTreeState::Listener
{
public:
    BreathLeadProcessor();
    ~BreathLeadProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================================
    const juce::String getName() const override { return "Breath Lead"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }

    double getTailLengthSeconds() const override { return 0.5f; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int index) override { (void)index; }

    const juce::String getProgramName(int index) override {
        (void)index;
        return {};
    }
    void changeProgramName(int index, const juce::String& name) override {
        (void)index; (void)name;
    }

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    void parameterChanged(const juce::String& parameterID, float newValue) override;

    //==============================================================================
    juce::AudioProcessorValueTreeState& getParameters() { return parameters_; }
    const juce::AudioProcessorValueTreeState& getParameters() const { return parameters_; }

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    // DSP voice (monophonic)
    breath::BreathLeadVoice voice_;

    // Parameters (minimal, intentional)
    juce::AudioProcessorValueTreeState parameters_;

    // MIDI state
    int lastNoteNumber_ = -1;
    float lastVelocity_ = 0.f;
    bool noteIsOn_ = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BreathLeadProcessor)
};
