;康师傅大礼包
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
               MSG_PLAYER 2 2 打开礼包，获得五色土一份，康师傅大厨帽一顶！
            ELSE 
               MSG_PLAYER 2 2 打开礼包，获得五色土一份！提醒您装备栏已满，可能会因此无法获得康师傅大厨帽！
            ENDIF  
            RETURN   
         ENDIF
         IF 9 = 2 or 9 = 3       
            ;如果玩家已经飞升，则不能重复获得
            GET_JOB 6
            IF 6 > 5
               MSG_PLAYER 2 2 打开礼包，获得五色土一份！
            ELSE   
               ;如果已经有醍醐丸，则不能重复获得  
               GETNUM_GOODS 12043 5
               IF 5 != 0 
                  MSG_PLAYER 2 2 打开礼包，获得五色土一份！
               ELSE    
                  ADD_GOODS 12043 1 4
                  IF 4 = 0
                     MSG_PLAYER 2 2 打开礼包，获得五色土一份，醍醐丸1个！
                  ELSE 
                     MSG_PLAYER 2 2 打开礼包，获得五色土一份！提醒您消耗品栏已满，可能会因此无法获得醍醐丸！
                  ENDIF    
               ENDIF    
            ENDIF 
            RETURN 
         ENDIF       
         MSG_PLAYER 2 2 打开礼包，获得五色土一份！      
      ELSE
         MSG_PLAYER 2 2 获得五色土失败！
         ADD_GOODS 12048 1 1
      ENDIF 
   ELSE
      MSG_PLAYER 2 2 删除物品失败！
   ENDIF 
ENDPROC 