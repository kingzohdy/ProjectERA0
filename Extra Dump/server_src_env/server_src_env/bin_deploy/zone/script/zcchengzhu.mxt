

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ҫ׷��ɹ���׷�����㣬�ɹ���׷�����㡣
	
	task_add_option 1098 1 104
	task_add_option 1109 1 105
	task_add_option 1110 1 106
	task_add_option 2009 1 113
	task_add_option 2015 1 116
	task_add_option 2022 1 117
	task_add_option 1111 1 118
	task_add_option 1112 1 119

;�޸�30~40
	task_add_option 2005 1 145
;	task_add_option 2018 1 146
;	task_add_option 2019 1 147
	task_add_option 2021 1 148 
	task_add_option 2014 1 149
;20~30����

;	task_add_option 1501 1 164	
;	task_add_option 1502 1 165	
;ͭ�Ĵ��޸�

	task_add_option 1301 1 166	
;�����޸�
	task_add_option 1082 1 167	
	task_add_option 3110 1 169	
;1~10�޸�
;	task_add_option 1077 1 170	

;;;;
	task_add_option 1801 1 171	
	task_add_option 1802 1 172	
	task_add_option 3030 1 173

	strvar Ret
	task_stat 1301 0 Ret
	if Ret = 2
		
		option 300 ʱ���̫���ˣ��ܷ�����һ�̣�
	endif
	
	strvar Ret1
	IS_WORLD_CITY_OWNER Ret1
	if Ret1 = 1
		option 250 #�����п����ĵ�ͼ
	endif
	

	option 500 ����ѯ�ʻ���Ԫ�ص��й�����
	
	task_stat 4446 1 20
	if 20 = 2
		option 600 �������£�����Ϊ�����Ǿ���������
	endif
    TALK               
endproc 

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bʱ����ԡ���Դ��Ϊ��Դ�����ڿ��У�ÿ�궼��Ҫ���Ĵ�����Ԫ��ʯΪ��Դע�����������ǣ�Ԫ��ʯ�Ĳ�����ʼ����Ӧ���ˡ�
	add_talk #b#b���о�����������Ԫ�������̺��Ŵ�����������ֻҪ��Ϊʱ��Ǵ�����Щ��Դ���Ϳ��Ի��������Ϊ������
	add_talk #b#b��л��ʱ���������һ�У�
	add_talk #b#b
	add_talk #b#b#c186506��ʾ������Ԫ�ػ���ÿ���11:00-16:00��18:00-23:00���������һ�Σ�ÿ�γ���8���ӡ�#n
	add_talk #b#b#c186506��ʾ������Ԫ�ػ��������������ѩԭ����Ŭ��˹����ʮ���ſ��е�ĳ���Ƚ�ƫƧ�ĵص㡣#n
	add_talk #b#b#c186506��ʾ�������ռ��������������Ŀ���ͨ�����˻��á�#n

		option 501 �Ͻ�1�����ص���������
		option 502 #�ر�
		
	talk
endproc 
proc 501
	strvar Rate
	var Rate 0
	strvar INum
	
	GET_VAILD_ITEM_NUM_INPAK 5300963 1 20
	if 20 > 0
		GET_PACK_SPACE 0 INum
		if INum > 0
			dec_item_num_inpak 5300963 1 30
			if 30 = 0
				Rand Rate 2
				if Rate = 1
					ADD_BIND_ITEM_NUM_INPAK 5052872 1 1 0
				else
					ADD_BIND_ITEM_NUM_INPAK 5052871 1 1 0
				endif
			endif
		else
			DIALOG 1
			add_talk $_npc$:
			add_talk #b#b���꣬��ı������ˣ�ȥ����һ�±���������ȡ�����ɡ�
			add_talk #b#b
			add_talk #b#b#c186506��ʾ��Ҫ��ȡ����������������Ҫ��һ�����еĸ��ӡ�#n
			talk
		endif	
	else
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��ʹ�����������ڻ���Ԫ�ش��ռ��������������Ұɡ�	
		talk
	endif
	
endproc 
proc 600
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#bԭ���Ǵ���������$_player$������Ը���տ��׶�԰�Ĺ¶���̫��л���ˣ��ҵ����ѣ�	
	add_talk #b#b�����ո�һ�յ�����С���׶�԰�̽�ա������볤���Ժ��ܱ���㹻ǿ׳��ȥ��������������е�������ȥ������Щ�����ɸ�ĸ�������ǰ���ķ羰ʤ�ء��������չ�ʱ����׶�԰��С���ѣ�ʵ�����ٺò����ˣ�

		option 601 #��л�������Ҷ����߾�ȫ���չ˺ú����ǣ�
	talk
