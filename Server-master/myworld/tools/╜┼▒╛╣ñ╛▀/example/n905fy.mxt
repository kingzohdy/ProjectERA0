;��ѵСͽ���ĵ�ҩ������
;����jq001��jq002��jq003����������������ͬʱ���С� 
;����mq003:ͨ������(408) �����ж�
;
PROC 1
;��ȡͨ������������������
GET_TASK_STATUS 408 158
GET_LEVEL 5 
    ;δ���ͨ������(�����л�δ��) 
    IF 158 = 2 or 158 = 0 
       DIALOG 1
       ADD_TALK $_npc$��
       ADD_TALK      ��֪����,����ҩʦ���Ǵ��ۼҳ����ģ����������޲�Ӧ�顣�ҵ����˾����������ⶦ�������ļ���ѵ����Ŷ!���������մ��ɵ�ͽ����!
       add_talk      ��ʦ����ҩ�������������ݵ��������ڻ������½�ϣ��ɶ��Ƕ�������!
       OPTION 11     Ŷ������������
       TALK
       return
    endif
    If 158 = 1
    GET_LEVEL 5    
    IF 5 < 15
       DIALOG 3
       ADD_TALK $_npc$��
       ADD_TALK �����������ȥ������Щ��Ҫ�Ĺ����������������ڵ�ʵ�������»�����Ŷ��
       TALK 
       RETURN
    ENDIF 
    IF 5 >= 15 and 5 < 25 
       GET_TASK_STATUS 101 281
       ;jq001��δ���ж�     
       IF 281 = 0      
          DIALOG 0
          ADD_TALK $_npc$��
          ADD_TALK ��ѵʦ���Ĳ���ʵ����̫���ˣ�ÿ���˶�״��ͬ�������ҩ��Ҳ������ͬ����һ����Ҫ׼�������ҩ����ʵ����æ�������أ�
          NEXT 2
          TALK 
          RETURN
       ENDIF 
       ;jq001�Ľ������ж�
       IF 281 = 2
    	  CALL 7
    	  RETURN
       ENDIF
       ;jq001��������ж�
       IF 281 = 1
    	  DIALOG 1
          ADD_TALK $_npc$:
          ADD_TALK �������ҵ���һЩҩ����ԭ���ϣ��治�������Ƕ����ݵ��ϵĲ�����˵���⻹��ԶԶδ����ĺǣ���ѵʦ������Ҫ�����ҩ�ģ�
          OPTION 10 �ã����������ɣ�
          OPTION 11 ���б���¡�
          TALK 
          RETURN 
       ENDIF
    ELSE
       IF 5 < 40
          GET_TASK_STATUS 101 281 
          GET_TASK_STATUS 102 289
          ;jq001�Ľ������ж�
          IF 281 = 2
    	     CALL 7
    	     RETURN
          ELSE 
             ;jq002��δ���ж�     
             IF 289 = 0      
                DIALOG 0
                ADD_TALK $_npc$��
                ADD_TALK �㻹�����ݵ���ת���أ����治���ף�Ҫ�����㻹��ȥ����������ѵʦ���������ĺ���ҩ���ɡ�
                NEXT 4
                TALK 
                RETURN 
             ENDIF 
             ;jq002�Ľ������ж�         
             IF 289 = 2
    	        CALL 8
    	        RETURN
	     ENDIF    	
             ;jq002��������ж�
             IF 289 = 1
    	        DIALOG 1
                ADD_TALK $_npc$:
                ADD_TALK �������ɵò��������Ҽ������˵��У����ǱȽϿ��һ�������Ҿ�������ʺ���ҩʦ����Ŷ����ô����Ҫ��Ҫ�������ռ�ҩ����ݺ���ǰ;��ְҵ�أ�
                OPTION 12 û���⣡
                OPTION 11 ���б���¡�
                TALK 
                RETURN 
             ENDIF
          ENDIF
       ELSE
         ;IF 5 <= 99 
            GET_TASK_STATUS 107 281 
            GET_TASK_STATUS 108 289
            GET_TASK_STATUS 109 297
            ;jq001�Ľ������ж�
            IF 281 = 2
    	       CALL 7
    	       RETURN
            ELSE 
               ;jq002�Ľ������ж�
               IF 289 = 2
                  CALL 8
                  RETURN
               ELSE
                  ;jq003��δ���ж�     
                  IF 297 = 0      
                     DIALOG 0
                     ADD_TALK $_npc$��
                     ADD_TALK ����������ɫ�����������ֵ�ҩʦ�����ҵ�����Ŷ��
                     NEXT 5
                     TALK 
                     RETURN 
                  ENDIF 
                  ;jq003�Ľ������ж�         
                  IF 297 = 2
    	             CALL 9
    	             RETURN
	          ENDIF    	
                  ;jq003��������ж�
                  IF 297 = 1
    	             DIALOG 1
                     ADD_TALK $_npc$:
                     ADD_TALK �������Ե�ʣ���ѵʦ����ҩ�����Ѿ����������ڳ�Ϊ������һ��Ʒ���ˣ����룬�ռ�ҩ�ĵĹ������Ǽ�������е��ȽϺ��أ�
                     
                     OPTION 13 û���⣡
                     OPTION 11 ���б���¡�
                     TALK 
                     RETURN 
                  ENDIF
               ENDIF 
            ENDIF 
       ENDIF     
    ENDIF    
    ENDIF      
