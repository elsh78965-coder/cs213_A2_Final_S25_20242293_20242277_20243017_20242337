#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;

    void saveLastSession();

private:
    juce::AudioSourcePlayer audioSourcePlayer;
    std::unique_ptr<juce::PropertiesFile> appProperties;

    PlayerGUI player1;
    PlayerGUI player2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
