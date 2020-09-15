#include "frei0r.hpp"

#include "libkaleidoscope.h"

class kaleid0sc0pe : public frei0r::filter, public libkaleidoscope::Kaleidoscope
{
public:
    kaleid0sc0pe(unsigned int width, unsigned int height):
        libkaleidoscope::Kaleidoscope(width,height,1,4),
        m_origin_x(0.5),
        m_origin_y(0.5),
        m_segmentation(16/128.0),
        m_direction(1.0),
        m_corner(0),
        m_corner_direction(true),
        m_reflect_edges(true),
        m_edge_threshold(0),
        m_bg_alpha(1),
        m_specify_source(false),
        m_source_segment(0),
        m_dirty(true)
    {
        m_bg_colour.r = 1.0;
        m_bg_colour.g = 0.0;
        m_bg_colour.b = 1.0;

        register_param(m_origin_x,
                                "origin_x",
                                "origin of the kaleidoscope in x. default 0.5");
        register_param(m_origin_y,
                                "origin_y",
                                "origin of the kaleidoscope in y. default 0.5");
        register_param(m_segmentation,
                                "segmentation",
                                "kaleidoscope segmentation / 128, segmentations of 1, 2 or multiples of 4 work best. default 16/128");
        register_param(m_specify_source,
                                "specify_source",
                                "if true then source angle is read from source_segment, otherwise auto-calculated");
        register_param(m_source_segment,
                                "source_segment",
                                "centre of source segment if specify_source is true. 0 is in +x and rotates counter clockwise"); 
        register_param(m_direction,
                                "segmentation_direction",
                                "segmentation direction, < 1/3 is none, < 2/3 is counter clockwise, otherwise clockwise");
        register_param(m_reflect_edges,
                                "reflect_edges",
                                "if true then reflections that end up outside the source reflect back into it, otherwise the specified background colour is used.");
        register_param(m_edge_threshold,
                                "edge_threshold",
                                "edge threshold / 4, reflections outside the image but within this distance clamp to the edge. default 0");
        register_param(m_corner,
                                "preferred_corner",
                                "preferred corner, 0 is top right, 0.25 top left, 0.5 bottom left, 0.75 bottom right");
        register_param(m_corner_direction,
                                "corner_search",
                                "if true search clockwise for furthest corner, otherwise counter clockwise");
        register_param(m_bg_colour,
                                "bg_color",
                                "colour to use if reflection lies outside of source image and not reflecting back in. default 1,0,1");
        register_param(m_bg_alpha,
                                "bg_alpha",
                                "alpha to use if reflection lies outside of source image and not reflecting back in. default 1");
        set_background_colour(m_background);
    }

    virtual void update(double time,
        uint32_t* out,
        const uint32_t* in) {

        if (m_dirty) {
            update_params();
        }
        process(in, out);
    }

    virtual void set_param_value(f0r_param_t param, int param_index)
    {
        fx::set_param_value(param, param_index);
        m_dirty = true;
    }

private:
    void update_params()
    {
        set_origin(static_cast<float>(m_origin_x), static_cast<float>(m_origin_y));
        set_segmentation(static_cast<std::uint32_t>(m_segmentation * 128));
        if (m_direction < 1/3.0) {
            set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::NONE);
        } else if (m_direction < 2/3.0) {
            set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
        } else {
            set_segment_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        }
        if (m_corner < 0.25) {
            set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TR);
        } else if (m_corner < 0.5) {
            set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::TL);
        } else if (m_corner < 0.75) {
            set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BL);
        } else {
            set_preferred_corner(libkaleidoscope::Kaleidoscope::Corner::BR);
        }
        if (m_corner_direction) {
            set_preferred_corner_search_direction(libkaleidoscope::Kaleidoscope::Direction::CLOCKWISE);
        } else {
            set_preferred_corner_search_direction(libkaleidoscope::Kaleidoscope::Direction::ANTICLOCKWISE);
        }
        set_reflect_edges(m_reflect_edges);
        set_edge_threshold(static_cast<std::uint32_t>(m_edge_threshold * 4));

        if (m_specify_source) {
            set_source_segment(static_cast<float>(m_source_segment) * 3.141592654f * 2);
        } else {
            set_source_segment(-1);
        }
        m_background[0] = static_cast<std::uint8_t>(m_bg_colour.r * 255);
        m_background[1] = static_cast<std::uint8_t>(m_bg_colour.g * 255);
        m_background[2] = static_cast<std::uint8_t>(m_bg_colour.b * 255);
        m_background[3] = static_cast<std::uint8_t>(m_bg_alpha * 255);

        m_dirty = false;
    }

    double m_origin_x;
    double m_origin_y;

    double m_segmentation;
    double m_direction;

    double m_corner;
    bool m_corner_direction;

    bool m_reflect_edges;

    double m_edge_threshold;

    f0r_param_color m_bg_colour;
    double m_bg_alpha;

    bool m_specify_source;
    double m_source_segment;

    std::uint8_t m_background[4];

    bool m_dirty;
};

frei0r::construct<kaleid0sc0pe> plugin("Kaleid0sc0pe", "Applies a kaleidoscope effect to a source image", "Brendan Hack", 1, 0, F0R_COLOR_MODEL_RGBA8888);