

proc 1
	DIALOG 1
	add_talk $_npc$:
	
	strvar randtalk
	rand randtalk 9
	switch randtalk
		case 0
			add_talk #b#b$_player$���������������ǵ����һ�������������Ĳ�������
		endcase
		case 1
			add_talk #b#bһ��ʼ�һ������������Ǵβ��ɥ����Ѫɫ׷����֮�֣��Ҿͺ����ˡ���
		endcase
		case 2
			add_talk #b#b�Ҳ�֪��Ϊʲô����ʹֻ��������˵����Ҳ��Ī���ĺ��£������뿪�����һ����ȫ�ĵط�ȥ����$_player$��������뿪�������
		endcase
		case 3
			add_talk #b#bû�õģ�û�õģ����ݶ�˹ӵ�������޾��ķ��������Ƕ��������ġ���
		endcase
		case 4
			add_talk #b#bΪʲô���˺��������ǣ�����ʲô��û������ģ���ʲô��û������
		endcase
		case 5
			add_talk #b#b����Ҫ���Ű�˹���أ����ᱣ�����ǵģ�
		endcase
		case 6
			add_talk #b#b÷��Ү����������Ѱ��ɱ�����ݶ�˹�İ취����һ����ɹ��ģ�
		endcase
		case 7
			add_talk #b#b÷��Ү����ֻ�Ǹ�Ů�ˣ�������ô������һ��Ů�ˣ�
		endcase
		case 8
			add_talk #b#b�����˹�򳤻��ھͺ��ˣ�
		endcase
	endswitch
	strvar Rad Ret
	task_stat 1544 1 Ret
	if Ret = 2
	
	
		get_task_var 1544 16 8 1 val
		if val < 1
			rand Rad 3
			switch Rad
;			case 0
;				option 110 #�Ȳ�����֮��
;			endcase
				case 0
					option 110 #�����������������ֻ���Լ��ƿأ�
				endcase
				case 1
					option 110 #Ϊ��ʤ����Ϊ�����棬����ս���ɣ�
				endcase
				case 2
					option 110 #��������Ը��ū��������
				endcase
			endswitch
		endif
	endif
	talk
endproc 
proc 110
	strvar Mid1 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid1
	set_task_var 1544 0 32 @Mid1 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid1 Retm
	
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
		strvar Loc
		get_task_var 1544 17 8 1 Loc
		var Loc 10
		set_task_var 1544 17 8 @Loc 1
		add_sys_msg_start
		add_sys_msg ��һ��������Ӧ����ĺ��٣��콫������ȥ����˹���ذɣ�
		set_sys_msg_mode 3 0 0
		msg_info	
	endif
	
	if Mid1 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc

proc 111
	strvar Mid2 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid2
	set_task_var 1544 4 32 @Mid2 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid2 Retm
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
	endif
	if Mid2 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc 
proc 112
	strvar Mid3 val Pmid Retm
	PLAYER_CALL_BUB 204107 40 Mid3
	set_task_var 1544 8 32 @Mid3 1
	GET_PLAYER_ID 1 Pmid
	SET_MON_CURR @Mid3 Retm
	if Retm = 0
		set_mon_var 0 32 @Pmid 1     
	endif   
	
	if Mid3 >= 0
		get_task_var 1544 16 8 1 val
		add val 1
		set_task_var 1544 16 8 @val 1
;		DEL_NPC 0 0
	endif
endproc 
;proc 113
;	strvar Mid4 val
;	PLAYER_CALL_BUB 204107 40 Mid4
;	set_task_var 1544 12 32 @Mid4 1
;	
;	if Mid4 >= 0
;		get_task_var 1544 16 8 1 val
;		add val 1
;		set_task_var 1544 16 8 @val 1
;	endif
;endproc 