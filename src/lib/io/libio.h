#ifndef LIBIO_LIBIO_H
#define LIBIO_LIBIO_H 1

#include <string>

namespace libio {

    
/// Write to the 'mig' file format, see https://rsdoc.migenius.com/doc/index.html#/resources/mig_image/index.html
/// \param filename to write to, better be writable because we don't check
/// \param width the image width
/// \param height the image height
/// \param comp_size the pixel componenet size in bytes
/// \param n_comp the number of componenets per pixel
/// \param data the pixel data.
void write_mig(const std::string& filename,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t comp_size,
    std::uint32_t n_comp,
    void* data);

/// Write to a PBM file
/// \param filename to write to, better be writable because we don't check
/// \param width the image width
/// \param height the image height
/// \param comp_size the pixel componenet size in bytes
/// \param n_comp the number of componenets per pixel
/// \param data the pixel data.
void write_pbm(const std::string& name,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t comp_size,
    std::uint32_t n_comp,
    void* data);

}
#endif 

