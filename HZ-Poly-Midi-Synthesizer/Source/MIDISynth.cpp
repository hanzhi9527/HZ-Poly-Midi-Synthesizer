/*
  ==============================================================================

    MIDISynth.cpp
    Created: 29 Nov 2023 4:09:42pm
    Author:  Hanzhi Zhang

  ==============================================================================
*/

#include "MIDISynth.h"

Tone::Tone(float frequency, float velocity, WaveType waveType, double sampleRate)
{
    this->frequency = frequency;
    this->velocity = velocity;
    this->waveType = waveType;
    this->sampleRate = sampleRate;
    gain = velocity/127.0f;
}

Tone::~Tone()
{
    
}

void Tone::setWaveType(WaveType newWaveType)
{
    waveType = newWaveType;
}

void Tone::setFrequency(double newFrequency)
{
    frequency = newFrequency;
}

void Tone::setGain(double newGain)
{
    gain = newGain;
}

bool Tone::shouldBeRemoved() const
{
    if (gain < threshold)
        return true;
    else
        return false;
}

void Tone::updateTone()
{
    if (isReleased == false)
    {
        if (gain <= velocity)
            gain *= ATTACK_FACTOR;
    }
    else if (isReleased == true)
    {
        gain *= DECAY_FACTOR;
        //DBG("gain = %f" << gain);
    }
}

void Tone::setReleased()
{
    isReleased = true;
}

double Tone::generateWaveSample()
{
    double gainFactor = 0.3;
    if (waveType == Sine)
    {
        auto sample = std::sin(2*PI*frequency*counter/sampleRate);
        counter++;
        return (double)sample;
    }
    else if (waveType == Sawtooth)
    {
        auto sample = 2*(frequency*counter/sampleRate - std::floor(0.5+frequency*counter/sampleRate));
        counter++;
        return (double)sample*gainFactor;
    }
    else if (waveType == Square)
    {
        if (std::sin(2*PI*frequency*counter/sampleRate)<0)
        {
            counter++;
            return (double)-1*gainFactor;
        }
        else if (std::sin(2*PI*frequency*counter/sampleRate)>0)
        {
            counter++;
            return (double)1*gainFactor;
        }
        else if (std::sin(2*PI*frequency*counter/sampleRate)==0)
        {
            counter++;
            return 0;
        }
    }
    counter++;
    //if (counter >= 2*PI) //if counter reaches a full 2PI cycle
    if (2*PI*frequency*counter/sampleRate >= 2*PI)
        counter = 0;

    return 0;
}

void Tone::processSample(float& sample)
{
    sample += gain * generateWaveSample();
}

ToneBank::ToneBank()
{
    
}

ToneBank::~ToneBank()
{
    

}

void ToneBank::prepareToPlay(double newSampleRate)
{
    sampleRate = newSampleRate;
    
//    for (auto& i: tones)
//        i->sampleRate = sa
}

void ToneBank::setSampleRate(double sampleRate)
{
    this->sampleRate = sampleRate;
}

void ToneBank::setWaveType(WaveType newWaveType)
{
    waveType = newWaveType;
}

WaveType ToneBank::getWaveType()
{
    return waveType;
}

void ToneBank::noteOn(float frequency, float velocity, WaveType newWaveType)
{
    if (tones.size()>=5)
        return;
    
    for (auto &i: tones)
    {
        if (i->frequency==frequency)
            return;
    }
    tones.push_back(std::make_unique<Tone>(frequency, velocity, newWaveType, this->sampleRate));
}

void ToneBank::noteOff(float freq)
{
    for (auto &i: tones)
    {
        if (i->frequency == freq)
        {
//            i.reset();
//            i.release();
            i->setReleased();
        }
    }
}

void ToneBank::renderBuffer(juce::AudioBuffer<float>& buffer)
{
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        if (channel > 0)
        {
            buffer.copyFrom(channel, 0, buffer, 0, 0, buffer.getNumSamples());
            break;
        }

        float* samples = buffer.getWritePointer(channel);
        for (int j = 0; j < buffer.getNumSamples(); j++)
        {
            float mixedSample = 0.0f;
            auto counter = tones.begin();
            for (auto& i : tones)
            {
                float tempSample = 0.0f;
                i->processSample(tempSample);
                mixedSample += tempSample;  // Apply amplitude scaling

                i->updateTone();
                if (i->shouldBeRemoved())
                {
                    tones.erase(counter);
                    break;
                }
                counter++;
            }
            samples[j] = mixedSample;
        }
    }
}
