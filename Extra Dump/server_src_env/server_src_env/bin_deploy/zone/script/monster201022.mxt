;������� ,С�ӳ����
;����
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	if x = 14808 and y = 14228
		return
	endif
	;���������
	get_pworld_var 24 32 1 16
	
	NEW_TMP_ITEM 1400020 @x @y 0 0 0 1
endproc
;����Ѳ���յ�
proc 4
	;�Ƿ��̨�յ�
	strvar x y dis
	GET_MON_INFO 1 0 0 0 0 0 x y 
	GET_POINT_DIST @x @y 14808 14228 dis
	if dis > 200
		return
	endif	
	
	;ɾ�Լ�
	DEL_MON 1
	;����̨��Ѫ
	;��̳��ʵ��id
	get_pworld_var 16 32 1 14
	mon_set_hp 5 @14 1
	;count30,�������
	var 30 0
	get_pworld_var 80 32 1 30
	add 30 1
	set_pworld_var 80 32 @30 1
	delete_mon_by_defid 261059 1
	delete_mon_by_defid 261060 1
	delete_mon_by_defid 261061 1
	delete_mon_by_defid 261062 1
	delete_mon_by_defid 261063 1
	delete_mon_by_defid 261064 1
	delete_mon_by_defid 261065 1
	delete_mon_by_defid 261066 1
	delete_mon_by_defid 261067 1
	delete_mon_by_defid 261068 1
	delete_mon_by_defid 261069 1
	if 30 < 10
		add_sys_msg_start
		add_sys_msg $@30$�������߳ɹ�����ڰ���̳�׼��� 
		set_sys_msg_mode 1 0 0
		map_msg_info
	endif
	get_pworld_var 8 32 1 12
	if 30 >= 10 and 12 != 6
		;��������ؼ���
		var 12 6
		set_pworld_var 8 32 @12 1
	endif
	strvar id 
	var id 261059
	if 30 > 0 and 30 <= 10
		add id @30
		new_mon @id 1 14808 14228 0 0 0 1 0
	endif
	
	
endproc
