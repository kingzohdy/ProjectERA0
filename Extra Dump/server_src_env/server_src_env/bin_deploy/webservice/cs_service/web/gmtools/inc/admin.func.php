<?php	if(!defined('IN_PHPMYWIND')) exit('Request Error!');

/*
**************************
(C)2010-2012 phpMyWind.com
update: 2012-9-3 14:33:47
person: Feng
**************************
*/



/*
 * 获取栏目分类
 *
 * @access public
 * @param  $type   string  要显示的模型ID
 * @param  $id     int     区别记录集的ID
 * @param  $i      int     option缩进位数
 * @return         string  输出<select>
*/
function ShowInfoType($type=0, $id=0, $i=0)
{
	global $dosql;

	switch($type)
	{
		case 0:
		$tbname = '#@__info';
		break;
		case 1:
		$tbname = '#@__infolist';
		break;
		case 2:
		$tbname = '#@__infoimg';
		break;
		case 3:
		$tbname = '#@__soft';
		break;
		default:
		$tbname = '#@__info';
	}

	if(isset($_GET['id'])) $row2 = $dosql->GetOne("SELECT classid FROM `$tbname` WHERE id=".$_GET['id']);

	$dosql->Execute("SELECT * FROM `#@__infoclass` WHERE parentid=$id ORDER BY orderid ASC", $id);
	$i++;
	$parentarr = array();

	while($row = $dosql->GetArray($id))
	{
		if($row['id']==@$row2['classid'] or $row['id']==@$_GET['tid']) $selected = ' selected="selected" ';
		else $selected = '';

		if($row['infotype'] != $type) $disabled = ' disabled="disabled"';
		else $disabled = '';

		echo '<option value="'.$row['id'].'"'.$selected.$disabled.'>';
		for($p=1; $p<$i; $p++) echo '&nbsp;&nbsp;&nbsp;&nbsp;';
		if($row['parentid'] != 0) echo '|- ';
		echo $row["classname"].'</option>';
		
		ShowInfoType($type, $row['id'], $i);
	}
}


/*
 * 获取自定义类别
 *
 * @access public
 * @param  $tbname   string  显示分类的表名称
 * @param  $tbname2  string  使用分类的表名称
 * @param  $colname  string  使用分类的表字段
 * @param  $id       int     区别记录集的ID
 * @param  $i        int     option缩进位数
 * echo    string            输出<select>
*/
function ShowType($tbname='', $tbname2='', $colname='', $id=0, $i=0)
{
	global $dosql;

	if(isset($_GET['id'])) $r = $dosql->GetOne("SELECT $colname FROM `$tbname2` WHERE id=".$_GET['id']);
	$dosql->Execute("SELECT * FROM `$tbname` WHERE parentid=$id ORDER BY orderid ASC",$id);
	$i++;

	while($row = $dosql->GetArray($id))
	{
		if(isset($r))
		{
			if($row['id'] == $r["$colname"])
			{
				$selected = ' selected="selected"';
			}
			else
			{
				$selected = '';
			}
		}

		echo '<option value="'.$row['id'].'"'.@$selected.'>';

		for($p=1; $p<$i; $p++)
		{
			echo '&nbsp;&nbsp;&nbsp;&nbsp;';
		}
		if($row['parentid'] != 0)
		{
			echo '|- ';
		}

		echo $row["classname"].'</option>';
		ShowType($tbname, $tbname2, $colname, $row['id'], $i);
	}
}


/*
 * 展示自定义类别(无下级)
 *
 * @access  public
 * @param   $tbname   string  显示分类的表名称
 * @param   $tbname2  string  使用分类的表名称
 * @param   $colname  string  使用分类的表字段
 * @param   $id       int     区别记录集的ID
 * @param   $i        int     option缩进位数
 * @return  string            输出<select>
*/
function ShowNoneType($tbname='', $tbname2='', $colname='', $id=0, $i=0)
{
	global $dosql;

	if(isset($_GET['id'])) $r = $dosql->GetOne("SELECT $colname FROM `$tbname2` WHERE id=".$_GET['id']);
	$dosql->Execute("SELECT * FROM `$tbname` ORDER BY orderid ASC",$id);
	$i++;

	while($row = $dosql->GetArray($id))
	{
		if($row['id'] == $r["$colname"])
		{
			$selected = ' selected="selected"';
		}
		else
		{
			$selected = '';
		}
		echo '<option value="'.$row['id'].'"'.$selected.'>'.$row["classname"].'</option>';
	}
}


