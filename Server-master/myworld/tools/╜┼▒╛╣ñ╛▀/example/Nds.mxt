;���Ĺ����һ��齱
;
;
PROC 1
 SKIP_CHECK  
 ;�����Ĺ���ֻ����2006��3��23��00��00--2006��4��20��23��59���жһ�  
  CMP_TIME 1000 20060323000000 20060420235900
  IF 1000 = 0
         DIALOG 1
         ADD_TALK $_npc$��
         ADD_TALK ��������ش���Q���Q�����ǣ��������Ұ�����ɣ�4��1��2��8��9��15��16�գ�ÿ������14��30��16��30������19��30��21��30ħ����ڼ䣬������������м��ʵ���ħ������֮�ֻ���
         OPTION 10 #1��ħ����ȡ���ﾭ��
         OPTION 20 #1��ħ����ȡ����
         OPTION 30 #10��ħ���鿨Ƭ
         OPTION 40 �˵��
         OPTION 50 #�ټ���
         TALK
  ENDIF
ENDPROC

;���ﾭ��ֵ��ʽ����(-3*PET_LV*PET_LV*PET_LV+830*PET_LV*PET_LV-5950*PET_LV+21644)*27/700
PROC 10
  SKIP_CHECK 
            GET_PET_ATTR 3 0 4            
					      MUL 4 @4
					      
					      GET_LEVEL 7
					      MUL 4 @7
					      MUL 4 3  
					
					      GET_LEVEL 5
					      MUL 5 @5
					      MUL 5 830
					
					      GET_LEVEL 6
					      MUL 6 5950
					
					      SUB 5 @6 
					      SUB 5 @4              
					      ADD 5 21644
					      MUL 5 27
					      DEV 5 700 
					                
                ADD_PET_EXP @5 1            
               	IF 1 = 0   
							      MSG_PLAYER 2 2 ��ĳ���������ħ��������������ֵ�����������
							      DEL_GOODS 12009 1 1
							  ELSE   
							      MSG_PLAYER 2 2 �Բ���ֻ�д�����ߵĳ��������ħ���������飡
							  ENDIF 
							  RETURN  
ENDPROC
    

;25�����Ͼ�����٣�25�����»�ȡ�����ˬ
;25������1��ħ����ȡ����ֵ��ʽ����(-3*LV*LV*LV+830*LV*LV-5950*LV+21644)/100
;25������1��ħ����ȡ����ֵ��ʽ����(-3*LV*LV*LV+830*LV*LV-5950*LV+21644)/50
PROC 20
  SKIP_CHECK 
  GET_LEVEL 8
  IF 8 >= 26      
      SKIP_CHECK  
      DEL_GOODS 12009 1 1
      IF 1 = 0
				      GET_LEVEL 4
				      MUL 4 @4
				      
				      GET_LEVEL 7      
				      MUL 4 @7
				      MUL 4 3  
				
				      GET_LEVEL 5
				      MUL 5 @5
				      MUL 5 830
				
				      GET_LEVEL 6
				      MUL 6 5950
				
				      SUB 5 @6 
				      SUB 5 @4              
				      ADD 5 21644
				      DEV 5 100 
				     
				      ADD_EXP @5
	         
			      MSG_PLAYER 2 2 ��������ħ��������������ֵ�������������
	ELSE  
			      MSG_PLAYER 2 2 ��û���㹻��ħ���һ���
	ENDIF 
  ELSE
     	 SKIP_CHECK  
	 DEL_GOODS 12009 1 1
		      IF 1 = 0
					      GET_LEVEL 4
					      MUL 4 @4
					      
					      GET_LEVEL 7      
					      MUL 4 @7
					      MUL 4 3  
					
					      GET_LEVEL 5
					      MUL 5 @5
					      MUL 5 830
					
					      GET_LEVEL 6
					      MUL 6 5950
					
					      SUB 5 @6 
					      SUB 5 @4              
					      ADD 5 21644
					      DEV 5 50 
					     
					      ADD_EXP @5
		         
				      MSG_PLAYER 2 2 ��������ħ��������������ֵ�������������
			ELSE  
			              MSG_PLAYER 2 2 �۳�ħ��ʧ�ܣ�
		        ENDIF 			   
  ENDIF
