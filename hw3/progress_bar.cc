#include "progress_bar.h"
#include <iostream>

using namespace std::chrono;

ProgressBar::ProgressBar(int barLimit)
        :limit {barLimit}, width {70}
{
    current = 0;
    start_time = steady_clock::now();
}

ProgressBar::ProgressBar(int barLimit, int barWidth)
        :limit {barLimit}, width {barWidth}
{
    current = 0;
    start_time = steady_clock::now();
}

int ProgressBar::operator++()
{
    return ++current;
}

void ProgressBar::display()
{
    float progress = (float) current / limit;
    int pos = (int) (width * progress);
    steady_clock::time_point now = steady_clock::now();
    auto time_elapsed = duration_cast<milliseconds>(now-start_time).count();

    std::cout << "[";
    for (int i = 0; i < width; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << int(progress * 100.0) << "% "
              << float(time_elapsed) / 1000.0 << "s\r";
    std::cout.flush();
}
