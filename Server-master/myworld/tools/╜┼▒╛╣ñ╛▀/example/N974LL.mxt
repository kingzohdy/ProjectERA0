;楼兰城飞空艇小姐
;永久变量ID：0；世界变量ID：806
;127		沙之石   	指引
;3月28日，intel任务
PROC 1
	;
   GET_TASK_STATUS 127 367
   GET_TASK_VAR 127 2 36
   
   ;沙之石任务进行中
   IF 367 = 2 and 36 = 0
      GET_JOB 6
      IF 6 <= 5    
         DIALOG 1
         ADD_TALK $_npc$:
         ADD_TALK 　　你的到来，是楼兰的荣幸，很高兴认识你。
         OPTION 11 继续对话
         OPTION 10 #我想使用传送服务
         OPTION 30 #稍后再来
         TALK
         RETURN
      ELSE
         get_level 5
		if 5 >= 100
	         DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK 　　你的到来，是楼兰的荣幸，很高兴认识你。
	         OPTION 11 继续对话
	         OPTION 10 #我想使用传送服务
	         OPTION 20 我想前往幻界
			 ;option 50 #我想前往云荒域
	         OPTION 30 #稍后再来         
	         TALK
	         RETURN   
		else
			 DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK 　　你的到来，是楼兰的荣幸，很高兴认识你。
	         OPTION 11 继续对话
	         OPTION 10 #我想使用传送服务
	         OPTION 20 我想前往幻界
	         OPTION 30 #稍后再来         
	         TALK
	         RETURN   
		endif		  
      ENDIF
   ENDIF 
   
   GET_JOB 6
   IF 6 <= 5        
      DIALOG 1
      ADD_TALK $_npc$：
      ADD_TALK 　　如果您想外出旅行，我可以为您提供服务，想去哪里都可以哦！
      OPTION 10 #我想使用传送服务      
      OPTION 30 #稍后再来
      TALK   
   ELSE 
      get_level 5
		if 5 >= 100
	         DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK 　　如果您想外出旅行，我可以为您提供服务，想去哪里都可以哦！
	         OPTION 10 #我想使用传送服务
	         OPTION 20 我想前往幻界
			; option 50 #我想前往云荒域
	         OPTION 30 #稍后再来         
	         TALK
	         RETURN   
		else
			 DIALOG 1
	         ADD_TALK $_npc$:
	         ADD_TALK 　　如果您想外出旅行，我可以为您提供服务，想去哪里都可以哦！
	         OPTION 10 #我想使用传送服务
	         OPTION 20 我想前往幻界
	         OPTION 30 #稍后再来         
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
	MSG_PLAYER 3 1 对不起，您现在不能被传送！             
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
;继续聊天
PROC 11
   DIALOG 1
   ADD_TALK $_player$:
   ADD_TALK 　　我也很高兴认识你，请先将沙漏交给我好吗？
   OPTION 12 继续对话
   TALK
ENDPROC
;
;
;继续聊天
PROC 12
   DIALOG 1
   ADD_TALK $_npc$:
   ADD_TALK 　　这就是命运沙漏之一，楼兰的安危就全靠你了。
   OPTION 13 继续对话
   TALK
ENDPROC
;
;
PROC 13
   DIALOG 3
   ADD_TALK $_player$:
   ADD_TALK 　　放心吧。
   CALL 14
   TALK
ENDPROC
;
;
PROC 14
   ADD_GOODS 3563 1 1
   IF 1 != 0
      MSG_PLAYER 2 2 对不起，由于您物品栏中的消耗品存放空间已满，因此无法获得任务奖励“命运沙漏一”。请在整理物品栏后再试一次！                
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
   ADD_TALK $_npc$：
   ADD_TALK 　　只有飞升后的勇士才能进入幻界，你准备好了吗？
   OPTION 22 #准备好了
   OPTION 30 #稍后再来
   TALK
ENDPROC 
;
;
PROC 22
     GOTO_MAP 86 2614 3554 100 1
     IF 1 =  0
        MSG_PLAYER 3 2 欢迎来到幻界天镜域！
     ELSE 
        MSG_PLAYER 3 2 传送失败！
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
   ADD_TALK $_npc$：
   ADD_TALK 　　大漠风情，拥有粗旷的美丽。来往此地做生意的商人络绎不绝！
   add_talk 　　不过不管怎样，没有我签发的幻想通行证就不能进出楼兰！那个来自未来的intel天使也不能例外！
   OPTION 46 #领取幻想通行证
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
			    ADD_TALK 　　楼兰的通行证已经给你了，所有的通行证你已经收齐了，带回去给intel天使吧！
				talk
			else
				msg_player 3 2 领取幻想通行证失败，请整理物品栏后再试！
			endif
		else
			DIALOG 3
		    ADD_TALK $_npc$:
		    ADD_TALK 　　这个小英雄，虽然你乐于助人的精神是很值得嘉奖啦，不过你要找的人似乎不是我哦！看清楚任务提示吧。
			talk
		endif
	else
		DIALOG 3
	    ADD_TALK $_npc$:
	    ADD_TALK 　　听说那位intel天使已经离开幻想了，你就别忙和了。
		talk
	endif
endproc 
;---------
proc 50
	get_level 5
	if 5 >= 100
		goto_map 143 5874 3843 10 1
		if 1 = 0
			msg_player 3 2 进入云荒域！
		else
			msg_player 3 2 进入云荒域失败，请稍后再试！
		endif
	else
		msg_player 3 2 只有100级以上的勇士才能进入云荒域！
	endif
endproc 