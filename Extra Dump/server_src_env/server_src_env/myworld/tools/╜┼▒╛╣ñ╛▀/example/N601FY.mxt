PROC 1
  GET_LEVEL 5
  GETNUM_MONEY 3

  IF 5 < 10 and 3 < 100
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK �ȼ�С��10���������100��
    TALK 
  ELSE 
    DIALOG 3
    ADD_TALK $_npc$��
    ADD_TALK �ȼ�����10��������Ҵ���100��
    TALK 
  ENDIF      
ENDPROC   
    
