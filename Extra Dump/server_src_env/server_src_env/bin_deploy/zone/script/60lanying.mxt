

proc 1
	dialog 1
	add_talk $_npc$:
	get_pworld_var 103 8 1 43 

	if 43 = 0
	;��ͨ
		add_talk #b#b�������ƣ�����˭ѽ��ѽ������������$_player$��
		add_talk #b#b���Ѿ���̽���ˣ�Ҫ�����ν�磬�ͱ���Ҫɱ������ػ��ߣ�Ԧ���ߡ����ֵ�³���������ˡ����������졣
		add_talk #b#b�һ�������Ƴ���һ�����֮�ţ������ǻ�ɱ�˽���ػ��ߣ���һ�����֮�žͻ�򿪣�
		add_talk #b#b����ֻ��Ҫ�����Ƴ����ͺã�����������ͽ����Ұɣ�
		add_talk #b#bף���Ǻ��ˣ�
		option 151 #����������
		
	endif
	if 43 = 1
		;ר��
		add_talk #b#b������������̫ǿ���ˣ�$_player$����û�취������ɾ��˹������������ǵİ�����
		add_talk #b#b�����Ƴ�������֮ǰ����һЩ���ұ���Ҫ˵��
		add_talk #b#b���ǵ�ʱ��ܽ��ȣ�һ���Ƴ�������֮�ţ����Ǳ�������̵�ʱ����ˣ��������ǻ���ͬ���ν��һ����𣡲������ν�����쳣����ƾ���������޷����������ԣ����Ǳ���Ҫ��һЩ�Ƽ���е��ֻ�пƼ���е���ܴݻ����ν�磬���ܾȳ����ǵ�ս�ѣ�
		add_talk #b#b$_player$��ϣ����������Ҹղ���˵�Ļ��������׼�����ˣ��͸����ң�
		
		option 150 #�������֮��
	endif
	if 43 = 10
		add_talk #b#b���Ѿ����ҽ���鷳����
	endif
	talk
endproc 
proc 150
	get_pworld_var 103 8 1 43 
	if 43 != 1
		return
	endif
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;ר�Ҹ�������ʱ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	strvar pwid
	GET_ID_PLAYER_INFO 0 20 pwid
	;��ȡ���npcʱ��ɫ���ڵĸ���id
	
	strvar time bian
	var bian 1800
	
	strvar pw1 pw2 pw3 pw4 pw5 pw6
	strvar ret play player1 player2 player3 player4 player5 player6 num
	GET_TEAM_PLAYER_ID ret 0 num player1 player2 player3 player4 player5 player6  
	if ret = 0
		GET_ID_PLAYER_INFO @player1 20 pw1
		if pwid = @pw1
			COUNT_DOWN 11 @player1 @bian ������ӥ��Ա����ʱ��
		endif
		GET_ID_PLAYER_INFO @player2 20 pw2
		if pwid = @pw2
			COUNT_DOWN 11 @player2 @bian ������ӥ��Ա����ʱ��
		endif
		GET_ID_PLAYER_INFO @player3 20 pw3
		if pwid = @pw3
			COUNT_DOWN 11 @player3 @bian ������ӥ��Ա����ʱ��
		endif
		GET_ID_PLAYER_INFO @player4 20 pw4
		if pwid = @pw4
			COUNT_DOWN 11 @player4 @bian ������ӥ��Ա����ʱ��
		endif
		GET_ID_PLAYER_INFO @player5 20 pw5
		if pwid = @pw5
			COUNT_DOWN 11 @player5 @bian ������ӥ��Ա����ʱ��
		endif
		GET_ID_PLAYER_INFO @player6 20 pw6
		if pwid = @pw6
			COUNT_DOWN 11 @player6 @bian ������ӥ��Ա����ʱ��
		endif
	else
		get_player_id 0 55
		COUNT_DOWN 11 @55 @bian ������ӥ��Ա����ʱ��
	endif
	
	;��¼����ʱ��ʱ��
	var 10 @bian
	set_pworld_var 24 32 @10 1
	
	strvar time 
	;��¼��1970����������ʱ������
	GET_TIME_SEC 20
	get_pworld_var 16 32 1 time
	var time @20
	set_pworld_var 16 32 @time 0
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
	;ɾ���赲����
	get_pworld_var 12 32 1 13 
	DEL_ID_MON @13 1
	;����ר�Ҹ������ж�-��ʱ��
	get_pworld_var 107 8 1 47 
	var 47 1
	set_pworld_var 107 8 @47 1
	
;;;;;;;;;;;;;����;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	strvar ret play player1 player2 player3 player4 player5 player6 num
;	GET_TEAM_PLAYER_ID ret 0 num player1 player2 player3 player4 player5 player6  
;	if ret = 0
;
;		TRANSMIT_CAMERA @player1 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player2 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player3 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player4 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player5 0 11600 20600 30900 0 0
;		TRANSMIT_CAMERA @player6 0 11600 20600 30900 0 0
;		
;	else
;	
;		GET_PLAYER_ID 1 play
;		TRANSMIT_CAMERA @play 45 11600 20600 30900 135 0
;	endif
	;��ѡ��
	var 43 10
	set_pworld_var 103 8 @43 1 	
	
	;������һ������
	strvar roc
	get_pworld_var 101 8 1 roc
	var roc 1
	set_pworld_var 101 8 @roc 1
	
	add_sys_msg_start
	add_sys_msg Ԧ���ߣ������������֣������������Ҽ��ᣬ30��󷢶�����
	set_sys_msg_mode 4 0 0
	map_msg_info
	
	get_player_mapinfo 1 20 1 1 1 
	if 20 = 11800 or 20 = 11900
		;6.27���� npcð��
		strvar mid
		GET_MAP_NPC_ID 201103 mid 1
		mon_speech @mid 0 2 1 �ո�ǰ������и����˺��ܹ�ȥ��,��֪������������ʲô
	endif
endproc 



proc 151

	get_pworld_var 103 8 1 43 
	if 43 != 0
		return
	endif
	;��ѡ��
	var 43 10
	set_pworld_var 103 8 @43 1 	
	
	;ɾ���赲����
	get_pworld_var 12 32 1 13 
	DEL_ID_MON @13 1
	
	
	;������һ������
	strvar roc
	get_pworld_var 101 8 1 roc
	var roc 1
	set_pworld_var 101 8 @roc 1
	
	add_sys_msg_start
	add_sys_msg Ԧ���ߣ������������֣������������Ҽ��ᣬ30��󷢶�����
	set_sys_msg_mode 4 0 0
	map_msg_info
	
	
	get_player_mapinfo 1 20 1 1 1 
	if 20 = 11800 or 20 = 11900
		
		;6.27���� npcð��
		strvar mid
		GET_MAP_NPC_ID 201103 mid 1
		mon_speech @mid 0 2 1 �ո�ǰ������и����˺��ܹ�ȥ��,��֪������������ʲô
	endif
endproc