ENDPROC


;���λ��ȡ��Ʒʧ�ܺ󣬲��ٲ����κ�ħ����80���н��ʡ��鲻����������ʾ��û�г��н�Ʒ��
PROC 30
   RAND 9 100000
   IF 9 >= 0 and 9 < 20000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         ADD_GOODS 3341 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ELSE
            MSG_PLAYER 2 2 ���ú���װ��ױ��һö�� 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 20000 and 9 < 40000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         ADD_GOODS 3342 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ELSE
            MSG_PLAYER 2 2 ���ü�˾װ��ױ��һö�� 
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 40000 and 9 < 60000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         ADD_GOODS 3343 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ELSE
            MSG_PLAYER 2 2 ��������װ��ױ��һö�� 
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 60000 and 9 < 70000 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         ADD_GOODS 3327 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ELSE
            MSG_PLAYER 2 2 ���þ���һö�� 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 70000 and 9 < 70010 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9353 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70010 and 9 < 70020 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9338 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70020 and 9 < 70030 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9354 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70030 and 9 < 70040 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9356 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF 

   IF 9 >= 70040 and 9 < 70050 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9339 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70050 and 9 < 70060 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9355 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70060 and 9 < 70070 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9333 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70070 and 9 < 70080 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9348 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70080 and 9 < 70090 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9334 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70090 and 9 < 70100 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9344 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70100 and 9 < 70110 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9343 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70110 and 9 < 70120 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9349 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70120 and 9 < 70130 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9341 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70130 and 9 < 70140 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9345 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70140 and 9 < 70150 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9351 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70150 and 9 < 70160 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9335 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70160 and 9 < 70170 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9340 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70170 and 9 < 70180 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9350 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70180 and 9 < 70220 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1022 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70220 and 9 < 70260 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1122 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70260 and 9 < 70300 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1521 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70300 and 9 < 70340 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1621 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70340 and 9 < 70380 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1821 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70380 and 9 < 70420 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1923 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70420 and 9 < 70500 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         GET_LEVEL 8
         IF 8 <= 26  
         		APPLY_GIFT my 0 1 1321 1
         		IF 1 != 0 
          		MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	  RETURN
						ENDIF
				 ELSE
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70500 and 9 < 70540 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 3331 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70540 and 9 < 70560 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9365 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70560 and 9 < 70580 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         APPLY_GIFT my 0 1 9366 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ENDIF
      ENDIF  
   ENDIF

   IF 9 >= 70580 and 9 < 80580 
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE
         ADD_GOODS 3328 1 1
         IF 1 != 0 
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
         ELSE
            MSG_PLAYER 2 2 ���ð��¿�һö�� 
         ENDIF
      ENDIF  
   ENDIF 
   
   IF 9 >= 80580 and 9 < 100000
      DEL_GOODS 12009 10 1  
      IF 1 != 0       
         MSG_PLAYER 2 2 ħ������10ö�����ܲμӳ齱���ʱ��������������ﶼ�����ħ����
         RETURN 
      ELSE  	
          	MSG_PLAYER 2 2 ��û�г��н�Ʒ
          	RETURN
      ENDIF 
ENDIF 

ENDPROC



PROC 40
     DIALOG 3
     ADD_TALK $_npc$��
     ADD_TALK ����/C154��1��2��8��9��15��16������/C0514��30-16��30/C00������/C0519��30-21��30/C00���У�ֻ�����ʱ���ڣ����й��ﶼ�м��ʵ���ħ���ʹ�֮�ֻ���ÿ1�������/C0450��ħ��/C00�����ݵ�����޵�����ϡ�ٵĵ���100������ħ����
     ADD_TALK ����ÿ1�������50��ħ����������������黯ױ��Ƭ��һ����ﾭ�飨���ٻ�����������飬ֻ��3��30��-4��20��֮����Ч������֮��ֻ���������̵���ߵȺ���һ�λ���š�
     TALK   
ENDPROC
;
;
PROC 50
   RETURN 
ENDPROC 
;
