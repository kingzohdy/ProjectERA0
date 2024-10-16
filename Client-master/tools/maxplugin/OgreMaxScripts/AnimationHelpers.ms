
function FindBlankKey nt time =
(
	if nt != undefined do
	(
		for key in nt.keys do
		(
			if key.time == time do
			(
				if key.value == "" then return key
			)
		)
	)
	
	return undefined
)
	
function CreateNoteKey nt time =
(
	blankKey = FindBlankKey nt time
	if blankKey == undefined do
	(
		AddBlankNoteKey nt time
		blankKey = FindBlankKey nt time
	)
	
	print "blankKey = "
	print blankKey
	print "\n"
	return blankKey
)

function SetSliderTime time =
(
	lastTime = time
	sliderTime = time
)

function uppercase instring = 
(
	-- this function converts instring to all capitol letters
	-- so that we can do a case-insensitive compare
	local upper, lower, outstring
	upper="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	lower="abcdefghijklmnopqrstuvwxyz" 
	outstring = copy instring 

	for i = 1 to outstring.count do 
	(
		j = findString lower outstring[i] 
		if (j != undefined) do outstring[i] = upper[j] 
	)
	outstring
)

function EqualsNoCase instring outstring =
(
	local string1 = uppercase instring
	local string2 = uppercase outstring
	return string1 == string2
)
-------------------------------------------------------------------------------------------------------------------------
---------------------------------------Trigger---------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
function GetActorTriggerNoteTrack node =
(
	if hasNoteTracks node do
	(
		for i = 1 to numnotetracks node do
		(
			nt = getnotetrack node i
			if nt != undefined do
			(
				for key in nt.keys do
				(
					foundValue = findstring key.value "OgreTrigger"
					if foundValue != undefined do
					(
						 return nt
					)
				)
			)
		)
	)
	return undefined
)


function SetTriggerKey nt name time type=
(
	key = CreateNoteKey nt time
	value = "OgreTrigger -name " + name
	value = value + " -type "
	value = value + (type as string)
	key.value = value
	return key
)

function SetTriggerType key= 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		
		value = ""
		index = 1
		for string in strings do
		(
			item = strings[index]
			item = item + " "
			value = value + item
			bEquals = EqualsNoCase string "-type"
			if bEquals then 
			(
				return value 
			)
			index = index + 1
		)
	)
	else
		return undefined
)

function GetTriggerKey nt index =
(
	if nt == undefined then return undefined
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreTrigger"
 			if foundValue != undefined do
		(
			i = i + 1
			if i == index then return key
		)
	)
	return undefined
)

function GetTriggerKeyIndex nt ActiveKey =
(
	if nt == undefined then return 0
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreTrigger"
		if foundValue != undefined do
		(
			i = i + 1
			if ActiveKey.value == key.value then return i
		)
	)
	return 0
)

function GetTriggerCount nt = 
(
	if nt == undefined then return 0
	index = 1
	count = 0
	key = GetTriggerKey nt index
	while key != undefined do
	(
		count = count + 1
		index = index + 1
		key = GetTriggerKey nt index
	)
	return count
)

function GetTriggerType key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-type"
			if bEquals then 
			(
				return (strings[index+1] as Integer)
			)
			index = index + 1
		)
	)
	else
		return undefined
)

function GetTriggerName key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-name"
			if bEquals then return strings[index+1] 
			index = index + 1
		)
	)
	else
		return undefined
)

-------------------------------------------------------------------------------------------------------------------------
---------------------------------------Trigger---------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------
function GetMaterialNoteTrack node =
(
	if hasNoteTracks node do
	(
		for i = 1 to numnotetracks node do
		(
			nt = getnotetrack node i
			if nt != undefined do
			(
				for key in nt.keys do
				(
					foundValue = findstring key.value "OgreMaterialBegin"
					if foundValue != undefined do
					(
						 return nt
					)
				)
			)
		)
	)
	return undefined
)

function SetMaterialStartKey nt name time loopMod=
(
	key = CreateNoteKey nt time
	value = "OgreMaterialBegin -name " + name
	value = value + " -loop "
	value = value + (loopMod as string)
	key.value = value
	return key
)

function SetMaterialEndKey nt time seqID =
(
	key = CreateNoteKey nt time
	value = "OgreMaterialEnd"
	value = value + " -seqID "
	value = value + (seqID as string)
	
	key.value = value
	return key 
)

function GetMaterialStartKey nt index =
(
	if nt == undefined then return undefined
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreMaterialBegin"
 			if foundValue != undefined do
		(
			i = i + 1
			if i == index then return key
		)
	)
	return undefined
)

