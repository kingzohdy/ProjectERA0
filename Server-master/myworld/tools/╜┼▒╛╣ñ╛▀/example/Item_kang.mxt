;��ʦ�������
;
;
PROC 0
   SKIP_CHECK
   DEL_GOODS 12048 1 1 
   IF 1 = 0
      ADD_GOODS 14492 1 2
      IF 2 = 0
         RAND 9 1000
         IF 9 = 0 or 9 = 1
            ADD_GOODS 9389 1 3
            IF 3 = 0
               MSG_PLAYER 2 2 ������������ɫ��һ�ݣ���ʦ�����ñһ����
            ELSE 
               MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�������װ�������������ܻ�����޷���ÿ�ʦ�����ñ��
            ENDIF  
            RETURN   
         ENDIF
         IF 9 = 2 or 9 = 3       
            ;�������Ѿ������������ظ����
            GET_JOB 6
            IF 6 > 5
               MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�
            ELSE   
               ;����Ѿ��������裬�����ظ����  
               GETNUM_GOODS 12043 5
               IF 5 != 0 
                  MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�
               ELSE    
                  ADD_GOODS 12043 1 4
                  IF 4 = 0
                     MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�������1����
                  ELSE 
                     MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�����������Ʒ�����������ܻ�����޷���������裡
                  ENDIF    
               ENDIF    
            ENDIF 
            RETURN 
         ENDIF       
         MSG_PLAYER 2 2 ������������ɫ��һ�ݣ�      
      ELSE
         MSG_PLAYER 2 2 �����ɫ��ʧ�ܣ�
         ADD_GOODS 12048 1 1
      ENDIF 
   ELSE
      MSG_PLAYER 2 2 ɾ����Ʒʧ�ܣ�
   ENDIF 
ENDPROC 