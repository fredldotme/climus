#include "progressoutput.h"

#include <iostream>

ProgressOutput::ProgressOutput(QObject *parent) : QObject(parent)
{

}

void ProgressOutput::handleProgressChange(const float progressPercent)
{
    const int progressBarMaxWidth = 20;
    const int progressBarEnd = static_cast<int>(progressBarMaxWidth *
                                                progressPercent);

    std::cout << "[";
    for (int i = 0; i < progressBarEnd; i++) {
        std::cout << "#";
    }
    for (int i = 0; i < progressBarMaxWidth - progressBarEnd; i++) {
        std::cout << " ";
    }
    std::cout << "]";
    std::cout << "\r";
    std::cout.flush();
}
