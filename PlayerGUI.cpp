
#include "PlayerGUI.h"

// ===== Playlist functions =====
void PlayerGUI::addTrackToPlaylist(const juce::File& file)
{
    if (file.existsAsFile())
    {
        playlist.push_back(file);
        playlistList.updateContent();
        playlistList.repaint();
    }
}

void PlayerGUI::playCurrentTrack()
{
    if (!playlist.empty() && currentTrackIndex >= 0 && currentTrackIndex < (int)playlist.size())
    {
        bool loaded = playerAudio.loadFile(playlist[currentTrackIndex]);
        if (loaded)
        {
            waveform.clear();
            hasWaveform = false;

            auto file = playlist[currentTrackIndex];
            if (file.existsAsFile())
            {
                waveform.setSource(new juce::FileInputSource(file));
                hasWaveform = true;
            }


            titleLabel.setText("Title: " + playerAudio.getCurrentTitle(), juce::dontSendNotification);
            artistLabel.setText("Artist: " + playerAudio.getCurrentArtist(), juce::dontSendNotification);
            albumLabel.setText("Album: " + playerAudio.getCurrentAlbum(), juce::dontSendNotification);
            playerAudio.start();
        }
    }
}

void PlayerGUI::nextTrack()
{
    if (!playlist.empty())
    {
        currentTrackIndex = (currentTrackIndex + 1) % playlist.size();
        playCurrentTrack();
    }
}

void PlayerGUI::previousTrack()
{
    if (!playlist.empty())
    {
        currentTrackIndex = (currentTrackIndex - 1 + playlist.size()) % playlist.size();
        playCurrentTrack();
    }
}

// ===== Audio callbacks =====
void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}



void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    auto box = juce::Rectangle<int>(20, 300, 600, 100);

    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillRect(box);

    g.setColour(juce::Colours::white);
    g.drawRect(box, 2);

    if (hasWaveform)
    {
        auto waveformArea = box.reduced(5);
        g.setColour(juce::Colours::blueviolet);

        waveform.drawChannels(g, waveformArea, 0.0, playerAudio.getLength(), 1.0f);

        // ====== Playhead ======
        double length = playerAudio.getLength();
        double pos = playerAudio.getPosition();

        if (length > 0.0)
        {
            float x = waveformArea.getX() + (float)(pos / length) * waveformArea.getWidth();
            g.setColour(juce::Colours::red);
            g.drawLine(x, waveformArea.getY(), x, waveformArea.getBottom(), 2.0f);
        }
    }
    else
    {
        g.setColour(juce::Colours::white);
        g.drawText("No Track Loaded", box, juce::Justification::centred);
    }
}


// ===== Timer Callback =====
void PlayerGUI::timerCallback()
{
    double pos = playerAudio.getPosition();
    double length = playerAudio.getLength();
    int minutes = static_cast<int>(pos) / 60;
    int seconds = static_cast<int>(pos) % 60;
    timeLabel.setText(juce::String::formatted("%02d:%02d", minutes, seconds), juce::dontSendNotification);

    if (length > 0.0)
        positionSlider.setValue(pos / length, juce::dontSendNotification);

    repaint();

}

// ===== Slider Changed =====
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &positionSlider)
    {
        double newPos = slider->getValue() * playerAudio.getLength();
        playerAudio.setPosition(newPos);
    }
    else if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
    else if (slider == &speedSlider)
        playerAudio.setSpeed((float)slider->getValue());
}

// ===== Constructor =====


PlayerGUI::PlayerGUI()
{
    formatManagerForMeta.registerBasicFormats();

    // ===== TextButtons =====
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &playButton, &muteButton,
                       &forwardButton, &rewindButton, &nextButton, &prevButton,
                       &setAButton, &setBButton, &addMarkerButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // ===== ToggleButtons =====
    for (auto* tbtn : { &loopButton, &segmentLoopButton })
    {
        tbtn->addListener(this);
        addAndMakeVisible(tbtn);
    }
    loopButton.setClickingTogglesState(true);

    // ===== Sliders =====
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    volumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    volumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(volumeSlider);

    speedSlider.setRange(0.25, 2.0, 0.25);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0);
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    // ===== Labels =====
    timeLabel.setText("00:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(timeLabel);

    titleLabel.setText("Title: -", juce::dontSendNotification);
    artistLabel.setText("Artist: -", juce::dontSendNotification);
    albumLabel.setText("Album: -", juce::dontSendNotification);
    addAndMakeVisible(titleLabel);
    addAndMakeVisible(artistLabel);
    addAndMakeVisible(albumLabel);

    isMuted = false;
    savedGain = (float)volumeSlider.getValue();

    startTimer(500);

    // ===== Marker List =====
    markerList.setModel(this);
    addAndMakeVisible(markerList);

    // ===== Playlist List =====
    playlistListModel = std::make_unique<PlaylistListModel>(playlist);
    playlistListModel->onItemClicked = [this](int index)
        {
            currentTrackIndex = index;
            playCurrentTrack();
        };
    playlistList.setModel(playlistListModel.get());
    addAndMakeVisible(playlistList);

    startTimerHz(30);  

}
// ===== Resized =====



