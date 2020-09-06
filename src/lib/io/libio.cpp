#include "libio.h"
#include <fstream>
#include <climits>

namespace libio {

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
}
