#ifndef LIBIO_LIBIO_H
#define LIBIO_LIBIO_H 1

#include <string>
#include <memory>

namespace libio {

/// An image frame and it's related information
/// \param width the image width
/// \param height the image height
/// \param comp_size the pixel component size in bytes
/// \param n_comp the number of components per pixel
/// \param data the pixel data.
struct Frame {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t comp_size;
    std::uint32_t n_comp;
    std::unique_ptr<std::uint8_t> data;

    Frame(std::uint32_t _width,
            std::uint32_t _height,
            std::uint32_t _comp_size,
            std::uint32_t _n_comp):
        width(_width),
        height(_height),
        comp_size(_comp_size),
        n_comp(_n_comp),
        data(new std::uint8_t[width * height * comp_size * n_comp])
    {
    }
};

/// Write to the 'mig' file format, see https://rsdoc.migenius.com/doc/index.html#/resources/mig_image/index.html
/// \param filename to write to, better be writable because we don't check
/// \param frame the frame to write
void write_mig(const std::string& filename, const Frame& frame);

/// Write to a PBM file
/// \param filename to write to, better be writable because we don't check
/// \param frame the frame to write
void write_pbm(const std::string& filename, const Frame& frame);

}
#endif 

