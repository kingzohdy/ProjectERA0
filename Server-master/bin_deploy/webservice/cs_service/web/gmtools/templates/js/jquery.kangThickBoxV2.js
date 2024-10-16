/*！

* jQuery kangThickBox plugin
*
* Copyright (c) 2011 Kang.LG
*
* Date:2011-11-9
*/
(function ($)
{
    $.fn.kangPosition = function (options)
    {
        var options = $.extend({
            mode: null
        },
        options || {});
        switch (options.mode)
        {
            default:
            case "center":
                var ow = $(this).outerWidth(),
                oh = $(this).outerHeight();
                //alert("window:" + $(window).scrollTop() + ";document:" + $(document).scrollTop());
                $(this).css({
                    left: ($(window).scrollLeft() + Math.max(($(window).width() - ow) / 2, 0)) + "px",
                    //top: ($(window).scrollTop() + Math.max(($(window).height() - oh) / 2, 0)) + "px"
					top:"120px"
                });
                break;
            case "auto":
                break;
            case "fixed":
                break;
        }
    }
})(jQuery); 
(function ($)
{
    $.fn.kangThickBox = function (options, callback/*初始化时回调事件*/, closeCallback/*关闭弹出层时回调事件*/)
    {
        if (typeof options == "function")
        {
            if (typeof callback == "function")
            {
                closeCallback = callback;
            }
            callback = options;
            options = {};
        }
        var options = $.extend({
            type: "html",
            source: null,
            width: null,
            height: null,
            _titleOn: true,             //是否显示头部标题
            title: null,                //头部标题
			_classname:'',				//标题span的样式
            _title: "",
            _frame: "",
            _div: "",
            _box: "",
            _con: "",
            _loading: "thickloading",
            close: false,
            _close: "",                 //值为null，则取消默认关闭按钮
            _close_val: "×",
            _fastClose: false,           //点击弹出层外或用Esc键，快速关闭弹出层
            _autoReposi: false,         //自动重新定位（如：窗口缩放及窗口滚动）
            _countdown: false           //倒计时自动关闭弹出层（0 or false表示不自动关闭，其他请设置倒计时时间[如：5，表示5秒后自动关闭]）
        }, options || {});
        var _object = (typeof this != "function") ? $(this) : null;
        var _timer;
        var _isIE6 = $.browser.msie && parseInt($.browser.version, 10) == 6;
        //_isIE6 = true;
        var close = function ()
        {
            clearInterval(_timer);
            //$(".thickframe").add(".thickdiv").hide(); //隐藏遮盖层及阴影层
            //$(".thickbox").remove();
            $(".thickbox").animate({ top: "-=30px", opacity: "0.2" }, 300, function () { $(this).remove(); $(".thickframe").add(".thickdiv").hide(); });
            if (options._autoReposi)
            {
                //$(window).unbind("resize.kangThickBox").unbind("scroll.kangThickBox")
                $(window).add(document).unbind(".kangThickBox");
            }
            else if(_isIE6){
                $(window).add(document).unbind(".kangThickBox");
            }
            if (closeCallback) closeCallback();
        }

        if (options.close)
        {
            close();
            return false;
        };

        var reg = function (str)
        {
            if (str != "")
            {
                return " id='" + str.match(/\w+/) + "' ";
            }
            else
            {
                return "";
            }
        };
        var init = function (element)
        {
            if ($(".thickdiv").length == 0)
            {
                if (_isIE6)
                {
                    $("<iframe class='thickframe' " + reg(options._frame) + " marginwidth='0' marginheight='0' frameborder='0' scrolling='no'></iframe>").appendTo($(document.body));
                }
                $("<div class='thickdiv' " + reg(options._div) + "></div>").appendTo($(document.body));
            }
            else
            {
                reposiIE6frame();
                $(".thickdiv").add(".thickframe").show();
            };
            $("<div class='thickbox' " + reg(options._box) + "></div>").appendTo($(document.body));
            if (options._titleOn) initTitle(element);
            $("<div class='thickcon " + options._loading + "' " + reg(options._con) + " style='width:" + options.width + "px;height:" + options.height + "px;'></div>").appendTo($(".thickbox"));
            if (options._countdown) initCountdown();
            //$(".thickcon").addClass(options._loading);
            reposi();
            initClose();
            inputData(element);
            if (options._autoReposi)
            {
                $(window).bind("resize.kangThickBox", reposi).bind("scroll.kangThickBox", reposi);
            }
            else if(_isIE6){
                $(window).bind("resize.kangThickBox", reposiIE6frame).bind("scroll.kangThickBox", reposiIE6frame);
            }
            if (options._fastClose)
            {
                $(document).bind("click.kangThickBox",
                function (e)
                {
                    e = e ? e : window.event;
                    var tag = e.srcElement ? e.srcElement : e.target;
                    if (tag.className == "thickdiv")
                    {
                        //$(this).unbind("click.kangThickBox");
                        close();
                    }
                });
                $(document).bind('keydown.kangThickBox', function (e)
                {
                    var key = e.keyCode;
                    if (key === 27)
                    {
                        e.preventDefault();
                        close();
                    }
                });
            }
        };

        //初始化标题栏
        var initTitle = function (element)
        {
            options.title = (options.title == null && element) ? element.attr("title") : options.title;
            $("<div class='thicktitle' " + reg(options._title) + " style='width:" + options.width + "'><span class='"+ options._classname+"'>" + options.title + "</span></div>").appendTo($(".thickbox"))
        };
        //初始化关闭事件
        var initClose = function ()
        {
            if (options._close != null)
            {
                $("<a href='javascript:void(0);' class='thickclose' " + reg(options._close) + ">" + options._close_val + "</a>").appendTo($(".thickbox"));
                $(".thickclose").one("click",
                function ()
                {
                    close();
                    return false;
                })
            }
        };
        //剩余多少时间自动关闭
        var initCountdown = function ()
        {
            var x = options._countdown;
            $("<div class='thickcountdown' style='width:" + options.width + "'><span id='kang-countdown'>" + x + "</span>秒后自动关闭</div>").appendTo($(".thickbox"));
            _timer = setInterval(function ()
            {
                x--;
                $("#kang-countdown").html(x);
                if (x == 0)
                {
                    close();
                }
            },
            1000);
        };
        //[重新定位]计算弹出层宽高及弹出位置
        var reposi = function ()
        {
            var w1 = $(".thickcon").outerWidth(),
            h1 = (options._titleOn ? $(".thicktitle").outerHeight() : 0) + $(".thickcon").outerHeight();
            $(".thickbox").css({
                width: w1 + "px",
                height: h1 + "px"
            });
            $(".thickbox").kangPosition({
                mode: "center"
            });
            reposiIE6frame();
        };
        var reposiIE6frame = function(){
            if (_isIE6)
            {
                $(".thickdiv").add(".thickframe").css({
                    width: width(),
                    height: height()
                });
            }
        }
        var height = function ()
        {
            var scrollHeight,
			offsetHeight;
            // handle IE 6
            if (_isIE6)
            {
                scrollHeight = Math.max(document.documentElement.scrollHeight,document.body.scrollHeight);
                offsetHeight = Math.max(document.documentElement.offsetHeight,document.body.offsetHeight);
                if (scrollHeight < offsetHeight)
                {
                    return $(window).height() + 'px';
                } else
                {
                    return scrollHeight + 'px';
                }
                // handle "good" browsers
            } 
            else
            {
                return $(document).height() + 'px';
            }
        };

        var width = function ()
        {
            var scrollWidth,
			offsetWidth;
            // handle IE
            if ($.browser.msie)
            {
                scrollWidth = Math.max(document.documentElement.scrollWidth,document.body.scrollWidth);
                offsetWidth = Math.max(document.documentElement.offsetWidth,document.body.offsetWidth);
                
                if (scrollWidth < offsetWidth)
                {
                    return $(window).width() + 'px';
                } 
                else
                {
                    return scrollWidth + 'px';
                }
                // handle "good" browsers
            } else
            {
                return $(document).width() + 'px';
            }
        };
        if (options.type == "image")
        {
            var _imgPreloader = new Image();
        }
        var preloaderImg = function (l/*图片总数量*/)
        {
            $(".thickcon").html('');
            _imgPreloader.onerror = function ()
            {
                $(".thickcon").html("<div style='text-align:center;font-size:14px;line-height:" + options.height + "px'>图片加载失败，请稍候重试...</div>");
            }
            _imgPreloader.onload = function ()
            {
                _imgPreloader.onerror = _imgPreloader.onload = null;
                var imgsize = resizeImg(_imgPreloader);
                var sty = (l != 1 ? "cursor:pointer;" : "") + "margin-top:" + (options.height - imgsize.height) / 2 + "px;width:" + imgsize.width + "px;height:" + imgsize.height + "px";
                $("<img>", {
                    "src": options.source.get(options._index),
                    style: sty,
                    click: function ()
                    {
                        if (l > 1)
                        {
                            if (options._index + 1 == l)
                            {
                                options._index = 0;
                            }
                            else
                            {
                                options._index++;
                            }
                            $('#kang-imgcount').text(options._index + 1);
                            preloaderImg(l);
                        }
                    }
                }).appendTo(".thickcon");
            };

            _imgPreloader.src = options.source.get(options._index);
        }
        var inputData = function (element)
        {
            options.source = (options.source == null) ? element.attr("href") : options.source;
            switch (options.type)
            {
                default:
                case "html":
                    $(".thickcon").html(options.source);
                    $(".thickcon").removeClass(options._loading);
                    if (callback)
                    {
                        callback();
                    };
                    break;
                case "image":
                    var len = options.source.length;
                    if (len > 1)
                    {
                        $("<div class='thickimgcount' style='width:" + options.width + "'><span id='kang-imgcount'>" + (options._index + 1) + "</span>/" + len + "</div>").appendTo($(".thickbox"));
                    }
                    preloaderImg(len);

                    //$(".thickcon").removeClass(options._loading);
                    if (callback)
                    {
                        callback();
                    };
                    break;
                case "ajax":
                case "json":
                    if (callback)
                    {
                        callback(options.source, $(".thickcon"),
                    function ()
                    {
                        $(".thickcon").removeClass(options._loading);
                    })
                    };
                    break;
                case "iframe":
					//$('<iframe scrolling="no" frameborder="0" width="500px" height="372px" src="http://chongzhi.360buy.com/index-4.htm"></iframe>').appendTo($(".thickcon"));
					$height = options.height - 3;
                    $("<iframe src='" + options.source + "' marginwidth='0' marginheight='0' width='98%' height='" + $height + "' frameborder='0' scrolling='no' allowtransparency></iframe>").appendTo($(".thickcon"));
                    $(".thickcon").removeClass(options._loading);
                    if (callback)
                    {
                        callback()
                    };
                    break
            }
        };
        var resizeImg = function (imgObj)
        {
            var _imgSize = {};
            var MAX_WIDTH = options.width;
            var MAX_HEIGHT = options.height;
            var ASPECT_RATIO = MAX_WIDTH / MAX_HEIGHT; //宽高比
            var originalWidth = imgObj.width;
            var originalHeight = imgObj.height;
            var newWidth, newHeight;
            if (originalWidth > MAX_WIDTH || originalHeight > MAX_HEIGHT)
            {
                var factor;
                if (originalWidth / originalHeight > ASPECT_RATIO)
                {
                    factor = originalWidth / MAX_WIDTH;
                    _imgSize.width = parseInt(originalWidth / factor);
                    _imgSize.height = parseInt(originalHeight / factor);
                }
                else
                {
                    factor = originalHeight / MAX_HEIGHT;
                    _imgSize.width = parseInt(originalWidth / factor);
                    _imgSize.height = parseInt(originalHeight / factor);
                }
            }
            else
            {
                _imgSize.width = imgObj.width;
                _imgSize.height = imgObj.height;
            }
            return _imgSize
        }
		if($('.thickbox').size())
		{
			$('.thickframe,.thickdiv,.thickbox').remove();
			//close();
		}
        if (_object != null)
        {
            _object.click(function ()
            {
                init($(this));  //1、当options.title==null，则以当前所选元素属性title作为标题。2、options.source==null，则以当前所选元素属性href作为信息来源
                return false;
            });
        }
        else
        {
            init();
        }

    };
    $.kangThickBox = $.fn.kangThickBox;
})(jQuery);

function kangThickBoxclose()
{
    $(".thickclose").trigger("click");
};