;
;script filename: npc1
;
PROC    1
	DIALOG 1
	ADD_TALK	���ã����Ǳ���Ա
	ADD_TALK	��ѡ��һ�ַ���
	OPTION 21 �򿪻���������
	OPTION 22 ����չ������
   TALK
ENDPROC

PROC 21
	OPEN_STASH 1
	FINISH
ENDPROC

PROC 22
	OPEN_STASH 2
	FINISH
ENDPROC
