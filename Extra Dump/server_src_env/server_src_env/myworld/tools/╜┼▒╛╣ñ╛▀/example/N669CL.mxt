;���ִ�СϪ
;��Ϳ������ID=22
; 
;
PROC 1
    GET_TASK_STATUS 23 106
    ;δ�ӻ����Ѿ�����˵�����         
    IF 106 = 0 or 106 = 1
        GET_LEVEL 5        
        IF 5 <= 10
           GET_TASK_STATUS 22 102
           ;Χ������
           IF 102 = 2
              DIALOG 1
              ADD_TALK $_npc$��
              ADD_TALK ��������ϲ�������ĳ���������Ƕ�Ư���������ܰ����ռ�һЩ����
              OPTION 20 �õģ�
              OPTION 10 ����Χ��
              OPTION 21 #�һ��б����
              TALK               
              RETURN 
           ELSE 
              DIALOG 1
              ADD_TALK $_npc$��
              ADD_TALK ��������ϲ�������ĳ���������Ƕ�Ư���������ܰ����ռ�һЩ����
              OPTION 20 �õģ�
              OPTION 21 #�һ��б����
              TALK
              RETURN 
           ENDIF 
        ELSE     
           GET_TASK_STATUS 22 102
           ;Χ������
           IF 102 = 2
              DIALOG 1
              ADD_TALK $_npc$��
              ADD_TALK ���������ƫ�ĵģ������СҶϴ�����飬ȴ���ǵø���ϴ�����е�����Ŷ��
              OPTION 10 ����Χ��
              OPTION 21 #�����Ի�
              TALK   
              RETURN               
           ELSE   
              DIALOG 1
              ADD_TALK $_npc$��
              ADD_TALK ���������ƫ�ĵģ������СҶϴ�����飬ȴ���ǵø���ϴ�����е�����Ŷ��
              OPTION 21 #�����Ի�
              TALK
              RETURN 
           ENDIF    
        ENDIF 
    ENDIF 
    
    ;�˵����������
    IF 106 = 2
        GET_TASK_STATUS 22 102
        ;Χ������
        IF 102 = 2
           DIALOG 1
           ADD_TALK $_npc$��
           ADD_TALK �������ռ��������ĺ��������
           OPTION 10 ����Χ��
           OPTION 22 �����������
           TALK    
        ELSE 
           DIALOG 1
           ADD_TALK $_npc$��
           ADD_TALK �������ռ��������ĺ��������
           OPTION 22 �����������
           TALK            
        ENDIF 
    ENDIF     
ENDPROC
;
;    
PROC 10    
     GET_TASK_VAR 22 0 103
     IF 103 = 10
       DIALOG 1
       ADD_TALK $_npc$��
       ADD_TALK ���������ƫ�ĵģ������СҶϴ�����飬ȴ���ǵø���ϴ�����е�����Ŷ��
       OPTION 11 ������ɫΧ��
       TALK           
       RETURN 
     ENDIF    
     
     IF 103 = 20
        ;����ɫΧ��
        GETNUM_GOODS 3423 104        
        IF 104 = 1
           DIALOG 1
           ADD_TALK $_npc$��
    	   ADD_TALK ���ҵ�Χ������ǻ�ɫ�ġ���
    	   OPTION 21 #�����Ի�
    	   TALK
    	   RETURN                    
	ELSE
          DIALOG 1
          ADD_TALK $_npc$��
          ADD_TALK ���������ƫ�ĵģ������СҶϴ�����飬ȴ���ǵø���ϴ�����е�����Ŷ��
    	  OPTION 13 ������ɫΧ��
    	  TALK   
    	  RETURN
	ENDIF         
     ENDIF 
     
     IF 103 = 30
        ;����ɫΧ��
        GETNUM_GOODS 3423 104          
        IF 104 = 1
           DIALOG 1
    	   ADD_TALK $_npc$��
    	   ADD_TALK ������ѽ��������ɫΧ������ҵ�������ָ�������鷳��ѻ�ɫ��Χ����СҶ��
    	   OPTION 14 #�õ�
    	   TALK         
    	   RETURN
        ELSE 
 	   DIALOG 1
           ADD_TALK $_npc$��
           ADD_TALK ������ɫΧ����СҶ�ģ��ҵ�Χ���Ǻ�ɫ�ġ�
           OPTION 21 #�����Ի�
           TALK  
           RETURN                    
        ENDIF        
     ENDIF 
     
     IF 103 = 40
        ;����ɫΧ��
        GETNUM_GOODS 3423 104          
        IF 104 = 1
           DIALOG 1
           ADD_TALK $_npc$��
           ADD_TALK �������ˣ���ɫΧ������ҵģ�лл�㡣������������裬��һ�㶼���䣬�����һ����Χ�������
           OPTION 15 #�õ�
           TALK    	  
           RETURN 
        ELSE 
           DIALOG 1
    	   ADD_TALK $_npc$��
    	   ADD_TALK �����鷳��ѻ�ɫ��Χ����СҶ��
    	   OPTION 21 #�õ�
    	   TALK                 
           RETURN            
        ENDIF    
     ENDIF   
     
     IF 103 = 50
        DIALOG 1
        ADD_TALK $_npc$��
        ADD_TALK ����лл����Χ����ҡ�������������裬�һ����Χ��ģ�������������
        OPTION 21 #������
        TALK                 
        RETURN      
     ENDIF   	   
