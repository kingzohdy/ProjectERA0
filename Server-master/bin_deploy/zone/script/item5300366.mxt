;������˹������ҩ��



proc 1
	get_player_mapinfo 0 1 2 3 4
	if 2 = 2300
;			effect 0 0 playEffct(6800,6001,"effect\\entity\\9544_16.ent")
			GET_MON_INFO 0 20 21 22 23 24 25 26
			dec_item_num_inpak 5300366 1 0
			add_item_num_inpak 5300345 1 1 0

			rand 100 5
			add 100 100
			switch 100
				case 100
					MON_SPEECH @21 11 1 4 �о��ö��ˣ�лл�㣬#Name#��
				endcase
				case 101
					MON_SPEECH @21 11 1 4 ��Ӧ�úܿ�ͻ῵���˰ɣ�
				endcase
				case 102
					MON_SPEECH @21 11 1 4 ��л�㣬#Name#����ʱ�˿̼�������Ψһһ��ֵ�ø��˵����ˡ�
				endcase
				
				case 103
					MON_SPEECH @21 11 1 4 �����鷳���ˣ����ҩ������Ĵ��������������������ٶ�������
				endcase
				
				case 104
					MON_SPEECH @21 11 1 4 �ܾ�û�м����������������ĵ��������ˣ���ϣ�������������ѡ�
				endcase
			endswitch
			del_mon 10			
			
	  else
			add_sys_msg_start
			add_sys_msg Ŀ�겻�ڱ���ͼ�У�
			set_sys_msg_mode 4 0 0
			msg_info
	  endif

endproc


