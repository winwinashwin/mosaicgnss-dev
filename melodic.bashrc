#!/bin/bash

source /opt/ros/$ROS_DISTRO/setup.bash

alias ..="cd .."
alias ...="cd ..."

alias cw="cd $ROS_WS && if [[ -f devel/setup.bash ]]; then source devel/setup.bash; fi"
alias cs="if [[ -d $ROS_WS/src ]]; then cd $ROS_WS/src; fi"
alias cm="cd $ROS_WS && catkin_make"
