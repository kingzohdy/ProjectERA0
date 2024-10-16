
	local strNone = "NONE"
	local strBlankSeq = "EMPTY_SEQUENCE"
	local AllActorRoots = #()
	local ActorRoot = undefined
	local LayerRoot = undefined
	local AllLayerRoots = #()
	local lastTime = 0.0
	local ActiveNoteTrack = undefined
	local ActiveSequence = undefined
	local ActiveSequenceEnd = undefined
	
	local strBlankMaterial = "EMPTY_MTLANI_SEQ"
	local ActiveMaterialNoteTrack = undefined
	local ActiveMaterialSequence = undefined
	local ActiveMaterialSequenceEnd = undefined
	local AllMaterialMeshes = #()

	local strBlankTrig = "EMPTY_TRIGGER"
	local ActiveTrigger = undefined
	local ActiveTriggerType = undefined
	local ActiveTriggerNoteTrack = undefined
	
	local ListBonesName = #()

	include "OgreMaxScripts\AnimationHelpers.ms"

	include "OgreMaxScripts\AnimationTrigger.ms"
	
	rollout OgreActorSequenceRollout "Active Sequence"
	(
		group  "Sequence"
		(
			dropdownlist  ddlSequences "Sequences:" align:#left enabled:false width:132 height:10
			label lblSeqName "Active Sequence Name:" align:#left enabled:false
			edittext editSeqName "" text:"NEW SEQUENCE" align:#left width:132 enabled:false
			label lblSeqCount "Sequence 0 of 0" enabled:false align:#left offset:[0, 6]
			--checkbox cbLooping "Loop Sequence" offset:[0,5] enabled:false
			checkbox cbExpOnly "Only export this sequence" offset:[0,5] enabled:false
			spinner spnLoopMode "Loop Mode: " range:[0,2,0] type:#integer align:#left enabled:false
			spinner spnSeqID "Sequence ID: " range:[-99999,99999,0] type:#integer align:#left enabled:false
			spinner spnStartFrame "Start Frame: " range:[-99999,99999,0] type:#integer align:#left enabled:false
			spinner spnEndFrame "End Frame: " range:[-99999,99999,0] type:#integer align:#left enabled:false	

			button btnPrev "Previous" across:2 align:#center width:65 enabled:false
			button btnNext "Next" across:2 align:#center width:65 enabled:false
			
			button btnCreate "Append New Sequence" align:#center width:132 enabled:false
			button btnDestroy "Delete Current Sequence"  align:#center width:132 enabled:false
		)
		group "Export Select bones Animation"
		(
			--checkbox ExportSelectBones "Export Select Bones Enable" align:#left offset:[0, 5] enabled:true
			MultiListBox SelectedBonesList "SelectedBones" align:#center width:132 enabled:false items:(for o in selection collect o.name)
			button btnAddBones "Add" align:#center width:55 across:2 
			button btnRemBones "Remove" align:#center width:55	
		)
		
		function UpdateSequenceUI =
		(
			print "Updating sequence UI..."
			print LayerRoot
			
			if LayerRoot != undefined do
			(
				if ActiveNoteTrack == undefined do
				(
					print "Active Note Track is undefined.. getting from layer root"
					ActiveNoteTrack = GetActorNoteTrack LayerRoot
				)
				print ActiveNoteTrack
				if ActiveSequence == undefined do
				(
					print "ActiveSequence == undefined"
					ActiveSequence = GetStartKey ActiveNotetrack 1
				)
				
				if ActiveSequenceEnd == undefined do
				(
					print "ActiveSequenceEnd == undefined"
					ActiveSequenceEnd = GetEndKey ActiveNotetrack ActiveSequence
					print "Active Sequence End:"
					print ActiveSequenceEnd
				)
				
				print "Active Sequence:"
				print ActiveSequence
				print "Active Sequence End:"
				print ActiveSequenceEnd

				list = #()
				listsize = GetSequenceCount ActiveNoteTrack
				if listsize != undefined do
				(
					for i = 1 to listsize do
					(
						startKey = GetStartKey ActiveNoteTrack i
						seqname = GetSequenceName startKey
						if seqname != undefined do
						(
							append list seqname
						)
					)
				)

				if listsize > 0 do
				(
					ddlSequences.items = list
				)
				if listsize == 0 do
				(
					ddlSequences.items = #()
				)
						
				ListBonesName = #()
				SelectedBonesList.items = #()
				listbonesnamesize = GetSequenceCount ActiveNoteTrack
				if listbonesnamesize != undefined do
				(
					for i = 1 to listbonesnamesize do
					(
						startKey = GetStartKey ActiveNoteTrack i
						bonesname = GetSequenceSelectBonesName startKey
							
						if bonesname != undefined then
						(
							append ListBonesName bonesname
						)
					)
				)
			)

			if ActiveSequence != undefined do
			(
				lblSeqName.enabled = true
				lblSeqCount.enabled = true
				editSeqName.enabled = true
				spnSeqID.enabled = true
				spnSeqID.value = GetSequenceID ActiveSequenceEnd
				editSeqName.text = GetSequenceName ActiveSequence
				lblSeqCount.enabled = true
				seqCount = GetSequenceCount ActiveNoteTrack
				seqIndex = GetStartKeyIndex ActiveNoteTrack ActiveSequence
				strSeqCount = "Sequence " + seqIndex as string + " of " + seqCount as string
				lblSeqCount.caption = strSeqCount
				cbExpOnly.enabled = true
				cbExpOnly.checked = GetExpOnly ActiveSequence		
				
				spnLoopMode.enabled = true
				if(GetSequenceLoop ActiveSequence == undefined) then
					spnLoopMode.value = 0
				else
					spnLoopMode.value = GetSequenceLoop ActiveSequence
					
				spnStartFrame.enabled = true
				spnEndFrame.enabled = true
				spnStartFrame.value = ActiveSequence.time
				spnEndFrame.value = ActiveSequenceEnd.time
				btnCreate.enabled = true
				
				if seqCount > 1 then 
					btnDestroy.enabled = true 
				else 
					btnDestroy.enabled = false
				
				if seqIndex == 1 then 
					btnPrev.enabled = false 
				else 
					btnPrev.enabled = true
					
				if seqIndex == seqCount then 
					btnNext.enabled = false 
				else 
					btnNext.enabled = true

				ddlSequences.enabled = true
				ddlSequences.selection = seqIndex
				
				btnAddBones.enabled = true
				btnRemBones.enabled = true
				SelectedBonesList.enabled = true
				SelectedBonesList.items = #()				
				--ExportSelectBones.enabled = true
				
				if(ListBonesName[seqIndex] != OK and ListBonesName[seqIndex] != undefined) then
				(
					--ExportSelectBones.checked = true
					
					names = filterString ListBonesName[seqIndex] "@"
					print names
					print "seqindex:"
					print seqIndex
					for i=1 to names.count do
					(
						n = filterString names[i] "#"
						s = ""
						for j=1 to n.count do
						(
							s = s + n[j]
							if(j != n.count) do
								s = s + " "							
						)
						names[i] = s
					)
					
					for o=1 to names.count do
					(
						SelectedBonesList.items = append SelectedBonesList.items (names[o] as string)
					)
				)
			)
			if ActiveSequence == undefined do
			(
				lblSeqName.enabled = false
				lblSeqCount.enabled = false
				editSeqName.enabled = false
				editSeqName.text = ""
				spnSeqID.enabled = false
				spnSeqID.value = 0
				lblSeqCount.enabled = false
				seqCount = 0
				seqIndex = 0
				strSeqCount = "Sequence " + seqIndex as string + " of " + seqCount as string
				lblSeqCount.caption = strSeqCount
				spnLoopMode.enabled = false
				spnLoopMode.value = 0				
				spnStartFrame.enabled = false
				spnEndFrame.enabled = false
				spnStartFrame.value = 0
				spnEndFrame.value = 0
				btnCreate.enabled = false	
				btnDestroy.enabled = false 
				btnPrev.enabled = false 
				btnNext.enabled = false 
				ddlSequences.items = #()
				ddlSequences.enabled = false
				
				--ExportSelectBones.enabled = false
				--ExportSelectBones.checked = false
				SelectedBonesList.enabled = false
				btnAddBones.enabled = false
				btnRemBones.enabled = false
				SelectedBonesList.items = #()
			)

		)
		
		function CanAddBone obj =
		(
			bCanAdd = true
			
			for n=1 to SelectedBonesList.items.count do
			(
				if SelectedBonesList.items[n] == obj then 
				(
					bCanAdd = false
				)
			)
			
			return bCanAdd
		) 
		
		function UpdateSelectedBones =
		(
			bonenames = ""
			for n=1 to SelectedBonesList.items.count do
			(
				names = filterstring SelectedBonesList.items[n] " "
				s = ""
				for i=1 to names.count do
				(
					s = s + names[i]
					if(i != names.count) do
						s = s + "#"
				)		
			
				--bonenames = bonenames + SelectedBonesList.items[n]
				bonenames = bonenames + s
				
				if n != SelectedBonesList.items.count do
					bonenames = bonenames + "@"
			)
		
			--print ActiveMaterialSequence
			
			if ActiveSequence == undefined then return true

			name = GetSequenceName ActiveSequence

			items = filterstring ActiveSequence.value " "
			value = ""
			bAppend = true

			for i = 1 to items.count do
			(
				item = items[i]
				if (EqualsNoCase item "-selectbonesname") then
				(
					bAppend = false 
				)
				
				item = item + " "
				
				if bAppend == true then value = value + item
			)
			value = value + "-selectbonesname"
			value = value + " "
			value = value + bonenames
			
			ActiveSequence.value = value

			print ActiveSequence.value
		)	
		
		on btnAddBones pressed do
		(	
			temp = (for o in selection collect o.name)
			
			--sort temp

			for n=1 to temp.count do
			(		
				bResult = CanAddBone (temp[n] as string)
				if bResult == true do
				(
					SelectedBonesList.items = append SelectedBonesList.items (temp[n] as string)
					sort SelectedBonesList.items
				)
			)
			
			UpdateSelectedBones()
		)
		
		on btnRemBones pressed do
		(
			i=0
			for n in SelectedBonesList.selection do
			(
				SelectedBonesList.items = deleteitem SelectedBonesList.items (n-i)
				i = i+1
			)
			
			UpdateSelectedBones()
		)
		
		function AddSequence nt name start end loopMod = 
		(
			if nt == undefined then return undefined
			NewSequence = SetSimpleStartKey nt name start loopMod
			NewSequenceEnd = SetSimpleEndKey nt end 0
			return NewSequence
		)
		
		function RemoveSequence nt StartKey EndKey=
		(
			if nt == undefined then return undefined
			
			keyArray = #()
			for key in nt.keys do
			(
				if key.time < startKey.time or key.time > endkey.time do
				(
					append keyArray key
				)
			)
			
			ntnew = notetrack "ActorManager2"
			addNoteTrack LayerRoot ntnew
			for key in keyArray do
			(
				newKey = CreateNoteKey ntnew key.time
				newKey.value = key.value
			)

			deleteNoteTrack LayerRoot nt
			ActiveNoteTrack = ntnew
			ntnew.name = "ActorManager"
		)
		
		function CanSetActiveSequenceTime start end =
		(
			if start > end then return false
			if start >= ActiveSequenceEnd.time then return false
			if end <= ActiveSequence.time then return false
			
			index = GetStartKeyIndex ActiveNoteTrack ActiveSequence
			if index > 1 do
			(
				PrevSequence = GetStartKey ActiveNoteTrack (index-1)
				PrevSequenceEnd = GetEndKey ActiveNoteTrack PrevSequence
				if PrevSequenceEnd.time >= start then return false
			)
			if index < GetSequenceCount ActiveNoteTrack do
			(
				NextSequence = GetStartKey ActiveNoteTrack (index+1)
				if NextSequence.time <= end then return false
			)
			return true
		)
		
		on OgreActorSequenceRollout open do
		(
			UpdateSequenceUI()
		)
		on ddlSequences selected seqnum do
		(
			index = seqnum
			ActiveSequence = GetStartKey ActiveNoteTrack index
			ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
			UpdateSequenceUI()
		)
		on editSeqName changed text do
		(
			if ActiveSequence == undefined then return true
			name = GetSequenceName ActiveSequence
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
		
			ddlSequences.selected = finaltext
			print finaltext
			items = filterstring ActiveSequence.value " "
		
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
			ActiveSequence.value = value
		
			if bHadToReviseText then UpdateSequenceUI()
		)
		
		on cbExpOnly changed state do
		(
			name = GetSequenceName ActiveSequence		
			items = filterstring ActiveSequence.value " "
		
			value = ""
			for i = 1 to items.count do
			(
				item = items[i]
				bAppend = true
				if (EqualsNoCase item "-exponly") then bAppend = false 
			    item = item + " "
		
				if bAppend == true then value = value + item
			)
			
			if cbExpOnly.checked == true then 
				value = value + " -exponly"
			ActiveSequence.value = value
		)
		on spnLoopMode changed frame do
		(
			name = GetSequenceName ActiveSequence		
			items = filterstring ActiveSequence.value " "
		
			value = ""
			bLoopAdded
			for i = 1 to (items.count - 1) do
			(
				item = items[i]
				bAppend = true
				if (EqualsNoCase item "-loop") then 
				(
					bAppend = false
					i = i + 1 
				)
			    item = item + " "
		
				if bAppend == true then value = value + item
			)
			
			value = value + " -loop "
			value = value + (frame as string)
			
			ActiveSequence.value = value			
		)

		on btnPrev pressed do
		(
			index = GetStartKeyIndex ActiveNoteTrack ActiveSequence
			if index > 1 do
			(
				index = index - 1
				ActiveSequence = GetStartKey ActiveNoteTrack index
				ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
				UpdateSequenceUI()
			)
		)
		on btnNext pressed do
		(
			index = GetStartKeyIndex ActiveNoteTrack ActiveSequence
			count = GetSequenceCount ActiveNoteTrack
			if index< count do
			(
				index = index + 1
				ActiveSequence = GetStartKey ActiveNoteTrack index
				ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
				UpdateSequenceUI()
			)
		)
		on btnCreate pressed do
		(
			index = GetStartKeyIndex ActiveNoteTrack ActiveSequence
			index = index + 1
			loopMod = spnLoopMode.value
			
			start = 0
			end = 0
			
			nextSeq = GetStartKey ActiveNoteTrack index
			if nextSeq == undefined do
			(
				start = ActiveSequenceEnd.time+1
				end = ActiveSequenceEnd.time + 10
			)
			if nextSeq != undefined do
			(
				start = ActiveSequenceEnd.time + 1
				end = nextSeq.time - 1
				if nextSeq.time == (ActiveSequenceEnd.time+1) or (start == end) do
				(
					message = "There is a sequence already immediately following this sequence!"
					messagebox message title:"Append error!"
					return ok
				)
			)
			
			strNewSeqName = strBlankSeq + "_AT_" + (start as string) 
			NewSequence = AddSequence ActiveNoteTrack strNewSeqName start end loopMod
			if NewSequence != undefined do
			(
				print "btnCreate pressed..."
				ActiveSequence = NewSequence
				print ActiveSequence
				ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
				UpdateSequenceUI()
			)
		)
		on btnDestroy pressed do
		(
			RemoveSequence ActiveNoteTrack ActiveSequence ActiveSequenceEnd
			ActiveSequence = undefined 
			ActiveSequenceEnd = undefined
			UpdateSequenceUI()
		)
		
		on spnStartFrame changed frame do
		(
			bAdd = false
			if CanSetActiveSequenceTime frame ActiveSequenceEnd.time do
			(
				seqIndex = GetStartKeyIndex ActiveNoteTrack ActiveSequence
				ActiveSequence.time = frame
				ActiveSequence = GetStartKey ActiveNoteTrack seqIndex
				ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
				setsliderTime ActiveSequence.time
				UpdateSequenceUI()
				bAdd = true
			)
			
			if bAdd == false do
			(
				message = "Cannot move the Start frame to the specified time.\nThe time either conflicts with a subsequent key or is later than the end key"
				messagebox message title:"Start Frame failure"
				spnStartFrame.value = ActiveSequence.time
				return true
			)
		)
		
		on spnSeqID changed frame do
		(
	
			seqIndex = GetStartKeyIndex ActiveNoteTrack ActiveSequence
			--ActiveSequenceEnd.time = frame
			ActiveSequence = GetStartKey ActiveNoteTrack seqIndex
			ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence

			value = ""
			value = value + SetSequenceID ActiveSequenceEnd				
			ActiveSequenceEnd.value = value + (frame as string)
							
			--setsliderTime ActiveTrigger.time
			
			UpdateSequenceUI()
		)
		
		on spnEndFrame changed frame do
		(
			bAdd = false
			if CanSetActiveSequenceTime ActiveSequence.time frame do
			(
			    seqIndex = GetStartKeyIndex ActiveNoteTrack ActiveSequence
				ActiveSequenceEnd.time = frame
				ActiveSequence = GetStartKey ActiveNoteTrack seqIndex
				ActiveSequenceEnd = GetEndKey ActiveNoteTrack ActiveSequence
				setsliderTime ActiveSequence.time
				
				bAdd = true
				UpdateSequenceUI()
			)
			
			if bAdd == false do
			(
				message = "Cannot move the End frame to the specified time.\nThe time either conflicts with a subsequent key or is earlier than the start key"
				messagebox message title:"Set End Frame failure"
				spnEndFrame.value = ActiveSequenceEnd.time
				return true
			)
		)
	)
	
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------MaterialAnimation Rollout-----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	rollout MaterialSequenceRollout "Active Material Sequence"
	(
		group  "Material_Sequence"
		(
			dropdownlist  ddlSequences "Sequences:" align:#left enabled:false width:132 height:10
			label lblSeqName "Active Sequence Name:" align:#left enabled:false
			edittext editSeqName "" text:"NEW SEQUENCE" align:#left width:132 enabled:false
			label lblSeqCount "Sequence 0 of 0" enabled:false align:#left offset:[0, 6]
			spinner spnLoopMode "Loop Mode: " range:[0,2,0] type:#integer align:#left enabled:false
			spinner spnSeqID "Sequence ID: " range:[-99999,99999,0] type:#integer align:#left enabled:false
			spinner spnStartFrame "Start Frame: " range:[-99999,99999,0] type:#integer align:#left enabled:false
			spinner spnEndFrame "End Frame: " range:[-99999,99999,0] type:#integer align:#left enabled:false	

			button btnPrev "Previous" across:2 align:#center width:65 enabled:false
			button btnNext "Next" across:2 align:#center width:65 enabled:false
			
			button btnCreate "Append New Sequence" align:#center width:132 enabled:false
			button btnDestroy "Delete Current Sequence"  align:#center width:132 enabled:false
		)
		
		group "Material Animation Meshes"
		(
			MultiListBox mbMaterialsMeshes "Meshes:" align:#center width:132 enabled:false items:#()
			--MultiListBox ddActorRoot "Meshes:" items:#("NONE") selection:1 width:120 align:#center 
			button btnAddMeshes "Add" align:#center width:55 across:2 
			button btnRemMeshes "Remove" align:#center width:55	
			
			checkbox uvanim "导出uv动画" align:#left width:132 offset:[0, 5] enabled:true
			checkbox alphaanim "导出alpha动画" align:#left width:132 offset:[0, 5] enabled:true
			checkbox glowanim "导出glow动画" align:#left width:132 offset:[0, 5] enabled:true
		)
		
		on mbMaterialsMeshes selectionEnd do
		(
			selectnum = 0
			for i in mbMaterialsMeshes.selection do
			(
				selectnum = selectnum + 1
			)

			if(selectnum == 1) do
			(
				for i in mbMaterialsMeshes.selection do
				(		
					uvanim.checked = false
					alphaanim.checked = false
					glowanim.checked = false
				
					b = mbMaterialsMeshes.items[i] as string
					print "1111111"
					print b
					mask = undefined
					for o in Geometry do
					(
						if (o.name == b) then
						(
							mask = getuserprop o "MtlAnimMask"
							exit
						)
					)

					if(mask != undefined) do
					(
						if ((bit.and  mask 1) == 1) do
						(
							uvanim.checked = true
						)
						if ((bit.and  mask 2) == 2) do
						(
							alphaanim.checked = true
						)
						if ((bit.and  mask 4) == 4) do
						(
							glowanim.checked = true
						)
					)					
				)
			)
		)
		
		on uvanim changed theState do 
		(
			selectnum = 0
			for i in mbMaterialsMeshes.selection do
			(
				selectnum = selectnum + 1
			)
			
			if (selectnum > 1) then
			(
				message = "请选取一个mesh!"
				messagebox message title:"选取错误!"
			)else if (selectnum == 1) then
			(
				for i in mbMaterialsMeshes.selection do
				(				
					b = mbMaterialsMeshes.items[i] as string
					for o in Geometry do
					(
						if (o.name == b) then
						(
							mask = getuserprop o "MtlAnimMask"
							if (mask == undefined) do
							(
								mask = 0
							)							
							if (uvanim.state == true) then
							(
								mask1 = 1
								mask = bit.or mask mask1
							)else
							(
								mask1 = 6
								mask = bit.and mask mask1
							)		
							setuserprop o "MtlAnimMask" mask
							exit
						)
					)				
				)
			)
		)
		
		on alphaanim changed theState do 
		(
			selectnum = 0
			for i in mbMaterialsMeshes.selection do
			(
				selectnum = selectnum + 1
			)
			
			if (selectnum > 1) then
			(
				message = "请选取一个mesh!"
				messagebox message title:"选取错误!"
			)else if (selectnum == 1) then
			(
				for i in mbMaterialsMeshes.selection do
				(				
					b = mbMaterialsMeshes.items[i] as string
					for o in Geometry do
					(
						if (o.name == b) then
						(
							mask = getuserprop o "MtlAnimMask"							
							if (mask == undefined) do
							(
								mask = 0
							)
							
							if (alphaanim.state == true) then
							(
								mask1 = 2
								mask = bit.or mask mask1
							)else
							(
								mask1 = 5
								mask = bit.and mask mask1
							)						
							setuserprop o "MtlAnimMask" mask
							exit
						)
					)				
				)
			)
		)
		
		on glowanim changed theState do 
		(
			selectnum = 0
			for i in mbMaterialsMeshes.selection do
			(
				selectnum = selectnum + 1
			)
			
			if (selectnum > 1) then
			(
				message = "请选取一个mesh!"
				messagebox message title:"选取错误!"
			)else if (selectnum == 1) then
			(
				for i in mbMaterialsMeshes.selection do
				(				
					b = mbMaterialsMeshes.items[i] as string
					for o in Geometry do
					(
						if (o.name == b) then
						(
							mask = getuserprop o "MtlAnimMask"
							if (mask == undefined) do
							(
								mask = 0
							)
							
							if (glowanim.state == true) then
							(
								mask1 = 4
								mask = bit.or mask mask1
							)else
							(
								mask1 = 3
								mask = bit.and mask mask1
							)
													
							setuserprop o "MtlAnimMask" mask
							exit
						)
					)				
				)
			)
		)
		
		function CanAddMesh obj =
		(
			bCanAdd = true
			
			for n=1 to mbMaterialsMeshes.items.count do
			(
				if mbMaterialsMeshes.items[n] == obj then 
				(
					bCanAdd = false
				)
			)
			
			return bCanAdd
		) 
		
		function UpdateMaterialMeshes =
		(
			meshnames = ""
			for n=1 to mbMaterialsMeshes.items.count do
			(
				names = filterstring mbMaterialsMeshes.items[n] " "
				s = ""
				for i=1 to names.count do
				(
					s = s + names[i]
					if(i != names.count) do
						s = s + "#"
				)		
			
				--meshnames = meshnames + mbMaterialsMeshes.items[n]
				meshnames = meshnames + s
				
				if n != mbMaterialsMeshes.items.count do
					meshnames = meshnames + "@"
			)
		
			print ActiveMaterialSequence
			
			if ActiveMaterialSequence == undefined then return true

			name = GetSequenceName ActiveMaterialSequence

			items = filterstring ActiveMaterialSequence.value " "
			value = ""
			bAppend = true

			for i = 1 to items.count do
			(
				item = items[i]
				if (EqualsNoCase item "-materialmeshes") then
				(
					bAppend = false 
				)
				
				item = item + " "
				
				if bAppend == true then value = value + item
			)
			value = value + "-materialmeshes"
			value = value + " "
			value = value + meshnames
			
			ActiveMaterialSequence.value = value

			print ActiveMaterialSequence.value
		)	
		
		on btnAddMeshes pressed do
		(	
			temp = (for o in selection collect o.name)
			
			--sort temp

			for n=1 to temp.count do
			(		
				bResult = CanAddMesh (temp[n] as string)
				if bResult == true do
				(
					mbMaterialsMeshes.items = append mbMaterialsMeshes.items (temp[n] as string)
					--sort mbMaterialsMeshes.items
				)
			)
			
			UpdateMaterialMeshes()
		)
		
		on btnRemMeshes pressed do
		(
			for i in mbMaterialsMeshes.selection do
			(
				b = mbMaterialsMeshes.items[i] as string
				for o in Geometry do
				(
					if (o.name == b) then
					(
						setuserprop o "MtlAnimMask" 0
						exit
					)
				)
			)
					
			i=0
			for n in mbMaterialsMeshes.selection do
			(
				mbMaterialsMeshes.items = deleteitem mbMaterialsMeshes.items (n-i)
				i = i+1
			)
			
			UpdateMaterialMeshes()
		)		
			
		
		function UpdateSequenceUI =
		(
			print "Updating Material sequence UI..."
			print LayerRoot
			
			if LayerRoot != undefined do
			(
				if ActiveMaterialNoteTrack == undefined do
				(
					print "Active Note Track is undefined.. getting from layer root"
					ActiveMaterialNoteTrack = GetMaterialNoteTrack LayerRoot
				)
				print ActiveMaterialNoteTrack
				
				if ActiveMaterialSequence == undefined do
				(
					print "ActiveMaterialSequence == undefined"
					ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack 1
				)
				
				if ActiveMaterialSequenceEnd == undefined do
				(
					print "ActiveMaterialSequenceEnd == undefined"
					ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
					print "Active Material Sequence End:"
					print ActiveMaterialSequenceEnd
				)
				
				print "Active Material Sequence:"
				print ActiveMaterialSequence
				print "Active Material Sequence End:"
				print ActiveMaterialSequenceEnd

				list = #()
				listsize = GetMaterialSequenceCount ActiveMaterialNoteTrack
				if listsize != undefined do
				(
					for i = 1 to listsize do
					(
						startKey = GetMaterialStartKey ActiveMaterialNoteTrack i
						seqname = GetSequenceName startKey
						if seqname != undefined do
						(
							append list seqname
						)
					)
				)

				if listsize > 0 do
				(
					ddlSequences.items = list
				)
				if listsize == 0 do
				(
					ddlSequences.items = #()
				)
				
				
				ListMaterialMeshesName = #()
				mbMaterialsMeshes.items = #()
				listmaterialmeshessize = GetMaterialSequenceCount ActiveMaterialNoteTrack
				if listmaterialmeshessize != undefined do
				(
					for i = 1 to listmaterialmeshessize do
					(
						startKey = GetMaterialStartKey ActiveMaterialNoteTrack i
						bonesname = GetMaterialSequenceSelectMeshesName startKey
							
						if bonesname != undefined then
						(
							append ListMaterialMeshesName bonesname
						)
					)
				)
				--print	ListMaterialMeshesName	
				
			)

			if ActiveMaterialSequence != undefined do
			(
				lblSeqName.enabled = true
				lblSeqCount.enabled = true
				editSeqName.enabled = true
				spnSeqID.enabled = true
				spnSeqID.value = GetSequenceID ActiveMaterialSequenceEnd
				editSeqName.text = GetSequenceName ActiveMaterialSequence
				lblSeqCount.enabled = true
				seqCount = GetMaterialSequenceCount ActiveMaterialNoteTrack
				seqIndex = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
				strSeqCount = "Sequence " + seqIndex as string + " of " + seqCount as string
				lblSeqCount.caption = strSeqCount
				--cbLooping.enabled = true
				--cbLooping.checked = GetSequenceLoop ActiveMaterialSequence
				spnLoopMode.enabled = true
				if(GetSequenceLoop ActiveMaterialSequence == undefined) then
					spnLoopMode.value = 0
				else
					spnLoopMode.value = GetSequenceLoop ActiveMaterialSequence
					
				spnStartFrame.enabled = true
				spnEndFrame.enabled = true
				spnStartFrame.value = ActiveMaterialSequence.time
				spnEndFrame.value = ActiveMaterialSequenceEnd.time
				btnCreate.enabled = true
				
				if seqCount > 1 then 
					btnDestroy.enabled = true 
				else 
					btnDestroy.enabled = false
				
				if seqIndex == 1 then 
					btnPrev.enabled = false 
				else 
					btnPrev.enabled = true
					
				if seqIndex == seqCount then 
					btnNext.enabled = false 
				else 
					btnNext.enabled = true

				ddlSequences.enabled = true
				ddlSequences.selection = seqIndex
				
				mbMaterialsMeshes.enabled = true
				btnAddMeshes.enabled = true
				btnRemMeshes.enabled = true	
				uvanim.enabled = true
				alphaanim.enabled = true
				glowanim.enabled = true											
				
				if(ListMaterialMeshesName[seqIndex] != OK and ListMaterialMeshesName[seqIndex] != undefined) then
				(	
					names = filterString ListMaterialMeshesName[seqIndex] "@"
					
					for i=1 to names.count do
					(
						n = filterString names[i] "#"
						s = ""
						for j=1 to n.count do
						(
							s = s + n[j]
							if(j != n.count) do
								s = s + " "							
						)
						names[i] = s
					)					
					
					for n=1 to names.count do
					(
						mbMaterialsMeshes.items = append mbMaterialsMeshes.items names[n]
					)
				)
				--else
				(
				--	mbMaterialsMeshes.enabled = false
				--	btnAddMeshes.enabled = false
				--	btnRemMeshes.enabled = false
				)				
				
			)
			if ActiveMaterialSequence == undefined do
			(
				lblSeqName.enabled = false
				lblSeqCount.enabled = false
				editSeqName.enabled = false
				editSeqName.text = ""
				spnSeqID.enabled = false
				spnSeqID.value = 0
				lblSeqCount.enabled = false
				seqCount = 0
				seqIndex = 0
				strSeqCount = "Sequence " + seqIndex as string + " of " + seqCount as string
				lblSeqCount.caption = strSeqCount
				--cbLooping.enabled = false
				--cbLooping.checked = false
				spnLoopMode.enabled = false
				spnLoopMode.value = 0				
				spnStartFrame.enabled = false
				spnEndFrame.enabled = false
				spnStartFrame.value = 0
				spnEndFrame.value = 0
				btnCreate.enabled = false	
				btnDestroy.enabled = false 
				btnPrev.enabled = false 
				btnNext.enabled = false 
				ddlSequences.items = #()
				ddlSequences.enabled = false
				
				mbMaterialsMeshes.enabled = false
				btnAddMeshes.enabled = false
				btnRemMeshes.enabled = false
				uvanim.enabled = false
				alphaanim.enabled = false
				glowanim.enabled = false			
			)

		)
		
		function AddMaterialSequence nt name start end loopMod = 
		(
			if nt == undefined then return undefined
			NewSequence = SetMaterialStartKey nt name start loopMod
			NewSequenceEnd = SetMaterialEndKey nt end 0
			return NewSequence
		)
		
		function RemoveSequence nt StartKey EndKey=
		(
			if nt == undefined then return undefined
			
			keyArray = #()
			for key in nt.keys do
			(
				if key.time < startKey.time or key.time > endkey.time do
				(
					append keyArray key
				)
			)
			
			ntnew = notetrack "ActorMaterialManager2"
			addNoteTrack LayerRoot ntnew
			for key in keyArray do
			(
				newKey = CreateNoteKey ntnew key.time
				newKey.value = key.value
			)

			deleteNoteTrack LayerRoot nt
			ActiveMaterialNoteTrack = ntnew
			ntnew.name = "ActorMaterialManager"
		)
		
		function CanSetActiveMaterialSequenceTime start end =
		(
			if start > end then return false
			if start >= ActiveMaterialSequenceEnd.time then return false
			if end <= ActiveMaterialSequence.time then return false
			
			index = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
			if index > 1 do
			(
				PrevSequence = GetMaterialStartKey ActiveMaterialNoteTrack (index-1)
				PrevSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack PrevSequence
				if PrevSequenceEnd.time >= start then return false
			)
			if index < GetMaterialSequenceCount ActiveMaterialNoteTrack do
			(
				NextSequence = GetMaterialStartKey ActiveMaterialNoteTrack (index+1)
				if NextSequence.time <= end then return false
			)
			return true
		)
		
		on OgreActorSequenceRollout open do
		(
			--UpdateSequenceUI()
		)
		on ddlSequences selected seqnum do
		(
			index = seqnum
			ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack index
			ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
			UpdateSequenceUI()
		)
		on editSeqName changed text do
		(
			if ActiveMaterialSequence == undefined then return true
			name = GetSequenceName ActiveMaterialSequence
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
		
			ddlSequences.selected = finaltext
			print finaltext
			items = filterstring ActiveMaterialSequence.value " "
		
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
			ActiveMaterialSequence.value = value
			
			if bHadToReviseText then UpdateSequenceUI()
		)
		
		on spnLoopMode changed frame do
		(
			name = GetSequenceName ActiveMaterialSequence		
			items = filterstring ActiveMaterialSequence.value " "
		
			value = ""
			bLoopAdded
			for i = 1 to (items.count - 1) do
			(
				item = items[i]
				bAppend = true
				if (EqualsNoCase item "-loop") then 
				(
					bAppend = false
					i = i + 1 
				)
			    item = item + " "
		
				if bAppend == true then value = value + item
			)
			
			value = value + " -loop "
			value = value + (frame as string)
			
			ActiveMaterialSequence.value = value			
		)

		on btnPrev pressed do
		(
			index = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
			if index > 1 do
			(
				index = index - 1
				ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack index
				ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
				UpdateSequenceUI()
			)
		)
		on btnNext pressed do
		(
			index = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
			count = GetMaterialSequenceCount ActiveMaterialNoteTrack
			if index< count do
			(
				index = index + 1
				ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack index
				ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
				UpdateSequenceUI()
			)
		)
		on btnCreate pressed do
		(
			index = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
			index = index + 1
			loopMod = spnLoopMode.value
			
			start = 0
			end = 0
			
			nextSeq = GetMaterialStartKey ActiveMaterialNoteTrack index
			if nextSeq == undefined do
			(
				start = ActiveMaterialSequenceEnd.time+1
				end = ActiveMaterialSequenceEnd.time + 10
			)
			if nextSeq != undefined do
			(
				start = ActiveMaterialSequenceEnd.time + 1
				end = nextSeq.time - 1
				if nextSeq.time == (ActiveMaterialSequenceEnd.time+1) or (start == end) do
				(
					message = "There is a sequence already immediately following this sequence!"
					messagebox message title:"Append error!"
					return ok
				)
			)
			
			strNewSeqName = strBlankMaterial + "_AT_" + (start as string) 
			NewSequence = AddMaterialSequence ActiveMaterialNoteTrack strNewSeqName start end loopMod
			if NewSequence != undefined do
			(
				print "btnCreate pressed..."
				ActiveMaterialSequence = NewSequence
				print ActiveMaterialSequence
				ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
				UpdateSequenceUI()
			)
		)
		on btnDestroy pressed do
		(
			RemoveSequence ActiveMaterialNoteTrack ActiveMaterialSequence ActiveMaterialSequenceEnd
			ActiveMaterialSequence = undefined 
			ActiveMaterialSequenceEnd = undefined
			UpdateSequenceUI()
		)
		
		on spnStartFrame changed frame do
		(
			bAdd = false
			if CanSetActiveMaterialSequenceTime frame ActiveMaterialSequenceEnd.time do
			(
				seqIndex = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
				ActiveMaterialSequence.time = frame
				ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack seqIndex
				ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
				setsliderTime ActiveMaterialSequence.time
				UpdateSequenceUI()
				bAdd = true
			)
			
			if bAdd == false do
			(
				spnStartFrame.value = ActiveMaterialSequence.time
				return true
			)
		)
		
		on spnSeqID changed frame do
		(
	
			seqIndex = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
			--ActiveMaterialSequenceEnd.time = frame
			ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack seqIndex
			ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence

			value = ""
			value = value + SetSequenceID ActiveMaterialSequenceEnd				
			ActiveMaterialSequenceEnd.value = value + (frame as string)
							
			--setsliderTime ActiveTrigger.time
			
			UpdateSequenceUI()
		)
		
		on spnEndFrame changed frame do
		(
			bAdd = false
			if CanSetActiveMaterialSequenceTime ActiveMaterialSequence.time frame do
			(
			    seqIndex = GetMaterialStartKeyIndex ActiveMaterialNoteTrack ActiveMaterialSequence
				ActiveMaterialSequenceEnd.time = frame
				ActiveMaterialSequence = GetMaterialStartKey ActiveMaterialNoteTrack seqIndex
				ActiveMaterialSequenceEnd = GetMaterialEndKey ActiveMaterialNoteTrack ActiveMaterialSequence
				setsliderTime ActiveMaterialSequence.time
				
				bAdd = true
				UpdateSequenceUI()
			)
			
			if bAdd == false do
			(
				spnEndFrame.value = ActiveMaterialSequenceEnd.time
				return true
			)
		)
	)	
	
