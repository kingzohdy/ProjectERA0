;����NPC����ɫŮ��
;158 ����֮��ˮ������
;
PROC 1
		GET_TASK_STATUS 158 208
		IF 208 = 0
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK ��������ĸ�¶�����˼䣬����ش�����ɫ��ϣ������Ϊ�Ҳɼ�/C04��ݴ�֮��¶/C00������Ҫ��������������۵�С���硣
			OPTION 2 ����Ϊ��Ч�ͣ�
			OPTION 3 ����ʱû��ʱ�䡣
			TALK
			RETURN
		ENDIF
		
		IF 208 = 2
			GET_TASK_VAR 158 0 209
			GET_TASK_VAR 158 1 210
			GET_TASK_VAR 158 2 211
			GET_TASK_VAR 158 3 212
			GET_TASK_VAR 158 4 213
			
			IF 209 = 1 and 210 = 1 and 211 = 1 and 212 = 1 and 213 = 1
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK ����лл��Ϊ���ռ���¶��������¶�Ľ��࣬С�����Ǻܿ�Ϳ��Գ��ɲ����������
				OPTION 4 ������¶
				TALK
			ELSE
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK �������۵�С���������Ź���أ����Щ�ɼ���/C04��ݴ�֮��¶/C00�������ǵ�/C05�������򡢹��������С����ݺݺ������Ժ��������С��/C00������Բɼ�����
				OPTION 5 #֪����
				TALK
			ENDIF 
			
			RETURN
		ENDIF
		
		IF 208 = 1
			GETNUM_GOODS 8806 997
			IF 997 = 0
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK ������ȴ����/C10С����/C00������ʹ��/C04����/C00���������߶�С���������Ч��Ŷ��
				OPTION 7	��ȡ����
				OPTION 10	�һ�99����ͨ���ѡ�
				OPTION 11	�һ�1�Ž����ѡ�
				OPTION 5	#�Ժ�����
				TALK
			ELSE
				DIALOG 1
				ADD_TALK $_npc$:
				ADD_TALK ���������Ļ��浺ԭ��Ҳ�Ǵ�½�ϵ�һƬ���ޣ��Ӵ˴�ظ��ѣ���ľ�����������һƬ��Į��Ϊ���û��浺����ӵ����ɫ��ֲ��������������Ϊ�Ҵ������ʵ����ѡ�ÿ/C0499��/C00��ͨ���ѿɶһ�һ�ν�����/C04������/C00����ÿһ�Ŷһ�һ�Ρ�
				OPTION 10	�һ�99����ͨ���ѡ�
				OPTION 11	�һ�1�Ž����ѡ�
				OPTION 5	#��ʱ���һ���
				TALK
			ENDIF
		ENDIF
	
ENDPROC

PROC 2
		GET_TASK_NUM 0  
		IF 0 >= 10
			DIALOG 3
      ADD_TALK /C04ϵͳ��ʾ��/C00
      ADD_TALK /C04�Բ�������ʮ�������ڽ����У��޷�����ȡ�µ�����/C00
      TALK 
		ELSE
			DIALOG 3
			ADD_TALK $_npc$:
			ADD_TALK ���������ǵ��������������Բɼ���/C04��֮��¶/C00��������/C05�������򡢹��������С����ݺݺ������Ժ��������С��/C00�����۵�С������������ɼ���/C04��֮��¶/C00�أ�
			TALK
			ACCEPT_TASK 158 1
			ADD_TASK_HINT 158 01
		ENDIF
ENDPROC

PROC 3
		DIALOG 3
		ADD_TALK $_npc$:
		ADD_TALK �������۵�С���������Ŵ�֮��¶�Ĺ�ȣ�Ҳ������һ����ʱ�䡣
		TALK
		RETURN
ENDPROC

PROC 4
		GETNUM_GOODS 12017 997
		DEL_GOODS 12017 @997 1
		
		IF 1 = 0			
			ADD_HONOR 10
			START_SCREENMSG
			ADD_HONOR_SCREENMSG 10
			SHOW_SCREENMSG   
			FINISH_TASK 158
				
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK ���������Ļ��浺ԭ��Ҳ�Ǵ�½�ϵ�һƬ���ޣ������ǰ�Ķ�����ս�У���߸�����÷����ȼ��޻��ĺ�֮ˮ���ɣ������Ļ���Ʈ�䵽���浺�ϣ�ȼ�����������棬�Ӵ˴�ظ��ѣ���ľ������ֻʣ��һƬ��Į��Ϊ���û��浺����ӵ����ɫ��ֲ������Ҫ���������֡�
			OPTION 6 �����
			TALK
	
		ELSE
			DIALOG 3
			ADD_TALK $_npc$:
			ADD_TALK �����ף������ϵ���¶��ô��������
			TALK
		ENDIF
ENDPROC

PROC 5
		RETURN
ENDPROC