/*
 * 管理页类别展示
 *
 * @access  public
 * @param   $tbname  string  显示分类的表名称
 * @param   $id      int     区别记录集的ID
 * @param   $i       int     option缩进位数
 * @return  string           输出类别切换HTML
*/
function ShowMgrType($tbname='', $id=0, $i=0)
{
	global $dosql;

	$dosql->Execute("SELECT * FROM `$tbname` WHERE parentid=$id ORDER BY orderid ASC",$id);
	$i++;
	while($row = $dosql->GetArray($id))
	{
		echo '<a href="?tid='.$row['id'].'">';
		for($p=1; $p<$i; $p++)
		{
			echo '　';
		}
		if($row['parentid'] != 0)
		{
			echo '|- ';
		}
		echo $row["classname"].'</a>';

		ShowMgrType($tbname, $row['id'], $i);
	}
}


/*
 * 获取ajax信息类型
 *
 * @access  public
 * @param   $tbname   string  显示分类的表名称
 * @param   $type     int     要显示的模型ID
 * @param   $id       int     区别记录集的ID
 * @param   $i        int     option缩进位数
 * @reutnr  string            输出类别切换HTML
*/
function ShowMgrType2($tbname='', $type='', $id=0, $i=0)
{
	global $dosql;

	$dosql->Execute("SELECT * FROM `$tbname` WHERE parentid=$id ORDER BY orderid ASC", $id);
	$i++;
	while($row = $dosql->GetArray($id))
	{
		if(!empty($type))
		{
			if($row['infotype'] == $type)
			{
				echo '<a href="javascript:;" onclick="GetType(\''.$row['id'].'\',\''.$row["classname"].'\')">';
				for($p=1; $p<$i; $p++)
				{
					echo '　';
				}
				if($row['parentid'] != 0)
				{
					echo '|- ';
				}
				echo $row["classname"].'</a>';
			}
		}
		else
		{
			echo '<a href="javascript:;" onclick="GetType(\''.$row['id'].'\',\''.$row["classname"].'\')">';
			for($p=1; $p<$i; $p++)
			{
				echo '　';
			}
			if($row['parentid'] != 0)
			{
				echo '|- ';
			}
			echo $row["classname"].'</a>';
		}
		ShowMgrType2($tbname, $type, $row['id'], $i);
	}
}


/*
 * 展示自定义信息
 *
 * @access  public
 * @param   $tbname   string  显示分类的表名称
 * @param   $tbname2  string  使用分类的表名称
 * @param   $colname  string  使用分类的表字段
 * @return  string            输出<select>
*/
function ShowListInfo($tbname, $tbname2, $colname)
{
	global $dosql;

	if(isset($_GET['id'])) $r = $dosql->GetOne("SELECT $colname FROM `$tbname2` WHERE id=".$_GET['id']);
	$dosql->Execute("SELECT * FROM `$tbname` ORDER BY orderid DESC");

	while($row = $dosql->GetArray())
	{
		if($row['id'] == $r["$colname"])
		{
			$selected = ' selected="selected"';
		}
		else
		{
			$selected = '';
		}
		echo '<option value="'.$row['id'].'"'.$selected.'>'.$row["title"].'</option>';
	}
}


/*
 * 获取排列序号
 *
 * @access  public
 * @param   $tbname   string  获取该表的最大ID
 * @return  $orderid  int     返回当前ID
*/
function GetOrderID($tbname)
{
	global $dosql;

	$row = $dosql->GetOne("SELECT Max(orderid) AS orderid FROM `$tbname`");
	$orderid = (empty($row['orderid']) ? 1 : ($row['orderid'] + 1));
	return $orderid;
}


