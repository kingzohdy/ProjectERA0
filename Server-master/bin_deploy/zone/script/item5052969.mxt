;���쳩���

proc 1
	strvar ret
	GET_PACK_SPACE 0 ret
	if ret > 0

		DEC_ITEM_NUM_INPAK 5052969 1 10
		if 10 = 0
			ADD_BIND_ITEM_NUM_INPAK 5200020 1 1 0
		endif
	else
		MAIL_TEXT #r �������ı����������㣬�����쳩����е���Ʒͨ���ʼĵķ�ʽ���Ÿ�������ע�������Ʒ��
		sys_mail_bind 1 5200020 1 1 0 0 0 ���쳩���$���Ԫ�ͷ�����
		DEC_ITEM_NUM_INPAK 5052969 1 11
	endif
endproc 

