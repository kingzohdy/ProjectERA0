
proc 2
	ADD_SYS_MSG_START
	rand 20 100
	; �û���ʵ
	if @20 >= 0 and @20 <= 59
		ADD_SYS_MSG �������ı��䣬��ûû���ʵ
		ADD_BIND_ITEM_NUM_INPAK 5233000 1 1 1
	; �����ʵ
	elseif @20 >= 60 and @20 <= 79
		ADD_SYS_MSG �������ı��䣬��û����ʵ
		ADD_BIND_ITEM_NUM_INPAK 5233500 1 1 1
	; ������ʵ
	else
		ADD_SYS_MSG �������ı��䣬���������ʵ
		ADD_BIND_ITEM_NUM_INPAK 5219600 1 1 1
	endif 
	SET_SYS_MSG_MODE 4 0 0
	MAP_MSG_INFO
	;DESTROY_MON 1
	bct_ui 0 0
endproc 

proc 6
	GET_PACK_SPACE 0 10
	if @10 <= 0
		ADD_SYS_MSG_START
		ADD_SYS_MSG �����޷����ɸ�����Ʒ����������
		SET_SYS_MSG_MODE 4 0 0
		MAP_MSG_INFO
		PLAYER_COLLECT_BREAK
	endif
endproc 