function GetMaterialEndKey nt StartKey = 
(
	print "Get Material End Key"
	print nt
	if nt == undefined or StartKey == undefined then return undefined
	index = 1
	startIdx = 0
	for key in nt.keys do
	(
		print ("Index = " + (index as string))
		print key.value
		if key.value == startKey.value do
		( 
			startIdx = index
			print "found start index..."
		)
		
		foundIdx = findString key.value "OgreMaterialEnd"
		if startIdx == 0 do
		(
			index = index + 1
			continue
		)			
		if foundIdx == undefined do
		(
			index = index + 1
			continue
		)
		if foundIdx != 1 do
		(
			index = index + 1
			continue
		)			
		if foundIdx == 1 do
		(
			print "Found material end key"
			print key
			return key
		) 
	)
	return undefined		
)

function GetMaterialSequenceCount nt = 
(
	if nt == undefined then return 0
	index = 1
	count = 0
	key = GetMaterialStartKey nt index
	while key != undefined do
	(
		count = count + 1
		index = index + 1
		key = GetMaterialStartKey nt index
	)
	return count
)

function GetMaterialStartKeyIndex nt ActiveKey =
(
	if nt == undefined then return 0
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreMaterialBegin"
		if foundValue != undefined do
		(
			i = i + 1
			if ActiveKey.value == key.value then return i
		)
	)
	return 0
)

function SetMaterialType key= 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		
		value = ""
		index = 1
		for string in strings do
		(
			item = strings[index]
			item = item + " "
			value = value + item
			bEquals = EqualsNoCase string "-type"
			if bEquals then 
			(
				return value 
			)
			index = index + 1
		)
	)
	else
		return undefined
)

function GetMaterialKey nt index =
(
	if nt == undefined then return undefined
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreMaterialBegin"
 			if foundValue != undefined do
		(
			i = i + 1
			if i == index then return key
		)
	)
	return undefined
)

function GetMaterialKeyIndex nt ActiveKey =
(
	if nt == undefined then return 0
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreMaterialBegin"
		if foundValue != undefined do
		(
			i = i + 1
			if ActiveKey.value == key.value then return i
		)
	)
	return 0
)

function GetMaterialCount nt = 
(
	if nt == undefined then return 0
	index = 1
	count = 0
	key = GetTriggerKey nt index
	while key != undefined do
	(
		count = count + 1
		index = index + 1
		key = GetTriggerKey nt index
	)
	return count
)

function GetMaterialType key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-type"
			if bEquals then 
			(
				return (strings[index+1] as Integer)
			)
			index = index + 1
		)
	)
	else
		return undefined
)

function GetMaterialName key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-name"
			if bEquals then return strings[index+1] 
			index = index + 1
		)
	)
	else
		return undefined
)

function GetMaterialSequenceSelectMeshesName key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-materialmeshes"
			if bEquals then return strings[index+1] 
			index = index + 1			
		)
	)
	else
		return undefined
)

-------------------------------------------------------------------------------------------------------------------------
---------------------------------------Sequence---------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------

function GetActorNoteTrack node =
(
	if hasNoteTracks node do
	(
		for i = 1 to numnotetracks node do
		(
			nt = getnotetrack node i
			if nt != undefined do
			(
				for key in nt.keys do
				(
					foundValue = findstring key.value "OgreBegin"
					if foundValue != undefined do
					(
						 return nt
					)
				)
			)
		)
	)
	return undefined
)

function SetSimpleStartKey nt name time loopMod=
(
	key = CreateNoteKey nt time
	value = "OgreBegin -name " + name
	value = value + " -loop "
	value = value + (loopMod as string)
	key.value = value
	return key
)

function SetSimpleEndKey nt time seqID =
(
	key = CreateNoteKey nt time
	value = "OgreEnd"
	value = value + " -seqID "
	value = value + (seqID as string)
	
	key.value = value
	return key 
)

function MakeActorNoteTrack node =
(
	nt = GetActorNoteTrack node
	if nt != undefined then return nt
	
	nt = notetrack "ActorManager"
	addNoteTrack node nt
	strNewSeqName = "EMPTY_SEQUENCE_AT_" + (sliderTime as string) 
	SetSimpleStartKey nt strNewSeqName animationRange.start 0
	SetSimpleEndKey nt animationRange.end 0
	
	nt = notetrack "ActorMaterialManager"
	addNoteTrack node nt
	strNewSeqName = "EMPTY_SEQUENCE_AT_" + (sliderTime as string) 
	SetMaterialStartKey nt strNewSeqName animationRange.start 0
	SetMaterialEndKey nt animationRange.end -1
	
	ntt = notetrack "ActorTriggerManager"
	addNoteTrack node ntt
	strNewTrigName = "EMPTY_TRIGGER_AT_" + (sliderTime as string) 
	SetTriggerKey ntt strNewTrigName animationRange.start -1
)