/*
 * 显示缩略图
 *
 * @access  public
 * @param   $picurl  string  <img src的值>
 * @return  $str     string  返回缩略图HTML
*/
function GetMgrThumbs($picurl='')
{
	$str = '<span class="thumbs"><img alt="';

	if($picurl != '')
	{
		if(substr($picurl, 0, 4) == 'http')
		{
			$str .= $picurl;
		}
		else if($picurl != '')
		{
			$str .= '../'.$picurl;
		}
	}
	else
	{
		$str .= 'templates/images/nofoundpic.gif';
	}

	$str .= '" /></span>';

	return $str;
}


/*
 * 获取指定ID与类型下所有子ID
 *
 * @access public
 * @param  $tbname  string  要查询的表名
 * @param  $id      int     要获取下级的ID
 * @param  $type    int     要显示的模型ID
 * @return          string  下级所有ID组合
*/
function GetChildID($tbname, $id='', $type='')
{
	global $dosql;

	if(empty($id) and $type=='')
	{
		$sql = "SELECT id FROM `$tbname` WHERE parentstr Like '0,%'";
	}
	if(!empty($id) and $type=='')
	{
		$sql = "SELECT id FROM `$tbname` WHERE parentstr Like '%,$id,%'";
	}
	if(empty($id) and $type!='')
	{
		$sql = "SELECT id FROM `$tbname` WHERE parentstr Like '0,%' AND infotype=$type";
	}
	if(!empty($id) and $type!='')
	{
		$sql = "SELECT id FROM `$tbname` WHERE parentstr Like '%,$id,%' AND infotype=$type";
	}

	$dosql->Execute($sql);
	$ids = '';

	while($row = $dosql->GetArray())
	{
		$ids .= $row['id'].',';
	}

	return $id.','.substr($ids,0,-1);
}


/*
 * 获取parentstr的第二位
 *
 * @access public
 * @param  $str    string  要拆分的整型序列如1,2,3
 * @param  $$i     int     为空返回str数组的第二位(第一位为0)
 * @return $topid  int     str的第一位
*/
function GetTopID($str, $i=1)
{
	if($str == '0,')
	{
		$topid = 0;
	}
	else
	{
		$ids = explode(',', $str);
		$topid = isset($ids[$i]) ? $ids[$i] : '';
	}
	
	return $topid;
}


//获得文章内容里的外部资源
function GetContFile($body)
{

	global $cfg_image_dir;

	//引入下载类
	require_once(ADMIN_INC.'/httpdown.class.php');

	//初始化变量
	$body = stripslashes($body);
	$host = 'http://'.$_SERVER['HTTP_HOST'];

	//过滤图片文件
    $pic_arr = array();
    preg_match_all("/src=[\"|'|\s]{0,}(http:\/\/([^>]*)\.(gif|jpg|png|bmp))/isU", $body, $pic_arr);
    $pic_arr = array_unique($pic_arr[1]);

	$htd = new HttpDown();

    foreach($pic_arr as $k=>$v)
    {

        if(preg_match('#'.$host.'#i', $v)) continue;
        if(!preg_match('#^http:\/\/#i', $v)) continue;

        $htd->OpenUrl($v);

        $type = $htd->GetHead("content-type");

        if($type == 'image/gif')
        {
            $tempfile_ext = 'gif';
        }
        else if($type == 'image/png')
        {
            $tempfile_ext = 'png';
        }
        else if($type == 'image/wbmp')
        {
            $tempfile_ext = 'bmp';
        }
        else
        {
            $tempfile_ext = 'jpg';
        }


		$upload_url = 'image';
		$upload_dir = $cfg_image_dir;

		$ymd = date('Ymd');
		$upload_url .= '/'.$ymd;
		$upload_dir .= '/'.$ymd;
	
		if(!file_exists($upload_dir))
		{
			mkdir($upload_dir);

			$fp = fopen($upload_dir.'/index.htm', 'w');
			fclose($fp);
		}
		
		//上传文件名称
		$filename = time()+rand(1,9999).'.'.$tempfile_ext;
	
	
		//上传文件路径
		$save_url = 'upload/'.$upload_url.'/'.$filename;

		$self = explode('/', $_SERVER['PHP_SELF']);
		$self_size = count($self) - 2;
		$self_str  = '';
		for($i=0; $i<$self_size; $i++)
		{
			$self_str .= $self[$i].'/';
		}

		$save_url = $self_str.'upload/'.$upload_url.'/'.$filename;
		$save_dir = $upload_dir.'/'.$filename;

        $rs = $htd->SaveToBin($save_dir);
        if($rs)
        {
            $body = str_replace(trim($v), $save_url, $body);
        }
    }

    $htd->Close();

	//回传转义字符串
    return _RunMagicQuotes($body);
}


