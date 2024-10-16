;飞扬城六合彩小姐
;
;
PROC 1
    GET_SEX 827
    IF 827 = 1
    	DIALOG 1
    	ADD_TALK $_npc$：
    	ADD_TALK 嗨！帅哥！你说我美不美？
    	OPTION 10 你是我见过最漂亮的女孩子啦
    	OPTION 20 还算可以吧
    	TALK
    ELSE
    	DIALOG 3
    	ADD_TALK $_npc$:
    	ADD_TALK 我最讨厌长得比我漂亮的女孩子了，我可不想给女孩子加名誉。
    	TALK 
    ENDIF
ENDPROC 
;
;
;继续对话
PROC 10
   IS_CAPTAIN 833
   IF 833 != 0
    	DIALOG 3
    	ADD_TALK $_npc$:    
    	ADD_TALK 真的吗？你真好！我给你加点名誉值吧！
    	ADD_RAND_HONOR 10
    	TALK
    ELSE
    	DIALOG 1
    	ADD_TALK $_npc$:
    	ADD_TALK 真的吗？你真好！我给你加点名誉值吧！哇，原来你是位队长，我也可以给你的队员加名誉值哦！
    	OPTION 30 只给我就好了！
    	OPTION 40 给我的队员一起加吧！
    	TALK	
    ENDIF 	
ENDPROC
;
;    
;选择了解玩法
PROC 20
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK 什么？人家长得这么漂亮，你居然只说还可以，真气死我啦，哼！我要减少你的名誉值！
    DEL_HONOR 5
    TALK
ENDPROC
;
;    
PROC 30
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK 已经给你加好名誉值啦！只不过你这个队长当得.......
    ADD_CAPTION_HONOR 5
    TALK
ENDPROC
;
;    
PROC 40
    DIALOG 3
    ADD_TALK $_npc$:
    ADD_TALK 已经给你和你的队员加好名誉值啦！
    ADD_TEAM_HONOR 5
    TALK
ENDPROC
