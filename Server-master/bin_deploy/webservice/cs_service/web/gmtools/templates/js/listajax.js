
/*
**************************
(C)2010-2012 phpMyWind.com 
update: 2011-12-31 14:12:14
person: Feng
**************************
*/

//获取列表
function GetList(par_tbn, par_tid, par_cut)
{
	pag     = par_tbn;  //imagelist
	tbn     = par_tbn;  //imagelist
	tid     = par_tid;  //''
	cut     = par_cut;  //''
	flag    = "all";
	keyword = "";
	page    = 1;

	$.ajax({
		//imagelist_do.php?tbname="imagelist&tid=&flag=all&fromurl="
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("body").append('<div id="listload"></div><div id="coverbg"></div>');
			$("#coverbg").css("height",$(document).height()).show();
		},
		success:ShowList
	});
}


//输出列表
function ShowList(data, textStatus, xmlHttp)
{
	$("#list_area").html(data);
	$("#coverbg").hide();
}


//输出列表
function GetDone(data, textStatus, xmlHttp)
{
	$("#list_area").html(data);
	$("#coverbg").hide();
	$("#listload").hide();
}


//选择分类函数
function GetType(par_tid, classname)
{
	tid = par_tid;
	$(".alltype .btn").html(classname);

	$.ajax({
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
		},
		success:GetDone
	});
}


//显示属性
function GetFlag(par_flag)
{
	flag = par_flag;
	$.ajax({
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
		},
		success:GetDone
	});
}


//显示查询列表
function GetSearch()
{
	var keyword = $("#keyword").val();
	if(keyword == '' || keyword == '请填写关键字')
	{
		$("#keyword").val("请填写关键字");
		return;
	}

	$.ajax({
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword),
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
		},
		success:GetDone
	});

	$("#list_load").css({"display":"block"});
}


//显示分页
function PageList(par_page)
{
	page = par_page;

	$.ajax({
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword)+"&page="+page,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
		},
		success:GetDone
	});
}


//删除信息
function ClearInfo(par_id)
{
	if(confirm("确定要删除选中的信息吗？"))
	{
		$.ajax({
			url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword)+"&page="+page+"&action=del&id="+par_id,
			type:'get',
			dataType:'html',
			success:GetDone
		});
	}
	else
	{
		return false;
	}
}

//启用、禁用信息
function AjaxChangeStatus(status)
{
	var ckobj = $("input[type='checkbox'][name!='checkid'][name^='checkid']:checked");
	var statusName = status==1?'启用':'禁用';
	if(ckobj.size() > 0)
	{
		if(confirm("确定要"+statusName+"选中的信息吗？"))
		{
			var ids = '';
	
			ckobj.each(function(){
				if($(this).val() != 'on'){
					ids += $(this).val() + ',';
				}
			});
		
			ids = ids.slice(0,-1);
		
			$.ajax({
				url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword)+"&page="+page+"&action=changestatus&status="+status+"&ids="+ids,
				type:'get',
				dataType:'html',
				success:GetDone
			});
		}
		else
		{
			return false;
		}
	}
	else
	{
		alert('没有任何选中信息！');
		return false;
	}
}


//删除信息
function AjaxClearAll()
{
	var ckobj = $("input[type='checkbox'][name!='checkid'][name^='checkid']:checked");

	if(ckobj.size() > 0)
	{
		if(confirm("确定要删除选中的信息吗？"))
		{
			var ids = '';
	
			ckobj.each(function(){
				if($(this).val() != 'on'){
					ids += $(this).val() + ',';
				}
			});
		
			ids = ids.slice(0,-1);
		
			$.ajax({
				url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword)+"&page="+page+"&action=delall&ids="+ids,
				type:'get',
				dataType:'html',
				success:GetDone
			});
		}
		else
		{
			return false;
		}
	}
	else
	{
		alert('没有任何选中信息！');
		return false;
	}
}


//更改置顶状态
function top(par_id,state)
{
	id = par_id;
	$.ajax({
		url : tbn+"_save.php?action=check&id="+id+"&top="+encodeURI(state),
		type:'get',
		dataType:'html',
		success:function(data){$("#check"+id).html(data);}
	});
}


/******************************* 回收站 *******************************/


//显示回收站
function ShowRecycle()
{
	var recycle_title;
	
	if(pag == "infolist")
	{
		recycle_title = "信息列表回收站";
	}
	else if(pag == "infoimg")
	{
		recycle_title = "图片列表回收站";
	}
	else if(pag == "soft")
	{
		recycle_title = "软件列表回收站";
	}
	else if(pag == "goods")
	{
		recycle_title = "商品列表回收站";
	}
	else
	{
		recycle_title = "参数获取失败";
	}
	
	$("body").append("<div id=\"recycle_window\"><div class=\"header\"><span class=\"title\">"+recycle_title+"：</span> <span class=\"close\"><a href=\"javascript:HideRecycle()\"></a></span><div class=\"cl\"></div></div><form id=\"recycleform\" name=\"recycleform\" method=\"post\"><div class=\"list\" id=\"recycle_list\"></div><div class=\"bottom\"><div class=\"selall\"><span>选择：</span> <a href=\"javascript:RecycleCheckAll(true);\">全部</a> - <a href=\"javascript:RecycleCheckAll(false);\">无</a> - <a href=\"javascript:;\" onclick=\"RecycleReAll('resetall')\">还原</a> - <a href=\"javascript:;\" onclick=\"RecycleReAll('delall')\">删除</a></div><a href=\"javascript:;\" onclick=\"RecycleReAll('empty')\"><img src=\"templates/images/empty_recycle.png\" /></a> </div></form></div>")

	$.ajax({
		url : "recycle_mini.php?type="+tbn,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
			$("#recycle_window").show();
			$("#recycle_list").html('<div class="loading" style="text-align:center;margin-top:75px;"><img src="templates/images/loading.gif">列表加载中...</div>');
		},
		success:RecycleDone
	});
}


//隐藏回收站
function HideRecycle()
{
	$.ajax({
		url : pag+"_do.php?tbname="+tbn+"&tid="+tid+"&flag="+flag+"&fromurl="+cut+"&keyword="+encodeURI(keyword)+"&page="+page,
		type:'get',
		dataType:'html',
		beforeSend:function(){
			$("#recycle_window").remove();
			$("#coverbg").css("height",$(document).height()).show();
			$("#listload").show();
		},
		success:GetDone
	});
}


//回收站内容操作
function RecycleRe(action,id)
{
	$.ajax({
		url : "recycle_mini.php?type="+tbn+"&action="+action+"&id="+id,
		type:'get',
		dataType:'html',
		success:RecycleDone
	});
}


//操作所有
function RecycleReAll(action)
{
	var ids = '';

	$("#recycleform input[type='checkbox'][id^='recycle_checkid']:checked").each(function(){
		ids += $(this).val() + ',';
	});

	ids = ids.slice(0,-1);
	if(ids=='' && action!='empty')
	{
		alert('没有任何选中信息！');
		return false;
	}

	$.ajax({
		url : "recycle_mini.php?type="+tbn+"&action="+action+"&ids="+ids,
		type:'get',
		dataType:'html',
		success:RecycleDone
	});
}


//完成操作
function RecycleDone(data)
{
	$("#recycle_list").html(data);
}
