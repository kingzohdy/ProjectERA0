;;;;;��������


proc 1

	strvar Tim
	var Tim 0
	set_task_var 4604 0 8 @Tim 1
	strvar Loc
	var Loc 0
	set_task_var 4604 1 8 @Loc 1
	;
	
	strvar Tim2
	var Tim2 0
	set_task_var 4604 2 8 @Tim2 1
	;��ʱ��  10����û����������ʧ��
	strvar Loc2
	var Loc2 0
	set_task_var 4604 3 8 @Loc2 1
	
	strvar Loc3
	var Loc3 0
	set_task_var 4604 4 8 @Loc3 1
	;�����ʼѡ��ǰ��������
	
	strvar Loc4
	var Loc4 0
	set_task_var 4604 5 8 @Loc4 1
	
	strvar LocNum
	var LocNum 0
	set_task_var 4604 6 8 @LocNum 1
	;����6��ʱ���������
	
endproc

proc 6
	
	get_task_var 4604 4 8 1 Loc3 
	if Loc3 = 0
		return
	endif
	;�����ʼѡ��ǰ���߳���Χʱ,return
	strvar px py M
	GET_PLAYER_MAPINFO 1 1 1 px py
	GET_POINT_DIST 4700 16900 @px @py M

	if M > 1000
		var Loc3 0
		set_task_var 4604 4 8 @Loc3 1 
		add_sys_msg_start
		add_sys_msg �����밢ά��̫Զ���޷�������ϣ����밢ά�ɶԻ������¿�ʼ����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
	
	get_task_var 4604 0 8 1 Tim
	add Tim 1
	set_task_var 4604 0 8 @Tim 1	
	if Tim = 1
		GET_MAP_NPC_ID 302095 50 0 
		mon_speech @50 0 0 1 �װ���#Name#���������ҵ�Ҫ����������Ҫ��һ�µĶ����ſ���Ӵ��	
		;;�����ʼʱ��ֵ1������������proc 8
		var Loc4 1
		set_task_var 4604 5 8 @Loc4 1
	endif
	if Tim < 5
		return
	endif

	;ǰ5�뻹û��ʼ������ʱ����
	if Tim = 5
		get_task_var 4604 1 8 1 Loc
		GET_MAP_NPC_ID 302095 50 0 
		
		strvar Rad
		rand Rad 14
		if Rad = 0
			var Loc 0
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������裬������ʼ��
		endif
		if Rad = 1
			var Loc 10
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ��������ˣ�ᣬ������ʼ��
		endif
		if Rad = 2
			var Loc 20
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ��������װ����������ʼ��
		endif
		if Rad = 3
			var Loc 30
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������֣�������ʼ��
		endif
		if Rad = 4
			var Loc 40
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ��������ʹ�࣬������ʼ��
		endif
		if Rad = 5
			var Loc 50
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������裬������ʼ��
		endif
		if Rad = 6
			var Loc 60
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ��������������������ʼ��
		endif
		if Rad = 7
			var Loc 70
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������ǣ�������ʼ��
		endif
		if Rad = 8
			var Loc 80
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ��������������������ʼ��
		endif
		if Rad = 9
			var Loc 90
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������ƣ�������ʼ��
		endif
		if Rad = 10
			var Loc 100
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ����������Ҿ��������ʼ��
		endif
		if Rad = 11
			var Loc 110
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������Σ�������ʼ��
		endif
		if Rad = 12
			var Loc 120
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ�����������֣�������ʼ��
		endif
		if Rad = 13
			var Loc 130
			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ��һ����������˼��������ʼ��
		endif
	endif
	
;	get_task_var 4604 1 8 1 Loc
;	add_sys_msg_start
;	add_sys_msg -----------��ʱL-o-c=$@Loc$
;;	set_sys_msg_mode 1 0 0
;	msg_info
	
	if Tim = 10
		get_task_var 4604 1 8 1 Loc
		GET_MAP_NPC_ID 302095 50 0 
		
		if Loc = 0
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif
		
		if Loc = 10
			mon_speech @50 0 0 1 #Gˣ��#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif
		if Loc = 20
			mon_speech @50 0 0 1 #Gװ��#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 30
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 40
			mon_speech @50 0 0 1 #Gʹ��#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 50
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 60
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 70
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 80
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 90
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 100
			mon_speech @50 0 0 1 #G��Ҿ#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 110
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 120
			mon_speech @50 0 0 1 #G����#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		if Loc = 130
			mon_speech @50 0 0 1 #G��˼#n����ʼ��
			
			;NPC���������������ֵ0
			var Loc4 0
			set_task_var 4604 5 8 @Loc4 1
		endif	
		
