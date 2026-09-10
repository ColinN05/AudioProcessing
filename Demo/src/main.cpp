#include <iostream>

#include "AudioLib/AudioEffect.h"
#include "AudioLib/AudioFile.h"
#include "AudioLib/fft.h"

using namespace AudioLib;

int main()
{
    // AudioFile audioFile("/home/colin/dev/AudioProcessing/TestAudioFiles/test.wav");
    // std::cout << "File info:\n"
    //           << "\tFilepath: " << audioFile.GetFilepath() << '\n'
    //           << "\tType: " << audioFile.GetType() << '\n'
    //           << "\tChannels: " << audioFile.GetChannels() << '\n'
    //           << "\tSample Rate: " << audioFile.GetSampleRate() << '\n'
    //           << "\tFrame Count: " << audioFile.GetFrameCount() << '\n'
    //           << "\tDuration Seconds: " << audioFile.GetDurationSeconds() << '\n';
    
    // GainEffect tripleVolume("tripleVolume", 3.0f);
    // AudioFile audioFileTripleVolume = tripleVolume.Apply(audioFile);
    // audioFileTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_triple_volume.wav");
    
    // GainEffect oneThirdVolume("oneThirdVolume", 1/3.0f);
    // AudioFile audioFileOneThirdVolume = oneThirdVolume.Apply(audioFile);
    // audioFileOneThirdVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_one_third_volume.wav");

    // MonoToStereoEffect monoToStereo("monoToStereo", false);
    // AudioFile audioFileStereo = monoToStereo.Apply(audioFile);
    // audioFileStereo.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo.wav");

    // StereoToMonoEffect stereoToMono("stereoToMono");
    // AudioFile audioFileMono = stereoToMono.Apply(audioFileStereo);
    // audioFileMono.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_mono.wav");

    // constexpr float pi = 3.1415926f;

    // PanEffect panRight("panRight", 3*pi/4);
    // AudioFile audioFilePannedRight = panRight.Apply(audioFile);
    // audioFilePannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_right.wav");
    // AudioFile audioFileStereoPannedRight = panRight.Apply(audioFileStereo);
    // audioFileStereoPannedRight.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_right.wav");

    // PanEffect panLeft("panLeft", pi/4);
    // AudioFile audioFilePannedLeft = panLeft.Apply(audioFile);
    // audioFilePannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left.wav");
    // AudioFile audioFileStereoPannedLeft = panLeft.Apply(audioFileStereo);
    // audioFileStereoPannedLeft.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_stereo_panned_left.wav");

    // ClipEffect volumeClip("volumeClip", 0.005f);
    // AudioFile audioFileVolumeClipped = volumeClip.Apply(audioFile);
    // audioFileVolumeClipped.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_volume_clipped.wav");

    // GainEffect increaseVolume("increaseVolume", 1/0.005f);
    // auto audioFileIncreasedVolume = increaseVolume.Apply(audioFileVolumeClipped);
    // audioFileIncreasedVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_increased_volume.wav");

    // CompositeAudioEffect panLeftAndTripleVolume(
    //     "panLeftAndTripleVolume",
    //     std::make_unique<PanEffect>("panLeft", 3*pi/4),
    //     std::make_unique<GainEffect>("tripleVolume", 3.0f)
    // );
    // AudioFile audioFilePannedLeftAndTripleVolume = panLeftAndTripleVolume.Apply(audioFile);
    // audioFilePannedLeftAndTripleVolume.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_panned_left_and_triple_volume.wav");

    // AudioFile audioFile2("/home/colin/dev/AudioProcessing/TestAudioFiles/test2.wav");
    // MixEffect mixer("mixer", &audioFile2, 0.5f, -2.0f);
    // AudioFile audioFileMixed = mixer.Apply(audioFile);
    // audioFileMixed.Write("/home/colin/dev/AudioProcessing/TestAudioFiles/test_mixed.wav");

    MonoToStereoEffect m2s("m2s", false);

    AudioFile af1m("/home/colin/dev/AudioProcessing/TestAudioFiles/test.wav");
    AudioFile af1s = m2s.Apply(af1m); 

    AudioFile af2m("/home/colin/dev/AudioProcessing/TestAudioFiles/test2.wav");
    AudioFile af2s = m2s.Apply(af2m);

    for (int mixerChannels : {1,2})
    {
        for (int channels : {1,2})
        {
            for (float mix : {0.25f,0.75f})
            {
                for (float offsetSeconds : {-1.0,0.0,1.0})
                {
                    MixEffect mixer("mixer", (mixerChannels==1) ? &af2m : &af2s, mix, offsetSeconds);
                    AudioFile afMixed = mixer.Apply((channels==1) ? af1m : af1s);
                    std::string filename = "/home/colin/dev/AudioProcessing/OutputAudioFiles/mixed";
                    filename += ((mixerChannels==1) ? "m" : "s");
                    filename += ((channels==1) ? "m" : "s");
                    filename += std::to_string(mix) + "_" + std::to_string(offsetSeconds) + ".wav";
                    std::cout << "Writing file to " << filename << '\n';
                    afMixed.Write(filename);
                }
            }
        }
    }    

    ResampleEffect downsampler("downsampler", 4'500);
    AudioFile af1sDownsampled = downsampler.Apply(af1s);
    af1sDownsampled.Write("/home/colin/dev/AudioProcessing/OutputAudioFiles/stereo_downsampled.wav");

    ResampleEffect upsampler("upsampler", 44'100);
    AudioFile af1mUpsampled = upsampler.Apply(af1m);
    af1mUpsampled.Write("/home/colin/dev/AudioProcessing/OutputAudioFiles/mono_upsampled.wav");

    CompressorEffect compressor("compressor", 0.0001f, 4.0f, 1.0f, 0.01f);
    AudioFile af1mCompressed = compressor.Apply(af1m);
    af1mCompressed.Write("/home/colin/dev/AudioProcessing/OutputAudioFiles/mono_compressed.wav");
    AudioFile af1sCompressed = compressor.Apply(af1s);
    af1mCompressed.Write("/home/colin/dev/AudioProcessing/OutputAudioFiles/stereo_compressed.wav");

    std::vector<float> samples = {1,2,3,4};
    std::vector<complex> fftsamples = fft(samples);
    for (complex c : fftsamples)
    {
        std::cout << c << '\n';
    }
    std::vector<complex> a = fft(fftsamples, true);
    for (complex c : a)
    {
        std::cout << c << '\n'; 
    }
}