ENDPROC 
;
;
PROC 11
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK �����ҵ�Χ������ǻ�ɫ��Ŷ�����������°ɣ�лл�㣡
    OPTION 12 #������
    TALK
ENDPROC     
;
;
PROC 12       
    GET_TASK_STATUS 22 102
    GET_TASK_VAR 22 0 103
    IF 102 = 2 and 103 = 10
       DEL_GOODS 3424 1 1
       IF 1 = 0       
          SET_TASK_VAR 22 0 20    
          ADD_TASK_HINT 22 05 
       ELSE 
          MSG_PLAYER 3 2 ɾ����Ʒʧ�ܣ�
       ENDIF 
    ELSE 
       MSG_PLAYER 3 2 ����Χ��ʧ�ܣ�
    ENDIF    
ENDPROC 
;
;  
PROC 13
    DIALOG 1
    ADD_TALK $_npc$��
    ADD_TALK �����������Ϳ����ɫΧ����СҶ�ģ��ҵ�Χ���Ǻ�ɫ�ģ�����Ҵ�СҶ���ﻻ�غ�ɫΧ��ɣ�
    OPTION 21 #֪����
    TALK         
    SET_TASK_VAR 22 0 30    
    ADD_TASK_HINT 22 03
ENDPROC 
;
;
PROC 14
    GET_TASK_STATUS 22 102
    GET_TASK_VAR 22 0 103
    IF 102 = 2 and 103 = 30
       DEL_GOODS 3423 1 1
       IF 1 = 0       
          ADD_GOODS 3424 1 1
          IF 1 != 0
             MSG_PLAYER 3 2 �Բ�����������Ʒ���е�����Ʒ��ſռ�����������޷��õ�����ɫΧ��������������Ʒ��������һ�Σ�
             ADD_GOODS 3423 1 1
             RETURN 
          ELSE
    	     SET_TASK_VAR 22 0 40    
    	     ADD_TASK_HINT 22 07
          ENDIF 	  
       ELSE 
          MSG_PLAYER 3 2 ɾ����Ʒʧ�ܣ�
       ENDIF  
    ELSE 
       MSG_PLAYER 3 2 ���Χ��ʧ�ܣ�
    ENDIF    
ENDPROC 
;
;
PROC 15
    GET_TASK_STATUS 22 102
    GET_TASK_VAR 22 0 103
    IF 102 = 2 and 103 = 40
       DEL_GOODS 3423 1 1
       IF 1 = 0
          SET_TASK_VAR 22 0 50    
          ADD_TASK_HINT 22 08            
       ELSE 
          MSG_PLAYER 3 2 ɾ����Ʒʧ�ܣ�
       ENDIF 
    ELSE 
       MSG_PLAYER 3 2 ����Χ��ʧ�ܣ�
    ENDIF         
ENDPROC 
;
;  
PROC 20     
    GET_TASK_NUM 0  
    IF 0 >= 10
    	DIALOG 3
    	ADD_TALK /C04ϵͳ��ʾ��/C00
    	ADD_TALK /C04�Բ�������ʮ�������ڽ����У��޷�����ȡ�µ�����/C00
	TALK 
    ELSE              
    	DIALOG 1
        ADD_TALK $_npc$:         
        ADD_TALK �����������ϱ��������ڣ���С��ͼ�Ͽ��Կ�����λ��ָ�������ߣ�������һ����ת��/C05��ɫ�⻷/C00ʱ���Ǿ��Ǵ���ͨ�����ɹȵ�·�ڣ����ڹ⻷�ϣ��ͻᱻ�������ɹȡ��������ɹ��������������ߣ��Ϳ��Ե����ɹȶ������ɹ����ˡ�
        ADD_TALK ������/C05���ɹȶ�/C00��/C05���ɹ���/C00������/C10����/C00���������������Ϳ��Խ��й�����������ǾͿ��Եõ������������������Ϳ��Լ������Ž������
        OPTION 21 #֪����
        TALK        
        ACCEPT_TASK 23 1        
        ADD_TASK_HINT 23 01      
    ENDIF       	
