
function Slider_OnValueChanged()
	local parent = getglobal(this:GetParent());
	parent:SetVerticalScroll(arg1);
end

function ScrollUpButtonOnClick()
	local parent = getglobal(this:GetParent());
	--parent:SetValue(parent:GetValue() - (parent:GetHeight() / 2));

	parent:SetValue(parent:GetValue() - parent:GetValueStep());
end

function ScrollDownButtonOnClick()
	local parent = getglobal(this:GetParent());
	--parent:SetValue(parent:GetValue() + (parent:GetHeight() / 2));

	parent:SetValue(parent:GetValue() + parent:GetValueStep());
end


-- Scrollframe functions
function ScrollFrame_OnLoad()
	local scrollbar = getglobal(this:GetName().."ScrollBar");
	scrollbar:SetHeight(this:GetHeight());

	getglobal(this:GetName().."ScrollBarScrollDownButton"):Disable();
	getglobal(this:GetName().."ScrollBarScrollUpButton"):Disable();

	local scrollbar = getglobal(this:GetName().."ScrollBar");
	scrollbar:SetMinValue(0);
	scrollbar:SetMaxValue(0);
	scrollbar:SetValue(0);
	this:SetOffset(0);

end

function ScrollFrame_OnScrollRangeChanged()
	scrollrange = arg1;
	local scrollbar = getglobal(this:GetName().."ScrollBar");
	if ( not scrollrange ) then
		scrollrange = this:GetVerticalScrollRange();
	end
	local value = scrollbar:GetValue();
	if ( value > scrollrange ) then
		value = scrollrange;
	end
	scrollbar:SetMinValue(0);
	scrollbar:SetMaxValue(scrollrange);
	scrollbar:SetValue(value);
	scrollbar:SetValueStep(scrollrange/2);
	if ( math.floor(scrollrange) == 0 ) then
		if (this.scrollBarHideable ) then
			getglobal(this:GetName().."ScrollBar"):Hide();
			getglobal(scrollbar:GetName().."ScrollDownButton"):Hide();
			getglobal(scrollbar:GetName().."ScrollUpButton"):Hide();
		else
			getglobal(scrollbar:GetName().."ScrollDownButton"):Disable();
			getglobal(scrollbar:GetName().."ScrollUpButton"):Disable();
			getglobal(scrollbar:GetName().."ScrollDownButton"):Show();
			getglobal(scrollbar:GetName().."ScrollUpButton"):Show();
		end
		getglobal(scrollbar:GetName().."ThumbTexture"):Hide();
	else
		getglobal(scrollbar:GetName().."ScrollDownButton"):Show();
		getglobal(scrollbar:GetName().."ScrollUpButton"):Show();
		getglobal(this:GetName().."ScrollBar"):Show();
		getglobal(scrollbar:GetName().."ScrollDownButton"):Enable();
		getglobal(scrollbar:GetName().."ThumbTexture"):Show();
	end
	
	--[[
	-- Hide/show scrollframe borders
	local top = getglobal(this:GetName().."Top");
	local bottom = getglobal(this:GetName().."Bottom");
	local middle = getglobal(this:GetName().."Middle");
	if ( top and bottom and this.scrollBarHideable) then
		if ( this:GetVerticalScrollRange() == 0 ) then
			top:Hide();
			bottom:Hide();
		else
			top:Show();
			bottom:Show();
		end
	end
	if ( middle and this.scrollBarHideable) then
		if ( this:GetVerticalScrollRange() == 0 ) then
			middle:Hide();
		else
			middle:Show();
		end
	end
	]]
end

function ScrollFrameTemplate_OnMouseWheel()
	local scrollBar = getglobal(this:GetName().."ScrollBar");
	if ( arg1 > 0 ) then
		scrollBar:SetValue(scrollBar:GetValue() - (scrollBar:GetHeight() / 2));
	else
		scrollBar:SetValue(scrollBar:GetValue() + (scrollBar:GetHeight() / 2));
	end
end

function ScrollFrame_OnVerticalScroll()
	local scrollbar = getglobal(this:GetName().."ScrollBar");
	scrollbar:SetValue(arg1);
	local min;
	local max;
	min = scrollbar:GetMinValue();
	max = scrollbar:GetMaxValue();

	if ( arg1 == 0 ) then
		getglobal(scrollbar:GetName().."ScrollUpButton"):Disable();
	else
		getglobal(scrollbar:GetName().."ScrollUpButton"):Enable();
	end

	if ((scrollbar:GetValue() - max) == 0) then
		getglobal(scrollbar:GetName().."ScrollDownButton"):Disable();
	else
		getglobal(scrollbar:GetName().."ScrollDownButton"):Enable();
	end
