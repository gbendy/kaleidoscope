#ifndef LIBKALEIDOSCOPE_LIBKALEIDOSCOPE_H
#define LIBKALEIDOSCOPE_LIBKALEIDOSCOPE_H 1

#include "ikaleidoscope.h"

#include <vector>
#include <cmath>
#include <functional>

#if _M_IX86_FP == 2 || _M_X64 == 100
#ifndef _M_ARM 
#define __SSE2__
#endif
#endif

#ifdef __SSE2__
#define USE_SSE2
#include <emmintrin.h>
#endif

namespace libkaleidoscope {

/**
 * Class which implements the kaleidoscope effect
 */
class Kaleidoscope: public IKaleidoscope {
public:
    /**
     * Constructor
     * @param width the frame width
     * @param height the frame height
     * @param component_size the byte size of each frame pixel component
     * @param num_components the number of components per pixel
     * @param stride the image stride, if \c 0 then calculated as \p width * \p component_size * \p num_components
     */
    Kaleidoscope(std::uint32_t width, std::uint32_t height, std::uint32_t component_size, std::uint32_t num_components, std::uint32_t stride = 0);

    /**
     * Sets the origin of the kaleidoscope effect. These are given in the range 0 -> 1.
     * Values other than 0.5,0.5 may end up reflecting outside the source image.
     * These areas will be filled depending on the settings in #set_reflect_edges and
     * #set_background_colour.
     * Defaults to 0.5, 0.5.
     * @param x x coordinate of the origin
     * @param y y coordinate of the origin
     * @return 
     *          -  0: Success
     *          - -1: Error
     *          - -2: Parameter out of range
     */
    virtual std::int32_t set_origin(float x, float y);

    /**
     * Returns the origin x coordinate.
     */
    virtual float get_origin_x() const;

    /**
     * Returns the origin y coordinate.
     */
    virtual float get_origin_y() const;
    
    /**
     * Sets the segmentation resulting in \p segmentation * 2 segments in the output frame.
     * Segmentation values that are 1, 2 or a multiple of 4, are oriented to an image corner
     * and centred will always reflect back to the source segment. 
     * Other settings may end up reflecting outside the source image. These areas will be filled
     * depending on the settings in #set_reflect_edges and #set_background_colour.
     * Defaults to 16.
     * @param segmentation the segmentation value
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Parameter out of range
     */
    virtual std::int32_t set_segmentation(std::uint32_t segmentation);

    /**
     * Returns the segmentation value
     */
    virtual std::uint32_t get_segmentation() const;
    
    /**
     * When #set_reflect_edges is not true and a reflected pixel ends up outside the image
     * we can clamp the pixels that fall outside the image but within \p threshold pixels
     * of the edge to the edge.
     * Defaults to 0
     * @param threshold the threshold in pixels.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_edge_threshold(std::uint32_t threshold);

    /**
     * Returns the edge threshold
     */
    virtual std::uint32_t get_edge_threshold() const;

    /**
     * Sets the direction that the source segment rotates in. If
     * Direction::NONE then the source segment is centred on the corner.
     * Otherwise it extends in the given direction.
     * Defaults to Direction::NONE
     * @param direction the direction
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_segment_direction(Direction direction);

    /**
     * Returns the segment direction
     */
    virtual Direction get_segment_direction() const;

    /**
     * Unless directly specified with #set_source_segment the source segment is always aligned to
     * the furthest corner of the image from the origin. 
     * The source segment has it's edge (or centre) on a line from the origin to the furthest corner,
     * and extends in the direction given by #set_segment_direction.
     * If multiple corners are equidistant from the origin then this indicates which
     * corner is preferred. The algorithm searches from this corner, in the direction
     * specified in #set_preferred_corner_search_direction to find the furthest corner.
     * Defaults to Corner::BR
     * @param corner the preferred corner
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_preferred_corner(Corner corner);

    /**
     * Returns the preferred corner
     */
    virtual Corner get_preferred_corner() const;