-------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------ActorLayer Rollout-----------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	rollout OgreActorLayerRollout "Active Layer"  
	(
		group "Layer"
		(
			dropdownlist ddLayerRoot "Known Layer Roots:" items:#("NONE") selection:1 width:120 align:#center  enabled:false
			pickbutton btnAddLayerRoot "Add" align:#center width:55 across:2 enabled:false
			button btnRemLayerRoot "Remove" align:#center width:55 enabled:false
		)
		
		function RefreshLayerRoots obj=
		(
			--nt = GetActorNoteTrack obj
			--if nt != undefined do
			--(
			--	append AllLayerRoots obj
			--)
			
			nt = GetActorNoteTrack obj
			if nt != undefined do
			(
				append AllLayerRoots obj
			)
			
			for child in obj.children do 		
			(
				RefreshLayerRoots child
			)
		)
		
		function UpdateLayerGroupUI =
		(
			if LayerRoot == undefined and ActorRoot != undefined then
				LayerRoot = ActorRoot
		
			if LayerRoot != undefined do
			(
				AllLayerRoots = #()
				RefreshLayerRoots ActorRoot
				print AllLayerRoots
				ddLayerRoot.items = GetNameArrayFromObjArray AllLayerRoots
				
				index = 1
				for layer in AllLayerRoots do
				(
					if layer == LayerRoot then ddLayerRoot.selection = index
					index = index + 1
				)
				
				print "Updating layer UI..."
				ddLayerRoot.enabled = true
				btnAddLayerRoot.enabled = true
				btnRemLayerRoot.enabled = true
				select LayerRoot
				OgreActorSequenceRollout.UpdateSequenceUI()
				MaterialSequenceRollout.UpdateSequenceUI()
				ActorTriggerRollout.UpdateTriggerUI()
				
			)	
			
			if LayerRoot == undefined do
			(
				AllLayerRoots = #()
				ddLayerRoot.items = #("None")
				ddLayerRoot.enabled = false
				btnAddLayerRoot.enabled = false
				btnRemLayerRoot.enabled = false
				ActiveSequence = undefined
				ActiveSequenceEnd = undefined
				ActiveNoteTrack = undefined

				ActiveMaterialSequence = undefined
				ActiveMaterialSequenceEnd = undefined
				ActiveMaterialNoteTrack = undefined

				ActiveTrigger = undefined
				ActiveTriggerType= undefined
				ActiveTriggerNoteTrack = undefined
				OgreActorSequenceRollout.UpdateSequenceUI()
				MaterialSequenceRollout.UpdateSequenceUI()
				ActorTriggerRollout.UpdateTriggerUI()					
			)
		)
		
		
		on btnAddLayerRoot picked obj do
		(
			bCanDo = ParentIsActor ActorRoot obj
			if bCanDo == true do
			(
				nt = MakeActorNoteTrack obj
				LayerRoot = obj
				ActiveSequence = undefined
				ActiveSequenceEnd = undefined
				ActiveNoteTrack = undefined
				
				ActiveMaterialSequence = undefined
				ActiveMaterialSequenceEnd = undefined
				ActiveMaterialNoteTrack = undefined
				
				ActiveTrigger = undefined
				ActiveTriggerType= undefined
				ActiveTriggerNoteTrack = undefined
				UpdateLayerGroupUI()
				select LayerRoot
			)
			if bCanDo == false do
			(
				message = "Object \"" + obj.name + "\" does not have Actor \"" + ActorRoot.name + "\" as a parent!"
				messagebox message title:"Add Layer Root Error!"
			)
		)
		
		on btnRemLayerRoot pressed do
		(
			if LayerRoot == ActorRoot do
			(
				message = "Cannot remove the topmost layer root!\nTo do this, you must remove the Actor root."
				messagebox message title:"Remove Layer Root Error!"
				return true
			)
			
			RemoveActorNoteTrack LayerRoot
			LayerRoot = ActorRoot
			ActiveSequence = undefined
			ActiveSequenceEnd = undefined
			ActiveNoteTrack = undefined	
			
			ActiveMaterialSequence = undefined
			ActiveMaterialSequenceEnd = undefined
			ActiveMaterialNoteTrack = undefined
			
			ActiveTrigger = undefined
			ActiveTriggerType= undefined
			ActiveTriggerNoteTrack = undefined
			UpdateLayerGroupUI()
		)
		
		on ddLayerRoot selected item do
		(
			name = ddLayerRoot.items[item]
			for obj in AllLayerRoots do
			(
				if obj.name == name do
				(
					LayerRoot = obj
					str = "layer root: " + obj.name
					print str
					ActiveSequence = undefined
					ActiveSequenceEnd = undefined
					ActiveNoteTrack = undefined
					
					ActiveMaterialSequence = undefined
					ActiveMaterialSequenceEnd = undefined
					ActiveMaterialNoteTrack = undefined
					
					ActiveTrigger = undefined
					ActiveTriggerType= undefined
					ActiveTriggerNoteTrack = undefined					
					
					OgreActorSequenceRollout.UpdateSequenceUI()
					MaterialSequenceRollout.UpdateSequenceUI()
					ActorTriggerRollout.UpdateTriggerUI()
					select LayerRoot
					return true
				)
			)
		)
		
		on OgreActorLayerRollout open do
		(
			UpdateLayerGroupUI()
		)
	)		

