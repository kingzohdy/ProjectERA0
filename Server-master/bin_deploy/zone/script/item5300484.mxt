;ԴѪ֮�� ɾ�� ˢ�Ѻù�


Proc 1
	STRVAR id1 id2 x y hp a

		GET_MON_INFO 1 1 id2 0 0 0 x y
		GET_MON_HP_PERCENT @id2 hp
		if hp < 40
			DESTROY_MON 1
				if 1 = -1
					return
				endif
				NEW_LIFE_MON 317012 1 @x @y 50 0 0 0 6 1 id1
				if 1 = -1
					return
				else
					RAND a 2
					switch a
						case 0
							MON_SPEECH @id1 1 0 1 �о���������һ���������Ρ���
						endcase	
						case 1
							MON_SPEECH @id1 1 0 1 ����˭����������ô�ˣ�
						endcase
						case 2
							MON_SPEECH @id1 1 0 1 ���۰����Դ��տյģ����������أ�
						endcase	
					endswitch
					ADD_ITEM_NUM_INPAK 5300470 1 0 1
					if 1 = -1
						return
					ENDIF
				endif
		else
			add_sys_msg_start
			add_sys_msg ��ѽ���ڻ��ò��ˣ����ٶ�������£�
			set_sys_msg_mode 1 0 0
			MSG_INFO			
		endif
endproc							