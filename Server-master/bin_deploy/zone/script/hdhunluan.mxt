;����Ԫ�ؽű���20-���������������� 30-�۳����߷��ر�־ 40-������������

proc 1
	DIALOG 1
	add_talk $_npc$:
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 20

	if 20 >= 10
		call 141
	else
		call 140
	endif

	talk
endproc 

proc 140
	add_talk #b#b�ƺ���һ��Ԫ��������ܸо��õ�����СС���������̺��ž޴��������ץ��ʱ�������������ռ������ɡ�
	add_talk #b#b
	add_talk #b#b#c186506��ʾ���������Ŀ���ͨ�����˻��á�#n
	
	strvar num
	rand num 5
	switch @num
		case 0
			option 150 �����ռ�������
			option 154 #����������ʲô
			option 155 #���Ǵ���������
			option 156 #�Ҽ����ڰ�����
			option 157 #��ֻ�Ǹ�·����
		endcase
		case 1
			option 154 #����������ʲô
			option 150 �����ռ�������
			option 155 #���Ǵ���������
			option 156 #�Ҽ����ڰ�����
			option 157 #��ֻ�Ǹ�·����
		endcase
		case 2
			option 154 #����������ʲô
			option 155 #���Ǵ���������
			option 150 �����ռ�������
			option 156 #�Ҽ����ڰ�����
			option 157 #��ֻ�Ǹ�·����
		endcase
		case 3
			option 154 #����������ʲô
			option 155 #���Ǵ���������
			option 156 #�Ҽ����ڰ�����
			option 150 �����ռ�������
			option 157 #��ֻ�Ǹ�·����
		endcase
		case 4
			option 154 #����������ʲô
			option 155 #���Ǵ���������
			option 156 #�Ҽ����ڰ�����
			option 157 #��ֻ�Ǹ�·����
			option 150 �����ռ�������
		endcase
	endswitch
endproc 

proc 141
	add_talk #b#b���ص�����������͸�����ȵ��������ǳ����ȶ���ͬʱЯ������������ļ���Σ�ա�
	add_talk #b#b
	add_talk #b#b#c186506��ʾ��ͬһʱ��ֻ��Я��10�������ص��������ġ������Ͻ����ټ����ռ������ɡ�#n
endproc 

proc 150
	
	
	strvar Mnum
	var Mnum 0
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 Mnum
	;���ص�������������
	strvar Znum
	var Znum 0
	GET_VAILD_ITEM_NUM_INPAK 5060030 1 Znum
	;������������
	
	strvar num
	var num 10
	SUB num @Mnum
	
	if Znum = 0
		call 153
		return
	else
		if @Znum >= @num
			dec_item_num_inpak 5060030 @num 30
			if 30 = 0
				add_item_num_inpak 5300963 @num 1 0
				
				add_sys_msg_start
				add_sys_msg ���������Ѿ�������������Ѹ�ٻص�ʱ��Ǵ������ٴ��Ͻ�
				set_sys_msg_mode 3 0 0
				msg_info
				
				call 151
			endif
		else
			if @num > @Znum
				dec_item_num_inpak 5060030 @Znum 30
				if 30 = 0
					add_item_num_inpak 5300963 @Znum 1 0
					
					add_sys_msg_start
					add_sys_msg ���������Ѿ�������������Ѹ�ٻص�ʱ��Ǵ������ٴ��Ͻ�
					set_sys_msg_mode 3 0 0
					msg_info
					
					call 151
				endif
			endif
		endif
	endif
endproc 

proc 151
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ռ��������󣬻���Ԫ���ƺ���С��һ�㡣
	add_talk #b#b
	add_talk #b#b#c186506��ʾ������Ԫ�ػ���8���Ӻ���ʧ����Ѹ�ٸϻ�ʱ��ǣ���#L��������@@[306013,1000]#n#c186506�Ͻ����ص��������ġ��ڻ���Ԫ�س����ڼ䣬�����ظ��Ͻ����޴������ơ�#n

	talk
endproc 

proc 153
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��û���������ģ����������Ԫ���ռ�������
	add_talk #b#b
	add_talk #b#b#c186506��ʾ���������Ŀ��������˻�л�á�#n
	add_talk #b#b#c186506��ʾ������Ԫ�ػ���8���Ӻ���ʧ����ץ��ʱ�䡣#n

	talk
endproc 

proc 154
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������һ�������ռ�װ�ã������������ռ������ڵĸ���������
	add_talk #b#b��������˵�������ͨ��������˵��������������װ�á�
	add_talk #b#b���ʱ��ǵĴ��������ƺ��߼����չ�װ���ռ���������

	talk
endproc

proc 155
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����������������Ѿ��ܾ��ˡ�
	add_talk #b#b��������ĩ�յĻ����У��ؿ��ѷ������Ť���ռ䣬���Ǳ����һ���ռ�����������

	talk
endproc

proc 156
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ڰ����ʣ���Ӧ��˵����Ҳ�ǻ���Ԫ�ذɡ�
	add_talk #b#b�Ѿ��ǲ�����ǰ�ˣ�ͻȻ���ֵĺڰ���ᣬץ�������ǵ�ͬ����
	add_talk #b#b������Щͬ�����ڰ�ħ����ʴ���������ڵĺڰ����ʡ�

	talk
endproc
	
	
	
	
