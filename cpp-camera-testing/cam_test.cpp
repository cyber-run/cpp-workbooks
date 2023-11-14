#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

class CameraApp {
public:
    CameraApp() : is_live(false) {
        // Initialize camera
        if (!cap.open(0)) {
            std::cerr << "Error: Could not open camera" << std::endl;
            exit(1);
        }
        // Set a lower resolution for the camera
        cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    }

    ~CameraApp() {
        stop_video_feed();
    }

    void adjust_exposure(double exposure_value) {
        // Set the camera's exposure
        cap.set(cv::CAP_PROP_EXPOSURE, exposure_value);
    }

    void toggle_video_feed() {
        if (is_live) {
            stop_video_feed();
        } else {
            start_video_feed();
        }
    }

    void start_video_feed() {
        is_live = true;
        video_thread = std::thread(&CameraApp::video_loop, this);
    }

    void stop_video_feed() {
        is_live = false;
        if (video_thread.joinable()) {
            video_thread.join();
        }
    }

private:
    cv::VideoCapture cap;
    std::atomic<bool> is_live;
    std::thread video_thread;

    void video_loop() {
        while (is_live) {
            cv::Mat frame;
            if (cap.read(frame)) {
                // Process frame (in this example, we'll just display it)
                cv::imshow("Camera", frame);
            }
            if (cv::waitKey(30) >= 0) break;
        }
        cv::destroyAllWindows();
    }
};

int main() {
    CameraApp app;

    // Toggle video feed on and off
    app.start_video_feed();
    std::this_thread::sleep_for(std::chrono::seconds(10)); // Simulate doing something for 10 seconds
    app.stop_video_feed();

    // Adjust exposure, assuming camera uses exposure values in a known range or units.
    app.adjust_exposure(50);

    return 0;
}
