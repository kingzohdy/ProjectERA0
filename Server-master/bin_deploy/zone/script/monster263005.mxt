;������� ,Ӱ���ܿˡ�����(boss)
proc 1
	var 10 0
	set_mon_var 0 32 @10 1
endproc
;��ʱ�¼�
proc 4
	;��̹�˼�ʱ
	get_mon_var 0 32 1 10
	add 10 1
	set_mon_var 0 32 @10 1
	;360��,120*3
	sub 10 1
	mod 10 120
	if 10 = 0
		;����̨
		var 10 65
		new_mon_by_dir 260000 1 18189 4877 0 0 0 @10 1 2
		new_mon_by_dir 260000 1 17223 5195 0 0 0 @10 1 2
	endif
endproc
;ai_home,�����λ�¼�
proc 9
	IS_MON_FIGHTING 1 10
	if 10 > 0
		return
	endif
	;boss5�Ƿ��λ��
	VAR 27 1
	set_pworld_var 68 32 @27 1
	strvar mid 
	GET_MON_INFO 1 0 mid 0 0 0 0 0
;	mon_speech @mid 2 2 1 ��Ҫ�ؼ�
	DESTROY_MON 1
endproc
