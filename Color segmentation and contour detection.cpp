#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 初始化摄像头
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "无法打开摄像头" << endl;
        return -1;
    }

    Mat frame, hsv, mask;
    while (true) {
        // 读取一帧图像
        cap >> frame;
        if (frame.empty()) {
            cerr << "无法读取图像" << endl;
            break;
        }

        // 转换颜色空间到 HSV
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 定义银色易拉罐的颜色范围（HSV）
        Scalar lower_silver = Scalar(0, 0, 150);
        Scalar upper_silver = Scalar(180, 30, 255);

        // 创建掩膜
        inRange(hsv, lower_silver, upper_silver, mask);

        // 进行形态学操作，去除噪声
        Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
        morphologyEx(mask, mask, MORPH_OPEN, kernel);
        morphologyEx(mask, mask, MORPH_CLOSE, kernel);

        // 查找轮廓
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(mask, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // 遍历轮廓
        for (size_t i = 0; i < contours.size(); i++) {
            // 计算轮廓的面积
            double area = contourArea(contours[i]);
            if (area > 500) {  // 过滤掉小的轮廓
                // 绘制轮廓
                drawContours(frame, contours, static_cast<int>(i), Scalar(0, 255, 0), 2);
                // 获取轮廓的边界矩形
                Rect rect = boundingRect(contours[i]);
                // 绘制边界矩形
                rectangle(frame, rect, Scalar(0, 0, 255), 2);
                // 在矩形上方添加文本
                putText(frame, "Can", Point(rect.x, rect.y - 10), FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 0), 2);
            }
        }

        // 显示处理后的图像
        imshow("Can Detection", frame);

        // 按 'q' 键退出循环
        if (waitKey(1) == 'q') {
            break;
        }
    }

    // 释放摄像头并关闭所有窗口
    cap.release();
    destroyAllWindows();

    return 0;
}
//test