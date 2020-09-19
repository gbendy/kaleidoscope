#include "ikaleidoscope.h"
#include "libkio.h"
#include <iostream>
#include <iomanip>
#include <sstream>

struct Options {
    std::string in_file;
    std::string out_file;

    std::int32_t segmentation;
    libkaleidoscope::IKaleidoscope::Direction direction;

    float origin_x;
    float origin_y;

    libkaleidoscope::IKaleidoscope::Corner corner;
    libkaleidoscope::IKaleidoscope::Direction corner_direction;

    std::uint8_t bg_colour[3];

    bool bg_set;

    std::uint32_t edge_threshold;

    float start_angle;

    std::uint32_t threads;

    bool use_reflections;

    Options():
        segmentation(16),
        direction(libkaleidoscope::IKaleidoscope::Direction::NONE),
        origin_x(0.5f),
        origin_y(0.5f),
        corner(libkaleidoscope::IKaleidoscope::Corner::BL),
        corner_direction(libkaleidoscope::IKaleidoscope::Direction::CLOCKWISE),
        bg_set(false),
        edge_threshold(0),
        start_angle(-1),
        threads(0),
        use_reflections(false)
    {
        bg_colour[0] = 0xff;
        bg_colour[1] = 0x00;
        bg_colour[2] = 0xff;
    }
};

void print_usage(const char* arg0)
{
    std::cerr << "usage: " << arg0 << " [-h] [-s segmentation] [-d c|cw|ccw] [-x origin_x] [-y origin_y] [-c tl|tr|bl|br] [-cd cw|ccw] [-b rrggbb] [-e threshold] [-a angle] [-t threads] [-r] infile outfile" << std::endl;
}

std::string to_string(libkaleidoscope::IKaleidoscope::Direction d)
{
    if (d == libkaleidoscope::IKaleidoscope::Direction::NONE) return "c";
    if (d == libkaleidoscope::IKaleidoscope::Direction::CLOCKWISE) return "cw";
    return "ccw";
}

std::string to_string(libkaleidoscope::IKaleidoscope::Corner c)
{
    if (c == libkaleidoscope::IKaleidoscope::Corner::TL) return "tl";
    if (c == libkaleidoscope::IKaleidoscope::Corner::TR) return "tr";
    if (c == libkaleidoscope::IKaleidoscope::Corner::BL) return "bl";
    return "br";
}

std::string to_string(std::uint8_t bg_colour[3])
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<std::int32_t>(bg_colour[0]);
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<std::int32_t>(bg_colour[1]);
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<std::int32_t>(bg_colour[2]);
    
    return ss.str();
}
void print_help(const char* arg0)
{
    print_usage(arg0);
    Options o;
    std::cerr << std::endl;
    std::cerr << "    -s  segmentation  kaleidoscope segmentation             (default " << o.segmentation << ")" << std::endl;
    std::cerr << "    -d  c|cw|ccw      kaleidoscope direction                (default " << to_string(o.direction) << ")" << std::endl;
    std::cerr << "    -x  float         x centre point of kaleidoscope        (default " << o.origin_x << ")" << std::endl;
    std::cerr << "    -y  float         y centre point of kaleidoscope        (default " << o.origin_x << ")" << std::endl;
    std::cerr << "    -c  tl|tr|bl|br   preferred corner of source segment    (default " << to_string(o.corner) << ")" << std::endl;
    std::cerr << "    -cd cw|ccw        search direction for source segment   (default " << to_string(o.corner_direction) << ")" << std::endl;
    std::cerr << "    -b  rrggbb        background colour as a hex color code (default " << to_string(o.bg_colour) << ")" << std::endl;
    std::cerr << "    -e  integer       edge threshold                        (default " << o.edge_threshold << ")" << std::endl;
    std::cerr << "    -a  float         start angle in degrees" << std::endl;
    std::cerr << "    -t  integer       number of threads to use.             (default " << o.threads << ")" << std::endl;
    std::cerr << "    -r                calculate using actual reflections" << std::endl;
    std::cerr << "    infile            input PBM (P6) image" << std::endl;
    std::cerr << "    outfile           output PBM (P6) image" << std::endl;
}

#define VALIDATE_IDX(_msg) { if ((i) >= argc) { throw std::string(_msg); } }

