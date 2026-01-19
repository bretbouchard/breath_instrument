/*
  ==============================================================================

    BreathLeadPlugin.cpp
    Created: 19 Jan 2026 5:00:00pm
    Author:  White Room Audio

    JUCE AudioProcessor wrapper for Breath Lead expressive monophonic instrument.

  ==============================================================================
*/

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>
#include "dsp/BreathLeadVoice.h"

//==============================================================================
class BreathLeadPlugin  : public juce::AudioProcessor
{
public:
    //==============================================================================
    BreathLeadPlugin()
        : AudioProcessor (BusesProperties()
                           .withInput ("Input",  juce::AudioChannelSet::stereo())
                           .withOutput ("Output", juce::AudioChannelSet::stereo())),
          currentPresetIndex (0),
          sampleRate (48000.0)
    {
        // Initialize parameters
        addParameter (airParam = new juce::AudioParameterFloat ("air", "Air", 0.0f, 1.0f, 0.5f));
        addParameter (toneParam = new juce::AudioParameterFloat ("tone", "Tone", 0.0f, 1.0f, 0.5f));
        addParameter (formantParam = new juce::AudioParameterFloat ("formant", "Formant", 0.0f, 1.0f, 0.5f));
        addParameter (resistanceParam = new juce::AudioParameterFloat ("resistance", "Resistance", 0.0f, 1.0f, 0.5f));
        addParameter (vibratoParam = new juce::AudioParameterFloat ("vibrato", "Vibrato", 0.0f, 1.0f, 0.0f));
        addParameter (masterGainParam = new juce::AudioParameterFloat ("master", "Master", 0.0f, 1.0f, 0.7f));

        // Load factory presets
        loadFactoryPresets();

        // Initialize voice with default preset
        if (!factoryPresets.empty())
        {
            currentPreset = factoryPresets[0];
            applyPresetToVoice();
        }
    }

    ~BreathLeadPlugin() override = default;

    //==============================================================================
    void prepareToPlay (double newSampleRate, int samplesPerBlock) override
    {
        sampleRate = newSampleRate;
        voice.prepare (sampleRate);
        juce::ignoreUnused (samplesPerBlock);
    }

    void releaseResources() override
    {
        // Free DSP resources here
    }

    void processBlock (juce::AudioBuffer<float>& buffer,
                       juce::MidiBuffer& midiMessages) override
    {
        juce::ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        // Clear output buffer
        for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        // Update voice parameters from host
        updateVoiceParameters();

        // Process MIDI for note triggers
        for (const auto metadata : midiMessages)
        {
            const auto message = metadata.getMessage();
            if (message.isNoteOn())
            {
                auto midiNote = message.getNoteNumber();
                auto velocity = message.getVelocity() / 127.0f;
                auto frequency = 440.0 * std::pow(2.0, (midiNote - 69) / 12.0);
                voice.noteOn (frequency, velocity);
            }
            else if (message.isNoteOff())
            {
                voice.noteOff();
            }
        }

        // Process audio (monophonic)
        auto numSamples = buffer.getNumSamples();
        auto* outputLeft = buffer.getWritePointer(0);
        auto* outputRight = buffer.getWritePointer(1);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float outL, outR;
            voice.process (outL, outR);

            // Apply master gain
            outputLeft[sample] = outL * *masterGainParam;
            if (totalNumOutputChannels > 1)
                outputRight[sample] = outR * *masterGainParam;
        }

