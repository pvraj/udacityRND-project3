# Project 3 AMCL
This uses all the old code from the prior 2 projects (existing gazebo world and robot).

## Misc
- Learned a little about coordinate frames in Gazebo
- Used `rosrun tf tf_echo map base_link
` then `dot -Tpdf ~/catkin_ws/src/frames.gv -o frames.pdf` to generate the PDF to show the tree of frames