<?php	if(!defined('IN_PHPMYWIND')) exit('Request Error!');

/*
 * 分页类
 *
**************************
(C)2010-2012 phpMyWind.com
update: 2011-4-26 15:00:19
person: Feng
**************************
*/


$dopage = new Page();

class Page
{
	var $page;      //当前页码
	var $totalpage; //总共页数
	var $pagenum;   //每页记录数
	var $total;     //总共记录数

    function __construct()
    {
		$this->Init();
    }

    function Page()
    {
		$this->__construct();
    }
	
	function Init()
    {
		$this->page = @$GLOBALS['page'];
    }

	//获取分页变量
	function GetPage($sql,$pagenum=20)
	{
		global $dosql;

		$dosql->Execute($sql);
		$this->total = $dosql->GetTotalRow();

		$this->pagenum = $pagenum;

		$this->totalpage = ceil($this->total / $this->pagenum);

		if(!isset($this->page) || !intval($this->page) || $this->page > $this->totalpage)
		{
			$this->page = 1;
		}

		$startnum = ($this->page-1) * $this->pagenum;

		$sql .= " limit $startnum, $this->pagenum";

		return $dosql->Execute($sql);
	}

	//显示分页列表
	function GetList()
	{
		global $cfg_isreurl;

		$pagetxt = '';

		if($this->total <= $this->pagenum)
		{
			$pagetxt = '<div class="page_info">共<span>'.$this->totalpage.'</span>页<span>'.$this->total.'</span>条记录</div>';
		}

		else
		{
			//获取除page参数外的其他参数
			$query_str = explode('&',$_SERVER["QUERY_STRING"]);

			if($query_str[0] != '')
			{
				$query_strs = '';

				foreach($query_str as $k)
				{
					$query_str_arr = explode('=', $k);

					if(strstr($query_str_arr[0],'page') == '')
					{
						$query_str_arr[0] = isset($query_str_arr[0]) ? $query_str_arr[0] : '';
						$query_str_arr[1] = isset($query_str_arr[1]) ? $query_str_arr[1] : '';

						//伪静态设置
						if($cfg_isreurl != 'Y')
						{
							$query_strs .= $query_str_arr[0].'='.$query_str_arr[1].'&';		
						}
						else
						{
							$query_strs .= '-'.$query_str_arr[1];	
						}
					}
				}
		
				$nowurl = '?'.$query_strs;
			}
			else
			{
				$nowurl = '?';
			}
			
			//伪静态设置
			if($cfg_isreurl == 'Y')
			{
				$request_arr  = explode('/',$_SERVER['REQUEST_URI']);
				$request_cont = count($request_arr);
				$request_rui  = explode('-', $request_arr[$request_cont-1]);
				$nowurl = ltrim($request_rui[0],'/').ltrim($nowurl,'?');
			}

			$previous = $this->page - 1;
			if($this->totalpage == $this->page) $next = $this->page;
			else $next = $this->page + 1;

			$pagetxt = '<div class="page_list">';

			//上一页 第一页
			if($this->page > 1)
			{
				//伪静态设置
				if($cfg_isreurl != 'Y')
				{
					$pagetxt .= '<a href="'.$nowurl.'page=1">&lt;&lt;</a>';
					$pagetxt .= '<a href="'.$nowurl.'page='.$previous.'">&lt;</a>';
				}
				else
				{
					$pagetxt .= '<a href="'.$nowurl.'-1.html">&lt;&lt;</a>';
					$pagetxt .= '<a href="'.$nowurl.'-'.$previous.'.html">&lt;</a>';
				}
			}
			else
			{
				$pagetxt .= '<a href="javascript:;">&lt;&lt;</a>';
				$pagetxt .= '<a href="javascript:;">&lt;</a>';
			}

			//当总页数小于10
			if($this->totalpage < 10)
			{
				for($i=1; $i <= $this->totalpage; $i++)
				{
					if($this->page == $i)
					{
						$pagetxt .= '<a href="javascript:;" class="on">'.$i.'</a>';
					}
					else
					{
						//伪静态设置
						if($cfg_isreurl != 'Y')
						{
							$pagetxt .= '<a href="'.$nowurl.'page='.$i.'" class="num">'.$i.'</a>';
						}
						else
						{
							$pagetxt .= '<a href="'.$nowurl.'-'.$i.'.html" class="num">'.$i.'</a>';
						}
					}
				}
			}
			else
			{
				if($this->page==1 or $this->page==2 or $this->page==3)
				{
					$m = 1;
					$b = 7;
				}
				//如果页面大于前三页并且小于后三页则显示当前页前后各三页链接
				if($this->page>3 and $this->page<$this->totalpage-2)
				{
					$m = $this->page-3;
					$b = $this->page+3;
				}
				//如果页面为最后三页则显示最后7页链接
				if($this->page==$this->totalpage or $this->page==$this->totalpage-1 or $this->page==$this->totalpage-2)
				{
					$m = $this->totalpage - 7;
					$b = $this->totalpage;
				}
				if($this->page > 4)
				{
					$pagetxt .= '<a href="javascript:;">...</a>';
				}
				//显示数字页码
				for($i=$m; $i<=$b; $i++)
				{
					if($this->page == $i)
					{
						$pagetxt .= '<a href="'.$nowurl.'page='.$i.'" class="on">'.$i.'</a>';
					}
					else
					{
						//伪静态设置
						if($cfg_isreurl != 'Y')
						{
							$pagetxt .= '<a href="'.$nowurl.'page='.$i.'" class="num">'.$i.'</a>';
						}
						else
						{
							$pagetxt .= '<a href="'.$nowurl.'-'.$i.'.html" class="num">'.$i.'</a>';
						}
					}
				}
				if($this->page < $this->totalpage-3)
				{
					$pagetxt .= '<a href="javascript:;">...</a>';
				}
			}

			//下一页 最后页
			if($this->page < $this->totalpage)
			{
				//伪静态设置
				if($cfg_isreurl != 'Y')
				{
					$pagetxt .= '<a href="'.$nowurl.'page='.$next.'">&gt;</a>';
					$pagetxt .= '<a href="'.$nowurl.'page='.$this->totalpage.'">&gt;&gt;</a>';
				}
				else
				{
					$pagetxt .= '<a href="'.$nowurl.'-'.$next.'.html">&gt;</a>';
					$pagetxt .= '<a href="'.$nowurl.'-'.$this->totalpage.'.html">&gt;&gt;</a>';
				}
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
}
?>