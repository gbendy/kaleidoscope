#ifndef LIBKALEIDOSCOPE_IKALEIDOSCOPE_H
#define LIBKALEIDOSCOPE_IKALEIDOSCOPE_H 1

#include <cstdint>
#include <memory>
namespace libkaleidoscope {
class IKaleidoscope;
}
namespace std
{

// default delete for IKaleidoscope
// this isn't going to work across an actaul shared object boundary but
// not particular worried about that at the moment
template<>
class default_delete<libkaleidoscope::IKaleidoscope>
{
public:
    void operator()(libkaleidoscope::IKaleidoscope* p);
};
}

namespace libkaleidoscope {

/**
 * Class which implements the kaleidoscope effect
 */
class IKaleidoscope {
public:
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
    virtual std::int32_t set_origin(float x, float y) = 0;

    /**
     * Returns the origin x coordinate.
     */
    virtual float get_origin_x() const = 0;

    /**
     * Returns the origin y coordinate.
     */
    virtual float get_origin_y() const = 0;
    
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
    virtual std::int32_t set_segmentation(std::uint32_t segmentation) = 0;

    /**
        * Returns the segmentation value
        */
    virtual std::uint32_t get_segmentation() const = 0;
    
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
    virtual std::int32_t set_edge_threshold(std::uint32_t threshold) = 0;

    /**
     * Returns the edge threshold
     */
    virtual std::uint32_t get_edge_threshold() const = 0;

    ///  Defines a corner
    enum class Corner {
        TL = 0,     //< Top Left
        TR,         //< Top Right
        BR,         //< Bottom Right
        BL          //< Bottom Left
    };

    ///  Defines an angular direction
    enum class Direction {
        CLOCKWISE = 0,  //< Clockwise
        ANTICLOCKWISE,  //< Anti Clockwise
        NONE            //< No direction
    };

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
    virtual std::int32_t set_segment_direction(Direction direction) = 0;

    /**
        * Returns the segment direction
        */
    virtual Direction get_segment_direction() const = 0;

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
    virtual std::int32_t set_preferred_corner(Corner corner) = 0;

    /**
     * Returns the preferred corner
     */
    virtual Corner get_preferred_corner() const = 0;

    /**
     * The direction to search for the furthest corner in.
     * Defaults to Direction::CLOCKWISE
     * @param direction the search direction
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (\p direction cannot by Direction::NONE)
     */
    virtual std::int32_t set_preferred_corner_search_direction(Direction direction) = 0;

    /**
     * Returns the corner search direction
     */
    virtual Direction get_preferred_corner_search_direction() const = 0;

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
    virtual std::int32_t set_reflect_edges(bool reflect) = 0;

    /**
     * Returns the reflect edges setting
     */
    virtual bool get_reflect_edges() const = 0;

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
    virtual std::int32_t set_background_colour(void* colour) = 0;

    /**
     * Returns the background colour
     */
    virtual void* get_background_colour() const = 0;

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
    virtual std::int32_t set_source_segment(float angle) = 0;

    /**
     * Returns the source segment
     */
    virtual float get_source_segment() const = 0;

    /**
     * Applies the kaleidoscope effect to \p in_frame and returns it in \p out_frame.
     * Each parameter must point to enough memory to contain the image specified in the 
     * constructor and must be aligned to an integer multiple of 16 bytes in memory.
     * @param in_frame the input frame to process
     * @param out_frame receives the output image
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    virtual std::int32_t process(const void* in_frame, void* out_frame) = 0;

    /**
     * Sets the number of threads to use when processing.
     * Default to 0.
     * @param threading the nubmer of threads to use. \c 0, calculate automatically,
     * otherwise the explicit thread count.
     * @return
     *          -  0: Success
     *          - -1: Error
     */
    virtual std::int32_t set_threading(std::uint32_t threading) = 0;

    /**
     * Returns the number of threads to use.
     */
    virtual std::uint32_t get_threading() const = 0;

    /**
     * Visualises the currently configured segmentation. The pure green segment is the 
     * source segment.
     * @param out_frame receives the output image
     * @return
     *          -  0: Success
     *          - -1: Error
     *          - -2: Invalid parameter (nullptr)
     */
    virtual std::int32_t visualise(void* out_frame) = 0;

    /**
     * Virtual destructor
     */
    virtual ~IKaleidoscope() {};

    /**
     * Static factory function.
     * @param width the frame width
     * @param height the frame height
     * @param component_size the byte size of each frame pixel component
     * @param num_components the number of components per pixel
     * @param stride the image stride, if \c 0 then calculated as \p width * \p component_size * \p num_components
     */
    static std::unique_ptr<IKaleidoscope> factory(std::uint32_t width, std::uint32_t height, std::uint32_t component_size, std::uint32_t num_components, std::uint32_t stride = 0) {
        return std::unique_ptr<IKaleidoscope>(create(width, height, component_size, num_components, stride));
    }

private:
    static IKaleidoscope* create(std::uint32_t width, std::uint32_t height, std::uint32_t component_size, std::uint32_t num_components, std::uint32_t stride = 0);

};

}

#endif 

