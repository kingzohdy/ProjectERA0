;�þ�����Ա

proc 1
	DIALOG 1
	add_talk $_npc$:	
	add_talk #b#b�޾��Ļþ��ȴ�����ʿ�ǵ�̽��!
	add_talk #b#b
	add_talk ����������
	add_talk #b#b1���ӳ��ȼ���20��,��������������3�ˣ�
	add_talk #b#b2��������ȫ���Ա����ʱ���,����û�д��ڽ���,��̯,�д�,���ߵ�״̬��
	add_talk #b#b#c005ebeÿ��ǰ50�δ��㣬ɱ�ֿɶ�����������#n
	
		task_add_option 1502 1 150
		task_add_option 3138 1 151
		task_add_option 3139 1 152
	
	
	option 100 ����þ�
	option 200 ����þ�����
	option 300 ���͵��ӳ����ڻþ�
	option 600 ��ѯ�����������
	option 500 ������֪
;	option 400 �鿴BOSSˢ�����
	talk	
endproc

proc 150
	task_do_option 1502 1
endproc 
proc 151
	task_do_option 3138 1
endproc  
proc 152
	task_do_option 3139 1
endproc 

;����þ�
proc 100
	OPEN_UI 63
endproc

;����þ�����
proc 200
	unreal_team_info 0
endproc

;���͵��ӳ����ڻþ�
proc 300
	goto_unreal_captain_near 0
endproc

;�鿴BOSSˢ�����
proc 400
	unreal_watch_boss_info 0
endproc

proc 500
	DIALOG 1
	add_talk $_npc$:
	add_talk ������֪��
	add_talk #b#b1�����븱��,��������,ȷ��������3����Ա��������״̬,�紦������,������״̬,���޷����͵���һ�㣻
	add_talk #b#b2���ӻþ���10�㿪ʼ,ÿ������һ��СBOSS,�м��ʻ��ϡ�е�װ���ͱ���.
	option 1 ����
	talk
endproc
proc 2
	get_trig_task 1 2
	if 2 = 1502
		task_fini 1502 1
	endif
endproc 

proc 600
	DIALOG 1
	add_talk ���������
	add_talk 1. �þ�ÿ��#c186506ǰ50��#n���㣬ɱ�ֶ�����������
	add_talk 2. ÿ���ϻþ�1�㣬�������#c186506����1��#n
	add_talk 3. ����#c186506ʣ�������һ��#n�������ܵ����ܵĴ�����
	add_talk 4 #c186506ÿ��1#n��ˢ�±��ܵĴ������
	add_talk 5. ���������߿ɻ��ܵ�#c186506100��#n
	add_talk #b#b
	strvar ret cishu
	UNREAL_GET_EXP_LAYER ret cishu
	if ret = 0 and cishu = 0
		add_talk ����ʣ�ള�������#cF80B0B0#n��
	elseif ret = 0 and cishu > 0
		add_talk ����ʣ�ള�������#c005ebe$@cishu$#n��
	else
	endif
	add_talk #b#b
	add_talk #c005ebeע�⣺ÿ�ν�����뿪�þ���������1�δ������#n
	option 1 ����
	talk
endproc
