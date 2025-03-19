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

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02; // transition time from the previous parameter value to the new one
    gainSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    // to be safe
    gain = 0.0f;
    // loads the current parameter value (in other words zero)
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters::update() noexcept      // this function won't throw an exception
{
    // Hundle with Decibel Scale
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
}