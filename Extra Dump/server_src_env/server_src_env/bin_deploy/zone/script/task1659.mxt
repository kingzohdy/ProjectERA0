
proc 1

	STRVAR mid x2 y2 a
	var 10 0
	set_task_var 1659 0 8 @10 1
	var 11 0
	set_task_var 1659 4 32 @11 1
	var 12 0
	set_task_var 1659 1 8 @12 1

endproc

proc 6
		get_task_var 1659 0 8 1 10
		get_task_var 1659 4 32 1 11
		get_task_var 1659 1 8 1 12
		
		get_player_mapinfo 0 1 2 x2 y2
		get_point_dist @x2 @y2 15500 13600 a

		
		if @12 = 1
			add 10 1
			
			if @10 = 3
				
					if @a > 1500
						TASK_FAIL 1659 1
					else
						MON_SPEECH  @11 1 0 1 ��˭��Ҫ���ˣ�����������ڣ��ٲ��úøɻ������ȫ����ǽ�ϵ����裡
					endif
			endif
			if @10 = 6
				
					if @a > 1500
						TASK_FAIL 1659 1
					else
						MON_SPEECH  @11 1 0 1 #Name#����ɵվ�������ʲô�����������𣡻����Ͻ��Ѷ��ٹ��˰ѿ�ض�ת�Ƶ��ƽ��ȥ��
					endif
				endif
			if @10 = 11
				
					if @a > 1500
						TASK_FAIL 1659 1
					else
						MON_SPEECH  @11 1 0 1 ���õ����ϴ������ǹ�����������ʮ����Ҳ�������𣬸Ͻ��ɻ�ȥ��
					endif
				endif
			if @10 = 13
				TASK_FINI 1659 1
			endif
			set_task_var 1659 0 8 @10 1
		endif
endproc






