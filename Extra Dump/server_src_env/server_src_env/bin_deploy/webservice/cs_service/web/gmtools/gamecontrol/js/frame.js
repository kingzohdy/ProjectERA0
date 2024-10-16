/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-6-15 11:22:22
person: Feng
**************************
*/

$(function(){

	//快捷菜单
	bindQuickMenu();

	//左侧菜单开关
	LeftMenuToggle();
	
	//锁屏按钮
	bindLockScreen();

	//打开功能地图
	bindFuncMap();

}).keydown(function(event){

	//快捷键
	if(event.keyCode == 27){
		location.href = 'logout.php';
	}
});


//快捷菜单
function bindQuickMenu(){
	$(".quick").bind("mouseover",function(){
		$(".quick .btn").addClass("on");
		$("#qucikmenu").slideDown("fast");
	}).bind("mouseout",function(){
		hidequcikmenu = setTimeout('$("#qucikmenu").slideUp("fast");$(".quick .btn").removeClass("on");',300);
		$(this).bind("mouseover",function(){clearTimeout(hidequcikmenu);});
	}).find("a").click(function(){
		$(this).blur();
		$("#qucikmenu").slideUp("fast");
	});
}


//左侧菜单开关
function LeftMenuToggle()
{
	$(".togglemenu").click(function(){
		if($(this).html() == "隐藏菜单"){
			$(".left").animate({width:"0px"},300,function(){$(this).hide()});
			$(".right").animate({left:"0px"},300);
			$(this).html("显示菜单");
			$("body").attr("class","hidemenu");
		}
		else{
			$(".left").show().animate({width:"207px"},300);
			$(".right").animate({left:"207px"},300);
			$(this).html("隐藏菜单");
			$("body").attr("class","showmenu");
		}
	});
}


//锁屏函数
function bindLockScreen(){
	$(".lockscreen").bind("click",function(){
		$("body").append("<div id=\"lockscreen_bg\"> </div><div id=\"lockscreen_win\"><div id=\"lockscreen_pwd\"><label>密码：</label><input type=\"password\" id=\"password\" /><input id=\"lockscreen_btn\" type=\"image\" src=\"templates/images/lockscreen_enter.png\" onclick=\"CheckPwd()\" /><div id=\"lockscreen_note\"></div></div></div><div id=\"lockscreen_copy\">© 2012 phpMyWind.com</div>")
		$("#lockscreen_bg, #lockscreen_win, #lockscreen_copy").css("display","block");
		$.get("lockscreen_do.php",{action:"lock"});
	});
}


//锁屏密码检测
function CheckPwd()
{
	if($("#password").val() == ''){
		$("#lockscreen_note").html("请输入解锁密码！");
		setTimeout('$("#lockscreen_note").html("")',5000);
	}
	else{
		$.get("lockscreen_do.php",{action:"check",password:$("#password").val()},function(response,status,xhr){
			if(response == true){
				$("#lockscreen_bg, #lockscreen_win, #lockscreen_copy").remove();
			}
			else{
				$("#lockscreen_note").html("密码错误，请重新输入！");
				setTimeout('$("#lockscreen_note").html("")',5000);
			}
		});
	}
}


//功能地图
function bindFuncMap()
{
	$(".getfuncmap").bind("click",function(){
		$(".funcmap").show(); 
	});

	//关闭功能地图
	$(".Close").bind("click",function(){
		$(".funcmap").hide();						 
	});

	$(".funcmap a").bind("click",function(){
		$(".funcmap").hide();
	});
}
