rollout  anchoridmgrrollout  "Ogre anchorid 管理器" 
(
	listbox anchorlist  "Anchors" height:18 width:240
	group "anchor id 管理"
	(
		dropDownList objectslist "选择物体" pos:[10,290] width:110 height:20 items:(for o in objects collect o.name)
		spinner spanchorid "Anchor ID" range:[1,99999,1] scale:1 pos:[40,340] width:80 height:20
		button addanchorbtn "添加anchorid" pos:[160,286] width:80 height:20
		button removeanchorbtn "删除anchorid" pos:[160,314] width:80 height:20
		button refreshbtn "刷新" pos:[160, 340] width:80 height:20		
	)

	fn Update =
	(
		allitem=#()
		anchorlist.items = #()
		anchoridlist = #()
		for o in objects do
		(
			mask = getuserprop o "anchorid"
			if (mask != undefined) and (mask != "undefined") do
			(
				for n=1 to anchoridlist.count do
				(
					if (mask == anchoridlist[n]) do
					(
						message = "添加了有冲突的anchorid"
						messagebox message title:"Add anchorid Error!"
						exit
					)
				)
			
				append anchoridlist mask	
				name = o.name
				Mstring = name as string + "                        " + mask as string
				append allitem Mstring
			)
		)
		--print allitem			
		for q=1 to allitem.count do
		(
			if q==1 then
			(
				inform="Name                       Anchorid"
				anchorlist.items = append anchorlist.items inform
			)
			itemq = allitem[q]
			anchorlist.items = append anchorlist.items itemq
		)	

	)

	on addanchorbtn pressed do
	(
		b = objectslist.selected as string
		for o in objects do
		(
			if (o.name == b) then
			(
				setuserprop o "anchorid" (spanchorid.value as integer)
				exit
			)
		)
		Update()
	)

	on removeanchorbtn pressed do
	(
		b = objectslist.selected as string
		for o in objects do
		(
			if (o.name == b) then
			(
				mask = getuserprop o "anchorid"
				if (mask != undefined) and (mask != "undefined") do
				(			
					setuserprop o "anchorid" undefined
				)
				exit
			)
		)
		Update()	
	)

	on refreshbtn pressed do
	(
		Update()
	)
)
createdialog anchoridmgrrollout width:260 height:380
