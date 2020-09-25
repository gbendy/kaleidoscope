#include "libkio.h"

#include <iostream>
#include <cmath>

void print_usage(const char* arg0)
{
    std::cerr << "usage: " << arg0 << " reference_image compare_image [out]" << std::endl;
}

std::uint32_t err(std::uint8_t *a, std::uint8_t *b)
{
    return static_cast<std::uint32_t>(std::abs(std::int32_t(*a) - std::int32_t(*b)));
}
int main(int argc, char** argv)
{
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    libkio::Frame img1(libkio::read_pbm(argv[1]));
    if (img1.width == 0) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    libkio::Frame img2(libkio::read_pbm(argv[2]));
    if (img2.width == 0) {
        std::cerr << "Error opening file: " << argv[2] << std::endl;
        print_usage(argv[0]);
        return 1;
    }

    if (img1.width != img2.width || img1.height != img2.height) {
        std::cout << "different resolution " << img1.width << "x" << img1.height << " " << img2.width << "x" << img2.height << std::endl;
        return 0;
    }
    std::uint8_t *i1 = img1.data.get();
    std::uint8_t* i2 = img2.data.get();

    std::uint32_t max_err(0);
    std::size_t cumulative_err(0);
    std::size_t cumulative_squared_err(0);
    std::size_t count_err(0);
    std::uint32_t pixel_size(img1.comp_size * img1.n_comp);
    std::size_t pixel_count(img1.width * static_cast<std::size_t>(img1.height));

    for (std::uint32_t i = 0; i < pixel_count; ++i) {
        std::uint32_t errs[4] = {
            err(i1++,i2++),
            err(i1++,i2++),
            err(i1++,i2++),
            0
        };
        if (pixel_size > 3) {
            errs[3] = err(i1++, i2++);
        }

        std::uint32_t diff = std::max(std::max(std::max(errs[0], errs[1]), errs[2]), errs[3]);
        if (diff) {
            max_err = std::max(max_err, diff);
            cumulative_err += diff;
            cumulative_squared_err += static_cast<std::size_t>(diff) * diff;
            count_err++;

            *(i1 - pixel_size) = 255;
        }
    }
    if (cumulative_err == 0) {
        std::cout << "identical" << std::endl;
    } else {
        std::cout << 100 * count_err / static_cast<float>(pixel_count) << "% " <<
            " MAX: " << max_err << 
            " MAE: " << cumulative_err / static_cast<float>(pixel_count) << 
            " MSE: " << cumulative_squared_err / static_cast<float>(pixel_count) << 
            " RMSE: " << std::sqrt(cumulative_squared_err / static_cast<float>(pixel_count)) <<
            " PSNR: " << 10 * std::log10(255 * 255 / (cumulative_squared_err / static_cast<float>(pixel_count))) <<
            " of " << count_err << " errors." << std::endl;
    }
    if (argc > 3) {
        libkio::write_pbm(argv[3], img1);
    }
    return cumulative_err ? 1 : 0;
}
