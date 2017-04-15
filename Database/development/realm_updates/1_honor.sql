ALTER TABLE `characters`
DROP COLUMN `honorRolloverTime`,
DROP COLUMN `killsToday`,
DROP COLUMN `killsYesterday`,
DROP COLUMN `killsLifeTime`,
DROP COLUMN `honorToday`,
DROP COLUMN `honorYesterday`,
DROP COLUMN `honorPoints`,
ADD COLUMN `highest_rank`  int(10) NOT NULL AUTO_INCREMENT AFTER `finished_quests`,
ADD COLUMN `standing_pos`  int(10) NOT NULL AUTO_INCREMENT AFTER `highest_rank`,
ADD COLUMN `stored_honor`  int(10) NOT NULL AUTO_INCREMENT AFTER `standing_pos`,
ADD COLUMN `stored_dishonorablekills`  int(10) NOT NULL AUTO_INCREMENT AFTER `stored_honor`,
ADD COLUMN `stored_honorablekills`  int(10) NOT NULL AUTO_INCREMENT AFTER `stored_dishonorablekills`;

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `character_honor_cp`
-- ----------------------------
DROP TABLE IF EXISTS `character_honor_cp`;
CREATE TABLE `character_honor_cp` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier',
  `victim_type` tinyint(3) unsigned NOT NULL DEFAULT '4',
  `victim` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Creature / Player Identifier',
  `honor` float NOT NULL DEFAULT '0',
  `date` int(11) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  KEY `idx_guid` (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='Player System';

