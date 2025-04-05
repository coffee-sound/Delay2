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
    jassert(destination);   // parameter does not exist or wrong type
}

static juce::String stringFromMilliseconds(float value, int)
{
    if (value < 10.0f) {
        return juce::String(value, 2) + " ms";
    }
    else if (value < 100.0f) {
        return juce::String(value, 1) + " ms";
    }
    else if (value < 1000.0f) {
        return juce::String((int)value) + " ms";
    }
    else {
        return juce::String(value * 0.001f, 2) + " s";
    }
}

static juce::String stringFromDecibels(float value, int)
{
    return juce::String(value, 2) + " dB";
}

static juce::String stringFromPercent(float value, int)
{
    return juce::String(int(value)) + " %";
}

Parameters::Parameters(juce::AudioProcessorValueTreeState& apvts)
{
    // get parameters from apvts
    castParameter(apvts, gainParamID, gainParam);
    castParameter(apvts, delayTimeParamID, delayTimeParam);
    castParameter(apvts, mixParamID, mixParam);
}

juce::AudioProcessorValueTreeState::ParameterLayout Parameters::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        gainParamID,
        "Output Gain",
        juce::NormalisableRange<float>{ -12.0f, 12.0f },
        0.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromDecibels)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        delayTimeParamID,
        "Delay Time",
        juce::NormalisableRange<float>{ minDelayTime, maxDelayTime, 0.001f, 0.25f },
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromMilliseconds)
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        mixParamID,
        "Mix",
        juce::NormalisableRange<float>(0.0f, 100.0f, 1.0f),
        100.0f,
        juce::AudioParameterFloatAttributes().withStringFromValueFunction(stringFromPercent)
    ));

    return layout;
}

void Parameters::prepareToPlay(double sampleRate) noexcept
{
    double duration = 0.02; // transition time from the previous parameter value to the new one
    gainSmoother.reset(sampleRate, duration);

    coeff = 1.0f - std::exp(-1.0f / (0.2f * float(sampleRate)));

    mixSmoother.reset(sampleRate, duration);
}

void Parameters::reset() noexcept
{
    // to be safe
    gain = 0.0f;
    delayTime = 0.0f;
    mix = 0.0f;
    
    // loads the current parameter value (in other words zero)
    gainSmoother.setCurrentAndTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));
    mixSmoother.setCurrentAndTargetValue(mixParam->get() * 0.01f);
    
}

void Parameters::update() noexcept      // this function won't throw an exception
{
    // Hundle with Decibel Scale
    gainSmoother.setTargetValue(juce::Decibels::decibelsToGain(gainParam->get()));

    targetDelayTime = delayTimeParam->get();
    if (delayTime == 0.0f) {
        delayTime = targetDelayTime;
    }

    mixSmoother.setTargetValue(mixParam->get() * 0.01f);
}

void Parameters::smoothen() noexcept
{
    gain = gainSmoother.getNextValue();
    delayTime += (targetDelayTime - delayTime) * coeff;
    mix = mixSmoother.getNextValue();
}