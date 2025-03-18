/*
  ==============================================================================

    Parameters.cpp
    Created: 18 Mar 2025 12:01:06pm
    Author:  coffee_sound_

  ==============================================================================
*/

#include "Parameters.h"

template<typename T>
static void castParameter(juce::AudioProcessorValueTreeState& apvts, const juce::ParameterID& id, T& destination)
{
    destination = dynamic_cast<T>(apvts.getParameter(id.getParamID()));
    jassert(destination);   // parameter doues not exist or wrong type
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    // get parameters from apvts
    castParameter(apvts, gainParamID, gainParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float>{ -12.0f, 12.0f },
        0.0f));

    return layout;
}

void Parameters::update() noexcept      // this function won't throw an exception
{
    // Hundle with Decibel Scale
    gain = juce::Decibels::decibelsToGain(gainParam->get());  // convert decibel into linear units
}