end

-- Function to handle the update of manually calculated scrollframes.  Used mostly for listings with an indeterminate number of items
function FauxScrollFrame_Update(frameName, numItems, numToDisplay, valueStep, button, smallWidth, bigWidth, highlightFrameName, smallHighlightWidth, bigHighlightWidth )
	-- If more than one screen full of skills then show the scrollbar
	local frame = getglobal(frameName);
	local scrollBar = getglobal( frameName.."ScrollBar" );
	local showScrollBar;
	if ( numItems > numToDisplay ) then
		frame:Show();
		showScrollBar = 1;
	else
		scrollBar:SetValue(0);
		frame:Hide();
	end
	if ( frame:IsShown() ) then
		local scrollChildFrame = getglobal( frameName.."ScrollChildFrame" );
		local scrollUpButton = getglobal( frameName.."ScrollBarScrollUpButton" );
		local scrollDownButton = getglobal( frameName.."ScrollBarScrollDownButton" );
		local scrollFrameHeight = 0;
		local scrollChildHeight = 0;

		if ( numItems > 0 ) then
			scrollFrameHeight = (numItems - numToDisplay) * valueStep;
			scrollChildHeight = numItems * valueStep;
			if ( scrollFrameHeight < 0 ) then
				scrollFrameHeight = 0;
			end
			scrollChildFrame:Show();
		else
			scrollChildFrame:Hide();
		end
		scrollBar:SetMinValue(0);
		scrollBar:SetMaxValue(scrollFrameHeight);
		scrollBar:SetValueStep(valueStep);
		scrollChildFrame:SetHeight(scrollChildHeight);
		
		-- Arrow button handling
		if ( scrollBar:GetValue() == 0 ) then
			scrollUpButton:Disable();
		else
			scrollUpButton:Enable();
		end

		if ((scrollBar:GetValue() - scrollFrameHeight) == 0) then
			scrollDownButton:Disable();
		else
			scrollDownButton:Enable();
		end
		
		--local highlightFrame = getglobal(highlightFrameName);
		-- Shrink because scrollbar is shown
		--if ( highlightFrame ) then
		--	highlightFrame:SetWidth(smallHighlightWidth);
		--end
		--if ( button ) then
		--	for i=1, numToDisplay do
		--		getglobal(button..i):SetWidth(smallWidth);
		--	end
		--end
	else
		-- Widen because scrollbar is hidden
		--if ( highlightFrame ) then
		--	highlightFrame:SetWidth(bigHighlightWidth);
		--end
		--if ( button ) then
		--	for i=1, numToDisplay do
		--		getglobal(button..i):SetWidth(bigWidth);
		--	end
		--end
	end
	return showScrollBar;
end

function FauxScrollFrame_GetOffset(frameName)
	local frame = getglobal(frameName);
	return frame:GetOffset();
end

function FauxScrollFrame_OnVerticalScroll(itemHeight, updateFunction)
	local scrollbar = getglobal(this:GetName().."ScrollBar");
	scrollbar:SetValue(arg1);
	this:SetOffset(math.floor((arg1 / itemHeight) + 0.5));
	updateFunction();
end

--[[
function ScrollUpButtonOnClick()
	local s = getglobal(this:GetParent());
	local val = s:GetValue();

	local scrollframeName = getglobal(s:GetParent());
	local scrollframe = getglobal(scrollframeName:GetName());

	if(s:GetMinValue() < val - scrollframe:GetHeight()/2) then
		s:SetValue(val - scrollframe:GetHeight()/2);
		scrollframe:SetVerticalScroll(val - scrollframe:GetHeight()/2);
	else
		s:SetValue(s:GetMinValue());
		scrollframe:SetVerticalScroll(s:GetValue());
	end
end


function ScrollDownButtonOnClick()
	local s = getglobal(this:GetParent());

	local scrollframeName = getglobal(s:GetParent());
	local scrollframe = getglobal(scrollframeName:GetName());

	local val = s:GetValue();
	if(s:GetMaxValue() > val + scrollframe:GetHeight()/2) then
		s:SetValue(val + scrollframe:GetHeight()/2);
		scrollframe:SetVerticalScroll(val + scrollframe:GetHeight()/2);
	else
		s:SetValue(s:GetMaxValue());
		scrollframe:SetVerticalScroll(s:GetValue());
	end
end
]]