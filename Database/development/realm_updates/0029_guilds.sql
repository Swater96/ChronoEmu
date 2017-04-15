DROP TABLE guild_logs;
DROP TABLE guild_data;
ALTER TABLE characters ADD `publicNote` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' AFTER `forced_rename_pending`;
ALTER TABLE characters ADD `officerNote` varchar(32) COLLATE utf8_unicode_ci NOT NULL DEFAULT '' AFTER `publicNote`;
ALTER TABLE characters ADD `guildid` int(10) unsigned NOT NULL DEFAULT '0' AFTER `officerNote`;
ALTER TABLE characters ADD `guildRank` int(10) unsigned NOT NULL DEFAULT '0' AFTER `guildid`;

TRUNCATE TABLE guilds;
ALTER TABLE `guilds` MODIFY COLUMN  `createdate` datetime NOT NULL DEFAULT '0000-00-00 00:00:00'

