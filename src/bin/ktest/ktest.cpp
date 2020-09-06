#include "libkaleidoscope.h"
#include <memory>
#include <fstream>

#include <climits>

template <typename T>
T swap_endian(T u)
{
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

void write_bs(std::ostream& str, std::uint32_t v)
{
    std::uint32_t outv(swap_endian(v));
    str.write(reinterpret_cast<char*>(&outv), 4);
}

void write_bs(std::ostream& str, float v)
{
    float outv(swap_endian(v));
    str.write(reinterpret_cast<char*>(&outv), 4);
}

// Write to the 'mig' file format, see https://rsdoc.migenius.com/doc/index.html#/resources/mig_image/index.html
void write_mig(const std::string& name,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t comp_size,
    std::uint32_t n_comp,
    void* data)
{
    std::ofstream out(name.c_str(), std::ios_base::binary | std::ios_base::out);
    out.write("mgtc", 4);
    std::uint32_t v(3);
    write_bs(out, v);
    write_bs(out, width);
    write_bs(out, height);
    v = 1; // 1 layer
    write_bs(out, v);
    v = 3; // uint8 component
    write_bs(out, v);
    write_bs(out, n_comp);
    v = 6; // Rgb
    write_bs(out, v);
    float gamma = 2.2f;
    write_bs(out, gamma);
    v = 0; // flags
    write_bs(out, v);
    v = 1; // mipmap levels
    write_bs(out, v);

    // and the image, which has to be bottom up
    std::uint32_t stride = width * n_comp * comp_size;
    char* row = reinterpret_cast<char*>(data) + (height-1) * stride;
    for (std::uint32_t y = 0; y < height; ++y, row-=stride) {
        out.write(row, stride);
    }
    out.close();
}

// Write to a PBM file
void write_pbm(const std::string& name,
                    std::uint32_t width,
                    std::uint32_t height,
                    std::uint32_t comp_size,
                    std::uint32_t n_comp,
                    void* data)
{
    std::ofstream out(name.c_str(), std::ios_base::binary | std::ios_base::out);
    out << "P6 " << width << " " << height << " 255 ";
    out.write(reinterpret_cast<char*>(data), width * height * comp_size * n_comp);
    out.close();
}

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
    write_pbm("tla.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.process(frame.get(), frame.get());
    write_pbm("bla.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.process(frame.get(), frame.get());
    write_pbm("bra.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.process(frame.get(), frame.get());
    write_pbm("tra.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
    
    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
    k.process(frame.get(), frame.get());
    write_pbm("tlc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
    k.process(frame.get(), frame.get());
    write_pbm("blc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
    k.process(frame.get(), frame.get());
    write_pbm("brc.pbm", width, height, comp_size, n_comp, frame.get());

    k.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
    k.process(frame.get(), frame.get());
    write_pbm("trc.pbm", width, height, comp_size, n_comp, frame.get());

    return 0;
}