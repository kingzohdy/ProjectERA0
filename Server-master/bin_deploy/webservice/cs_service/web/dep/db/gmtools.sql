/*
MySQL Data Transfer
Source Host: 192.168.0.77
Source Database: gmtools
Target Host: 192.168.0.77
Target Database: gmtools
Date: 2012-12-14 12:28:41
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for adminor
-- ----------------------------
DROP TABLE IF EXISTS `adminor`;
CREATE TABLE `adminor` (
  `id` int(11) NOT NULL auto_increment,
  `username` varchar(20) default NULL,
  `passwrod` varchar(32) default NULL,
  `prem` int(11) default '1',
  UNIQUE KEY `id` (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for gmlog
-- ----------------------------
DROP TABLE IF EXISTS `gmlog`;
CREATE TABLE `gmlog` (
  `LogId` int(11) NOT NULL auto_increment COMMENT 'æ—¥å¿—ID',
  `UserId` int(11) NOT NULL COMMENT '用户ID',
  `GameUserID` int(11) default NULL COMMENT 'Ã¨Â¢Â«Ã¦â€œÂÃ¤Â½Å“Ã§Å¡â€žÃ¨Â´Â¦Ã¦Ë†Â·ID',
  `GameUserRole` varchar(32) default NULL COMMENT '被操作的账户角色名',
  `RegionID` int(11) default NULL COMMENT 'è¢«æ“ä½œçš„è§’è‰²æ‰€åœ¨å¤§åŒº',
  `WorldID` int(11) default NULL COMMENT 'è¢«æ“ä½œçš„è§’è‰²æ‰€åœ¨æœ',
  `time` date default NULL COMMENT 'ÃƒÂ¦Ã¢â‚¬Å“Ã‚ÂÃƒÂ¤Ã‚Â½Ã…â€œÃƒÂ¦Ã¢â‚¬â€Ã‚Â¶ÃƒÂ©Ã¢â‚¬â€Ã‚Â´',
  `StartTime` date default NULL COMMENT 'ç”Ÿæ•ˆæ—¶é—´',
  `ContinueTime` int(11) default NULL COMMENT 'æŒç»­æ—¶é—´',
  `OperateType` varchar(20) default NULL COMMENT 'æ“ä½œç±»åž‹ï¼š1.å…¬å‘Šæ·»åŠ ï¼Œ2.å…¬å‘Šåˆ é™¤',
  `OperateState` varchar(20) default NULL COMMENT 'Ã¦â€œÂÃ¤Â½Å“Ã§Å Â¶Ã¦â‚¬Â,1:Ã¨Â¡Â¨Ã§Â¤ÂºÃ¥Â¤Â±Ã¨Â´Â¥ 0Ã¨Â¡Â¨Ã§Â¤ÂºÃ¦Ë†ÂÃ¥Å Å¸',
  `log` text COMMENT '操作记录(哪种操作)',
  PRIMARY KEY  (`LogId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Table structure for reg_world
-- ----------------------------
DROP TABLE IF EXISTS `reg_world`;
CREATE TABLE `reg_world` (
  `dbid` varchar(10) NOT NULL COMMENT '数据表名',
  `worldid` int(11) NOT NULL COMMENT '服务器ID',
  `worldname` varchar(64) NOT NULL default '' COMMENT '服务器名',
  `regid` int(11) NOT NULL COMMENT '大区ID',
  `regname` varchar(64) NOT NULL COMMENT '大区',
  `dbip` varchar(32) NOT NULL COMMENT '服务器IP',
  `dbport` int(11) NOT NULL default '3306' COMMENT '数据库端口',
  `dbuser` varchar(20) default NULL COMMENT '服务器账号',
  `dbpass` varchar(32) default NULL COMMENT '服务器密码',
  PRIMARY KEY  (`dbid`),
  UNIQUE KEY `id` (`dbid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records 
-- ----------------------------
INSERT INTO `adminor` VALUES ('1', 'wanyou_admin', 'e5315a1dc146120f3e35211f0915291c', '1');
INSERT INTO `adminor` VALUES ('4', 'admin1', '353f4789ad69a9d9d39ee145e264fc15', '1');
INSERT INTO `adminor` VALUES ('6', 'ljy', 'c16a640dd7b9973fbdc275dddac65628', '2');
INSERT INTO `adminor` VALUES ('7', 'wanyou', 'c16a640dd7b9973fbdc275dddac65628', '3');
INSERT INTO `adminor` VALUES ('9', 'ljy1', 'c16a640dd7b9973fbdc275dddac65628', '4');
INSERT INTO `gmlog` VALUES ('1', '9', '0', '', '0', '0', '2012-12-14', '0000-00-00', '200', null, null, '????9,??????2012-12-14 11:13:11,??0??,<br>??????2?,?????:200?,<br>???<pre>??????12kkkkk</pre><br>????');
INSERT INTO `gmlog` VALUES ('2', '9', '0', '', '3', '10', '2012-12-14', '0000-00-00', '500', null, null, '??????????,??:3 ,??10<br>????9,??????2012-12-14 11:30:22,??0??,<br>??????6?,?????:500?,<br>???<pre>?????oooooo??12313</pre><br>????');
INSERT INTO `reg_world` VALUES ('1_1', '1', '1服', '1', '1大区', '192.168.0.57', '3306', 'mmog', 'mmog');
INSERT INTO `reg_world` VALUES ('3_10', '10', '10服', '3', '3大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('3_12', '12', '12服', '3', '3大区', '192.168.0.78', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('3_5', '5', '5服', '3', '3大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('3_6', '6', '6服', '3', '3大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('3_7', '7', '7服', '3', '3大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('3_9', '9', '9服', '3', '3大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
INSERT INTO `reg_world` VALUES ('4_10', '4', '10服', '4', '4大区', '192.168.0.77', '3306', 'server', 'tulin#1234');
