/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Delay2AudioProcessorEditor::Delay2AudioProcessorEditor (Delay2AudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

Delay2AudioProcessorEditor::~Delay2AudioProcessorEditor()
{
}

//==============================================================================
void Delay2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::blue);
    g.setColour(juce::Colours::white);
    g.setFont(40.0f);
    g.drawFittedText("Delay Plug-in",
        getLocalBounds(), juce::Justification::centred, 1);
}

void Delay2AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
