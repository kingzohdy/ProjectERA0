;����
;���Ż���
;
PROC 1
	GET_TASK_STATUS 456 270
	GET_TASK_VAR 456 0 271
	GETNUM_GOODS 12075 997
	if 270 = 2 and 271 = 20 and 997 != 0
		DIALOG 1
		ADD_TALK $_npc$��
    		ADD_TALK ���������˰��������ҵ�Ů�������������Դ�ȥ��ɽ�����Ҳ�ҩ����Ҳû�л����ˣ��ҿ����ĺ��Ӱ���
    		OPTION 10 ���ڻ���
    		OPTION 20 #ûʲô���飬�����ˣ�
		TALK
		return	
	ENDIF 
	
	DIALOG 3
      	ADD_TALK $_npc$��
      	ADD_TALK ���������˰��������ҵ�Ů�������������Դ�ȥ��ɽ�����Ҳ�ҩ����Ҳû�л����ˣ��ҿ����ĺ��Ӱ���			
      	TALK    
ENDPROC
;----------
PROC 10
	DIALOG 1
	ADD_TALK $_npc$��
    	ADD_TALK �����ҵĹ�Ů���Ҳ����ˣ�����Ҳû��ʲô��˼����û�л���Ҳ��û�й�ϵ�ˡ���������лл������������
    	ADD_TALK ��������˭û�л��֣����񶫱��ǵ�/C05Ī��/C00��û�аɣ��Ҳ�̫���Ŷ��     
    	OPTION 12 �Խת��
    	OPTION 11 #��������
	TALK
	return	
ENDPROC
;----------------
PROC 11
	GET_TASK_STATUS 456 270
	GET_TASK_VAR 456 0 271
	GETNUM_GOODS 12075 997
	if 270 = 2 and 271 = 20 and 997 != 0
		DEL_GOODS 12075 1 1
		if 1 = 0
			SET_TASK_VAR 456 0 30
			ADD_TASK_HINT 456 03
		else
			MSG_PLAYER 3 2 ɾ������ʧ�ܣ����Ժ����ԣ�
		ENDIF 
	ENDIF
ENDPROC
;---------
PROC 12
	DIALOG 1
	ADD_TALK $_npc$��
    	ADD_TALK �����ݵ��ϻ���һ��ֱ��ʮ�׵�ԲȦ������ţһͷ��ԲȦ���Ĳ���һ��ľ׮��ţ��һ�����׳�������˨�ţ������������ӣ�Ҳ���⿪���ӣ���ô��ţ�ܷ�Ե�Ȧ��Ĳݣ�    
    	OPTION 13 ����
	TALK
ENDPROC
;---------
PROC 13
	DIALOG 3
	ADD_TALK $_npc$��
    	ADD_TALK ������Ϊ����û�а���ľ׮�ϣ���Ȼ���ԳԲ��ˡ���������¶�����
	TALK
ENDPROC
;--------
PROC 20
	return
ENDPROC