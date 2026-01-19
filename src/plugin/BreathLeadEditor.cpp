/*
  BreathLeadEditor.cpp - Minimal UI implementation
*/

#include "plugin/BreathLeadEditor.h"

//==============================================================================
BreathLeadEditor::BreathLeadEditor(BreathLeadProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Set up sliders
    auto& params = processorRef.getParameters();

    airSlider_ = std::make_unique<juce::Slider>();
    toneSlider_ = std::make_unique<juce::Slider>();
    formantSlider_ = std::make_unique<juce::Slider>();
    resistanceSlider_ = std::make_unique<juce::Slider>();
    vibratoSlider_ = std::make_unique<juce::Slider>();

    // Configure sliders (rotary knobs)
    airSlider_->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    airSlider_->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(airSlider_.get());

    toneSlider_->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    toneSlider_->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(toneSlider_.get());

    formantSlider_->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    formantSlider_->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(formantSlider_.get());

    resistanceSlider_->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resistanceSlider_->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(resistanceSlider_.get());

    vibratoSlider_->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    vibratoSlider_->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(vibratoSlider_.get());

    // Attach to parameters
    airAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        params, "air", *airSlider_);
    toneAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        params, "tone", *toneSlider_);
    formantAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        params, "formant", *formantSlider_);
    resistanceAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        params, "resistance", *resistanceSlider_);
    vibratoAttachment_ = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        params, "vibrato", *vibratoSlider_);

    setSize(400, 200);
}

BreathLeadEditor::~BreathLeadEditor()
{
}

//==============================================================================
void BreathLeadEditor::paint(juce::Graphics& g)
{
    // Background (clean, minimal)
    g.fillAll(juce::Colour(20, 20, 25));

    // Title
    g.setColour(juce::Colour(200, 200, 210));
    g.setFont(juce::FontOptions(16.0f));
    g.drawText("BREATH LEAD", getLocalBounds().removeFromTop(30),
               juce::Justification::centred, false);
}

void BreathLeadEditor::resized()
{
    auto bounds = getLocalBounds();
    bounds.removeFromTop(40); // Space for title

    // Layout 5 knobs in a row
    auto knobArea = bounds.withSizeKeepingCentre(360, 100);
    const int knobWidth = 360 / 5;

    airSlider_->setBounds(knobArea.removeFromLeft(knobWidth).withSizeKeepingCentre(60, 60));
    toneSlider_->setBounds(knobArea.removeFromLeft(knobWidth).withSizeKeepingCentre(60, 60));
    formantSlider_->setBounds(knobArea.removeFromLeft(knobWidth).withSizeKeepingCentre(60, 60));
    resistanceSlider_->setBounds(knobArea.removeFromLeft(knobWidth).withSizeKeepingCentre(60, 60));
    vibratoSlider_->setBounds(knobArea.removeFromLeft(knobWidth).withSizeKeepingCentre(60, 60));
}
