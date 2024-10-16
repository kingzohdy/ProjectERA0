#!/bin/bash
# //  这是个极端优秀的脚本,需要大家共同来维护和添加相关的case
# //  让编程变得更轻松些,但reivew 代码的人工步骤还是不可或缺的
# //  该脚本能检查出以下case 的潜在core的发生
# //  伟大的人,做伟大的事,  潜力蛙(搞了两天才弄个大概) 信赖 splint
# //  create date :  20110330
# //  auth        :  ouyang
# //  modify date :
# //  modify auth :

usage()
{
	echo "./check_code.sh zone_shop.c"
	exit 2
}

[[ $# < 1 ]] && usage

codefile=${1}
logname=`whoami`
silence=${2}

# // 用例1：查找解析错误
# // 要这个步骤过了,下面的步骤才能过
if [ -z "$silence" ];then
echo "查找解析错误"
fi
/usr/local/bin/splint  -I/usr/local/$logname/myworld/comm_release/include \
	   -I/usr/local/$logname/myworld/src/comm \
	   -I/usr/local/$logname/myworld/src/dirty \
	   -I/usr/local/$logname/myworld/comm_release/include/apps/ \
	   -I/usr/local/$logname/myworld/src/auth_svr \
	   -I/usr/local/mysql/include/mysql \
	   -I/usr/local/$logname/myworld/src/zone/zone_svr \
	   $codefile
if [ -z "$silence" ];then
echo "导出问题点"
fi
/usr/local/bin/splint   -strict  \
          -I/usr/local/$logname/myworld/comm_release/include \
	   -I/usr/local/$logname/myworld/src/comm \
	   -I/usr/local/$logname/myworld/src/dirty \
	   -I/usr/local/$logname/myworld/comm_release/include/apps/ \
	   -I/usr/local/$logname/myworld/src/auth_svr \
           -I/usr/local/mysql/include/mysql \
	   -I/usr/local/$logname/myworld/src/zone/zone_svr \
	   $codefile > checksplint.txt
# // 用例2：NULL指针直接用
# // TAGNUM *ptagNum=NULL; z = ptagNum->iNum; 没赋值就用了NULL指针
if [ -z "$silence" ];then
echo " "
echo "NULL指针直接用"
cat checksplint.txt|egrep "Arrow access from null pointer"
cat checksplint.txt|egrep "Arrow access from possibly null pointer"	   
fi

# // 用例3：野指针直接用
# // TAGNUM *ptagNum; z = ptagNum->iNum; 没赋值就用了
if [ -z "$silence" ];then
	
echo " "
echo "野指针直接用"
fi
cat checksplint.txt|egrep "used before definition"|egrep "Variable" |egrep -v "stCltTime"|egrep -v "stConnectTime"|egrep -v "stConnectTime" |egrep -v "stCltTime"|egrep  -v "stDiff"|egrep -v "stSub"

# // 用例3：打印错误
# // printf("%s\n",i);

if [ -z "$silence" ];then
echo " "
echo "打印错误"
fi

cat checksplint.txt|egrep "Format argument" |egrep -v "expects int gets unsigned" |egrep -v "expects unsigned int gets"| \
	   egrep -v "snprintf \(%llu\) expects unsigned int"|egrep -v "sprintf \(%llu\) expects unsigned int" | \
	   egrep -v "snprintf \(%08x\) expects unsigned int"|egrep -v "s[,n]printf \(%d\) expects int gets" | \
	   egrep -v "snprintf \(%llu\) expects unsigned"|egrep -v "sprintf \(%d\) expects int gets" | \
	   egrep -v "snprintf \(%llu\) expects" |egrep -v "snprintf \(%ld\) expects long int gets"
	   



# // 用例4：内存溢出的情况,排出memcpy,memset等操作
# // int fun1(TAGNUM *pTagNum){ pTagNum[2].iNum = 1;}
if [ -z "$silence" ];then
		
echo " "
echo "内存溢出的情况"
fi
cat checksplint.txt|egrep   "Possible out-of-bounds store"|awk -F "store:" '{if ($2!="") {print $0}}'|egrep -v "\[i\]"|egrep -v "\[j\]"|egrep -v "\[k\]"|egrep -v "\[0\]"|egrep -v "\[1\]"|egrep "&"
cat checksplint.txt|egrep   "Possible out-of-bounds read"|awk -F "read:" '{if ($2!="") {print $0}}' |egrep -v "\[i\]"| \
                egrep -v "\[j\]"|egrep -v "\[k\]"|egrep -v "\[0\]"|egrep -v "\[1\]" |egrep "&"|egrep -v "iSize"|egrep -v "iMsgSrc"
   
# // 用例5：函数调用指针，指针已经初始化为NULL,然后被函数使用了
# // int fun1(TAGNUM *pTagNum){ pTagNum[2].iNum = 1;}
if [ -z "$silence" ];then
	
echo " "
echo "NULL被函数调用了"
fi

cat checksplint.txt|egrep   "passed as non-null param"|egrep -v "Null storage passed as non-null param"

# // 用例6: 测试sizeof指针
if [ -z "$silence" ];then
	
echo " "
echo "检查 sizeof(pst)是否被调用了!"
fi
  	egrep -n "sizeof\(p" $codefile|egrep -v "\-" 

rm checksplint.txt

if [ -z "$silence" ];then
	
echo "---OK---快去检查你的代码吧--"  
fi
