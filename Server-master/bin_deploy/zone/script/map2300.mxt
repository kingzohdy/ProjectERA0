;��ͼ�ű�, ����ɳĮ��1/500ˢ�±����

proc 4
	;4Ϊ��������id��5Ϊ�������6��7Ϊ����,8Ϊ��������ȼ�
	get_mon_info 1 4 1 1 1 1 6 7
	get_mon_level 1 8
	if @4 != 227009
	rand 5 1000
	if @5 < 0
	NEW_LIFE_DYN_MON 1 227009 1 @6 @7 0 0 0 1 0 @8 0 60
	endif
	endif
endproc
















