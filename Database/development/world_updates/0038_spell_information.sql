SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `spell_information`
-- ----------------------------
DROP TABLE IF EXISTS `spellfixes`;
DROP TABLE IF EXISTS `spell_information`;
CREATE TABLE `spell_information` (
  `Entry` int(30) NOT NULL DEFAULT '0',
  `SchoolType` int(30) NOT NULL DEFAULT '0',
  `Category` int(30) NOT NULL DEFAULT '0',
  `DispelType` int(30) NOT NULL DEFAULT '0',
  `MechanicsType` int(30) NOT NULL DEFAULT '0',
  `Attributes` int(30) NOT NULL DEFAULT '0',
  `AttributesEx` int(30) NOT NULL DEFAULT '0',
  `TriggerSpell` int(30) NOT NULL DEFAULT '0',
  `ProcFlags` int(30) NOT NULL DEFAULT '0',
  `ProcChance` int(30) NOT NULL DEFAULT '0',
  `ProcsPerMinute` int(30) NOT NULL DEFAULT '0',
  `ProcCharges` int(30) NOT NULL DEFAULT '0',
  `Cooldown` int(30) NOT NULL DEFAULT '0',
  `MaxStack` int(30) NOT NULL DEFAULT '0',
  `FamilyName` int(30) NOT NULL DEFAULT '0',
  `GroupType` int(30) NOT NULL DEFAULT '0',
  `MaxTargets` int(30) NOT NULL DEFAULT '0',
  `CustomFlag` int(30) NOT NULL DEFAULT '0',
  `ProcInterval` int(30) NOT NULL DEFAULT '0',
  `BuffType` int(30) NOT NULL DEFAULT '0',
  `NameHash` int(30) NOT NULL DEFAULT '0',
  `CanCrit` BOOL NOT NULL DEFAULT '0',
  `IsRanged` BOOL NOT NULL DEFAULT '0',
  `IsMelee` BOOL NOT NULL DEFAULT '0',									
  PRIMARY KEY (`Entry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

