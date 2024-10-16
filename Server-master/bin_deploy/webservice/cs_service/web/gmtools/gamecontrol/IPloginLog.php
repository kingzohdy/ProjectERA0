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
<title>输入IP查询账号--日志查询</title>
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
		$("#us1").html("请输入账号名");
		return;
	}
	if(stp != '')
	{
		$("#us1").html(""); 
		j=1;
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
	 if(i ==1 && j == 1 && t==1){
	var qu=$("#dq").val();
	var fu=$("#fw").val();
		 //选择服务器(用于选择数据库)
		var f = qu+'_'+fu;
		//时间(用于选择查询表)
		var tb=$("#posttime").val();
		var tb1=$("#posttime1").val();
		//帐号(用于查询)
		var ip=$("#us").val();
		
		$.ajax({
  			type: 'POST',
 			url: 'select_log.php',
			dataType: 'json',
  			data: {"acclogIP":"","ldb": f,"time":tb, "time1":tb1, "ip":ip}, 
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
				if("结束时间与开始时间相差不能超过7天"==msg)
				{
					$("#ajaxlist").empty();
					$("#tt1").html("结束时间与开始时间相差不能超过7天");
					$("#ttt").show();
					exit();
				}
				else
				{
					if(msg.nums == 0)
					{
						$("#tt1").html("未检索到数据......");
						$("#ttt").show();
						$("#ajaxlist").empty();
						exit;
					}
					$("#ttt").hide();
					$("#ajaxlist").empty();
					var ss='<tr class=thead2 align=center><td >账号</td><td >Uin</td><td >登录时间</td><td >Ip地址</td><td >客户端类型</td><td >MacAddr</td></tr>';
					$("#ajaxlist").append(ss);
					//处理检索到的数据......
					var row1 = msg.row1;
					var row2 = msg.row2;
					var nums = msg.nums;
					var nums1 = msg.nums1;
					var nums2 = msg.nums2;
					var denglv = "";
					var dengchu = "";
					var ii=0;
					var jj=0;
					for(var x=0;x<nums;x++)
					{
						if(ii<nums1)
						{
							denglv = row1[ii];
							var type = "";
							if(denglv.CltType == 0){ type = "空";}if(denglv.CltType == 1){ type = "pc客户端";}
							if(denglv.CltType == 2){ type = "web客户端";}if(denglv.CltType == 3){ type = "微端";}
							if(denglv.CltType == 99){ type = "VIP网吧";}
							//alert(value.RoleName);
							var s = '<tr class=mgr_tr align=center ><td height=32 >'+denglv.AccountLoginName+'</td><td height=32 >'+denglv.Uin+'</td><td >'+denglv.LogTime+'</td><td >'+denglv.IP+'</td><td >'+type+'</td><td >'+denglv.MacAddr+'</td></tr>';
							$("#ajaxlist").append(s);
							ii++;
						}
						
					}
					var dd = '<tr><td colspan=6 class=mgr_divt></td></tr>';
					$("#ajaxlist").append(dd);
				}
			}
		});
	}
	else{
		$("#tt1").html("未检索到数据......");
		$("#ttt").show();
		$("#ajaxlist").empty();
	}
}
function allPrpos ( obj )
{    
    var props = "";
    for ( var p in obj )
    {
        if ( typeof ( obj[p] ) == " function " )
        {
            //obj[p](); 
        } else {
            // p 为属性名称，obj[p]为对应属性的值
            props += p + " => " + obj[p] + "\n\n";
        }
    }
    alert ( props );
}
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
<div class="mgr_header"> <span class="title">帐号--登录记录</span>
    <span class="reload"><a href="javascript:location.reload();">刷新</a></span>
</div>
<div class="mgr_divt" id="ttt">
<span id="tt1" style="color:#F00; font-size:16px; font-weight:bold; margin-left:180px;"></span>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
    	<td style="width:180px"></td>
		<td> &nbsp;&nbsp;&nbsp;请选择·······大区：<select name="dq" id="dq" onchange="xf()">
        <option value="-请选择-" selected="selected">-请选择-</option>
        <?php
        $dosql->Execute("select distinct regid,regname from `reg_world` order by `regid` ");
        while($row=$dosql->GetArray()){?>
        				<option value="<?php echo $row['regid']; ?>"><?php echo $row['regname']; ?></option>
        <?php } ?> d
                     </select><span style="color:#F00" id="qu"></span>&nbsp;&nbsp;
    		&nbsp;&nbsp;&nbsp;服务器：<select name="fw" id="fw" >
        	 </select>&nbsp;&nbsp;&nbsp;
			IP：<input type="text" id="us" name="us"  onblur="jc()" />
            <span style="color:#F00" id="us1"></span>
        </td>
     </tr>
</table>
</div>
<div class="mgr_divt" id="sea">
<table width="100%" height="100%">
	<tr class="tcss">
    	<td style="width:160px"></td>
		<td>
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
				</script>&nbsp;
				<input type="button" id="s1" title="搜索" value="搜索" onclick="sear()" style="width:50px; height:22px;"/> 
                &nbsp;&nbsp;<span  name="ts" id="ts" style="color:#F00"></span>
        </td>
     </tr>
</table></div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">		
</table>
</body>
</html>