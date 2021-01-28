#!/bin/bash

source /opt/ros/$ROS_DISTRO/setup.bash

cw () {
    cd $ROS_WS
    if [[ -f $ROS_WS/devel/setup.bash ]]; then
        source $ROS_WS/devel/setup.bash
    fi
}

cs () {
    if [[ -d $ROS_WS/src ]]; then
        cd $ROS_WS/src
    fi
}

cm () {
    cd $ROS_WS
    catkin_make 
    source $ROS_WS/devel/setup.bash
}

alias ..="cd .."
alias ...="cd ..."
alias cls=clear