ENDPROC
;
;
PROC 2
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK  ��ϣ�������ܸ��Ҵ���֣����ܰ���ȥ�ռ�ҩ����
      OPTION 10 ��Ȼ���ԣ�
      OPTION 11 ���б����
      TALk    
ENDPROC
;
;
PROC 4
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK �����������ݵ�����ѵʦ����ҩ�������������ĺ�����Ը�����Ϊ���յĽ�����Ŭ����
      OPTION 12 ��Ȼ���ԣ�
      OPTION 11 ���б����
      TALK    
ENDPROC
;
;
PROC 5
      DIALOG 1
      ADD_TALK $_npc$:
      ADD_TALK  ���ֿ����㣬Ϊ��ҩʦְҵ�Ĺ��٣�Ϊ����ѵʦ��������������������ռ�ҩ��Ŷ��
      OPTION 13 �Ѳ�����
      OPTION 11 ���б����
      TALK   
ENDPROC
;
;
PROC 7
   GET_TASK_VAR 101 0 282
   IF 282 = 1
      GETNUM_GOODS 4103 283
      IF 283 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ����ҩ���е�/C0410/C00ֻ/C05������֦/C00��
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK  ��������Ŷ�����Ѿ��û���ҩ�����زģ��Ҵ����ݵ������İ��ո�л�㣬������
    	 OPTION 20 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF 
   ENDIF
   if 282 = 2
   GETNUM_GOODS  4159 284
      IF 284 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ����ҩ���е�/C0410/C00ֻ/C05���/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK ��������Ŷ�����Ѿ��û���ҩ�����زģ��Ҵ����ݵ������İ��ո�л�㣬������
    	 OPTION 20 #��ȡ����
    	 TALK
    	 RETURN	
     ENDIF	
   ENDIF
   if 282 = 3
   GETNUM_GOODS 4151 285
      IF 285 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ����ҩ���е�/C0410/C00ֻ/C05�ز���/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK ��������Ŷ�����Ѿ��û���ҩ�����زģ��Ҵ����ݵ������İ��ո�л�㣬������
    	 OPTION 20 #��ȡ����
    	 TALK
    	 RETURN
       ENDIF		
   ENDIF
   if 282 = 4
   GETNUM_GOODS 3005 286
      IF 286 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ����ҩ���е�/C045/C00ֻ/C05���հ�/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK ��������Ŷ�����Ѿ��û���ҩ�����زģ��Ҵ����ݵ������İ��ո�л�㣬������
    	 OPTION 20 #��ȡ����
    	 TALK
    	 RETURN
    	ENDIF		
   ENDIF 
   if 282 = 5
   GETNUM_GOODS 3006 287
      IF 287 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ����ҩ���е�/C045/C00ƿ/C05�˲ξ�/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK ��������Ŷ�����Ѿ��û���ҩ�����زģ��Ҵ����ݵ������İ��ո�л�㣬������
    	 OPTION 20 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF   	
