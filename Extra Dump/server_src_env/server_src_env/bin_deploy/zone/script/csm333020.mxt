;������ҫ������

proc 1
	choose 0 100 101 �뿪����ͼ����Ϊ�Զ������������Ƿ�ȷ���뿪��
endproc

proc 100
	strvar rFlag
	DIANFENG_CLEAR_BAOMING_INFO 11 rFlag
	if @rFlag = 1 and @11 = 0
	;�������˳�
	else
		TRANSMIT 1000 26600 21400 0
	endif
endproc
