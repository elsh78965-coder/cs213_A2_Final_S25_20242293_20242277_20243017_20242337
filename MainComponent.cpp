#include "MainComponent.h"

MainComponent::MainComponent()
{
    juce::PropertiesFile::Options options;
    options.applicationName = "MyAudioPlayer";
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Application Support";
    appProperties = std::make_unique<juce::PropertiesFile>(options);

    juce::String lastFilePath1 = appProperties->getValue("lastFilePath1", "");
    double lastPosition1 = appProperties->getDoubleValue("lastPosition1", 0.0);

    if (lastFilePath1.isNotEmpty())
    {
        juce::File file(lastFilePath1);
        if (file.existsAsFile())
        {
            player1.getPlayerAudio().loadFile(file);
            player1.getPlayerAudio().setPosition(lastPosition1);
        }
    }

    juce::String lastFilePath2 = appProperties->getValue("lastFilePath2", "");
    double lastPosition2 = appProperties->getDoubleValue("lastPosition2", 0.0);

    if (lastFilePath2.isNotEmpty())
    {
        juce::File file(lastFilePath2);
        if (file.existsAsFile())
        {
            player2.getPlayerAudio().loadFile(file);
            player2.getPlayerAudio().setPosition(lastPosition2);
        }
    }

    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    setSize(800, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    saveLastSession(); 
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::AudioBuffer<float> tempBuffer;
    tempBuffer.setSize(bufferToFill.buffer->getNumChannels(), bufferToFill.numSamples);
    tempBuffer.clear();

    juce::AudioSourceChannelInfo tempInfo(&tempBuffer, 0, bufferToFill.numSamples);

    player1.getNextAudioBlock(bufferToFill);

    player2.getNextAudioBlock(tempInfo);

    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
            tempBuffer, channel, 0, bufferToFill.numSamples);
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    auto halfHeight = getHeight() / 2;

    player1.setBounds(0, 0, getWidth(), halfHeight - 5);
    player2.setBounds(0, halfHeight + 5, getWidth(), halfHeight - 5);
}

void MainComponent::saveLastSession()
{
    if (player1.getPlayerAudio().isFileLoaded())
    {
        appProperties->setValue("lastFilePath1", player1.getPlayerAudio().getCurrentFile().getFullPathName());
        appProperties->setValue("lastPosition1", player1.getPlayerAudio().getPosition());
    }

    if (player2.getPlayerAudio().isFileLoaded())
    {
        appProperties->setValue("lastFilePath2", player2.getPlayerAudio().getCurrentFile().getFullPathName());
        appProperties->setValue("lastPosition2", player2.getPlayerAudio().getPosition());
    }

    appProperties->saveIfNeeded();
}