;		add_sys_msg_start
;		add_sys_msg 10��ʱL-o-c=$@Loc$
;		set_sys_msg_mode 1 0 0
;		msg_info
	endif
	if Tim >= 20
		var Tim 4
		set_task_var 4604 0 8 @Tim 1
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;����6���������
	get_task_var 4604 6 8 1 LocNum
	if LocNum >= 6
		GET_MAP_NPC_ID 302095 50 0 
		mon_speech @50 0 0 1 OK������������б�����֣�ϣ�����л����ܿ�����ľ����ݳ����ټ���
		task_fini 4604 1
	endif	
	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;10�벻������ȷ�Ķ�����ʾ����
	if Tim >= 10 
	
		get_task_var 4604 5 8 1 Loc4
		if Loc4 = 1
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			return
		endif	
		;�����ɹ��󲻽�����ж�
		get_task_var 4604 2 8 1 Tim2 
		add Tim2 1
		set_task_var 4604 2 8 @Tim2 1

		if Tim2 = 10
			add_sys_msg_start
			add_sys_msg ����10����û���������������µ��NPC��ʼ��
			set_sys_msg_mode 4 0 0
			msg_info
			call 101
		endif
	endif
endproc

proc 100
	add_sys_msg_start
	add_sys_msg ����ʹ�ô��������µ��NPC��ʼ
	set_sys_msg_mode 4 0 0
	msg_info
	call 101
;	TASK_FAIL 4604 1
endproc
proc 101 
	get_task_var 4604 4 8 1 Loc3 
	var Loc3 0
	set_task_var 4604 4 8 @Loc3 1
	
	;��������
	var LocNum 0
	set_task_var 4604 6 8 @LocNum 1	
	
	get_task_var 4604 2 8 1 Tim2 
	var Tim2 0
	set_task_var 4604 2 8 @Tim2 1
	
	
	get_task_var 4604 1 8 1 Loc
	var Loc 0
	set_task_var 4604 1 8 @Loc 1
endproc




proc 8
	strvar Ret
	task_stat 4604 1 Ret
	if Ret = 1
		return
	endif	


	get_task_var 4604 4 8 1 Loc3 
	if Loc3 = 0
		return
	endif
	;�����ʼѡ���
	
	get_task_var 4604 0 8 1 Tim
	if Tim < 5
		return
	endif
	;ǰ5�뻹û��ʼ������ʱ����
	
	get_task_var 4604 5 8 1 Loc4 
	;�����ȷ�Ķ���������
	if Loc4 = 1
		return
	endif

	
	strvar MotIon
	ID_PLAYER_MOTION 0 0 MotIon    
	GET_MAP_NPC_ID 302095 50 0              
	
	get_task_var 4604 1 8 1 Loc
	get_task_var 4604 5 8 1 Loc4
	get_task_var 4604 6 8 1 LocNum 
	if Loc = 0	
		if MotIon = 10702
		;����
;			var Loc 10
;			set_task_var 4604 1 8 @Loc 1
			
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	
	if Loc = 10
		if MotIon = 10707
		;ˣ��
;			var Loc 20
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 20
		if MotIon = 10704
		;װ��
;			var Loc 30
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 30
		if MotIon = 10708
		;����
;			var Loc 40
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 40
		if MotIon = 10713
		;ʹ��
;			var Loc 50
;			set_task_var 4604 1 8 @Loc 1
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 50
		if MotIon = 10703
		;����
		;	var Loc 30
		;	set_task_var 4604 1 8 @Loc 1
	;		GET_MAP_NPC_ID 302095 50 0 
	;		mon_speech @50 0 0 1 OK����л#Name#������������ݵ��˽��������������ټ���
	;		task_fini 4604 1
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 60
		if MotIon = 10700
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 70
		if MotIon = 10701
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 80
		if MotIon = 10705
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 90
		if MotIon = 10706
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 100
		if MotIon = 10709
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 110
		if MotIon = 10710
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 120
		if MotIon = 10711
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
	if Loc = 130
		if MotIon = 10712
		;����
			mon_speech @50 0 0 1 ̫���ˣ���ֱ���������
			
			get_task_var 4604 2 8 1 Tim2 
			var Tim2 0
			set_task_var 4604 2 8 @Tim2 1
			
			;�����ȷ�Ķ�����ֵΪ1
			var Loc4 1
			set_task_var 4604 5 8 @Loc4 1
			
			;����6���������
			add LocNum 1
			set_task_var 4604 6 8 @LocNum 1
			return
		else
			call 100
		endif
	endif
endproc
