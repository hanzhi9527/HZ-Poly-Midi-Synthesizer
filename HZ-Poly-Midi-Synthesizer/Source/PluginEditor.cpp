/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HZ_Poly_Midi_SynthesizerAudioProcessorEditor::HZ_Poly_Midi_SynthesizerAudioProcessorEditor (HZ_Poly_Midi_SynthesizerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
    
    
    addAndMakeVisible(midiKeyboardComponent);
    midiKeyboardComponent.setMidiChannel(2);
    midiKeyboardComponent.setVelocity(0.7f, true);
    midiKeyboardState.addListener(&audioProcessor.getMidiMessageCollector());
    
    messageLabel.setText("This is a Poly MIDI Synth.\n"
                         "It allows at most 5 keys to play at the same time.\n"
                         "C0 = Sine, C#0 = Square, D0 = Sawtooth.\n"
                         "Enjoy!",
                         juce::dontSendNotification);
    messageLabel.setFont(juce::Font(16.0f));
    messageLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(messageLabel);
}

HZ_Poly_Midi_SynthesizerAudioProcessorEditor::~HZ_Poly_Midi_SynthesizerAudioProcessorEditor()
{
}

//==============================================================================
void HZ_Poly_Midi_SynthesizerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!222222", getLocalBounds(), juce::Justification::centred, 1);
}

void HZ_Poly_Midi_SynthesizerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    // ... other component layouts ...
    messageLabel.setBounds(getLocalBounds().reduced(10));
    
    midiKeyboardComponent.setBounds (0, getLocalBounds().getHeight()*0.65, getLocalBounds().getWidth(), getLocalBounds().getHeight()*0.35);
}