ENDPROC 
;
;
PROC 8
   GET_TASK_VAR 102 0 290
   IF 290 = 1
      GETNUM_GOODS 4347 291
      IF 291 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�����ѵʦ�������ĺ���ҩ���е�/C0410/C00ֻ/C05����ţ��/C00����
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK  �ɵò������ҿ�����Ӧ���ܳ�Ϊһ�������ҩʦ�ģ�ף����ˣ�
    	 OPTION 30 #��ȡ����
    	 TALK
    	 RETURN
     ENDIF		
   ENDIF
   IF 290 = 2
      GETNUM_GOODS 4304 292
      IF 292 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�����ѵʦ�������ĺ���ҩ���е�/C0410/C00ֻ/C05ɽ��β��/C00����
	 TALK 
	 RETURN
      ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ɵò������ҿ�����Ӧ���ܳ�Ϊһ�������ҩʦ�ģ�ף����ˣ�
    	 OPTION 30 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF 
   ENDIF
   if 290 = 3
   GETNUM_GOODS 4122 293
      IF 293 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�����ѵʦ�������ĺ���ҩ���е�/C0410/C00ֻ/C05�׻�Ƥ/C00����
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ɵò������ҿ�����Ӧ���ܳ�Ϊһ�������ҩʦ�ģ�ף����ˣ�
    	 OPTION 30 #��ȡ����
    	 TALK
    	 RETURN		
    ENDIF
   ENDIF
   if 290 = 4
   GETNUM_GOODS 4312 294
      IF 294 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�����ѵʦ�������ĺ���ҩ���е�/C045/C00ֻ/C05����/C00����
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ɵò������ҿ�����Ӧ���ܳ�Ϊһ�������ҩʦ�ģ�ף����ˣ�
    	 OPTION 30 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF
   ENDIF 
   if 290 = 5
   GETNUM_GOODS 3007 295
      IF 295 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�����ѵʦ�������ĺ���ҩ���е�/C045/C00ֻ/C05������/C00����
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ɵò������ҿ�����Ӧ���ܳ�Ϊһ�������ҩʦ�ģ�ף����ˣ�
    	 OPTION 30 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF   	
ENDPROC
;
;
PROC 9
   GET_TASK_VAR 103 0 298
   if 298 = 1
   GETNUM_GOODS 4311 299
      IF 299 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ�����˲�֪�����˲����ĵ�ҩ���е�/C0410/C00ֻ/C05������/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ۣ�������ô�������ˣ����������µĿɲ���Ŷ�������кñ����Ҳ�������ʧ���ģ��ٺ١�
    	 OPTION 40 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 2
   GETNUM_GOODS 4352 300
      IF 300 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ�����˲�֪�����˲����ĵ�ҩ���е�/C0410/C00ֻ/C05������/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ۣ�������ô�������ˣ����������µĿɲ���Ŷ�������кñ����Ҳ�������ʧ���ģ��ٺ١�
    	 OPTION 40 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 3
   GETNUM_GOODS 4315 301
      IF 301 < 10 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ�����˲�֪�����˲����ĵ�ҩ���е�/C0410/C00ֻ/C05������/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ۣ�������ô�������ˣ����������µĿɲ���Ŷ�������кñ����Ҳ�������ʧ���ģ��ٺ١�
    	 OPTION 40 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 4
   GETNUM_GOODS 3012 302
      IF 302 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ�����˲�֪�����˲����ĵ�ҩ���е�/C045/C00ֻ/C05��С������ҩˮ/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ۣ�������ô�������ˣ����������µĿɲ���Ŷ�������кñ����Ҳ�������ʧ���ģ��ٺ١�
    	 OPTION 40 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
   if 298 = 5
   GETNUM_GOODS 3013 303
      IF 303 < 5 
	 DIALOG 3
    	 ADD_TALK $_npc$��
    	 ADD_TALK �����㻹û�ռ�����ѵʦ�����˲�֪�����˲����ĵ�ҩ���е�/C045/C00ֻ/C05��С������ҩˮ/C00��
	 TALK 
	 RETURN
       ELSE
     	 DIALOG 1
    	 ADD_TALK $_npc$��
    	 ADD_TALK �ۣ�������ô�������ˣ����������µĿɲ���Ŷ�������кñ����Ҳ�������ʧ���ģ��ٺ١�
    	 OPTION 40 #��ȡ����
    	 TALK
    	 RETURN		
      ENDIF   
   ENDIF
ENDPROC
;
;
;      
;ѡ��jq001:���ݵ���ҩ������
PROC 10 
    GET_TASK_NUM 0  
    IF 0 >= 10
    	DIALOG 3
    	ADD_TALK /C04$ϵͳ��ʾ$��/C00
    	ADD_TALK /C04�Բ�������ʮ�������ڽ����У��޷�����ȡ�µ�����/C00
	TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����������һ��ҩʦ�Ļ�������������ˣ�����㲻��ҩʦ����Ҳû��������ô��ʦ����ҩ������Ҫ/C0410/C00ֻ/C05������֦/C00��������Ҵ������ɣ�
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 1
           ADD_TASK_HINT 101 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����������һ��ҩʦ�Ļ�������������ˣ�����㲻��ҩʦ����Ҳû��������ô��ʦ����ҩ������Ҫ/C0410/C00ֻ/C05���/C00��������Ҵ������ɣ�
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 2
           ADD_TASK_HINT 101 02
           ENDCASE 
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����������һ��ҩʦ�Ļ�������������ˣ�����㲻��ҩʦ����Ҳû��������ô��ʦ����ҩ������Ҫ/C0410/C00ֻ/C05�ز���/C00��������Ҵ������ɣ�
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 3
           ADD_TASK_HINT 101 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����������һ��ҩʦ�Ļ�������������ˣ�����㲻��ҩʦ����Ҳû��������ô��ʦ����ҩ������Ҫ/C045/C00ֻ/C05���հ�/C00��������Ҵ������ɣ�
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 4
           ADD_TASK_HINT 101 04 
           ENDCASE
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����������һ��ҩʦ�Ļ�������������ˣ�����㲻��ҩʦ����Ҳû��������ô��ʦ����ҩ������Ҫ/C045/C00ֻ/C05�˲ξ�/C00��������Ҵ������ɣ�
           TALK
           ACCEPT_TASK 101 1
           SET_TASK_VAR 101 0 5
           ADD_TASK_HINT 101 05 
           ENDCASE
        ENDSWITCH              
    ENDIF     