    /**
     * The direction to search for the furthest corner in.
     * Defaults to Direction::CLOCKWISE
     * @param direction the search direction
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (\p direction cannot by Direction::NONE)
     */
    virtual std::int32_t set_preferred_corner_search_direction(Direction direction);

    /**
     * Returns the corner search direction
     */
    virtual Direction get_preferred_corner_search_direction() const;

    /**
     * Reflected points can end up outside the source image depending on segmentation,
     * source segment and origin settings. When this occurs three options are provided,
     * lookup the pixel in a reflected tessellation of the original image, set the pixel
     * to the background colour provided in #set_background_colour or write nothing to the
     * output frame for that pixel.
     * Defaults to \c true which is to lookup in the reflected tessellation.
     * @param reflect if \c true then lookup in a reflection, if \c false use background color
     * or do nothing if no background color was set.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_reflect_edges(bool reflect);

    /**
     * Returns the reflect edges setting
     */
    virtual bool get_reflect_edges() const;

    /**
     * If not reflecting edges then this sets the colour to use when the kaleidoscope effect 
     * for a point ends up outside the source image. The data pointed to should be at least as
     * wide as a pixel and must be valid for  the lifetime of the class instance.
     * The caller retains ownership of the passed memory.
     * Defaults to \c nullptr
     * @param colour the background colour, if \c nullptr then the output buffer is not modified
     * if reflection does not land in the source segment.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_background_colour(void* colour);

    /**
     * Returns the background colour
     */
    virtual void *get_background_colour() const;

    /**
     * Allows to explicitly specify the location of the source segment. 0 radians is in the positive
     * horizontal direction and +ve rotates anti-clockwise.
     * @param angle If positive or 0 then the angle of the centre of the source segment in radians. If negative
     * (the default) then the source segment is auto calculated based on origin, preferred corner, 
     * direction and corner search direction.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_source_segment(float angle);

    /**
     * Returns the source segment
     */
    virtual float get_source_segment() const;

    /**
     * Applies the kaleidoscope effect to \p in_frame and returns it in \p out_frame.
     * Each parameter must point to enough memory to contain the image specified in the 
     * constructor.
     * @param in_frame the input frame to process
     * @param out_frame receives the output image
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    virtual std::int32_t process(const void* in_frame, void* out_frame);

    /**
     * Sets the number of threads to use when processing.
     * Default to 0.
     * @p threading the nubmer of threads to use. \c 0, calculate automatically,
     * otherwise the explicit thread count.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_threading(std::uint32_t threading);

    /**
     * Returns the number of threads to use.
     */
    virtual std::uint32_t get_threading() const;

