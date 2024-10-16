;阿力芭芭

proc 1
	DIALOG 1
	add_talk $_npc$:
	add_talk #b#b亲，为了给亲们创造更多的致富机会，我可是倾力提供大量铺位的出租哟！
	add_talk #b#b租赁一间店铺做生意，除了商品更好被顾客浏览，还能享受【#c186506代理服务#n】呢！这样亲们就能随时做生意发大财啦！亲。
	add_talk #b#b不过万事开头难，亲们只要勤奋努力，加上对市场敏锐的洞察力，总有一天也会像我光彩照人哟！亲！还等什么呢！
	add_talk #b#b
	add_talk #b#c186506人物达到20级后可在此租赁【店铺】出售商品#n
	
	strvar LV
	GET_PLAYER_BASE_ATT 1 1 LV 1
	
	if @LV >= 20
		option 101 #【打开店铺】
		option 102 查阅《店铺宝典》
	endif
	
	talk
endproc

proc 101
	strvar ret unl
	PKG_IS_UNLOCK ret unl
	if ret = 0 and unl = 1
		OPEN_STORE_UI 0
	endif
endproc

proc 102
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#c005ebe《店铺宝典》
	add_talk #b#b#b#b#b#b#b#b#b#b目录#n
	add_talk #b#b全面剖析店铺要领，快速掌握致富之路。
	
		option 201 第一章《代理服务》
		option 202 第二章《店铺资产》
		option 203 第三章《店铺货架》
		option 204 第四章《店铺信誉》
		option 205 第五章《店铺宣传》
		option 1 我已经掌握店铺要领
	
	talk
endproc

proc 201
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe 第一章
	add_talk #b#b#b#b#b#b#b#b《代理服务》#n
	add_talk #b#b#b#b#b#b店铺无偿提供代理服务
	add_talk #b#b#b卖家离线后店铺仍可保持正常运作。
	add_talk #b#b#b#b#b#b#b#b★代理范畴★
	add_talk #b#b#b#b代理商品出售，代理商品收入。
	add_talk #b#b#b#b收纳租赁费时，店铺代理缴纳。
	add_talk #b#b#b#b负资产#c18650648小时#n，店铺代理收店。
	add_talk #b#b#b#b负资产收店后，代理退还商品。
	
		option 102 返回 - 《店铺事项》目录
	
	talk
endproc

proc 202
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe 第二章
	add_talk #b#b#b#b#b#b#b#b《店铺资产》#n
	add_talk #b#b#b#b资产关乎着店铺的运作与发展。
	add_talk #b卖家可自行存入资产，维持店铺长期的运作。
	add_talk #b成功出售商品的收入，将自动存入店铺资产。
	add_talk #b卖家可根据自身情况，自行从资产取回资金。
	add_talk #b#b
    add_talk #b#b#b#b#b#b#b ★店铺负资产★
	add_talk 资产不足支付租赁费时，店铺将处于负资产运作。
	add_talk #b#b#b负资产#c18650648小时#n后，店铺将自行收店。
	add_talk #b#b#b店铺自行收店后，由邮件退还商品。
	add_talk #b#b#b#b#b#b#b ★店铺租赁费★
	add_talk #b开店后每间隔#c18650660分钟#n，资产自动缴纳租赁费。
	add_talk #b#b#b#b 店铺的租赁费随市场汇率浮动
	add_talk #b#b#b#b#b#b#b ★商品保管费★
	add_talk #b#b售出商品 #c1865065%的收入 #n，用作商品的保管费。
	
		option 301 如何【存取资产】
		option 102 返回 - 《店铺事项》目录
	
	talk
endproc

proc 301
	DIALOG 1
	add_talk 资产存取方法：
	add_talk #b#b点击店铺面板的【#c186506资产#n】按钮，即可进行资产存取的相关操作。
	add_talk #b#b
	add_talk 存取注意事项：
	add_talk #b#b存取资产时，请确保身上或资产有足够存取的银币。
	
		option 202 返回 - 第二章《店铺资产》
	
	talk
