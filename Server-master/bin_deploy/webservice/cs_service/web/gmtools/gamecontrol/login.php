<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>零纪元后台登录</title>
<title>后台登录</title>
<link href="style/login.css" rel="stylesheet" type="text/css" />
<script src="js/jquery.min.js" type="text/javascript"></script>
<script type="text/javascript">
function CheckForm() {
	if ($.trim($('#username').val()) == '' || $.trim($('#password').val()) == '') {
		$('#tip').html('错　误：账号和密码不能为空！');
		return false;
	}
}
</script>
</head>
<body>
<noscript>
        <div id="divNoscript">
            <img src="/Admin/Image/adminLogin/plaint.png" alt="" />
            <h2>零纪元后台管理系统友情提示：</h2>
            <p>Your browser must support javascript.</p>
            <p>你的浏览器必须支持javascript才能使用该系统。</p>
        </div>
        <style type="text/css">#container{display: none;}body,html{background-image:none;background-color:#FFFF99;}h2{color:Red;}</style>
    </noscript>
    <form name="form" id="form" method="post" action="dudge.php" onsubmit="return CheckForm()">
    <div id="container">
        <div class="login">
            <ul>
                <li id="tip" runat="server"></li>
                <li><label for="username">帐　号：</label><input type="text" id="username" name="username" class="classOne" maxlength="30" onblur="this.className='classOne'" onfocus="this.className='changClassName'" autocomplete="off" /></li>
                <li><label for="password">密　码：</label><input type="password" id="password" name="password" maxlength="30" class="classOne" onblur="this.className='classOne'" onfocus="this.className='changClassName'" /></li>
                <li><input type="submit" value="登录" class="submitbt"/></li>
                <input type="hidden" name="dopost" value="login" />
            </ul>
        </div>
    </div>
    </form>
</body>
</html>