-- MySQL dump 10.11
--
-- Host: localhost    Database: dirty
-- ------------------------------------------------------
-- Server version	5.0.77

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `t_business`
--

DROP TABLE IF EXISTS `t_business`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_business` (
  `busid` int(11) NOT NULL auto_increment,
  `name` varchar(32) NOT NULL default '',
  `busname` varchar(40) default NULL,
  PRIMARY KEY  (`busid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `t_business`
--

LOCK TABLES `t_business` WRITE;
/*!40000 ALTER TABLE `t_business` DISABLE KEYS */;
set names 'utf8'; INSERT INTO `t_business` VALUES (1,'bowl','大碗菜'),(2,'xxz','仙侠传'),(3,'sanguo','QQ三国'),(4,'xunxian','寻仙'),(5,'feiche','QQ飞车'),(6,'OTOT','OTOT'),(7,'pet','QQ宠物');
/*!40000 ALTER TABLE `t_business` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_dirty_OTOT`
--

DROP TABLE IF EXISTS `t_dirty_OTOT`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_OTOT` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_TestProject`
--

DROP TABLE IF EXISTS `t_dirty_TestProject`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_TestProject` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM AUTO_INCREMENT=2294 DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_bowl`
--

DROP TABLE IF EXISTS `t_dirty_bowl`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_bowl` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_feiche`
--

DROP TABLE IF EXISTS `t_dirty_feiche`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_feiche` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_pet`
--

DROP TABLE IF EXISTS `t_dirty_pet`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_pet` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_sanguo`
--

DROP TABLE IF EXISTS `t_dirty_sanguo`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_sanguo` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_xunxian`
--

DROP TABLE IF EXISTS `t_dirty_xunxian`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_xunxian` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_dirty_xxz`
--

DROP TABLE IF EXISTS `t_dirty_xxz`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_dirty_xxz` (
  `id` int(11) NOT NULL auto_increment,
  `level` int(11) default '1',
  `word` varchar(255) NOT NULL default '',
  `type` char(1) NOT NULL default '1',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `key1` (`word`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `t_log`
--

DROP TABLE IF EXISTS `t_log`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_log` (
  `id` int(11) NOT NULL auto_increment,
  `userid` int(11) NOT NULL default '0',
  `recordtime` datetime NOT NULL default '0000-00-00 00:00:00',
  `busid` int(11) NOT NULL default '0',
  `type` int(11) NOT NULL default '0',
  `word` varchar(255) default '0',
  `execvalue` int(11) NOT NULL default '0',
  `remark` varchar(255) default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `t_log`
--

--
-- Table structure for table `t_role`
--

DROP TABLE IF EXISTS `t_role`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_role` (
  `roleid` int(11) NOT NULL auto_increment,
  `rolename` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`roleid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `t_role`
--

LOCK TABLES `t_role` WRITE;
/*!40000 ALTER TABLE `t_role` DISABLE KEYS */;
set names 'utf8'; INSERT INTO `t_role` VALUES (1,'Admin'),(2,'User');
/*!40000 ALTER TABLE `t_role` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_type`
--

DROP TABLE IF EXISTS `t_type`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_type` (
  `type` varchar(100) NOT NULL default '',
  `name` varchar(100) default '',
  PRIMARY KEY  (`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `t_type`
--

LOCK TABLES `t_type` WRITE;
/*!40000 ALTER TABLE `t_type` DISABLE KEYS */;
set names 'utf8'; INSERT INTO `t_type` VALUES ('1','Add'),('2','Delete'),('3','Edit'),('4','Import'),('5','Clear');
/*!40000 ALTER TABLE `t_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_user`
--

DROP TABLE IF EXISTS `t_user`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
CREATE TABLE `t_user` (
  `userid` int(11) NOT NULL default '0',
  `username` varchar(100) NOT NULL default '',
  `roleid` int(11) NOT NULL default '0',
  `busid` int(11) NOT NULL default '0',
  PRIMARY KEY  (`userid`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
SET character_set_client = @saved_cs_client;

--
-- Dumping data for table `t_user`
--

LOCK TABLES `t_user` WRITE;
/*!40000 ALTER TABLE `t_user` DISABLE KEYS */;
set names 'utf8'; INSERT INTO `t_user` VALUES (4733,'kailu(卢凯)',1,0),(506,'sean(吴佳伟)',1,0),(3551,'gacktzhang(张赟)',1,0),(8376,'lulujiang(蒋璐)',1,0),(536,'steve(聂志明)',1,0),(8048,'norkchang(常攀峰)',2,2),(4826,'cilightyang(杨若虚)',2,2),(10403,'lenagao(高丽娜)',2,2),(4820,'yanbinchen(陈延彬)',2,1),(10363,'shawnluo(罗翔宇)',2,1),(6169,'dijinzhou(周大军)',2,3),(8190,'remindliu(刘明昊)',2,2),(3039,'terrywang(王兆财)',1,0),(934,'kentwei(魏文强)',1,0);
/*!40000 ALTER TABLE `t_user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2009-06-29  7:43:55