-------------------------------------------------------------------------------------------------------------------------------------------------------------------
-------------------------------------------------------ActorManager Rollout----------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------------------------------------------------


	rollout OgreActorManagerRollout "Actors"
	(
		button btnRefreshAll "Refresh All" pos:[7,7] width:150 height:21
			
		group "Actor"
		(
			dropdownlist ddActorRoot "Known Actor Roots:" items:#("NONE") selection:1 width:120 align:#center 
			pickbutton btnAddActorRoot "Add" align:#center width:55 across:2 
			button btnRemActorRoot "Remove" align:#center width:55		
		)
		
		function UpdateActorGroupUI =
		(
			if ActorRoot != undefined do
			(
				ddActorRoot.items = GetNameArrayFromObjArray AllActorRoots
				index = 1;
				for actor in AllActorRoots do
				(
					if actor == ActorRoot do
					(
						ddActorRoot.selection = index
					)
					index = index + 1
				)
				
				OgreActorLayerRollout.UpdateLayerGroupUI()
			)
			if ActorRoot == undefined do
			(
				ddActorRoot.items = #("None")
				
				OgreActorLayerRollout.UpdateLayerGroupUI()
			)
		)
		
		
		function RefreshAll =
		(
			AllActorRoots = #()
			OgreGetActorRoots rootNode AllActorRoots
			
			if AllActorRoots.count == 0 then 
				ActorRoot = undefined
			else
				ActorRoot = AllActorRoots[1]
			LayerRoot = undefined
			AllLayerRoots = #()
			ActiveSequence = undefined
			ActiveSequenceEnd = undefined
			ActiveNoteTrack = undefined
			
			ActiveMaterialSequence = undefined
			ActiveMaterialSequenceEnd = undefined
			ActiveMaterialNoteTrack = undefined
			
			ActiveTrigger = undefined
			ActiveTriggerType= undefined
			ActiveTriggerNoteTrack = undefined
			ListBonesName = #()
			UpdateActorGroupUI()
		)


		function CanCreateActorRoot obj =
		(
			bCanAdd = true
			for actor in AllActorRoots do
			(
				if actor == obj then bCanAdd = false
			)
			parent = obj.parent
			
			while parent != undefined do
			(
				nt = GetActorNoteTrack parent
				if nt != undefined then bCanAdd = false	
				parent = parent.parent
			)
			
			return bCanAdd
		) 
		
		function OgreMakeActorRoot obj = 
		(
			MakeActorNoteTrack obj
			select obj
			append AllActorRoots obj
			ActorRoot = obj
			LayerRoot = obj
			ActiveSequence = undefined
			ActiveSequenceEnd = undefined
			ActiveNoteTrack = undefined
			
			ActiveMaterialSequence = undefined
			ActiveMaterialSequenceEnd = undefined
			ActiveMaterialNoteTrack = undefined
			
			ActiveTrigger = undefined
			ActiveTriggerType= undefined
			ActiveTriggerNoteTrack = undefined
		)
		
		
		on OgreActorManagerRollout open do
		(
			RefreshAll()
		)
		on OgreActorManagerRollout close do
		(
		    try(callbacks.removeScripts #filePreOpen id:#OgreActorManagerUtilPreOpen);catch()
		    try(callbacks.removeScripts #filePostOpen id:#OgreActorManagerUtilPostOpen);catch()	
		    try(callbacks.removeScripts #systemPreReset id:#OgreActorManagerUtilPreReset);catch()
		        
		)
		on btnRefreshAll pressed do
		(
			RefreshAll()
		)
		on ddActorRoot selected itemnumber do
		(
			name = ddActorRoot.items[itemnumber]
			for root in AllActorRoots do
			(
				if root.name == name do
				( 
					ActorRoot = root
					str =  "Actor Root: " + ActorRoot.name
					print str
					LayerRoot = ActorRoot
					ActiveSequence = undefined
					ActiveSequenceEnd = undefined
					ActiveNoteTrack = undefined
					
					ActiveMaterialSequence = undefined
					ActiveMaterialSequenceEnd = undefined
					ActiveMaterialNoteTrack = undefined
					
					ActiveTrigger = undefined
					ActiveTriggerType= undefined
					ActiveTriggerNoteTrack = undefined
					UpdateActorGroupUI()
					select ActorRoot
				)
			)
		)
		on btnAddActorRoot picked obj do
		(
			bResult = CanCreateActorRoot obj
			if bResult == true do
			(
				print "Creating actor root!"
				print obj.name
		
				OgreMakeActorRoot obj
				UpdateActorGroupUI()
				select ActorRoot
			)
			if bResult == false do
			(
		
				str = "Cannot add \"" + obj.name + "\" as an actor root.\nThis is because the object has a parent that is an actor root or it is already an actor root\n"
				messagebox str title:"Add Actor Root Error"
			)
		)
		on btnRemActorRoot pressed do
		(
			if ActorRoot != undefined do
			(
				nt = GetActorNoteTrack ActorRoot
		
				if nt != undefined do
				(
					print "Deleting note track"
					deletenotetrack ActorRoot nt
				)
				
				ntt = GetActorNoteTrack ActorRoot
				
				if ntt != undefined do
				(
					print "Deleting Trigger note track"
					deletenotetrack ActorRoot ntt
				)
				
				RefreshAll()
			)
		)
	)