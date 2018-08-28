#!/bin/bash

# sed行内容处理，文件内容并未改变:
#     -n 只有经过sed特殊处理的那一行(或者动作)才会被列出来
#     动作:
#         a 新增，新增内容会出现在下一行
#         i 插入，出现在上一行
#         d 删除
#         c 取代，其后的内容会替换n1行与n2行
#         s 取代，一般配合正则使用

echo -e "只输出影响到的2到7行"
nl delete.txt | sed -n '2,7p'

echo -e "\n替换2到7行为空"
nl delete.txt | sed '2,7c \'

echo -e "\n删除2到7行"
nl delete.txt | sed '2,7d'

echo -e "\n对每一行进行正则替换:删除前面的'-- Up-to-date: '"
fileLists=$(cat delete.txt | sed 's/-- Up-to-date: //g')
for file in $fileLists; do
    sudo rm $file
done