        // Clear remaining channels if any
        for (int channel = 2; channel < totalNumOutputChannels; ++channel)
            buffer.clear(channel, 0, numSamples);
    }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override
    {
        // TODO: Create editor component
        return nullptr;
    }

    bool hasEditor() const override
    {
        return false; // TODO: Return true once editor is created
    }

    //==============================================================================
    const juce::String getName() const override
    {
        return "Breath Lead";
    }

    bool acceptsMidi() const override
    {
        return true;
    }

    bool producesMidi() const override
    {
        return false;
    }

    double getTailLengthSeconds() const override
    {
        return 0.0;
    }

    //==============================================================================
    int getNumPrograms() override
    {
        return static_cast<int>(factoryPresets.size());
    }

    int getCurrentProgram() override
    {
        return currentPresetIndex;
    }

    void setCurrentProgram (int index) override
    {
        if (index >= 0 && index < static_cast<int>(factoryPresets.size()))
        {
            currentPresetIndex = index;
            currentPreset = factoryPresets[index];
            applyPresetToVoice();
        }
    }

    const juce::String getProgramName (int index) override
    {
        if (index >= 0 && index < static_cast<int>(factoryPresets.size()))
            return factoryPresets[index].name;

        return {};
    }

    void changeProgramName (int index, const juce::String& newName) override
    {
        if (index >= 0 && index < static_cast<int>(factoryPresets.size()))
            factoryPresets[index].name = newName;
    }

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override
    {
        // Save current preset and parameters
        juce::ValueTree state ("state");

        state.setProperty ("air", airParam->get(), nullptr);
        state.setProperty ("tone", toneParam->get(), nullptr);
        state.setProperty ("formant", formantParam->get(), nullptr);
        state.setProperty ("resistance", resistanceParam->get(), nullptr);
        state.setProperty ("vibrato", vibratoParam->get(), nullptr);
        state.setProperty ("master", masterGainParam->get(), nullptr);
        state.setProperty ("preset", currentPresetIndex, nullptr);

        juce::MemoryOutputStream stream (destData, false);
        state.writeToStream (stream);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        // Restore preset and parameters
        auto xml = juce::XmlDocument::parse (juce::String ((const char*) data, size_t (sizeInBytes)));

        if (xml != nullptr)
        {
            auto state = juce::ValueTree::fromXml (*xml);

            if (state.isValid())
            {
                *airParam = state.getProperty ("air", 0.5f);
                *toneParam = state.getProperty ("tone", 0.5f);
                *formantParam = state.getProperty ("formant", 0.5f);
                *resistanceParam = state.getProperty ("resistance", 0.5f);
                *vibratoParam = state.getProperty ("vibrato", 0.0f);
                *masterGainParam = state.getProperty ("master", 0.7f);

                currentPresetIndex = state.getProperty ("preset", 0);

                if (currentPresetIndex >= 0 && currentPresetIndex < static_cast<int>(factoryPresets.size()))
                {
                    currentPreset = factoryPresets[currentPresetIndex];
                }
            }
        }

        applyPresetToVoice();
    }

