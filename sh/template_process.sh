#!/bin/bash
cur_path=$(pwd)
echo ${cur_path}
cp template_process.txt template_process_1.sh
sed -i "s#{{path}}#$cur_path#g" template_process_1.sh