/*
 * 获取一个远程图片
 *
 * @access  public
 * @param   $url       string  获取字段所属模型
 * @return  $save_url  string  返回上传后地址
*/
function GetRemPic($url)
{

	global $cfg_image_dir;

	//引入下载类
	require_once(ADMIN_INC.'/httpdown.class.php');

	//初始化变量
    $htd = new HttpDown();
    $htd->OpenUrl($url);
    $sparr = array('image/pjpeg', 'image/jpeg', 'image/gif', 'image/png', 'image/xpng', 'image/wbmp');

    if(!in_array($htd->GetHead("content-type"),$sparr))
    {
        return '';
    }
    else
    {
        $type = $htd->GetHead("content-type");

        if($type == 'image/gif')
        {
            $tempfile_ext = 'gif';
        }
        else if($type == 'image/png')
        {
            $tempfile_ext = 'png';
        }
        else if($type == 'image/wbmp')
        {
            $tempfile_ext = 'bmp';
        }
        else
        {
            $tempfile_ext = 'jpg';
        }

		$upload_url = 'image';
		$upload_dir = $cfg_image_dir;

		$ymd = date('Ymd');
		$upload_url .= '/'.$ymd;
		$upload_dir .= '/'.$ymd;
	
		if(!file_exists($upload_dir))
		{
			mkdir($upload_dir);

			$fp = fopen($upload_dir.'/index.htm', 'w');
			fclose($fp);
		}

		//上传文件名称
		$filename = time()+rand(1,9999).'.'.$tempfile_ext;

		//上传文件路径
		$save_url = 'uploads/'.$upload_url.'/'.$filename;
		$save_dir = $upload_dir.'/'.$filename;

        $rs = $htd->SaveToBin($save_dir);
    }

    $htd->Close();
    return ($rs ? $save_url : '');
}


/*
 * 文档自动分页
 *
 * @access  public
 * @param   $body    string  要设置分页内容
 * @param   $spsize  string  自动分页大小
 * @param   $sptag   string  分页标示符
 * @return  $body    string  设置分页符的内容
*/
function ContAutoPage($body, $spsize,
		 $sptag='<hr style="page-break-after:always;" class="ke-pagebreak" />')
{
    if(strlen($body) < $spsize)
    {
        return $body;
    }

    $body = stripslashes($body);
    $bds = explode('<', $body);
    $npageBody = '';
    $istable = 0;
    $body = '';

    foreach($bds as $i=>$k)
    {
        if($i==0)
        {
            $npageBody .= $bds[$i]; continue;
        }

        $bds[$i] = '<'.$bds[$i];

        if(strlen($bds[$i])>6)
        {
            $tname = substr($bds[$i],1,5);
            if(strtolower($tname) == 'table')
            {
                $istable++;
            }
            else if(strtolower($tname) == '/tabl')
            {
                $istable--;
            }
            if($istable > 0)
            {
                $npageBody .= $bds[$i]; continue;
            }
            else
            {
                $npageBody .= $bds[$i];
            }
        }
        else
        {
            $npageBody .= $bds[$i];
        }

        if(strlen($npageBody)>$spsize)
        {
            $body .= $npageBody.$sptag;
            $npageBody = '';
        }
    }

    if($npageBody!='')
    {
        $body .= $npageBody;
    }
    return _RunMagicQuotes($body);
}


