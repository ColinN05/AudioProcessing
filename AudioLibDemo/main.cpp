#include "audiolibdemo.h"

#include "AudioLib/AudioFile.h"

#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioLibDemo w;
    w.show();

    AudioLib::AudioFile audioFile("/home/colin/dev/AudioProcessing/TestAudioFiles/test.wav");
    std::cout << "Sample rate = " << audioFile.GetSampleRate() << '\n';

    return a.exec();
}
