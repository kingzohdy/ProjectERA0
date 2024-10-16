<?php 
	require_once(dirname(__FILE__).'/../inc/config.inc.php'); 
//检测用户是否有权限
if($_SESSION['admin1'] == "")
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
<title>列表信息管理</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<style type="text/css">
 a{ color:#00F;}
.thickdiv{position:fixed;top:0;left:0;z-index:10000001;width:100%;height:100%;background:#F7F7F7;border:0;filter:alpha(opacity=70);opacity:0.70;}
.thickbox{position:absolute;z-index:100000002;overflow:hidden;box-shadow:0 10px 8px #6c6c6c; background-color:#fff;}
.thicktitle{height:30px;padding:0 15px; margin-top:8px; text-align:left;line-height:27px;font-family:arial,"宋体";font-size:14px;font-weight:bold;color:#F00;}

.thickclose:link,.thickclose:visited{display:block;position:absolute;z-index:100000;top:10px;right:12px;overflow:hidden;width:21px;height:21px;background:url(../images/close_ico_dis.gif) no-repeat 0 3px;font-size:0;line-height:100px;}
.thickclose:hover{ background-position:left -18px;}
.thickcon{overflow:auto;padding:10px;text-align:center; vertical-align:top;}
</style>
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="js/mgr.func.js"></script>
<script type="text/javascript" src="js/jquery.kangThickBoxV2.js" ></script>
<script type="text/javascript" >
var d;
$(document).ready(function(){
		$("#xdq").empty();
		$.ajax({
		url : "soap.php",
		type:'post',
		dataType:'json',
		data: {"fwq": "z"},
		success:function(msg){
 			$.each(msg, function (index, value) {
 				$("#xdq")[0].options.add(new Option(msg.ACCOUNTCONNINFO.RegionID, msg.ACCOUNTCONNINFO.RegionName, false, false));
  			});
		}});
		var c = "<option value='不指定' selected='selected' >不指定</option>";
		$("#xdq").append(c);
});
function fck()
{
	$.kangThickBox({
		type: "iframe",
		title:"修改安全锁密码",
		source: 'ContainerLock.php',
		width: 240,
		height: 100,
		_autoReposi: true
	});
}
function mingxi(type)
{
	var tp=type;//代表账户类型()
	$.ajax({
  			type: 'POST',
 			url: 'soap.php',
			dataType: 'json',
  			data: {"a": "z","b":12,"c":13,"d":14}, 
  			success: function(msg){
				$("#ajaxlist").empty();
				$.each(msg, function (index, value) {
					d ='<tr align=center class=mgr_tr onmouseover=this.className=mgr_tr_on onmouseout=this.className=mgr_tr><td height=32 width="12%" >kls</td><td width="12%" >jianfank</td><td width="15%" style=color:#F00; >'+msg.ErrorMessage+'</td><td width="15%" >终极测服</td><td width="6%" >无</td><td width="20%" >[<a href="roleSelect.php?roleUid='+msg.ErrorMessage+'">详细</a>][<a href=# >踢下线</a>][<a href="#" >禁言</a>]';
					if("高级游戏管理员"==tp)
					{
						d += '[<a href="#" >冻结</a>][<a href="#" class="box a2" onclick="fck()"  >改安全锁</a>]';
					}
					d += '</td></tr>';
				});
				$("#ajaxlist").append(d);
				//setTimeout('myrefresh()',2000); 
				//	window.location.reload(); 
				
			},
			error:function(msg){
				alert(msg+'<br/>发送数据出错，请重新发送');
			}
		});
		
}
</script>
</head>
<body>
<div id="DialogDiv" style="display:none">
	<div style="background:#FFF; border:1px solid #39B8C6; height:370px; width:450px;">
    <center style=" margin-top:5px;font-size:16px">订单详情</center>
    <br />
    <table style="padding:5px;" width="98%" id="mx">
    </table>
    </div>
</div>
<div class="mgr_divt">
<table width="100%" height="100%">
	<tr>
         <td style=" padding:0px; margin:0px;vertical-align:top; ">
				<div id="search" class="search" style="float:left;vertical-align:middle;"> <span class="s">
				<input style="float:left;vertical-align:middle;" name="account"  id="account" type="text" title="输入角色进行搜索"/>
				</span> <span class="b"><a href="javascript:;" onclick="mingxi('<?php echo $_SESSION['admin1']; ?>')"><img src="../images/search_btn.png" title="搜索" /></a></span></div>
            <span style="color:#F00" id="us1"></span>
        </td>
     </tr>
</table>
</div>
<div id="list_area">

<!--/header end-->
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist1">
		<tr class="thead2" align="center">
                <td width="12%">角色名</td>
                <td width="12%">账号</td>
                <td width="15%">大区</td>
                <td width="15%">服</td>
				<td width="8%">是否在线</td>
                <td width="20%">操作</td>
		</tr>
 </table>
 <table width="100%" border="0" cellpadding="0" cellspacing="0" class="mgr_table" id="ajaxlist">
  <form action="#" method="post" id="form">
   </form>                                  
</table>
<div class="mgr_divb"> 
</div>
<div class="page_area">
	<?php //echo $dopage->AjaxPage(); ?>
</div>

<!--/list end-->
</div>
</body>
</html>