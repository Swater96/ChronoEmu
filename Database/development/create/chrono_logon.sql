-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               5.7.18-log - MySQL Community Server (GPL)
-- Server OS:                    Win64
-- HeidiSQL Version:             9.4.0.5125
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping structure for table chrono_logon.accounts
CREATE TABLE IF NOT EXISTS `accounts` (
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

-- Data exporting was unselected.
-- Dumping structure for table chrono_logon.account_data
CREATE TABLE IF NOT EXISTS `account_data` (
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

-- Data exporting was unselected.
-- Dumping structure for table chrono_logon.ipbans
CREATE TABLE IF NOT EXISTS `ipbans` (
  `ip` varchar(16) NOT NULL DEFAULT '' COMMENT 'Remote host',
  `expire` int(30) NOT NULL DEFAULT '0' COMMENT 'Expiry time (s)',
  PRIMARY KEY (`ip`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- Data exporting was unselected.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
