<?php	require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] == "游戏管理员")
{
	$_SESSION = array();
	session_destroy();	
	echo '<script type="text/javascript">window.top.location.href="login.php";</script>';
	exit();
}
 ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>聊天日志查询</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
.thickdiv{position:fixed;top:0;left:0;z-index:10000001;width:100%;height:100%;background:#F7F7F7;border:0;filter:alpha(opacity=70);opacity:0.70;}
.thickbox{position:absolute;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
.thicktitle{height:30px;padding:0 15px; margin-top:8px; text-align:left;line-height:27px;font-family:arial,"宋体";font-size:14px;font-weight:bold;color:#F00;}

.thickclose:link,.thickclose:visited{display:block;position:absolute;z-index:100000;top:10px;right:12px;overflow:hidden;width:21px;height:21px;background:url(images/close_ico_dis.gif) no-repeat 0 3px;font-size:0;line-height:100px;}
.thickclose:hover{ background-position:left -18px;}
.thickcon{overflow:auto;padding:7px;text-align:center; vertical-align:top;}

#BgDiv{background-color:#F7F7F7; position:absolute; z-index:99; left:0; top:0; display:none; width:100%; height:100%;opacity:0.5;filter: alpha(opacity=50);-moz-opacity: 0.5;} 
#DialogDiv{position:absolute;width:300px; left:55%; top:20%; margin-left:-200px; height:50px; z-index:100; border:0px #ccc solid; padding:1px;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" src="../data/plugin/calendar/calendar1.js"></script>
<script type="text/javascript">
	var i = 0;
	var j = 0;
	var k = 0;
	var t = 0;
	var $tri=0;
$(document).ready(function(){
  $("#ttt").hide();
});
function jingyanRole(AccName,q,f)
{
	var qu=q;
	var RoleName = AccName;
	var fu = f;
	$.kangThickBox({
		type: "iframe",
		title:"禁言角色",
		source: 'SilenceAccount.php?q='+qu+'&&fu='+fu+'&&RoleName='+RoleName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function dongjieRole(RoleName,q,f)
{
	var qu=q;
	var fu= f;
	var RoleName = RoleName;
	$.kangThickBox({
		type: "iframe",
		title:"封角色",
		source: 'FreezeAccount.php?FreezeRole=1&&q='+qu+'&&f='+fu+'&&RoleName='+RoleName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function jingyan(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"禁言账号",
		source: 'SilenceAccount.php?q='+qu+'&&AccName='+AccName+'',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function dongjie(AccName,q)
{
	var qu=q;
	var AccName = AccName;
	$.kangThickBox({
		type: "iframe",
		title:"冻结账号",
		source: 'FreezeAccount.php?FreezeAccount=0&&q='+qu+'&&AccName='+AccName+'',
		width: 300,
		height: 260,
		_autoReposi: true
	});
}
function xf()
{
	//$GLOBALS['db_name'];
	var keyword = $("#dq").val();
	if(keyword == '' || keyword == '-请选择-')
	{
		$("#qu").html("请选择大区");
		$("#fw").empty();
		return;
	}
	if(keyword != '' && keyword != '-请选择-')
	{
		$("#qu").html("");
		i=1;
	}

	$.ajax({
		url : "select_log.php",
		type:'post',
		dataType:'json',
		data:{"daqu":keyword},
		success:function(msg){
			 $(document).ready(function () {
 				 $("#fw").empty();
 				 $.each(msg, function (index, value) {
 				 	$("#fw")[0].options.add(new Option(value.worldname, value.worldid, false, false));
  				});
  			});
		}
	});
}
function jc()
{
	 var stp = $("#us").val();
	if(stp == '')
	{
		$("#us1").html("1");
		//$("#tt1").html("请输入角色名");
		//$("#ttt").show();
		return;
	}
	if(stp != '')
	{
		$("#us1").html(""); 
		j=1;
	}
}
function gjz()
{
	 var stp1 = $("#key").val();
	if(stp1 == '')
	{
		$("#key1").html("2");
		return;
	}
	if(stp1 != '')
	{
		$("#key1").html(""); 
		k=1;
	}
}
function stime()
{
	 var st = $("#posttime").val();
	if(st == '')
	{
		$("#sta").html("请选择开始时间");
		return;
	}
	if(st != '')
	{
		$("#sta").html(""); 
		t=1;
	}
}
//搜索
function sear()
{
	var tp="<?php echo $_SESSION['admin1']; ?>";
	if(i ==1 && t==1 && ((j == 1 || k == 1) ||(j==1 && k==1))){
	var qu=$("#dq").val();
	var fu=$("#fw").val();
		 //选择服务器
		var f = qu+'_'+fu;
		//时间
		var tb=$("#posttime").val();
		var tb1=$("#posttime1").val();
		//角色名
		var jsm=$("#us").val();
		//关键字
		var skey=$("#key").val();
		
		$.ajax({
  			type: 'POST',
 			url: 'select_log.php',
			dataType: 'json',
  			data: {"ldb": f,"time":tb, "time1":tb1, "us":jsm, "key":skey, "tab":'LogRoleChat_'}, 
  			success: function(msg){
				if( null ==msg || ""== msg)
				{
					$("#tt1").html("未检索到数据......");
					$("#ttt").show();
					$("#ajaxlist").empty();
					exit();
				}
				if("不存在数据表"==msg)
				{
					$("#tt1").html("不存在数据表");
					$("#ttt").show();
					$("#ajaxlist").empty();
					exit();
				}
				if("结束时间不能低于开始时间"==msg)
				{
					$("#tt1").html("结束时间不能低于开始时间!!!");
					$("#ttt").show();
					$("#ajaxlist").empty();
					exit();
				}
				if("结束时间与开始时间相差不能超过3天"==msg)
				{
					$("#ajaxlist").empty();
					$("#tt1").html("结束时间与开始时间相差不能超过3天");
					$("#ttt").show();
					exit();
				}
				else
				{
					$("#ttt").hide();
					$("#ajaxlist").empty();
					var ss='<tr class=thead2 align=center><td >账号</td><td >角色名</td><td >频道</td><td >聊天对象</td><td >聊天内容</td><td >聊天时间</td><td >来源表</td><td >操作</td></tr>';
					$("#ajaxlist").append(ss);
					//处理检索到的数据......
					$.each(msg, function (index, value) {
						var type = "";
						if(value['ChatType'] == 0){ type = "附近";}if(value['ChatType'] == 1){ type = "队伍";}
						if(value['ChatType'] == 2){ type = "私聊";}if(value['ChatType'] == 3){ type = "公会";}
						if(value['ChatType'] == 4){ type = "世界";}if(value['ChatType'] == 5){ type = "小喇叭";}
						if(value['ChatType'] == 6){ type = "好友聊天";}if(value['ChatType'] == 7){ type = "战场聊天";}
						if(value['ChatType'] == 8){ type = "MUD聊天室";}if(value['ChatType'] == 9){ type = "自动组队聊";}
						//alert(value.RoleName);
						var s = '<tr class=mgr_tr align=center ><td height=32 >'+value['LoginName']+'</td><td >'+value['RoleName']+'</td><td >'+type+'</td><td >'+value['ToName']+'</td><td >'+value['Content']+'</td><td >'+value['LogTime']+'</td><td >LogRoleChat_</td><td >[<a href="#" onclick = jingyanRole("'+value['RoleName']+'","'+qu+'","'+fu+'") >禁言角色</a>][<a href="#" onclick = jingyan("'+value['LoginName']+'","'+qu+'") >禁言账号</a>[<a href="#" onclick = dongjieRole("'+value['RoleName']+'","'+qu+'","'+fu+'") >冻结角色</a>][<a href="#" onclick = dongjie("'+value['LoginName']+'","'+qu+'") >冻结此帐号</a>]</td></tr>';
						$("#ajaxlist").append(s);
  					});
				}
			}
		});
	}
	else{
		$("#tt1").html("未检索到数据......");
		$("#ttt").show();
		$("#tabr").empty();
	}
}
/*//分页,当前页,总条数,每页几条,sql语句
function AjaxPage(page,pagenum,num,sql)
{
	var page = $page;
	var totalpage = pagenum/num;
	var total = pagenum;
	//上一页
	var previous = page - 1;

	//下一页
	var next = page;
	if(totalpage > page)
	{
		next = page+1;
	}

	if(total <= num)
	{
		var pagetxt = '<div class="page_info">共<span>'+totalpage+'</span>页<span>'+total+'</span>条记录</div>';
	}
	else
	{	
		//正常的分页链接
		
		//显示首页的裢接
		if(page > 1)
		{
			pagetxt += '<a href="javascript:;" onclick="PageL(\'1\',\''+$tid+'\',\''+$flag+'\',\''+$keyword+'\')">&lt;&lt;</a>';
			pagetxt += '<a href="javascript:;" onclick="PageL(\''.$previous.'\',\''.$tid.'\',\''.$flag.'\',\''.$keyword.'\')">&lt;</a>';
		}
		else
		{
			pagetxt .= '<a href="javascript:;">&lt;&lt;</a>';
			pagetxt .= '<a href="javascript:;">&lt;</a>';
		}
		
		//如果分页小于10页则显示正常分页链接否则显示带省略号的分页链接
		if($this->totalpage < 10)
		{
			//显示数字页码
			for($i=1; $i <= $this->totalpage; $i++)
			{
				if($this->page == $i)
				{
					$pagetxt .= '<a href="javascript:;" class="on">'.$i.'</a>';
				}
				else
				{
					$pagetxt .= '<a href="javascript:;" onclick="PageList(\''.$i.'\',\''.$tid.'\',\''.$flag.'\',\''.$keyword.'\')" class="num">'.$i.'</a>';
				}
			}
		}
		else
		{
			//如果页面为前三页则显示1到7页链接
			if($this->page==1 or $this->page==2 or $this->page==3)
			{
				$m = 1;
				$b = 7;
			}
	
			//如果页面大于前三页并且小于后三页则显示当前页前后各三页链接
			if ($this->page>3 and $this->page<$this->totalpage-2)
			{
				$m = $this->page-3;
				$b = $this->page+3;
			}
	
			//如果页面为最后三页则显示最后7页链接
			if($this->page==$this->totalpage or $this->page==$this->totalpage-1 or $this->page==$this->totalpage-2)
			{
				$m = $this->totalpage-7;
				$b = $this->totalpage;
			}
			if($this->page > 4)
			{
				$pagetxt .= '<a href="javascript:;">...</a>';
			}
	
			//显示数字页码
			for($i=$m; $i <= $b; $i++)
			{
				if($this->page == $i)
				{
					$pagetxt .= '<a href="javascript:;" class="on">'.$i.'</a>';
				}
				else
				{
					$pagetxt .= '<a href="javascript:;" onclick="PageList(\''.$i.'\',\''.$tid.'\',\''.$flag.'\',\''.$keyword.'\')" class="num">'.$i.'</a>';
				}
			}
			if($this->page < $this->totalpage-3)
			{
				$pagetxt .= '<a href="javascript:;">...</a>';
			}
		}

		//显示下一页的裢接
		if($this->page < $this->totalpage)
		{
			$pagetxt .= '<a href="javascript:;" onclick="PageList(\''.$next.'\',\''.$tid.'\',\''.$flag.'\',\''.$keyword.'\')">&gt;</a>';
			$pagetxt .= '<a href="javascript:;" onclick="PageList(\''.$this->totalpage.'\',\''.$tid.'\',\''.$flag.'\',\''.$keyword.'\')">&gt;&gt;</a>';
		}
		else
		{
			$pagetxt .= '<a href="javascript:;">&gt;</a>';
			$pagetxt .= '<a href="javascript:;">&gt;&gt;</a>';
		}

		$pagetxt .= '</div>';  
	}
	
	return $pagetxt;
}
*/
</script>
<style type="text/css">
body
{
	margin:0 auto;
	padding:0 auto;
	background:#FFF;
}
.tcss
{
	border:0px;
	margin-left:70px;
}
</style>
</head>

<body>
<div class="mgr_header"> <span class="title">聊天信息管理</span>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<div class="mgr_divt" id="ttt">
<span id="tt1" style="color:#F00; font-size:16px; font-weight:bold; margin-left:180px;"></span>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择·······大区：<select name="dq" id="dq" onchange="xf()">
        <option value="-请选择-" selected="selected">-请选择-</option>
        <?php
        $dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
        while($row=$dosql->GetArray()){?>
        				<option value="<?php echo $row['regid']; ?>"><?php echo $row['regname']; ?></option>
        <?php } ?> d
                     </select><span style="color:#F00" id="qu"></span>&nbsp;&nbsp;
    		&nbsp;&nbsp;&nbsp;服务器：<select name="fw" id="fw" >
        	 </select>&nbsp;&nbsp;&nbsp;
			角色名：<input type="text" id="us" name="us"  onblur="jc()" />
            <span style="color:#F00" id="us1"></span>&nbsp;&nbsp;&nbsp;
       	 	关键字/内容：<input type="text" id="key" name="key" onblur="gjz()" />
            <span style="color:#F00" id="key1"></span>
        </td>
     </tr>
</table>
</div>
<div class="mgr_divt" id="sea">
<table width="100%" height="100%">
	<tr class="tcss">
		<td>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
          开始时间：<input name="time" type="text" id="posttime" class="input_short" onblur="stime()" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script> <span style="color:#F00" id="sta"></span>
         结束时间：<input name="time1" type="text" id="posttime1" class="input_short" />
				<script type="text/javascript">
				date = new Date();
				Calendar.setup({
					inputField     :    "posttime1",
					ifFormat       :    "%Y-%m-%d %H:%M:%S",
					showsTime      :    true,
					timeFormat     :    "24"
				});
				</script>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<input type="button" id="s1" title="搜索" value="搜索" onclick="sear()" style="width:50px; height:22px;"/> 
                &nbsp;&nbsp;<span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table></div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">		
</table>
<div class="page_list">
</div>
</body>
</html>