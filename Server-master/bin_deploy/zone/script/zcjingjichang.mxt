;������
	
proc 1

	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b�̼������Ѫ�ȣ�������
    add_talk #b#b����������ǿ�ߵ����磬����ǿʳ����ʤ��̭���������֣�ǿ���������ã�
    add_talk #b#bֻҪ�㹻ǿ��ֻҪ�㹻Ѫ�ȡ����������ｫ�������ϵ���ҫ��
    add_talk #b#b�����ǣ�ĩ����ҫ���ã�
	add_talk #c0040FFĩ���������ÿ���ʱ�䣺���� 9��00-23��00
	add_talk #c186506���� 19��30-23��00�μӿɶ����õ�����������
	add_talk #c186506��ÿ�����ֻ�ܻ��300�����������
;	task_add_option 1601 1 101

		option 110 #�򿪵��������̵�
		option 300 #ĩ����ѫ���а���
	talk


	
endproc

PROC 101
;	task_do_option 1601 1
ENDPROC

proc 110
	 OPEN_NPC_SHOP
endproc

proc 2
;	get_trig_task 1 2
;	if 2 = 1602
;		task_fini 1602 1
;	endif


endproc


proc 300
	dialog 1
	add_talk #cff4e00��һʹ���ʼ��ķ�ʽ�������а���,��ע����գ�#n
	add_talk #b#b#c0040FF��1��������#n
	add_talk #b#b����ĩ������*4���ྦྷ������*3
	add_talk #b#b٪����־ҩ��*5�����֮��*1
	add_talk #b#b#c0040FF��2������20��������#n
	add_talk #b#b����ĩ������*4������������*3
	add_talk #b#b٪����־ҩ��*3���Ⱥ�֮��*1������
	add_talk #b#b#c0040FF��21������50��������#n
	add_talk #b#b����ĩ������*3���̾�������*5��
	add_talk #b#b٪����־ҩ��*3������
	add_talk #b#b#c0040FF��51������100��������#n
	add_talk #b#b����ĩ������*2���̾�������*3
	add_talk #b#b٪����־ҩ��*2������
	add_talk #b#b#c0040FF��101������400��������#n
	add_talk #b#b٪����־ҩ��*1���̾�������*1������
	add_talk #b#b

		option 1 ����
	talk
endproc









