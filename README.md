```markdown
# Vision Module - Real-Time Yellow Can Detection

This module provides real-time detection and tracking of yellow cans using OpenCV and QT. It integrates camera input processing, color-based object detection, and visual feedback through bounding boxes and center points.

## Prerequisites

```
apt install libopencv-dev libcamera-dev
sudo apt install qtbase5-dev libqwt-qt5-dev
```

## Compilation and Installation

```
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=/path/to/qt ..
make
sudo make install
```

## How to Use It

1. Include `window.h` and link dependencies in your `CMakeLists.txt`:
```cmake
target_link_libraries(yourproj 
    cam2opencv 
    Qt5::Widgets 
    qwt
)
```

2. Implement the detection pipeline:
```cpp
// In Window class constructor
myCallback.window = this;
camera.registerCallback(&myCallback);

// Detection logic in detectCans()
cv::Mat hsv;
cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
cv::inRange(hsv, Scalar(15, 120, 80), Scalar(30, 255, 255), yellow_mask);
// ... (full processing chain)
```

3. Create GUI layout:
```cpp
// Qt widgets setup
thermo = new QwtThermo;
image = new QLabel;
hLayout = new QHBoxLayout();
hLayout->addWidget(thermo);
hLayout->addWidget(image);
```

4. Start/Stop camera:
```cpp
camera.start();  // Begin frame processing
camera.stop();   // End processing
```

## Key Features
- **HSV Color Filtering**: Isolate yellow objects with calibrated thresholds
- **Morphological Optimization**: Noise reduction using opening/closing operations
- **Aspect Ratio Filtering**: (0.5-0.7) range for can-shaped objects
- **Visual Feedback**:
  - Yellow bounding boxes around detected cans
  - Blue center points for tracking
  - QwtThermo widget showing center pixel brightness

## Example Integration
```cpp
// In callback implementation
void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
    cv::Mat processed = frame.clone();
    detectCans(processed);  // Main detection logic
    updateImage(processed); // Update GUI and thermometers
    
    // Access detection results:
    if (best_rect.area() > 0) {
        int center_x = best_rect.x + best_rect.width/2;
        int center_y = best_rect.y + best_rect.height/2;
    }
}
```

## Performance Notes
- Tested at 30 FPS on Raspberry Pi 4 (800x600 resolution)
- Typical detection latency: <50ms per frame
- Adjust HSV thresholds for different lighting conditions

## Subdirectory: AdvancedDemo
Contains a complete QT application demonstrating:
- Real-time camera preview with overlay graphics
- Dynamic threshold adjustment controls
- Performance metrics display
- CSV logging of detection results