;
;script filename: npc1
;
PROC    1
	DIALOG 1
	ADD_TALK	俺读得书少，老爸叫俺出来买卖杂货
	GET_SEX 1
	IF 1 = 1
		ADD_TALK	大哥，你要买东西还是卖东西？
	ELSE
		ADD_TALK	大姐，你要买东西还是卖东西？
	ENDIF
	OPTION 21 买
	OPTION 22 卖
   TALK
ENDPROC

PROC 21
	START_DEAL 1
	FINISH
ENDPROC

PROC 22
	START_DEAL 0
	FINISH
ENDPROC
