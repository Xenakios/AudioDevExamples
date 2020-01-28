#pragma once

#include <JuceHeader.h>

class MySamplerSound : public SynthesiserSound
{
public:
	MySamplerSound(File f, int rootnote_, Range<int> noteRange_) : midiRootNote(rootnote_), noteRange(noteRange_)
	{
		setFile(f, rootnote_);
	}
	void setFile(File f, int rootNote);
	void setLooping(bool b) { looping = b; }
	bool isLooping() const { return looping; }
	friend class MySamplerVoice;
private:
	AudioBuffer<float> buffer;
	int sampleRate = 0;
	bool looping = true;
	int midiRootNote = 60;
	Range<int> noteRange;
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
		envelope.setParameters({ 0.05,0.5,0.5,0.5 });
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
