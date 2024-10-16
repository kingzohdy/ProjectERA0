<?php require_once(dirname(__FILE__).'/../inc/config.inc.php'); ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>列表信息</title>
<link href="style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript">
	var i=0;
	var j=0;
function yz()
{
	var ym="<?php echo $_SESSION['pw']; ?>";
	var sym=$("#ypw").val();
	if(ym != sym){
		$("#ypw1").html("密码错误！");
	}
	if(ym == sym){
		$("#ypw1").html("验证正确");
		i=1;
	}
}
function yz1()
{
	var xpw1=$("#pw").val();;
	var xpw2=$("#qr").val();
	if(xpw1 != xpw2){
		$("#qr1").html("密码不一致！");
	}
	if(xpw1 == xpw2){
		$("#qr1").html("验证正确");
		j=1;
	}
}
function tij(){
	yz();yz1();
	if(i==1 && j==1 ){
		$("#form").submit();
	}
}
</script>
</head>
<body>
<div class="gray_header"> <span class="title">修改账号</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="adminorlist_save.php">
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
        <tr>
			<td height="35" align="right">原密码：</td>
			<td><input type="password" id="ypw" class="class_input" onblur="yz()" />
				<span class="maroon" id="ypw1" >*</span>
			</td>
		</tr>
		<tr>
			<td height="35" align="right">新密码：</td>
			<td><input type="password" name="pw" id="pw" class="class_input" />
            <span class="maroon" id="pw1">*</span></td>
		</tr>
        <tr>
			<td height="35" align="right">确认输入：</td>
			<td><input type="password" name="qr" id="qr" class="class_input" onblur="yz1()" />
            <span class="maroon" id="qr1">*</span></td>
		</tr>
	</table>
	<div style="margin-top:20px; margin-left:200px;">
		<input type="button" class="blue_submit_btn" onclick="tij()" />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
		<input type="hidden" name="action" id="action" value="update1" />
	</div>
</form>
</body>
</html>