;�������,Ѳ�߶�ͷĿ
;�����¼�
proc 1
	;��С�ܼ���
	var 10 0
	set_mon_var 0 32 @10 1
endproc
proc 2
	strvar x y
	GET_MON_INFO 1 0 0 0 0 0 x y
	;��ʱ��е,��ӿ�
	;new_tmp_machine ::tempmachine3::
	;NEW_MON_MACHINE #����ֵ #�����еDefID #���� #���� #����x #����y #ʵ��id
	NEW_MON_MACHINE 1 291002 0 1 @x @y 0
endproc

;��ʱ�¼�
proc 4
	;��С�ܼ���
	get_mon_var 0 32 1 10
	if 10 > 0
		return
	endif
	
	IS_MON_FIGHTING 1 11
	if 11 = 0
		return
	endif
	
	var 10 1
	set_mon_var 0 32 @10 1
	
	get_mon_group 1 12
	strvar x y mid
	GET_MON_INFO 1 0 mid 0 0 0 x y

	new_mon 261046 1 @x @y 300 0 0 1 2
	SET_MON_GROUP 1 @2 @12
	
	LINK_MON 1 @12

	mon_speech @mid 2 2 1 ���������ߣ���ס���ǣ�	
endproc

