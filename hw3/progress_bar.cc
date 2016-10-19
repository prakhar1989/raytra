#include "progress_bar.h"
#include <iostream>

ProgressBar::ProgressBar(int barLimit)
        :limit {barLimit}, width {70}
{
    current = 0;
}

ProgressBar::ProgressBar(int barLimit, int barWidth)
        :limit {barLimit}, width {barWidth}
{
    current = 0;
}

int ProgressBar::operator++()
{
    return ++current;
}

void ProgressBar::display()
{
    float progress = (float) current / limit;
    int pos = (int) (width * progress);

    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}
