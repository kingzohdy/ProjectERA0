;��������ϲ�С��
;
;
PROC 1
    GET_SEX 827
    IF 827 = 1
    	DIALOG 1
    	ADD_TALK $_npc$��
    	ADD_TALK �ˣ�˧�磡��˵����������
    	OPTION 10 �����Ҽ�����Ư����Ů������
    	OPTION 20 ������԰�
    	TALK
    ELSE
    	DIALOG 3
    	ADD_TALK $_npc$:
    	ADD_TALK �������᳤�ñ���Ư����Ů�����ˣ��ҿɲ����Ů���Ӽ�������
    	TALK 
    ENDIF
ENDPROC 
;
;
;�����Ի�
PROC 10
   IS_CAPTAIN 833
   IF 833 != 0
    	DIALOG 3
    	ADD_TALK $_npc$:    
    	ADD_TALK ���������ã��Ҹ���ӵ�����ֵ�ɣ�
    	ADD_RAND_HONOR 10
    	TALK
    ELSE
    	DIALOG 1
    	ADD_TALK $_npc$:
    	ADD_TALK ���������ã��Ҹ���ӵ�����ֵ�ɣ��ۣ�ԭ������λ�ӳ�����Ҳ���Ը���Ķ�Ա������ֵŶ��
    	OPTION 30 ֻ���Ҿͺ��ˣ�
    	OPTION 40 ���ҵĶ�Աһ��Ӱɣ�
    	TALK	
    ENDIF 	
ENDPROC
;
;    
;ѡ���˽��淨
PROC 20
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK ʲô���˼ҳ�����ôƯ�������Ȼֻ˵�����ԣ��������������ߣ���Ҫ�����������ֵ��
    DEL_HONOR 5
    TALK
ENDPROC
;
;    
PROC 30
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK �Ѿ�����Ӻ�����ֵ����ֻ����������ӳ�����.......
    ADD_CAPTION_HONOR 5
    TALK
ENDPROC
;
;    
PROC 40
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK �Ѿ��������Ķ�Ա�Ӻ�����ֵ����
    ADD_TEAM_HONOR 5
    TALK
ENDPROC
