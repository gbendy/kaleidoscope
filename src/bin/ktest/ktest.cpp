#include "libkaleidoscope.h"
#include "libio.h"

int main(int argc, char** argv)
{
    libio::Frame frame(1920, 1080, 1, 3);
    libkaleidoscope::Kaleidoscope k(frame.width, frame.height, frame.comp_size, frame.n_comp);

    k.set_segmentation(4);

    // Test preferred corner and segmentation direction
    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
    
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
    k.visualise(frame.data.get());
    libio::write_pbm("tla.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.visualise( frame.data.get());
    libio::write_pbm("bla.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.visualise( frame.data.get());
    libio::write_pbm("bra.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.visualise( frame.data.get());
    libio::write_pbm("tra.pbm", frame);

    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
    
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
    k.visualise( frame.data.get());
    libio::write_pbm("tlc.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.visualise( frame.data.get());
    libio::write_pbm("blc.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.visualise( frame.data.get());
    libio::write_pbm("brc.pbm", frame);

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.visualise( frame.data.get());
    libio::write_pbm("trc.pbm", frame);

    return 0;
}