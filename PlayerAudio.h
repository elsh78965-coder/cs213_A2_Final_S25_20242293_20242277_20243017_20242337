#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    void setLooping(bool shouldLoop);
    bool isLooping() const;

    void setSpeed(float ratio);

    // ===== Metadata =====
    juce::String currentTitle = "Unknown";
    juce::String currentArtist = "Unknown";
    juce::String currentAlbum = "Unknown";

    // ===== Getters =====
    juce::String getCurrentTitle() const { return currentTitle; }
    juce::String getCurrentArtist() const { return currentArtist; }
    juce::String getCurrentAlbum() const { return currentAlbum; }
    juce::File getCurrentFile() const { return currentFile; }
    bool isFileLoaded() const { return currentFile.existsAsFile(); }

    // A-B segment loop setters
    void setLoopPoints(double start, double end);
    void enableSegmentLoop(bool shouldLoop);

    double getLoopStart() const { return loopStart; }
    double getLoopEnd() const { return loopEnd; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    juce::File currentFile;

    // A-B segment looping
    double loopStart = 0.0;
    double loopEnd = 0.0;
    bool isSegmentLooping = false;

    std::unique_ptr<juce::ResamplingAudioSource> resampler;

    bool userLooping = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio)
};
