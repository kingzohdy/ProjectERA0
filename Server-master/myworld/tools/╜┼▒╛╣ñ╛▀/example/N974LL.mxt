;¥���Ƿɿ�ͧС��
;���ñ���ID��0���������ID��806
;127		ɳ֮ʯ   	ָ��
;3��28�գ�intel����
PROC 1
	;
   GET_TASK_STATUS 127 367
   GET_TASK_VAR 127 2 36
   
   ;ɳ֮ʯ���������
   IF 367 = 2 and 36 = 0
      GET_JOB 6
      IF 6 <= 5    
         DIALOG 1
         ADD_TALK $_npc$:
         ADD_TALK ������ĵ�������¥�������ң��ܸ�����ʶ�㡣
         OPTION 11 �����Ի�
         OPTION 10 #����ʹ�ô��ͷ���
         OPTION 30 #�Ժ�����
         TALK
         RETURN
      ELSE
         get_level 5
		if 5 >= 100
	         DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK ������ĵ�������¥�������ң��ܸ�����ʶ�㡣
	         OPTION 11 �����Ի�
	         OPTION 10 #����ʹ�ô��ͷ���
	         OPTION 20 ����ǰ���ý�
			 ;option 50 #����ǰ���ƻ���
	         OPTION 30 #�Ժ�����         
	         TALK
	         RETURN   
		else
			 DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK ������ĵ�������¥�������ң��ܸ�����ʶ�㡣
	         OPTION 11 �����Ի�
	         OPTION 10 #����ʹ�ô��ͷ���
	         OPTION 20 ����ǰ���ý�
	         OPTION 30 #�Ժ�����         
	         TALK
	         RETURN   
		endif		  
      ENDIF
   ENDIF 
   
   GET_JOB 6
   IF 6 <= 5        
      DIALOG 1
      ADD_TALK $_npc$��
      ADD_TALK �����������������У��ҿ���Ϊ���ṩ������ȥ���ﶼ����Ŷ��
      OPTION 10 #����ʹ�ô��ͷ���      
      OPTION 30 #�Ժ�����
      TALK   
   ELSE 
      get_level 5
		if 5 >= 100
	         DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK �����������������У��ҿ���Ϊ���ṩ������ȥ���ﶼ����Ŷ��
	         OPTION 10 #����ʹ�ô��ͷ���
	         OPTION 20 ����ǰ���ý�
			; option 50 #����ǰ���ƻ���
	         OPTION 30 #�Ժ�����         
	         TALK
	         RETURN   
		else
			 DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK �����������������У��ҿ���Ϊ���ṩ������ȥ���ﶼ����Ŷ��
	         OPTION 10 #����ʹ�ô��ͷ���
	         OPTION 20 ����ǰ���ý�
	         OPTION 30 #�Ժ�����         
	         TALK
	         RETURN   
		endif		
   ENDIF    
ENDPROC 
;
;
PROC 10 
   EXEC TRANSMIT 0  
   GET_VAR  0 1 806
   IF 806 = 0
	MSG_PLAYER 3 1 �Բ��������ڲ��ܱ����ͣ�             
   ELSE
        HAD_BEEN_ACTIVATED 3 13  
        IF 13 = 0
           BE_ACTIVATED 3
           OPEN_TRANS_LIST
        ELSE       
           OPEN_TRANS_LIST
        ENDIF    
   ENDIF    
ENDPROC  
;
;
;��������
PROC 11
   DIALOG 1
   ADD_TALK $_player$:
   ADD_TALK ������Ҳ�ܸ�����ʶ�㣬���Ƚ�ɳ©�����Һ���
   OPTION 12 �����Ի�
   TALK
ENDPROC
;
;
;��������
PROC 12
   DIALOG 1
   ADD_TALK $_npc$:
   ADD_TALK �������������ɳ©֮һ��¥���İ�Σ��ȫ�����ˡ�
   OPTION 13 �����Ի�
   TALK
ENDPROC
;
;
PROC 13
   DIALOG 3
   ADD_TALK $_player$:
   ADD_TALK �������İɡ�
   CALL 14
   TALK
ENDPROC
;
;
PROC 14
   ADD_GOODS 3563 1 1
   IF 1 != 0
      MSG_PLAYER 2 2 �Բ�����������Ʒ���е�����Ʒ��ſռ�����������޷����������������ɳ©һ��������������Ʒ��������һ�Σ�                
   ELSE
      SET_TASK_VAR 127 2 1
      GETNUM_GOODS 3564 998
      GETNUM_GOODS 3565 999
      GETNUM_GOODS 3566 1000
      IF 998 != 0 and 999 != 0 and 1000 != 0
         ADD_TASK_HINT 127 02
      ENDIF
   ENDIF
   RETURN
ENDPROC
;
;
PROC 20
   DIALOG 1
   ADD_TALK $_npc$��
   ADD_TALK ����ֻ�з��������ʿ���ܽ���ý磬��׼��������
   OPTION 22 #׼������
   OPTION 30 #�Ժ�����
   TALK
ENDPROC 
;
;
PROC 22
     GOTO_MAP 86 2614 3554 100 1
     IF 1 =  0
        MSG_PLAYER 3 2 ��ӭ�����ý��쾵��
     ELSE 
        MSG_PLAYER 3 2 ����ʧ�ܣ�
     ENDIF 
ENDPROC
;
;
PROC 30
    RETURN
ENDPROC  
;--------
proc 45
	DIALOG 1
   ADD_TALK $_npc$��
   ADD_TALK ������Į���飬ӵ�дֿ��������������˵���������������ﲻ����
   add_talk ������������������û����ǩ���Ļ���ͨ��֤�Ͳ��ܽ���¥�����Ǹ�����δ����intel��ʹҲ�������⣡
   OPTION 46 #��ȡ����ͨ��֤
   TALK
endproc 
;---------------
proc 46
	cmp_time 1000 20070424000000 20070831235959
	if 1000 = 0
		get_task_status 304 600
		get_task_var 304 0 601
		if 600 = 2 and 601 = 40
			add_goods 3098 1 1
			if 1 = 0
				set_task_var 304 0 50
				add_task_hint 304 05
				;
				DIALOG 3
			    ADD_TALK $_npc$:
			    ADD_TALK ����¥����ͨ��֤�Ѿ������ˣ����е�ͨ��֤���Ѿ������ˣ�����ȥ��intel��ʹ�ɣ�
				talk
			else
				msg_player 3 2 ��ȡ����ͨ��֤ʧ�ܣ���������Ʒ�������ԣ�
			endif
		else
			DIALOG 3
		    ADD_TALK $_npc$:
		    ADD_TALK �������СӢ�ۣ���Ȼ���������˵ľ����Ǻ�ֵ�üν�����������Ҫ�ҵ����ƺ�������Ŷ�������������ʾ�ɡ�
			talk
		endif
	else
		DIALOG 3
	    ADD_TALK $_npc$:
	    ADD_TALK ������˵��λintel��ʹ�Ѿ��뿪�����ˣ���ͱ�æ���ˡ�
		talk
	endif
endproc 
;---------
proc 50
	get_level 5
	if 5 >= 100
		goto_map 143 5874 3843 10 1
		if 1 = 0
			msg_player 3 2 �����ƻ���
		else
			msg_player 3 2 �����ƻ���ʧ�ܣ����Ժ����ԣ�
		endif
	else
		msg_player 3 2 ֻ��100�����ϵ���ʿ���ܽ����ƻ���
	endif
endproc 