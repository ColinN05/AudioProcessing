#include "audiolibdemo.h"

#include "AudioLib/AudioFile.h"

#include <iostream>
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    AudioLibDemo w;
    w.show();
    std::cout << "Welcome to AudioLibDemo.\n";
    std::cout << "Select a .wav file and apply some of the effects in the left panel.\n";
    return a.exec();
}
