# Project 3 AMCL
This uses all the old code from the prior 2 projects (existing gazebo world and robot).
You can ignore the `ball_chaser` and `my_robot` (from the old projects); these are needed to reference the old gazebo world and old robot and reuse their code. The `gazebo_ros_2Dmap_plugin` was used to generate the map, as suggested by a mentor (link below). The `teleop_twist_keyboard` is for manual control of the robot

## Misc

### frame
Learned a little about coordinate frames in Gazebo. Used `rosrun tf tf_echo map base_link
` then `dot -Tpdf ~/catkin_ws/src/frames.gv -o frames.pdf` to generate the PDF to show the tree of frames. The TF tree shows the relationship between the map and base_link frames, where map is the root frame and base_link is a descendent child frame.

![Directed graph visualization showing ROS transform tree hierarchy with map and base_link coordinate frames and their parent-child relationships](./tf_frames.png)

### map
Lesson uses a 2D map of the existing gazebo world from past projects. Instructed to use this [repo](https://github.com/marinaKollmitz/gazebo_ros_2Dmap_plugin/tree/master) by the [mentor](https://knowledge.udacity.com/questions/1072496), since the one in the lesson is broken/out of date. 

![Gazebo simulation environment displaying a brick-textured world with a mobile robot positioned in the simulated space](./brick_world_map.png)
