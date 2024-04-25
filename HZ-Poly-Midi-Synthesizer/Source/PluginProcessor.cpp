/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HZ_Poly_Midi_SynthesizerAudioProcessor::HZ_Poly_Midi_SynthesizerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    toneBank.reset(new ToneBank());
}

HZ_Poly_Midi_SynthesizerAudioProcessor::~HZ_Poly_Midi_SynthesizerAudioProcessor()
{
}

//==============================================================================
const juce::String HZ_Poly_Midi_SynthesizerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool HZ_Poly_Midi_SynthesizerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool HZ_Poly_Midi_SynthesizerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool HZ_Poly_Midi_SynthesizerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double HZ_Poly_Midi_SynthesizerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int HZ_Poly_Midi_SynthesizerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int HZ_Poly_Midi_SynthesizerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void HZ_Poly_Midi_SynthesizerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String HZ_Poly_Midi_SynthesizerAudioProcessor::getProgramName (int index)
{
    return {};
}

void HZ_Poly_Midi_SynthesizerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void HZ_Poly_Midi_SynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    toneBank->prepareToPlay(sampleRate);
//    DBG("ready to play");
    
    midiMessageCollector.reset(sampleRate);
}

void HZ_Poly_Midi_SynthesizerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool HZ_Poly_Midi_SynthesizerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void HZ_Poly_Midi_SynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    
    //4. Iterate over MIDI messages to handle note on and note off events
//    DBG("here!!");
    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());
    
    for (const auto in: midiMessages)
    {
//        DBG("here!!");
        auto msg = in.getMessage();
        if (msg.isNoteOn())
        {
//            DBG("here!!");
//            toneBank->setWaveType(Sine); // default wavetype
            if (msg.getNoteNumber()==24) //C0
                toneBank->setWaveType(Sine);
            else if (msg.getNoteNumber()==25) //C#0
                toneBank->setWaveType(Square);
            else if (msg.getNoteNumber()==26) //D0
                toneBank->setWaveType(Sawtooth);
            else
            {
                toneBank->noteOn((double)msg.getMidiNoteInHertz(msg.getNoteNumber()), msg.getFloatVelocity(), toneBank->getWaveType());
            }
        }
        else if (msg.isNoteOff())
        {
            toneBank->noteOff((double)msg.getMidiNoteInHertz(msg.getNoteNumber()));
            //new*** DBG("noteoff");
        }
    }
    //midiMessages.clear(); no need to clear it manually
    
    //DBG("before!");
    toneBank->renderBuffer(buffer);
    
    
    
}

//==============================================================================
bool HZ_Poly_Midi_SynthesizerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* HZ_Poly_Midi_SynthesizerAudioProcessor::createEditor()
{
    return new HZ_Poly_Midi_SynthesizerAudioProcessorEditor (*this);
}

//==============================================================================
void HZ_Poly_Midi_SynthesizerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void HZ_Poly_Midi_SynthesizerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new HZ_Poly_Midi_SynthesizerAudioProcessor();
}
