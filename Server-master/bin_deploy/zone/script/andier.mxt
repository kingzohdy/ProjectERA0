

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��̫���������Ա�ر����ڰ�����ƺ�������ı��ĳ��������ʽ�����Ǳ���������ǳɹ�ǰ��ֹ������ı��
	add_talk #b#b������ʱ����ȣ������������˾����ټ���·����Э����ֻҪ���㹻��ʵ�����Ž���飬������Ҵ��͵�̫������С�
	add_talk #b#b#c186506[̫�����]��������#n	
	add_talk #b#b#c186506����ȼ���75��#n	
;	add_talk #b#bͨ��̫�����Ĵ������Ѿ���,�����޸��С���
	option 110 #����̫�����
	
	talk
endproc 
proc 20 
	return
endproc

proc 110
;	ENTER_PWORLD 100 0 0
;
;		
;		TEAM_PWORLD_INFO 300 100 0 0
;		if 300 = 0
;			ENTER_PWORLD 100 0 0
;		OPEN_PWORLD_UI 100
		OPEN_PWORLD_UI 11
;		else
;	
;			IS_CAPTAIN 15 0
;			if 15 = 0 
;				open_pworld_ui 100
;			else
;
;				add_sys_msg_start
;;				add_sys_msg �㲻�Ƕӳ����˸�����Ҫ�ӳ��������룡
	;			set_sys_msg_mode 4 0 0
;				msg_info
;
;			endif
;		endif
endproc