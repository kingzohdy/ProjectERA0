; ���ԶԻ��ű�ָ��
;
PROC 1
	GET_PLAYER_BASE_ATT 4 1 2 3
	DIALOG 1
	IF 3 = 1
	ADD_TALK $_npc$��
	ADD_TALK ��ã�$_player$,����λսʿ,��������Ĳֿ����Ա.��ʲô�ܰ������?
	ELSEIF 3 = 2
	ADD_TALK $_npc$��
	ADD_TALK ��ã�$_player$,����λ��ʦ,��������Ĳֿ����Ա.��ʲô�ܰ������?
	ELSE
	ADD_TALK $_npc$��
	ADD_TALK ��ã�$_player$,����λǹ��,��������Ĳֿ����Ա.��ʲô�ܰ������?
	ENDIF
   OPTION 22 #���븱��
   OPTION 23 #�򿪲ֿ�.
   TALK               
ENDPROC

PROC 22
	ENTER_PWORLD 100 0 1
ENDPROC  

PROC 23
	OPEN_STASH	
	RETURN
ENDPROC 


