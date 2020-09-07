#include "libkaleidoscope.h"
#include "libio.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 3) {
        std::cerr << argv[0] << ": usage infile outfile" << std::endl;
        return -1;
    }
    libio::Frame frame(libio::read_pbm(argv[1]));
    if (frame.width == 0) {
        std::cerr << "Error: unable to read file " << argv[1] << std::endl;
        return -2;
    }
    libkaleidoscope::Kaleidoscope k(frame.width, frame.height, frame.comp_size, frame.n_comp);
    libio::Frame out_frame(frame.width, frame.height, frame.comp_size, frame.n_comp);

    k.set_segmentation(16);
    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);

    k.process(frame.data.get(), out_frame.data.get());
    libio::write_pbm(argv[2], out_frame);

    std::string outfile(argv[2]);
    std::size_t i = outfile.rfind('.');
    std::string file2(outfile.substr(0, i) + "_ac.pbm");

    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
    k.process(frame.data.get(), out_frame.data.get());
    libio::write_pbm(file2.c_str(), out_frame);

    return 0;
}