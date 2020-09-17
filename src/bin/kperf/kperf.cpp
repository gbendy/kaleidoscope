#include "libkaleidoscope.h"
#include "libkio.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>

//#define HEURISTICS 1

#ifdef HEURISTICS
#include <thread>
#else
void report(const libkio::Frame& frame, std::size_t frame_count, const std::chrono::duration<float>& duration)
{
    std::cout << frame_count << "x" << frame.width << "x" << frame.height << " took " << duration.count() << "s" << std::endl;
    std::cout << "    " << static_cast<float>(frame_count) / duration.count() << " f/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height / 1000000.0f) / duration.count() << " megapixels/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height * frame.comp_size * frame.n_comp / 1000000.0f) / duration.count() << " megabytes/sec" << std::endl;
    std::cout << std::endl;
}
#endif
int main(int argc, char** argv)
{
    libkio::Frame frame_in(1920, 1080, 1, 4);
    libkio::Frame frame_out(1920, 1080, 1, 4);
    libkaleidoscope::Kaleidoscope k(frame_in.width, frame_in.height, frame_in.comp_size, frame_in.n_comp);
    if (argc > 1) {
        std::stringstream ss(argv[1]);
        std::uint32_t t;
        ss >> t;
        if (ss.fail() || !ss.eof()) {
            std::cerr << "usage: " << argv[0] << " [thread_count]" << std::endl;
            return 1;
        }
        k.set_threading(t);
    }
#ifdef HEURISTICS
    std::vector<std::int32_t> segs = { 2, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64 };
    std::vector<std::int32_t> threads = { 1, 2, 4, 8, 12, 16, 20, 24, 28, 32 };
#else
    std::vector<std::int32_t> segs = { 2, 4, 8, 12, 16, 24, 32, 64, 128 };
#endif
    std::chrono::duration<float> total(0);
    std::size_t total_frames(0);
#ifdef HEURISTICS
    std::cout << "threads: " << std::thread::hardware_concurrency();
    for (auto seg : segs) {
        std::cout << "," << seg;
    }
    std::cout << std::endl;
    for (auto t: threads) {
    k.set_threading(t);
    std::vector<std::chrono::duration<float>> totals;
#endif

    for (auto seg : segs) {
        k.set_segmentation(seg);
        // preprocess
        k.process(frame_in.data.get(), frame_out.data.get());

        std::chrono::duration<float> duration(0);

        std::size_t frame_count(100);
#ifndef HEURISTICS
        std::cout << frame_count << " tests at segmentation " << seg << " (" << frame_in.width << "," << frame_in.height << ")" << std::endl;
#endif
        for (std::size_t i = 0; i < frame_count; ++i) {
            auto start = std::chrono::steady_clock::now();
            k.process(frame_in.data.get(), frame_out.data.get());
            duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
        }
#ifdef HEURISTICS
        totals.push_back(duration);
#else
        report(frame_in, frame_count, duration);
#endif
        total += duration;
        total_frames += frame_count;
    }
#ifdef HEURISTICS
    std::cout << t;
    for (auto duration : totals) {
        std::cout << "," << duration.count();
    }
    std::cout << std::endl;
    }
#else
    report(frame_in, total_frames, total);
#endif
    return 0;
}
