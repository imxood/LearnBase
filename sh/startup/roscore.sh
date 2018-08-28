#!/bin/bash

source /home/peak/robot/develop/ros_test/devel/setup.bash

export ROSCONSOLE_FORMAT='[${severity}][${node}][${logger}][${function}][${line}]: ${message}'

roscore
