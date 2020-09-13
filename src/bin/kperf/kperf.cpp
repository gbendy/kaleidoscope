#include "libkaleidoscope.h"
#include "libio.h"

#include <chrono>
#include <iostream>
#include <vector>

void report(const libio::Frame& frame, std::size_t frame_count, const std::chrono::duration<float>& duration)
{
    std::cout << frame_count << "x" << frame.width << "x" << frame.height << " took " << duration.count() << "s" << std::endl;
    std::cout << "    " << static_cast<float>(frame_count) / duration.count() << " f/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height / 1000000.0f) / duration.count() << " megapixels/sec" << std::endl;
    std::cout << "    " << (frame_count * frame.width * frame.height * frame.comp_size * frame.n_comp / 1000000.0f) / duration.count() << " megabytes/sec" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv)
{
    libio::Frame frame_in(1920, 1080, 1, 4);
    libio::Frame frame_out(1920, 1080, 1, 4);
    libkaleidoscope::Kaleidoscope k(frame_in.width, frame_in.height, frame_in.comp_size, frame_in.n_comp);
    
    std::vector<std::int32_t> segs = { 2, 4, 8, 12, 16, 32, 64, 128 };
    std::chrono::duration<float> total(0);
    std::size_t total_frames(0);
    for (auto seg : segs) {
        k.set_segmentation(seg);
        // preprocess
        k.process(frame_in.data.get(), frame_out.data.get());

        //std::chrono::microseconds duration(0);
        std::chrono::duration<float> duration(0);

        std::size_t frame_count = 10;
        std::cout << frame_count << " tests at segmentation " << seg << " (" << frame_in.width << "," << frame_in.height << ")" << std::endl;
        for (std::size_t i = 0; i < frame_count; ++i) {
            auto start = std::chrono::steady_clock::now();
            k.process(frame_in.data.get(), frame_out.data.get());
            duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
        }
        report(frame_in, frame_count, duration);

        total += duration;
        total_frames += frame_count;
    }
    report(frame_in, total_frames, total);
    return 0;
}
