
proc 1
	strvar tim time2 time3 loc
	var tim 0
	set_mon_var 0 8 @tim 1

	var time2 0
	set_mon_var 1 8 @time2 1
	
	var time3 0
	set_mon_var 2 8 @time3 1
	
	var loc 0
	set_mon_var 3 8 @loc 1
endproc 

proc 4
	strvar Mid
	get_mon_var 0 8 1 tim
	add tim 1
	set_mon_var 0 8 @tim 1
	if tim <= 80
	;5����֮�� ��30��ð��
		get_mon_var 1 8 1 time2
		add time2 1
		set_mon_var 1 8 @time2 1
		if time2 = 20
			strvar rad
			GET_MON_INFO 0 0 Mid 0 0 0 0 0
			rand rad 4
			if rad = 0
				mon_speech @Mid 0 1 1 ��ѽ������������һ�����������ĵȸ������ӣ���������ˡ�
			endif
			if rad = 1
				mon_speech @Mid 0 1 1 �����ɣ�����
			endif
			if rad = 2
				mon_speech @Mid 0 1 1 ����ܿ�����ˣ�������Ӵ��
			endif
			if rad = 3
				mon_speech @Mid 0 1 1 ������ʱ����1���ӵ�ʱ�򣬾͸Ͻ�������ɡ�
			endif
			var time2 0
			set_mon_var 1 8 @time2 1
		endif
		
		;
		get_mon_var 2 8 1 time3
		add time3 1
		set_mon_var 2 8 @time3 1
		if time3 = 3
			var time3 0
			set_mon_var 2 8 @time3 1
			
			get_mon_var 3 8 1 loc
			var loc 0
			set_mon_var 3 8 @loc 1
			
		endif
	endif
endproc 

proc 6
	get_mon_var 0 8 1 tim
	if tim <= 80
		PLAYER_COLLECT_BREAK
		GET_MON_INFO 0 0 Mid 0 0 0 0 0
		get_mon_var 3 8 1 loc
		if loc = 0
			;9����ֻ˵һ��
			mon_speech @Mid 0 1 1 ��̫�ż��ˣ����꣬�԰��������������ǻ������ӵ�Ӵ��
			var loc 1
			set_mon_var 3 8 @loc 1
			
			var time3 0
			set_mon_var 2 8 @time3 1
		endif
	else
		var tim 101	
		set_mon_var 0 8 @tim 1
	
	endif
endproc 








