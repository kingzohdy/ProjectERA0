;�����ػ���Ŧ�    ,����������̵�
proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�ҽ�$_npc$���ҿ����ṩһЩ�򵥵Ļ�е���߸��㣬��Ȼ����ҪǮ�ģ���Ҳ�ܰ�����������ޣ�����Ҫ�շѵģ���������
	option 300 #���������
	option 400 #�����е����
	talk
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bϣ��������Ŧ������������ʻ�Ļ�е����ǿ���������ṩ��������������;����
	add_talk #b#b���������ˣ����ģ�����Ҳ���԰�������
	option 301 ����ͼ������ޣ����ã�200����
	option 302 ����ǿ�������ޣ����ã�1000����
	option 303 ����ǿ�������ޣ����ã�5000����
	talk
endproc
proc 301
	choose 2 311 312 ����ͼ������޷���200������ȷ������
endproc
proc 302
	choose 2 321 322 ����ǿ�������޷���1000������ȷ������
endproc
proc 303
	choose 2 331 332 ����ǿ�������޷���5000������ȷ������
endproc


proc 311
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 200 and @ret >= 200
		choose 1 313 312 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 200
		choose 1 313 312 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 313
	endif
endproc 


proc 313
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300881 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 200 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG ���Ҳ��㣬����ʧ��!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300881 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300881 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 200 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG ��е�����״̬����������
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
                ADD_SYS_MSG_START
                ADD_SYS_MSG ����ʧ�ܣ���������û����Ҫ����ĵͼ������ޡ�
                SET_SYS_MSG_MODE 3 0 0
                MSG_INFO
        endif	
endproc
proc 312
endproc

proc 321
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 1000 and @ret >= 1000
		choose 1 323 312 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 1000
		choose 1 323 312 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 323
	endif
endproc 
proc 323
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300882 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 1000 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG ���Ҳ��㣬����ʧ��!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300882 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300882 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 1000 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG ��е�����״̬����������
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
			ADD_SYS_MSG_START
			ADD_SYS_MSG ����ʧ�ܣ���������û����Ҫ�����ǿ�������ޡ�
			SET_SYS_MSG_MODE 3 0 0
			MSG_INFO
        endif	
endproc
proc 322
endproc

proc 331
	strvar ret
	get_id_player_info 0 21 ret
	get_id_player_info 0 22 30
	if 30 < 5000 and @ret >= 5000
		choose 1 333 312 ���������Ѵﵽÿ��������ޣ�����������ֱ���������ң��Ƿ����������
	elseif ret < 5000
		choose 1 333 312 �������㣬����������ֱ���������ң��Ƿ����������
	else
		call 333
	endif
endproc 
proc 333
        strvar Num Retm
        GET_ITEM_NUM_INPAK 5300883 1 Num
        if Num > 0
                strvar retmid
                GET_ROLE_RIDE_ON_MACHINE 0 retmid
                if retmid = 0
                        DEC_MONEY_TEST 0 5000 21 Retm
                        if Retm = -1
                                ADD_SYS_MSG_START
                                ADD_SYS_MSG ���Ҳ��㣬����ʧ��!
                                SET_SYS_MSG_MODE 3 0 0
                                MSG_INFO
                        else
                                strvar Ret
                                DEC_ITEM_NUM_INPAK 5300883 1 Ret
                                if Ret = 0
                                        strvar Retitem
                                        add_item_num_inpak 5300883 1 1 Retitem
                                        if Retitem = 0
                                                DEC_MONEY 0 5000 21 10
                                        endif
                                endif
                        endif
                else
                        ADD_SYS_MSG_START
                        ADD_SYS_MSG ��е�����״̬����������
                        SET_SYS_MSG_MODE 3 0 0
                        MSG_INFO
                endif
        else
                ADD_SYS_MSG_START
                ADD_SYS_MSG ����ʧ�ܣ���������û����Ҫ�����ǿ�������ޡ�
                SET_SYS_MSG_MODE 3 0 0
                MSG_INFO
        endif	
endproc
proc 332
endproc
proc 400
	open_npc_shop
endproc
