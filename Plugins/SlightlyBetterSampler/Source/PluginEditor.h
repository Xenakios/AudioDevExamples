

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SlightlyBetterSamplerAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SlightlyBetterSamplerAudioProcessorEditor (SlightlyBetterSamplerAudioProcessor&);
    ~SlightlyBetterSamplerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    SlightlyBetterSamplerAudioProcessor& processor;
	TextButton importButton;
	ToggleButton loopButton;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SlightlyBetterSamplerAudioProcessorEditor)
};
