PROC 1
  GET_LEVEL 5
  GETNUM_MONEY 3

  IF 5 < 10 and 3 < 100
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 等级小于10，天币少于100。
    TALK 
  ELSE 
    DIALOG 3
    ADD_TALK $_npc$：
    ADD_TALK 等级大于10，或者天币大于100。
    TALK 
  ENDIF      
ENDPROC   
    
