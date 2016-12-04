ALTER TABLE `creature_proto`
ADD COLUMN `immune_mask`  int(11) UNSIGNED NOT NULL DEFAULT 0 AFTER `extra_a9_flags`,
ADD COLUMN `school_immune`  tinyint(3) UNSIGNED NOT NULL DEFAULT 0 AFTER `immune_mask`;
