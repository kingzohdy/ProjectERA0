proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ʵ����������Ҳ�ܿ��������Լ���Ӱ�ӣ������ǵ��۾�������ǵ��ˣ��ǻ������Ǻ�ƽ����Ļ��ף������ǵ��۾�������ǵ��ˣ����������ǲƸ�����������������
	add_talk #b#b�������ź�����ƽ���ദ���ҳɹ��ˣ�����˵�������ɹ��ˣ���������Դ����֮ǰ����ˡ�
;	task_add_option 1312 1 101
	task_add_option 1313 1 102
	task_add_option 1314 1 103
	task_add_option 1315 1 104
	task_add_option 1316 1 105
;	task_add_option 1317 1 106
	task_add_option 1318 1 107
	task_add_option 1319 1 108
	task_add_option 1323 1 109
	task_add_option 1324 1 110
	
	
	task_add_option 1322 1 111
	task_add_option 1337 1 112
	task_add_option 1335 1 113
	
	strvar Ret
	task_stat 1319 1 Ret
	if Ret = 2
;		option 300 �Ƿ���Ҫ������һ�̣�
	endif
	

	talk
endproc

;proc 101
;    task_do_option 1312 1
;endproc

proc 102
    task_do_option 1313 1
endproc
proc 103
    task_do_option 1314 1
endproc
proc 104
    task_do_option 1315 1
endproc
proc 105
    task_do_option 1316 1
endproc

proc 107
    task_do_option 1318 1
endproc

proc 108
    task_do_option 1319 1
endproc
proc 109
    task_do_option 1323 1
endproc

proc 110
    task_do_option 1324 1
endproc
proc 111
    task_do_option 1322 1
endproc
proc 112
    task_do_option 1337 1
endproc
proc 113
    task_do_option 1335 1
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҿ���ֱ�ӽ����͵�Ŀ�ĵأ�������;����ͣӴ��
	
		option 301 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 302 #�����������Լ���ȥ��
	talk

endproc 
proc 301
	
	NEW_OWNER_TMP_ITEM 1460062 9700 16500 0 0 0 303 1095 1 3600 1
endproc 
proc 302
	return
endproc 

proc 2
	get_trig_task 1 600
	if 600 = 1324 
		task_fini 1324 1
	endif

	if 600 = 1319
;		dialog 1
;		add_talk $_npc$:
;		add_talk #b#bС���㣬���������׺ݵĿ�����ħ��
		task_fini 1319 0
;		option 300 �Ƿ���Ҫ������һ�̣�
;		talk
	endif
	if 600 = 1316
	
			ADD_ITEM_NUM_INPAK 5300385 1 1  0
	;		ADD_ITEM_NUM_INPAK 5300386 1 1 0
	endif

;	if 600 = 1060
;		GET_PLAYER_ID 1 54
;		GET_PLAYER_BASE_ATT 1 50 51 52
;
;		ADD_SYS_MSG_START
;		ADD_SYS_MSG ���ID=$@54$ �Ա�=$@50$ 
;		SET_SYS_MSG_MODE 1 0 0
;		MSG_INFO
;
;		IF 50 = 1
;			TARGET_ADD_STATUS 1 @54 1088 1 300 53
;		ENDIF
;		IF 50 = 2
;			TARGET_ADD_STATUS 1 @54 1088 2 300 53
;		ENDIF
;	endif


endproc
