; ��ʧ�Ľ���

proc 1

	strvar num1 num2 num3
	GET_ITEM_NUM_INPAK 5062201 0 num1
	GET_ITEM_NUM_INPAK 5062202 0 num2
	GET_ITEM_NUM_INPAK 5062203 0 num3
	
	if num1 > 0
		if num2 > 0
			if num3 > 0
				strvar ret1 ret2 ret3
				DEC_ITEM_NUM_INPAK 5062201 1 ret1
				DEC_ITEM_NUM_INPAK 5062202 1 ret2
				DEC_ITEM_NUM_INPAK 5062203 1 ret3
				if ret1 = 0
					if ret2 = 0
						if ret3 = 0
							ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5062200 1 1 0
							
							add_sys_msg_start
							add_sys_msg ��ʹ����ʧ�Ľ��н����ͽ��ʣ��ϳ��ˡ���ʧ�ı�����
							set_sys_msg_mode 3 0 0
							msg_info
						else
							add_sys_msg_start
							add_sys_msg ����ʧ�Ľ��ʡ������ˣ��ϳɡ���ʧ�ı�����ʧ��
							set_sys_msg_mode 3 0 0
							msg_info
						endif
					else
						add_sys_msg_start
						add_sys_msg ����ʧ�Ľ����������ˣ��ϳɡ���ʧ�ı�����ʧ��
						set_sys_msg_mode 3 0 0
						msg_info
					endif
				else
					add_sys_msg_start
					add_sys_msg ����ʧ�Ľ��С������ˣ��ϳɡ���ʧ�ı�����ʧ��
					set_sys_msg_mode 3 0 0
					msg_info
				endif
			else
				add_sys_msg_start
				add_sys_msg �������Ҳ�������ʧ�Ľ��ʡ����޷��ϳɡ���ʧ�ı�����
				set_sys_msg_mode 3 0 0
				msg_info
			endif
		else
			add_sys_msg_start
			add_sys_msg �������Ҳ�������ʧ�Ľ��������޷��ϳɡ���ʧ�ı�����
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg �������Ҳ�������ʧ�Ľ��С����޷��ϳɡ���ʧ�ı�����
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
