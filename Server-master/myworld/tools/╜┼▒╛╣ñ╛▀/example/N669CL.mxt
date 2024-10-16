;长乐村小溪
;糊涂的妈妈ID=22
; 
;
PROC 1
    GET_TASK_STATUS 23 106
    ;未接或者已经完成扑蝶任务         
    IF 106 = 0 or 106 = 1
        GET_LEVEL 5        
        IF 5 <= 10
           GET_TASK_STATUS 22 102
           ;围巾任务
           IF 102 = 2
              DIALOG 1
              ADD_TALK $_npc$：
              ADD_TALK 　　我最喜欢蝴蝶的翅膀啦，它们多漂亮啊，你能帮我收集一些来吗？
              OPTION 20 好的！
              OPTION 10 关于围巾
              OPTION 21 #我还有别的事
              TALK               
              RETURN 
           ELSE 
              DIALOG 1
              ADD_TALK $_npc$：
              ADD_TALK 　　我最喜欢蝴蝶的翅膀啦，它们多漂亮啊，你能帮我收集一些来吗？
              OPTION 20 好的！
              OPTION 21 #我还有别的事
              TALK
              RETURN 
           ENDIF 
        ELSE     
           GET_TASK_STATUS 22 102
           ;围巾任务
           IF 102 = 2
              DIALOG 1
              ADD_TALK $_npc$：
              ADD_TALK 　　妈妈好偏心的，昨天给小叶洗了两遍，却不记得给我洗，我有点生气哦。
              OPTION 10 关于围巾
              OPTION 21 #结束对话
              TALK   
              RETURN               
           ELSE   
              DIALOG 1
              ADD_TALK $_npc$：
              ADD_TALK 　　妈妈好偏心的，昨天给小叶洗了两遍，却不记得给我洗，我有点生气哦。
              OPTION 21 #结束对话
              TALK
              RETURN 
           ENDIF    
        ENDIF 
    ENDIF 
    
    ;扑蝶任务进行中
    IF 106 = 2
        GET_TASK_STATUS 22 102
        ;围巾任务
        IF 102 = 2
           DIALOG 1
           ADD_TALK $_npc$：
           ADD_TALK 　　有收集到美丽的蝴蝶翅膀吗？
           OPTION 10 关于围巾
           OPTION 22 交出蝴蝶翅膀
           TALK    
        ELSE 
           DIALOG 1
           ADD_TALK $_npc$：
           ADD_TALK 　　有收集到美丽的蝴蝶翅膀吗？
           OPTION 22 交出蝴蝶翅膀
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
       ADD_TALK $_npc$：
       ADD_TALK 　　妈妈好偏心的，昨天给小叶洗了两遍，却不记得给我洗，我有点生气哦。
       OPTION 11 交出黄色围巾
       TALK           
       RETURN 
     ENDIF    
     
     IF 103 = 20
        ;检测红色围巾
        GETNUM_GOODS 3423 104        
        IF 104 = 1
           DIALOG 1
           ADD_TALK $_npc$：
    	   ADD_TALK 　我的围巾好像不是黄色的……
    	   OPTION 21 #结束对话
    	   TALK
    	   RETURN                    
	ELSE
          DIALOG 1
          ADD_TALK $_npc$：
          ADD_TALK 　　妈妈好偏心的，昨天给小叶洗了两遍，却不记得给我洗，我有点生气哦。
    	  OPTION 13 交出黄色围巾
    	  TALK   
    	  RETURN
	ENDIF         
     ENDIF 
     
     IF 103 = 30
        ;检测红色围巾
        GETNUM_GOODS 3423 104          
        IF 104 = 1
           DIALOG 1
    	   ADD_TALK $_npc$：
    	   ADD_TALK 　　对呀，这条红色围巾才是我的嘛，妈妈又搞错啦，麻烦你把黄色的围巾交给小叶。
    	   OPTION 14 #好的
    	   TALK         
    	   RETURN
        ELSE 
 	   DIALOG 1
           ADD_TALK $_npc$：
           ADD_TALK 　　黄色围巾是小叶的，我的围巾是红色的。
           OPTION 21 #结束对话
           TALK  
           RETURN                    
        ENDIF        
     ENDIF 
     
     IF 103 = 40
        ;检测红色围巾
        GETNUM_GOODS 3423 104          
        IF 104 = 1
           DIALOG 1
           ADD_TALK $_npc$：
           ADD_TALK 　　对了，红色围巾才是我的，谢谢你。请你告诉我妈妈，我一点都不冷，不过我会戴上围巾的啦！
           OPTION 15 #好的
           TALK    	  
           RETURN 
        ELSE 
           DIALOG 1
    	   ADD_TALK $_npc$：
    	   ADD_TALK 　　麻烦你把黄色的围巾交给小叶。
    	   OPTION 21 #好的
    	   TALK                 
           RETURN            
        ENDIF    
     ENDIF   
     
     IF 103 = 50
        DIALOG 1
        ADD_TALK $_npc$：
        ADD_TALK 　　谢谢你送围巾给我。请你告诉我妈妈，我会戴上围巾的，叫她别担心啦！
        OPTION 21 #不客气
        TALK                 
        RETURN      
     ENDIF   	   
