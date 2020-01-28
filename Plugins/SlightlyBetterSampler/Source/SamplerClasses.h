#pragma once

#include <JuceHeader.h>

class MySamplerSound : public SynthesiserSound
{
public:
	MySamplerSound(File f, int rootnote_) : midiRootNote(rootnote_)
	{
		setFile(f, rootnote_);
	}
	void setFile(File f, int rootNote)
	{
		AudioFormatManager man;
		man.registerBasicFormats();
		auto reader = man.createReaderFor(f);
		if (reader)
		{
			AudioBuffer<float> temp;
			temp.setSize(reader->numChannels, reader->lengthInSamples);
			reader->read(&temp, 0, reader->lengthInSamples, 0, true, true);
			{
				ScopedLock locker(cs);
				sampleRate = reader->sampleRate;
				length = reader->lengthInSamples;
				std::swap(temp, buffer);
			}
			delete reader;
		}
	}
	friend class MySamplerVoice;
private:
	AudioBuffer<float> buffer;
	int sampleRate = 0;
	bool looping = true;
	int midiRootNote = 60;
	int64 length = 0;
	bool appliesToNote(int midiNoteNumber) override;
	bool appliesToChannel(int midiChannel) override;
	CriticalSection cs;
};

class MySamplerVoice : public SynthesiserVoice
{
public:
	MySamplerVoice() 
	{
		envelope.setParameters({ 0.1,0.5,0.5,0.5 });
	}

	bool canPlaySound(SynthesiserSound *) override;
	void startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition) override;
	void stopNote(float velocity, bool allowTailOff) override;
	void pitchWheelMoved(int newPitchWheelValue) override;
	void controllerMoved(int controllerNumber, int newControllerValue) override;
	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
private:
	double sourcePosition = 0.0;
	MySamplerSound* currentSamplerSound = nullptr;
	ADSR envelope;
	int currentNote = -1;
	float velocity = 0.0f;
	double pitchRatio = 0.0;
};
