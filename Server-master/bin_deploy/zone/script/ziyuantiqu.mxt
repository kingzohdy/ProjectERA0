


proc 1
	DIALOG 1
	
	add_talk $_npc$:
	add_talk #b#b���ã�����Ϊ���Ὺ����̬�İ����ǣ���Ȼ��������̬�޷�Ϊ�˸��õĲɼ���Դ�������������ﻹ�а��˲ɼ����������ء�
	add_talk #b#b
		option 200 #��ȡ�ɼ�����Դ���뱳��
			
	strvar Ret
	task_stat 3675 1 Ret
	if Ret = 2
		option 250 ��������ȡ����õ���Դ��
	endif
	talk
endproc 

proc 200
	LAYDOWN_CLAN_ORE 0 0
endproc
proc 250
	DIALOG 1	
	add_talk $_npc$:
 	add_talk #b#b�����ҵ���Ҫ����Դ�ˣ��ѻ��Ķ���ʵ��̫�࣬����õȷǳ��ı�Ǹ��
		option 251 #��ȡ����õ���Դ
	talk
endproc 
proc 251
	add_item_num_inpak 5300872 1 1 0
endproc 