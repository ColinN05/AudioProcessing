#include "audiolibdemo.h"

#include "AudioLib/AudioFile.h"

#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioLibDemo w;
    w.show();
    std::cout << "Welcome to AudioLibDemo.\n";
    return a.exec();
}
