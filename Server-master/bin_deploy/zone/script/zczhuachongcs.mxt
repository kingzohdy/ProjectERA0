;ץ���ͼ�ʺ絺����Ա 317005


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bһ�������ĳ���������֡��������¸����°빦������ʹ��������Ҳ�Ƿ���֮������
	task_add_option 3001 1 102	
	
	task_add_option 3002 1 103	
	task_add_option 3007 1 104
	task_add_option 3008 1 105
	task_add_option 3009 1 106
	
	task_add_option 3000 1 107
;	task_add_option 1502 1 130
	
	task_add_option 3010 1 121
	task_add_option 3011 1 108
	task_add_option 3012 1 109
	task_add_option 3013 1 120
	task_add_option 3014 1 122
	task_add_option 3015 1 123
	task_add_option 3004 1 124
	
	task_add_option 1082 1 125
	task_add_option 1351 1 126
	task_add_option 1084 1 127
	
	task_add_option 3016 1 128
	task_add_option 3017 1 129

	strvar Ret
	task_stat 1082 1 Ret
	GET_ID_PLAYER_INFO 0 8 lv
	if Ret = 2
		
;		option 500 ʱ���̫���ˣ��ܷ�����һ�̣�
	endif
	
	
	
	task_stat 3001 1 21
	if 21 = 2
		option 301 ��ʼ����
	endif
	task_stat 3014 1 22
	if 22 = 2
		option 201 ��ʼ����
	endif
	task_stat 3015 1 23
	if 23 = 2
		option 401 ��ʼ����
	endif
	
	task_stat 3016 1 24
	task_stat 3017 1 25
	if 24 = 2
		option 430 #����ת��-�ύ���ﵰ
	endif
	if 25 = 2
		option 440 #��������-�ύ���ﵰ
	endif
	
	
	OPTION 800 #(����)�򿪳����̵�
	if 24 = 3
		OPTION 600 #����ת��  ����ת�Ƴ����ǿ���Ǽ���
	endif
	if 25 = 3
		OPTION 601 #��������  �������ճ���ľ��顢�û����ʣ�
	endif
	
	if lv >= 70
		option 610 #�������  ���ɽ���Ϊǿ���ս�裩
	endif
	
	if lv >= 50
		option 611 #�������������������ɳ�Ǳ����
	endif
	
	strvar lv
	task_stat 3003 0 50
	; GET_ID_PLAYER_INFO 0 8 lv (�Ƶ���1��strvar���壩
	if 50 = 3 
	;	OPTION 110 #ȥ�ʺ絺ץ��������
	endif
	
	TASK_STAT 3002 0 60
;	TASK_STAT 3008 0 61
	TASK_STAT 3007 0 62
		
	if 60 = 1 or 60 = 3

		if 62 != 1 
			if 62 != 3 
				option 320 #ȥ�ʺ��������ҿ���
			endif
		endif

	endif
	talk

endproc
proc 600
	open_ui 81
endproc
proc 601
	open_ui 82
endproc
proc 610
	open_ui 83
endproc
proc 611
	open_ui 90
endproc
proc 430
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ܸ�����Ը������ң����������ɹ����ҵ����ң���ֻ��Ҫ������Ŀ�������һ�ų��ﵰ���ɣ�Ʒ�ֲ��ޣ�
	add_talk #b#b
	add_talk ��ó��ﵰ�ķ�ʽ�кܶ��֣�
	add_talk #b#b��ʡǮ�ķ�ʽ��#c186506ȥ�ʺ絺��׽#n
	add_talk #b#b��ʡ���ķ�ʽ��#c186506���˺������׿�����#n
	add_talk #b#b��ʡ�ĵķ�ʽ��#c186506��̯λ�Ϲ���#n
	add_talk #b#b#c6d00d2
	add_talk #b#b
;	add_talk #b#b�����ﵰ�����·��Ŀ���

		NPC_COMMIT_OPT 431
	talk
endproc
proc 431
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 5 50
	;�Ƿ��ǳ��ﵰ
	if 50 = -1
		add_sys_msg_start
		add_sys_msg ���ύ�ĺ����ǳ��ﵰ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		task_stat 3016 1 24
		if 24 = 2
			task_fini 3016 1
		endif
	endif
	PLAYER_CLOSE_DIALOG 0 0

endproc 
proc 440
	dialog 3
	add_talk $_npc$:
	add_talk #b#b�ܸ�����Ը������ң����������ɹ����ҵ����ң���ֻ��Ҫ������Ŀ�������һ�ų��ﵰ���ɣ�Ʒ�ֲ��ޣ�
	add_talk #b#b
	add_talk ��ó��ﵰ�ķ�ʽ�кܶ��֣�
	add_talk #b#b��ʡǮ�ķ�ʽ��#c186506ȥ�ʺ絺��׽#n
	add_talk #b#b��ʡ���ķ�ʽ��#c186506���˺������׿�����#n
	add_talk #b#b��ʡ�ĵķ�ʽ��#c186506��̯λ�Ϲ���#n
	add_talk #b#b#c6d00d2
	add_talk #b#b
;	add_talk #b#b�����ﵰ�����·��Ŀ���

		NPC_COMMIT_OPT 441
	talk
endproc
proc 441
	strvar itemId pos index ret flag
	GET_COMMIT_ITEM_INFO 1 itemId
	if itemId = -1
		add_sys_msg_start
		add_sys_msg ����û���ύ��Ʒ
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	
	GET_COMMIT_ITEM_INFO 5 50
	;�Ƿ��ǳ��ﵰ
	if 50 = -1
		add_sys_msg_start
		add_sys_msg ���ύ�ĺ����ǳ��ﵰ��
		set_sys_msg_mode 4 0 0
		msg_info
		return
	endif
	GET_COMMIT_ITEM_INFO 2 pos
	GET_COMMIT_ITEM_INFO 3 index
	DEC_ITEM_NUM_BYIDX @itemId 1 @pos @index ret
	if ret = 0
		task_stat 3017 1 25

		if 25 = 2
			task_fini 3017 1
		endif
	
	endif
	PLAYER_CLOSE_DIALOG 0 0

endproc 


PROC 800
	
	TASK_STAT 3009 1 22
	if 22 = 2
		task_fini 3009 1
	endif

	OPEN_NPC_SHOP
ENDPROC


proc 301
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��������ϣ��ܹ�����ͬ�ʹ��ࡢ�������桢���벻�����������ء����������Я��ͬ�ε���__��
	add_talk #b#b<��ѡ����ȷ�Ĵ�>
		option 303  ѡ��A������
		option 304  ѡ��B�����泬��
		option 304  ѡ��C����͹��
		option 304  ѡ��D����������
	talk
endproc 
proc 302
	return
endproc 
proc 303
	dialog 1
	add_talk $_npc$:
	add_talk  #b#b���������Ը��Ϊ��������������߲嵶�����������ʹ����__��
	add_talk #b#b<��ѡ����ȷ�Ĵ�>
		option 304  ѡ��A�����泬��
		option 304  ѡ��B����͹��
		option 312  #ѡ��C������
		option 304  ѡ��D����������
		
	talk
endproc

proc 304
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�ף���ɲ��ǲ�ѧ����Ӧ��ѡ�Ĵ�Ӵ���ú����������»ش�ɡ�
		option 301 ���¿�ʼ
		option 302 #����
		
	talk
endproc

proc 312
	TASK_FINI 3001 0
endproc

proc 102
	task_do_option 3001 1
endproc
proc 103
	task_do_option 3002 1
endproc
proc 104
	task_do_option 3007 1
endproc
proc 105
	task_do_option 3008 1
endproc
proc 106
	task_do_option 3009 1
endproc
proc 107
	task_do_option 3000 1
endproc
proc 108
	task_do_option 3011 1
endproc
proc 109
	task_do_option 3012 1
endproc

proc 120
	task_do_option 3013 1
endproc 
proc 121
	task_do_option 3010 1
endproc 
proc 122
	task_do_option 3014 1
endproc 
proc 123
	task_do_option 3015 1
endproc 
proc 124
	task_do_option 3004 1
endproc 
proc 125
	task_do_option 1082 1
endproc  
proc 126
	task_do_option 1351 1
endproc  
proc 127
	task_do_option 1084 1
endproc  
proc 128
	task_do_option 3016 1
endproc  
proc 129
	task_do_option 3017 1
endproc 

proc 110

	get_player_base_att 1 2 3 4
	if 3 >= 20
		transmit 4400 23725 27202 1
	else
		add_sys_msg_start
		add_sys_msg ʵ�ڱ�Ǹ,���ĵȼ�δ�ﵽ20��,��������ʱ��������...
		set_sys_msg_mode 4 0 0
		msg_info
	endif
endproc


proc 320

		ENTER_PWORLD 202 23725 27202 1
endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b��Ȼ���ԣ������Ǹ��������ء�
	add_talk #b#b����������ֱ�ӽ����͵�Ŀ�ĵأ���;����ͣ��Ӵ��
	add_talk #b#b��ȷ��Ҫ����ֱ��೵��
		option 501 #��ֱ�ӽ����͵�Ŀ�ĵذɣ�
		option 502 #�����������Լ���ȥ�����ܹ��ʱ��ǣ�
	talk
endproc
proc 501
	
;	NEW_OWNER_TMP_ITEM 1460072 29400 26600 0 0 0 312 1095 1 3600 1
	AUTO_MOVE 0 1000 26500 38800 1460072 0 3081 1 3600
endproc 
proc 502
	return
endproc 

proc 2
	get_trig_task 1 2
	if 2 = 1084
		task_fini 1084 1
	endif

	if 2 = 1082
	;	DIALOG 1
	;	add_talk $_npc$:
	;	add_talk #b#b�������и��־���Ļ�Ŷ�������ϵȵ�Ʒ�֣�
			task_fini 1082 1
	;		option 500 ʱ���̫���ˣ��ܷ�����һ�̣�
	;	talk
	endif
	
	if 2 = 3002
		TASK_FINI 3002 0
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����Ҫ����ȥ����������׼��������
		
			option 320 #ȥ�ҿ���
			option 321 #�Ժ���ȥ
		talk
	endif
	if 2 = 3001
		dialog 1
		add_talk $_npc$:
		add_talk #b#b����Ҫ�����ˣ���׼��������
		
			option 301 ��ʼ����
		talk
	endif
	if 2 = 3014
		call 201
	endif
	if 2 = 3015
		call 401
	endif	

	if 2 = 3016
	
	endif
	if 2 = 3017
	
	endif
	

endproc 
proc 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����������ʲô�ã�<��ѡ����ȷ�Ĵ�>

		option 290 A���ÿ�
		option 204 B������Խ�ߵĳ���ս����Խ��
		option 290 C������Խ�ߵĳ���ս����Խ��
		option 290 D�������	
	talk
endproc 
proc 290
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506����Խ�ߵĳ���ս����Խ��#n
	add_talk #b#b#b
	add_talk #b#b#c0181f9����Ӱ�����ս�������������Գɳ�������ʱ��ǵġ����¶ࡿ��ѡ�񡾳���û���ѡ����ġ��û���ʵ�����ı��������ʡ�#n

		option 201 ���»ش����
	talk
endproc

;proc 202
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b���������ǳ����ںϵ�������
;	add_talk #b#b<��ѡ����ȷ�Ĵ�>
;
;		option 291 A����10�������㡢�ܳ����������ʣ�Ʒ����ͬ
;		option 291 B����50�������㡢�ܳ����������ʣ�Ʒ�ֲ�ͬ
;		option 291 C����40�������ʲ��ޣ�Ʒ�֣�����
;		option 203 D����50�������㡢�ܳ����������ʣ�Ʒ����ͬ
;
;	talk
;endproc 
;proc 291
;	dialog 1
;	add_talk $_npc$:
;	add_talk �ش����
;	add_talk #b#b
;	add_talk ��ȷ���ǣ�
;	add_talk #b#b#c186506�ȼ���50�������ʣ����㡢�ܳ���������Ʒ�֣���ͬ#n
;	add_talk #b#b#b
;	add_talk #b#b#c0181f9�ڴﵽ�ں�������ǰ���£�ѡ����Ҫ��֮�ںϵĳ������Ļ�м��Ŀ��ͷ�����һ���������ں�ѡ��#n
;
;		option 202 ���»ش����
;
;	talk
;endproc
;;;;;;;;;;;;;;;;;;;;;;;;----------------------------------------------------------
proc  202
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������������ʲô�ã�
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 292 A������
		option 292 B����߳��﹥��
		option 204 C�������˳����˺�
		option 292 D�������

	talk
endproc 
proc 292
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506�����˳����˺�#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�������ʵĳ����ǣУ˱ر�����ͬ�ĳ���Ʒ�֣������ʲ�����ͬ�������ʲ������ų���ĵȼ������ʸı䣬����ͨ�����＼��������߳����������#n

		option 202 ���»ش����

	talk
endproc 

;;;;;;;;;;;;;;;;;;;;;;;;----------------------------------------------------------
proc 204
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������츳������ο�����
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 205 A����������ʱ�м��ʻ��
		option 293 B���ü�����ѧϰ
		option 293 C�������ͻ���
		option 293 D�������

	talk
endproc 
proc 293
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506��������ʱ�м��ʻ��#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9������츳�����������ģ��޷�ͨ������;����ã�ֻ���ڷ������ﵰʱ��Ȼ����
		option 204 ���»ش����

	talk
endproc 

proc 205
	dialog 1
	add_talk $_npc$:
	add_talk #b#b���＼����λ�ã�
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 296 A�����������������
		option 206 B������ʱ�����ͨ������������
		option 296 C����ȥ������԰ѧϰ
		option 296 D�������

	talk
endproc

proc 296
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506ͨ����������＼��������#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�㻹�����ڳ���ѵ��Ա-���¶�[ʱ���]�Գ��＼�ܽ������ã�ʹ���������ɵļ��ܣ������µļ��ܡ������������ʹ���ܵ��������ӻ���١�
	
		option 205 ���»ش����

	talk
endproc 

proc 206
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������������ܹ��ı��������ʣ�<��ѡ����ȷ�Ĵ�>

		option 295 A���������
		option 295 B������ǿ��
		option 212 C�������¶ദ���У�����û�
		option 295 D�������
	talk
endproc

proc 295
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506�����¶ദ���У�����û�#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�û��Ľ���������桿�Ż���Ч�����û�������ֽ���ʱ�����ԡ������û�����ȡ�������棬������ѡ�񶼲���ʹ��������Խ��͡�
		option 206 ���»ش����

	talk
endproc 
;proc 208
;	dialog 1
;	add_talk $_npc$:
;	add_talk #b#b����ǿ�������У�ʹ�á���Ԫ���񡱵��ߣ�������ǿ���������ǣ���δ�����µ�ǿ�����ʱ�ر�ǿ����壬�����õ����ǻ���ʧ��
;
;
;		option 212 A��������ʧ����ʱ���Խ���ǿ��
;		option 220 B������ʧ���ص�����û��
;		option 220 C�������
;	talk
;endproc
;proc 220
;	dialog 1
;	add_talk $_npc$:
;	add_talk �ش����
;	add_talk #b#b
;	add_talk ��ȷ���ǣ�
;	add_talk #b#b#c186506������ʧ����ʱ���Խ���ǿ��#n
;
;	add_talk #b#b#b
;	add_talk #b#b#c0181f9ʹ�á���Ԫ���������ǿ���������Ǻ�������Ǿͻ�һֱ���ã�ֱ�����ٴα���ǿ��������������ܵ�ʱ������ƣ�Ҳ������Ϊ�ر���ǿ�������ָ�����ǰ��ǿ�������#n
;
;		option 208 ���»ش����
;
;	talk
;endproc
proc 212
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�������������ʹ���ø�����ĳ��
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 297 A������
		option 213 B���μ��г��ｱ���Ļ
		option 297 C�����ϵ���Ը
		option 297 D�������
	talk
endproc
proc 213
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������Ǵ�����������ô��ͽ����ץ��ѱ��������ڻ��ͨ�������ͨ������ս�������а����鿴���˵ĳ��ϣ��ĳһ�������ڳ���ս�������а���ҳ����������֣�
		TASK_FINI 3014 1
		option 230 #лл�佱
	talk
endproc 
proc 230
	return
endproc

proc 297
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506�μ��г��ｱ���Ļ#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�����ڲ�׽�����⣬�ܶ������Ի�ó��ﵰ�����硾�����ȡ�-������������ɽ����-Ұ��������������������-�������̻������������������֡���ݼ� H ��#n

		option 212 ���»ش����

	talk
endproc 


;;;;;;;;;;;;;;;3015


proc 401
	dialog 1
	add_talk $_npc$:
	add_talk #b#b�����ҳ϶�̫�ͣ��ᵼ�³����޷���ս����ʲô�������Իָ�������ҳ϶ȣ�
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 421 A������
		option 403 B��ʹ�ÿ������ҳ϶ȵĵ��ߣ�ѱ���ǹ�
		option 421 C���ں�
		option 421 D�������
	talk
endproc

proc 421
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506ʹ�ÿ����ƣ�Ͷȵĵ��ߣ�ѱ�޵���#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�����ͨ���Զ�ι�����ó�����ҳ϶�ʼ�ձ������ܹ���ս��״̬�����岽�����£�#n
	add_talk #b#b#c0181f9��һ�����򿪳�����塾��ݼ� �ء�#n
	add_talk #b#b#c0181f9�ڶ�����������Զ�ι������ť#n
	add_talk #b#b#c0181f9���������������е�ѱ���ǹ��϶����Զ�ι�������е�3�����ֺ�Ŀ�ͼ����#n
	add_talk #b#b#c0181f9���Ĳ����޸ġ��ҳ϶ȵ��ڡ���������ָĳ� 80#n
	add_talk #b#b#c0181f9���岽���������ý���������ȷ������ť#n

		option 401 ���»ش����

	talk
endproc 
proc 403
	dialog 1
	add_talk $_npc$:
	add_talk #b#bȥ�Ķ���׽��������ĳ��
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 404 A�����������м��ʵ��䡰�ɲ�׽���ĳ���
		option 422 B�������ʱ��ѳ���������
		option 422 C���������̫����
		option 422 D�������
	talk
endproc

proc 422
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506���������м��ʵ��䡰�ɲ�׽���ĳ���#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9�ɲ�׽�ɳ���Ĺ���ͷ������ɫ�ġ��ɲ�׽������������Ѫ��Խ�ߣ���׽�ɹ���Խ�͡�#n

		option 403 ���»ش����

	talk
endproc 

proc 404
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��׽����ĵ�����ô��ã�
	add_talk #b#b<��ѡ����ȷ�Ĵ�>

		option 423 A��������Ĭ���Ҫץ�衱
		option 423 B���������
		option 405 #C����������̵��ﹺ��
		option 423 D�������
	talk
endproc
proc 405
	TASK_FINI 3015 1
endproc

proc 423
	dialog 1
	add_talk $_npc$:
	add_talk �ش����
	add_talk #b#b
	add_talk ��ȷ���ǣ�
	add_talk #b#b#c186506��������̵��ﹺ��#n

	add_talk #b#b#b
	add_talk #b#b#c0181f9ÿ�β�׽�����۳ɹ���񣬶�������һ�����ߡ�#n

		option 404 ���»ش����

	talk
endproc 













