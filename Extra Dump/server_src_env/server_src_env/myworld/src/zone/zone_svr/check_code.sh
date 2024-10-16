#!/bin/bash
# //  ���Ǹ���������Ľű�,��Ҫ��ҹ�ͬ��ά���������ص�case
# //  �ñ�̱�ø�����Щ,��reivew ������˹����軹�ǲ��ɻ�ȱ��
# //  �ýű��ܼ�������case ��Ǳ��core�ķ���
# //  ΰ�����,��ΰ�����,  Ǳ����(���������Ū�����) ���� splint
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

# // ����1�����ҽ�������
# // Ҫ����������,����Ĳ�����ܹ�
if [ -z "$silence" ];then
echo "���ҽ�������"
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
echo "���������"
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
# // ����2��NULLָ��ֱ����
# // TAGNUM *ptagNum=NULL; z = ptagNum->iNum; û��ֵ������NULLָ��
if [ -z "$silence" ];then
echo " "
echo "NULLָ��ֱ����"
cat checksplint.txt|egrep "Arrow access from null pointer"
cat checksplint.txt|egrep "Arrow access from possibly null pointer"	   
fi

# // ����3��Ұָ��ֱ����
# // TAGNUM *ptagNum; z = ptagNum->iNum; û��ֵ������
if [ -z "$silence" ];then
	
echo " "
echo "Ұָ��ֱ����"
fi
cat checksplint.txt|egrep "used before definition"|egrep "Variable" |egrep -v "stCltTime"|egrep -v "stConnectTime"|egrep -v "stConnectTime" |egrep -v "stCltTime"|egrep  -v "stDiff"|egrep -v "stSub"

# // ����3����ӡ����
# // printf("%s\n",i);

if [ -z "$silence" ];then
echo " "
echo "��ӡ����"
fi

cat checksplint.txt|egrep "Format argument" |egrep -v "expects int gets unsigned" |egrep -v "expects unsigned int gets"| \
	   egrep -v "snprintf \(%llu\) expects unsigned int"|egrep -v "sprintf \(%llu\) expects unsigned int" | \
	   egrep -v "snprintf \(%08x\) expects unsigned int"|egrep -v "s[,n]printf \(%d\) expects int gets" | \
	   egrep -v "snprintf \(%llu\) expects unsigned"|egrep -v "sprintf \(%d\) expects int gets" | \
	   egrep -v "snprintf \(%llu\) expects" |egrep -v "snprintf \(%ld\) expects long int gets"
	   



# // ����4���ڴ���������,�ų�memcpy,memset�Ȳ���
# // int fun1(TAGNUM *pTagNum){ pTagNum[2].iNum = 1;}
if [ -z "$silence" ];then
		
echo " "
echo "�ڴ���������"
fi
cat checksplint.txt|egrep   "Possible out-of-bounds store"|awk -F "store:" '{if ($2!="") {print $0}}'|egrep -v "\[i\]"|egrep -v "\[j\]"|egrep -v "\[k\]"|egrep -v "\[0\]"|egrep -v "\[1\]"|egrep "&"
cat checksplint.txt|egrep   "Possible out-of-bounds read"|awk -F "read:" '{if ($2!="") {print $0}}' |egrep -v "\[i\]"| \
                egrep -v "\[j\]"|egrep -v "\[k\]"|egrep -v "\[0\]"|egrep -v "\[1\]" |egrep "&"|egrep -v "iSize"|egrep -v "iMsgSrc"
   
# // ����5����������ָ�룬ָ���Ѿ���ʼ��ΪNULL,Ȼ�󱻺���ʹ����
# // int fun1(TAGNUM *pTagNum){ pTagNum[2].iNum = 1;}
if [ -z "$silence" ];then
	
echo " "
echo "NULL������������"
fi

cat checksplint.txt|egrep   "passed as non-null param"|egrep -v "Null storage passed as non-null param"

# // ����6: ����sizeofָ��
if [ -z "$silence" ];then
	
echo " "
echo "��� sizeof(pst)�Ƿ񱻵�����!"
fi
  	egrep -n "sizeof\(p" $codefile|egrep -v "\-" 

rm checksplint.txt

if [ -z "$silence" ];then
	
echo "---OK---��ȥ�����Ĵ����--"  
fi
