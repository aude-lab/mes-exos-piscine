#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>

inline char pixelToAscii(int pixelValue)
{
    const std::string ascii_chars = ".:-=+*#%@8WM";
    int index = pixelValue * ascii_chars.length() / 256;
    return ascii_chars[index];
}

inline std::string rgbToAnsi(int r, int g, int b)
{
    return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";"
        + std::to_string(b) + "m";
}

/**
 * @brief Plays a given video file.
 *
 * @param video_path The path to the video file to play.
 */
inline void play_video(std::string& video_path)
{
    std::cout << std::endl;
    cv::VideoCapture video(video_path);

    if (!video.isOpened())
    {
        std::cerr << "Error: Impossible to open video." << std::endl;
        return;
    }

    cv::Mat frame, resizedFrame;
    while (true)
    {
        video >> frame;
        if (frame.empty())
            break;

        cv::resize(frame, resizedFrame, cv::Size(105, 56));
        for (int i = 0; i < resizedFrame.rows; ++i)
        {
            for (int j = 0; j < resizedFrame.cols; ++j)
            {
                cv::Vec3b pixel = resizedFrame.at<cv::Vec3b>(i, j);
                int blue = pixel[0];
                int green = pixel[1];
                int red = pixel[2];

                char asciiChar = pixelToAscii((red + green + blue) / 3);
                std::string colorCode = rgbToAnsi(red, green, blue);

                std::cout << "\033[" << i + 1 << ";" << j + 1 << "H";
                std::cout << colorCode << asciiChar;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        std::cout << "\033[0m";
    }
    video.release();
    std::cout << std::endl;
}
