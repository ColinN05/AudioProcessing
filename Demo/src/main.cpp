#include <iostream>

#include "AudioLib/AudioEffect.h"
#include "AudioLib/AudioFile.h"

using namespace AudioLib;

int main()
{
    AudioFile audioFile("/home/colin/dev/AudioProcessing/TestAudioFiles/test.wav");
    std::cout << "File info:\n"
              << "\tFilepath: " << audioFile.GetFilepath() << '\n'
              << "\tType: " << audioFile.GetType() << '\n'
              << "\tChannels: " << audioFile.GetChannels() << '\n'
              << "\tSample Rate: " << audioFile.GetSampleRate() << '\n'
              << "\tFrame Count: " << audioFile.GetFrameCount() << '\n'
              << "\tDuration Seconds: " << audioFile.GetDurationSeconds() << '\n';
    
    GainEffect tripleVolume("tripleVolume", 3.0f);
    AudioFile audioFileTripleVolume = tripleVolume.Apply(audioFile);
    audioFileTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_triple_volume.wav");
    
    GainEffect oneThirdVolume("oneThirdVolume", 1/3.0f);
    AudioFile audioFileOneThirdVolume = oneThirdVolume.Apply(audioFile);
    audioFileOneThirdVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_one_third_volume.wav");

    MonoToStereoEffect monoToStereo("monoToStereo", false);
    AudioFile audioFileStereo = monoToStereo.Apply(audioFile);
    audioFileStereo.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo.wav");

    StereoToMonoEffect stereoToMono("stereoToMono");
    AudioFile audioFileMono = stereoToMono.Apply(audioFileStereo);
    audioFileMono.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_mono.wav");

    constexpr float pi = 3.1415926f;

    PanEffect panRight("panRight", 3*pi/4);
    AudioFile audioFilePannedRight = panRight.Apply(audioFile);
    audioFilePannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_right.wav");
    AudioFile audioFileStereoPannedRight = panRight.Apply(audioFileStereo);
    audioFileStereoPannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_right.wav");

    PanEffect panLeft("panLeft", pi/4);
    AudioFile audioFilePannedLeft = panLeft.Apply(audioFile);
    audioFilePannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left.wav");
    AudioFile audioFileStereoPannedLeft = panLeft.Apply(audioFileStereo);
    audioFileStereoPannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_left.wav");

    ClipEffect volumeClip("volumeClip", 0.005f);
    AudioFile audioFileVolumeClipped = volumeClip.Apply(audioFile);
    audioFileVolumeClipped.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_volume_clipped.wav");

    GainEffect increaseVolume("increaseVolume", 1/0.005f);
    auto audioFileIncreasedVolume = increaseVolume.Apply(audioFileVolumeClipped);
    audioFileIncreasedVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_increased_volume.wav");

    CompositeAudioEffect panLeftAndTripleVolume(
        "panLeftAndTripleVolume",
        std::make_unique<PanEffect>("panLeft", 3*pi/4),
        std::make_unique<GainEffect>("tripleVolume", 3.0f)
    );
    AudioFile audioFilePannedLeftAndTripleVolume = panLeftAndTripleVolume.Apply(audioFile);
    audioFilePannedLeftAndTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left_and_triple_volume.wav");

    AudioFile audioFile2("/home/colin/dev/AudioProcessing/TestAudioFiles/test2.wav");
    MixEffect mixer("mixer", &audioFile2, 0.5f, 1.0f);
    AudioFile audioFileMixed = mixer.Apply(audioFile);
    audioFileMixed.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_mixed.wav");
}