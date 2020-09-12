#include "libkaleidoscope.h"
#include "libio.h"

#include <chrono>
#include <iostream>
#include <vector>

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

        std::size_t frame_count = 100;
        std::cout << frame_count << " tests at segmentation " << seg << " (" << frame_in.width << "," << frame_in.height << ")" << std::endl;
        for (std::size_t i = 0; i < frame_count; ++i) {
            auto start = std::chrono::steady_clock::now();
            k.process(frame_in.data.get(), frame_out.data.get());
            duration += std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
        }
        std::cout << frame_count << "x" << frame_in.width << "x" << frame_in.height << " took " << duration.count() << "s" << std::endl;
        std::cout << "    " << static_cast<float>(frame_count) / duration.count() << " f/sec" << std::endl;
        std::cout << "    " << (frame_count * frame_in.width * frame_in.height) / duration.count() << " pixels/sec" << std::endl;
        std::cout << "    " << (frame_count * frame_in.width * frame_in.height * frame_in.comp_size * frame_in.n_comp) / duration.count() << " bytes/sec" << std::endl;
        std::cout << std::endl;
        total += duration;
        total_frames += frame_count;
    }
    std::cout << total_frames << "x" << frame_in.width << "x" << frame_in.height << " took " << total.count() << "s" << std::endl;
    std::cout << "    " << static_cast<float>(total_frames) / total.count() << " f/sec" << std::endl;
    std::cout << "    " << (total_frames * frame_in.width * frame_in.height) / total.count() << " pixels/sec" << std::endl;
    std::cout << "    " << (total_frames * frame_in.width * frame_in.height * frame_in.comp_size * frame_in.n_comp) / total.count() << " bytes/sec" << std::endl;
    std::cout << std::endl;
    return 0;
}
