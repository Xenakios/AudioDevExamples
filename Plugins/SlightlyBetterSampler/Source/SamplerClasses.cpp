#include "SamplerClasses.h"

bool MySamplerVoice::canPlaySound(SynthesiserSound *)
{
	return true;
}

void MySamplerVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound * sound, int currentPitchWheelPosition)
{
	currentSamplerSound = dynamic_cast<MySamplerSound*>(sound);
	jassert(currentSamplerSound);
	envelope.noteOn();
	sourcePosition = 0.0;
	envelope.setSampleRate(getSampleRate());
	currentNote = midiNoteNumber;
	pitchRatio = std::pow(2.0, (midiNoteNumber - currentSamplerSound->midiRootNote) / 12.0)
		* currentSamplerSound->sampleRate / getSampleRate();
}

void MySamplerVoice::stopNote(float velocity, bool allowTailOff)
{
	if (allowTailOff)
	{
		envelope.noteOff();
	}
	else
	{
		clearCurrentNote();
		envelope.reset();
		currentSamplerSound = nullptr;
	}
}

void MySamplerVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void MySamplerVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void MySamplerVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (currentSamplerSound == nullptr)
		return;
	auto& data = currentSamplerSound->buffer;
	const float* const inL = data.getReadPointer(0);
	const float* const inR = data.getNumChannels() > 1 ? data.getReadPointer(1) : nullptr;

	float* outL = outputBuffer.getWritePointer(0, startSample);
	float* outR = outputBuffer.getNumChannels() > 1 ? outputBuffer.getWritePointer(1, startSample) : nullptr;
	bool isLooping = currentSamplerSound->looping;
	while (--numSamples >= 0)
	{
		auto pos = (int)sourcePosition;
		auto alpha = (float)(sourcePosition - pos);
		auto invAlpha = 1.0f - alpha;

		// just using a very simple linear interpolation here..
		float l = (inL[pos] * invAlpha + inL[pos + 1] * alpha);
		float r = (inR != nullptr) ? (inR[pos] * invAlpha + inR[pos + 1] * alpha)
			: l;

		auto envelopeValue = envelope.getNextSample();

		float lgain = 1.0f;
		float rgain = 1.0f;

		l *= lgain * envelopeValue;
		r *= rgain * envelopeValue;

		if (outR != nullptr)
		{
			*outL++ += l;
			*outR++ += r;
		}
		else
		{
			*outL++ += (l + r) * 0.5f;
		}

		sourcePosition += pitchRatio;

		if (sourcePosition > currentSamplerSound->length)
		{
			if (isLooping)
			{
				sourcePosition = 0.0;
			}
			else
			{
				stopNote(0.0f, false);
				break;
			}
		}
	}

}

void MySamplerSound::setFile(File f, int rootNote)
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

bool MySamplerSound::appliesToNote(int midiNoteNumber)
{
	return noteRange.contains(midiNoteNumber);
}

bool MySamplerSound::appliesToChannel(int midiChannel)
{
	return true;
}