PROC 6
		DIALOG 1
		ADD_TALK $_npc$:
		ADD_TALK ���������ݵ�����޵���Ұ�⣬����ഺ��������/C10С����/C00���������/C04����/C00����¶�������ǣ���С���糤������ǻ�͸��������/C04����/C00��
		ADD_TALK ����ÿ�����ռ���/C0499��/C00��ʱ�򣬾Ϳ����õ����������һ����Ľ�������������õĻ�������ܻ�ɼ����ǳ�ϡ�ٵ�/C04������/C00�������ң�����õ���ֵ�Ľ���Ŷ��
		OPTION  7 ��ȡ����
		OPTION	5 #֪����
		TALK
ENDPROC

PROC 7
		ADD_GOODS 8806 1 1
		IF 1 = 0
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK ��������������/C10С����/C00�ֲ������ݵ�����޵���Ұ�⣬��ȥ�û����������ǰɣ������Ĺ��߶�С���������Ч��Ŷ��ף����ˣ�
			OPTION 5 #֪����
			TALK
		ELSE
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK ���������ϵĶ���̫���ˣ��Ų��»�������������һ�±���Ȼ��������ȡ�ɣ�
			OPTION 5 #֪����
			TALK
		ENDIF
ENDPROC

PROC 10
		GET_LEVEL 5
		GETNUM_GOODS 12010 997
		GET_VAR 48 16 998
		IF 997 < 99
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK �����Բ���������û�жһ�Ҫ����㹻���������ѡ�
			OPTION 5 #�Ժ�������
			TALK
		ELSE
			SKIP_CHECK
			DEL_GOODS 12010 99 1
			IF 1 != 0
				DIALOG 3
				ADD_TALK $_npc$:
				ADD_TALK ����ɾ����Ʒʧ�ܣ����Ժ����ԣ�
				TALK
				RETURN
			ELSE
				;���;������Ʒ
				DIALOG 3
				ADD_TALK $_npc$:
				ADD_TALK ������ϲ�����һ��ɹ��ˣ���ȥ�ռ���һЩ���Ӱɣ�
				TALK
				IF 998 >= 0 and 998 <= 395
					IF 5 >= 20
						CALL 21
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 0
						SET_RANK 5606 0 3600
					ENDIF
					MSG_PLAYER 3 2 ����С��ʿ����ӳɣ����һЩ���飡
				ENDIF
				
				IF 998 >= 396 and 998 <= 890 
					IF 5 >= 20
						CALL 22
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 396
						SET_RANK 5607 0 3600
					ENDIF
					MSG_PLAYER 3 2 ����С��ʹ����ӳɣ���ø��ྭ�飡
				ENDIF
				
				IF 998 >= 891 and 998 <= 1385
					IF 5 >= 20
						CALL 23
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 891
						SET_RANK 5608 0 3600
					ENDIF
					MSG_PLAYER 3 2 ��ɫ�ػ��߾���ӳɣ���úܶྭ�飡
				ENDIF
				
				IF 998 >= 1386
					IF 5 >= 20
						CALL 24
					ELSE
						CALL 20
					ENDIF
					
					IF 998 = 1386
						SET_RANK 5609 0 3600
					ENDIF
					MSG_PLAYER 3 2 ��֮���龭��ӳɣ���÷ǳ��ྭ�飡
				ENDIF
				
				;�ۻ�������Ŀ
				ADD 998 99
				SET_VAR 48 16 @998
				RETURN 
			ENDIF
		ENDIF
		
ENDPROC

PROC 11
		GETNUM_GOODS 12011 997
		IF 997 < 1
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK �����Բ���������û�н����ѡ�
			OPTION 5 #�Ժ�����
			TALK
		ELSE
			DIALOG 1
			ADD_TALK $_npc$:
			ADD_TALK ����һ��ֻ�ܶһ�һ�������ѡ�
			OPTION 12 #��ʼ�һ���
			TALK
		ENDIF
ENDPROC

PROC 12
		SKIP_CHECK
		DEL_GOODS 12011 1 1
		IF 1 = 0
			GET_LEVEL 5
			IF 5 >= 20	
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
				DEV 5 20
				MUL 5 3
				ADD_EXP @5
				START_SCREENMSG
				ADD_EXP_SCREENMSG @5
				SHOW_SCREENMSG
			ELSE
				CALL 25
			ENDIF
		ELSE
			MSG_PLAYER 3 2 �һ�ʧ�ܣ����Ժ����ԣ�
		ENDIF
ENDPROC


PROC 20
		SKIP_CHECK
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
		DEV 5 20
		MUL 5 3				     
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 21
		SKIP_CHECK
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
		MUL 5 3
		DEV 5 5
		ADD_EXP @5		
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 22
		SKIP_CHECK
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
		MUL 5 7
		DEV 5 10
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 23
		SKIP_CHECK
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
		ADD_EXP @5	
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 24
		SKIP_CHECK
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
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC

PROC 25
		SKIP_CHECK
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
		DEV 5 20
    MUL 5 3
		ADD_EXP @5
		START_SCREENMSG
		ADD_EXP_SCREENMSG @5
		SHOW_SCREENMSG
ENDPROC