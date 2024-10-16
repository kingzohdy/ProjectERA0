;祭祖神殿 ,先遣队员2双炮机器人
proc 2
	strvar ret defid memid hp mapdef map x y
	GET_MON_INFO ret defid memid hp mapdef map x y
	
	NEW_TMP_ITEM 1420004 @x @y 0 0 0 1
endproc
