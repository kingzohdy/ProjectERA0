;�ڰ�ħ��ʦ	306014


proc 1
	dialog 1

	ADD_TALK $_npc$��
	add_talk ��Ϊ�ҵ�ū�Ͱɡ�
	GET_PWORLD_VAR  16 32 1 13
	If 13 = 57
	option 16 #��ʼս��!
	option 15 #����׼��׼����
	endif
	talk
endproc


proc 16

	GET_PWORLD_VAR  16 32 1 13
	If 13 = 57
	ADD 13 1
	set_pworld_var 16 32 @13 1

	GET_PWORLD_VAR  4 32 1 11
	ADD 11 1
	set_pworld_var 4 32 @11 1

		NEW_MON 284009 1 18642 4300 300 0 0 1 2		;�質����
		NEW_MON 284009 1 19648 4250 300 0 0 1 2		
		NEW_MON 284009 1 19848 3250 300 0 0 1 2
		NEW_MON 284009 1 19848 3392 300 0 0 1 2		
		NEW_MON 284009 1 18548 3790 300 0 0 1 2

	Else return

	
	
	
	endif
endproc 

proc 15
	return
endproc
