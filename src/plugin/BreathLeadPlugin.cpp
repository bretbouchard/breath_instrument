/*
  BreathLeadPlugin.cpp - JUCE plugin factory
*/

#include "plugin/BreathLeadProcessor.h"

//==============================================================================
// This function creates the plugin instance
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BreathLeadProcessor();
}
