;��������

proc 6
	RIDE_OFF 1
	strvar ret
	IS_CAN_BIANSHEN ret 1
	if ret != 1
		ADD_SYS_MSG_START
		ADD_SYS_MSG �������޷��������ӣ��ɻָ�������̬������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc

Proc 2
	strvar memID ret
	get_player_id 0 memID
	TARGET_ADD_STATUS 1 @memID 1330 2 1800 ret
	if ret = 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG ��ɹ��ҵ�һ�����������ʡ����Ͻ����ȥ���Ͷ���ȡ������
		SET_SYS_MSG_MODE 3 0 0
		MSG_INFO
	endif
endproc
