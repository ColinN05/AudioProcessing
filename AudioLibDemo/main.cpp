#include "audiolibdemo.h"

#include "AudioLib/AudioFile.h"

#include <iostream>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioLibDemo w;
    w.show();

    return a.exec();
}