ENDPROC                    	
;
;
PROC 22
    GETNUM_GOODS 4160 107
    IF 107 = 0
       DIALOG 1
       ADD_TALK $_npc$       
       ADD_TALK ������û�д�����ϲ���ĺ������Ŷ����/C05���ɹȶ�/C00��/C05���ɹ���/C00������/C10����/C00���������������Ϳ��Խ��й�����������ǾͿ��Եõ������������������Ϳ��Լ������Ž������        
       OPTION 21 #֪����
       TALK 
       RETURN 
    ELSE    
       DIALOG 1
       ADD_TALK $_npc$��
       ADD_TALK ������Ư���ĺ������лл��Ŷ��
       OPTION 23 #��ȡ����
       TALK 
       RETURN                
    ENDIF 
ENDPROC
;
;
PROC 23
   GET_TASK_STATUS 23 106
   IF 106 = 2 
      GETNUM_GOODS 4160 107  
      GET_VAR 25 0 73
      ADD 73 @107
      SET_VAR 25 0 @73
       
       IF 107 = 0 
    	   MSG_PLAYER 3 2 ɾ����Ʒʧ�ܣ�
       ELSE 
    	   DEL_GOODS 4160 @107 1    	   
           MUL 107 100
           ADD_MONEY @107
           START_SCREENMSG 
           ADD_MONEY_SCREENMSG @107
           SHOW_SCREENMSG	   
    	   FINISH_TASK 23 
    	    
           IF 73 >= 20 and 73 < 50
              GET_VAR 4 4 70 
              IF 70 = 0
                 ADD_EXP 120
	         ADD_HONOR 1	
	         ADD_GOODS 3434 1 1  
	         START_SCREENMSG 
                 ADD_EXP_SCREENMSG 120
                 ADD_HONOR_SCREENMSG 1
                 ADD_ITEM_SCREENMSG 3434 1
                 SHOW_SCREENMSG	         
	         GET_RANK 1020
                 IF 1020 = 5101
	            SET_VAR 4 4 1
	            RETURN        
                 ENDIF         
                 IF 1020 = 5102
	            SET_VAR 4 4 1
	            RETURN        
                 ENDIF                 
                 SET_RANK 5100 0 36000 	  
                 START_SCREENMSG 
                 ADD_RANK_SCREENMSG 5100
                 SHOW_SCREENMSG	
                 MSG_PLAYER 3 2 ��ϲ������������ĳƺţ�������Alt+C�����򿪽�ɫ��壬���"�ƺ�"��ť��������ڳƺ��б���ѡ����ʾ��ͬ�ĳƺš�
                 SET_VAR 4 4 1
                 RETURN                	       
              ENDIF
           ENDIF    
           IF 73 >= 50 and 73 < 100
              GET_VAR 4 5 101 
              IF 101 = 0
    	         ADD_EXP 300	    	   
	         ADD_HONOR 1	
	         ADD_GOODS 3434 1 1 
	         START_SCREENMSG 
                 ADD_EXP_SCREENMSG 300
                 ADD_HONOR_SCREENMSG 1
                 ADD_ITEM_SCREENMSG 3434 1
                 SHOW_SCREENMSG	  
	         GET_RANK 1020
	         IF 1020 != 5102
	            SET_RANK 5101 0 36000 
	            START_SCREENMSG 
                    ADD_RANK_SCREENMSG 5101
                    SHOW_SCREENMSG	
	            MSG_PLAYER 3 2 ��ϲ������������ĳƺţ�������Alt+C�����򿪽�ɫ��壬���"�ƺ�"��ť��������ڳƺ��б���ѡ����ʾ��ͬ�ĳƺš�
	         ENDIF	         
	         SET_VAR 4 5 1
	         RETURN 
	      ENDIF    
    	   ENDIF         
       	   IF 73 >= 100
    	      ADD_EXP 600	    	   
	      ADD_HONOR 1	
	      ADD_GOODS 3434 1 1   
	      SET_RANK 5102 0 36000 
	      START_SCREENMSG 
              ADD_EXP_SCREENMSG 600
              ADD_HONOR_SCREENMSG 1
              ADD_ITEM_SCREENMSG 3434 1
              ADD_RANK_SCREENMSG 5102
              SHOW_SCREENMSG
	      MSG_PLAYER 3 2 ��ϲ������������ĳƺţ�������Alt+C�����򿪽�ɫ��壬���"�ƺ�"��ť��������ڳƺ��б���ѡ����ʾ��ͬ�ĳƺš�
	      SET_VAR 25 0 0
	      SET_VAR 4 4 0
	      SET_VAR 4 5 0
	      RETURN 
    	   ENDIF          
       ENDIF 	   
   ENDIF     	
ENDPROC 
;
;
PROC 21
   RETURN 
ENDPROC   


       	     
       
     	
    	