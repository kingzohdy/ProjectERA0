;��Ⱦ��

; 5301021	��˿
; 5301022	��Ⱦ��
; 5301023	��Ⱦ��
; 5301024	��Ⱦ��
; 5301025	��˿
; 5301026	��˿
; 5301027	��˿
; 5301028	��˿
; 5301029	��˿
; 5301030	��˿

proc 1
	strvar ret ret_1 ret_2 ret_3 ret_4
	GET_PACK_SPACE 1 ret
	GET_ITEM_NUM_INPAK 5301021 1 ret_1
	GET_ITEM_NUM_INPAK 5300162 1 ret_2
	GET_ITEM_NUM_INPAK 5301026 1 ret_3
	GET_ITEM_NUM_INPAK 5301027 1 ret_4
	
	if @ret >= 1
		if @ret_2 >= 1
			if @ret_1 >= 1
				DEC_ITEM_NUM_INPAK 5301021 1 0
				DEC_ITEM_NUM_INPAK 5300162 1 1
				if 0 = 0 and 1 = 0
					ADD_ITEM_NUM_INPAK 5301025 1 1 0
					
					add_sys_msg_start
					add_sys_msg �ɹ�Ⱦ�ƺ�˿��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif @ret_4 < 1 and @ret_3 < 1
					add_sys_msg_start
					add_sys_msg ȱ�ٰ�˿���ϣ���ʱ�޷�Ⱦ�ƺ�˿
					set_sys_msg_mode 3 0 0
					msg_info
			elseif @ret_4 >= 1
				DEC_ITEM_NUM_INPAK 5300162 1 2
				DEC_ITEM_NUM_INPAK 5301027 1 3
				if 2 = 0 and 3 = 0
					ADD_ITEM_NUM_INPAK 5301030 1 1 0
					
					add_sys_msg_start
					add_sys_msg �ɹ�Ⱦ�Ƴ�˿��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			elseif @ret_3 < 1
					add_sys_msg_start
					add_sys_msg ȱ�ٻ�˿���ϣ���ʱ�޷�Ⱦ�Ƴ�˿
					set_sys_msg_mode 3 0 0
					msg_info
			elseif @ret_3 >= 1
				DEC_ITEM_NUM_INPAK 5300162 1 4
				DEC_ITEM_NUM_INPAK 5301026 1 5
				if 4 = 0 and 5 = 0
					ADD_ITEM_NUM_INPAK 5301028 1 1 0
					
					add_sys_msg_start
					add_sys_msg �ɹ�Ⱦ����˿��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg ȱ����˿���ϣ���ʱ�޷�Ⱦ����˿
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg ȱ�ٺ�Ⱦ�ϣ���ʱ�޷�Ⱦ�Ʋ�ɫ˿��
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �����ռ䲻�㣡
		set_sys_msg_mode 1 0 0
		msg_info	
	endif
endproc 
