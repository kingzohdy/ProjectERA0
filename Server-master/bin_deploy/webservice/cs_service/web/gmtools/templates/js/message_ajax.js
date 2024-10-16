
/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-1-10 13:09:18
person: Feng
**************************
*/


//Ajax发送内容
function ReMsg()
{
	msg = $("#msg").val();

	$.ajax({
		url : "message_do.php",
		type:'post',
		data:{"action":"remsg", "msg":msg, "id":id},
		dataType:'html',
		beforeSend:function(){},
		success:HideReWin
	});
}


//显示回复窗口
function ShowReWin(reid)
{
	id = reid;
	var cont = $("#recont_"+id).attr("rel");

	$("#graybg").show();
	$("#popup_window").show();
	$("#msg_area").html('<form name="form" id="form" method="post"><textarea name="msg" id="msg" class="smgarea" onkeyup="CkReMsgLen(this);">'+ cont +'</textarea><a href="javascript:;" onclick="ReMsg();" class="sendbtn"><img src="templates/images/remessage.gif" /></a><span id="length"></span></form>');
}


//隐藏回复窗口
function HideReWin(data)
{
	$("#graybg").hide();
	$("#popup_window").hide();
	$("#recont_"+id).attr("rel", msg);
	$("#tit_"+id).html(data);

	//setInterval('location.reload();',1000);
}


//检测输入文字
function CkReMsgLen(txt)
{
	$("#length").html('您已经输入了： '+ txt.value.length +' 个字');
}


//创建回复窗口
$(function(){
	$("body").append('<div id="graybg"> </div> <div id="popup_window"><div class="header"><span class="title">回复留言：</span> <span class="close_div"><a href="javascript:HideReWin();" ></a></span><div class="cl"></div></div><div class="msg_area" id="msg_area"> </div></div>');
});