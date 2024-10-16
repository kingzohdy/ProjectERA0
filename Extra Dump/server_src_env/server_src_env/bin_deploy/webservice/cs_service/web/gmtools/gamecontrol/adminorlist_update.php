<?php 
require_once(dirname(__FILE__).'/../inc/config.inc.php');
//检测用户是否有权限
if($_SESSION['admin1'] != "超级游戏管理员")
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
<title>修改列表信息</title>
<link href="../templates/style/admin.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/jquery.min.js"></script>
<script type="text/javascript" src="../templates/js/getuploadify.js"></script>
<script type="text/javascript" src="../templates/js/checkf.func.js"></script>
<script type="text/javascript" src="../templates/js/getjcrop.js"></script>
<script type="text/javascript" src="../editor/kindeditor-min.js"></script>
<script type="text/javascript" src="../editor/lang/zh_CN.js"></script>
</head>
<body>
<?php
$row = $dosql->GetOne("SELECT * FROM `#@__adminor` WHERE id=$id");
?>
<div class="gray_header"> <span class="title">修改列表信息</span> <span class="reload"><a href="javascript:location.reload();">刷新</a></span> </div>
<form name="form" id="form" method="post" action="adminorlist_save.php" onsubmit="return cfm_infolm();">
	<table width="100%" border="0" cellspacing="0" cellpadding="0" class="form_table">
		<tr>
			<td height="35" align="right">类型：</td>
			<td><select name="admintype">
            		<option value="3"  <?php if($row['prem']==3){ echo 'selected=selected'; }?> > 高级游戏管理员</option>
                    <option value="4" <?php if($row['prem']==4){ echo 'selected=selected'; }?> >游戏管理员</option>
            	</select>
				<span class="maroon">*</span>
			</td>
		</tr>
        <tr>
			<td height="35" align="right">用户名：</td>
			<td><input type="text" name="adm" id="title" class="class_input" value="<?php echo $row['username']; ?>"  />
				<span class="maroon">*</span>
			</td>
		</tr>
		<tr>
			<td height="35" align="right">密码：</td>
			<td><input type="text" name="pw" id="author" class="class_input" />
            <span class="maroon">*</span></td>
		</tr>
	</table>
	<div class="subbtn_area">
		<input type="submit" class="blue_submit_btn" value="" />
		<input type="button" class="blue_back_btn" value="" onclick="history.go(-1)"  />
		<input type="hidden" name="action" id="action" value="update" />
		<input type="hidden" name="id" id="id" value="<?php echo $id; ?>" />
		<input type="hidden" name="tid" id="tid" value="<?php echo ($tid = isset($tid) ? $tid : ''); ?>" />
	</div>
</form>
</body>
</html>