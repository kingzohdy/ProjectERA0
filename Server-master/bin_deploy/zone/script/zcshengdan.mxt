;;;;ʥ��NPC


proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#bฺǺǣ�һ��һ�ȵ�ʥ�����ֵ��ˣ���ҽ��տ��ְ���
	add_talk #b#b��������׼���˲������������ʥ���ǹ���ѩ��ʥ���̻���ʥ��ħ��Ӧ�о��У���Ȼ�ٲ���ʥ��ʱװ��ѱ¹������
	add_talk #b#b��Щ���ܴ�ʥ�������￪��Ŷ��ฺǺǺǣ�
		
	task_add_option 3121 15 100

;	task_add_option 3122 1 101
;	task_add_option 3123 1 102
;	task_add_option 3124 1 103
;	task_add_option 3125 1 104
;	task_add_option 3126 1 105
;	task_add_option 3127 1 106
;	task_add_option 3128 1 107
;	task_add_option 3129 1 108

	TASKCYCLE_ADD_OPTION 121 12 110
	
	option 200 #��ʥ�������͡�
	option 300 #�������ߵ����䡿
	

	talk
endproc
PROC 100
	task_do_option 3121 1
ENDPROC

PROC 110	
	TASKCYCLE_DO_OPTION 121 1
ENDPROC
proc 2
	get_trig_task 1 2
	strvar Mid
	GET_PLAYER_ID 1 Mid
	if 2 = 3121		
		
		ADD_BIND_ITEM_NUM_INPAK 5012105 5 0 0
	endif

endproc 

PROC 200
	dialog 1
	add_talk $_npc$:
	add_talk #b#bฺǺǣ�Ϊ�����ʥ���ڣ��Ϸ���ǷѾ���˼׼���˺ö����
	add_talk #b#bֻҪ���Ϸ�����þͻ������ѵ�ʱ��ǵĴ�ʥ�����£���Ҫ����ĺ��ӿɲ�Ҫ�������
	add_talk #b#b
	add_talk #c005ebeʥ��������ʱ�䣬ʱ��ǵĴ�ʥ�����½�����������ʥ�������䡿#n
	add_talk #b#b#c186506����ڣ�2013��12��25��-2014��1��3��
	add_talk #b#b�ʱ�䣺10:00-22:00��ÿ���2Сʱ��#n
	
	option 201 #��ʥ���������ģ�
	option 1 #����
	
	talk
ENDPROC

PROC 201
	dialog 1
	add_talk $_npc$:
	add_talk #b#bʱ����������#c18650610��#n��ʥ����Ӵ��
	add_talk #b#b
	add_talk #b#b��1��~#L��ʥ����@@(1000,29504,33330)#n
	add_talk #b#b��2��~#L��ʥ����@@(1000,29448,29696)#n
	add_talk #b#b��3��~#L��ʥ����@@(1000,23301,29856)#n
	add_talk #b#b��4��~#L��ʥ����@@(1000,23349,33149)#n
	add_talk #b#b��5��~#L��ʥ����@@(1000,38795,29661)#n
	add_talk #b#b��6��~#L��ʥ����@@(1000,39694,31108)#n
	add_talk #b#b��7��~#L��ʥ����@@(1000,37960,32411)#n
	add_talk #b#b��8��~#L��ʥ����@@(1000,38847,34238)#n
	add_talk #b#b��9��~#L��ʥ����@@(1000,38432,43607)#n
	add_talk #b#b��10��~#L��ʥ����@@(1000,21414,46004)#n
	
	
	option 1 #����
	
	talk
ENDPROC

PROC 300
	dialog 1
	add_talk $_npc$:
	add_talk #b#b������ʥ���ڡ��ڰ����ʡ����ܳ���������~
	add_talk #b#b����Ϊ��װ�ɶ�׼���ġ�#c005ebeʥ��������#n��������һ����~���ֻ�ð�����������������
	add_talk #b#b��Ҫ��˭�������Ӿ͹�˭���ˣ�ฺǺǡ�
	add_talk #b#b
	add_talk #c005ebe�ڰ���Ϯ��ڼ䣬���ڰ����ʡ�������ӿ��������ѩԭ#n
	add_talk #b#b#c186506�ʱ�䣺10:30-10:40��15:15-15:25#n

	option 1 #����
	
	talk
ENDPROC
