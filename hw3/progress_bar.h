#ifndef HW_PROGRESS_BAR_H
#define HW_PROGRESS_BAR_H
#include <chrono>

/**
 * A simple progress bar class that displays a pretty
 * progress bar.
 */
class ProgressBar {
private:
    const int limit;
    const int width;
    std::chrono::steady_clock::time_point start_time;
    int current;
public:
    ProgressBar(int barLimit);
    ProgressBar(int barLimit, int barWidth);
    int operator++();
    void display();
};

#endif //HW_PROGRESS_BAR_H
