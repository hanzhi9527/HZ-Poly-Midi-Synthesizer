/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MIDISynth.h"

//==============================================================================
/**
*/
class HZ_Poly_Midi_SynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HZ_Poly_Midi_SynthesizerAudioProcessorEditor (HZ_Poly_Midi_SynthesizerAudioProcessor&);
    ~HZ_Poly_Midi_SynthesizerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    juce::Label messageLabel;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HZ_Poly_Midi_SynthesizerAudioProcessor& audioProcessor;
    
    juce::MidiKeyboardState midiKeyboardState;
    juce::MidiKeyboardComponent midiKeyboardComponent
    {
        midiKeyboardState, juce::MidiKeyboardComponent::horizontalKeyboard
    };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HZ_Poly_Midi_SynthesizerAudioProcessorEditor)
};