endproc 
proc 601
	task_fini 4446 1	
endproc
PROC 104
	task_do_option 1098 1
ENDPROC
PROC 105
	task_do_option 1109 1
ENDPROC
PROC 106
	task_do_option 1110 1
ENDPROC
PROC 107
	task_do_option 4100 1
ENDPROC

PROC 113
	task_do_option 2009 1
ENDPROC

PROC 116
	task_do_option 2015 1
ENDPROC
PROC 117
	task_do_option 2022 1
ENDPROC
PROC 118
	task_do_option 1111 1
ENDPROC
PROC 119
	task_do_option 1112 1
ENDPROC


PROC 145
	task_do_option 2005 1
ENDPROC
PROC 146
	task_do_option 2018 1
ENDPROC
PROC 147 
	task_do_option 2019 1
ENDPROC
PROC 148
	task_do_option 2021 1
ENDPROC
PROC 149
	task_do_option 2014 1
ENDPROC

PROC 157
	task_do_option 4104 1
ENDPROC
PROC 158
	task_do_option 4119 1
ENDPROC

;PROC 164
;	task_do_option 1501 1
;ENDPROC

;PROC 165
;	task_do_option 1502 1
;ENDPROC
PROC 166
	task_do_option 1301 1
ENDPROC
PROC 167
	task_do_option 1082 1
ENDPROC
PROC 169
	task_do_option 3110 1
ENDPROC
;PROC 170
;	task_do_option 1077 1
;ENDPROC
PROC 171
	task_do_option 1801 1
ENDPROC
PROC 172
	task_do_option 1802 1
ENDPROC
PROC 173
	task_do_option 3030 1
ENDPROC
proc 250
	strvar Ret2
	IS_WORLD_CITY_OWNER Ret2
	if Ret2 = 0
		add_sys_msg_start
		add_sys_msg ���Ѿ����ǹ����Ա�ˣ����ܽ���˵�ͼ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif

	strvar Rad
	rand Rad 6
	switch Rad
		
		case 0
			TRANSMIT 4404 19700 21100 0
		endcase
		case 1
			TRANSMIT 4404 19700 20600 0
		endcase
		case 2
			TRANSMIT 4404 19500 20500 0
		endcase
		case 3
			TRANSMIT 4404 19300 20700 0
		endcase
		case 4
			TRANSMIT 4404 19300 20700 0
		endcase
		case 5
			TRANSMIT 4404 20100 21000 0
		endcase
	endswitch
endproc 


proc 2
	get_trig_task 1 2

	if 2 = 1098
		task_fini 1098 1
	endif
	if 2 = 1110
		task_fini 1110 1
	endif
	if 2 = 1111
		task_fini 1111 1
	endif 

	if 2 = 2009
		task_fini 2009 1
	endif
	if 2 = 2012
		task_fini 2012 1
	endif
	if 2 = 2015
		task_fini 2015 1
	endif
	if 2 = 2022
		task_fini 2022 1
	endif
	if 2 = 2025
		task_fini 2025 1
	endif
	if 2 = 2030
		task_fini 2030 1
	endif

	if 2 = 2123
		task_fini 2123 1
	endif

	if 2 = 2137
		task_fini 2137 1
	endif
;	if 2 = 3110
;		task_fini 3110 1
;	endif

;	if 2 = 1502
;		task_fini 1502 1
;	endif
	if 2 = 1301
		DIALOG 1
		add_talk $_npc$:
		add_talk #b#b��Ҫ׷��ɹ���׷�����㣬�ɹ���׷�����㡣
		
			task_fini 1301 1
			option 300 ʱ���̫���ˣ��ܷ�����һ�̣�
		
		talk
	endif
	if 2 = 3101
		task_fini 3101 1
	endif
;	if 2 = 3000
;		task_fini 3000 1
;	endif
	if 2 = 1802
		task_fini 1802 1
	endif
endproc
proc 300
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ���ԡ�
	add_talk #b#b����������ֱ�ӽ����͵�Ŀ�ĵأ���;����ͣ��Ӵ��
	add_talk #b#b��ȷ��Ҫ����ֱ��೵��
		option 301 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 302 #�����������Լ���ȥ�����ܹ��ʱ��ǣ�
	talk

endproc 
proc 301
;	NEW_OWNER_TMP_ITEM 1460073 26500 38700 0 0 0 313 1095 1 3600 1
	AUTO_MOVE 20 2100 4200 3900 1460073 0 3081 1 3600

endproc 
proc 302
	return
endproc 




