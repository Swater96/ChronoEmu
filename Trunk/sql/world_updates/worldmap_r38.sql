ALTER TABLE `worldmap_info`
DROP COLUMN `heroic_keyid_1`,
DROP COLUMN `heroic_keyid_2`,
MODIFY COLUMN `viewingDistance`  float NOT NULL DEFAULT 80 AFTER `required_item`;