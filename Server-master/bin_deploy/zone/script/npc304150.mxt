;����

proc 1
	dialog 1
	add_talk $_npc$:
	add_talk #b#b��ӭ���١�
	add_talk #b#b������ʲô�ܰﵽ����
	add_talk #b#b��
	add_talk #b#b����
	add_talk #b#b������
	add_talk #b#bлл��ˡ�
	add_talk #b#b�ڴ���һ��Ϊ������
	add_talk #b#b
	add_talk #b#b #c186506�����Ծ�Ͷ�ҹ���������ҷ�������#n
	
	option 100 #(����)���̵�
	
	talk

endproc 

proc 100
	OPEN_NPC_SHOP
endproc
