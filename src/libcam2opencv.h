// libcam2opencv.h
#ifndef __LIBCAM2OPENCV
#define __LIBCAM2OPENCV

// 
#include <sys/mman.h>  // 
#include <opencv2/opencv.hpp>

// 
#pragma push_macro("signals")
#pragma push_macro("slots")
#pragma push_macro("emit")
#undef signals
#undef slots
#undef emit


#include <libcamera/libcamera.h>


#pragma pop_macro("emit")
#pragma pop_macro("slots")
#pragma pop_macro("signals")



/**
 * Settings
 **/
struct Libcam2OpenCVSettings {
    /**
     * Width of the video capture. A zero lets libcamera decide the width.
     **/
    unsigned int width = 0;
    
    /**
     * Height of the video capture. A zero lets libcamera decide the height.
     **/
    unsigned int height = 0;

    /**
     * Framerate. A zero lets libcamera decide the framerate.
     **/
    unsigned int framerate = 0;

    /**
     * Brightness
     **/
    float brightness = 0.0;

    /**
     * Contrast
     **/
    float contrast = 1.0;
};

class Libcam2OpenCV {
public:
    struct Callback {
	virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &metadata) = 0;
	virtual ~Callback() {}
    };

    /**
     * Register the callback for the frame data
     **/
    void registerCallback(Callback* cb) {
	callback = cb;
    }

    /**
     * Starts the camera and the callback at default resolution and framerate
     **/
    void start(Libcam2OpenCVSettings settings = Libcam2OpenCVSettings() );

    /**
     * Stops the camera and the callback
     **/
    void stop();
    
private:
    std::shared_ptr<libcamera::Camera> camera;
    std::map<libcamera::FrameBuffer *, std::vector<libcamera::Span<uint8_t>>> mapped_buffers;
    std::unique_ptr<libcamera::CameraConfiguration> config;
    cv::Mat frame;
    Callback* callback = nullptr;
    libcamera::FrameBufferAllocator* allocator = nullptr;
    libcamera::Stream *stream = nullptr;
    std::unique_ptr<libcamera::CameraManager> cm;
    std::vector<std::unique_ptr<libcamera::Request>> requests;
    libcamera::ControlList controls;

    std::vector<libcamera::Span<uint8_t>> Mmap(libcamera::FrameBuffer *buffer) const
    {
	auto item = mapped_buffers.find(buffer);
	if (item == mapped_buffers.end())
	    return {};
	return item->second;
    }

    /*
     * --------------------------------------------------------------------
     * Handle RequestComplete
     *
     * For each Camera::requestCompleted Signal emitted from the Camera the
     * connected Slot is invoked.
     */
    void requestComplete(libcamera::Request *request);
};

#endif
