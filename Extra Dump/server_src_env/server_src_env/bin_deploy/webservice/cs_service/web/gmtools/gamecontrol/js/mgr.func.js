
/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-5-16 15:58:02
person: Feng
**************************
*/


//选择所有
function CheckAll(value)
{
	$("input[type='checkbox'][name^='checkid']").attr("checked",value);
}


//回收站选择所有
function RecycleCheckAll(value)
{
	$("input[type='checkbox'][name^='recycle_checkid']").attr("checked",value);
}


//删除单条提示
function ConfDel(i)
{
	var tips = Array();
	tips[0] = "确定要删除选中的信息吗？";
	tips[1] = "系统会自动删除类别下所有子类别以及信息，确定删除吗？";
	tips[2] = "系统会自动删除类别下所有子类别，确定删除吗？";

	if(confirm(tips[i])) return true;
	else return false;
}


//删除选中提示
function ConfDelAll(i)
{
	var tips = Array();
	tips[0] = "确定要删除选中的信息吗？";
	tips[1] = "系统会自动删除类别下所有子类别以及信息，确定删除吗？";
	tips[2] = "系统会自动删除类别下所有子类别，确定删除吗？";

	if($("input[type='checkbox'][name!='checkid'][name^='checkid']:checked").size() > 0)
	{
		if(confirm(tips[i])) return true;
		else return false;
	}
	else
	{
		alert('没有任何选中信息！');
		return false;
	}
}


//删除所有(包含子分类)
function DelAll(url,par)
{
	var par = arguments[1] ? arguments[1] : "";
	$("#form").attr("action", url+"?action=delall"+par).submit();
}


//删除所有(不包含子分类)
function DelAllNone(url)
{
	$("#form").attr("action", url+"?action=delall2").submit();
}


//执行特定参数
function SubUrlParam(url)
{
	$("#form").attr("action", url).submit();
}


//更新排序
function UpOrderID(url)
{
	$("#form").attr("action", url+"?action=uporder").submit();
}


//展开合并下级
function DisplayRows(id)
{
	var rowpid = $("div[rel='rowpid_"+id+"']");
	var rowid  = $("span[id='rowid_"+id+"']");

	if(rowid.attr("class") == "disimg")
	{
		rowpid.hide();
		rowid.attr("class","disimg2");
	}
	else
	{
		rowpid.show();
		rowid.attr("class","disimg");
	}
}


//全部显示行
function ShowAllRows()
{
	$("div[rel^='rowpid'][rel!='rowpid_0']").show();
	$("span[id^='rowid']").attr("class","disimg");
}


//全部隐藏行
function HideAllRows()
{
	$("div[rel^='rowpid'][rel!='rowpid_0']").hide();
	$("span[id^='rowid']").attr("class","disimg2");
}


//显示类别菜单
function ShowAllType()
{
	$('.alltype .btn').addClass("on");
	$('.alltype span').show();
}


//隐藏类别菜单
function HideAllType()
{
	$('.alltype .btn').removeClass("on");
	$('.alltype span').hide();
}


//文件上传提示
function UploadPrompt(string)
{
	if(string == 0)
	{
		$(".uploading").html('<div class="upload_newfile_loading"><img src="templates/images/loading.gif">文件上传中...</div>');
	}
	else
	{
		$('.uploading').html(string);
	}
}


//检查是否存在上传文件
function CheckIsUpload()
{
	if($("#upfile").val() == "")
	{
		UploadPrompt("请选择上传文件！");
		return false;
	}
	else
	{
		return true;
	}
}


//显示div
function ShowDiv(id)
{
	$("#"+id).show();
}


//隐藏div
function HideDiv(id)
{
	$("#"+id).hide();
}


//搜索按钮
function Search(key)
{
	
}


$(function(){
	$(".mgr_tr").mouseover(function(){
		$(this).attr("class","mgr_tr_on");
	}).mouseout(function(){
		$(this).attr("class","mgr_tr");
	});
});