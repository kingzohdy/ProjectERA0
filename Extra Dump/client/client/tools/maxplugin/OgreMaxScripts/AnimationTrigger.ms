
rollout ActorTriggerRollout "Active Trigger"
(
	group  "Trigger"
	(
		dropdownlist  ddlTriggers "Triggers:" align:#left enabled:false width:132 height:10
		label lbltrigName "Active Trigger Name:" align:#left enabled:false
		edittext edittrigName "" text:"NEW Trigger" align:#left width:132 enabled:false
		label lbltrigCount "Trigger 0 of 0" enabled:false align:#left offset:[0, 6]
		spinner spnStartFrame "Trigger Frame: " range:[-99999,99999,0] type:#integer align:#left enabled:false
		spinner spnEndFrame "Trigger Type: " range:[-99999,99999,0] type:#integer align:#left enabled:false	

		button btnPrev "Previous" across:2 align:#center width:65 enabled:false
		button btnNext "Next" across:2 align:#center width:65 enabled:false
		
		button btnCreate "Append New Trigger" align:#center width:132 enabled:false
		button btnDestroy "Delete Current Trigger"  align:#center width:132 enabled:false
	)
	
	function UpdateTriggerUI =
	(
		print "Updating Trigger UI..."
		print LayerRoot

		if LayerRoot != undefined do
		(
			if ActiveTriggerNoteTrack  == undefined do
			(
				print "Active Note Track is undefined.. getting from layer root"
				ActiveTriggerNoteTrack  = GetActorTriggerNoteTrack LayerRoot
			)
			print ActiveTriggerNoteTrack 
			if ActiveTrigger == undefined do
			(
				print "ActiveTrigger == undefined"
				ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  1
			)
			
			print "Active Trigger:"
			print ActiveTrigger

			list = #()
			listsize = GetTriggerCount ActiveTriggerNoteTrack
			if listsize != undefined do
			(
				for i = 1 to listsize do
				(
					startKey = GetTriggerKey ActiveTriggerNoteTrack  i
					
					trigname = GetTriggerName startKey
					if trigname != undefined do
					(
						append list trigname
					)
				)
			)

			if listsize > 0 do
			(
				ddlTriggers.items = list
			)
			if listsize == 0 do
			(
				ddlTriggers.items = #()
			)
		

		)

		if ActiveTrigger != undefined do
		(
			lbltrigName.enabled = true
			lbltrigCount.enabled = true
			edittrigName.enabled = true
			edittrigName.text = GetTriggerName ActiveTrigger
			lbltrigCount.enabled = true
			trigCount = GetTriggerCount ActiveTriggerNoteTrack 
			trigIndex = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
			strtrigCount = "Trigger " + trigIndex as string + " of " + trigCount as string
			lbltrigCount.caption = strtrigCount
			spnStartFrame.enabled = true
			spnEndFrame.enabled = true
			spnStartFrame.value = ActiveTrigger.time
			spnEndFrame.value = GetTriggerType ActiveTrigger
			btnCreate.enabled = true
			
			if trigCount > 1 then 
				btnDestroy.enabled = true 
			else 
				btnDestroy.enabled = false
			
			if trigIndex == 1 then 
				btnPrev.enabled = false 
			else 
				btnPrev.enabled = true
				
			if trigIndex == trigCount then 
				btnNext.enabled = false 
			else 
				btnNext.enabled = true

			ddlTriggers.enabled = true
			ddlTriggers.selection = trigIndex
		)
		if ActiveTrigger == undefined do
		(
			lbltrigName.enabled = false
			lbltrigCount.enabled = false
			edittrigName.enabled = false
			edittrigName.text = ""
			lbltrigCount.enabled = false
			trigCount = 0
			trigIndex = 0
			strtrigCount = "Trigger " + trigIndex as string + " of " + trigCount as string
			lbltrigCount.caption = strtrigCount
			spnStartFrame.enabled = false
			spnEndFrame.enabled = false
			spnStartFrame.value = 0
			spnEndFrame.value = 0
			btnCreate.enabled = false	
			btnDestroy.enabled = false 
			btnPrev.enabled = false 
			btnNext.enabled = false 
			ddlTriggers.items = #()
			ddlTriggers.enabled = false
		)

	)
	
	
	function AddTrigger nt name start end type = 
	(
		if nt == undefined then return undefined
		NewTrigger = SetTriggerKey nt name start type
		--NewTriggerEnd = SetSimpleEndKey nt end
		return NewTrigger
	)
	
	function RemoveTrigger nt StartKey=
	(
		if nt == undefined then return undefined
		
		keyArray = #()
		for key in nt.keys do
		(
			if key.time != startKey.time do
			(
				append keyArray key
			)
		)
		
		ntnew = notetrack "ActorTriggerManager2"
		addNoteTrack LayerRoot ntnew
		for key in keyArray do
		(
			newKey = CreateNoteKey ntnew key.time
			newKey.value = key.value
		)

		deleteNoteTrack LayerRoot nt
		ActiveTriggerNoteTrack  = ntnew
		ntnew.name = "ActorTriggerManager"
	)
	
	on ActorTriggerRollout open do
	(
		UpdateTriggerUI()
	)
	on ddlTriggers selected trignum do
	(
		index = trignum
		ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  index
		--ActiveTriggerEnd = GetEndKey ActiveTriggerNoteTrack  ActiveTrigger
		UpdateTriggerUI()
	)
	on edittrigName changed text do
	(
		if ActiveTrigger == undefined then return true
		name = GetTriggerName ActiveTrigger
		if name == text then return true
		
		local finaltext = ""
		textwords = filterstring text " \t\n#@!%^&*()+=[]{}\|:;\"'<>,.?/~`"
		print textwords
		bHadToReviseText = false
		if textwords.count == 1 then finaltext = textwords[1]
		if textwords.count > 1 do
		(			
			finaltext = ""
			for i = 1 to textwords.count do
			(
				if i == textwords.count then 
					finaltext = finaltext + textwords[i] 
				else
					finaltext = finaltext + textwords[i] + "_" 
			)
			print "Had to revise text: "
			print finaltext
			bHadToReviseText = true
		)
		if text[text.count-1] == ' ' then finaltext = finaltext + "_"
	
		ddlTriggers.selected = finaltext
		print finaltext
		items = filterstring ActiveTrigger.value " "
	
		value = ""
		ignoreindex = 0
		for i = 1 to items.count do
		(
			item = items[i]
			bAppend = true
			if (EqualsNoCase item "-name") then bAppend = false
			if i == ignoreindex then continue 
			item = item + " "
			if bAppend == true then value = value + item
			if bAppend == false do
			(
				value = value + "-name " + finaltext + " "
				ignoreindex = i + 1
			)
		)
		print value
		ActiveTrigger.value = value
	
		if bHadToReviseText then UpdateTriggerUI()
	)

	on btnPrev pressed do
	(
		index = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
		if index > 1 do
		(
			index = index - 1
			ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  index
			--ActiveTriggerEnd = GetEndKey ActiveTriggerNoteTrack  ActiveTrigger
			UpdateTriggerUI()
		)
	)
	on btnNext pressed do
	(
		index = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
		count = GetTriggerCount ActiveTriggerNoteTrack 
		if index< count do
		(
			index = index + 1
			ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  index
			--ActiveTriggerEnd = GetEndKey ActiveTriggerNoteTrack  ActiveTrigger
			UpdateTriggerUI()
		)
	)
	on btnCreate pressed do
	(
		index = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
		index = index + 1
		start = 0
		end = 0
		
		nexttrig = GetTriggerKey ActiveTriggerNoteTrack  index
		if nexttrig == undefined do
		(
			start = ActiveTrigger.time+1
			--end = ActiveTriggerEnd.time + 10
		)
		if nexttrig != undefined do
		(
			start = ActiveTrigger.time + 1
			--end = nexttrig.time - 1
			---if nexttrig.time == (ActiveTriggerEnd.time+1) or (start == end) do
			--(
			--	message = "There is a Trigger already immediately following this Trigger!"
			--	messagebox message title:"Append error!"
			--	return ok
			--)
		)
		
		strNewtrigName = strBlanktrig + "_AT_" + (start as string) 
		NewTrigger = AddTrigger ActiveTriggerNoteTrack  strNewtrigName start end 0
		if NewTrigger != undefined do
		(
			print "btnCreate pressed..."
			ActiveTrigger = NewTrigger
			print ActiveTrigger
			--ActiveTriggerEnd = GetEndKey ActiveTriggerNoteTrack  ActiveTrigger
			UpdateTriggerUI()
		)
	)
	on btnDestroy pressed do
	(
		RemoveTrigger ActiveTriggerNoteTrack  ActiveTrigger
		ActiveTrigger = undefined 
		UpdateTriggerUI()
	)
	
	on spnStartFrame changed frame do
	(
		trigIndex = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
		ActiveTrigger.time = frame
		ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  trigIndex
		--ActiveTriggerEnd = GetEndKey ActiveTriggerNoteTrack  ActiveTrigger
		--setsliderTime ActiveTrigger.time
		UpdateTriggerUI()
		bAdd = true
	)
	
	on spnEndFrame changed frame do
	(
	    trigIndex = GetTriggerKeyIndex ActiveTriggerNoteTrack  ActiveTrigger
		ActiveTrigger = GetTriggerKey ActiveTriggerNoteTrack  trigIndex

		value = ""
		value = value + SetTriggerType ActiveTrigger				
		ActiveTrigger.value = value + (frame as string)
						
		--setsliderTime ActiveTrigger.time
		
		UpdateTriggerUI()
	)
)
