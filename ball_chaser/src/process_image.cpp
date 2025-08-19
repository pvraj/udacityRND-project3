#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>
#include <iostream>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // based on look_away.cpp
    ROS_INFO_STREAM("Driving Robot to lin_x " << lin_x << "; ang_z: " << ang_z);
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv;
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    // call the  service
    if (!client.call(srv)) {
        ROS_ERROR("Failed to call service ball_chaser/command_robot");
    }
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    size_t num_elements = img.data.size();
    // std::cout << "the number of elements: " << num_elements << std::endl;
    // std::cout << "the endcoding: " << img.encoding << std::endl;
    // std::cout << "the step size: " << img.step << std::endl;
    int white_byte_start_index = -1;
    int pane = -1;


    for (size_t i = 2; i < num_elements; i += 3) {
        // if the last 3 bytes 
        if ((img.data[i] == white_pixel) && (img.data[i-1] == white_pixel) && (img.data[i-2] == white_pixel)) {
            std::cout << "found a white pixel! at byte: " << i << std::endl;
            white_byte_start_index = i;
            break;
        }
    }
    if (white_byte_start_index == -1) {
        // no ball found, don't move / stop motion
        drive_robot(0.0f, 0.0f);

        return;
    }

    // determine where hte white pixel falls:
    // we only need to know the column in order to know if its left, middle, or right
    int white_pixel_col_index = (white_byte_start_index % img.step) / 3;
    if (white_pixel_col_index < (img.width / 3)) {
        // camera is 1.3962634 radians field of view which is ~80 degrees
        // so ~0.465 radians or ~26.6 degrees for each of the 3 panes
        // and 800 pixels is width of view, so
        // 0.1 degrees per pixel or 10 pixels per degree or 0.00174533 radians per pixel
        // ~0.465/2 = 0.2325
        // to be simple, just pick the mid point of the pane?
        // drive left
        drive_robot(0.1f, 0.2325f);
    }
    else if (white_pixel_col_index < (2 * img.width / 3)) {
        // drive straight
        drive_robot(0.1f, 0.0f);
    }
    else {
        // drive right. clockwise is negative
        drive_robot(0.1f, -0.2325f);
    }    
    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}