function RemoveActorNoteTrack node =
(
	nt = GetActorNoteTrack node
	if nt == undefined then return false
	
	deletenotetrack node nt
	
	ntt = GetActorTriggerNoteTrack node
	if ntt == undefined then return false
	
	deletenotetrack node ntt
)

function OgreGetActorRoots Parent RootArray =
(
	for Object in Parent.children do
	(
		if hasNoteTracks Object then
		(
			for i = 1 to numNoteTracks Object do
			(
				Notes = getNoteTrack Object i
				for key in Notes.keys do
				(
					retVal = findString key.value "OgreBegin"
					if retVal != undefined then
					(
						append RootArray Object
						exit
					)
				)
			)
		)
		else
			OgreGetActorRoots Object RootArray
	)
)

function GetStartKey nt index =
(
	if nt == undefined then return undefined
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreBegin"
 			if foundValue != undefined do
		(
			i = i + 1
			if i == index then return key
		)
	)
	return undefined
)

function GetSequenceCount nt = 
(
	if nt == undefined then return 0
	index = 1
	count = 0
	key = GetStartKey nt index
	while key != undefined do
	(
		count = count + 1
		index = index + 1
		key = GetStartKey nt index
	)
	return count
)

function GetStartKeyIndex nt ActiveKey =
(
	if nt == undefined then return 0
	i = 0
	for key in nt.keys do
	(
		foundValue = findstring key.value "OgreBegin"
		if foundValue != undefined do
		(
			i = i + 1
			if ActiveKey.value == key.value then return i
		)
	)
	return 0
)

function GetEndKey nt StartKey = 
(
	print "Get End Key"
	print nt
	if nt == undefined or StartKey == undefined then return undefined
	index = 1
	startIdx = 0
	for key in nt.keys do
	(
		print ("Index = " + (index as string))
		print key.value
		if key.value == startKey.value do
		( 
			startIdx = index
			print "found start index..."
		)
		
		foundIdx = findString key.value "OgreEnd"
		if startIdx == 0 do
		(
			index = index + 1
			continue
		)			
		if foundIdx == undefined do
		(
			index = index + 1
			continue
		)
		if foundIdx != 1 do
		(
			index = index + 1
			continue
		)			
		if foundIdx == 1 do
		(
			print "Found end key"
			print key
			return key
		) 
	)
	return undefined		
)

function GetSequenceName key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-name"
			if bEquals then return strings[index+1] 
			index = index + 1
		)
	)
	else
		return undefined
)

function GetSequenceSelectBonesName key = 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		index = 1
		for string in strings do
		(
			bEquals = EqualsNoCase string "-selectbonesname"
			if bEquals then return strings[index+1] 
			index = index + 1			
		)
	)
	else
		return undefined
)

function GetExpOnly key =
(
	strings = filterstring key.value " "
	index = 1
	for string in strings do
	(
		bEquals = EqualsNoCase string "-exponly"
		if bEquals then return true 
		index = index + 1
	)
	return false
)

function GetSequenceLoop key =
(
	strings = filterstring key.value " "
	index = 1
	for string in strings do
	(
		bEquals = EqualsNoCase string "-loop"
		if bEquals then 
		(
			if(strings[index+1] == undefined) then
				return undefined
			else
				return (strings[index+1] as integer)
		)
		index = index + 1
	)
	return undefined
)

function GetSequenceID key =
(
	strings = filterstring key.value " "
	index = 1
	for string in strings do
	(
		bEquals = EqualsNoCase string "-seqID"
		if bEquals then 
		(
			return (strings[index+1] as integer)
		)
		index = index + 1
	)
	return undefined
)

function SetSequenceID key= 
(
	if key != undefined then
	(
		strings = filterstring key.value " "
		
		value = ""
		index = 1
		for string in strings do
		(
			item = strings[index]
			item = item + " "
			value = value + item
			bEquals = EqualsNoCase string "-seqID"
			if bEquals then 
			(
				return value 
			)
			index = index + 1
		)
	)
	else
		return undefined
)

function GetNameArrayFromObjArray objarray =
(
	nameArray = #()
	for obj in objarray do
	(
		if obj != undefined then append nameArray obj.name
	)
	return nameArray
)

function ParentIsActor actor obj =
(
	if obj == actor then return true
	parent = obj.parent
	while parent != undefined do
	(
		if parent == actor then return true
		parent = parent.parent
	)
	
	return false
)
