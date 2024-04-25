/*
  ==============================================================================

    MIDISynth.h
    Created: 29 Nov 2023 4:09:42pm
    Author:  Hanzhi Zhang

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>


#define PI 3.14
#define ATTACK_FACTOR 1.05
#define DECAY_FACTOR 0.95
enum WaveType { Sine, Square, Sawtooth };

class Tone {
public:
    Tone(float frequency, float velocity, WaveType waveType, double sampleRate);
    ~Tone();
    void setSampleRate(double newSampleRate);
    void setWaveType(WaveType newWaveType);
    void setFrequency(double newFrequency);
    void setGain(double newGain);
    void setReleased();
    void updateTone();
    void processSample(float& sample);
    bool shouldBeRemoved() const;
    double frequency;

private:
    WaveType waveType;
    
    bool isReleased = false; // note held by default
    double gain=0.01, velocity; // gain is current velocity
    long long counter = 0; // internal state counter
    double sampleRate;
    double generateWaveSample();
    double threshold = 0.00005;
};


class ToneBank {
public:
ToneBank();
~ToneBank();
    void prepareToPlay(double newSampleRate);
    void setSampleRate(double sampleRate);
    void setWaveType(WaveType waveType);
    void noteOn(float frequency, float velocity, WaveType waveType);
    WaveType getWaveType();
    void noteOff(float freq);
    void renderBuffer(juce::AudioBuffer<float>& buffer);
//
private:
    std::vector<std::unique_ptr<Tone>> tones;
    WaveType waveType = Sine;
    double sampleRate;
};


