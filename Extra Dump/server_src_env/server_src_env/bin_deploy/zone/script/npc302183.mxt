;����

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b����Ͱ�˹���һ���Ʒ�����ɲ������ҵ��Ϳ����ˣ��������Ҹ���������ҲŻ������Ժ����һ�����ġ���ʥ�ǹ���ȯ����
	add_talk #b#bÿ�����10�Σ�ÿ����Ҫ5����������Ƽ��������ϣ��ǳ��ﳬ��ֵ�أ�
	add_talk #c18650��ʯ��ԭ�ʿ���Ӣ�����ܿ�
	add_talk ���ϣ�ԭ���͡��͹��͡������
	add_talk �������ؾ������������������䴨����#n
	add_talk #c0181f9��˹��ÿ��08:00-24:00 ÿ10���������ĳЩ��ͼ�г��֣�����ͨ��ʱ���-����������ѯ��˹����������λ�á�
	
	task_stat 4640 0 101
	if 101 = 2
		OPTION 300 #�Ͻ�5���Ƽ��������
	endif

	TASK_ADD_OPTION 4640 0 200
	
	talk
endproc

PROC 200
	task_do_option 4640 0
ENDPROC

PROC 300
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�����������������ʲô��
	add_talk #b#b
	add_talk #c186506�ռ�5����������ĿƼ���������
	add_talk ��ʯ��ԭ�ʿ���Ӣ�����ܿ�
	add_talk ���ϣ�ԭ���͡��͹��͡������
	add_talk �������ؾ������������������䴨����
	
		NPC_COMMIT_OPT 301
	
	talk
ENDPROC

proc 301

	strvar ID Pos_1 Pos_2 ret
	GET_COMMIT_ITEM_INFO 1 ID
	GET_COMMIT_ITEM_INFO 2 Pos_1
	GET_COMMIT_ITEM_INFO 3 Pos_2
	
	if @ID = -1
		add_sys_msg_start
		add_sys_msg �㻹û���ύ��Ʒ��
		set_sys_msg_mode 1 0 0
		msg_info		
		return 
	endif

	task_stat 4640 0 100
	if 100 = 2
		if @ID = 5101001 or @ID = 5101002 or @ID = 5101003 or @ID = 5102001 or @ID = 5102002 or @ID = 5102003 or @ID = 5103001 or @ID = 5103002 or @ID = 5103003
			switch @ID
				case 5101001
					DEC_ITEM_NUM_BYIDX 5101001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5101002
					DEC_ITEM_NUM_BYIDX 5101002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase	
				case 5101003
					DEC_ITEM_NUM_BYIDX 5101003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102001
					DEC_ITEM_NUM_BYIDX 5102001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102002
					DEC_ITEM_NUM_BYIDX 5102002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5102003
					DEC_ITEM_NUM_BYIDX 5102003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103001
					DEC_ITEM_NUM_BYIDX 5103001 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103002
					DEC_ITEM_NUM_BYIDX 5103002 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
				case 5103003
					DEC_ITEM_NUM_BYIDX 5103003 5 @Pos_1 @Pos_2 ret
					if @ret = -1
						call 303
					else
						if @ret = 0
							task_fini 4640 0
							call 302
						ENDIF
					ENDIF
				endcase
			endswitch
		else
			add_sys_msg_start
			add_sys_msg ��Ʒ��������Ҫ��δ�ܳɹ��Ͻ�
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	endif
endproc
	
proc 302
	add_sys_msg_start
	add_sys_msg �ɹ��Ͻ���Ʒ���������
	set_sys_msg_mode 3 0 0
	msg_info
	PLAYER_CLOSE_DIALOG 0 0
endproc

proc 303
	add_sys_msg_start
	add_sys_msg ��Ʒ����Ҫ��������δ�ܳɹ��Ͻ�
	set_sys_msg_mode 3 0 0
	msg_info
endproc
