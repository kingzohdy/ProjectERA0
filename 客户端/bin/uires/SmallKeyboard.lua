local MAX_KEY_NUM	= 37;
local n_UpperFlag	= 0;
local n_Index		= 0;
local str_Password	= "";

local t_KeyValue = {
			"0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f","g",
			"h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x",
			"y","z","_"
		   }

function UpdateSmallKeyboardFrame()
	local n_Circle_i    = 0;
	local n_Cur_Len		= MAX_KEY_NUM;
	local str_Tmp_Char	= "";

	for n_Circle_i = 1, MAX_KEY_NUM do
		local KeyFont = getglobal( "Key_"..n_Cur_Len.."KeyValue" );
		local Key     = getglobal( "Key_"..n_Cur_Len ); 
		if KeyFont ~= nil then
			n_Index = math.random( 1, n_Cur_Len );
			str_Tmp_Char = t_KeyValue[ n_Cur_Len ];
			t_KeyValue[ n_Cur_Len ] = t_KeyValue[ n_Index ];
			t_KeyValue[ n_Index ] = str_Tmp_Char;

			if n_UpperFlag == 1 then
				KeyFont:SetText( string.upper(t_KeyValue[ n_Cur_Len ]) )
			else
				KeyFont:SetText( t_KeyValue[ n_Cur_Len ] )
			end
			KeyFont:Show();
			Key:SetClientUserData( 0, n_Cur_Len );
			n_Cur_Len = n_Cur_Len -1;
		end
	end
end

function SmallKeyboardFrame_OnShow()
	this:SetPoint( "center", "$parent", "center", 290, 43 );
	UpdateSmallKeyboardFrame();
end

function Key_OnClick()
	str_Password	= PassWordEdit:GetPassWord();
	local n_Index	= this:GetClientUserData( 0 );
	local KeyFont	= getglobal( "Key_"..n_Index.."KeyValue" );
	local str_TmpPassword = KeyFont:GetText();
	if n_UpperFlag == 1 then
		str_TmpPassword = string.upper( str_TmpPassword );
	end 
	str_Password    = str_Password..str_TmpPassword;
	PassWordEdit:SetText( str_Password );
	SetFocusFrame( "PassWordEdit" );
	UpdateSmallKeyboardFrame();
end

function BigKey_OnClick()
	if n_UpperFlag == 0 then
		n_UpperFlag = 1;
		--Key_38_Select:Show();
		Key_38KeyValue:SetText("ะกะด")
	else
		n_UpperFlag = 0;
		--Key_38_Select:Hide();
		Key_38KeyValue:SetText("ด๓ะด")
	end	
	UpdateSmallKeyboardFrame();
	SetFocusFrame( "PassWordEdit" );
end

function BigKey_OnMouseUp()
	SetFocusFrame( "PassWordEdit" );
end

function Key_OnEnter()
	local keyFont = getglobal( this:GetName().."KeyValue" );
	keyFont:SetTextColor(255,255,190);
	keyFont:SetPoint("topleft",this:GetName(),"toplef",0,4);
end

function Key_OnLeave()
	local keyFont = getglobal( this:GetName().."KeyValue" );
	keyFont:SetTextColor(255,255,190);
	keyFont:SetPoint("topleft",this:GetName(),"toplef",0,5);
end