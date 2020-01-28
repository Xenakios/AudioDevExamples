

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SlightlyBetterSamplerAudioProcessorEditor::SlightlyBetterSamplerAudioProcessorEditor (SlightlyBetterSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	addAndMakeVisible(importButton);
	importButton.setButtonText("Import...");
	importButton.onClick = [this]() 
	{
		FileChooser myChooser("Please select audio file...",
			File::getSpecialLocation(File::userHomeDirectory),
			"*.wav");

		if (myChooser.browseForFileToOpen())
		{
			processor.importFile(myChooser.getResult());
			
		}
	};
    setSize (400, 300);
}

SlightlyBetterSamplerAudioProcessorEditor::~SlightlyBetterSamplerAudioProcessorEditor()
{
}

//==============================================================================
void SlightlyBetterSamplerAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::grey);
}

void SlightlyBetterSamplerAudioProcessorEditor::resized()
{
	importButton.setBounds(1, 1, 100, 20);
}