endproc

proc 203
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe 第三章
	add_talk #b#b#b#b#b#b#b#b《店铺货架》#n
	add_talk #b#b#b 货架关乎着店铺可上架商品的容量。
	add_talk #b#b#b#b#b#b每间店铺附带8格货架。
    add_talk #b#b#b#b#b#b#b ★扩充货架★
	add_talk #b卖家可自行扩充货架，增加店铺商品的容量。
	add_talk #b#b#b店铺的货架越多，租赁费额度越高。
	add_talk #b#b#b#b#b#b#b ★使用权限★
	add_talk #b 扩充的货架可在当前租赁的店铺永久使用。
	add_talk #b#b#b#b#b#b#b ★拆除货架★
	add_talk #b卖家可自行拆除货架，调控店铺租赁费额度。
	
		option 302 如何【扩充货架】
		option 303 如何【拆除货架】
		option 102 返回 - 《店铺事项》目录
	
	talk
endproc

proc 302
	DIALOG 1
	add_talk 货架扩充方法：
	add_talk #b#b点击店铺面板的【#c186506扩充货架#n】按钮，即可进行扩充货架的相关操作。
	add_talk #b#b
	add_talk 扩充注意事项：
	add_talk #b#b扩充货架时需要提供【#c186506货架扩充合同#n】，请确保身上已携带相应数量的【#c186506货架扩充合同#n】。
	add_talk #b#b时间城的【布嘉兰】提供【#c186506货架扩充合同#n】的出售。
	
		option 203 返回 - 第三章《店铺货架》
	
	talk
endproc

proc 303
	DIALOG 1
	add_talk 货架拆除方法：
	add_talk #b#b点击店铺面板的【#c186506拆除货架#n】按钮，即可进行拆除货架的相关操作。
	add_talk #b#b
	add_talk 拆除注意事项：
	add_talk #b#b拆除货架时，请确保拆除的货架上无上架的商品。
	
		option 203 返回 - 第三章《店铺货架》
	
	talk
endproc

proc 204
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe 第四章
	add_talk #b#b#b#b#b#b#b#b《店铺信誉》#n
	add_talk #b#b#b#b#b信誉关乎着店铺的关注度。
    add_talk #b#b#b#b#b#b#b ★信誉优势★
	add_talk #b#b#b#b店铺信誉越高，店铺排名越高。
	add_talk #b#b#b#b店铺排名越高，店铺客流越高。
	add_talk #b#b#b#b#b#b#b ★赢取信誉★
	add_talk #b#b#b#b#b 货架数量影响店铺信誉。
	add_talk #b#b#b#b#b 开店时长影响店铺信誉。
	add_talk #b#b#b#b#b 收入总额影响店铺信誉。
	
		option 102 返回 - 《店铺事项》目录
	
	talk
endproc

proc 205
	DIALOG 1
	add_talk #b#b#b#b#b#b#b#b#b#c005ebe 第五章
	add_talk #b#b#b#b#b#b#b#b《店铺宣传》#n
	add_talk #b#b#b#b#b宣传关乎着店铺的知名度。
    add_talk #b#b#b#b#b#b#b ★广告宣传★
	add_talk #b#b 卖家可在店铺的广告频道中宣传店铺。
	add_talk #b#b#b#b#b#b#b ★广告优势★
	add_talk #b#b 顾客可通过卖家的广告直接进入店铺。
	
		option 304 如何【宣传店铺】
		option 102 返回 - 《店铺事项》目录
	
	talk
endproc

proc 304
	DIALOG 1
	add_talk 店铺宣传方法：
	add_talk #b#b点击店铺面板的【#c186506宣传店铺#n】按钮，即可在店铺的广告频道中宣传店铺。
	add_talk #b#b
	add_talk 宣传注意事项：
	add_talk #b#b宣传店铺时需要用到【#c186506小喇叭#n】，请确保身上已携带足够的【#c186506小喇叭#n】。
	
		option 205 返回 - 第五章《店铺宣传》
	
	talk
endproc
