#!/bin/bash
# //  ���Ǹ���������Ľű�,��Ҫ��ҹ�ͬ��ά���������ص�case
# //  �ñ�̱�ø�����Щ,��reivew ������˹����軹�ǲ��ɻ�ȱ��
# //  �ýű��ܼ�������case ��Ǳ��core�ķ���
# //  ΰ�����,��ΰ�����,  Ǳ����(���������Ū�����) ���� splint
# //  create date :  20110330
# //  auth        :  ouyang
# //  modify date :
# //  modify auth :
cd ~/myworld/src/zone/zone_svr/
nohup ls *.c |egrep ".c"|awk -F "" '{print "./check_code.sh "$0}'|sh > ~/myworld/src/zone/zone_svr/tmp/check_code_everyday.txt 2>&1 &
