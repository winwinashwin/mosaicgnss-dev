FROM ros:kinetic-ros-base-xenial

RUN apt-get update && apt-get install --no-install-recommends -y \
    libpcap-dev \
    ros-kinetic-nmea-msgs \
    && rm -rf /var/lib/apt/lists/*

COPY root.bashrc /root/.bashrc

ENV ROS_WS /catkin_ws
RUN mkdir -p ${ROS_WS}/src
RUN ["bash", "-c", "source /opt/ros/kinetic/setup.bash && cd ${ROS_WS} && catkin_make"]

WORKDIR ${ROS_WS}