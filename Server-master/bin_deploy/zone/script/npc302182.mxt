;��˹��

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bHey��#Name#����������ʥ�ǹ���ȯ���������кö������Ժ��㻻Ŷ�����ж�������ȯ���ҿ��Ը��㻻���ٸ���Ʒ��
	add_talk #b#bֻ������ʥ�ڲ���������ᣬ�úð��հɡ�
	add_talk #b#b#c186506��ʥ�ǹ���ȯ��ͨ��ʱ���-�����������ã�ÿ�����10�Ρ�#n

	
		OPTION 200 һ����ȯ�һ�һ����Ʒ

	talk
endproc

proc 200
	STRVAR ret num ret1 kg
	
	GET_ITEM_NUM_INPAK 5012139 ret num
	GET_PACK_SPACE 0 kg
	
	if kg > 0
		if ret = 0 and num > 0
			DEC_ITEM_NUM_INPAK 5012139 1 ret1
			if ret1 = 0
				rand 10 100
					if 10 < 3
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3050041 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1�����������ۡ�ʱװ
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 3 and 10 < 6
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3450041 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1�����������ӡ�ʱװ
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 6 and 10 < 9
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3750041 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1����������ѥ��ʱװ
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 9 and 10 < 12
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3850041 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1�����������֡�ʱװ
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 12 and 10 < 15
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 3650002 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1������Ȼ�������Ρ�
						set_sys_msg_mode 3 0 0
						msg_info
					endif
					if 10 >= 15
						ADD_BIND_ITEM_NUM_INPAK_NOLIMIT 5012136 1 1 0
						add_sys_msg_start
						add_sys_msg ������ʥ�ǹ���ȯ�ɹ���ȡ��1�š���ʥ���ǹ������ߡ�
						set_sys_msg_mode 3 0 0
						msg_info
					endif
			endif
		else
			add_sys_msg_start
			add_sys_msg �����1������ʥ�ǹ���ȯ����û��Ŷ���ҿɲ����������⡣
			set_sys_msg_mode 3 0 0
			msg_info
		endif
	else
		add_sys_msg_start
		add_sys_msg ��ճ�1������������������Ŷ��
		set_sys_msg_mode 3 0 0
		msg_info
	endif
endproc
