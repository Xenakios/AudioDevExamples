
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SlightlyBetterSamplerAudioProcessor::SlightlyBetterSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
	samplerSound = new MySamplerSound(File("C:\\MusicAudio\\sourcesamples\\_70699__beskhu__sanzanais-72-c4-minibz-b.aif"), 60);
	synth.addSound(samplerSound);
	for (int i = 0; i < 16; ++i)
		synth.addVoice(new MySamplerVoice());
	
}

SlightlyBetterSamplerAudioProcessor::~SlightlyBetterSamplerAudioProcessor()
{
	
}

//==============================================================================
const String SlightlyBetterSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SlightlyBetterSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SlightlyBetterSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SlightlyBetterSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SlightlyBetterSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SlightlyBetterSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SlightlyBetterSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SlightlyBetterSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const String SlightlyBetterSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SlightlyBetterSamplerAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SlightlyBetterSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	synth.setCurrentPlaybackSampleRate(sampleRate);
}

void SlightlyBetterSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SlightlyBetterSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void SlightlyBetterSamplerAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

	synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SlightlyBetterSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SlightlyBetterSamplerAudioProcessor::createEditor()
{
    return new SlightlyBetterSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SlightlyBetterSamplerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SlightlyBetterSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

String SlightlyBetterSamplerAudioProcessor::importFile(File f)
{
	samplerSound->setFile(f, 60);
	return String();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SlightlyBetterSamplerAudioProcessor();
}
