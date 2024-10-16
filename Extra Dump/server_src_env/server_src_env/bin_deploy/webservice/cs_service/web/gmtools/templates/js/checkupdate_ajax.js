var xmlHttp;

function xmlhttprequest(){
	if(window.ActiveXObject){
		xmlHttp = new ActiveXObject('Microsoft.XMLHTTP');
	}
	else if(window.XMLHttpRequest){
		xmlHttp = new XMLHttpRequest();
	}
	else{
		alert('您的浏览器不支持Ajax技术！');
	}
}

//发送显示请求
function GetUpdateList(uptime,cfg_soft_lang)
{
	xmlhttprequest();
	var url = "check_update_do.php?"+parseInt(Math.random()*(15271)+1)+"&uptime="+uptime+"&cfg_soft_lang="+cfg_soft_lang;
	xmlHttp.open("GET", url, true);
	xmlHttp.onreadystatechange = ReturnUpdateList;
	xmlHttp.send(null);
}

//输出回收站内容
function ReturnUpdateList()
{
	if(xmlHttp.readyState == 4 && xmlHttp.status == 200)
	{
		document.getElementById('update_list').innerHTML = xmlHttp.responseText;
	}
}