/*
SQLyog Enterprise - MySQL GUI v8.05 RC 
MySQL - 5.1.43-community : Database - chrono2logon
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

/*Table structure for table `account_data` */

DROP TABLE IF EXISTS `account_data`;

CREATE TABLE `account_data` (
  `acct` int(30) NOT NULL,
  `uiconfig0` longtext,
  `uiconfig1` longtext,
  `uiconfig2` longtext,
  `uiconfig3` longtext,
  `uiconfig4` longtext,
  `uiconfig5` longtext,
  `uiconfig6` longtext,
  `uiconfig7` longtext,
  `uiconfig8` longtext,
  PRIMARY KEY (`acct`),
  UNIQUE KEY `a` (`acct`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `account_data` */

/*Table structure for table `accounts` */

DROP TABLE IF EXISTS `accounts`;

CREATE TABLE `accounts` (
  `acct` int(10) unsigned NOT NULL COMMENT 'Unique ID',
  `login` varchar(32) NOT NULL COMMENT 'Login username',
  `password` varchar(32) NOT NULL COMMENT 'Login password',
  `encrypted_password` varchar(42) NOT NULL DEFAULT '',
  `gm` varchar(32) NOT NULL DEFAULT '' COMMENT 'Game permissions',
  `banned` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Account Standing',
  `lastlogin` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00' COMMENT 'Last login timestamp',
  `lastip` varchar(16) NOT NULL DEFAULT '' COMMENT 'Last remote address',
  `email` varchar(64) NOT NULL DEFAULT '' COMMENT 'Contact e-mail address',
  `flags` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT 'Client flags',
  `forceLanguage` varchar(5) NOT NULL DEFAULT 'enUS',
  `muted` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`acct`),
  UNIQUE KEY `login` (`login`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `accounts` */

/*Table structure for table `ipbans` */

DROP TABLE IF EXISTS `ipbans`;

CREATE TABLE `ipbans` (
  `ip` varchar(16) NOT NULL DEFAULT '' COMMENT 'Remote host',
  `expire` int(30) NOT NULL DEFAULT '0' COMMENT 'Expiry time (s)',
  PRIMARY KEY (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `ipbans` */

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
