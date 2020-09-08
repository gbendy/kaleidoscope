extern "C" {
#include "frei0r.h"
}

#include "libkaleidoscope.h"
#include <string>

struct Instance {
    libkaleidoscope::Kaleidoscope kaleidoscope;

    std::uint32_t width;
    std::uint32_t height;

    std::uint8_t bg_colour[4];

    void set_segmentation(double v) {
        kaleidoscope.set_segmentation(static_cast<std::uint32_t>(v * 128));
    }
    double get_segmentation() {
        return kaleidoscope.get_segmentation() / 128.0;
    }

    void set_edge_threshold(double v) {
        kaleidoscope.set_edge_threshold(static_cast<std::uint32_t>(v * 4));
    }
    double get_edge_threshold() {
        return kaleidoscope.get_edge_threshold() / 4.0;
    }

    Instance(std::uint32_t _width, std::uint32_t _height) :
        kaleidoscope(_width,_height,1,4),
        width(_width),
        height(_height)
    {
        bg_colour[0] = 0xff;
        bg_colour[1] = 0x00;
        bg_colour[2] = 0xff;
        bg_colour[3] = 0xff;

        kaleidoscope.set_origin(0.5f, 0.5f);
        kaleidoscope.set_segmentation(16);
        kaleidoscope.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        kaleidoscope.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
        kaleidoscope.set_preferred_corner_search_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        kaleidoscope.set_edge_threshold(0);
        kaleidoscope.set_background_colour(bg_colour);
    }
};