    /**
     * Visualises the currently configured segmentation. The pure green segment is the 
     * source segment.
     * @param out_frame receives the output image
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    virtual std::int32_t visualise(void* out_frame);

private:
    void init();

#ifdef USE_SSE2
    /// Defines reflection information for a given point in the frame
    struct Reflect_info {
        __m128 screen_x;                 ///< x coordinate in screen space (range -0.5->0.5, left negative)
        __m128 screen_y;                 ///< y coordinate in screen space (range -0.5->0.5, top negative)
        __m128 angle;                    ///< angle from this point to the start of the source segment
        __m128 segment_number;           ///< segment number the point resides in
        __m128i segment_number_i;
        __m128 reference_angle;          ///< positive angle to start of source segment
    };

    Reflect_info calculate_reflect_info(__m128i *x, __m128i *y);

    /// Converts coordinates to screen space
    /// @param x x coordinate
    /// @param y y coordinate
    /// @param sx source x coordinate
    /// @param sy source y coordinate
    void to_screen(__m128 *x, __m128 *y, __m128i *sx, __m128i *sy);

    /// Converts coordinates from screen space in place
    /// @param x x coordinate
    /// @param y y coordinate
    void from_screen(__m128 *x, __m128 *y);

    void process_rotation(std::int32_t segment_number, float source_x, float source_y, const std::uint8_t* in, std::uint8_t* out);
#else
    /// Defines reflection information for a given point in the frame
    struct Reflect_info {
        float screen_x;                 ///< x coordinate in screen space (range -0.5->0.5, left negative)
        float screen_y;                 ///< y coordinate in screen space (range -0.5->0.5, top negative)
        float angle;                    ///< angle from this point to the start of the source segment
        std::uint32_t segment_number;   ///< segment number the point resides in
        float reference_angle;          ///< positive angle to start of source segment
    };

    /// Calculates the reflection information for a given point.
    /// NB: init() must have already been called.
    /// @param x the x coordinate
    /// @param x the y coordinate
    /// @return the reflection information for the point
    Reflect_info calculate_reflect_info(std::uint32_t x, std::uint32_t y);

    /// Converts coordinates to screen space
    /// @param x x coordinate
    /// @param y y coordinate
    /// @param sx source x coordinate
    /// @param sy source y coordinate
    void to_screen(float& x, float& y, std::uint32_t sx, std::uint32_t sy);

    /// Converts coordinates from screen space in place
    /// @param x x coordinate
    /// @param y y coordinate
    void from_screen(float& x, float& y);
#endif    
    /// A block of data to process
    struct Block {
        const std::uint8_t* in_frame;
        std::uint8_t* out_frame;
        std::uint32_t x_start;
        std::uint32_t y_start;
        std::uint32_t x_end;
        std::uint32_t y_end;

        /// \param in_frame the input frame
        /// \param out_frame the output frame
        /// \param x_start start x coordinate of block to process
        /// \param y_start start y coordinate of block to process
        /// \param x_end end x coordinate of block to process (inclusive)
        /// \param y_end end y coordinate of block to process (inclusive)
        Block(const std::uint8_t* _in_frame, std::uint8_t* _out_frame, std::uint32_t _x_start, std::uint32_t _y_start, std::uint32_t _x_end, std::uint32_t _y_end):
            in_frame(_in_frame),
            out_frame(_out_frame),
            x_start(_x_start),
            y_start(_y_start),
            x_end(_x_end),
            y_end(_y_end)
        {}
    };
    
    /// Process a block
    void process_block(Block *block);

    std::uint8_t *lookup(std::uint8_t *p, std::uint32_t x, std::uint32_t y);

    const std::uint8_t* lookup(const std::uint8_t* p, std::uint32_t x, std::uint32_t y);
        
    std::uint32_t m_width;
    std::uint32_t m_height;
    std::uint32_t m_component_size;
    std::uint32_t m_num_components;
    std::uint32_t m_stride;
    std::uint32_t m_pixel_size;

    float m_aspect;

    float m_origin_x;
    float m_origin_y;
    float m_origin_native_x;
    float m_origin_native_y;

    std::uint32_t m_segmentation;
    Direction m_segment_direction;

    Corner m_preferred_corner;
    Direction m_preferred_search_dir;

    bool m_edge_reflect;

    void* m_background_colour;
    std::uint32_t m_edge_threshold;

    float m_source_segment_angle;

    std::uint32_t m_n_segments;
    float m_start_angle;
    float m_segment_width;

    std::uint32_t m_n_threads;

#ifdef USE_SSE2
    __m128 m_sse_aspect;
    __m128 m_sse_origin_native_x;
    __m128 m_sse_origin_native_y;
    __m128 m_sse_start_angle;
    __m128 m_sse_segment_width;
    __m128 m_sse_half_segment_width;
    __m128 m_sse_ps_0;
    __m128 m_sse_ps_1;
    __m128 m_sse_ps_2;
    __m128i m_sse_epi32_1;
    __m128i m_sse_epi32_2;
    __m128i m_sse_shift_1;
#endif
};

}

#endif 