ENDPROC
;
;
;ѡ��jq002:�����ĺ���ҩ������
PROC 12 
    GET_TASK_NUM 0  
    IF 0 >= 10
       DIALOG 3
    	ADD_TALK /C04$ϵͳ��ʾ$��/C00
    	ADD_TALK /C04�Բ�������ʮ�������ڽ����У��޷�����ȡ�µ�����/C00
       TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����ռ�ҩ�����ǳ�Ϊһ����ҩʦ�ıؾ�֮·Ŷ�����ڣ���ѵʦ�������ĺ���ҩ������Ҫ/C0410/C00ֻ/C05����ţ��/C00����������������ɣ�
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 1
           ADD_TASK_HINT 102 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����ռ�ҩ�����ǳ�Ϊһ����ҩʦ�ıؾ�֮·Ŷ�����ڣ���ѵʦ�������ĺ���ҩ������Ҫ/C0410/C00ֻ/C05ɽ��β��/C00����������������ɣ�
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 2
           ADD_TASK_HINT 102 02 
           ENDCASE
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����ռ�ҩ�����ǳ�Ϊһ����ҩʦ�ıؾ�֮·Ŷ�����ڣ���ѵʦ�������ĺ���ҩ������Ҫ/C0410/C00ֻ/C05�׻�Ƥ/C00����������������ɣ�
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 3
           ADD_TASK_HINT 102 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����ռ�ҩ�����ǳ�Ϊһ����ҩʦ�ıؾ�֮·Ŷ�����ڣ���ѵʦ�������ĺ���ҩ������Ҫ/C045/C00ֻ/C05����/C00����������������ɣ�
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 4
           ADD_TASK_HINT 102 04 
           ENDCASE
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK �����ռ�ҩ�����ǳ�Ϊһ����ҩʦ�ıؾ�֮·Ŷ�����ڣ���ѵʦ�������ĺ���ҩ������Ҫ/C045/C00ֻ/C05������/C00����������������ɣ�
           TALK
           ACCEPT_TASK 102 1
           SET_TASK_VAR 102 0 5
           ADD_TASK_HINT 102 05 
           ENDCASE
        ENDSWITCH      
    ENDIF 
ENDPROC
;
;
;ѡ��jq003:��ѵ��ҩ������
PROC 13
    GET_TASK_NUM 0  
    IF 0 >= 10
       DIALOG 3
    	ADD_TALK /C04$ϵͳ��ʾ$��/C00
    	ADD_TALK /C04�Բ�������ʮ�������ڽ����У��޷�����ȡ�µ�����/C00
       TALK 
    ELSE
        RAND 9  5
        SWITCH 9
        CASE 0 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK ����һ�����������м�������֮�ģ���ѵʦ����ҩ���л���Ҫ/C0410/C00��/C05������/C00������������������Ұɣ�
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 1
           ADD_TASK_HINT 103 01 
           ENDCASE
        CASE 1
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK ����һ�����������м�������֮�ģ���ѵʦ����ҩ���л���Ҫ/C0410/C00��/C05������/C00������������������Ұɣ�
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 2
           ADD_TASK_HINT 103 02 
           ENDCASE
        CASE 2 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK ����һ�����������м�������֮�ģ���ѵʦ����ҩ���л���Ҫ/C0410/C00��/C05������/C00������������������Ұɣ�
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 3
           ADD_TASK_HINT 103 03
           ENDCASE
        CASE 3 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK ����һ�����������м�������֮�ģ���ѵʦ����ҩ���л���Ҫ/C045/C00��/C05��С������ҩˮ/C00������������������Ұɣ�
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 4
           ADD_TASK_HINT 103 04
           ENDCASE 
        CASE 4 	
           DIALOG 3
           ADD_TALK $_npc$:
           ADD_TALK ����һ�����������м�������֮�ģ���ѵʦ����ҩ���л���Ҫ/C045/C00��/C05��С������ҩˮ/C00������������������Ұɣ�
           TALK
           ACCEPT_TASK 103 1
           SET_TASK_VAR 103 0 5
           ADD_TASK_HINT 103 05 
           ENDCASE
        ENDSWITCH      
    ENDIF 
