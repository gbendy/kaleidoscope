#include "libkaleidoscope.h"
#include "libkio.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <sstream>
#include <thread>

void report(const libkio::Frame& frame, std::size_t frame_count, const std::chrono::duration<float>& duration)
{
    std::cout << frame_count << "x" << frame.width << "x" << frame.height << " took " << duration.count() << "s" << std::endl;
    std::cout << "    " << static_cast<float>(frame_count) / duration.count() << " f/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height / 1000000.0f) / duration.count() << " megapixels/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height * frame.comp_size * frame.n_comp / 1000000.0f) / duration.count() << " megabytes/sec" << std::endl;
    std::cout << std::endl;
}

void print_usage(const char* arg0)
{
    std::cerr << "usage: " << arg0 << " [-h] [-H] [-f frames] [-t threads]" << std::endl;
}

void print_help(const char* arg0)
{
    print_usage(arg0);
    std::cerr << std::endl;
    std::cerr << "    -H                enable heuristics mode" << std::endl;
    std::cerr << "    -f frames         number of frames to render            (default 100)" << std::endl;
    std::cerr << "    -t threads        number of threads in normal mode      (default 1)" << std::endl;
    std::cerr << "    -h                help" << std::endl;
}

#define VALIDATE_IDX(_msg) { if ((i) >= argc) { throw std::string(_msg); } }

int main(int argc, char** argv)
{
    libkio::Frame frame_in(1920, 1080, 1, 4);
    libkio::Frame frame_out(1920, 1080, 1, 4);
    libkaleidoscope::Kaleidoscope k(frame_in.width, frame_in.height, frame_in.comp_size, frame_in.n_comp);
    std::uint32_t n_threads(1);
    bool heuristics(false);
    std::uint32_t frame_count(100);
    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);
            if (arg == "-H") {
                heuristics = true;
            } else if (arg == "-f") {
                // frame count
                i++;
                VALIDATE_IDX("-f has no argument");
                std::stringstream ss(argv[i]);
                ss >> frame_count;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -f argument " + std::string(argv[i]) + " to an integer.";
                }
            } else if (arg == "-t") {
                // thread count
                i++;
                if (i >= argc) {
                    std::cerr << "Error processing command line: -t has no argument" << std::endl;
                    print_usage(argv[0]);
                    return 1;
                }
                std::stringstream ss(argv[i]);
                ss >> n_threads;
                if (ss.fail() || !ss.eof()) {
                    std::cerr << "Error processing command line: Could not convert -t argument " << argv[i] << " to an integer." << std::endl;
                    print_usage(argv[0]);
                    return 1;
                }
            } else if (arg == "-h") {
                print_help(argv[0]);
                return 1;
            }
        }
    } catch (const std::string& s) {
        std::cerr << "Error processing command line: " << s << std::endl;
        print_usage(argv[0]);
        return 1;

    }
    std::vector<std::int32_t> segs;
    std::vector<std::uint32_t> threads;

    if (heuristics) {
        segs = { 2, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64 };
        threads = { 1, 2, 4, 8, 12, 16, 20, 24, 28, 32 };
    } else {
        segs = { 2, 4, 8, 12, 16, 24, 32, 64, 128 };
        threads = { n_threads };
    }

    std::chrono::duration<float> total(0);
    std::size_t total_frames(0);
    if (heuristics) {
        std::cout << "threads:" << std::thread::hardware_concurrency();
        for (auto seg : segs) {
            std::cout << "," << seg;
        }
        std::cout << std::endl;
    }
    for (auto t: threads) {
        k.set_threading(t);
        std::vector<std::chrono::duration<float>> totals;

        for (auto seg : segs) {
            k.set_segmentation(seg);

            // preprocess
            k.process(frame_in.data.get(), frame_out.data.get());

            std::chrono::duration<float> duration(0);
            if (!heuristics) {
                std::cout << frame_count << " tests at segmentation " << seg << " (" << frame_in.width << "," << frame_in.height << ")" << std::endl;
            }
            for (std::size_t i = 0; i < frame_count; ++i) {
                auto start = std::chrono::steady_clock::now();
                k.process(frame_in.data.get(), frame_out.data.get());
                duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
            }
            if (heuristics) {
                totals.push_back(duration);
            } else {
                report(frame_in, frame_count, duration);
            }
            total += duration;
            total_frames += frame_count;
        }
        if (heuristics) {
            std::cout << t;
            for (auto duration : totals) {
                std::cout << "," << duration.count();
            }
            std::cout << std::endl;
        }
    }
    if (!heuristics) {
        report(frame_in, total_frames, total);
    }

    return 0;
}