ENDPROC 
;
;
PROC 11
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 　　我的围巾好像不是黄色的哦……我先收下吧，谢谢你！
    OPTION 12 #不客气
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
          MSG_PLAYER 3 2 删除物品失败！
       ENDIF 
    ELSE 
       MSG_PLAYER 3 2 交出围巾失败！
    ENDIF    
ENDPROC 
;
;  
PROC 13
    DIALOG 1
    ADD_TALK $_npc$：
    ADD_TALK 　　妈妈真糊涂，黄色围巾是小叶的，我的围巾是红色的，你帮我从小叶那里换回红色围巾吧！
    OPTION 21 #知道了
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
             MSG_PLAYER 3 2 对不起，由于您物品栏中的消耗品存放空间已满，因此无法得到“黄色围巾”。请在整理物品栏后再试一次！
             ADD_GOODS 3423 1 1
             RETURN 
          ELSE
    	     SET_TASK_VAR 22 0 40    
    	     ADD_TASK_HINT 22 07
          ENDIF 	  
       ELSE 
          MSG_PLAYER 3 2 删除物品失败！
       ENDIF  
    ELSE 
       MSG_PLAYER 3 2 获得围巾失败！
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
          MSG_PLAYER 3 2 删除物品失败！
       ENDIF 
    ELSE 
       MSG_PLAYER 3 2 交出围巾失败！
    ENDIF         
ENDPROC 
;
;  
PROC 20     
    GET_TASK_NUM 0  
    IF 0 >= 10
    	DIALOG 3
    	ADD_TALK /C04系统提示：/C00
    	ADD_TALK /C04对不起，你有十个任务在进行中，无法再领取新的任务。/C00
	TALK 
    ELSE              
    	DIALOG 1
        ADD_TALK $_npc$:         
        ADD_TALK 　　村子有南北两个出口，在小地图上可以看到方位的指向。往南走，当看见一个旋转的/C05红色光环/C00时，那就是村子通往吟松谷的路口，踩在光环上，就会被传到吟松谷。经过吟松谷往东或者往西走，就可以到吟松谷东或吟松谷西了。
        ADD_TALK 　　在/C05吟松谷东/C00和/C05吟松谷西/C00里，有许多/C10蝴蝶/C00，鼠标左键单击它就可以进行攻击，打败它们就可以得到翅膀啦，鼠标左键点击就可以捡起来放进背包里。
        OPTION 21 #知道了
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
       ADD_TALK 　　你没有带来我喜欢的蝴蝶翅膀哦！在/C05吟松谷东/C00和/C05吟松谷西/C00里，有许多/C10蝴蝶/C00，鼠标左键单击它就可以进行攻击，打败它们就可以得到翅膀啦，鼠标左键点击就可以捡起来放进背包里。        
       OPTION 21 #知道了
       TALK 
       RETURN 
    ELSE    
       DIALOG 1
       ADD_TALK $_npc$：
       ADD_TALK 　　好漂亮的蝴蝶翅膀，谢谢你哦！
       OPTION 23 #领取奖励
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
    	   MSG_PLAYER 3 2 删除物品失败！
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
                 MSG_PLAYER 3 2 恭喜你获得任务授予的称号！单击“Alt+C”键打开角色面板，点击"称号"按钮，你可以在称号列表中选择显示不同的称号。
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
	            MSG_PLAYER 3 2 恭喜你获得任务授予的称号！单击“Alt+C”键打开角色面板，点击"称号"按钮，你可以在称号列表中选择显示不同的称号。
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
	      MSG_PLAYER 3 2 恭喜你获得任务授予的称号！单击“Alt+C”键打开角色面板，点击"称号"按钮，你可以在称号列表中选择显示不同的称号。
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


       	     
       
     	
    	