ENDPROC
;
;
;ѡ���˳��Ի�
PROC 11
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK �����������Ƿ���ҩʦ����Ӧ����һ�ż����������ѽ��
    TALK
ENDPROC  
;
;
;
;jq001�Ľ���
PROC 20
   GET_TASK_VAR 101 0 282
   GET_JOB 1016
IF 1016 = 4
   If 282 = 1
    DEL_GOODS 4103 10 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;����   	             
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 2
    DEL_GOODS 4159 10 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;����   	             
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 3
    DEL_GOODS 4151 10 1
    ADD_MONEY 1000
    ADD_EXP 500
    ;����
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 4
    DEL_GOODS 3005 5 1
 ADD_MONEY 1000
    ADD_EXP 500
    ;����
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
      if 282 = 5
    DEL_GOODS 3006 5 1
    ADD_MONEY 1000
    ADD_EXP 500
     ;����   	             
   ADD_MONEY 1000
    ADD_EXP 500
    ;����
    GET_VAR 14 0 288
       ADD 288 1                
       IF 288 < 20
    	     SET_VAR 14 0 @288
           MUL 288 200 
           ADD_MONEY @288
           ADD_EXP @288
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
;
ELSE
if 282 = 1
    DEL_GOODS 4103 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 2
    DEL_GOODS 4159 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 3
    DEL_GOODS 4151 10 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
    if 282 = 4
    DEL_GOODS 3005 5 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
      if 282 = 5
    DEL_GOODS 3006 5 1
    ADD_MONEY 500
    SET_TASK_VAR 101 0 0
    FINISH_TASK 101 
    ENDIF
ENDIF
ENDPROC
;
;
;
;jq002�Ľ���
PROC 30
   GET_TASK_VAR 102 0 290
   GET_JOB 1016
IF 1016 = 4
   If 290 = 1
    DEL_GOODS 4347 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 2
    DEL_GOODS 4304 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 3
    DEL_GOODS 4122 10 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
    if 290 = 4
    DEL_GOODS 4312 5 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
      if 290 = 5
    DEL_GOODS 3007 5 1
    ADD_MONEY 1500
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   296
       ADD 296 1                
       IF 296 < 20
    	     SET_VAR 14 0 @296
           MUL 296 500 
           ADD_MONEY @296
           ADD_EXP @296
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102 
    ENDIF
;
ELSE
if 290 = 1
    DEL_GOODS 4347 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 2
    DEL_GOODS 4304 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 3
    DEL_GOODS 4122 10 1
    ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
    if 290 = 4
    DEL_GOODS 4312 5 1
   ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
      if 290 = 5
    DEL_GOODS 3007 5 1
   ADD_MONEY 800
    SET_TASK_VAR 102 0 0
    FINISH_TASK 102
    ENDIF
ENDIF
ENDPROC
;
;
;
;jq003�Ľ���
PROC 40
   GET_TASK_VAR 103 0 298
   GET_JOB 1016
IF 1016 = 4
   If 298 = 1
    DEL_GOODS 4311 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 2
    DEL_GOODS 4352 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 3
    DEL_GOODS 4315 10 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
    if 298 = 4
    DEL_GOODS 3012 5 1
    ADD_MONEY 2000
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
      if 298 = 5
    DEL_GOODS 3013 5 1
     ADD_MONEY 2000
    ADD_EXP 1000
     ;����   	             
    GET_VAR 14 0   304
       ADD 304 1                
       IF 304 < 20
    	     SET_VAR 14 0 @304
           MUL 304 1000 
           ADD_MONEY @304
           ADD_EXP @304
       ELSE
           SET_VAR 14 0 0
       ENDIF
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103 
    ENDIF
;
ELSE
if 298 = 1
    DEL_GOODS 4311 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 2
    DEL_GOODS 4352 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 3
    DEL_GOODS 4315 10 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
    if 298 = 4
    DEL_GOODS 3012 5 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
      if 298 = 5
    DEL_GOODS 3013 5 1
    ADD_MONEY 1000
    SET_TASK_VAR 103 0 0
    FINISH_TASK 103
    ENDIF
ENDIF
ENDPROC
;
;

 	