/*
 * 获取自定义字段
 *
 * @access  public
 * @param   $type  string  要查询的字段模型ID
 * @param   $row   string  传递外部记录集的变量(编辑时用到)
 * @return         string  返回HTML
*/
function GetDiyField($type='',$row='')
{

	global $dosql, $cfg_max_file_size, $cfg_max_file_size;

	$dosql->Execute("SELECT * FROM `#@__diyfield` WHERE infotype='$type' AND checkinfo=true ORDER BY orderid ASC");
	while($r = $dosql->GetArray())
	{
		if(isset($row[$r['fieldname']]))
		{
			$fieldvalue = $row[$r['fieldname']];
		}
		else
		{
			$fieldvalue = '';
		}

		echo '<tr';
		if($r['fieldtype'] == 'mediumtext')
		{
			echo ' height="304"';
		}
		echo '><td height="35" align="right">'.$r['fieldtitle'].'：</td><td>';


		//文本框
		if($r['fieldtype']=='varchar' or $r['fieldtype']=='int' or $r['fieldtype']=='decimal')
		{
			echo '<input type="text" name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" class="class_input" value="'.$fieldvalue.'" />';
			if(!empty($r['fieldcheck']))
			{
				echo '&nbsp;<span class="maroon">*</span>';
			}
			echo '<span class="cnote">'.$r['fielddesc'].'</span>';
		}


		//多行文本
		else if($r['fieldtype'] == 'text')
		{
			echo '<textarea name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" class="class_areatext" style="margin:7px 0;">'.$fieldvalue.'</textarea>';
			if(!empty($r['fieldcheck']))
			{
				echo '&nbsp;<span class="maroon">*</span>';
			}
			echo '<span class="cnote">'.$r['fielddesc'].'</span>';
		}


		//单选按钮
		else if($r['fieldtype'] == 'radio')
		{
			if(!empty($r['fieldsel']))
			{
				$fieldsel = explode(',', $r['fieldsel']);
				foreach($fieldsel as $k=>$fieldsel_arr)
				{
					$fieldsel_val = explode('=', $fieldsel_arr);

					if($fieldvalue != '')
					{
						if($fieldsel_val[1] == $fieldvalue)
						{
							$checked = 'checked="checked"';
						}
						else
						{
							$checked = '';
						}
					}
					else
					{
						if($k == 0)
						{
							$checked = 'checked="checked"';
						}
						else
						{
							$checked = '';
						}
					}

					echo '<input type="radio" name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" value="'.$fieldsel_val[1].'" '.$checked.' />&nbsp;'.$fieldsel_val[0];
					if($k < (count($fieldsel)-1)) echo '&nbsp;&nbsp;&nbsp;';
				}
				if(!empty($r['fieldcheck']))
				{
					echo '&nbsp;<span class="maroon">*</span>';
				}
				echo '<span class="cnote">'.$r['fielddesc'].'</span>';
			}
			
		}


		//多选按钮
		else if($r['fieldtype'] == 'checkbox')
		{
			if(!empty($r['fieldsel']))
			{
				$fieldsel = explode(',', $r['fieldsel']);
				foreach($fieldsel as $k=>$fieldsel_arr)
				{
					$fieldsel_val = explode('=', $fieldsel_arr);

					if($fieldvalue != '')
					{
						$fileall = explode(',',$fieldvalue);
						if(is_array($fileall))
						{
							if(in_array($fieldsel_val[1], $fileall))
							{
								$checked = 'checked="checked"';
							}
							else
							{
								$checked = '';
							}
						}
						else
						{
							if($fieldsel_val[1] == $fieldvalue)
							{
								$checked = 'checked="checked"';
							}
							else
							{
								$checked = '';
							}
						}
					}
					else
					{
						$checked = '';
					}

					echo '<input type="checkbox" name="'.$r['fieldname'].'[]" id="'.$r['fieldname'].'[]" value="'.$fieldsel_val[1].'" '.$checked.' />&nbsp;'.$fieldsel_val[0];
					if($k < (count($fieldsel)-1)) echo '&nbsp;&nbsp;&nbsp;';
				}
				if(!empty($r['fieldcheck']))
				{
					echo '&nbsp;<span class="maroon">*</span>';
				}
				echo '<span class="cnote">'.$r['fielddesc'].'</span>';
			}

		}


		//下拉菜单
		else if($r['fieldtype'] == 'select')
		{
			if(!empty($r['fieldsel']))
			{

				echo '<select name="'.$r['fieldname'].'" id="'.$r['fieldname'].'">';
				$fieldsel = explode(',', $r['fieldsel']);
				foreach($fieldsel as $k=>$fieldsel_arr)
				{
					$fieldsel_val = explode('=', $fieldsel_arr);

					if($fieldvalue != '')
					{
						if($fieldsel_val[1] == $fieldvalue)
						{
							$selected = 'selected="selected"';
						}
						else
						{
							$selected = '';
						}
					}
					else
					{
						$selected = '';
					}

					$fieldsel_val = explode('=', $fieldsel_arr);
					echo '<option name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" value="'.$fieldsel_val[1].'"'.$selected.' />'.$fieldsel_val[0].'</option>';
					if($k < (count($fieldsel)-1)) echo '&nbsp;&nbsp;&nbsp;';
				}
				echo '</select>';
				if(!empty($r['fieldcheck']))
				{
					echo '&nbsp;<span class="maroon">*</span>';
				}
				echo '<span class="cnote">'.$r['fielddesc'].'</span>';
			}
		}


		//单个附件
		else if($r['fieldtype'] == 'file')
		{
			echo '<input type="text" name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" class="class_input" value="'.$fieldvalue.'" />';
			echo '<span class="cnote"><span class="gray_btn" onclick="GetUploadify(\'uploadify\',\''.$r['fieldtitle'].'\',\'all\',\'all\',1,'.$cfg_max_file_size.',\''.$r['fieldname'].'\')">上 传</span></span>';
			if(!empty($r['fieldcheck']))
			{
				echo '&nbsp;<span class="maroon">*</span>';
			}
			echo '<span class="cnote">'.$r['fielddesc'].'</span>';
		}


		//多个附件
		else if($r['fieldtype'] == 'fileall')
		{
			echo '<fieldset class="picarr"><legend>列表</legend><div>最多可以上传<strong>50</strong>个附件<span onclick="GetUploadify(\'uploadify2\',\''.$r['fieldtitle'].'\',\'all\',\'all\',50,'.$cfg_max_file_size.',\''.$r['fieldname'].'\',\''.$r['fieldname'].'_area\')">开始上传</span></div><ul id="'.$r['fieldname'].'_area">';
			if(isset($fieldvalue))
			{
				$picarr = explode(',',$fieldvalue);
				if(!empty($picarr[0]))
				{
					foreach($picarr as $v)
					{
						echo '<li rel="'.$v.'"><input type="text" name="'.$r['fieldname'].'[]" value="'.$v.'"><a href="javascript:void(0);" onclick="ClearPicArr(\''.$v.'\')">删除</a></li>';
					}
				}
			}
			echo '</ul></fieldset>';
		}


		//日期时间
		else if($r['fieldtype'] == 'datetime')
		{
			if(!empty($fieldvalue))
			{
				$dtime = GetDateTime($fieldvalue);
			}
			else
			{
				$dtime = GetDateTime(time());
			}
			echo '<style>#'.$r['fieldname'].'{background:url(templates/images/calendar.gif) 127px no-repeat;cursor:pointer;}</style>';
			echo '<input type="text" name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" class="input_short" value="'.$dtime .'" readonly="readonly" />';
			if(!empty($r['fieldcheck']))
			{
				echo '&nbsp;<span class="maroon">*</span>';
			}
			echo '<span class="cnote">'.$r['fielddesc'].'</span>';
			echo '<script type="text/javascript">Calendar.setup({inputField:"'.$r['fieldname'].'",ifFormat:"%Y-%m-%d %H:%M:%S",showsTime:true,timeFormat:"24"});</script>';
		}


		//编辑器模式
		else if($r['fieldtype'] == 'mediumtext')
		{
			echo '<textarea name="'.$r['fieldname'].'" id="'.$r['fieldname'].'" class="kindeditor">'.$fieldvalue.'</textarea>';
			echo '<script type="text/javascript">var editor_'.$r['fieldname'].';KindEditor.ready(function(K) {editor_'.$r['fieldname'].' = K.create(\'textarea[name="'.$r['fieldname'].'"]\', {allowFileManager : true,width:\'667px\',height:\'280px\'});});</script>';
		}
	}

	echo '</td></tr>';
}
?>