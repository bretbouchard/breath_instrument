/*
  BreathLeadProcessor.cpp - JUCE AudioProcessor implementation
*/

#include "plugin/BreathLeadProcessor.h"
#include "plugin/BreathLeadEditor.h"

//==============================================================================
BreathLeadProcessor::BreathLeadProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
      parameters_(*this, nullptr, juce::Identifier("BreathLead"), createParameterLayout())
#endif
{

    // Initialize voice
    voice_.prepare(48000.0);

    // Default parameter values (Golden Init Patch)
    // Soft breath, clear pitch, no vibrato, slight warmth, medium release
    voice_.air = 0.5f;
    voice_.tone = 0.6f;         // Slightly bright
    voice_.formantParam = 0.5f; // Neutral vowel
    voice_.resistance = 0.4f;   // Medium-tight
    voice_.vibratoDepth = 0.f;  // No vibrato
}

BreathLeadProcessor::~BreathLeadProcessor()
{
}

//==============================================================================
void BreathLeadProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    voice_.prepare(sampleRate);
}

void BreathLeadProcessor::releaseResources()
{
}

void BreathLeadProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                       juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    // Clear output
    for (int ch = 0; ch < numChannels; ++ch)
        buffer.clear(ch, 0, numSamples);

    // Process MIDI
    for (const auto metadata : midiMessages) {
        if (metadata.numBytes < 3) continue;

        const auto message = metadata.getMessage();

        if (message.isNoteOn()) {
            const int noteNumber = message.getNoteNumber();
            const float velocity = message.getVelocity() / 127.f;

            voice_.noteOn(440.f * std::pow(2.f, (noteNumber - 69) / 12.f), velocity);
            lastNoteNumber_ = noteNumber;
            lastVelocity_ = velocity;
            noteIsOn_ = true;
        }
        else if (message.isNoteOff()) {
            voice_.noteOff();
            noteIsOn_ = false;
        }
        else if (message.isPitchWheel()) {
            // Pitch bend ±2 semitones (expressive, not synthy)
            const float bend = message.getPitchWheelValue() / 8192.f - 1.f; // -1 to 1
            const float semitones = bend * 2.f;
            if (lastNoteNumber_ >= 0) {
                const float baseFreq = 440.f * std::pow(2.f, (lastNoteNumber_ - 69) / 12.f);
                voice_.freq = baseFreq * std::pow(2.f, semitones / 12.f);
            }
        }
        else if (message.isControllerOfType(0x01)) { // Mod wheel
            // Mod wheel → air pressure
            const float modValue = message.getControllerValue() / 127.f;
            voice_.envelope.target = modValue * voice_.air;
        }
        else if (message.isChannelPressure()) {
            // Aftertouch → resistance / brightness
            const float pressure = message.getChannelPressureValue() / 127.f;
            voice_.resistance = 0.3f + pressure * 0.5f; // 0.3 to 0.8
            voice_.tone = 0.3f + pressure * 0.4f;      // 0.3 to 0.7 (brighter with more pressure)
        }
    }

    // Process audio
    float* outL = buffer.getWritePointer(0);
    float* outR = numChannels > 1 ? buffer.getWritePointer(1) : outL;

    for (int i = 0; i < numSamples; ++i) {
        voice_.process(outL[i], outR[i]);
    }
}

//==============================================================================
juce::AudioProcessorEditor* BreathLeadProcessor::createEditor()
{
    return new BreathLeadEditor(*this);
}

//==============================================================================
void BreathLeadProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    // Save parameters
    auto state = parameters_.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void BreathLeadProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    // Restore parameters
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName(parameters_.state.getType())) {
        parameters_.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

//==============================================================================
void BreathLeadProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    // Update voice parameters
    if (parameterID == "air") {
        voice_.air = newValue;
    } else if (parameterID == "tone") {
        voice_.tone = newValue;
    } else if (parameterID == "formant") {
        voice_.formantParam = newValue;
    } else if (parameterID == "resistance") {
        voice_.resistance = newValue;
    } else if (parameterID == "vibrato") {
        voice_.vibratoDepth = newValue;
    }
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout BreathLeadProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    // Primary Knobs (Front Panel)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "air", "Air", 0.0f, 1.0f, 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "tone", "Tone", 0.0f, 1.0f, 0.6f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "formant", "Formant", 0.0f, 1.0f, 0.5f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "resistance", "Resistance", 0.0f, 1.0f, 0.4f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "vibrato", "Vibrato", 0.0f, 1.0f, 0.0f));

    return { params.begin(), params.end() };
}
