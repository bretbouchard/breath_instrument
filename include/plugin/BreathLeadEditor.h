/*
  BreathLeadEditor.h - Minimal UI for Breath Lead

  Clean, simple interface with 5 primary knobs.
  No labels, no tooltips - just direct control.
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "plugin/BreathLeadProcessor.h"

class BreathLeadEditor  : public juce::AudioProcessorEditor
{
public:
    BreathLeadEditor(BreathLeadProcessor& p);
    ~BreathLeadEditor() override;

    //==============================================================================
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    // Reference to processor
    BreathLeadProcessor& processorRef;

    // UI Components
    std::unique_ptr<juce::Slider> airSlider_;
    std::unique_ptr<juce::Slider> toneSlider_;
    std::unique_ptr<juce::Slider> formantSlider_;
    std::unique_ptr<juce::Slider> resistanceSlider_;
    std::unique_ptr<juce::Slider> vibratoSlider_;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> airAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> formantAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> resistanceAttachment_;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoAttachment_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BreathLeadEditor)
};