private:
    //==============================================================================
    /**
        Preset structure for Breath Lead
    */
    struct Preset
    {
        juce::String name;
        float air;
        float tone;
        float formant;
        float resistance;
        float vibrato;
        float masterGain;
    };

    //==============================================================================
    /**
        Load all factory presets
    */
    void loadFactoryPresets()
    {
        factoryPresets.clear();

        // Preset 1: Soft Flute
        Preset softFlute;
        softFlute.name = "Soft Flute";
        softFlute.air = 0.4f;
        softFlute.tone = 0.3f;
        softFlute.formant = 0.5f;
        softFlute.resistance = 0.3f;
        softFlute.vibrato = 0.2f;
        softFlute.masterGain = 0.6f;
        factoryPresets.push_back (softFlute);

        // Preset 2: Expressive Clarinet
        Preset clarinet;
        clarinet.name = "Expressive Clarinet";
        clarinet.air = 0.6f;
        clarinet.tone = 0.5f;
        clarinet.formant = 0.6f;
        clarinet.resistance = 0.5f;
        clarinet.vibrato = 0.3f;
        clarinet.masterGain = 0.7f;
        factoryPresets.push_back (clarinet);

        // Preset 3: Breathy Vocal
        Preset vocal;
        vocal.name = "Breathy Vocal";
        vocal.air = 0.7f;
        vocal.tone = 0.6f;
        vocal.formant = 0.4f;
        vocal.resistance = 0.4f;
        vocal.vibrato = 0.4f;
        vocal.masterGain = 0.7f;
        factoryPresets.push_back (vocal);

        // Preset 4: Wind Chime
        Preset chime;
        chime.name = "Wind Chime";
        chime.air = 0.3f;
        chime.tone = 0.8f;
        chime.formant = 0.7f;
        chime.resistance = 0.2f;
        chime.vibrato = 0.0f;
        chime.masterGain = 0.5f;
        factoryPresets.push_back (chime);

        // Preset 5: Ambient Pad
        Preset pad;
        pad.name = "Ambient Pad";
        pad.air = 0.8f;
        pad.tone = 0.4f;
        pad.formant = 0.3f;
        pad.resistance = 0.6f;
        pad.vibrato = 0.1f;
        pad.masterGain = 0.7f;
        factoryPresets.push_back (pad);

        // Preset 6: Ethereal
        Preset ethereal;
        ethereal.name = "Ethereal";
        ethereal.air = 0.5f;
        ethereal.tone = 0.7f;
        ethereal.formant = 0.5f;
        ethereal.resistance = 0.3f;
        ethereal.vibrato = 0.5f;
        ethereal.masterGain = 0.6f;
        factoryPresets.push_back (ethereal);

        // Preset 7: Classical Flute
        Preset flute;
        flute.name = "Classical Flute";
        flute.air = 0.5f;
        flute.tone = 0.5f;
        flute.formant = 0.6f;
        flute.resistance = 0.4f;
        flute.vibrato = 0.3f;
        flute.masterGain = 0.7f;
        factoryPresets.push_back (flute);

        // Preset 8: Saxophone
        Preset sax;
        sax.name = "Saxophone";
        sax.air = 0.7f;
        sax.tone = 0.4f;
        sax.formant = 0.5f;
        sax.resistance = 0.6f;
        sax.vibrato = 0.4f;
        sax.masterGain = 0.8f;
        factoryPresets.push_back (sax);

        // Preset 9: Oboe
        Preset oboe;
        oboe.name = "Oboe";
        oboe.air = 0.6f;
        oboe.tone = 0.6f;
        oboe.formant = 0.7f;
        oboe.resistance = 0.5f;
        oboe.vibrato = 0.3f;
        oboe.masterGain = 0.7f;
        factoryPresets.push_back (oboe);

        // Preset 10: Breath Controller
        Preset controller;
        controller.name = "Breath Controller";
        controller.air = 1.0f;
        controller.tone = 0.5f;
        controller.formant = 0.5f;
        controller.resistance = 0.5f;
        controller.vibrato = 0.2f;
        controller.masterGain = 0.7f;
        factoryPresets.push_back (controller);
    }

    //==============================================================================
    /**
        Apply current preset to voice
    */
    void applyPresetToVoice()
    {
        voice.air = currentPreset.air;
        voice.tone = currentPreset.tone;
        voice.formantParam = currentPreset.formant;
        voice.resistance = currentPreset.resistance;
        voice.vibratoDepth = currentPreset.vibrato;
    }

    //==============================================================================
    /**
        Update voice parameters from host automation
    */
    void updateVoiceParameters()
    {
        voice.air = airParam->get();
        voice.tone = toneParam->get();
        voice.formantParam = formantParam->get();
        voice.resistance = resistanceParam->get();
        voice.vibratoDepth = vibratoParam->get();
    }

    //==============================================================================
    // DSP voice instance
    breath::BreathLeadVoice voice;

    // Parameters
    juce::AudioParameterFloat* airParam;
    juce::AudioParameterFloat* toneParam;
    juce::AudioParameterFloat* formantParam;
    juce::AudioParameterFloat* resistanceParam;
    juce::AudioParameterFloat* vibratoParam;
    juce::AudioParameterFloat* masterGainParam;

    // Preset system
    std::vector<Preset> factoryPresets;
    Preset currentPreset;
    int currentPresetIndex;

    // State
    double sampleRate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BreathLeadPlugin)
};

//==============================================================================
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BreathLeadPlugin();
}
