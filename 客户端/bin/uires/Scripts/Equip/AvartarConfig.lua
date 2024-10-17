local t_AvartarControl = {};
function GetAvartarControl()
	return t_AvartarControl;
end

-- 枪手只要装备了左手或者右手中的任意一边武器，则播放备战动作
t_AvartarControl["getHairConfig"] =
function ( self, data )
	local nHeadArmId = data["itemID"];
	if nHeadArmId == 0 then
		return getAvatarAttrib( POS_ID_HAIR );
	end
	
	local nHariStyle = getItemDisplayInfoHairStyle( nHeadArmId );
	if nHariStyle == 0 then
		return 0;
	else
		return getAvatarAttrib( POS_ID_HAIR );
	end
end

t_AvartarControl["getShouldConfig"] =
function ( self, data )
	local nInitRace			= data["race"];
	local nFashionArmId		= data["fashionItemID"];
	local nShouldArmId		= data["shouldItemID"];
	
	if nFashionArmId == 0 or nInitRace == CLIENT_CAREER_GUNER then
		return nShouldArmId;
	end

	return 0;
end

t_AvartarControl["getAvatarConfigAnimID"] =
function ( self, data )
	local armId		= data["itemID"];
	local nInitRace	= data["race"];
	if armId == 0 then
		return 10100;
	end
	
	local nPreAnimID = 10100;
	local arm_sub_type  = math.floor(armId/10000);
	local arm_race_type = math.floor(armId/100000);
	for i = 1, table.getn( t_posestate ) do
		if arm_race_type == t_posestate[i][1] and arm_sub_type == t_posestate[i][2] then
			if t_posestate[i][3]  > POSE_END then
				return 10108; -- 各职业的基本备战动作 后三位108
			else
				nPreAnimID =t_posestate[i][3] * 1000 + 100; -- 各职业的穿装备的备战动作 后三位108
				break;
			end
		end
	end
	
	if nInitRace == CLIENT_CAREER_GUNER then
		return nPreAnimID + 8;
	else
		return nPreAnimID;
	end
end

t_AvartarControl["getAvatarAnim"] =
function ( self, data )
	local nInitRace		= data["race"];
	local nLeftArmID	= data["leftHandItem"];
	local nRightArmID	= data["rightHandItem"];
	
	local nArmID = nRightArmID;
	if nArmID == 0 then
		nArmID = nLeftArmID;
	end

	return self:getAvatarConfigAnimID( { ["race"] = nInitRace, ["itemID"] = nArmID } );
end