void PlayerGUI::resized()
{
    int y = 60;
    positionSlider.setBounds(20, y, 600, 20);
    timeLabel.setBounds(positionSlider.getRight() + 10, y, 60, 20);

    volumeSlider.setBounds(640, 100, 40, 150);
    speedSlider.setBounds(690, 100, 40, 150);

    int yButtons = 200;
    loadButton.setBounds(1000, 20, 80, 30);
    restartButton.setBounds(380, 250, 80, 30);
    stopButton.setBounds(200, yButtons, 80, 30);
    playButton.setBounds(290, yButtons, 80, 30);
    muteButton.setBounds(470, 250, 80, 30);
    loopButton.setBounds(560, yButtons, 80, 30);
    rewindButton.setBounds(20, yButtons, 80, 30);
    forwardButton.setBounds(470, yButtons, 80, 30);
    prevButton.setBounds(110, yButtons, 80, 30);
    nextButton.setBounds(380, yButtons, 80, 30);

    // A-B + Segment Loop
    int yAB = 250;
    setAButton.setBounds(20, yAB, 80, 30);
    setBButton.setBounds(110, yAB, 80, 30);
    segmentLoopButton.setBounds(200, yAB, 120, 30);

    // Meta Labels
    int yMeta = 300;
    titleLabel.setBounds(20, 80, getWidth() - 40, 20);
    artistLabel.setBounds(20, 80 + 25, getWidth() - 40, 20);
    albumLabel.setBounds(20, 80 + 50, getWidth() - 40, 20);

    // Marker Button + List

    int yLine = 380;
    int spacing = 40;

    addMarkerButton.setBounds(750, 20, 100, 30);
    markerList.setBounds(750, 50, 240, 300);
    playlistList.setBounds(1000, 50, 500, 300);

}


// ===== Destructor =====
PlayerGUI::~PlayerGUI()
{
    stopTimer();

    // ===== TextButtons =====
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &playButton, &muteButton,
                       &forwardButton, &rewindButton, &nextButton, &prevButton,
                       &setAButton, &setBButton, &addMarkerButton })
    {
        btn->removeListener(this);
    }

    // ===== ToggleButtons =====
    for (auto* tbtn : { &loopButton, &segmentLoopButton })
    {
        tbtn->removeListener(this);
    }

    volumeSlider.removeListener(this);
    speedSlider.removeListener(this);
    positionSlider.removeListener(this);
}
// ===== Button Clicked =====
void PlayerGUI::buttonClicked(juce::Button* button)
{

    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select Audio Files", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& fc)
            {
                auto files = fc.getResults();
                for (auto& file : files)
                {
                    if (file.existsAsFile())
                        addTrackToPlaylist(file);
                }
                if (!playlist.empty())
                {
                    currentTrackIndex = playlist.size() - files.size();
                    playCurrentTrack();
                }
            });
    }

    else if (button == &nextButton) nextTrack();
    else if (button == &prevButton) previousTrack();
    else if (button == &restartButton) { playerAudio.setPosition(0.0); playerAudio.start(); }
    else if (button == &stopButton) playerAudio.stop();
    else if (button == &playButton) playerAudio.start();
    else if (button == &muteButton)
    {
        if (!isMuted)
        {
            savedGain = (float)volumeSlider.getValue();
            playerAudio.setGain(0.0f);
            isMuted = true;
            muteButton.setButtonText("Unmute");
        }
        else
        {
            playerAudio.setGain(savedGain);
            volumeSlider.setValue(savedGain, juce::dontSendNotification);
            isMuted = false;
            muteButton.setButtonText("Mute");
        }
    }
    else if (button == &loopButton)
    {
        bool shouldLoop = loopButton.getToggleState();
        playerAudio.setLooping(shouldLoop);
        loopButton.setButtonText(shouldLoop ? "Loop:On" : "Loop:Off");
    }
    else if (button == &forwardButton)
    {
        double newPos = playerAudio.getPosition() + 10.0;
        if (newPos > playerAudio.getLength()) newPos = playerAudio.getLength();
        playerAudio.setPosition(newPos);
    }
    else if (button == &rewindButton)
    {
        double newPos = playerAudio.getPosition() - 10.0;
        if (newPos < 0.0) newPos = 0.0;
        playerAudio.setPosition(newPos);
    }
    else if (button == &setAButton) playerAudio.setLoopPoints(playerAudio.getPosition(), playerAudio.getLoopEnd());
    else if (button == &setBButton) playerAudio.setLoopPoints(playerAudio.getLoopStart(), playerAudio.getPosition());
    else if (button == &segmentLoopButton) playerAudio.enableSegmentLoop(segmentLoopButton.getToggleState());
    else if (button == &addMarkerButton)
    {
        double currentPos = playerAudio.getPosition();
        markers.push_back(currentPos);
        juce::String markerLabel = "Marker " + juce::String(markers.size()) +
            " (" + juce::String(int(currentPos / 60)) + ":" +
            juce::String(int(currentPos) % 60).paddedLeft('0', 2) + ")";
        markerNames.push_back(markerLabel);
        markerList.updateContent();
        markerList.repaint();
    }
}

// ===== ListBoxModel (Markers) =====
int PlayerGUI::getNumRows() { return (int)markerNames.size(); }

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowNumber < markerNames.size())
    {
        if (rowIsSelected) g.fillAll(juce::Colours::lightyellow);
        g.setColour(juce::Colours::black);
        g.drawText(markerNames[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft);
    }
}

void PlayerGUI::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    if (row < markers.size())
        playerAudio.setPosition(markers[row]);
}