Options parse_options(int argc, char** argv)
{
    Options options;
    try {
        for (int i = 1; i < argc; i++) {
            std::string arg = argv[i];
            if (arg == "-s") {
                // segmentation
                i++;
                VALIDATE_IDX("-s has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.segmentation;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -s argument " + std::string(argv[i]) + " to an integer.";
                }
            } else if (arg == "-d") {
                // direction
                i++;
                VALIDATE_IDX("-d has no argument");
                std::string value(argv[i]);
                if (value != "c" && value != "cw" && value != "ccw") {
                    throw "-d argument " + std::string(argv[i]) + " is not c cw or ccw.";
                } else if (value == "c") {
                    options.direction = libkaleidoscope::IKaleidoscope::Direction::NONE;
                } else if (value == "cw") {
                    options.direction = libkaleidoscope::IKaleidoscope::Direction::CLOCKWISE;
                } else {
                    options.direction = libkaleidoscope::IKaleidoscope::Direction::ANTICLOCKWISE;
                }
            } else if (arg == "-x") {
                // x origin
                i++;
                VALIDATE_IDX("-x has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.origin_x;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -x argument " + std::string(argv[i]) + " to a float.";
                }
            } else if (arg == "-y") {
                // y origin
                i++;
                VALIDATE_IDX("-y has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.origin_y;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -y argument " + std::string(argv[i]) + " to a float.";
                }
            } else if (arg == "-c") {
                // direction
                i++;
                VALIDATE_IDX("-c has no argument");
                std::string value(argv[i]);
                if (value == "tl") {
                    options.corner = libkaleidoscope::IKaleidoscope::Corner::TL;
                } else if (value == "tr") {
                    options.corner = libkaleidoscope::IKaleidoscope::Corner::TR;
                } else if (value == "bl") {
                    options.corner = libkaleidoscope::IKaleidoscope::Corner::BL;
                } else if (value == "br") {
                    options.corner = libkaleidoscope::IKaleidoscope::Corner::BR;
                } else {
                    throw "-c argument " + std::string(argv[i]) + " is not tl, tr, bl or br.";
                }
            } else if (arg == "-cd") {
                // direction
                i++;
                VALIDATE_IDX("-cd has no argument");
                std::string value(argv[i]);
                if (value != "cw" && value != "ccw") {
                    throw "-cd argument " + std::string(argv[i]) + " is not cw or ccw.";
                } else if (value == "cw") {
                    options.corner_direction = libkaleidoscope::IKaleidoscope::Direction::CLOCKWISE;
                } else {
                    options.corner_direction = libkaleidoscope::IKaleidoscope::Direction::ANTICLOCKWISE;
                }
            } else if (arg == "-b") {
                // direction
                i++;
                VALIDATE_IDX("-b has no argument");
                std::string value(argv[i]);
                if (value[0] == '#') {
                    value = value.substr(1);
                } else if (value.substr(0, 2) == "0x" || value.substr(0, 2) == "0X") {
                    value = value.substr(2);
                }
                if (value.length() != 6) {
                    throw "-b argument " + std::string(argv[i]) + " is not a colour code.";
                }
                try {
                    std::string comp(value.substr(0, 2));
                    std::size_t end;
                    unsigned long v = std::stoul(comp, &end, 16);
                    if (end != 2) {
                        throw "-b argument " + std::string(argv[i]) + " is not a colour code.";
                    }
                    options.bg_colour[0] = static_cast<std::uint8_t>(v);
                    
                    comp = value.substr(2, 2);                    
                    v = std::stoul(comp, &end, 16);
                    if (end != 2) {
                        throw "-b argument " + std::string(argv[i]) + " is not a colour code.";
                    }
                    options.bg_colour[1] = static_cast<std::uint8_t>(v);
                    comp = value.substr(4, 2);
                    v = std::stoul(comp, &end, 16);
                    if (end != 2) {
                        throw "-b argument " + std::string(argv[i]) + " is not a colour code.";
                    }
                    options.bg_colour[2] = static_cast<std::uint8_t>(v);
                } catch (const std::exception&) {
                    throw "-b argument " + std::string(argv[i]) + " is not a colour code.";
                }
                options.bg_set = true;
            } else if (arg == "-e") {
                // edge threshold
                i++;
                VALIDATE_IDX("-e has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.edge_threshold;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -e argument " + std::string(argv[i]) + " to an integer.";
                }
            } else if (arg == "-a") {
                // start angle
                i++;
                VALIDATE_IDX("-a has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.start_angle;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -a argument " + std::string(argv[i]) + " to a float.";
                }
            } else if (arg == "-t") {
                // thread count
                i++;
                VALIDATE_IDX("-t has no argument");
                std::stringstream ss(argv[i]);
                ss >> options.threads;
                if (ss.fail() || !ss.eof()) {
                    throw "Could not convert -t argument " + std::string(argv[i]) + " to an integer.";
                }
            } else if (arg == "-r") {
                options.use_reflections = true;
            } else if (arg == "-h") {
                print_help(argv[0]);
                return Options();
            } else if (arg[0] == '-') {
                throw "unsupported argument " + arg;
            } else if (options.in_file.empty()) {
                options.in_file = arg;
            } else if (options.out_file.empty()) {
                options.out_file = arg;
            } else {
                throw std::string("more than two filenames provided");
            }
        }
    } catch (const std::string &s) {
        std::cerr << "Error processing command line: " << s << std::endl;
        print_usage(argv[0]);
        return Options();

    }
    if (options.in_file == "") {
        std::cerr << "Error: No input file specified." << std::endl;
        print_usage(argv[0]);
        return Options();
    }
    if (options.out_file == "") {
        std::cerr << "Error: No output file specified." << std::endl;
        print_usage(argv[0]);
        return Options();
    }
    return options;
}

int main(int argc, char** argv)
{

    Options opts(parse_options(argc, argv));
    if (opts.in_file == "") {
        return -1;
    }

    libkio::Frame frame(libkio::read_pbm(opts.in_file.c_str()));
    if (frame.width == 0) {
        std::cerr << "Error: unable to read file " << opts.in_file << std::endl;
        return -2;
    }
    std::unique_ptr<libkaleidoscope::IKaleidoscope> k(libkaleidoscope::IKaleidoscope::factory(frame.width, frame.height, frame.comp_size, frame.n_comp));
    libkio::Frame out_frame(frame.width, frame.height, frame.comp_size, frame.n_comp);

    k->set_segmentation(opts.segmentation);
    k->set_segment_direction(opts.direction);
    k->set_origin(opts.origin_x, opts.origin_y);
    k->set_preferred_corner(opts.corner);
    k->set_preferred_corner_search_direction(opts.corner_direction);
    k->set_reflect_edges(!opts.bg_set);
    k->set_background_colour(opts.bg_colour);
    k->set_edge_threshold(opts.edge_threshold);
    k->set_source_segment(opts.start_angle * 3.14159254f / 180);
    k->set_threading(opts.threads);
    k->use_reflection(opts.use_reflections);

    k->process(frame.data.get(), out_frame.data.get());
    libkio::write_pbm(opts.out_file.c_str(), out_frame);

    return 0;
}
