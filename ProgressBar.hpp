/**
 *
 *  Progress-Cpp library (http://github.com/prakhar1989/progress-cpp)
 *
 *  (The MIT License)
 *
 *  Copyright (c) 2016 Prakhar Srivastav <prakhar@prakhar.me>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  'Software'), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 *  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 *  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef PROGRESSBAR_PROGRESSBAR_HPP
#define PROGRESSBAR_PROGRESSBAR_HPP

#include <chrono>
#include <iostream>

class ProgressBar {
private:
    /* ticks completed */
    int current;

    const int barTotal;
    const int barWidth;
    const char completeChar;
    const char incompleteChar;
    const std::chrono::steady_clock::time_point start_time;
public:
    ProgressBar(int total, int width):
            barTotal {total}, barWidth {width}, completeChar {'='},
            incompleteChar {' '}, start_time {std::chrono::steady_clock::now()}
    {
        current = 0;
    }
    ProgressBar(int total, int width, char complete, char incomplete) :
            barTotal {total}, barWidth {width}, completeChar {complete},
      incompleteChar {incomplete}, start_time {std::chrono::steady_clock::now()}
    {
        current = 0;
    }

    int operator++()
    {
        return ++current;
    }

    void display()
    {
        float progress = (float) current / barTotal;
        int pos = (int) (barWidth * progress);

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now-start_time).count();

        std::cout << "[";

        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << completeChar;
            else if (i == pos) std::cout << ">";
            else std::cout << incompleteChar;
        }
        std::cout << "] " << int(progress * 100.0) << "% "
                  << float(time_elapsed) / 1000.0 << "s\r";
        std::cout.flush();
    }

    void done()
    {
        display();
        std::cout << std::endl;
    }
};

#endif //PROGRESSBAR_PROGRESSBAR_HPP
