#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlaylistListModel : public juce::ListBoxModel
{
public:
    PlaylistListModel(std::vector<juce::File>& playlistRef) : playlist(playlistRef) {}

    int getNumRows() override { return static_cast<int>(playlist.size()); }

    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override
    {
        if (rowNumber < playlist.size())
        {
            if (rowIsSelected)
                g.fillAll(juce::Colours::darkorange);

            g.setColour(juce::Colours::white);
            g.drawText(playlist[rowNumber].getFileName(), 2, 0, width - 4, height, juce::Justification::centredLeft);
        }
    }

    void listBoxItemClicked(int row, const juce::MouseEvent&) override
    {
        if (row < playlist.size())
        {
            if (onItemClicked)
                onItemClicked(row);
        }
    }

    std::function<void(int)> onItemClicked;

private:
    std::vector<juce::File>& playlist;
};


class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer,
    public juce::ListBoxModel
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    // Audio callbacks
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    // GUI overrides
    void paint(juce::Graphics& g) override;
    void resized() override;

    // Callbacks
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    // ===== ListBoxModel overrides =====
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;

    PlayerAudio& getPlayerAudio() { return playerAudio; }

    // ===== Playlist functions =====
    void addTrackToPlaylist(const juce::File& file);
    void playCurrentTrack();
    void nextTrack();
    void previousTrack();




private:
    PlayerAudio playerAudio;

    // Buttons
    juce::TextButton loadButton{ "Load" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playButton{ "Play" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton forwardButton{ ">>10s" };
    juce::TextButton rewindButton{ "<<10s" };
    juce::ToggleButton loopButton{ "Loop" };

    // Playlist navigation buttons
    juce::TextButton nextButton{ "Next" };
    juce::TextButton prevButton{ "Previous" };

    // Sliders
    juce::Slider volumeSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    // Labels
    juce::Label timeLabel;
    juce::Label titleLabel;
    juce::Label artistLabel;
    juce::Label albumLabel;

    // Segment loop buttons
    juce::TextButton setAButton{ "Set A" };
    juce::TextButton setBButton{ "Set B" };
    juce::ToggleButton segmentLoopButton{ "Segment Loop" };

    // Track Markers
    std::vector<double> markers;
    std::vector<juce::String> markerNames;
    juce::TextButton addMarkerButton{ "Add Marker" };
    juce::ListBox markerList;

    // Playlist data
    std::vector<juce::File> playlist;
    int currentTrackIndex = 0;

    bool isMuted = false;
    float savedGain = 0.5f;

    juce::ListBox playlistList;
    std::unique_ptr<PlaylistListModel> playlistListModel;

    juce::AudioFormatManager formatManagerForMeta;
    std::unique_ptr<juce::FileChooser> fileChooser;


    // ===== Waveform =====
    juce::AudioThumbnailCache waveformCache{ 5 };
    juce::AudioThumbnail waveform{ 512, formatManagerForMeta, waveformCache };
    bool hasWaveform = false;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