extern "C" {

int f0r_init()
{
    return 1;
}

void f0r_deinit()
{
}

void f0r_get_plugin_info(f0r_plugin_info_t* info)
{
    info->name = "Kaleid0sc0pe";
    info->author = "Brendan Hack";
    info->plugin_type = F0R_PLUGIN_TYPE_FILTER;
    info->color_model = F0R_COLOR_MODEL_RGBA8888;
    info->frei0r_version = FREI0R_MAJOR_VERSION;
    info->major_version = 1;
    info->minor_version = 0;
    info->num_params = 8;
    info->explanation = "Applies a kaleidoscope effect to a source image";
}

void f0r_get_param_info(f0r_param_info_t* info, int param_index)
{
    switch (param_index) {
    case(0): {
        info->name = "origin";
        info->type = F0R_PARAM_POSITION;
        info->explanation = "origin of the kaleidoscope. default 0.5,0.5";
        break;
    }
    case(1): {
        info->name = "segmentation";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "kaleidoscope segmentation / 128, segmentations of 1, 2 or multiples of 4 work best. default 16/128";
        break;
    }
    case(2): {
        info->name = "direction";
        info->type = F0R_PARAM_STRING;
        info->explanation = "direction to reflect in, either 'cw' or 'ccw'. default 'cw'";
        break;
    }
    case(3): {
        info->name = "edge_threshold";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "edge threshold / 4, reflections outside the image but within this distance clamp to the edge. default 0";
        break;
    }
    case(4): {
        info->name = "preferred_corner";
        info->type = F0R_PARAM_STRING;
        info->explanation = "preferred corner for source segment, either 'tl', 'tr', 'bl' or 'br'. default 'bl'";
        break;
    }
    case(5): {
        info->name = "corner_search";
        info->type = F0R_PARAM_STRING;
        info->explanation = "direction to search in to find furthest corner, either 'cw' or 'ccw'. default 'cw'";
        break;
    }
    case(6): {
        info->name = "bg_color";
        info->type = F0R_PARAM_COLOR;
        info->explanation = "color to use if reflection lies outside of source image. default 1,0,1";
        break;
    }
    case(7): {
        info->name = "bg_alpha";
        info->type = F0R_PARAM_DOUBLE;
        info->explanation = "alpha to use if reflection lies outside of source image. default 1";
        break;
    }
    }
}

f0r_instance_t f0r_construct(unsigned int width, unsigned int height)
{
    return reinterpret_cast<f0r_instance_t>(new Instance(width, height));
}

void f0r_destruct(f0r_instance_t instance)
{
    delete reinterpret_cast<Instance*>(instance);
}

void f0r_set_param_value(f0r_instance_t instance,
    f0r_param_t param, int param_index)
{
    Instance *inst = reinterpret_cast<Instance*>(instance);
    switch (param_index) {
    case(0): {
        // origin
        f0r_param_position *pos = reinterpret_cast<f0r_param_position*>(param);
        inst->kaleidoscope.set_origin(static_cast<float>(pos->x), static_cast<float>(pos->y));
        break;
    }
    case(1): {
        // segmentation
        inst->set_segmentation(*reinterpret_cast<f0r_param_double*>(param));
        break;
    }
    case(2): {
        // direction
        std::string str(*reinterpret_cast<f0r_param_string*>(param));
        if (str == "cw") {
            inst->kaleidoscope.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        } else if (str == "ccw") {
            inst->kaleidoscope.set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
        }
        break;
    }
    case(3): {
        // edge_threshold
        inst->set_edge_threshold(*reinterpret_cast<f0r_param_double*>(param));
        break;
    }
    case(4): {
        // preferred_corner
        std::string str(*reinterpret_cast<f0r_param_string*>(param));
        if (str == "tl") {
            inst->kaleidoscope.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
        } else if (str == "tr") {
            inst->kaleidoscope.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
        } else if (str == "bl") {
            inst->kaleidoscope.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
        } else if (str == "br") {
            inst->kaleidoscope.set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
        }
        break;
    }
    case(5): {
        // corner_search
        std::string dir(*reinterpret_cast<f0r_param_string*>(param));
        if (dir == "cw") {
            inst->kaleidoscope.set_preferred_corner_search_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        } else if (dir == "ccw") {
            inst->kaleidoscope.set_preferred_corner_search_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
        }
        break;
    }
    case(6): {
        // bg_color
        f0r_param_color * col = reinterpret_cast<f0r_param_color*>(param);
        inst->bg_colour[0] = static_cast<std::uint8_t>(col->r * 255);
        inst->bg_colour[1] = static_cast<std::uint8_t>(col->g * 255);
        inst->bg_colour[2] = static_cast<std::uint8_t>(col->b * 255);
        break;
    }
    case(7): {
        // bg_alpha
        inst->bg_colour[3] = static_cast<std::uint8_t>(*reinterpret_cast<f0r_param_double*>(param));
        break;
    }
    }
}

char dir_strings[2][4] = { "cw", "ccw" };
char corner_strings[4][3] = { "tl", "tr", "bl", "br" };

void f0r_get_param_value(f0r_instance_t instance,
    f0r_param_t param, int param_index)
{
    Instance* inst = reinterpret_cast<Instance*>(instance);
    switch (param_index) {
    case(0): {
        // origin
        f0r_param_position* store = reinterpret_cast<f0r_param_position*>(param);
        store->x = static_cast<double>(inst->kaleidoscope.get_origin_x());
        store->y = static_cast<double>(inst->kaleidoscope.get_origin_y());
        break;
    }
    case(1): {
        // segmentation
        f0r_param_double* store = reinterpret_cast<f0r_param_double*>(param);
        *store = inst->get_segmentation();
        break;
    }
    case(2): {
        // direction
        f0r_param_string* store = reinterpret_cast<f0r_param_string*>(param);
        *store = (inst->kaleidoscope.get_segment_direction() == libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE) ? dir_strings[0] : dir_strings[1];
        break;
    }
    case(3): {
        // edge_threshold
        f0r_param_double* store = reinterpret_cast<f0r_param_double*>(param);
        *store = inst->get_edge_threshold();
        break;
    }
    case(4): {
        // preferred_corner
        f0r_param_string* store = reinterpret_cast<f0r_param_string*>(param);

        libkaleidoscope::Kaleidoscope::Corner corner = inst->kaleidoscope.get_preferred_corner();
        switch (corner) {
            case(libkaleidoscope::Kaleidoscope::Corner::TL):
                *store = corner_strings[0];
                break;
            case(libkaleidoscope::Kaleidoscope::Corner::TR):
                *store = corner_strings[1];
                break;
            case(libkaleidoscope::Kaleidoscope::Corner::BL):
                *store = corner_strings[2];
                break;
            case(libkaleidoscope::Kaleidoscope::Corner::BR):
                *store = corner_strings[3];
                break;
        }
        break;
    }
    case(5): {
        // corner_search
        f0r_param_string* store = reinterpret_cast<f0r_param_string*>(param);
        *store = (inst->kaleidoscope.get_preferred_corner_search_direction() == libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE) ? dir_strings[0] : dir_strings[1];
        break;
    }
    case(6): {
        // bg_color
        f0r_param_color* store = reinterpret_cast<f0r_param_color*>(param);
        store->r = inst->bg_colour[0] / 255.0f;
        store->g = inst->bg_colour[1] / 255.0f;
        store->b = inst->bg_colour[2] / 255.0f;
        break;
    }
    case(7): {
        // bg_alpha
        f0r_param_double* store = reinterpret_cast<f0r_param_double*>(param);
        *store = inst->bg_colour[3] / 255.0;
        break;
    }
    }
}

void f0r_update(f0r_instance_t instance, double time,
    const uint32_t* inframe, uint32_t* outframe)
{
    Instance* inst = reinterpret_cast<Instance*>(instance);
    inst->kaleidoscope.process(inframe, outframe);
}

void f0r_update2(f0r_instance_t instance,
    double time,
    const uint32_t* inframe1,
    const uint32_t* inframe2,
    const uint32_t* inframe3,
    uint32_t* outframe)
{

}

}

