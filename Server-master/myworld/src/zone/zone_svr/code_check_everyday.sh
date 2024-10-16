#!/bin/bash
# //  这是个极端优秀的脚本,需要大家共同来维护和添加相关的case
# //  让编程变得更轻松些,但reivew 代码的人工步骤还是不可或缺的
# //  该脚本能检查出以下case 的潜在core的发生
# //  伟大的人,做伟大的事,  潜力蛙(搞了两天才弄个大概) 信赖 splint
# //  create date :  20110330
# //  auth        :  ouyang
# //  modify date :
# //  modify auth :
cd ~/myworld/src/zone/zone_svr/
nohup ls *.c |egrep ".c"|awk -F "" '{print "./check_code.sh "$0}'|sh > ~/myworld/src/zone/zone_svr/tmp/check_code_everyday.txt 2>&1 &
