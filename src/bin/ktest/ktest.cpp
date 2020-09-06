#include "libkaleidoscope.h"
#include "libio.h"

#include <memory>

int main(int argc, char** argv)
{
    std::uint32_t width(1920);
    std::uint32_t height(1080);
    std::uint32_t comp_size(1);
    std::uint32_t n_comp(3);
    libkaleidoscope::Kaleidoscope k(width,height,comp_size,n_comp);
    std::unique_ptr<std::uint8_t> frame(new uint8_t[width * height * comp_size * n_comp]);

    k.set_segmentation(4);

    // Test preferred corner and segmentation direction
    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
    
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
    k.process(frame.get(),frame.get());
    libio::write_pbm("tla.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.process(frame.get(), frame.get());
    libio::write_pbm("bla.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.process(frame.get(), frame.get());
    libio::write_pbm("bra.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.process(frame.get(), frame.get());
    libio::write_pbm("tra.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
    
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
    k.process(frame.get(), frame.get());
    libio::write_pbm("tlc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.process(frame.get(), frame.get());
    libio::write_pbm("blc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.process(frame.get(), frame.get());
    libio::write_pbm("brc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.process(frame.get(), frame.get());
    libio::write_pbm("trc.pbm", width, height, comp_size, n_comp, frame.get());

    return 0;
}