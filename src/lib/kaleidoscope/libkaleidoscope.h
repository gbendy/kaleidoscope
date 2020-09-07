#ifndef LIBKALEIDOSCOPE_LIBKALEIDOSCOPE_H
#define LIBKALEIDOSCOPE_LIBKALEIDOSCOPE_H 1

#include <cstdint>

namespace libkaleidoscope {

/**
 * Class which implements the kaleidoscope effect
 */
class Kaleidoscope {
public:
    /***
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
     * Defaults to 0.5, 0.5.
     * @param x y coordinate of the origin
     * @param y y coordinate of the origin
     * @return 
     *          -  0:  Success
     *          - -1: Error
     *          - -2: Parameter out of range
     */
    std::int32_t set_origin(float x, float y);

    /**
     * Returns the origin x coordinate.
     */
    float get_origin_x() const;

    /**
     * Returns the origin y coordinate.
     */
    float get_origin_y() const;
    
    /**
     * Sets the segmentation.
     * Defaults to 16.
     * @param segmentation the segmentation value
     * @return
     *          -  0:  Success
     *          - -1: Error
     *          - -2: Parameter out of range
     */
    std::int32_t set_segmentation(std::uint32_t segmentation);

    /**
     * Returns the segmentation value
     */
    std::uint32_t get_segmentation() const;
    
    ///  Defines a corner
    enum class Corner {
        TL = 0,
        TR,
        BR,
        BL
    };

    ///  Defines an angular direction
    enum class Direction {
        CLOCKWISE = 0,
        ANTICLOCKWISE
    };

    /**
     * Sets the direction that segments rotate in.
     * Defaults to Direction::CLOCKWISE
     * @param direction the direction
     * @return
     *          -  0:  Success
     *          - -1: Error
     */
    std::int32_t set_segment_direction(Direction direction);

    /**
     * Returns the segment direction
     */
    Direction get_segment_direction() const;

    /**
     * Segments are always aligned to the furthest corner of the image from the origin. 
     * If multiple corners are equidistant from the origin then this indicates which
     * corner is preferred. The algorithm searches from this corner, in the direction
     * specified in #set_preferred_corner_search_direction to find the furthest corner.
     * Defaults to Corner::BR
     * @param corner the preferred corner
     * @return
     *          -  0:  Success
     *          - -1: Error
     */
    std::int32_t set_preferred_corner(Corner corner);

    /**
     * Returns the preferred corner
     */
    Corner get_preferred_corner() const;

    /**
     * The direction to search for the furthest corner in.
     * Defaults to Direction::CLOCKWISE
     * @param direction the search direction
     * @return
     *          -  0:  Success
     *          - -1: Error
     */
    std::int32_t set_preferred_corner_search_direction(Direction direction);

    /**
     * Returns the corner search direction
     */
    Direction get_preferred_corner_search_direction() const;

    /**
     * Applies the kaleidoscope effect to \p in_frame and returns it in \p out_frame.
     * Each parameter must point to enough memory to contain the image specified in the 
     * constructor
     * @param in_frame the input frame to process
     * @param out_frame receives the output image
     * @return
     *          -  0:  Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    std::int32_t process(const void* in_frame, void* out_frame);

    /**
     * Visualises the currently configured segmentation. The pure green segment is the 
     * source segment and it reflects in the direction of the adjacent pure blue segment.
     * @param out_frame receives the output image
     * @return
     *          -  0:  Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    std::int32_t visualise(void* out_frame);

private:
    void init();


    /// Defines reflection information for a given point in the frame
    struct Reflect_info {
        std::uint32_t x;                ///< x coordinate
        std::uint32_t y;                ///< y coordinate
        float screen_x;                 ///< x coordinate in screen space (range -0.5->0.5, left negative)
        float screen_y;                 ///< y coordinate in screen space (range -0.5->0.5, bottom negative)
        float angle;                    ///< angle from this point to the start of the source segment
        std::uint32_t segment_number;   ///< segment number the point resides in
        float reflection_angle;         ///< angle from this point to the point it reflects
    };

    /// Calculates the reflection information for a given point.
    /// NB: init() must have already been called.
    /// @param x the x coordinate
    /// @param x the y coordinate
    /// @return the reflection information for the point
    Reflect_info calculate_reflect_info(std::uint32_t x, std::uint32_t y);

    std::uint32_t m_width;
    std::uint32_t m_height;
    std::uint32_t m_component_size;
    std::uint32_t m_num_components;
    std::uint32_t m_stride;

    float m_origin_x;
    float m_origin_y;

    std::uint32_t m_segmentation;
    Direction m_segment_direction;

    Corner m_preferred_corner;
    Direction m_preferred_search_dir;

    std::uint32_t m_n_segments;
    float m_start_angle;
    float m_segment_width;
};

}
#endif 

