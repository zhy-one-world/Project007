/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50628
Source Host           : localhost:3306
Source Database       : low_game_schema

Target Server Type    : MYSQL
Target Server Version : 50628
File Encoding         : 65001

Date: 2024-07-09 09:35:53
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `account`
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` int(32) NOT NULL AUTO_INCREMENT,
  `account` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `password` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `login_type` int(11) NOT NULL DEFAULT '0',
  `login_type_plus` int(10) unsigned NOT NULL DEFAULT '0',
  `restricted` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `register_type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `create_time` datetime NOT NULL DEFAULT '2010-08-31 00:00:00',
  `is_freeze` int(32) NOT NULL DEFAULT '0',
  `platform_account` varchar(25) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `is_white_account` int(32) NOT NULL,
  `create_ip` varchar(17) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `device_type` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `online_duration` int(11) DEFAULT NULL,
  `online_time` int(11) DEFAULT NULL,
  `month_recharge_num` int(11) DEFAULT NULL,
  `month_recharge_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `accountnameindex` (`account`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=87 DEFAULT CHARSET=gbk ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of account
-- ----------------------------

-- ----------------------------
-- Table structure for `attack_city_info`
-- ----------------------------
DROP TABLE IF EXISTS `attack_city_info`;
CREATE TABLE `attack_city_info` (
  `group_level` int(11) NOT NULL DEFAULT '0',
  `legion_guid` bigint(20) NOT NULL DEFAULT '0',
  `server_id` int(11) DEFAULT '0',
  `legion_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `winning_streak_num` int(11) DEFAULT '0',
  `is_check` int(11) DEFAULT '0',
  PRIMARY KEY (`legion_guid`,`group_level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of attack_city_info
-- ----------------------------

-- ----------------------------
-- Table structure for `attack_city_rank_info`
-- ----------------------------
DROP TABLE IF EXISTS `attack_city_rank_info`;
CREATE TABLE `attack_city_rank_info` (
  `group_level` int(11) NOT NULL DEFAULT '0',
  `group_rank` int(11) NOT NULL DEFAULT '0',
  `legion_guid` bigint(20) DEFAULT '0',
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `rank_index` int(11) DEFAULT '0',
  `server_id` int(11) DEFAULT '0',
  `class_type` int(11) DEFAULT '0',
  `role_source` int(11) DEFAULT '0',
  `kill_player_num` int(11) DEFAULT '0',
  `kill_statue_num` int(11) DEFAULT '0',
  `kill_gate_num` int(11) DEFAULT '0',
  `repair_statue_num` int(11) DEFAULT '0',
  `kill_boss_num` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`group_rank`,`group_level`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of attack_city_rank_info
-- ----------------------------

-- ----------------------------
-- Table structure for `auction`
-- ----------------------------
DROP TABLE IF EXISTS `auction`;
CREATE TABLE `auction` (
  `seller_guid` bigint(20) DEFAULT '0',
  `seller_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `buyer_guid` bigint(20) DEFAULT '0',
  `buyer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `trade_type` int(11) DEFAULT '0',
  `seller_vip_level` int(11) DEFAULT '0',
  `auction_tab` int(11) NOT NULL DEFAULT '0',
  `auction_sub_tab` int(11) NOT NULL DEFAULT '0',
  `item_color` int(11) DEFAULT '0',
  `quality_level` int(11) DEFAULT '0',
  `class_limit` int(11) DEFAULT '0',
  `item_name` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `sell_time` bigint(20) DEFAULT '0',
  `deal_time` bigint(20) DEFAULT '0',
  `sell_money_type` int(11) DEFAULT '0',
  `sell_total_money` bigint(20) DEFAULT '0',
  `sell_single_money` bigint(20) DEFAULT '0',
  `cur_bid_money` bigint(20) DEFAULT '0',
  `is_show` int(11) DEFAULT '1',
  `add_time` int(11) DEFAULT NULL,
  `is_sky_order` int(11) DEFAULT NULL,
  `show_guid` bigint(20) DEFAULT '0',
  `item_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT '0',
  `e_item_info_illusion_had_byte` int(11) DEFAULT '0',
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`item_guid`,`item_name`,`auction_tab`,`auction_sub_tab`),
  KEY `item_guid` (`item_guid`),
  KEY `seller_guid` (`seller_guid`),
  KEY `buyer_guid` (`buyer_guid`),
  KEY `item_color` (`item_color`),
  KEY `class_limit` (`class_limit`),
  KEY `quality_level` (`quality_level`),
  KEY `e_item_info_info_id` (`e_item_info_info_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auction
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_bid_notice`
-- ----------------------------
DROP TABLE IF EXISTS `auction_bid_notice`;
CREATE TABLE `auction_bid_notice` (
  `role_guid` bigint(20) DEFAULT NULL,
  `item_guid` bigint(20) DEFAULT NULL,
  `create_time` int(11) DEFAULT NULL,
  `template_id` int(11) DEFAULT NULL,
  `template_num` int(11) DEFAULT NULL,
  `money_type` int(11) DEFAULT NULL,
  `bid_money` bigint(11) DEFAULT NULL,
  `is_sell` int(11) DEFAULT '0',
  `buy_guid` bigint(20) DEFAULT NULL,
  `update_time` int(11) DEFAULT NULL,
  `is_read` int(11) DEFAULT NULL,
  KEY `item_guid` (`item_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of auction_bid_notice
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_bid_record`
-- ----------------------------
DROP TABLE IF EXISTS `auction_bid_record`;
CREATE TABLE `auction_bid_record` (
  `role_guid` bigint(20) NOT NULL,
  `item_guid` bigint(20) NOT NULL DEFAULT '0',
  `deal_time` bigint(20) NOT NULL DEFAULT '0',
  `is_read` int(11) DEFAULT '0',
  PRIMARY KEY (`item_guid`,`role_guid`,`deal_time`),
  KEY `item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auction_bid_record
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_bid_replace`
-- ----------------------------
DROP TABLE IF EXISTS `auction_bid_replace`;
CREATE TABLE `auction_bid_replace` (
  `record_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_guid` bigint(20) DEFAULT '0',
  `create_time` int(11) DEFAULT NULL,
  `is_read` int(11) DEFAULT NULL,
  `seller_guid` bigint(20) DEFAULT '0',
  `seller_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `buyer_guid` bigint(20) DEFAULT '0',
  `buyer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `trade_type` int(11) DEFAULT '0',
  `seller_vip_level` int(11) DEFAULT '0',
  `auction_tab` int(11) DEFAULT '0',
  `auction_sub_tab` int(11) DEFAULT '0',
  `item_color` int(11) DEFAULT '0',
  `quality_level` int(11) DEFAULT '0',
  `class_limit` int(11) DEFAULT '0',
  `item_name` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `sell_time` bigint(20) DEFAULT '0',
  `deal_time` bigint(20) DEFAULT '0',
  `sell_money_type` int(11) DEFAULT '0',
  `sell_total_money` bigint(20) DEFAULT '0',
  `sell_single_money` bigint(20) DEFAULT '0',
  `cur_bid_money` bigint(20) DEFAULT '0',
  `is_show` int(11) DEFAULT '0',
  `add_time` int(11) DEFAULT NULL,
  `item_guid` bigint(20) DEFAULT '0',
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT '0',
  `e_item_info_illusion_had_byte` int(11) DEFAULT '0',
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`record_guid`),
  KEY `record_guid` (`record_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of auction_bid_replace
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_record`
-- ----------------------------
DROP TABLE IF EXISTS `auction_record`;
CREATE TABLE `auction_record` (
  `record_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_guid` bigint(20) DEFAULT '0',
  `is_sell` tinyint(1) DEFAULT NULL,
  `seller_guid` bigint(20) DEFAULT '0',
  `seller_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `buyer_guid` bigint(20) DEFAULT '0',
  `buyer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `trade_type` int(11) DEFAULT '0',
  `seller_vip_level` int(11) DEFAULT '0',
  `auction_tab` int(11) DEFAULT '0',
  `auction_sub_tab` int(11) DEFAULT '0',
  `item_color` int(11) DEFAULT '0',
  `quality_level` int(11) DEFAULT '0',
  `class_limit` int(11) DEFAULT '0',
  `item_name` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `sell_time` bigint(20) DEFAULT '0',
  `deal_time` bigint(20) DEFAULT '0',
  `sell_money_type` int(11) DEFAULT '0',
  `sell_total_money` bigint(20) DEFAULT '0',
  `sell_single_money` bigint(20) DEFAULT '0',
  `cur_bid_money` bigint(20) DEFAULT '0',
  `is_show` int(11) DEFAULT '0',
  `add_time` int(11) DEFAULT '0',
  `is_sky_order` int(11) DEFAULT NULL,
  `show_guid` bigint(20) DEFAULT NULL,
  `item_guid` bigint(20) DEFAULT '0',
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT '0',
  `e_item_info_illusion_had_byte` int(11) DEFAULT '0',
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`record_guid`),
  KEY `record_guid` (`record_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auction_record
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_share_list`
-- ----------------------------
DROP TABLE IF EXISTS `auction_share_list`;
CREATE TABLE `auction_share_list` (
  `item_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `share_rate` float DEFAULT '0',
  `share_mail_title` varchar(65) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `share_mail_content` varchar(401) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`item_guid`,`role_guid`),
  KEY `item_guid` (`item_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auction_share_list
-- ----------------------------

-- ----------------------------
-- Table structure for `best_record_info`
-- ----------------------------
DROP TABLE IF EXISTS `best_record_info`;
CREATE TABLE `best_record_info` (
  `map_template_id` int(11) NOT NULL DEFAULT '0',
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `best_record_role_name` varchar(511) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `best_record_value` int(11) DEFAULT '0',
  PRIMARY KEY (`map_template_id`,`role_guid`),
  KEY `map_template_id` (`map_template_id`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of best_record_info
-- ----------------------------

-- ----------------------------
-- Table structure for `big_player`
-- ----------------------------
DROP TABLE IF EXISTS `big_player`;
CREATE TABLE `big_player` (
  `big_player_type` int(11) NOT NULL DEFAULT '0',
  `big_player_guid` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`big_player_type`),
  KEY `big_player_type` (`big_player_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of big_player
-- ----------------------------

-- ----------------------------
-- Table structure for `cloud_shop_role_record`
-- ----------------------------
DROP TABLE IF EXISTS `cloud_shop_role_record`;
CREATE TABLE `cloud_shop_role_record` (
  `player_uid` bigint(20) unsigned NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `activity_type` int(11) NOT NULL DEFAULT '0',
  `class_type` int(11) NOT NULL DEFAULT '0',
  `server_id` int(11) NOT NULL DEFAULT '0',
  `buy_number` int(11) unsigned NOT NULL DEFAULT '0',
  `robber_number` int(11) unsigned NOT NULL DEFAULT '0',
  `basic_award_count1` int(11) unsigned NOT NULL DEFAULT '0',
  `basic_award_count2` int(11) unsigned NOT NULL DEFAULT '0',
  `basic_award_count3` int(11) unsigned NOT NULL DEFAULT '0',
  `basic_award_count4` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`player_uid`,`activity_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of cloud_shop_role_record
-- ----------------------------

-- ----------------------------
-- Table structure for `croos_world_boss`
-- ----------------------------
DROP TABLE IF EXISTS `croos_world_boss`;
CREATE TABLE `croos_world_boss` (
  `boss_template_id` int(11) NOT NULL,
  `boss_is_dead` int(11) NOT NULL,
  `kill_boss_time` int(11) DEFAULT NULL,
  `kill_boss_player_server_id` int(11) DEFAULT NULL,
  `kill_boss_player_guid` int(11) DEFAULT NULL,
  `kill_boss_player_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `kill_boss_legion_guid` int(11) DEFAULT NULL,
  `kill_boss_legion_name` varchar(11) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`boss_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of croos_world_boss
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_ladder_ranking`
-- ----------------------------
DROP TABLE IF EXISTS `cross_ladder_ranking`;
CREATE TABLE `cross_ladder_ranking` (
  `ranking_id` int(11) NOT NULL AUTO_INCREMENT,
  `role_guid` bigint(20) DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `template_id` int(11) DEFAULT '0',
  `role_gs` int(11) DEFAULT '0',
  `ladder_score` int(11) DEFAULT '0',
  PRIMARY KEY (`ranking_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_ladder_ranking
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_ladder_role_info`
-- ----------------------------
DROP TABLE IF EXISTS `cross_ladder_role_info`;
CREATE TABLE `cross_ladder_role_info` (
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `server_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `gs_value` bigint(20) DEFAULT NULL,
  `template_id` int(11) DEFAULT NULL,
  `role_gs` int(11) DEFAULT NULL,
  `vip_level` int(11) DEFAULT NULL,
  `server_id` int(11) DEFAULT NULL,
  `ladder_score` int(11) DEFAULT NULL,
  `total_join_num` int(11) DEFAULT NULL,
  `total_win_num` int(11) DEFAULT NULL,
  `last_win_num` int(11) DEFAULT NULL,
  `ticket_num` int(11) DEFAULT NULL,
  `can_buy_num` int(11) DEFAULT NULL,
  `history_score` int(11) DEFAULT '0',
  `last_score` int(11) DEFAULT '0',
  `season_reward` int(11) DEFAULT NULL,
  `cur_season_join_num` int(11) DEFAULT NULL,
  `last_season_join_num` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_ladder_role_info
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_role_info`
-- ----------------------------
DROP TABLE IF EXISTS `cross_role_info`;
CREATE TABLE `cross_role_info` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `template_id` int(11) DEFAULT '0',
  `role_gs` int(11) DEFAULT '0',
  `war_num` int(11) DEFAULT '0',
  `win_num` int(11) DEFAULT '0',
  `last_win` int(11) DEFAULT '0',
  `join_num` int(11) DEFAULT '0',
  `ladder_score` int(11) DEFAULT '0',
  `refresh_time` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_role_info
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_role_war`
-- ----------------------------
DROP TABLE IF EXISTS `cross_role_war`;
CREATE TABLE `cross_role_war` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `target_name_1` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `target_name_2` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `target_name_3` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `war_result` int(11) DEFAULT '0',
  `cross_honor` int(11) DEFAULT '0',
  `ladder_score` int(11) DEFAULT '0',
  `war_time` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_role_war
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_server_harry_info`
-- ----------------------------
DROP TABLE IF EXISTS `cross_server_harry_info`;
CREATE TABLE `cross_server_harry_info` (
  `server_id` int(11) NOT NULL DEFAULT '0',
  `server_money` int(11) DEFAULT NULL,
  `special_money` int(11) DEFAULT NULL,
  `first_player_guid` bigint(20) DEFAULT NULL,
  `second_player_guid` bigint(20) DEFAULT NULL,
  `third_player_guid` bigint(20) DEFAULT NULL,
  `first_player_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of cross_server_harry_info
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_server_overload_city_info`
-- ----------------------------
DROP TABLE IF EXISTS `cross_server_overload_city_info`;
CREATE TABLE `cross_server_overload_city_info` (
  `legion_guid` bigint(20) DEFAULT NULL,
  `server_id` int(11) DEFAULT NULL,
  `chief_guid` bigint(20) DEFAULT NULL,
  `legion_rank` int(11) NOT NULL,
  `legion_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `chief_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`legion_rank`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_server_overload_city_info
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_server_pk_info`
-- ----------------------------
DROP TABLE IF EXISTS `cross_server_pk_info`;
CREATE TABLE `cross_server_pk_info` (
  `war_idex` int(11) NOT NULL,
  `winner_player_guid` bigint(20) DEFAULT '0',
  `island_pk_state` int(11) DEFAULT '0',
  `winner_player_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `winner_player_win_num` int(11) DEFAULT '0',
  `winner_player_server_id` int(11) DEFAULT '0',
  `one_player_is_winner` int(11) DEFAULT NULL,
  `lose_player_guid` bigint(20) DEFAULT '0',
  `lose_player_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `lose_player_win_num` int(11) DEFAULT '0',
  `lose_player_server_id` int(11) DEFAULT '0',
  `other_player_is_winner` int(11) DEFAULT NULL,
  PRIMARY KEY (`war_idex`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_server_pk_info
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_world_boss`
-- ----------------------------
DROP TABLE IF EXISTS `cross_world_boss`;
CREATE TABLE `cross_world_boss` (
  `boss_template_id` int(11) NOT NULL,
  `boss_is_dead` int(11) NOT NULL,
  `kill_boss_time` int(11) DEFAULT NULL,
  `kill_boss_player_server_id` int(11) DEFAULT NULL,
  `kill_boss_player_guid` bigint(20) DEFAULT NULL,
  `kill_boss_player_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `kill_boss_legion_guid` bigint(20) DEFAULT NULL,
  `kill_boss_legion_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`boss_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of cross_world_boss
-- ----------------------------

-- ----------------------------
-- Table structure for `databases_name`
-- ----------------------------
DROP TABLE IF EXISTS `databases_name`;
CREATE TABLE `databases_name` (
  `database_name` varchar(31) NOT NULL DEFAULT '0',
  `database_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`database_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of databases_name
-- ----------------------------

-- ----------------------------
-- Table structure for `element_war_info`
-- ----------------------------
DROP TABLE IF EXISTS `element_war_info`;
CREATE TABLE `element_war_info` (
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(100) NOT NULL DEFAULT '',
  `server_name` varchar(100) NOT NULL DEFAULT '',
  `gs_value` bigint(20) DEFAULT NULL,
  `template_id` int(11) DEFAULT NULL,
  `vip_level` int(11) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `server_id` int(11) DEFAULT NULL,
  `score_num` int(11) DEFAULT NULL,
  `cur_season` int(11) DEFAULT '0',
  `last_season` int(11) DEFAULT NULL,
  `total_join_num` int(11) DEFAULT NULL,
  `total_win_num` int(11) DEFAULT NULL,
  `last_win_num` int(11) DEFAULT NULL,
  `ticket_num` int(11) DEFAULT NULL,
  `can_buy_num` int(11) DEFAULT NULL,
  `have_buy_num` int(11) DEFAULT NULL,
  `history_score` int(11) DEFAULT NULL,
  `cur_season_join_num` int(11) DEFAULT NULL,
  `last_score` int(11) DEFAULT NULL,
  `season_reward` int(11) DEFAULT NULL,
  `last_season_join_num` int(11) DEFAULT NULL,
  `mission1` int(11) DEFAULT NULL,
  `mission2` int(11) DEFAULT NULL,
  `mission3` int(11) DEFAULT NULL,
  `mission4` int(11) DEFAULT NULL,
  `mission1_have_num` int(11) DEFAULT NULL,
  `mission2_have_num` int(11) DEFAULT NULL,
  `mission3_have_num` int(11) DEFAULT NULL,
  `mission4_have_num` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of element_war_info
-- ----------------------------

-- ----------------------------
-- Table structure for `gain_treasure_record`
-- ----------------------------
DROP TABLE IF EXISTS `gain_treasure_record`;
CREATE TABLE `gain_treasure_record` (
  `boss_spawn_point_template_id` int(11) NOT NULL,
  `killed_stamp` int(11) NOT NULL,
  `first_role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `first_gain_item_id` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `first_role_guid` bigint(20) NOT NULL DEFAULT '0',
  `second_role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `second_gain_item_id` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `second_role_guid` bigint(20) DEFAULT NULL,
  `third_role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `third_gain_item_id` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `third_role_guid` bigint(20) DEFAULT NULL,
  `fourth_role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `fourth_gain_item_id` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `fourth_role_guid` bigint(20) DEFAULT NULL,
  `fifth_role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `fifth_gain_item_id` varchar(51) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `fifth_role_guid` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`boss_spawn_point_template_id`,`killed_stamp`,`first_role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of gain_treasure_record
-- ----------------------------

-- ----------------------------
-- Table structure for `gm_role_lock`
-- ----------------------------
DROP TABLE IF EXISTS `gm_role_lock`;
CREATE TABLE `gm_role_lock` (
  `role_mark` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `is_login` int(11) DEFAULT '0',
  `is_send_chat` int(11) DEFAULT '0',
  `jewel_num` int(11) DEFAULT '0',
  `is_gm` int(11) DEFAULT '0',
  `is_radio_host` int(11) DEFAULT '0',
  PRIMARY KEY (`role_mark`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of gm_role_lock
-- ----------------------------

-- ----------------------------
-- Table structure for `last_server_list`
-- ----------------------------
DROP TABLE IF EXISTS `last_server_list`;
CREATE TABLE `last_server_list` (
  `server_id` int(11) NOT NULL DEFAULT '0',
  `last_server_list` varchar(256) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of last_server_list
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_applicant`
-- ----------------------------
DROP TABLE IF EXISTS `legion_applicant`;
CREATE TABLE `legion_applicant` (
  `legion_guid` bigint(20) NOT NULL,
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `exist_time` float DEFAULT '0',
  `gs_value` bigint(20) DEFAULT NULL,
  `ELegionApplicantInfo_level` int(11) DEFAULT '0',
  `ELegionApplicantInfo_player_template_id` int(11) DEFAULT '0',
  `ELegionApplicantInfo_fighting_power` int(11) DEFAULT '0',
  `ELegionApplicantInfo_vip_level` int(11) DEFAULT '0',
  PRIMARY KEY (`legion_guid`,`role_guid`),
  KEY `legion_guid` (`legion_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_applicant
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_bonus_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_bonus_info`;
CREATE TABLE `legion_bonus_info` (
  `legion_guid` bigint(20) NOT NULL,
  `mission_id` int(11) NOT NULL,
  `finish_count` int(11) DEFAULT NULL,
  PRIMARY KEY (`legion_guid`,`mission_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_bonus_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_boss_record`
-- ----------------------------
DROP TABLE IF EXISTS `legion_boss_record`;
CREATE TABLE `legion_boss_record` (
  `legion_guid` bigint(20) NOT NULL,
  `boss_map_guid` bigint(20) DEFAULT '0',
  `killer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `ELegionBossRecord_template_id` int(11) NOT NULL,
  `ELegionBossRecord_unlock` int(11) DEFAULT '0',
  `ELegionBossRecord_killed` int(11) DEFAULT '0',
  PRIMARY KEY (`legion_guid`,`ELegionBossRecord_template_id`),
  KEY `legion_guid` (`legion_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_boss_record
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_city_bid_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_city_bid_info`;
CREATE TABLE `legion_city_bid_info` (
  `territory_id` int(11) NOT NULL DEFAULT '0',
  `index_in_terr` int(11) NOT NULL DEFAULT '0',
  `legion_guid` bigint(20) DEFAULT '0',
  `cur_bid_price` int(11) DEFAULT '0',
  `is_prior_maintain` int(11) DEFAULT '0',
  `occupy_time_stamp` int(11) DEFAULT '0',
  `server_id` int(11) NOT NULL DEFAULT '0',
  `legion_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`territory_id`,`index_in_terr`),
  KEY `territory_id` (`territory_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of legion_city_bid_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_city_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_city_info`;
CREATE TABLE `legion_city_info` (
  `occupation_legion_guid` bigint(20) NOT NULL DEFAULT '0',
  `city_master_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  PRIMARY KEY (`occupation_legion_guid`),
  KEY `occupation_legion_guid` (`occupation_legion_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of legion_city_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_city_war_member_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_city_war_member_info`;
CREATE TABLE `legion_city_war_member_info` (
  `legion_guid` bigint(20) DEFAULT '0',
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `use_type` bigint(20) NOT NULL DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `player_temp_id` bigint(20) DEFAULT '0',
  `week_contribution` bigint(20) DEFAULT '0',
  `job_title` bigint(20) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`use_type`),
  KEY `legion_guid` (`legion_guid`) USING BTREE,
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_city_war_member_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_event`
-- ----------------------------
DROP TABLE IF EXISTS `legion_event`;
CREATE TABLE `legion_event` (
  `legion_guid` bigint(20) NOT NULL,
  `event_idx` int(11) NOT NULL,
  `extra_data` varchar(257) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `happen_time` int(11) DEFAULT '0',
  PRIMARY KEY (`legion_guid`,`event_idx`),
  KEY `legion_guid` (`legion_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of legion_event
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_get_boss_award_log`
-- ----------------------------
DROP TABLE IF EXISTS `legion_get_boss_award_log`;
CREATE TABLE `legion_get_boss_award_log` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `boss_map_id` int(11) NOT NULL,
  PRIMARY KEY (`role_guid`,`boss_map_id`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_get_boss_award_log
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_info`;
CREATE TABLE `legion_info` (
  `legion_guid` bigint(20) NOT NULL DEFAULT '0',
  `legion_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `announcement` varchar(401) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `gs_value` bigint(20) DEFAULT NULL,
  `ELegionInfo_construction_level_main` bigint(20) DEFAULT '1',
  `ELegionInfo_construction_level_1` bigint(20) DEFAULT '1',
  `ELegionInfo_construction_level_2` bigint(20) DEFAULT '1',
  `ELegionInfo_construction_level_3` bigint(20) DEFAULT '1',
  `ELegionInfo_asset_money` bigint(20) DEFAULT '0',
  `ELegionInfo_asset_item_1` bigint(20) DEFAULT '0',
  `ELegionInfo_asset_item_2` bigint(20) DEFAULT '0',
  `ELegionInfo_asset_item_3` bigint(20) DEFAULT '0',
  `ELegionInfo_asset_item_4` bigint(20) DEFAULT '0',
  `ELegionInfo_fighting_power` bigint(20) DEFAULT '0',
  `ELegionInfo_auto_accept_new_member` bigint(20) DEFAULT '0',
  `ELegionInfo_last_refresh_time` bigint(20) DEFAULT '0',
  `ELegionInfo_glory_glow` bigint(20) DEFAULT '0',
  `ELegionInfo_act_score_raid` bigint(20) DEFAULT '0',
  `ELegionInfo_act_score_bonfire` bigint(20) DEFAULT '0',
  `ELegionInfo_legion_welfare_num` bigint(20) DEFAULT '0',
  PRIMARY KEY (`legion_guid`),
  KEY `legion_guid` (`legion_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_member_info`
-- ----------------------------
DROP TABLE IF EXISTS `legion_member_info`;
CREATE TABLE `legion_member_info` (
  `legion_guid` bigint(20) DEFAULT '0',
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `gs_value` bigint(20) DEFAULT '0',
  `e_legion_member_info_is_online` bigint(20) DEFAULT '0',
  `e_legion_member_info_level` bigint(20) DEFAULT '0',
  `e_legion_member_info_player_template_id` bigint(20) DEFAULT '0',
  `e_legion_member_info_fighting_power` bigint(20) DEFAULT '0',
  `e_legion_member_info_job_title` bigint(20) DEFAULT '0',
  `e_legion_member_info_city_war_score` bigint(20) DEFAULT '0',
  `e_legion_member_info_donate_contribution` bigint(20) DEFAULT '0',
  `e_legion_member_info_add_fuel_times` bigint(20) DEFAULT '0',
  `e_legion_member_info_last_logout_stamp` bigint(20) DEFAULT '0',
  `e_legion_member_info_last_get_city_award` bigint(20) DEFAULT NULL,
  `e_legion_member_info_in_voice_channel` bigint(20) DEFAULT '0',
  `e_legion_member_info_join_time_stamp` bigint(20) DEFAULT NULL,
  `e_legion_member_info_vip_level` bigint(20) DEFAULT NULL,
  `e_legion_member_info_get_master_city_award` bigint(20) DEFAULT NULL,
  `e_legion_member_info_week_contribution` bigint(20) DEFAULT NULL,
  `e_legion_member_info_is_show_vip` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `legion_guid` (`legion_guid`) USING BTREE,
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_member_info
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_warehouse`
-- ----------------------------
DROP TABLE IF EXISTS `legion_warehouse`;
CREATE TABLE `legion_warehouse` (
  `legion_guid` bigint(20) NOT NULL DEFAULT '0',
  `add_time` bigint(20) NOT NULL DEFAULT '0',
  `item_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT NULL,
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT '0',
  `e_item_info_illusion_had_byte` int(11) DEFAULT '0',
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT NULL,
  `e_item_info_forge_property2` int(11) DEFAULT NULL,
  `e_item_info_forge_property3` int(11) DEFAULT NULL,
  `e_item_info_forge_property4` int(11) DEFAULT NULL,
  `e_item_info_forge_property5` int(11) DEFAULT NULL,
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT ''
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_warehouse
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_warehouse_log`
-- ----------------------------
DROP TABLE IF EXISTS `legion_warehouse_log`;
CREATE TABLE `legion_warehouse_log` (
  `role_guid` bigint(20) NOT NULL,
  `item_guid` bigint(20) NOT NULL,
  `legion_guid` bigint(20) NOT NULL,
  `add_time` bigint(20) NOT NULL,
  `job` int(11) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `operation_type` int(11) NOT NULL,
  `e_item_info_info_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`item_guid`,`legion_guid`,`operation_type`,`add_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_warehouse_log
-- ----------------------------

-- ----------------------------
-- Table structure for `mail_globel_mail`
-- ----------------------------
DROP TABLE IF EXISTS `mail_globel_mail`;
CREATE TABLE `mail_globel_mail` (
  `empty_guid` bigint(20) DEFAULT NULL,
  `mail_guid` bigint(20) NOT NULL,
  `EMailInfo_IsNeedDelete` int(11) DEFAULT '0',
  `EMailInfo_SendTime_num` int(11) DEFAULT '0',
  `EMailInfo_ItemGuid1` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid2` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid3` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid4` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid5` bigint(11) DEFAULT '0',
  `EMailInfo_MoneyTyp1` int(11) DEFAULT '0',
  `EMailInfo_MoneyNum1` int(11) DEFAULT '0',
  `EMailInfo_MoneyTyp2` int(11) DEFAULT '0',
  `EMailInfo_MoneyNum2` int(11) DEFAULT '0',
  `EMailInfo_ReadState` int(11) DEFAULT '0',
  `EMailInfo_SenderGuid` bigint(11) DEFAULT '0',
  `EMailInfo_ServerId` int(11) DEFAULT NULL,
  `text_contents` varchar(401) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `sender_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `mail_title` varchar(65) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`mail_guid`),
  KEY `mail_guid` (`mail_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mail_globel_mail
-- ----------------------------

-- ----------------------------
-- Table structure for `mail_globel_mail_item`
-- ----------------------------
DROP TABLE IF EXISTS `mail_globel_mail_item`;
CREATE TABLE `mail_globel_mail_item` (
  `empty_guid` bigint(20) DEFAULT NULL,
  `item_guid` bigint(20) NOT NULL,
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT NULL,
  `e_item_info_illusion_had_byte` int(11) DEFAULT NULL,
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`item_guid`),
  KEY `item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of mail_globel_mail_item
-- ----------------------------

-- ----------------------------
-- Table structure for `oracle_trial_player_info`
-- ----------------------------
DROP TABLE IF EXISTS `oracle_trial_player_info`;
CREATE TABLE `oracle_trial_player_info` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `tier_num` int(11) NOT NULL DEFAULT '0',
  `customs_state` int(11) DEFAULT '0',
  `reward_state` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`tier_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of oracle_trial_player_info
-- ----------------------------

-- ----------------------------
-- Table structure for `other_server_mail_record`
-- ----------------------------
DROP TABLE IF EXISTS `other_server_mail_record`;
CREATE TABLE `other_server_mail_record` (
  `source_server_id` int(11) NOT NULL,
  `target_server_id` int(11) NOT NULL DEFAULT '0',
  `mail_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_guid` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`source_server_id`,`target_server_id`,`mail_guid`),
  KEY `source_server_id` (`source_server_id`) USING BTREE,
  KEY `target_server_id` (`target_server_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of other_server_mail_record
-- ----------------------------

-- ----------------------------
-- Table structure for `ranking_arena`
-- ----------------------------
DROP TABLE IF EXISTS `ranking_arena`;
CREATE TABLE `ranking_arena` (
  `position` int(11) NOT NULL,
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `gs_value` bigint(20) DEFAULT '0',
  `role_template_id` int(11) DEFAULT '0',
  `robot_template_id` int(11) DEFAULT '0',
  `military_rank_level` int(11) DEFAULT '0',
  `vip_level` int(11) DEFAULT '0',
  `game_channel` varchar(129) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`position`,`role_guid`),
  KEY `role_guid` (`role_guid`),
  KEY `position` (`position`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of ranking_arena
-- ----------------------------

-- ----------------------------
-- Table structure for `ranking_arena_challenge_log`
-- ----------------------------
DROP TABLE IF EXISTS `ranking_arena_challenge_log`;
CREATE TABLE `ranking_arena_challenge_log` (
  `log_id` int(11) NOT NULL AUTO_INCREMENT,
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `role_rank` int(11) NOT NULL,
  `target_guid` bigint(20) NOT NULL,
  `target_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `target_rank` int(11) DEFAULT '0',
  `initiative` int(11) NOT NULL DEFAULT '1',
  `result` int(11) NOT NULL DEFAULT '1',
  `date` int(11) NOT NULL,
  PRIMARY KEY (`log_id`),
  KEY `log_id` (`log_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of ranking_arena_challenge_log
-- ----------------------------

-- ----------------------------
-- Table structure for `ranking_role_worship`
-- ----------------------------
DROP TABLE IF EXISTS `ranking_role_worship`;
CREATE TABLE `ranking_role_worship` (
  `role_guid` bigint(20) NOT NULL,
  `worship` bigint(11) DEFAULT NULL,
  `worship_time` bigint(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of ranking_role_worship
-- ----------------------------

-- ----------------------------
-- Table structure for `recharge_record`
-- ----------------------------
DROP TABLE IF EXISTS `recharge_record`;
CREATE TABLE `recharge_record` (
  `order_id` varchar(40) NOT NULL,
  `role_guid` bigint(20) NOT NULL,
  `game_goods_id` int(11) NOT NULL,
  `is_sucess` int(11) NOT NULL,
  `begin_time` bigint(20) NOT NULL,
  `end_time` bigint(20) NOT NULL,
  `payment_type` int(11) NOT NULL DEFAULT '0',
  `direct_diamond` int(11) DEFAULT '0',
  `order_token` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`order_id`,`payment_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of recharge_record
-- ----------------------------

-- ----------------------------
-- Table structure for `red_package_info`
-- ----------------------------
DROP TABLE IF EXISTS `red_package_info`;
CREATE TABLE `red_package_info` (
  `red_package_guid` bigint(20) NOT NULL DEFAULT '0',
  `red_package_template_id` int(11) DEFAULT '0',
  `red_package_sender_guid` bigint(20) DEFAULT '0',
  `red_package_create_time` int(11) DEFAULT '0',
  `red_package_sender_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `red_package_send_text` varchar(61) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `red_package_sender_vip_level` int(11) DEFAULT '0',
  `red_package_remain_times` int(11) DEFAULT '0',
  `red_package_remain_money_num` int(11) DEFAULT '0',
  PRIMARY KEY (`red_package_guid`),
  KEY `red_package_guid` (`red_package_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of red_package_info
-- ----------------------------

-- ----------------------------
-- Table structure for `red_package_receiver_info`
-- ----------------------------
DROP TABLE IF EXISTS `red_package_receiver_info`;
CREATE TABLE `red_package_receiver_info` (
  `red_package_guid` bigint(20) NOT NULL DEFAULT '0',
  `receiver_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `money_num` int(11) DEFAULT '0',
  PRIMARY KEY (`red_package_guid`,`receiver_guid`),
  KEY `red_package_guid` (`red_package_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of red_package_receiver_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_achievement`
-- ----------------------------
DROP TABLE IF EXISTS `role_achievement`;
CREATE TABLE `role_achievement` (
  `role_guid` bigint(20) NOT NULL,
  `eaid_id` int(11) NOT NULL,
  `eaid_current_num` int(11) DEFAULT '0',
  `eaid_current_state` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`eaid_id`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_achievement
-- ----------------------------

-- ----------------------------
-- Table structure for `role_active_degree`
-- ----------------------------
DROP TABLE IF EXISTS `role_active_degree`;
CREATE TABLE `role_active_degree` (
  `role_guid` bigint(20) NOT NULL,
  `e_daily_active_degree_type_cost_one_diamond` int(11) DEFAULT '0',
  `e_daily_active_degree_type_ten_daily_mission` int(11) DEFAULT '0',
  `e_daily_active_degree_type_three_normal_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_three_elite_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_three_hell_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_empire_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_temple_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_warp_raid` int(11) DEFAULT '0',
  `e_daily_active_degree_type_three_upgrade` int(11) DEFAULT '0',
  `e_daily_active_degree_type_three_addon` int(11) DEFAULT '0',
  `e_daily_active_degree_type_kill_200_monster` int(11) DEFAULT '0',
  `e_daily_active_degree_type_kill_500_monster` int(11) DEFAULT '0',
  `e_daily_active_degree_type_kill_1000_monster` int(11) DEFAULT '0',
  `e_daily_active_degree_type_login` int(11) DEFAULT '0',
  `e_daily_active_degree_type_online_30_minutes` int(11) DEFAULT '0',
  `e_daily_active_degree_type_strengthing_one_spirit` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_strengthing_one_mount` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_strengthing_one_wing` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_upgrade_one_element_heart` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_upgrade_one_patron_saint` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_upgrade_one_belief` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_submit_one_illustration` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_attend_three_arena` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_attend_one_city_war` int(11) DEFAULT NULL,
  `e_daily_active_degree_type_attend_one_legion_war` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_active_degree
-- ----------------------------

-- ----------------------------
-- Table structure for `role_apppearance_info`
-- ----------------------------
DROP TABLE IF EXISTS `role_apppearance_info`;
CREATE TABLE `role_apppearance_info` (
  `appearance_id` int(11) NOT NULL AUTO_INCREMENT,
  `appearance_info` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`appearance_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_apppearance_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_att`
-- ----------------------------
DROP TABLE IF EXISTS `role_att`;
CREATE TABLE `role_att` (
  `role_guid` bigint(20) NOT NULL,
  `e_base_att_info_none` bigint(20) DEFAULT '0',
  `e_base_att_info_strength` bigint(20) DEFAULT '0',
  `e_base_att_info_agility` bigint(20) DEFAULT '0',
  `e_base_att_info_intellect` bigint(20) DEFAULT '0',
  `e_base_att_info_stamina` bigint(20) DEFAULT '0',
  `e_base_att_info_hp_cur` bigint(20) DEFAULT '0',
  `e_base_att_info_pk_value` bigint(20) DEFAULT '0',
  `e_base_att_info_pk_mode` bigint(20) DEFAULT '0',
  `e_base_att_info_community_type` bigint(20) DEFAULT '0',
  `e_base_att_info_equip_title_id` bigint(20) DEFAULT '0',
  `e_base_att_info_magic_cur` bigint(20) DEFAULT '0',
  `e_base_att_info_body_power_cur` bigint(20) DEFAULT '0',
  `e_base_att_info_body_strength` bigint(20) DEFAULT '0',
  `e_base_att_info_body_agility` bigint(20) DEFAULT '0',
  `e_base_att_info_body_intellect` bigint(20) DEFAULT '0',
  `e_base_att_info_body_stamina` bigint(20) DEFAULT '0',
  `e_base_att_info_hp_main` bigint(20) DEFAULT NULL,
  `e_base_att_info_cur_energy` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_att
-- ----------------------------

-- ----------------------------
-- Table structure for `role_belief`
-- ----------------------------
DROP TABLE IF EXISTS `role_belief`;
CREATE TABLE `role_belief` (
  `role_guid` bigint(20) NOT NULL,
  `e_belief_info_template_id` int(11) NOT NULL,
  `e_belief_info_upgrade_count` int(11) DEFAULT '0',
  `e_belief_info_try_upgrade_times` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_belief_info_template_id`),
  KEY `role_guid` (`role_guid`),
  KEY `e_belief_info_template_id` (`e_belief_info_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_belief
-- ----------------------------

-- ----------------------------
-- Table structure for `role_boss_island`
-- ----------------------------
DROP TABLE IF EXISTS `role_boss_island`;
CREATE TABLE `role_boss_island` (
  `role_guid` bigint(20) NOT NULL,
  `kill_boss_count` int(11) DEFAULT NULL,
  `collect_count` int(11) DEFAULT NULL,
  `cross_pk_collect_count` int(11) DEFAULT NULL,
  `oracle_trial_count` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_boss_island
-- ----------------------------

-- ----------------------------
-- Table structure for `role_buff`
-- ----------------------------
DROP TABLE IF EXISTS `role_buff`;
CREATE TABLE `role_buff` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `buff_guid` bigint(20) NOT NULL DEFAULT '0',
  `sender_role_guid` bigint(20) DEFAULT '0',
  `EBuffInfo_template_id` int(11) DEFAULT '0',
  `EBuffInfo_load_type` int(11) DEFAULT '0',
  `EBuffInfo_time` int(11) DEFAULT '0',
  `EBuffInfo_pile_count` int(11) DEFAULT '0',
  `EBuffInfo_series` int(11) DEFAULT '0',
  `EBuffInfo_skill_template_id` int(11) DEFAULT '0',
  `EBuffInfo_percent` int(11) DEFAULT '0',
  `EBuffInfo_effect_index` int(11) DEFAULT '-1',
  PRIMARY KEY (`role_guid`,`buff_guid`),
  KEY `role_guid` (`role_guid`),
  KEY `buff_guid` (`buff_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_buff
-- ----------------------------

-- ----------------------------
-- Table structure for `role_chat_record`
-- ----------------------------
DROP TABLE IF EXISTS `role_chat_record`;
CREATE TABLE `role_chat_record` (
  `role_guid` bigint(20) NOT NULL,
  `pos` int(11) NOT NULL DEFAULT '0',
  `record` varchar(256) DEFAULT '',
  PRIMARY KEY (`role_guid`,`pos`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_chat_record
-- ----------------------------

-- ----------------------------
-- Table structure for `role_competition`
-- ----------------------------
DROP TABLE IF EXISTS `role_competition`;
CREATE TABLE `role_competition` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `have_type` int(11) NOT NULL DEFAULT '0',
  `is_buy` int(11) NOT NULL DEFAULT '0',
  `buy_manual_type` int(11) NOT NULL DEFAULT '0',
  `point_add_percent` int(11) NOT NULL DEFAULT '0',
  `season` int(11) NOT NULL DEFAULT '0',
  `competition_begin_time` int(11) NOT NULL DEFAULT '0',
  `competition_end_time` int(11) NOT NULL DEFAULT '0',
  `level` int(11) NOT NULL DEFAULT '0',
  `cur_exp` bigint(20) NOT NULL DEFAULT '0',
  `need_reset` int(11) NOT NULL DEFAULT '0',
  `assist_fight_guid` bigint(20) NOT NULL DEFAULT '0',
  `reward1` int(11) NOT NULL DEFAULT '0',
  `reward2` int(11) NOT NULL DEFAULT '0',
  `reward3` int(11) NOT NULL DEFAULT '0',
  `reward4` int(11) NOT NULL DEFAULT '0',
  `reward5` int(11) NOT NULL DEFAULT '0',
  `reward6` int(11) NOT NULL DEFAULT '0',
  `reward7` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_guid`,`have_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_competition
-- ----------------------------

-- ----------------------------
-- Table structure for `role_convert`
-- ----------------------------
DROP TABLE IF EXISTS `role_convert`;
CREATE TABLE `role_convert` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `currency_index` int(11) NOT NULL DEFAULT '0',
  `convert_times` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`currency_index`),
  KEY `role_guid` (`role_guid`),
  KEY `currency_index` (`currency_index`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of role_convert
-- ----------------------------

-- ----------------------------
-- Table structure for `role_daily_must_do_count`
-- ----------------------------
DROP TABLE IF EXISTS `role_daily_must_do_count`;
CREATE TABLE `role_daily_must_do_count` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_daily_must_do_typ_raid_exp` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_daily` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_devil_square` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_ancient_battlefield` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_siliver` int(20) DEFAULT '0',
  `e_daily_must_do_typ_community_fight` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_drame` int(20) DEFAULT '0',
  `e_daily_must_do_typ_bloody_castle` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_1` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_2` int(20) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_3` int(20) DEFAULT '0',
  `e_daily_must_do_typ_demon_tower` int(20) DEFAULT '0',
  `e_daily_must_do_typ_angel_temple` int(20) DEFAULT '0',
  `e_daily_must_do_typ_pk_king` int(20) DEFAULT '0',
  `e_daily_must_do_typ_arena` int(20) DEFAULT '0',
  `e_daily_must_do_typ_crusade_mission` int(20) DEFAULT '0',
  `e_daily_must_do_typ_crystal_illusion` int(20) DEFAULT '0',
  `e_daily_must_do_typ_mingmie_xukong` int(20) DEFAULT '0',
  `e_daily_must_do_typ_out_board` int(20) DEFAULT '0',
  `e_daily_must_do_typ_convert` int(20) DEFAULT '0',
  `e_daily_must_do_typ_rank` int(20) DEFAULT '0',
  `e_daily_must_do_typ_gold_army` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_world_boss` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_sky_treasure_box` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_field_treasure_box` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_boss` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_meditation` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_activaty_degree` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_city_war` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_cross_fight` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_gold_store` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_magic_crystal_store` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_ancient_coin_store` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_hope_store` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_store` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_hope_item` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_shop` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_bonfire` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_war` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_boss_home` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_boss_island` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_belief_cloister` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_cross_pk` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_cross_rob` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_dance` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_legion_answer` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_cross_ladder` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_single_boss` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_element_war` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_vip_boss_home` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_cross_server_boss` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_oracle_trial` int(20) DEFAULT NULL,
  `e_daily_must_do_typ_attack_city` int(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_daily_must_do_count
-- ----------------------------

-- ----------------------------
-- Table structure for `role_demons_tower`
-- ----------------------------
DROP TABLE IF EXISTS `role_demons_tower`;
CREATE TABLE `role_demons_tower` (
  `role_guid` bigint(20) NOT NULL,
  `e_demons_tower_info_cur_challenge_tier` int(11) DEFAULT '0',
  `e_demons_tower_info_left_mopping_up_count` int(11) DEFAULT '0',
  `e_demons_tower_info_cur_mopping_up_tier` int(11) DEFAULT '0',
  `e_demons_tower_info_can_get_award` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_demons_tower
-- ----------------------------

-- ----------------------------
-- Table structure for `role_dragontrip`
-- ----------------------------
DROP TABLE IF EXISTS `role_dragontrip`;
CREATE TABLE `role_dragontrip` (
  `role_guid` bigint(20) NOT NULL,
  `e_dragontrip_free_time` int(11) DEFAULT NULL,
  `e_dragontrip_round` int(11) DEFAULT '0',
  `e_dragontrip_pos` int(11) DEFAULT '0',
  `e_dragontrip_rewardflag` int(11) DEFAULT '0',
  `e_dragontrip_oper_time` int(11) DEFAULT '0',
  `e_dragontrip_active_state` int(11) DEFAULT NULL,
  `e_dragontrip_day_times` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_dragontrip
-- ----------------------------

-- ----------------------------
-- Table structure for `role_element_heart`
-- ----------------------------
DROP TABLE IF EXISTS `role_element_heart`;
CREATE TABLE `role_element_heart` (
  `role_guid` bigint(20) NOT NULL,
  `item_guid` bigint(20) NOT NULL,
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`item_guid`),
  KEY `role_guid` (`role_guid`),
  KEY `item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_element_heart
-- ----------------------------

-- ----------------------------
-- Table structure for `role_event`
-- ----------------------------
DROP TABLE IF EXISTS `role_event`;
CREATE TABLE `role_event` (
  `role_guid` bigint(20) NOT NULL,
  `event_guid1` bigint(20) DEFAULT '0',
  `event_guid2` bigint(20) DEFAULT '0',
  `event_guid3` bigint(20) DEFAULT '0',
  `event_guid4` bigint(20) DEFAULT '0',
  `event_guid5` bigint(20) DEFAULT '0',
  `event_guid6` bigint(20) DEFAULT '0',
  `event_guid7` bigint(20) DEFAULT '0',
  `event_guid8` bigint(20) DEFAULT '0',
  `event_guid9` bigint(20) DEFAULT '0',
  `event_guid10` bigint(20) DEFAULT '0',
  `event_guid11` bigint(20) DEFAULT '0',
  `event_guid12` bigint(20) DEFAULT '0',
  `event_guid13` bigint(20) DEFAULT '0',
  `event_guid14` bigint(20) DEFAULT '0',
  `event_guid15` bigint(20) DEFAULT '0',
  `event_guid16` bigint(20) DEFAULT '0',
  `event_guid17` bigint(20) DEFAULT '0',
  `event_guid18` bigint(20) DEFAULT '0',
  `event_guid19` bigint(20) DEFAULT '0',
  `event_guid20` bigint(20) DEFAULT '0',
  `event_guid21` bigint(20) DEFAULT '0',
  `event_guid22` bigint(20) DEFAULT '0',
  `event_guid23` bigint(20) DEFAULT '0',
  `event_guid24` bigint(20) DEFAULT '0',
  `event_guid25` bigint(20) DEFAULT '0',
  `event_guid26` bigint(20) DEFAULT '0',
  `event_guid27` bigint(20) DEFAULT '0',
  `event_guid28` bigint(20) DEFAULT '0',
  `event_guid29` bigint(20) DEFAULT '0',
  `event_guid30` bigint(20) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_event
-- ----------------------------

-- ----------------------------
-- Table structure for `role_fight_att`
-- ----------------------------
DROP TABLE IF EXISTS `role_fight_att`;
CREATE TABLE `role_fight_att` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_unit_attack_att_none` double(11,0) DEFAULT '0',
  `e_unit_attack_att_strength` double(11,0) DEFAULT '0',
  `e_unit_attack_att_agility` double(11,0) DEFAULT '0',
  `e_unit_attack_att_intellect` double(11,0) DEFAULT '0',
  `e_unit_attack_att_stamina` double(11,0) DEFAULT '0',
  `e_unit_attack_att_attack_min` double(11,0) DEFAULT '0',
  `e_unit_attack_att_attack_max` double(11,0) DEFAULT '0',
  `e_unit_attack_att_armor` double(11,0) DEFAULT '0',
  `e_unit_attack_att_hp_max` double(11,0) DEFAULT '0',
  `e_unit_attack_att_hit_force` double(11,0) DEFAULT '0',
  `e_unit_attack_att_dodge_force` double(11,0) DEFAULT '0',
  `e_unit_attack_att_critical_rate` double(11,0) DEFAULT '0',
  `e_unit_attack_att_critical_armor_rate` double(11,0) DEFAULT '0',
  `e_unit_attack_att_critical_attack_rate` double(11,0) DEFAULT '0',
  `e_unit_attack_att_critical_attack_relief` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_attack_1` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_armor_1` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_attack_2` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_armor_2` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_attack_3` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_armor_3` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_attack_4` double(11,0) DEFAULT '0',
  `e_unit_attack_att_att_armor_4` double(11,0) DEFAULT '0',
  `e_unit_attack_att_element_ability` double(11,0) DEFAULT '0',
  `e_unit_attack_att_element_resist` double(11,0) DEFAULT '0',
  `e_unit_attack_att_effect_damage` double(11,0) DEFAULT '0',
  `e_unit_attack_att_effect_damage_relief` double(11,0) DEFAULT '0',
  `e_unit_attack_att_hit_prob` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_hit_prob` double(11,0) DEFAULT '0',
  `e_unit_attack_att_crit_prob` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_crit_prob` double(11,0) DEFAULT '0',
  `e_unit_attack_att_move_speed` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_dizzy` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_move_speed` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_frozen` double(11,0) DEFAULT '0',
  `e_unit_attack_att_un_no_skill` double(11,0) DEFAULT '0',
  `e_unit_attack_att_magic` double(11,0) DEFAULT '0',
  `e_unit_attack_att_body_power` double(11,0) DEFAULT '0',
  `e_unit_attack_att_exp_addition` double(11,0) DEFAULT NULL,
  `e_unit_attack_att_attack_npc` double DEFAULT NULL,
  `e_unit_attack_att_attack_player` double DEFAULT NULL,
  `e_unit_attack_att_exp_add_all` double DEFAULT NULL,
  `e_unit_attack_att_be_attack_player` double DEFAULT NULL,
  `e_unit_attack_att_pure_attack` double DEFAULT NULL,
  `e_unit_attack_att_pure_armor` double DEFAULT NULL,
  `e_unit_attack_att_attack_multiple` double DEFAULT NULL,
  `e_unit_attack_att_flight_energy` double DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_fight_att
-- ----------------------------

-- ----------------------------
-- Table structure for `role_first_time_do`
-- ----------------------------
DROP TABLE IF EXISTS `role_first_time_do`;
CREATE TABLE `role_first_time_do` (
  `role_guid` bigint(20) NOT NULL,
  `do_type` int(11) NOT NULL,
  PRIMARY KEY (`role_guid`,`do_type`),
  KEY `role_guid` (`role_guid`),
  KEY `do_type` (`do_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_first_time_do
-- ----------------------------

-- ----------------------------
-- Table structure for `role_gain_treasure`
-- ----------------------------
DROP TABLE IF EXISTS `role_gain_treasure`;
CREATE TABLE `role_gain_treasure` (
  `role_guid` bigint(20) NOT NULL,
  `killed_field_boss` int(11) DEFAULT NULL,
  `killed_simple_boss` int(11) DEFAULT NULL,
  `buy_simple_boss` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_gain_treasure
-- ----------------------------

-- ----------------------------
-- Table structure for `role_goods`
-- ----------------------------
DROP TABLE IF EXISTS `role_goods`;
CREATE TABLE `role_goods` (
  `role_guid` bigint(20) NOT NULL,
  `e_goods_info_id` int(11) NOT NULL,
  `e_goods_info_owner_store` int(11) NOT NULL DEFAULT '0',
  `e_goods_info_purchased_num` int(11) DEFAULT '0',
  `e_goods_info_last_update_time` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_goods_info_id`,`e_goods_info_owner_store`),
  KEY `role_guid` (`role_guid`),
  KEY `e_goods_info_id` (`e_goods_info_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_goods
-- ----------------------------

-- ----------------------------
-- Table structure for `role_guide_record`
-- ----------------------------
DROP TABLE IF EXISTS `role_guide_record`;
CREATE TABLE `role_guide_record` (
  `role_guid` bigint(20) NOT NULL,
  `guide_record_id` int(11) NOT NULL,
  `state` int(11) NOT NULL,
  PRIMARY KEY (`role_guid`,`guide_record_id`),
  KEY `role_guid` (`role_guid`),
  KEY `guide_record_id` (`guide_record_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_guide_record
-- ----------------------------

-- ----------------------------
-- Table structure for `role_harry_record`
-- ----------------------------
DROP TABLE IF EXISTS `role_harry_record`;
CREATE TABLE `role_harry_record` (
  `role_guid` bigint(20) NOT NULL,
  `e_harry_record_harry_value` int(11) DEFAULT NULL,
  `e_harry_record_defend_value` int(11) DEFAULT NULL,
  `e_harry_record_normal_harry_count` int(11) DEFAULT NULL,
  `e_harry_record_special_harry_count` int(11) DEFAULT NULL,
  `e_harry_record_kill_ct_num` int(11) DEFAULT NULL,
  `e_harry_record_kill_terrorist_num` int(11) DEFAULT NULL,
  `e_harry_record_harry_item_server_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_harry_record
-- ----------------------------

-- ----------------------------
-- Table structure for `role_history_highest_record`
-- ----------------------------
DROP TABLE IF EXISTS `role_history_highest_record`;
CREATE TABLE `role_history_highest_record` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `single_equip_upgrade` bigint(20) DEFAULT '0',
  `total_equip_upgrade_num` bigint(20) DEFAULT '0',
  `total_equip_addon_num` bigint(20) DEFAULT '0',
  `item_succinct_reach_mark_level` bigint(20) DEFAULT '0',
  `total_sprite_equip_level` bigint(20) DEFAULT '0',
  `total_element_heart_equip_level` bigint(20) DEFAULT '0',
  `arena_higest` bigint(20) DEFAULT '0',
  `demon_tower_highest_level` bigint(20) DEFAULT '0',
  `higest_friend_num` bigint(20) DEFAULT '0',
  `higest_friendliness_value` bigint(20) DEFAULT '0',
  `higest_weapon_upgrade_buff` bigint(20) DEFAULT '0',
  `higest_equip_upgrade_buff` bigint(20) DEFAULT '0',
  `highest_rune_total_level` bigint(20) DEFAULT NULL,
  `highest_addon_weapon` bigint(20) DEFAULT NULL,
  `highest_addon_equip` bigint(20) DEFAULT NULL,
  `highest_belief_spring` bigint(20) DEFAULT NULL,
  `highest_belief_summer` bigint(20) DEFAULT NULL,
  `highest_belief_autumn` bigint(20) DEFAULT NULL,
  `highest_belief_winter` bigint(20) DEFAULT NULL,
  `weapon_full_succinct_grade` bigint(20) DEFAULT NULL,
  `equip_full_succinct_grade` bigint(20) DEFAULT NULL,
  `battle_achievement` bigint(20) DEFAULT NULL,
  `gs_value` bigint(20) DEFAULT NULL,
  `mount_value` bigint(20) DEFAULT NULL,
  `wing_value` bigint(20) DEFAULT NULL,
  `equip_value` bigint(20) DEFAULT NULL,
  `spirit_value` bigint(20) DEFAULT NULL,
  `max_awaken_level` bigint(20) DEFAULT NULL,
  `max_forge_level` bigint(20) DEFAULT NULL,
  `max_equip_enchant_level` bigint(20) DEFAULT NULL,
  `max_jewelry_enchant_level` bigint(20) DEFAULT NULL,
  `max_spirit_composit_level` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_history_highest_record
-- ----------------------------

-- ----------------------------
-- Table structure for `role_info`
-- ----------------------------
DROP TABLE IF EXISTS `role_info`;
CREATE TABLE `role_info` (
  `role_guid` bigint(20) NOT NULL,
  `account` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `role_appearance` varchar(1000) DEFAULT NULL,
  `role_first_kill_npc_id_arr` varchar(281) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `map_guid` bigint(20) DEFAULT '0',
  `e_role_i64_info_gs_value` bigint(20) DEFAULT NULL,
  `e_role_i64_info_history_best_gs_value` bigint(20) DEFAULT NULL,
  `e_role_i64_info_zero_mount_up_start_value` bigint(20) DEFAULT NULL,
  `e_role_i64_info_zero_wing_up_start_value` bigint(20) DEFAULT NULL,
  `e_role_i64_info_zero_equip_up_start_value` bigint(20) DEFAULT NULL,
  `e_role_i64_info_zero_spirit_up_start_value` bigint(20) DEFAULT NULL,
  `e_role_info_template_id` int(11) DEFAULT '0',
  `e_role_info_server_id` int(11) DEFAULT NULL,
  `e_role_info_img_id` int(11) unsigned DEFAULT '0',
  `e_role_info_class_type` int(11) unsigned DEFAULT '0',
  `e_role_info_class_branch` int(11) DEFAULT '0',
  `e_role_info_gender` int(11) unsigned DEFAULT '0',
  `e_role_info_exp_level` int(11) unsigned DEFAULT '0',
  `e_role_info_vip_level` int(11) unsigned DEFAULT '0',
  `e_role_info_vip_experience_level` int(11) DEFAULT '0',
  `e_role_info_recharge_num` int(11) DEFAULT '0',
  `e_role_info_main_map_id` int(11) DEFAULT '0',
  `e_role_info_main_pos_x` int(11) DEFAULT '0',
  `e_role_info_main_pos_y` int(11) DEFAULT '0',
  `e_role_info_main_pos_z` int(11) DEFAULT '0',
  `e_role_info_main_pos_yaw` int(11) DEFAULT '0',
  `e_role_info_main_line_id` int(11) DEFAULT '0',
  `e_role_info_move_map_id` int(11) DEFAULT '0',
  `e_role_info_move_pos_x` int(11) DEFAULT '0',
  `e_role_info_move_pos_y` int(11) DEFAULT '1',
  `e_role_info_move_pos_z` int(11) DEFAULT '0',
  `e_role_info_move_pos_yaw` int(11) DEFAULT '0',
  `e_role_info_move_line_id` int(11) DEFAULT '0',
  `e_role_info_GS_value` int(11) DEFAULT '0',
  `e_role_info_upgrade_id` int(11) DEFAULT '0',
  `e_role_info_daily_done_num` int(11) DEFAULT '0',
  `e_role_info_against_done_num` int(11) DEFAULT '0',
  `e_role_info_wing_showd_template_id` int(11) DEFAULT '0',
  `e_role_info_mount_showd_template_id` int(11) DEFAULT '0',
  `e_role_info_element_heart_drop_group` int(11) DEFAULT '0',
  `e_role_info_arena_times` int(11) DEFAULT '0',
  `e_role_info_arena_vip_times` int(11) DEFAULT '0',
  `e_role_info_arena_vip_buy_times` int(11) DEFAULT '0',
  `e_role_info_arena_winning_streak` int(11) DEFAULT '0',
  `e_role_info_legion_donate_earn_by_money` int(11) DEFAULT '0',
  `e_role_info_legion_donate_earn_by_item` int(11) DEFAULT '0',
  `e_role_info_worship_other_time` int(11) DEFAULT '0',
  `e_role_info_get_func_unlock_award_record` int(11) DEFAULT '0',
  `e_role_info_vip_item_geted_level` int(11) DEFAULT '0',
  `e_role_info_vip_free_revive_time` int(11) DEFAULT '0',
  `e_role_info_element_heart_refine_time` int(11) DEFAULT '0',
  `e_role_info_total_login_days` int(11) DEFAULT '0',
  `e_role_info_continue_login_time` int(11) DEFAULT '0',
  `e_role_info_daily_activity_degree` int(11) DEFAULT '0',
  `e_role_info_daily_recharge_num` int(11) DEFAULT '0',
  `e_role_info_total_diamond_cost_num` int(11) DEFAULT '0',
  `e_role_info_daily_active_degree_get_flag` int(11) DEFAULT '0',
  `e_role_info_fund_recharge_num` int(11) DEFAULT '0',
  `e_role_info_fund_payment_flag` int(11) DEFAULT '0',
  `e_role_info_fund_time_get_flag` int(11) DEFAULT '0',
  `e_role_info_fund_pride_get_flag` int(11) DEFAULT '0',
  `e_role_info_fund_reborn_get_flag` int(11) DEFAULT '0',
  `e_role_info_fund_cost_num` int(11) DEFAULT '0',
  `e_role_info_lord_of_city_worship_time` int(11) DEFAULT '0',
  `e_role_info_lord_of_war_worship_time` int(11) DEFAULT '0',
  `e_role_info_lord_of_sanctuary_worship_time` int(11) DEFAULT '0',
  `e_role_info_lord_of_god_worship_time` int(11) DEFAULT '0',
  `e_role_info_hope_value` int(11) DEFAULT '0',
  `e_role_info_del_time` int(11) DEFAULT '0',
  `e_role_info_kill_player_num` int(11) DEFAULT '0',
  `e_role_info_spirit_value` int(11) DEFAULT '0',
  `e_role_info_watch_count` int(11) DEFAULT '30',
  `e_role_info_crystal_dreamland_chests` int(11) DEFAULT '0',
  `e_role_info_protect_mark_slot_open_num` int(11) DEFAULT '0',
  `e_role_info_element_heart_slot_open_num` int(11) DEFAULT '0',
  `e_role_info_sprite_equip_slot_open_num` int(11) DEFAULT '0',
  `e_role_info_exclusive_flag` int(11) DEFAULT '0',
  `e_role_info_red_package_geted_times` int(11) DEFAULT '0',
  `e_role_info_free_red_package_send_times` int(11) DEFAULT '0',
  `e_role_info_world_boss_chests` int(11) DEFAULT '0',
  `e_role_info_leave_map_id` int(11) DEFAULT '0',
  `e_role_info_assist_chests` int(11) DEFAULT '0',
  `e_role_info_next_side_mission_main_trigger_id` int(11) DEFAULT '0',
  `e_role_info_show_fashion` int(11) DEFAULT '1',
  `e_role_info_real_recharge_jewel_num` int(11) DEFAULT '0',
  `e_role_info_relic_kill_num` int(11) DEFAULT '0',
  `e_role_info_relic_exp_num` int(11) DEFAULT '0',
  `e_role_info_head_frame` int(11) DEFAULT '0',
  `e_role_info_fund_pride_cost_value` int(11) DEFAULT '0',
  `e_role_info_tinder_prop_slot_open_num` int(11) DEFAULT '0',
  `e_role_info_show_sky_suit_shape` int(11) DEFAULT '0',
  `e_role_info_move_server_id` int(11) DEFAULT NULL,
  `e_role_info_move_war_idex` int(11) DEFAULT NULL,
  `e_role_info_goddess_slot_flag` int(11) DEFAULT NULL,
  `e_role_info_daily_cost` int(11) DEFAULT '0',
  `e_role_info_marry_done_num` int(11) DEFAULT '0',
  `e_role_info_marry_dati_id` int(11) DEFAULT '0',
  `e_role_info_marry_dati_num` int(11) DEFAULT '0',
  `e_role_info_marry_dati_right_num` int(11) DEFAULT '0',
  `e_role_info_marry_dati_current_answer` int(11) DEFAULT '0',
  `e_role_info_legion_dance_exp_num` int(11) DEFAULT '0',
  `e_role_info_legion_donate_num` int(11) DEFAULT '0',
  `e_role_info_appearance_id` int(11) DEFAULT '0',
  `e_role_info_is_show_vip` int(11) DEFAULT '0',
  `e_role_info_need_check_guide` int(11) DEFAULT '0',
  `e_role_info_enchant_show_type` int(11) DEFAULT '0',
  `e_role_info_push_map_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  UNIQUE KEY `role_name` (`role_name`) USING BTREE,
  KEY `account` (`account`) USING HASH,
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of role_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_invited`
-- ----------------------------
DROP TABLE IF EXISTS `role_invited`;
CREATE TABLE `role_invited` (
  `role_guid` bigint(20) NOT NULL,
  `target_guid` bigint(20) NOT NULL,
  `state` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`target_guid`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_invited
-- ----------------------------

-- ----------------------------
-- Table structure for `role_item`
-- ----------------------------
DROP TABLE IF EXISTS `role_item`;
CREATE TABLE `role_item` (
  `role_guid` bigint(20) NOT NULL,
  `item_guid` bigint(20) NOT NULL,
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) NOT NULL DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT '0',
  `e_item_info_illusion_had_byte` int(11) DEFAULT '0',
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`role_guid`,`item_guid`,`e_item_info_container_type`),
  KEY `role_guid` (`role_guid`),
  KEY `item_guid` (`item_guid`),
  KEY `e_item_info_container_type` (`e_item_info_container_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_item
-- ----------------------------

-- ----------------------------
-- Table structure for `role_item_upgrade`
-- ----------------------------
DROP TABLE IF EXISTS `role_item_upgrade`;
CREATE TABLE `role_item_upgrade` (
  `role_guid` bigint(20) NOT NULL,
  `data_array` text,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_item_upgrade
-- ----------------------------

-- ----------------------------
-- Table structure for `role_legion_skill`
-- ----------------------------
DROP TABLE IF EXISTS `role_legion_skill`;
CREATE TABLE `role_legion_skill` (
  `role_guid` bigint(20) NOT NULL,
  `legion_skill_id` int(11) NOT NULL,
  `curr_level` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`legion_skill_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_legion_skill
-- ----------------------------

-- ----------------------------
-- Table structure for `role_logic`
-- ----------------------------
DROP TABLE IF EXISTS `role_logic`;
CREATE TABLE `role_logic` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `data_array` text,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_logic
-- ----------------------------

-- ----------------------------
-- Table structure for `role_mail`
-- ----------------------------
DROP TABLE IF EXISTS `role_mail`;
CREATE TABLE `role_mail` (
  `role_guid` bigint(20) NOT NULL,
  `mail_guid` bigint(20) NOT NULL,
  `EMailInfo_IsNeedDelete` int(11) DEFAULT '0',
  `EMailInfo_SendTime_num` int(11) DEFAULT '0',
  `EMailInfo_ItemGuid1` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid2` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid3` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid4` bigint(11) DEFAULT '0',
  `EMailInfo_ItemGuid5` bigint(11) DEFAULT '0',
  `EMailInfo_MoneyTyp1` int(11) DEFAULT '0',
  `EMailInfo_MoneyNum1` int(11) DEFAULT '0',
  `EMailInfo_MoneyTyp2` int(11) DEFAULT '0',
  `EMailInfo_MoneyNum2` int(11) DEFAULT '0',
  `EMailInfo_ReadState` int(11) DEFAULT '0',
  `EMailInfo_SenderGuid` bigint(11) DEFAULT '0',
  `server_id` int(11) DEFAULT NULL,
  `text_contents` varchar(401) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `sender_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `mail_title` varchar(65) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`role_guid`,`mail_guid`),
  KEY `role_guid` (`role_guid`) USING BTREE,
  KEY `mail_guid` (`mail_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_mail
-- ----------------------------

-- ----------------------------
-- Table structure for `role_mail_by_id`
-- ----------------------------
DROP TABLE IF EXISTS `role_mail_by_id`;
CREATE TABLE `role_mail_by_id` (
  `role_guid` bigint(20) NOT NULL,
  `create_time` bigint(20) NOT NULL DEFAULT '0',
  `show_send_time` int(11) DEFAULT '0',
  `mail_id` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_guid`,`create_time`,`mail_id`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_mail_by_id
-- ----------------------------

-- ----------------------------
-- Table structure for `role_mail_item`
-- ----------------------------
DROP TABLE IF EXISTS `role_mail_item`;
CREATE TABLE `role_mail_item` (
  `role_guid` bigint(20) NOT NULL,
  `item_guid` bigint(20) NOT NULL,
  `e_item_info_info_id` int(11) DEFAULT '0',
  `e_item_info_slot` int(11) DEFAULT '0',
  `e_item_info_container_type` int(11) DEFAULT '0',
  `e_item_info_stack_count` int(11) DEFAULT '0',
  `e_item_info_activate` int(11) DEFAULT '0',
  `e_item_info_is_first` int(11) DEFAULT '0',
  `e_item_info_upgrade_count` int(11) DEFAULT '0',
  `e_item_info_stargrade_count` int(11) DEFAULT '0',
  `e_item_info_starskill_flag` int(11) DEFAULT '0',
  `e_item_info_add_on` int(11) DEFAULT '0',
  `e_item_info_locked` int(11) DEFAULT '0',
  `e_item_info_wing_exp` int(11) DEFAULT '0',
  `e_item_info_wing_bless` int(11) DEFAULT '0',
  `e_item_info_random_had_flag` int(11) DEFAULT '0',
  `e_item_info_random_property1` int(11) DEFAULT '0',
  `e_item_info_random_property2` int(11) DEFAULT '0',
  `e_item_info_random_property3` int(11) DEFAULT '0',
  `e_item_info_random_property4` int(11) DEFAULT '0',
  `e_item_info_random_property5` int(11) DEFAULT '0',
  `e_item_info_random_property6` int(11) DEFAULT '0',
  `e_item_info_succinct_property1` int(11) DEFAULT '0',
  `e_item_info_succinct_property2` int(11) DEFAULT '0',
  `e_item_info_succinct_property3` int(11) DEFAULT '0',
  `e_item_info_succinct_property4` int(11) DEFAULT '0',
  `e_item_info_succinct_property5` int(11) DEFAULT '0',
  `e_item_info_succinct_property6` int(11) DEFAULT '0',
  `e_item_info_showing_illusion_idex` int(11) DEFAULT '0',
  `e_item_info_put_in_bag_count` int(11) DEFAULT NULL,
  `e_item_info_illusion_had_byte` int(11) DEFAULT NULL,
  `e_item_info_over_time` int(11) DEFAULT '0',
  `e_item_info_succinct_level` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_1` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_2` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_3` int(11) DEFAULT '0',
  `e_item_info_jewel_slot_4` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_0` int(11) DEFAULT '0',
  `e_item_info_jewel_vip_slot_1` int(11) DEFAULT '0',
  `e_item_info_prototype_id` int(11) DEFAULT '0',
  `e_item_info_forge_property1` int(11) DEFAULT '0',
  `e_item_info_forge_property2` int(11) DEFAULT '0',
  `e_item_info_forge_property3` int(11) DEFAULT '0',
  `e_item_info_forge_property4` int(11) DEFAULT '0',
  `e_item_info_forge_property5` int(11) DEFAULT '0',
  `e_item_info_forge_property6` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_0` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_1` int(11) DEFAULT '0',
  `e_item_info_jewel_carve_2` int(11) DEFAULT '0',
  `e_item_info_jewel_lucky_carve` int(11) DEFAULT '0',
  `param_str` varchar(41) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`role_guid`,`item_guid`),
  KEY `role_guid` (`role_guid`),
  KEY `item_guid` (`item_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_mail_item
-- ----------------------------

-- ----------------------------
-- Table structure for `role_map_record`
-- ----------------------------
DROP TABLE IF EXISTS `role_map_record`;
CREATE TABLE `role_map_record` (
  `role_guid` bigint(20) NOT NULL,
  `data_array` text,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_map_record
-- ----------------------------

-- ----------------------------
-- Table structure for `role_marry`
-- ----------------------------
DROP TABLE IF EXISTS `role_marry`;
CREATE TABLE `role_marry` (
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `mate_guid` bigint(20) DEFAULT NULL,
  `mate_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `marry_state` int(11) DEFAULT NULL,
  `save_time` int(11) DEFAULT NULL,
  `role_class_template_id` int(11) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `mate_class_template_id` int(11) DEFAULT NULL,
  `mate_level` int(11) DEFAULT NULL,
  `marry_item_id` int(11) DEFAULT '0',
  `role_ring_level` int(11) DEFAULT '0',
  `mate_ring_level` int(11) DEFAULT '0',
  `heart_value` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of role_marry
-- ----------------------------

-- ----------------------------
-- Table structure for `role_mission`
-- ----------------------------
DROP TABLE IF EXISTS `role_mission`;
CREATE TABLE `role_mission` (
  `role_guid` bigint(20) NOT NULL,
  `emid_id` int(11) NOT NULL,
  `emid_mission_type` int(11) DEFAULT '0',
  `emid_mission_slot` int(11) DEFAULT '0',
  `emid_goal1_num` int(11) DEFAULT '0',
  `emid_goal2_num` int(11) DEFAULT '0',
  `emid_goal3_num` int(11) DEFAULT '0',
  `emid_goal4_num` int(11) DEFAULT '0',
  `emid_star_level` int(11) DEFAULT '0',
  `emid_mission_state` int(11) DEFAULT '0',
  `emid_finish_item_id1` int(11) DEFAULT '0',
  `emid_finish_item_num1` int(11) DEFAULT '0',
  `emid_finish_item_id2` int(11) DEFAULT '0',
  `emid_finish_item_num2` int(11) DEFAULT '0',
  `emid_finish_item_id3` int(11) DEFAULT '0',
  `emid_finish_item_num3` int(11) DEFAULT '0',
  `emid_finish_item_id4` int(11) DEFAULT '0',
  `emid_finish_item_num4` int(11) DEFAULT '0',
  `e_mission_inst_get_time` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`emid_id`),
  KEY `role_guid` (`role_guid`),
  KEY `emid_id` (`emid_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_mission
-- ----------------------------

-- ----------------------------
-- Table structure for `role_money`
-- ----------------------------
DROP TABLE IF EXISTS `role_money`;
CREATE TABLE `role_money` (
  `role_guid` bigint(20) NOT NULL,
  `e_money_save_type` int(11) NOT NULL,
  `e_money_type_exp` bigint(20) DEFAULT '0',
  `e_money_type_silver` bigint(20) DEFAULT '0',
  `e_money_type_silver_bind` bigint(20) DEFAULT '0',
  `e_money_type_jewel` bigint(20) DEFAULT '0',
  `e_money_type_jewel_bind` bigint(20) DEFAULT '0',
  `e_money_type_belief_point` bigint(20) DEFAULT '0',
  `e_money_type_magic_crystal` bigint(20) DEFAULT '0',
  `e_money_type_remodeling_point` bigint(20) DEFAULT '0',
  `e_money_type_spirit_score` bigint(20) DEFAULT '0',
  `e_money_type_spirit_crystal` bigint(20) DEFAULT '0',
  `e_money_type_pound` bigint(20) DEFAULT '0',
  `e_money_type_guard_point` bigint(20) DEFAULT '0',
  `e_money_type_achievement_point` bigint(20) DEFAULT '0',
  `e_money_type_reputation` bigint(20) DEFAULT '0',
  `e_money_type_hope_point` bigint(20) DEFAULT '0',
  `e_money_type_battle_achievement` bigint(20) DEFAULT '0',
  `e_money_type_cross_honor` bigint(20) DEFAULT '0',
  `e_money_type_talent` bigint(20) DEFAULT '0',
  `e_money_type_assist_fighting` bigint(20) DEFAULT NULL,
  `e_money_type_treasure_score` bigint(20) DEFAULT NULL,
  `e_money_type_belief_rune_piece` bigint(20) DEFAULT '0',
  `e_money_type_legion_warehouse_score` bigint(20) DEFAULT NULL,
  `e_money_type_attribute_talent` bigint(20) DEFAULT NULL,
  `e_money_type_world_essence` bigint(20) DEFAULT NULL,
  `e_money_type_cross_server_money` bigint(20) DEFAULT NULL,
  `e_money_type_red_diamond` bigint(20) DEFAULT '0',
  `e_money_type_feather_piece` bigint(20) DEFAULT '0',
  `e_money_type_cross_ladder_honor` bigint(20) DEFAULT '0',
  `e_money_type_season_point` bigint(20) DEFAULT '0',
  `e_money_type_element_score` bigint(20) DEFAULT '0',
  `e_money_type_element_point` bigint(20) DEFAULT '0',
  `e_money_type_energy` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`e_money_save_type`),
  KEY `role_guid` (`role_guid`),
  KEY `e_money_save_type` (`e_money_save_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_money
-- ----------------------------

-- ----------------------------
-- Table structure for `role_offline_chat`
-- ----------------------------
DROP TABLE IF EXISTS `role_offline_chat`;
CREATE TABLE `role_offline_chat` (
  `chat_guid` bigint(20) NOT NULL,
  `role_guid` bigint(20) NOT NULL,
  `sender_guid` bigint(20) NOT NULL,
  `sender_name` varchar(100) NOT NULL,
  `chat_type` int(11) NOT NULL,
  `content_text` varchar(300) NOT NULL,
  `sender_template_id` int(11) DEFAULT NULL,
  `relation_template_id` int(11) DEFAULT NULL,
  `relation_level` int(11) DEFAULT NULL,
  `vip_title_template_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`chat_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_offline_chat
-- ----------------------------

-- ----------------------------
-- Table structure for `role_passive_skill`
-- ----------------------------
DROP TABLE IF EXISTS `role_passive_skill`;
CREATE TABLE `role_passive_skill` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `data_array` text NOT NULL,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_passive_skill
-- ----------------------------

-- ----------------------------
-- Table structure for `role_person_information`
-- ----------------------------
DROP TABLE IF EXISTS `role_person_information`;
CREATE TABLE `role_person_information` (
  `role_guid` bigint(20) NOT NULL,
  `infor_state` int(11) NOT NULL DEFAULT '0',
  `reward_state` int(11) NOT NULL DEFAULT '0',
  `gender` varchar(100) DEFAULT NULL,
  `personalized_signature` varchar(100) DEFAULT NULL,
  `birthday_year` int(11) NOT NULL DEFAULT '0',
  `birthday_mouth` int(11) NOT NULL DEFAULT '0',
  `birthday_day` int(11) NOT NULL DEFAULT '0',
  `city` varchar(100) DEFAULT NULL,
  `hometown` varchar(100) DEFAULT NULL,
  `online_state` varchar(100) DEFAULT '0',
  `tag1` int(11) NOT NULL DEFAULT '0',
  `tag2` int(11) NOT NULL DEFAULT '0',
  `tag3` int(11) NOT NULL DEFAULT '0',
  `tag4` int(11) NOT NULL DEFAULT '0',
  `tag5` int(11) NOT NULL DEFAULT '0',
  `tag6` int(11) NOT NULL DEFAULT '0',
  `tag7` int(11) NOT NULL DEFAULT '0',
  `tag8` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_person_information
-- ----------------------------

-- ----------------------------
-- Table structure for `role_phantom`
-- ----------------------------
DROP TABLE IF EXISTS `role_phantom`;
CREATE TABLE `role_phantom` (
  `role_guid` bigint(20) NOT NULL,
  `e_phm_config_id` int(11) NOT NULL DEFAULT '0',
  `e_phm_state` int(11) NOT NULL DEFAULT '0',
  `e_phm_item_1` int(11) DEFAULT '0',
  `e_phm_item_2` int(11) DEFAULT '0',
  `e_phm_item_3` int(11) DEFAULT '0',
  `e_phm_item_4` int(11) DEFAULT '0',
  `e_phm_item_5` int(11) DEFAULT '0',
  `e_phm_item_6` int(11) DEFAULT '0',
  `e_phm_item_7` int(11) DEFAULT '0',
  `e_phm_item_8` int(11) DEFAULT '0',
  `e_phm_item_9` int(11) DEFAULT '0',
  `e_phm_item_10` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_phm_config_id`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_phantom
-- ----------------------------

-- ----------------------------
-- Table structure for `role_pokedex`
-- ----------------------------
DROP TABLE IF EXISTS `role_pokedex`;
CREATE TABLE `role_pokedex` (
  `role_guid` bigint(20) NOT NULL,
  `e_pokedex_Info_template_id` int(11) NOT NULL,
  `e_pokedex_Info_upgrade_num` int(11) DEFAULT '0',
  `e_pokedex_Info_state` int(11) DEFAULT '0',
  `e_pokedex_Info_fetter_state_1` int(11) DEFAULT '0',
  `e_pokedex_Info_fetter_state_2` int(11) DEFAULT '0',
  `e_pokedex_Info_fetter_state_3` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_1` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_2` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_3` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_4` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_5` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_6` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_7` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_8` int(11) DEFAULT '0',
  `e_pokedex_Info_psyche_num_9` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_10` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_11` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_12` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_13` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_14` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_15` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_16` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_17` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_18` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_19` int(11) DEFAULT NULL,
  `e_pokedex_Info_psyche_num_20` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`e_pokedex_Info_template_id`),
  KEY `role_guid` (`role_guid`),
  KEY `Pokedex_index` (`e_pokedex_Info_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_pokedex
-- ----------------------------

-- ----------------------------
-- Table structure for `role_ranking`
-- ----------------------------
DROP TABLE IF EXISTS `role_ranking`;
CREATE TABLE `role_ranking` (
  `role_guid` bigint(20) NOT NULL,
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '0',
  `ERankingInfo_ranking_type` bigint(20) NOT NULL DEFAULT '0',
  `ERankingInfo_ranking_value` bigint(20) DEFAULT '0',
  `ERankingInfo_ranking_time` bigint(20) DEFAULT '0',
  `ERankingInfo_supporting_guid` bigint(20) DEFAULT '0',
  `ERankingInfo_supporting_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '0',
  `ERankingInfo_vip_level` int(11) DEFAULT NULL,
  `ERankingInfo_peak` int(11) unsigned zerofill DEFAULT '00000000000',
  `ERankingInfo_server_id` int(11) DEFAULT NULL,
  `ERankingInfo_game_channel` varchar(129) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `ERankingInfo_stanby_parameter` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`ERankingInfo_ranking_type`),
  KEY `role_guid` (`role_guid`),
  KEY `ERankingInfo_ranking_type` (`ERankingInfo_ranking_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_ranking
-- ----------------------------

-- ----------------------------
-- Table structure for `role_recycle`
-- ----------------------------
DROP TABLE IF EXISTS `role_recycle`;
CREATE TABLE `role_recycle` (
  `role_guid` bigint(20) NOT NULL,
  `e_start_time` int(11) NOT NULL,
  `e_reward_state` int(11) DEFAULT '0',
  `e_prop_state` int(11) DEFAULT '0',
  `inviter_guid` bigint(20) DEFAULT '0',
  `e_invited_state` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_start_time`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_recycle
-- ----------------------------

-- ----------------------------
-- Table structure for `role_recycle_task`
-- ----------------------------
DROP TABLE IF EXISTS `role_recycle_task`;
CREATE TABLE `role_recycle_task` (
  `role_guid` bigint(20) NOT NULL,
  `e_config_id` int(11) NOT NULL,
  `e_finish_num` int(11) DEFAULT '0',
  `e_state` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_config_id`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_recycle_task
-- ----------------------------

-- ----------------------------
-- Table structure for `role_relation`
-- ----------------------------
DROP TABLE IF EXISTS `role_relation`;
CREATE TABLE `role_relation` (
  `role_guid` bigint(20) NOT NULL,
  `relation_guid` bigint(20) NOT NULL,
  `relation_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `relation_gs` bigint(20) DEFAULT NULL,
  `e_relation_data_type` int(11) NOT NULL DEFAULT '0',
  `e_relation_data_level` int(11) DEFAULT '0',
  `e_relation_data_template_id` int(11) DEFAULT '0',
  `e_relation_data_gs` int(11) DEFAULT '0',
  `e_relation_data_relation_state` int(11) unsigned DEFAULT NULL,
  `e_relation_data_last_login_time` int(11) DEFAULT NULL,
  `e_relation_data_friendliness` int(11) DEFAULT NULL,
  `e_relation_data_vip_title_template_id` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`relation_guid`,`e_relation_data_type`),
  KEY `role_guid` (`role_guid`),
  KEY `relation_guid` (`relation_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_relation
-- ----------------------------

-- ----------------------------
-- Table structure for `role_service_goal`
-- ----------------------------
DROP TABLE IF EXISTS `role_service_goal`;
CREATE TABLE `role_service_goal` (
  `role_guid` bigint(20) NOT NULL,
  `eaid_id` int(11) NOT NULL,
  `eaid_schedule` int(11) DEFAULT NULL,
  `eaid_state` int(11) DEFAULT NULL,
  `eaid_active_state` int(11) DEFAULT NULL,
  `eaid_over_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`eaid_id`),
  KEY `role_guid` (`role_guid`),
  KEY `eaid_id` (`eaid_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_service_goal
-- ----------------------------

-- ----------------------------
-- Table structure for `role_service_rank`
-- ----------------------------
DROP TABLE IF EXISTS `role_service_rank`;
CREATE TABLE `role_service_rank` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `rank_type` int(11) NOT NULL,
  `role_num` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`rank_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_service_rank
-- ----------------------------

-- ----------------------------
-- Table structure for `role_service_rank_reward_state`
-- ----------------------------
DROP TABLE IF EXISTS `role_service_rank_reward_state`;
CREATE TABLE `role_service_rank_reward_state` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `rank_type` int(11) NOT NULL,
  `receive_reward_state` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`rank_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_service_rank_reward_state
-- ----------------------------

-- ----------------------------
-- Table structure for `role_seven_day_goal`
-- ----------------------------
DROP TABLE IF EXISTS `role_seven_day_goal`;
CREATE TABLE `role_seven_day_goal` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `seven_day_goal_id` int(11) NOT NULL DEFAULT '0',
  `seven_day_goal_schedule` int(11) DEFAULT NULL,
  `seven_day_goal_state` int(11) DEFAULT NULL,
  `seven_day_goal_open_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`seven_day_goal_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_seven_day_goal
-- ----------------------------

-- ----------------------------
-- Table structure for `role_show_info`
-- ----------------------------
DROP TABLE IF EXISTS `role_show_info`;
CREATE TABLE `role_show_info` (
  `role_guid` bigint(20) NOT NULL,
  `showed_mount_template_id` int(11) DEFAULT '0',
  `showed_wing_template_id` int(11) DEFAULT '0',
  `showed_mount_guid` bigint(20) DEFAULT '0',
  `showed_wing_guid` bigint(20) DEFAULT '0',
  `quick_call_mount_1` bigint(20) DEFAULT '0',
  `quick_call_mount_2` bigint(20) DEFAULT '0',
  `quick_call_mount_3` bigint(20) DEFAULT '0',
  `showed_quickly_hp_guid` bigint(20) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_show_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_skill`
-- ----------------------------
DROP TABLE IF EXISTS `role_skill`;
CREATE TABLE `role_skill` (
  `role_guid` bigint(20) NOT NULL,
  `e_skill_info_template_id` int(11) NOT NULL DEFAULT '0',
  `e_skill_info_cd_cur_time` int(11) DEFAULT '0',
  `e_skill_info_cd_max_time` int(11) DEFAULT '0',
  `e_skill_info_activated` int(11) DEFAULT '0',
  `e_skill_info_skill_type` int(11) DEFAULT '0',
  `e_skill_info_maturity` int(11) DEFAULT NULL,
  `e_skill_info_use_pos` int(11) DEFAULT NULL,
  `e_skill_info_level` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`e_skill_info_template_id`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_skill
-- ----------------------------

-- ----------------------------
-- Table structure for `role_skytreasure`
-- ----------------------------
DROP TABLE IF EXISTS `role_skytreasure`;
CREATE TABLE `role_skytreasure` (
  `role_guid` bigint(20) NOT NULL,
  `skytreasure_layer_num` int(11) NOT NULL DEFAULT '-1',
  `skytreasure_pos` int(11) NOT NULL DEFAULT '-1',
  `skytreasure_pos_in_array` int(11) NOT NULL DEFAULT '-1',
  PRIMARY KEY (`role_guid`,`skytreasure_layer_num`,`skytreasure_pos`,`skytreasure_pos_in_array`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_skytreasure
-- ----------------------------

-- ----------------------------
-- Table structure for `role_special_name`
-- ----------------------------
DROP TABLE IF EXISTS `role_special_name`;
CREATE TABLE `role_special_name` (
  `role_guid` bigint(20) NOT NULL,
  `AchievementLevel` int(11) DEFAULT '0',
  `AchievementStar` int(11) DEFAULT NULL,
  `RankLevel` int(11) DEFAULT '0',
  `RankStar` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of role_special_name
-- ----------------------------

-- ----------------------------
-- Table structure for `role_spirit`
-- ----------------------------
DROP TABLE IF EXISTS `role_spirit`;
CREATE TABLE `role_spirit` (
  `role_guid` bigint(20) NOT NULL,
  `spirit_guid` bigint(20) NOT NULL,
  `ESpiritInfo_template_id` int(11) DEFAULT '0',
  `ESpiritInfo_spirit_slot` int(11) DEFAULT '0',
  `ESpiritInfo_container_type` int(11) DEFAULT '0',
  `ESpiritInfo_spirit_is_equiped` int(11) DEFAULT '0',
  `ESpiritInfo_spirit_is_fighting` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_1` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_2` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_3` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_4` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_5` int(11) DEFAULT '0',
  `ESpiritInfo_flair_property_6` int(11) DEFAULT '0',
  `ESpiritInfo_spirit_typ` int(11) DEFAULT '0',
  `ESpiritInfo_showing_illusion_idex` int(11) DEFAULT '-1',
  `ESpiritInfo_illusion_had_byte` int(11) DEFAULT '0',
  `ESpiritInfo_upgrade_count` int(11) DEFAULT '0',
  `ESpiritInfo_star_upgrade_count` int(11) DEFAULT '0',
  `ESpiritInfo_spirit_exp` int(11) DEFAULT '0',
  `ESpiritInfo_random_has_flag` int(11) DEFAULT '0',
  `ESpiritInfo_star_skill_flag` int(11) DEFAULT '0',
  `ESpiritInfo_efficient_skill_id` int(11) DEFAULT '0',
  `ESpiritInfo_efficient_skill_id_2` int(11) unsigned DEFAULT '0',
  PRIMARY KEY (`role_guid`,`spirit_guid`),
  KEY `role_guid` (`role_guid`),
  KEY `spirit_guid` (`spirit_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_spirit
-- ----------------------------

-- ----------------------------
-- Table structure for `role_sprite_fetter`
-- ----------------------------
DROP TABLE IF EXISTS `role_sprite_fetter`;
CREATE TABLE `role_sprite_fetter` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `data_array` text,
  PRIMARY KEY (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_sprite_fetter
-- ----------------------------

-- ----------------------------
-- Table structure for `role_starark`
-- ----------------------------
DROP TABLE IF EXISTS `role_starark`;
CREATE TABLE `role_starark` (
  `role_guid` bigint(20) NOT NULL,
  `step` int(11) DEFAULT '0',
  `times` int(11) DEFAULT '0',
  `last_id` int(11) DEFAULT '0',
  `head` int(11) DEFAULT '0',
  `body` varchar(325) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_starark
-- ----------------------------

-- ----------------------------
-- Table structure for `role_subscribe_daily_info`
-- ----------------------------
DROP TABLE IF EXISTS `role_subscribe_daily_info`;
CREATE TABLE `role_subscribe_daily_info` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `must_type` int(11) NOT NULL DEFAULT '0',
  `info_state` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`must_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_subscribe_daily_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_talent`
-- ----------------------------
DROP TABLE IF EXISTS `role_talent`;
CREATE TABLE `role_talent` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_talent_info_template_id` int(11) NOT NULL DEFAULT '0',
  `e_talent_info_cast_money` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`e_talent_info_template_id`),
  KEY `role_guid` (`role_guid`),
  KEY `e_talent_info_template_id` (`e_talent_info_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_talent
-- ----------------------------

-- ----------------------------
-- Table structure for `role_time`
-- ----------------------------
DROP TABLE IF EXISTS `role_time`;
CREATE TABLE `role_time` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_time_type_create_time` bigint(20) DEFAULT '0',
  `e_time_type_login_time` bigint(20) DEFAULT '0',
  `e_time_type_logout_time` bigint(20) DEFAULT '0',
  `e_time_type_login_time_all` bigint(20) DEFAULT '0',
  `e_time_type_last_save_data_time` bigint(20) DEFAULT '0',
  `e_time_type_last_refresh_time` bigint(20) DEFAULT '0',
  `e_time_type_spirit_last_free_hunt_time` bigint(20) DEFAULT '0',
  `e_time_type_arena_last_update` bigint(20) DEFAULT '0',
  `e_time_type_arena_colling_time` bigint(20) DEFAULT '0',
  `e_time_type_hope_item_free_get_time` bigint(20) DEFAULT '0',
  `e_time_type_next_free_change_gold_time` bigint(20) DEFAULT '0',
  `e_time_type_have_meditation_time` bigint(20) DEFAULT '0',
  `e_time_type_month_card_activity_time` bigint(20) DEFAULT '0',
  `e_time_type_month_card_remain_time` bigint(20) DEFAULT '0',
  `e_time_type_bag_time` bigint(20) DEFAULT '0',
  `e_time_type_last_daily_info_refresh_time` bigint(20) DEFAULT '0',
  `e_time_type_ancient_battelfield` bigint(20) DEFAULT '0',
  `e_time_type_last_map_record_update_time` bigint(20) DEFAULT '0',
  `e_time_type_cur_level_time` bigint(20) DEFAULT '0',
  `e_time_type_get_occupy_city_daily_award_time` bigint(20) DEFAULT '0',
  `e_time_type_convert_last_update_time` bigint(20) DEFAULT '0',
  `e_time_type_arena_last_get_reward_time` bigint(20) DEFAULT '0',
  `e_time_type_map_auto_revive_time` bigint(20) DEFAULT '0',
  `e_time_type_exclusive_card_activity_time` bigint(20) DEFAULT '0',
  `e_time_type_last_month_info_refresh_time` bigint(20) DEFAULT '0',
  `e_time_type_welfare_last_refresh_time` bigint(20) DEFAULT '0',
  `e_time_type_active_degree_last_refresh_time` bigint(20) DEFAULT '0',
  `e_time_type_meditation_full_send_mail` bigint(20) DEFAULT '0',
  `e_time_type_legion_bonue_award_flags` bigint(20) DEFAULT '0',
  `e_time_type_vip_experience_level_time` bigint(20) DEFAULT '0',
  `e_time_type_treasure_jewel_free` bigint(20) DEFAULT '0',
  `e_time_type_treasure_element_free` bigint(20) DEFAULT '0',
  `e_time_type_treasure_spirit_free` bigint(20) DEFAULT '0',
  `e_time_type_storage_time` bigint(20) DEFAULT '0',
  `e_time_type_meditation_remaining_time` bigint(20) DEFAULT '0',
  `e_time_type_last_refresh_by_zero_no_cross` bigint(20) DEFAULT '0',
  `e_time_type_next_world_boss_can_count` bigint(20) DEFAULT '0',
  `e_time_type_last_harry_clear_time` bigint(20) DEFAULT '0',
  `e_time_type_last_refresh_by_zero_cross` bigint(20) DEFAULT '0',
  `e_time_type_last_refresh_gs_up_start_value_time` bigint(20) DEFAULT '0',
  `e_time_type_frist_recharge_time` bigint(20) DEFAULT '0',
  `e_time_type_exp_raid_join_time` bigint(20) DEFAULT '0',
  `e_time_type_exp_raid_skill_cd` bigint(20) DEFAULT '0',
  `e_time_type_exp_raid_extra_time` bigint(20) DEFAULT '0',
  `e_time_type_service_goal_close_time` bigint(20) DEFAULT '0',
  `e_time_type_first_recharge_open_time` bigint(20) DEFAULT '0',
  `e_time_type_fuben_level_buy_tip_time` bigint(20) DEFAULT '0',
  `e_time_type_money_buy_tip_time` bigint(20) DEFAULT '0',
  `e_time_type_red_convert_last_time` bigint(20) DEFAULT '0',
  `e_time_type_energy_back` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_time
-- ----------------------------

-- ----------------------------
-- Table structure for `role_time_limit_activity`
-- ----------------------------
DROP TABLE IF EXISTS `role_time_limit_activity`;
CREATE TABLE `role_time_limit_activity` (
  `role_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `type` int(11) DEFAULT '0',
  `start_time` int(11) DEFAULT '0',
  `end_time` int(11) DEFAULT '0',
  `act_guid` int(11) NOT NULL DEFAULT '0',
  `first_branch_id` int(11) DEFAULT NULL,
  `schedule_first` int(11) DEFAULT '0',
  `schedule_second` int(11) DEFAULT '0',
  `schedule_third` int(11) DEFAULT '0',
  `schedule_fourth` int(11) DEFAULT '0',
  `schedule_fifth` int(11) DEFAULT '0',
  `schedule_6` int(11) DEFAULT '0',
  `schedule_7` int(11) DEFAULT '0',
  `schedule_8` int(11) DEFAULT '0',
  `schedule_9` int(11) DEFAULT '0',
  `schedule_10` int(11) DEFAULT '0',
  `schedule_11` int(11) DEFAULT '0',
  `schedule_12` int(11) DEFAULT '0',
  `schedule_13` int(11) DEFAULT '0',
  `schedule_14` int(11) DEFAULT '0',
  `schedule_15` int(11) DEFAULT '0',
  `schedule_16` int(11) DEFAULT '0',
  `schedule_17` int(11) DEFAULT '0',
  `schedule_18` int(11) DEFAULT '0',
  `schedule_19` int(11) DEFAULT '0',
  `schedule_20` int(11) DEFAULT '0',
  `schedule_str_1` varchar(601) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `reward_flag` bigint(20) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`act_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of role_time_limit_activity
-- ----------------------------

-- ----------------------------
-- Table structure for `role_time_limit_activity_info`
-- ----------------------------
DROP TABLE IF EXISTS `role_time_limit_activity_info`;
CREATE TABLE `role_time_limit_activity_info` (
  `role_guid` bigint(20) unsigned NOT NULL DEFAULT '0',
  `type` int(11) NOT NULL DEFAULT '0',
  `start_time` int(11) DEFAULT '0',
  `end_time` int(11) DEFAULT '0',
  `schedule_first` int(11) DEFAULT '0',
  `schedule_second` int(11) DEFAULT '0',
  `schedule_third` int(11) DEFAULT '0',
  `schedule_fourth` int(11) DEFAULT '0',
  `schedule_fifth` int(11) DEFAULT '0',
  `reward_flag` bigint(20) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_time_limit_activity_info
-- ----------------------------

-- ----------------------------
-- Table structure for `role_tinder`
-- ----------------------------
DROP TABLE IF EXISTS `role_tinder`;
CREATE TABLE `role_tinder` (
  `role_guid` bigint(20) NOT NULL,
  `current_level_id` int(11) DEFAULT '0',
  `current_unlock_id` int(11) DEFAULT '0',
  `already_play_animation_id` int(11) DEFAULT '0',
  `total_star_level` int(11) DEFAULT '0',
  `add_star_level` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_tinder
-- ----------------------------

-- ----------------------------
-- Table structure for `role_title`
-- ----------------------------
DROP TABLE IF EXISTS `role_title`;
CREATE TABLE `role_title` (
  `role_guid` bigint(20) NOT NULL,
  `e_title_info_titletemplate_id` int(11) NOT NULL,
  `e_title_info_start_time` int(11) DEFAULT '0',
  `e_title_info_end_time` int(11) DEFAULT '0',
  `e_title_info_if_geted` int(11) DEFAULT '0',
  `e_title_info_if_need_see` int(11) DEFAULT NULL,
  `e_title_info_if_need_equip` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`e_title_info_titletemplate_id`),
  KEY `role_guid` (`role_guid`),
  KEY `e_title_info_titletemplate_id` (`e_title_info_titletemplate_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of role_title
-- ----------------------------

-- ----------------------------
-- Table structure for `role_treasure`
-- ----------------------------
DROP TABLE IF EXISTS `role_treasure`;
CREATE TABLE `role_treasure` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_treasure_record_time` int(11) NOT NULL DEFAULT '0',
  `e_treasure_record_treasure_type` int(11) DEFAULT NULL,
  `e_treasure_record_prize_type` int(11) DEFAULT NULL,
  `e_treasure_record_prize_0` int(11) DEFAULT NULL,
  `e_treasure_record_prize_1` int(11) DEFAULT NULL,
  `e_treasure_record_prize_2` int(11) DEFAULT NULL,
  `e_treasure_record_prize_3` int(11) DEFAULT NULL,
  `e_treasure_record_prize_4` int(11) DEFAULT NULL,
  `e_treasure_record_prize_5` int(11) DEFAULT NULL,
  `e_treasure_record_prize_6` int(11) DEFAULT NULL,
  `e_treasure_record_prize_7` int(11) DEFAULT NULL,
  `e_treasure_record_prize_8` int(11) DEFAULT NULL,
  `e_treasure_record_prize_9` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`e_treasure_record_time`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_treasure
-- ----------------------------

-- ----------------------------
-- Table structure for `role_welfare`
-- ----------------------------
DROP TABLE IF EXISTS `role_welfare`;
CREATE TABLE `role_welfare` (
  `role_guid` bigint(20) NOT NULL,
  `e_welfare_info_online_time_geted_phase` int(11) DEFAULT '0',
  `e_welfare_info_total_login_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_continue_login_get_phase` int(11) DEFAULT '0',
  `e_welfare_info_daily_activity_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_monthcard_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_daily_recharge_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_total_recharge_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_total_cost_geted_flag` int(11) DEFAULT '0',
  `e_welfare_info_exp_level_reward_flag` int(11) DEFAULT '0',
  `e_welfare_info_gs_level_reward_flag` int(11) DEFAULT '0',
  `e_welfare_info_first_recharge_geted` int(11) DEFAULT '0',
  `e_welfare_info_first_recharge_type_geted` int(11) DEFAULT NULL,
  `e_welfare_info_regular_check_in_flag` int(11) DEFAULT NULL,
  `e_welfare_info_exclusive_check_in_flag` int(11) DEFAULT NULL,
  `e_welfare_info_vip_double_check_in_flag` int(11) DEFAULT NULL,
  `e_welfare_info_is_today_check_in` int(11) DEFAULT NULL,
  `e_welfare_info_replace_times` int(11) DEFAULT NULL,
  `e_welfare_info_time_limit_reward_flag` int(11) DEFAULT '0',
  `e_welfare_info_cumulative_sign_in` int(11) DEFAULT '0',
  `e_welfare_type_weekend_recharge_reward` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_welfare
-- ----------------------------

-- ----------------------------
-- Table structure for `role_welfare_random_get`
-- ----------------------------
DROP TABLE IF EXISTS `role_welfare_random_get`;
CREATE TABLE `role_welfare_random_get` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `e_welfare_random_item_daily_online_0` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_1` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_2` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_3` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_4` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_5` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_6` int(11) DEFAULT '0',
  `e_welfare_random_item_daily_online_7` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_0` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_1` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_2` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_3` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_4` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_5` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_6` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_7` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_8` int(11) DEFAULT '0',
  `e_welfare_random_item_continue_login_9` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_welfare_random_get
-- ----------------------------

-- ----------------------------
-- Table structure for `role_worship_target`
-- ----------------------------
DROP TABLE IF EXISTS `role_worship_target`;
CREATE TABLE `role_worship_target` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `ranking_index` int(11) NOT NULL DEFAULT '0',
  `target_guid` bigint(20) NOT NULL DEFAULT '0',
  PRIMARY KEY (`role_guid`,`ranking_index`,`target_guid`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of role_worship_target
-- ----------------------------

-- ----------------------------
-- Table structure for `role_yesterday_must_do_remain`
-- ----------------------------
DROP TABLE IF EXISTS `role_yesterday_must_do_remain`;
CREATE TABLE `role_yesterday_must_do_remain` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_exp_level_in_that_day` int(11) DEFAULT '0',
  `date_info` int(11) DEFAULT '0',
  `role_vip_level_in_that_day` int(11) DEFAULT '0',
  `distance_today_day_count` int(11) NOT NULL DEFAULT '0',
  `role_arena_rank` int(11) NOT NULL DEFAULT '-1',
  `e_daily_must_do_typ_raid_exp` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_daily` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_devil_square` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_ancient_battlefield` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_siliver` int(11) DEFAULT '0',
  `e_daily_must_do_typ_community_fight` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_drame` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_bloody_castle` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_1` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_2` int(11) DEFAULT '0',
  `e_daily_must_do_typ_raid_multiple_3` int(11) DEFAULT '0',
  `e_daily_must_do_typ_demon_tower` int(11) DEFAULT '0',
  `e_daily_must_do_typ_angel_temple` int(11) DEFAULT '0',
  `e_daily_must_do_typ_pk_king` int(11) DEFAULT '0',
  `e_daily_must_do_typ_arena` int(11) DEFAULT '0',
  `e_daily_must_do_typ_crusade_mission` int(11) DEFAULT '0',
  `e_daily_must_do_typ_crystal_illusion` int(11) DEFAULT '0',
  `e_daily_must_do_typ_mingmie_xukong` int(11) DEFAULT '0',
  `e_daily_must_do_typ_out_board` int(11) DEFAULT '0',
  `e_daily_must_do_typ_convert` int(11) DEFAULT '0',
  `e_daily_must_do_typ_rank` int(11) DEFAULT '0',
  `e_daily_must_do_typ_gold_army` int(11) DEFAULT '0',
  `e_daily_must_do_typ_world_boss` int(11) DEFAULT '0',
  `e_daily_must_do_typ_sky_treasure_box` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_field_treasure_box` int(11) DEFAULT '0',
  `e_daily_must_do_typ_legion_boss` int(11) DEFAULT '0',
  `e_daily_must_do_typ_meditation` int(11) DEFAULT '0',
  `e_daily_must_do_typ_activaty_degree` int(11) DEFAULT '0',
  `e_daily_must_do_typ_city_war` int(11) DEFAULT '0',
  `e_daily_must_do_typ_cross_fight` int(11) DEFAULT '0',
  `e_daily_must_do_typ_gold_store` int(11) DEFAULT '0',
  `e_daily_must_do_typ_magic_crystal_store` int(11) DEFAULT '0',
  `e_daily_must_do_typ_ancient_coin_store` int(11) DEFAULT '0',
  `e_daily_must_do_typ_hope_store` int(11) DEFAULT '0',
  `e_daily_must_do_typ_legion_store` int(11) DEFAULT '0',
  `e_daily_must_do_typ_hope_item` int(11) DEFAULT '0',
  `e_daily_must_do_typ_shop` int(11) DEFAULT '0',
  `e_daily_must_do_typ_legion_bonfire` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_legion_war` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_boss_home` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_boss_island` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_belief_cloister` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_cross_pk` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_cross_rob` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_legion_dance` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_legion_answer` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_cross_ladder` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_single_boss` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_element_war` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_vip_boss_home` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_oracle_trial` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_cross_server_boss` int(11) DEFAULT NULL,
  `e_daily_must_do_typ_attack_city` int(11) DEFAULT NULL,
  PRIMARY KEY (`role_guid`,`distance_today_day_count`),
  KEY `role_guid` (`role_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- ----------------------------
-- Records of role_yesterday_must_do_remain
-- ----------------------------

-- ----------------------------
-- Table structure for `server_info`
-- ----------------------------
DROP TABLE IF EXISTS `server_info`;
CREATE TABLE `server_info` (
  `big_group_id` int(11) NOT NULL,
  `server_group_id` int(11) NOT NULL,
  `begin_time` int(11) DEFAULT NULL,
  `begin_cross_server_time` int(11) DEFAULT NULL,
  `server_state` int(11) DEFAULT NULL,
  `server_cross_state` int(11) DEFAULT NULL,
  `begin_first_cross_server_time` int(11) DEFAULT NULL,
  `begin_first_merge_server_time` int(11) DEFAULT NULL,
  `time_limit_ranking_level` int(11) DEFAULT NULL,
  `is_close_exchange` int(11) DEFAULT NULL,
  `is_only_create_role` int(11) DEFAULT NULL,
  `is_close_login` int(11) DEFAULT NULL,
  `is_begin_cross` int(11) DEFAULT NULL,
  `is_begin_cross_pk` int(11) DEFAULT NULL,
  `is_begin_cross_activity` int(11) DEFAULT NULL,
  `is_begin_cross_harry` int(11) DEFAULT NULL,
  `is_begin_cross_city_war` int(11) DEFAULT NULL,
  `is_begin_cross_territory_war` int(11) DEFAULT NULL,
  `is_begin_cross_world_boss` int(11) DEFAULT NULL,
  `is_begin_cross_ladder` int(11) DEFAULT NULL,
  `is_begin_cross_cloud` int(11) DEFAULT NULL,
  `is_begin_element_war` int(11) DEFAULT NULL,
  `is_begion_attack_city` int(11) DEFAULT NULL,
  PRIMARY KEY (`big_group_id`,`server_group_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of server_info
-- ----------------------------

-- ----------------------------
-- Table structure for `server_refresh`
-- ----------------------------
DROP TABLE IF EXISTS `server_refresh`;
CREATE TABLE `server_refresh` (
  `time_type` int(11) NOT NULL,
  `refresh_time` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`time_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of server_refresh
-- ----------------------------

-- ----------------------------
-- Table structure for `time_feed_back_info`
-- ----------------------------
DROP TABLE IF EXISTS `time_feed_back_info`;
CREATE TABLE `time_feed_back_info` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `sub_template_id` int(11) NOT NULL DEFAULT '0',
  `info_state` int(11) DEFAULT '0',
  `get_time` int(11) DEFAULT '0',
  `end_time` int(11) DEFAULT '0',
  `money_type` int(11) DEFAULT '0',
  `money_num` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`template_id`,`sub_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of time_feed_back_info
-- ----------------------------

-- ----------------------------
-- Table structure for `time_limit_activity_ws_info`
-- ----------------------------
DROP TABLE IF EXISTS `time_limit_activity_ws_info`;
CREATE TABLE `time_limit_activity_ws_info` (
  `act_type` int(11) NOT NULL,
  `act_guid` int(11) DEFAULT NULL,
  `act_refresh_time` int(11) DEFAULT NULL,
  `act_branch_temp_id` int(11) DEFAULT NULL,
  `act_schedule` int(11) DEFAULT NULL,
  `act_flag` int(11) DEFAULT NULL,
  `param_string_1` varchar(3200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_string_2` varchar(3200) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`act_type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of time_limit_activity_ws_info
-- ----------------------------

-- ----------------------------
-- Table structure for `time_limit_branch_template`
-- ----------------------------
DROP TABLE IF EXISTS `time_limit_branch_template`;
CREATE TABLE `time_limit_branch_template` (
  `act_guid` int(11) NOT NULL DEFAULT '0',
  `attribute_id` int(11) DEFAULT NULL,
  `activity_type` int(11) DEFAULT NULL,
  `cycle_index` int(11) DEFAULT NULL,
  `condition` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `reward` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `text_image` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr1` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr2` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr3` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr4` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr5` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  PRIMARY KEY (`act_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of time_limit_branch_template
-- ----------------------------

-- ----------------------------
-- Table structure for `time_limit_gift_info`
-- ----------------------------
DROP TABLE IF EXISTS `time_limit_gift_info`;
CREATE TABLE `time_limit_gift_info` (
  `role_guid` bigint(20) NOT NULL DEFAULT '0',
  `template_id` int(11) NOT NULL DEFAULT '0',
  `state_info` int(11) DEFAULT '0',
  `begin_time` int(11) DEFAULT '0',
  `end_time` int(11) DEFAULT '0',
  `trigger_num` int(11) DEFAULT '0',
  PRIMARY KEY (`role_guid`,`template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of time_limit_gift_info
-- ----------------------------

-- ----------------------------
-- Table structure for `time_limit_template`
-- ----------------------------
DROP TABLE IF EXISTS `time_limit_template`;
CREATE TABLE `time_limit_template` (
  `act_guid` int(11) NOT NULL DEFAULT '0',
  `attribute_id` int(11) DEFAULT NULL,
  `activity_type` int(11) DEFAULT NULL,
  `cycle_param_type` int(11) DEFAULT NULL,
  `cycle_param_num` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `cycle_first_id` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `open_type` int(11) DEFAULT NULL,
  `open_server_date` int(11) DEFAULT NULL,
  `open_spencific_date` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `first_open_day_of_week` int(11) DEFAULT NULL,
  `duration_days` int(11) DEFAULT NULL,
  `cycle_days` int(11) DEFAULT NULL,
  `show_type` int(11) DEFAULT NULL,
  `text_image` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr1` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr2` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr3` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr4` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `param_arr5` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `is_end_act` int(11) DEFAULT NULL,
  `original_cycle_firstId` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `old_cycle_firstId` varchar(512) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `create_time` int(11) DEFAULT NULL,
  `end_time` int(11) DEFAULT NULL,
  `template_from_type` int(11) DEFAULT NULL,
  `refresh_time` int(11) DEFAULT NULL,
  PRIMARY KEY (`act_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of time_limit_template
-- ----------------------------

-- ----------------------------
-- Table structure for `transfer_channel`
-- ----------------------------
DROP TABLE IF EXISTS `transfer_channel`;
CREATE TABLE `transfer_channel` (
  `obj_type` tinyint(4) NOT NULL DEFAULT '0',
  `obj_id` bigint(11) NOT NULL DEFAULT '0',
  `ordinal` int(11) NOT NULL DEFAULT '0',
  `data` mediumtext,
  PRIMARY KEY (`obj_type`,`obj_id`,`ordinal`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of transfer_channel
-- ----------------------------

-- ----------------------------
-- Table structure for `world_boss`
-- ----------------------------
DROP TABLE IF EXISTS `world_boss`;
CREATE TABLE `world_boss` (
  `world_boss_npc_array_index` int(11) DEFAULT '0',
  `npc_respawn_point_template_id` int(11) NOT NULL,
  `is_alive` int(11) DEFAULT '0',
  `line_id` int(11) DEFAULT '0',
  `last_respawn_time` bigint(20) DEFAULT '0',
  `respawn_phase` int(11) DEFAULT '0',
  `killer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `first_killer_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `first_killed_time` int(11) DEFAULT NULL,
  `is_get_prize` int(11) DEFAULT NULL,
  PRIMARY KEY (`npc_respawn_point_template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of world_boss
-- ----------------------------

-- ----------------------------
-- Table structure for `ws_lucky_draw_record`
-- ----------------------------
DROP TABLE IF EXISTS `ws_lucky_draw_record`;
CREATE TABLE `ws_lucky_draw_record` (
  `info_guid` bigint(20) NOT NULL DEFAULT '0',
  `role_guid` bigint(11) DEFAULT '0',
  `role_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '',
  `item_template_id` int(11) DEFAULT NULL,
  `get_type` int(11) DEFAULT NULL,
  `record_type` int(11) DEFAULT NULL,
  `record_time` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`info_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Records of ws_lucky_draw_record
-- ----------------------------

-- ----------------------------
-- Procedure structure for `save_other_server_mail_record`
-- ----------------------------
DROP PROCEDURE IF EXISTS `save_other_server_mail_record`;
DELIMITER ;;
CREATE PROCEDURE `save_other_server_mail_record`(in in_source_server_id int, in in_target_server_id int,   in _sql mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `other_server_mail_record` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;
				SELECT COUNT(*) INTO exist_data_num FROM other_server_mail_record WHERE source_server_id=in_source_server_id AND target_server_id=in_target_server_id;
				IF exist_data_num > 0 THEN
						DELETE FROM other_server_mail_record WHERE source_server_id=in_source_server_id AND target_server_id=in_target_server_id;
				END IF;

				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_add_legion_mail_to_sql`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_add_legion_mail_to_sql`;
DELIMITER ;;
CREATE PROCEDURE `sp_add_legion_mail_to_sql`(in in_legion_guid bigint, in in_mail_id int)
BEGIN
	
	DECLARE data_index INT DEFAULT 0;
	DECLARE max_data_num INT DEFAULT 1;
	DECLARE exist_data_num INT DEFAULT 0;
	DECLARE temp_data BIGINT DEFAULT 0;

	SELECT COUNT(role_guid) FROM legion_member_info 
		WHERE legion_guid = in_legion_guid INTO max_data_num;
		
	START TRANSACTION;
		WHILE data_index < max_data_num DO
			
			SELECT role_guid FROM legion_member_info ORDER BY role_guid LIMIT data_index,1 INTO temp_data;

			SELECT COUNT(*) INTO exist_data_num FROM role_mail_by_id 
				WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;

			IF exist_data_num > 0 THEN
				DELETE FROM role_mail_by_id WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;
			END IF;

			INSERT INTO role_mail_by_id VALUES 
				(temp_data, UNIX_TIMESTAMP() * 1000, UNIX_TIMESTAMP(), in_mail_id);

			SET data_index = data_index + 1;

		END WHILE;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_info_clear`()
BEGIN
	delete from `attack_city_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_info_load`()
BEGIN
	SELECT * FROM `attack_city_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_info_save`(IN `_legion_guid` bigint,in in_sql_cmd_str mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
						SELECT COUNT(*) INTO exist_data_num FROM attack_city_info WHERE `_legion_guid` = legion_guid;
						IF exist_data_num > 0 THEN
								DELETE FROM attack_city_info WHERE `_legion_guid` = legion_guid;
						END IF;
            SET @sql_cmd = concat('INSERT INTO `attack_city_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_rank_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_rank_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_rank_info_clear`()
BEGIN
	#Routine body goes here...
	delete from `attack_city_rank_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_rank_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_rank_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_rank_info_load`()
BEGIN
	#Routine body goes here...
	SELECT * FROM `attack_city_rank_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_attack_city_rank_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_attack_city_rank_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_attack_city_rank_info_save`(IN `_role_guid` bigint,in in_sql_cmd_str mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
						SELECT COUNT(*) INTO exist_data_num FROM attack_city_rank_info WHERE `_role_guid` = role_guid;
						IF exist_data_num > 0 THEN
								DELETE FROM attack_city_rank_info WHERE `_role_guid` = role_guid;
						END IF;
            SET @sql_cmd = concat('INSERT INTO `attack_city_rank_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_add_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_add_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_add_item`(in `in_item_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `auction` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
	SELECT COUNT(*) INTO exist_data_num FROM auction WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM auction WHERE item_guid = `in_item_guid`; 
	END IF;

  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_add_record`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_add_record`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_add_record`(in `in_role_guid` bigint, in `in_trade_type` int,in `in_max_num` int, in _sql mediumtext)
BEGIN
	DECLARE _count integer default 0;
	DECLARE _min_deal_time BIGINT default 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `auction_record` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		IF length(_sql)>0 THEN		
			SELECT count(1) INTO _count FROM `auction_record` WHERE role_guid = in_role_guid AND trade_type = in_trade_type;
			IF _count >= in_max_num THEN
				
				SELECT deal_time INTO _min_deal_time  FROM `auction_record` WHERE role_guid  = in_role_guid  AND trade_type = in_trade_type ORDER BY deal_time DESC LIMIT in_max_num, 1;
				DELETE FROM auction_record WHERE trade_type = in_trade_type AND deal_time <= _min_deal_time;
				
				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;
			
			ELSE
				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

			END IF;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_add_share_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_add_share_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_add_share_list`(in `in_item_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `auction_share_list` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM auction_share_list WHERE item_guid = `in_item_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM auction_share_list WHERE  item_guid = `in_item_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_notice_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_notice_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_notice_add`(in `in_role_guid` bigint, in `in_item_guid`  bigint  , in `in_cur_time` int , in `in_template_id`  int , in `in_template_num`  int ,  in `in_money_type`  int, in `in_bid_money`  bigint, in `in_max_num` int)
BEGIN
	DECLARE _count integer default 0;
	DECLARE _min_create_time BIGINT default 0; 
  
  START TRANSACTION;

    IF (SELECT COUNT(*) FROM auction_bid_notice 
			WHERE item_guid = `in_item_guid` and role_guid = `in_role_guid`) <= 0 THEN
				INSERT INTO `auction_bid_notice` VALUES 
					(`in_role_guid`, `in_item_guid`, `in_cur_time`,`in_template_id`,`in_template_num`,`in_money_type`,`in_bid_money`, 0, `in_role_guid`, `in_cur_time`, 0);
    END IF;

    UPDATE `auction_bid_notice` set bid_money = in_bid_money, buy_guid = in_role_guid, update_time = in_cur_time, is_read = 0 where item_guid = in_item_guid;

		SELECT count(1) INTO _count FROM `auction_bid_notice` WHERE role_guid = in_role_guid;
		IF _count >= in_max_num THEN	
				SELECT create_time INTO _min_create_time  FROM `auction_bid_notice` WHERE role_guid  = in_role_guid  ORDER BY create_time DESC LIMIT in_max_num, 1;
				DELETE FROM auction_bid_notice WHERE role_guid  = in_role_guid AND create_time <= _min_create_time;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_notice_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_notice_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_notice_del`(in `in_item_guid`  bigint, in  `in_role_guid`  bigint,  in `in_bid_money`  bigint,  in `in_cur_time`  int)
BEGIN
  START TRANSACTION;
   UPDATE auction_bid_notice set bid_money = in_bid_money, buy_guid = in_role_guid,  update_time = in_cur_time, is_sell = 1, is_read = 0  where item_guid = in_item_guid;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_notice_read`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_notice_read`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_notice_read`(in `in_role_guid` bigint)
BEGIN
	##update auction_bid_notice set is_read = 1 WHERE role_guid = `in_role_guid`;
	update auction_bid_record set is_read = 1 WHERE role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_notice_red`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_notice_red`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_notice_red`(in `in_role_guid` bigint)
BEGIN
	select count(*) from auction_bid_record where role_guid = in_role_guid and is_read = 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_record_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_record_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_record_add`(in `in_role_guid` bigint,in `in_item_guid` bigint,in `in_deal_time` bigint)
BEGIN
	IF (SELECT COUNT(*) FROM auction_bid_record 
			WHERE item_guid = `in_item_guid` AND role_guid = `in_role_guid`) <= 0 THEN
				INSERT INTO `auction_bid_record` VALUES 
					(`in_role_guid`, `in_item_guid`, `in_deal_time`,0); 
	END IF;
  UPDATE `auction_bid_record` set  is_read = 0 where item_guid = in_item_guid; 
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_record_clear_old`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_record_clear_old`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_record_clear_old`(in `in_cur_time` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction_bid_record WHERE deal_time != 0 AND deal_time < `in_cur_time`;
	IF exist_data_num > 0 THEN
			DELETE FROM auction_bid_record WHERE deal_time != 0 AND deal_time < `in_cur_time`;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_record_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_record_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_record_del`(in `in_item_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction_bid_record WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM auction_bid_record WHERE item_guid = `in_item_guid`;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_replace_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_replace_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_replace_add`(in `in_role_guid` bigint,in `in_max_num` int, in _sql mediumtext)
BEGIN
	DECLARE _count integer default 0;
	DECLARE _min_create_time BIGINT default 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `auction_bid_replace` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		IF length(_sql)>0 THEN		
			SELECT count(1) INTO _count FROM `auction_bid_replace` WHERE role_guid = in_role_guid;
			IF _count >= in_max_num THEN
				
				SELECT create_time INTO _min_create_time  FROM `auction_bid_replace` WHERE role_guid  = in_role_guid  ORDER BY create_time DESC LIMIT in_max_num, 1;
				DELETE FROM auction_bid_replace WHERE role_guid  = in_role_guid AND create_time <= _min_create_time;
				
				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;
			
			ELSE
				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

			END IF;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_bid_replace_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_bid_replace_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_bid_replace_del`(in `in_item_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction_bid_replace WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM auction_bid_replace WHERE item_guid = `in_item_guid`;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_buy`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_buy`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_buy`(IN `in_buyer_guid` bigint,IN `in_role_guid` bigint,IN `in_item_guid` bigint)
BEGIN
	DECLARE t_error INTEGER DEFAULT 0;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;
	START TRANSACTION;
		INSERT INTO `role_item`(
        	`role_guid`,
		`item_guid`,
		`e_item_info_info_id`,
		`e_item_info_slot`,
		`e_item_info_container_type`,
		`e_item_info_stack_count`,
		`e_item_info_activate`,
		`e_item_info_is_first`,
		`e_item_info_upgrade_count`,
		`e_item_info_add_on`,
		`e_item_info_locked`,
		`e_item_info_wing_exp`,
		`e_item_info_wing_bless`,
		`e_item_info_random_property1`,
		`e_item_info_random_property2`,
		`e_item_info_random_property3`,
		`e_item_info_random_property4`,
		`e_item_info_random_property5`,
		`e_item_info_random_property6`,
		`e_item_info_succinct_property1`,
		`e_item_info_succinct_property2`,
		`e_item_info_succinct_property3`,
		`e_item_info_succinct_property4`,
		`e_item_info_succinct_property5`,
		`e_item_info_succinct_property6`,
		`e_item_info_showing_illusion_idex`,
            `e_item_info_put_in_bag_count`)
            SELECT 			
		`in_buyer_guid`,
		`item_guid`,
		`e_item_info_info_id`,
		`e_item_info_slot`,
		`e_item_info_container_type`,
		`e_item_info_stack_count`,
		`e_item_info_activate`,
		`e_item_info_is_first`,
		`e_item_info_upgrade_count`,
		`e_item_info_add_on`,
		`e_item_info_locked`,
		`e_item_info_wing_exp`,
		`e_item_info_wing_bless`,
		`e_item_info_random_property1`,
		`e_item_info_random_property2`,
		`e_item_info_random_property3`,
		`e_item_info_random_property4`,
		`e_item_info_random_property5`,
		`e_item_info_random_property6`,
		`e_item_info_succinct_property1`,
		`e_item_info_succinct_property2`,
		`e_item_info_succinct_property3`,
		`e_item_info_succinct_property4`,
		`e_item_info_succinct_property5`,
		`e_item_info_succinct_property6`,
		`e_item_info_showing_illusion_idex`,
		`e_item_info_put_in_bag_count` 
            FROM `auction` WHERE  role_guid = `in_role_guid` AND item_guid = `in_item_guid`;
  		DELETE FROM `auction` WHERE  role_guid = `in_role_guid` AND item_guid = `in_item_guid`;
	IF t_error = 1 THEN    
		ROLLBACK;    
	ELSE    
		COMMIT;    
	END IF;   
	SELECT t_error;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_check_wrold_sell`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_check_wrold_sell`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_check_wrold_sell`(IN `anction_type` int, IN `cur_time` bigint)
BEGIN
	#Routine body goes here...
	select COUNT(1) FROM auction where auction.trade_type = anction_type and auction.deal_time > cur_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_del`(in `in_item_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM `auction` WHERE item_guid = `in_item_guid`;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_del_batch`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_del_batch`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_del_batch`(in _sql mediumtext)
BEGIN
  -- DECLARE _del_sql mediumtext DEFAULT 'insert into `role_item` values ';
	DECLARE _del_sql mediumtext DEFAULT 'DELETE FROM auction WHERE item_guid IN ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_del_sql,'(',_sql,')',';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_del_no_buyer_old_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_del_no_buyer_old_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_del_no_buyer_old_item`(in `in_trade_type` int, in `in_cur_time` bigint, in `in_handle_num` int)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	DECLARE word_trade_type INT DEFAULT 2;

	SELECT COUNT(*) INTO exist_data_num FROM auction WHERE (trade_type = word_trade_type AND buyer_guid = 0 AND (deal_time > 0 AND deal_time < in_cur_time));
	IF exist_data_num > 0 THEN
			DELETE FROM `auction` WHERE (trade_type = word_trade_type AND buyer_guid = 0 AND (deal_time > 0 AND deal_time < in_cur_time));
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_del_share_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_del_share_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_del_share_list`(in `in_item_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction_share_list WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM auction_share_list WHERE item_guid = `in_item_guid`;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find`(in `in_item_guid` bigint)
BEGIN
	SELECT * FROM `auction` WHERE item_guid = `in_item_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_bid_notice`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_bid_notice`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_bid_notice`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM auction_bid_notice  WHERE role_guid = `in_role_guid` ORDER BY create_time DESC;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_bid_record_failure`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_bid_record_failure`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_bid_record_failure`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM auction_bid_replace  WHERE role_guid = `in_role_guid` ORDER BY create_time DESC;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_other_sell_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_other_sell_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_other_sell_info`(in `in_sell_money_type` int, in `in_item_template_id` int)
BEGIN
	-- SELECT * FROM `auction` WHERE e_item_info_info_id = `in_item_template_id` AND sell_money_type =  `in_sell_money_type` AND trade_type =  0 ORDER BY sell_total_money LIMIT 10;
	SELECT * FROM `auction` WHERE e_item_info_info_id = `in_item_template_id` AND trade_type =  0 ORDER BY sell_total_money LIMIT 10;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_read_bid_record_failure`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_read_bid_record_failure`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_read_bid_record_failure`(in `in_role_guid` bigint)
BEGIN
	update auction_bid_replace set is_read = 1 WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_self_bid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_self_bid`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_self_bid`(in `in_role_guid` bigint, in `in_trade_type` int, in `in_cur_time` bigint, in `in_start_index` int, in `in_show_num` int)
BEGIN
	-- SELECT * FROM auction_bid_record
	-- 	WHERE (role_guid = `in_role_guid` 
	-- 					AND  trade_type = in_trade_type 
	-- 					AND deal_time > in_cur_time)
	-- 	ORDER BY deal_time DESC LIMIT in_start_index,in_show_num;
  if in_trade_type = 3 THEN
	  SELECT * FROM auction WHERE (deal_time > in_cur_time
						   AND item_guid IN 
							(SELECT item_guid FROM auction_bid_record WHERE role_guid = `in_role_guid`)
						)
		ORDER BY deal_time DESC LIMIT in_start_index,in_show_num;
  ELSE
    SELECT * FROM auction WHERE (trade_type = in_trade_type 
  					AND deal_time > in_cur_time
						AND item_guid IN 
							(SELECT item_guid FROM auction_bid_record WHERE role_guid = `in_role_guid`)
						)
		ORDER BY deal_time DESC LIMIT in_start_index,in_show_num;
  end if;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_self_bid_num`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_self_bid_num`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_self_bid_num`(in `in_role_guid` bigint, in `in_trade_type` int, in `in_cur_time` bigint, in `in_start_index` int, in `in_show_num` int)
BEGIN
    if in_trade_type = 3 THEN
		 SELECT COUNT(*) FROM auction WHERE ( deal_time > in_cur_time
						 AND item_guid IN 
							(SELECT item_guid FROM auction_bid_record WHERE role_guid = `in_role_guid`)
						);
     ELSE
     	 SELECT COUNT(*) FROM auction WHERE (trade_type = in_trade_type 
  					AND deal_time > in_cur_time
						AND item_guid IN 
							(SELECT item_guid FROM auction_bid_record WHERE role_guid = `in_role_guid`)
						);
     end if;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_share_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_share_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_share_list`(in `in_item_guid` bigint)
BEGIN
	SELECT * FROM auction_share_list WHERE item_guid = `in_item_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_trade_record`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_trade_record`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_trade_record`(in `in_role_guid` bigint,in `in_trade_type` int,in `in_start_index` int,in `in_show_num` int)
BEGIN
	SELECT * FROM `auction_record` 
		WHERE role_guid = `in_role_guid` AND  trade_type = in_trade_type 
		ORDER BY deal_time DESC LIMIT in_start_index,in_show_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_find_trade_record_num`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_find_trade_record_num`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_find_trade_record_num`(in `in_role_guid` bigint,in `in_trade_type` int,in `in_start_index` int,in `in_show_num` int)
BEGIN
	SELECT COUNT(*) FROM `auction_record` 
		WHERE role_guid = `in_role_guid` AND  trade_type = in_trade_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_get_old_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_get_old_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_get_old_item`(in `in_trade_type` int, in `in_cur_time` bigint, in `in_handle_num` int)
BEGIN

	DECLARE word_trade_type INT DEFAULT 2;
	DECLARE legion_trade_type INT DEFAULT 1;

	SELECT * FROM `auction` WHERE (trade_type = word_trade_type OR trade_type = legion_trade_type) AND (deal_time > 0 AND deal_time < in_cur_time) LIMIT 0,in_handle_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_get_time_out`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_get_time_out`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_get_time_out`(IN `in_cur_time` bigint, in in_max int)
BEGIN
  SELECT * from auction where trade_type IN(0,2) and is_show = 1 and (sell_time < in_cur_time or deal_time = 0)  LIMIT in_max;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_load_self_selling`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_load_self_selling`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_load_self_selling`(in `in_role_guid` bigint, in `num_limit` int)
BEGIN
	SELECT * FROM `auction` WHERE seller_guid = `in_role_guid` LIMIT `num_limit`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_req_auction_data_num`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_req_auction_data_num`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_req_auction_data_num`(in `in_main_type` int, in `in_sub_type` int, in `in_item_color` int, in `in_quality_level` int, in `in_single_money_sort_mode` int, in `in_total_money_sort_mode` int, in `in_is_show_gold` bool, in `in_is_show_diamond` bool, in `in_start_index` int, in `in_show_num_per_page` int, in `in_trade_type` int,in `in_career_type` int, in `in_cur_time` bigint, in `in_seller_guid` bigint, in `in_self_guid` bigint, in_key_word mediumtext)
BEGIN
	DECLARE _gold_money_type integer default 1;
	DECLARE _diamond_money_type integer default 3;

		IF in_single_money_sort_mode!=0 THEN
				IF in_single_money_sort_mode > 0 THEN
					SELECT COUNT(*) FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level) 
					AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid);
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type));
				ELSE
					SELECT COUNT(*) FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level)
          AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid);
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type));
				END IF;
		ELSEIF in_total_money_sort_mode!=0 THEN
				IF in_total_money_sort_mode > 0 THEN
					SELECT COUNT(*) FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level)
          AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid);
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type));
				ELSE
					SELECT COUNT(*) FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level)
          AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid);
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type));
				END IF;
		ELSE
				-- sort by sell time, desc
				SELECT COUNT(*) FROM auction WHERE
				(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
				AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
				AND (in_item_color = 0 OR item_color = in_item_color)
				AND (in_quality_level = 0 OR quality_level = in_quality_level)
        AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
				AND (in_trade_type = trade_type)
				AND (deal_time = 0 OR in_cur_time < deal_time)
				AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
				AND (in_self_guid != seller_guid)
				AND (item_name like in_key_word )
        AND (is_show = 1)
        AND (show_guid = 0 or show_guid = in_self_guid);
				-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
				-- 			OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type));
		END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_req_auction_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_req_auction_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_req_auction_list`(in `in_main_type` int, in `in_sub_type` int, in `in_item_color` int, in `in_quality_level` int, in `in_single_money_sort_mode` int, in `in_total_money_sort_mode` int, in `in_is_show_gold` bool, in `in_is_show_diamond` bool, in `in_start_index` int, in `in_show_num_per_page` int, in `in_trade_type` int,in `in_career_type` int, in `in_cur_time` bigint, in `in_seller_guid` bigint, in `in_self_guid` bigint, in_key_word mediumtext)
BEGIN
	DECLARE _gold_money_type integer default 1;
	DECLARE _diamond_money_type integer default 3;
	DECLARE _single_sort_name MEDIUMTEXT;
	
	IF in_trade_type = 0 THEN
		SET _single_sort_name = 'sell_single_money';
	ELSE
		SET _single_sort_name = 'cur_bid_money';
	END IF;

		IF in_single_money_sort_mode!=0 THEN
				IF in_single_money_sort_mode > 0 THEN
						IF in_trade_type = 0 THEN
								SELECT * FROM auction WHERE
									(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
									AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
									AND (in_item_color = 0 OR item_color = in_item_color)
									AND (in_quality_level = 0 OR quality_level = in_quality_level)
                  AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
									AND (in_trade_type = trade_type)
									AND (deal_time = 0 OR in_cur_time < deal_time)
									AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
									AND (in_self_guid != seller_guid)
									AND (item_name like in_key_word )
                  AND (is_show = 1)
                  AND (show_guid = 0 or show_guid = in_self_guid)
									-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
									-- 	OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
									ORDER BY sell_single_money,sell_time,item_guid LIMIT in_start_index,in_show_num_per_page;
						ELSE
								SELECT * FROM auction WHERE
									(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
									AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
									AND (in_item_color = 0 OR item_color = in_item_color)
									AND (in_quality_level = 0 OR quality_level = in_quality_level)
                  AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
									AND (in_trade_type = trade_type)
									AND (deal_time = 0 OR in_cur_time < deal_time)
									AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
									AND (in_self_guid != seller_guid)
									AND (item_name like in_key_word )
                  AND (is_show = 1)
                  AND (show_guid = 0 or show_guid = in_self_guid)
									-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
									-- 	OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
									ORDER BY cur_bid_money,sell_time,item_guid LIMIT in_start_index,in_show_num_per_page;
						END IF;
				ELSE
						IF in_trade_type = 0 THEN
								SELECT * FROM auction WHERE
									(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
									AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
									AND (in_item_color = 0 OR item_color = in_item_color)
									AND (in_quality_level = 0 OR quality_level = in_quality_level)
                  AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
									AND (in_trade_type = trade_type)
									AND (deal_time = 0 OR in_cur_time < deal_time)
									AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
									AND (in_self_guid != seller_guid)
									AND (item_name like in_key_word )
                  AND (is_show = 1)
                  AND (show_guid = 0 or show_guid = in_self_guid)
									-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
									-- 	OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
									ORDER BY sell_single_money DESC,sell_time,item_guid DESC LIMIT in_start_index,in_show_num_per_page;
						ELSE
								SELECT * FROM auction WHERE
									(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
									AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
									AND (in_item_color = 0 OR item_color = in_item_color)
									AND (in_quality_level = 0 OR quality_level = in_quality_level)
                  AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
									AND (in_trade_type = trade_type)
									AND (deal_time = 0 OR in_cur_time < deal_time)
									AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
									AND (in_self_guid != seller_guid)
									AND (item_name like in_key_word )
                  AND (is_show = 1)
                  AND (show_guid = 0 or show_guid = in_self_guid)
									-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
									-- 	OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
									ORDER BY cur_bid_money DESC,sell_time,item_guid DESC LIMIT in_start_index,in_show_num_per_page;
						END IF;
				END IF;
		ELSEIF in_total_money_sort_mode!=0 THEN
				IF in_total_money_sort_mode > 0 THEN
					SELECT * FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level)
          AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid)
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
					ORDER BY sell_total_money,sell_time,item_guid LIMIT in_start_index,in_show_num_per_page;
				ELSE
					SELECT * FROM auction WHERE
					(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type = 99 and auction_tab >=1 and  auction_tab <=4))
					AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
					AND (in_item_color = 0 OR item_color = in_item_color)
					AND (in_quality_level = 0 OR quality_level = in_quality_level)
          AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
					AND (in_trade_type = trade_type)
					AND (deal_time = 0 OR in_cur_time < deal_time)
					AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
					AND (in_self_guid != seller_guid)
					AND (item_name like in_key_word )
          AND (is_show = 1)
          AND (show_guid = 0 or show_guid = in_self_guid)
					-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
					-- 		OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
					ORDER BY sell_total_money DESC,sell_time,item_guid DESC LIMIT in_start_index,in_show_num_per_page;
				END IF;
		ELSE
				-- sort by sell time, desc
				SELECT * FROM auction WHERE
				(in_main_type = 0 OR auction_tab = in_main_type or (in_main_type =99 and auction_tab >=1 and  auction_tab <=4))
				AND (in_sub_type = 0 OR auction_sub_tab = in_sub_type)
				AND (in_item_color = 0 OR item_color = in_item_color)
				AND (in_quality_level = 0 OR quality_level = in_quality_level)
        AND (in_career_type = 0 OR class_limit = 0 OR class_limit = in_career_type)
				AND (in_trade_type = trade_type)
				AND (deal_time = 0 OR in_cur_time < deal_time)
				AND (in_seller_guid = 0 OR in_seller_guid = seller_guid)
				AND (in_self_guid != seller_guid)
				AND (item_name like in_key_word )
        AND (is_show = 1)
        AND (show_guid = 0 or show_guid = in_self_guid)
				-- AND ((in_is_show_gold = TRUE AND sell_money_type = _gold_money_type)
				-- 			OR (in_is_show_diamond = TRUE AND sell_money_type = _diamond_money_type))
				ORDER BY is_sky_order desc, sell_time DESC,item_guid DESC LIMIT in_start_index,in_show_num_per_page;
				-- SELECT FOUND_ROWS();
		END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_req_legion_selling_num`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_req_legion_selling_num`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_req_legion_selling_num`(in `in_legion_guid` bigint, in `in_cur_time` bigint)
BEGIN
		SELECT COUNT(*) FROM auction WHERE (seller_guid = in_legion_guid AND deal_time > in_cur_time);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_show`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_show`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_show`(IN `in_cur_time` bigint,IN `in_end_time` bigint,IN `in_item_guid` bigint)
BEGIN
	DECLARE t_error INTEGER DEFAULT 0;
  DECLARE res INTEGER DEFAULT 0;
	DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;
	START TRANSACTION;
  UPDATE `auction` set sell_time = in_cur_time, deal_time = in_end_time, is_show = 1 WHERE item_guid =`in_item_guid`;	
	IF t_error = 1 THEN    
		ROLLBACK;    
	ELSE    
		COMMIT;    
	END IF;
  if t_error = 0 THEN 
		SELECT count(*) into res from `auction`  WHERE item_guid = `in_item_guid`; 
  end if;
  SELECT res;   

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_auction_show_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_auction_show_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_auction_show_all`(IN `in_cur_time` bigint,IN `in_end_time` bigint)
BEGIN 
  update auction set sell_time = in_cur_time, deal_time = in_end_time, is_show = 1 where trade_type = 0 and is_show = 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_best_record_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_best_record_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_best_record_info_load_all`()
BEGIN
	SELECT * FROM `best_record_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_best_record_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_best_record_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_best_record_info_save`(in `in_map_id` int,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `best_record_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
	
		SELECT COUNT(*) INTO exist_data_num FROM best_record_info WHERE `map_template_id` = in_map_id;
		IF exist_data_num > 0 THEN
			DELETE FROM best_record_info WHERE `map_template_id` = in_map_id;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_big_player_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_big_player_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_big_player_load`()
BEGIN
	SELECT * FROM `big_player`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_big_player_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_big_player_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_big_player_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `big_player` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		DELETE FROM big_player;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_change_role_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_change_role_name`;
DELIMITER ;;
CREATE PROCEDURE `sp_change_role_name`(IN `in_role_guid` bigint,IN `in_role_name` mediumtext)
BEGIN
	DECLARE original_name VARCHAR(41);
	#Routine body goes here...
	START TRANSACTION;
		SELECT role_name INTO original_name FROM role_info WHERE role_guid = in_role_guid;
		UPDATE role_info SET role_name = in_role_name where role_guid = in_role_guid;

		UPDATE gain_treasure_record SET first_role_name = in_role_name WHERE first_role_name = original_name;
		UPDATE gain_treasure_record SET second_role_name = in_role_name WHERE second_role_name = original_name;
		UPDATE gain_treasure_record SET third_role_name = in_role_name WHERE third_role_name = original_name;
		UPDATE gain_treasure_record SET fourth_role_name = in_role_name WHERE fourth_role_name = original_name;
		UPDATE gain_treasure_record SET fifth_role_name = in_role_name WHERE fifth_role_name = original_name;
		#auction
		UPDATE auction SET seller_name = in_role_name WHERE seller_guid = in_role_guid;
		UPDATE auction_record SET seller_name = in_role_name WHERE seller_guid = in_role_guid;
		UPDATE auction_record SET buyer_name = in_role_name	WHERE buyer_guid = in_role_guid;
		
		UPDATE best_record_info SET best_record_role_name = in_role_name WHERE role_guid = in_role_guid;
		UPDATE cross_ladder_ranking SET role_name = in_role_name	WHERE role_guid = in_role_guid;
		UPDATE cross_role_info SET role_name = in_role_name WHERE role_guid = in_role_guid;
		#legion
		UPDATE legion_applicant SET role_name = in_role_name WHERE role_guid = in_role_guid;
		UPDATE legion_member_info SET role_name = in_role_name WHERE role_guid = in_role_guid;
		UPDATE legion_boss_record SET killer_name = in_role_name WHERE killer_name = original_name;
		#arena
		UPDATE ranking_arena SET role_name = in_role_name WHERE role_guid = in_role_guid;
		UPDATE ranking_arena_challenge_log SET role_name = in_role_name WHERE role_guid = in_role_guid;
		#marry
    UPDATE role_marry SET role_name = in_role_name WHERE role_guid = in_role_guid;
		UPDATE role_marry SET mate_name = in_role_name WHERE mate_guid = in_role_guid;
		#ranking
		UPDATE role_ranking SET role_name = in_role_name WHERE role_guid = in_role_guid;
		#relation
		UPDATE role_relation SET relation_name = in_role_name WHERE relation_guid = in_role_guid;
		
		UPDATE world_boss SET killer_name = in_role_name WHERE killer_name = original_name;
		UPDATE world_boss SET first_killed_time = in_role_name WHERE first_killer_name = original_name;
		UPDATE ws_lucky_draw_record SET role_name = in_role_name WHERE role_guid = in_role_guid;
	COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_check_role_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_check_role_name`;
DELIMITER ;;
CREATE PROCEDURE `sp_check_role_name`(IN `database_names` mediumtext,IN `game_count` int,IN `in_role_name` mediumtext)
BEGIN

	DECLARE _ret integer DEFAULT 1;
	DECLARE _count integer DEFAULT 0;
	DECLARE sql_front VARCHAR(500) DEFAULT 'SELECT count(role_name) into @name_count FROM ';
	DECLARE sql_back VARCHAR(500) DEFAULT '.role_info WHERE role_name = ';
	
	START TRANSACTION;
		
		set @index_loop = 1;
		WHILE @index_loop <= game_count DO
			
			SET @base_name = SUBSTRING_INDEX(SUBSTRING_INDEX(database_names,',',@index_loop),',',-1);
			
			SET @sql = concat(sql_front,@base_name,sql_back, '\'',in_role_name, '\';');

			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
			
			set _count= @name_count;
			
			IF _count > 0 THEN
				set _ret = 2;
				set @index_loop = game_count + 1;
			END IF;

			set @index_loop = @index_loop + 1;
		END WHILE;
 COMMIT;
	SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_client_login`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_client_login`;
DELIMITER ;;
CREATE PROCEDURE `sp_client_login`(_account varchar(100),_password varchar(100),_create_ip varchar(16),_device_type varchar(100),_login_type int, _login_type_plus int, _server_id int,in_main_db varchar(30),_is_new_account int)
begin
	declare t_account_id int default 0;
	DECLARE _close_login INT default 0;
	DECLARE _login_lock INT default 0;
	DECLARE _jewel_num INT default 0;
	DECLARE _is_gm INT default 0;
	DECLARE _is_radio_host INT default 0;
	DECLARE _is_create INT default 0;
	DECLARE _online_duration INT default 0;
	DECLARE _online_time INT default 0;
	DECLARE _month_recharge_num INT default 0;
	DECLARE _month_recharge_time INT default 0;


	declare exit handler for sqlexception
	begin
		set t_account_id = 0;
		select t_account_id, _jewel_num,_is_radio_host,_is_create,_online_duration,_online_time,_month_recharge_num,_month_recharge_time;
	end;

	if _is_new_account > 0 and not exists (select account from `account` where `account` = _account) then
			insert into `account`(account,password,login_type,login_type_plus,create_time,platform_account,is_white_account,create_ip,device_type,online_duration,online_time,month_recharge_num,month_recharge_time) 
						values(_account ,_password , _login_type , _login_type_plus , now(),"1",0,_create_ip,_device_type,0,0,0,0);
			SET _is_create = 1;
	END if;

	select id,online_duration,online_time,month_recharge_num,month_recharge_time into t_account_id,_online_duration,_online_time,_month_recharge_num,_month_recharge_time from `account` where `account`.`account`=_account and `password` = _password;

	select t_account_id, _jewel_num,_is_radio_host,_is_create,_online_duration,_online_time,_month_recharge_num,_month_recharge_time;
	commit;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_client_login_main_db_check`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_client_login_main_db_check`;
DELIMITER ;;
CREATE PROCEDURE `sp_client_login_main_db_check`(_account varchar(100),_password varchar(100),_create_ip varchar(16),_device_type varchar(100),_login_type int, _login_type_plus int, _server_id int,in_main_db varchar(30),_is_new_account int)
begin
	declare t_account_id int default 0;
	DECLARE _close_login INT default 0;
	DECLARE _login_lock INT default 0;
	DECLARE _jewel_num INT default 0;
	DECLARE _is_gm INT default 0;
	DECLARE _is_radio_host INT default 0;
	DECLARE _is_create INT default 0;
	DECLARE _online_duration INT default 0;
	DECLARE _online_time INT default 0;
	DECLARE _month_recharge_num INT default 0;
	DECLARE _month_recharge_time INT default 0;





	start transaction;
		SELECT is_close_login INTO _close_login FROM server_info WHERE big_group_id = _server_id div 10000 AND server_group_id =_server_id mod 10000;
		if _is_new_account > 0 and not exists (select account from `account` where `account` = _account) then
			if ISNULL(_close_login) OR _close_login = 0 THEN
				set _is_create = 1;
			ELSE
				set t_account_id = -2;
			END if;
		end if;	
	 
	IF t_account_id >= 0 THEN
		SELECT is_login,jewel_num,is_gm,is_radio_host INTO _login_lock,_jewel_num,_is_gm,_is_radio_host FROM gm_role_lock WHERE role_mark = _account;
		IF ISNULL(_login_lock) OR _login_lock = 0 THEN
			SET	t_account_id = 1;
		ELSE
			SET t_account_id = -1;
		END IF;
		IF _is_gm > 0 AND _jewel_num = 0 THEN
			SET _jewel_num = 1;
		END IF;
	END IF;
	
	if _is_create > 0 THEN
			select id,online_duration,online_time,month_recharge_num,month_recharge_time into t_account_id,_online_duration,_online_time,_month_recharge_num,_month_recharge_time from `account` where `account`.`account`=_account and `password` = _password;
	end if;

	select t_account_id, _jewel_num,_is_radio_host,_is_create,_online_duration,_online_time,_month_recharge_num,_month_recharge_time;
	commit;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cloud_shop_clear_player_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cloud_shop_clear_player_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cloud_shop_clear_player_info`(in in_activity_type int)
BEGIN
	#Routine body goes here...
	DELETE FROM cloud_shop_role_record WHERE `activity_type` = in_activity_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cloud_shop_role_record_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cloud_shop_role_record_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_cloud_shop_role_record_load`(in in_row int, 
  in in_load_max int)
BEGIN
  select * from cloud_shop_role_record LIMIT in_row, in_load_max;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cloud_shop_role_record_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cloud_shop_role_record_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_cloud_shop_role_record_save`(in in_role_id bigint, in in_activity_type int, in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `cloud_shop_role_record` values ';
  DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
    SELECT COUNT(*) INTO exist_data_num FROM `cloud_shop_role_record` WHERE `player_uid` = in_role_id AND `activity_type` = in_activity_type;
    IF exist_data_num > 0 THEN
        DELETE FROM `cloud_shop_role_record` WHERE `player_uid` = in_role_id AND `activity_type` = in_activity_type;
    END IF;

    IF length(_sql)>0 THEN
      SET @sql = concat(_insert_sql,_sql,';');
      PREPARE stmt FROM @sql;
      EXECUTE stmt;
      DEALLOCATE PREPARE stmt;
    END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cloud_shop_unfinished_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cloud_shop_unfinished_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_cloud_shop_unfinished_list`()
BEGIN
  DECLARE today_number INT;
  SET today_number = CAST(FROM_UNIXTIME(UNIX_TIMESTAMP(), '%Y%m%d') AS UNSIGNED integer);
  SELECT * FROM `cloud_shop_server_info` WHERE `flag_send_award` = 0 AND activity_date < today_number ORDER BY `activity_date` DESC;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_create_role_init_guide_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_create_role_init_guide_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_create_role_init_guide_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_guide_record` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		
		SELECT COUNT(*) INTO exist_data_num FROM role_guide_record WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
			DELETE FROM role_guide_record WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_get_new_ranking`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_get_new_ranking`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_get_new_ranking`()
BEGIN
	DELETE FROM cross_ladder_ranking;
	alter table cross_ladder_ranking auto_increment = 1;
	INSERT INTO cross_ladder_ranking(role_guid, role_name, template_id, role_gs, ladder_score) 
		SELECT role_guid, role_name, template_id, role_gs, ladder_score FROM `cross_role_info` 
		ORDER BY ladder_score DESC;
	SELECT * FROM cross_ladder_ranking LIMIT 10;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_get_role_by_score`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_get_role_by_score`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_get_role_by_score`(IN `_low_score` int,IN `_high_score` int)
BEGIN
	SELECT * FROM cross_ladder_ranking WHERE ladder_score = (_high_score-_low_score) * RAND() + _low_score LIMIT 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_add_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_add_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_add_role_info`(IN `_role_guid` bigint,IN `_role_name` varchar(100),IN `_server_name` varchar(100),IN `_gs_value` bigint,IN `_template_id` int,IN `_role_gs` int,IN `_vip_level` int,IN `_server_id` int,IN `_ladder_score` int,IN `_total_join_num` int,IN `_total_win_num` int,IN `_last_win_num` int,IN `_ticket_num` int,IN `_can_buy_num` int,IN `_history_score` int,IN `_last_score` int,IN `_season_reward` int,IN `_cur_season_join_num` int,IN `_last_season_join_num` int)
BEGIN
	insert into cross_ladder_role_info value(_role_guid, _role_name, _server_name, _gs_value, _template_id, _role_gs, _vip_level, _server_id, _ladder_score, _total_join_num, _total_win_num, _last_win_num, _ticket_num,_can_buy_num, _history_score,_last_score,_season_reward,_cur_season_join_num,_last_season_join_num );
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_load_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_load_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_load_role_info`(IN `_offset` int)
BEGIN
	select * from cross_ladder_role_info limit 500 offset _offset;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_refresh_season_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_refresh_season_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_refresh_season_info`(IN `_role_guid` bigint,IN `_ladder_score` int,IN `_history_score` int,IN `_last_score` int,IN `_season_reward` int,IN `_cur_season_join_num` int,IN `_last_season_join_num` int)
BEGIN
	UPDATE cross_ladder_role_info SET ladder_score = _ladder_score, history_score = _history_score, last_score = _last_score, season_reward = _season_reward, cur_season_join_num = _cur_season_join_num, last_season_join_num = _last_season_join_num WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_refresh_ticket_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_refresh_ticket_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_refresh_ticket_info`(IN `_ticket_num` int,IN `_can_buy_num` int)
BEGIN
	UPDATE cross_ladder_role_info SET ticket_num = _ticket_num , can_buy_num = _can_buy_num;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_save_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_save_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_save_role_info`(IN `_role_guid` bigint,IN`_win_result` int,IN `_ladder_score` int)
BEGIN
		UPDATE cross_role_info SET ladder_score 	= _ladder_score 			WHERE role_guid = _role_guid;
		UPDATE cross_role_info SET total_join_num	= total_join_num + 1 	WHERE role_guid = _role_guid;
		IF _win_result = 0 OR _win_result = 1 THEN
			UPDATE cross_role_info SET last_win_num = 0 WHERE role_guid = _role_guid;
		ELSE
			UPDATE cross_role_info SET total_win_num 	= total_win_num + 1 	WHERE role_guid = _role_guid;
			UPDATE cross_role_info SET last_win_num		= last_win_num 	+ 1 	WHERE role_guid = _role_guid;
		END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_update_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_update_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_update_info`(IN `_role_guid` bigint,IN `_total_win_num` int,IN `_last_win_num` int,IN `_ladder_score` int,IN `_ticket_num` int,IN `_history_score` int,IN `_cur_season_join_num` int)
BEGIN
	UPDATE cross_ladder_role_info SET total_join_num = total_join_num + 1, total_win_num = _total_win_num, last_win_num = _last_win_num, ladder_score = _ladder_score, ticket_num = _ticket_num, history_score = _history_score, cur_season_join_num = _cur_season_join_num where role_guid = _role_guid;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_update_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_update_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_update_role_info`(IN `_role_guid` bigint,IN `_role_name` varchar(100),IN `_server_name` varchar(100),IN `_role_gs`bigint,IN `_vip_level` int,IN `_server_id` int,IN `_template_id` int)
BEGIN
	UPDATE cross_ladder_role_info SET role_name = _role_name, server_name = _server_name, gs_value = _role_gs, vip_level = _vip_level, server_id = _server_id , template_id = _template_id where role_guid = _role_guid;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_ladder_update_ticket_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_ladder_update_ticket_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_ladder_update_ticket_info`(IN `_role_guid` bigint,IN `_ticket_num` int,IN `_can_buy_num` int)
BEGIN
	UPDATE cross_ladder_role_info SET ticket_num = _ticket_num , can_buy_num = _can_buy_num where role_guid = _role_guid;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_load_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_load_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_load_role_info`(IN `_role_guid` bigint,IN `_role_name` varchar(100),IN `_template_id` int,IN `_role_gs` int)
BEGIN
	declare _war_num integer default NULL;
	declare _win_num integer default NULL;
	declare _last_win integer default NULL;
	declare _join_num integer default NULL;
	declare _ladder_score integer default NULL;
	declare _ranking_num integer default NULL;
	declare _refresh_time integer default NULL;
	SELECT war_num, win_num, last_win, join_num, ladder_score, refresh_time INTO _war_num,_win_num,_last_win,_join_num,_ladder_score,_refresh_time FROM cross_role_info WHERE role_guid = _role_guid;
	IF _war_num is NULL THEN
		SET _war_num = 0;
		SET _win_num = 0;
		SET _last_win = 0;
		SET _join_num = 0;
		SET _ladder_score = 0;
		SET _ranking_num = 0;
		INSERT INTO cross_role_info VALUES(_role_guid, _role_name, _template_id, _role_gs, _war_num, _win_num, _last_win, _join_num, _ladder_score, 0);
	ELSE
		SELECT ranking_id INTO _ranking_num FROM cross_ladder_ranking WHERE role_guid = _role_guid;
		IF _ranking_num is NULL THEN
			SET _ranking_num = 0;			
		END IF;
	END IF;
	SELECT _role_guid, _role_name, _template_id,_role_gs,_war_num,_win_num,_last_win,_join_num,_ladder_score,_ranking_num,_refresh_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_save_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_save_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_save_role_info`(IN `_role_guid` bigint,IN `_ladder_score` int,IN`_target_name_1` varchar(100),IN`_target_name_2` varchar(100),IN`_target_name_3` varchar(100),IN`_win_result` int,IN`_cross_honor`int)
BEGIN
		UPDATE cross_role_info SET ladder_score = ladder_score + _ladder_score WHERE role_guid = _role_guid;
		UPDATE cross_role_info SET war_num = war_num + 1 WHERE role_guid = _role_guid;
		UPDATE cross_role_info SET join_num = join_num + 1 WHERE role_guid = _role_guid AND join_num < 5;
		IF _win_result = 0 OR _win_result = 1 THEN
			UPDATE cross_role_info SET last_win = 0 WHERE role_guid = _role_guid;
		ELSE
			UPDATE cross_role_info SET win_num = win_num + 1 WHERE role_guid = _role_guid;
			UPDATE cross_role_info SET last_win = last_win + 1 WHERE role_guid = _role_guid;
		END IF;
		INSERT INTO cross_role_war(role_guid, target_name_1, target_name_2, target_name_3, war_result, cross_honor, ladder_score, war_time) 
			VALUES(_role_guid, _target_name_1, _target_name_2, _target_name_3, _win_result, _cross_honor, _ladder_score, unix_timestamp());
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_save_role_info_honor_refresh`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_save_role_info_honor_refresh`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_save_role_info_honor_refresh`(IN `_role_guid` bigint,IN `_refresh_time` int)
BEGIN
		UPDATE cross_role_info SET refresh_time = _refresh_time , join_num = 0 WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_harry_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_harry_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_harry_info_load_all`()
BEGIN
	SELECT * FROM `cross_server_harry_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_harry_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_harry_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_harry_save`(in `in_server_id` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `cross_server_harry_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM cross_server_harry_info WHERE server_id = `in_server_id` ;
		IF exist_data_num > 0 THEN
		DELETE FROM cross_server_harry_info WHERE server_id = `in_server_id` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_overload_city_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_overload_city_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_overload_city_clear`()
BEGIN
	DELETE FROM `cross_server_overload_city_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_overload_city_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_overload_city_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_overload_city_load`()
BEGIN
	SELECT * FROM `cross_server_overload_city_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_overload_city_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_overload_city_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_overload_city_save`(in `in_rank_idex` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `cross_server_overload_city_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM cross_server_overload_city_info WHERE legion_rank = `in_rank_idex`  ;
		IF exist_data_num > 0 THEN
		DELETE FROM cross_server_overload_city_info WHERE legion_rank = `in_rank_idex` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_pk_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_pk_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_pk_info_clear`()
BEGIN
	delete from `cross_server_pk_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_pk_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_pk_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_pk_info_load_all`()
BEGIN
	SELECT * FROM `cross_server_pk_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_server_pk_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_server_pk_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_server_pk_save`(in `in_war_idex` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `cross_server_pk_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM cross_server_pk_info WHERE war_idex = `in_war_idex` ;
		IF exist_data_num > 0 THEN
		DELETE FROM cross_server_pk_info WHERE war_idex = `in_war_idex` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_war_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_war_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_war_info_load`(IN `_role_guid` bigint)
BEGIN

	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM cross_role_war WHERE role_guid = _role_guid AND war_time < unix_timestamp() - 259200;
	IF exist_data_num > 0 THEN
			DELETE FROM cross_role_war WHERE role_guid = _role_guid AND war_time < unix_timestamp() - 259200;
	END IF;

	SELECT target_name_1, target_name_2, target_name_3, ladder_score, war_result, cross_honor FROM cross_role_war WHERE role_guid = _role_guid ORDER BY id DESC LIMIT 50;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_world_boss_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_world_boss_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_world_boss_info_load`()
BEGIN
	SELECT * FROM `cross_world_boss`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_cross_world_boss_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_cross_world_boss_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_cross_world_boss_save`(in `in_boss_tem_id` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `cross_world_boss` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM cross_world_boss WHERE boss_template_id = `in_boss_tem_id`;
		IF exist_data_num > 0 THEN
		DELETE FROM cross_world_boss WHERE boss_template_id = `in_boss_tem_id` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_delete_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_delete_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_delete_role_info`(IN `_role_guid` bigint)
BEGIN
	DECLARE _role_guid BIGINT DEFAULT 0;
	DECLARE _i INT DEFAULT 0;
	WHILE _i < 1000 DO
		SET _i = _i +1;
		SELECT role_guid INTO _role_guid FROM role_info WHERE account like 'robot%' LIMIT 1;
	-- DELETE FROM activity_record_info WHERE role_guid = _role_guid;
	-- DELETE FROM auction WHERE role_guid = _role_guid;
		DELETE FROM best_record_info WHERE role_guid = _role_guid;
	-- DELETE FROM big_player WHERE role_guid = _role_guid;
		DELETE FROM cross_ladder_ranking WHERE role_guid = _role_guid;
		DELETE FROM cross_role_info WHERE role_guid = _role_guid;
		DELETE FROM cross_role_war WHERE role_guid = _role_guid;
		DELETE FROM legion_applicant WHERE role_guid = _role_guid;
	-- DELETE FROM legion_boss_record WHERE role_guid = _role_guid;
	-- DELETE FROM legion_city_bid_info WHERE role_guid = _role_guid;
	-- DELETE FROM legion_city_info WHERE role_guid = _role_guid;
		DELETE FROM legion_get_boss_award_log WHERE role_guid = _role_guid;
	-- DELETE FROM legion_info WHERE role_guid = _role_guid;
		DELETE FROM legion_member_info WHERE role_guid = _role_guid;
	-- DELETE FROM mail_globel_mail WHERE role_guid = _role_guid;
	-- DELETE FROM mail_globel_mail_item WHERE role_guid = _role_guid;
		DELETE FROM ranking_arena;
		DELETE FROM ranking_arena_challenge_log;
		DELETE FROM ranking_role_worship;
	-- DELETE FROM red_package_info WHERE role_guid = _role_guid;
	-- DELETE FROM red_package_receiver_info WHERE role_guid = _role_guid;
		DELETE FROM role_achievement WHERE role_guid = _role_guid;
		DELETE FROM role_active_degree WHERE role_guid = _role_guid;
		DELETE FROM role_att WHERE role_guid = _role_guid;
		DELETE FROM role_belief WHERE role_guid = _role_guid;
		DELETE FROM role_buff WHERE role_guid = _role_guid;
		DELETE FROM role_convert WHERE role_guid = _role_guid;
		DELETE FROM role_demons_tower WHERE role_guid = _role_guid;
		DELETE FROM role_element_heart WHERE role_guid = _role_guid;
		DELETE FROM role_event WHERE role_guid = _role_guid;
		DELETE FROM role_fight_att WHERE role_guid = _role_guid;
		DELETE FROM role_first_time_do WHERE role_guid = _role_guid;
		DELETE FROM role_goods WHERE role_guid = _role_guid;
		DELETE FROM role_guide_record WHERE role_guid = _role_guid;
		DELETE FROM role_harry_record WHERE role_guid = _role_guid;
		DELETE FROM role_history_highest_record WHERE role_guid = _role_guid;
		DELETE FROM role_info WHERE role_guid = _role_guid;
		DELETE FROM role_item WHERE role_guid = _role_guid;
		DELETE FROM role_mail WHERE role_guid = _role_guid;
		DELETE FROM role_mail_item WHERE role_guid = _role_guid;
		DELETE FROM role_mission WHERE role_guid = _role_guid;
		DELETE FROM role_money WHERE role_guid = _role_guid;
		DELETE FROM role_pokedex WHERE role_guid = _role_guid;
		DELETE FROM role_protect WHERE role_guid = _role_guid;
		DELETE FROM role_ranking WHERE role_guid = _role_guid;
		DELETE FROM role_relation WHERE role_guid = _role_guid;
		DELETE FROM role_service_goal WHERE role_guid = _role_guid;
		DELETE FROM role_show_info WHERE role_guid = _role_guid;
		DELETE FROM role_skill WHERE role_guid = _role_guid;
		DELETE FROM role_special_name WHERE role_guid = _role_guid;
		DELETE FROM role_spirit WHERE role_guid = _role_guid;
		DELETE FROM role_talent WHERE role_guid = _role_guid;
		DELETE FROM role_time WHERE role_guid = _role_guid;
		DELETE FROM role_title WHERE role_guid = _role_guid;
		DELETE FROM role_welfare WHERE role_guid = _role_guid;
		DELETE FROM role_welfare_random_get WHERE role_guid = _role_guid;
		DELETE FROM role_worship_target WHERE role_guid = _role_guid;
		DELETE FROM role_yesterday_must_do_remain WHERE role_guid = _role_guid;
	-- DELETE FROM transfer_channel WHERE role_guid = _role_guid;
	-- DELETE FROM world_boss WHERE role_guid = _role_guid;
		DELETE FROM ws_lucky_draw_record WHERE role_guid = _role_guid;
	END WHILE;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_del_marry_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_del_marry_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_del_marry_info`(in `in_role_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM role_marry WHERE in_role_guid = role_guid;
	IF exist_data_num > 0 THEN
			DELETE  FROM role_marry WHERE in_role_guid = role_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_dp_ping`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_dp_ping`;
DELIMITER ;;
CREATE PROCEDURE `sp_dp_ping`()
begin
    declare server_id int unsigned default 0;
    select dp_app_id into server_id from dp_list where conn_id = connection_id() limit 1;
    if server_id > 0 then
        delete from dp_ping_info where dp_app_id = server_id;
        insert ignore into dp_ping_info values(server_id, now());
    end if;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_add_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_add_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_add_role_info`(IN `_role_guid` bigint,IN `_role_name` varchar(100),IN `_server_name` varchar(100),IN `_gs_value` bigint,IN `_template_id` int,IN `_vip_level` int,IN `_role_level` int,IN `_server_id` int,IN `_score_num` int,IN `_cur_season` int,IN `_last_season` int,IN `_total_join_num` int,IN `_total_win_num` int,IN `_last_win_num` int,IN `_ticket_num` int,IN `_can_buy_num` int,IN `_have_buy_num` int,IN `_history_score` int,IN `_cur_season_join_num` int,IN `_last_score` int,IN `_season_reward` int,IN `_last_season_join_num` int,IN `_mission1` int,IN `_mission2` int,IN `_mission3` int,IN `_mission4` int,IN `_mission1_have_num` int,IN `_mission2_have_num` int,IN `_mission3_have_num` int,IN `_mission4_have_num` int)
BEGIN
	insert into element_war_info value(_role_guid, _role_name, _server_name, _gs_value, _template_id, _vip_level, _role_level, _server_id, _score_num, _cur_season, _last_season, _total_join_num, _total_win_num, _last_win_num, _ticket_num,_can_buy_num, _have_buy_num, _history_score,_cur_season_join_num,_last_score,_season_reward,_last_season_join_num,
	_mission1, _mission2, _mission3, _mission4, _mission1_have_num, _mission2_have_num, _mission3_have_num, _mission4_have_num );
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_clear_mission`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_clear_mission`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_clear_mission`()
BEGIN
	UPDATE element_war_info SET mission1 = 0, mission2 = 0, mission3 = 0, mission4 = 0, mission1_have_num = 0, mission2_have_num = 0, mission3_have_num = 0, mission4_have_num = 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_load_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_load_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_load_role_info`(IN `_offset` int)
BEGIN
	select * from element_war_info limit 500 offset _offset;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_refresh_mission`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_refresh_mission`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_refresh_mission`(IN `_role_guid` bigint,IN `_mission1` int,IN `_mission2` int,IN `_mission3` int,IN `_mission4` int,IN `_mission1_have_num` int,IN `_mission2_have_num` int,IN `_mission3_have_num` int,IN `_mission4_have_num` int)
BEGIN
	UPDATE element_war_info SET mission1 = _mission1, mission2 = _mission2, mission3 = _mission3, mission4 = _mission4, mission1_have_num = _mission1_have_num, mission2_have_num = _mission2_have_num, mission3_have_num = _mission3_have_num, mission4_have_num = _mission4_have_num WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_refresh_season_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_refresh_season_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_refresh_season_info`(IN `_role_guid` bigint,IN `_score_num` int,IN `_history_score` int,IN `_last_score` int,IN `_last_season` int,IN `_season_reward` int,IN `_cur_season_join_num` int,IN `_last_season_join_num` int)
BEGIN
	UPDATE element_war_info SET score_num = _score_num, history_score = _history_score, last_score = _last_score, last_season = _last_season, season_reward = _season_reward, cur_season_join_num = _cur_season_join_num, last_season_join_num = _last_season_join_num WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_refresh_ticket_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_refresh_ticket_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_refresh_ticket_info`(IN `_ticket_num` int,IN `_have_buy_num` int)
BEGIN
	UPDATE element_war_info SET ticket_num = _ticket_num , have_buy_num = _have_buy_num;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_update_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_update_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_update_info`(IN `_role_guid` bigint,IN `_total_win_num` int,IN `_last_win_num` int,IN `_score_num` int,IN `_ticket_num` int,IN `_history_score` int,IN `_cur_season_join_num` int,IN `_have_buy_num` int,IN `_total_join_num` int)
BEGIN
	UPDATE element_war_info SET total_join_num = _total_join_num, total_win_num = _total_win_num, have_buy_num = _have_buy_num, last_win_num = _last_win_num, score_num = _score_num, ticket_num = _ticket_num, history_score = _history_score, cur_season_join_num = _cur_season_join_num where role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_element_war_update_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_element_war_update_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_element_war_update_role_info`(IN `_role_guid` bigint,IN `_role_name` varchar(100),IN `_server_name` varchar(100),IN `_role_gs`bigint,IN `_vip_level` int,IN `_role_level` int,IN `_server_id` int,IN `_template_id` int,IN `_can_buy_num` int)
BEGIN
	UPDATE element_war_info SET role_name = _role_name, server_name = _server_name, gs_value = _role_gs, vip_level = _vip_level, role_level = _role_level, server_id = _server_id, template_id = _template_id, can_buy_num = _can_buy_num WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_fight_ranking_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_fight_ranking_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_fight_ranking_load`()
BEGIN
	DELETE FROM ranking_gs;
	alter table ranking_gs auto_increment = 1;
	INSERT INTO ranking_gs(guid, fight,role_name) SELECT role_guid,ERankingInfo_fight_value, role_name FROM `role_ranking` ORDER BY  ERankingInfo_fight_value DESC,ERankingInfo_fight_time ASC;
	SELECT * FROM ranking_gs LIMIT 100;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gain_treasure_record_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gain_treasure_record_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_gain_treasure_record_info_load_all`(in `in_boss_spawn_point_template_id` int)
BEGIN
	SELECT * FROM `gain_treasure_record` WHERE boss_spawn_point_template_id = `in_boss_spawn_point_template_id` ORDER BY killed_stamp ASC;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gain_treasure_record_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gain_treasure_record_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_gain_treasure_record_info_save`(in `in_boss_spawn_point_template_id` int, in `del_stamp` int, in `save_stamp` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `gain_treasure_record` values ';
	DECLARE exist_data_num_del INT DEFAULT 0;
	DECLARE exist_data_num_save INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num_del FROM gain_treasure_record WHERE boss_spawn_point_template_id = `in_boss_spawn_point_template_id` AND del_stamp = `killed_stamp`;
		IF exist_data_num_del > 0 THEN
		DELETE FROM gain_treasure_record WHERE boss_spawn_point_template_id = `in_boss_spawn_point_template_id` AND del_stamp = `killed_stamp`;
		END IF;

		SELECT COUNT(*) INTO exist_data_num_save FROM gain_treasure_record WHERE boss_spawn_point_template_id = `in_boss_spawn_point_template_id` AND save_stamp = `killed_stamp`;
		IF exist_data_num_save > 0 THEN
		DELETE FROM gain_treasure_record WHERE boss_spawn_point_template_id = `in_boss_spawn_point_template_id` AND save_stamp = `killed_stamp`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_cur_databases_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_cur_databases_name`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_cur_databases_name`(in in_game_name varchar(100))
BEGIN
	SELECT * FROM databases_name;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_databases_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_databases_name`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_databases_name`(in in_game_name varchar(100))
BEGIN
	DELETE FROM databases_name;
	INSERT INTO databases_name select SCHEMA_NAME, right(SCHEMA_NAME, 5) from information_schema.`SCHEMATA` WHERE SCHEMA_NAME LIKE in_game_name;
	SELECT * FROM databases_name;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_group_name_by_role_guid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_group_name_by_role_guid`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_group_name_by_role_guid`(IN `_role_guid` bigint)
BEGIN
	SELECT legion_member_info.e_legion_member_info_job_title, legion_info.legion_name FROM legion_info,legion_member_info WHERE legion_member_info.role_guid = _role_guid AND legion_member_info.legion_guid=legion_info.legion_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_base_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_base_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_base_info`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM role_info WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_buff_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_buff_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_buff_info`(IN `in_role_guid` bigint)
BEGIN
		SELECT * FROM `role_buff` WHERE role_guid = `in_role_guid` AND 
				(EBuffInfo_series = 221100 			-- Upgrade Weapon
					OR EBuffInfo_series = 231101 	-- Upgrade Equip
					OR EBuffInfo_series = 240018	-- Addon Weapon
					OR EBuffInfo_series = 240019	-- Addon Equip
					OR EBuffInfo_series = 990289	-- Rune 1
					OR EBuffInfo_series = 990290	-- Rune 2
					OR EBuffInfo_series = 990291	-- Rune 3
					OR EBuffInfo_series = 990292	-- Rune 4
					OR EBuffInfo_series = 990293	-- Rune 5
					OR EBuffInfo_series = 990294	-- Rune 6
					OR EBuffInfo_series = 990295	-- Rune 7
					OR EBuffInfo_series = 990296	-- Rune 8
					OR EBuffInfo_series = 990297	-- Rune 9
					OR EBuffInfo_series = 990298	-- Rune 10
					OR EBuffInfo_series = 990299	-- Rune 11
					OR EBuffInfo_series = 990300	-- Rune 12
					OR EBuffInfo_series = 990301	-- Rune 13
					OR EBuffInfo_series = 990302	-- Rune 14
					OR EBuffInfo_series = 990303	-- Rune 15
					OR EBuffInfo_series = 990304	-- Rune 16
					OR EBuffInfo_series = 990306	-- Succinct Weapon
					OR EBuffInfo_series = 990307	-- Succinct Equip
					OR EBuffInfo_series = 130010	-- City Master Foot
					OR EBuffInfo_series = 990606	-- Berserker Fashion Weapon
					OR EBuffInfo_series = 990607	-- Caster Fashion Weapon
					OR EBuffInfo_series = 990608	-- Rider Fashiion Weapon
					OR EBuffInfo_series = 990609	-- Assassin Fashion Weapon
					OR EBuffInfo_series = 992005
					OR EBuffInfo_series = 240020
					OR EBuffInfo_series = 240024
					OR EBuffInfo_series = 240025
					OR EBuffInfo_series = 240026
					OR EBuffInfo_series = 990636
					OR EBuffInfo_series = 990637
					OR EBuffInfo_series = 990638
					OR EBuffInfo_series = 992005
					OR EBuffInfo_series = 990654
					OR EBuffInfo_series = 990658
					OR EBuffInfo_series = 992150

);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_equiping_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_equiping_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_equiping_info`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM role_item WHERE role_guid = _role_guid AND (e_item_info_container_type = 1 OR e_item_info_container_type = 12 OR e_item_info_container_type = 24);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_fake_item_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_fake_item_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_fake_item_info`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM role_item WHERE role_guid = _role_guid AND (e_item_info_container_type = 1 OR e_item_info_container_type = 12 OR e_item_info_container_type = 18 OR e_item_info_container_type = 10);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_mount_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_mount_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_mount_info`(in `in_role_guid` bigint, in `in_mount_id` int,in `in_mount_illusion_id` int)
BEGIN
		-- SELECT role_item.* FROM role_info,role_item WHERE role_info.role_guid = _role_guid AND role_info.role_guid = role_item.role_guid AND role_info.e_role_info_mount_showd_template_id = role_item.e_item_info_info_id;
		SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND (e_item_info_info_id = in_mount_id OR e_item_info_info_id = in_mount_illusion_id);

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_special_name_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_special_name_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_special_name_info`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM `role_special_name` WHERE  role_guid = `_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_spirit_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_spirit_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_spirit_info`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM role_item WHERE role_guid = _role_guid AND role_item.e_item_info_container_type = 19 AND role_item.e_item_info_activate > 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_other_player_wing_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_other_player_wing_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_other_player_wing_info`(in `in_role_guid` bigint, in `in_wing_id` int,in `in_wing_illusion_id` int)
BEGIN
	-- SELECT role_item.* FROM role_info,role_item WHERE role_info.role_guid = _role_guid AND role_info.role_guid = role_item.role_guid AND role_info.e_role_info_wing_showd_template_id = role_item.e_item_info_info_id;
	SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND (e_item_info_info_id = in_wing_id OR e_item_info_info_id = in_wing_illusion_id);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_person_information`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_person_information`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_person_information`(in in_role_guid bigint)
BEGIN
	SELECT * FROM role_person_information WHERE role_guid = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_get_role_competition`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_get_role_competition`;
DELIMITER ;;
CREATE PROCEDURE `sp_get_role_competition`(in in_role_guid bigint, in in_type int)
BEGIN
	SELECT * FROM role_competition WHERE role_guid = in_role_guid AND have_type = in_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_change_account`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_change_account`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_change_account`(IN `change_name` varchar(100),IN `target_name` varchar(100))
BEGIN
	DECLARE _change_account VARCHAR(100);
	DECLARE _target_account VARCHAR(100);
	declare _res integer default 0;
	
	SELECT account INTO _change_account FROM `role_info` WHERE role_name = change_name;
	SELECT account INTO _target_account FROM `role_info` WHERE role_name = target_name;
	IF ISNULL(_change_account) OR ISNULL(_target_account) THEN
			SET _res = 1;
	ELSE
			UPDATE role_info SET account = _target_account WHERE role_name = change_name;
	END IF;
	SELECT _res;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_del_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_del_role`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_del_role`(IN `_role_guid` bigint,IN `_is_del` int)
BEGIN
		UPDATE role_info SET e_role_info_del_time = _is_del WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_get_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_get_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_get_role_info`(in `in_role_mark` varchar(100),in `in_get_type` int)
BEGIN
	IF `in_get_type`  = 0 THEN
		SELECT * FROM role_info WHERE account = in_role_mark AND (e_role_info_del_time > unix_timestamp() - 86400 OR e_role_info_del_time = 0);
	ELSEIF `in_get_type`  = 1 THEN
		SELECT * FROM role_info WHERE role_guid = in_role_mark AND (e_role_info_del_time > unix_timestamp() - 86400 OR e_role_info_del_time = 0);
	ELSE
		SELECT * FROM role_info WHERE role_name = in_role_mark AND (e_role_info_del_time > unix_timestamp() - 86400 OR e_role_info_del_time = 0);
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_get_role_right`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_get_role_right`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_get_role_right`(IN `_account` varchar(100),IN `_role_guid` bigint,IN `_role_name` varchar(30))
BEGIN
	DECLARE _login_lock INT;
	DECLARE _send_chat INT;
	SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = _account;
	IF ISNULL(_login_lock) THEN
		SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = _role_name;
	END IF;
	IF ISNULL(_login_lock) THEN
		SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = _role_guid;
	END IF;
	IF ISNULL(_login_lock) THEN
		SET _login_lock = 0;
		SET _send_chat = 0;
	END IF;
	SELECT _login_lock, _send_chat;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_set_role_right`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_set_role_right`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_set_role_right`(IN `_role_mark` varchar(100),IN `_mark_type` int,IN `_mark_value` int)
BEGIN
	IF _mark_value > 0 THEN
		IF _mark_type = 0 THEN
			INSERT INTO gm_role_lock (role_mark, is_login) VALUES(_role_mark, 1) ON DUPLICATE KEY UPDATE is_login = 1;
		ELSEIF _mark_type = 1 THEN
			INSERT INTO gm_role_lock (role_mark, is_send_chat) VALUES(_role_mark, 1) ON DUPLICATE KEY UPDATE is_send_chat = _mark_value;
		ELSEIF _mark_type = 2 THEN
			INSERT INTO gm_role_lock (role_mark, is_radio_host) VALUES(_role_mark, _mark_value) ON DUPLICATE KEY UPDATE is_radio_host = _mark_value;
		ELSEIF _mark_type = 3 THEN
			INSERT INTO gm_role_lock (role_mark, jewel_num) VALUES(_role_mark, _mark_value) ON DUPLICATE KEY UPDATE jewel_num = _mark_value;
		END IF;
	ELSE
		IF _mark_type = 0 THEN
			UPDATE gm_role_lock SET is_login = 0 WHERE role_mark = _role_mark;
		ELSEIF _mark_type = 1 THEN
			UPDATE gm_role_lock SET is_send_chat = 0 WHERE role_mark = _role_mark;
		ELSEIF _mark_type = 2 THEN
			UPDATE gm_role_lock SET is_radio_host = 0 WHERE role_mark = _role_mark;
		ELSEIF _mark_type = 3 THEN
			UPDATE gm_role_lock SET jewel_num = 0 WHERE role_mark = _role_mark;
		END IF;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_gm_stop_login`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_gm_stop_login`;
DELIMITER ;;
CREATE PROCEDURE `sp_gm_stop_login`(IN `_stop_login` int,IN `_server_id` int)
BEGIN
	UPDATE server_info SET is_close_login = _stop_login WHERE big_group_id = _server_id div 10000 AND server_group_id =_server_id mod 10000;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_harry_info_server_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_harry_info_server_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_harry_info_server_delete`(in in_server_id int)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM cross_server_harry_info WHERE server_id = in_server_id;
	IF exist_data_num > 0 THEN
    DELETE FROM `cross_server_harry_info` WHERE server_id = in_server_id;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_last_server_list_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_last_server_list_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_last_server_list_load`(IN `in_server_id` bigint)
BEGIN
	SELECT * FROM last_server_list WHERE server_id = `in_server_id` ;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_last_server_list_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_last_server_list_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_last_server_list_save`(in `in_server_id` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `last_server_list` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM last_server_list WHERE server_id = `in_server_id`  ;
		IF exist_data_num > 0 THEN
		DELETE FROM last_server_list WHERE server_id = `in_server_id` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_applicant_del_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_applicant_del_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_applicant_del_one`(in `in_role_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_applicant WHERE role_guid = `in_role_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM legion_applicant WHERE role_guid = `in_role_guid`;
	END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_applicant_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_applicant_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_applicant_load_all`(in in_legion_guid bigint)
BEGIN
	SELECT * FROM `legion_applicant` WHERE `legion_guid` = in_legion_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_applicant_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_applicant_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_applicant_save`(
    in in_legion_guid bigint,
    in in_role_guid bigint,
    in in_sql_cmd_str mediumtext
)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
            DELETE FROM `legion_applicant` WHERE `legion_guid` = in_legion_guid AND `role_guid` = in_role_guid;
            SET @sql_cmd = concat('INSERT INTO `legion_applicant` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_bonus_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_bonus_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_bonus_info_clear`(in in_legion_guid bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_bonus_info WHERE `legion_guid` = in_legion_guid;
	IF exist_data_num > 0 THEN
			DELETE FROM legion_bonus_info WHERE `legion_guid` = in_legion_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_bonus_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_bonus_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_bonus_info_load_all`()
BEGIN
	SELECT * FROM `legion_bonus_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_bonus_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_bonus_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_bonus_info_save`(in in_legion_guid bigint,in in_mission_id int,in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
	START TRANSACTION;
		IF length(in_sql_cmd_str) > 0 THEN
			SELECT COUNT(*) INTO exist_data_num FROM legion_bonus_info WHERE `legion_guid` = in_legion_guid AND `mission_id` = in_mission_id;
			IF exist_data_num > 0 THEN
				DELETE FROM legion_bonus_info WHERE `legion_guid` = in_legion_guid AND `mission_id` = in_mission_id;
			END IF;
				
			SET @sql_cmd = concat('INSERT INTO `legion_bonus_info` VALUES ', in_sql_cmd_str, ';');
			PREPARE stmt FROM @sql_cmd;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
	COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_boss_record_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_boss_record_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_boss_record_clear`(in in_legion_guid bigint)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_boss_record WHERE `legion_guid` = in_legion_guid;
	IF exist_data_num > 0 THEN
			DELETE FROM legion_boss_record WHERE `legion_guid` = in_legion_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_boss_record_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_boss_record_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_boss_record_load_all`(in in_legion_guid bigint)
BEGIN
    SELECT * FROM `legion_boss_record` WHERE `legion_guid` = in_legion_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_boss_record_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_boss_record_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_boss_record_save`(
    in in_legion_guid bigint,
    in in_boss_map_id bigint,
    in in_sql_cmd_str mediumtext
)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
            DELETE FROM `legion_boss_record` WHERE `legion_guid` = in_legion_guid AND `ELegionBossRecord_template_id` = in_boss_map_id;
            SET @sql_cmd = concat('INSERT INTO `legion_boss_record` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_bid_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_bid_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_bid_info_clear`()
BEGIN
	delete from `legion_city_bid_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_bid_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_bid_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_bid_info_load_all`()
BEGIN
	SELECT * FROM `legion_city_bid_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_bid_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_bid_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_bid_info_save`(in in_territory_id int,
 in in_index_in_terr int, in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
						SELECT COUNT(*) INTO exist_data_num FROM legion_city_bid_info WHERE `territory_id` = in_territory_id AND `index_in_terr` = in_index_in_terr;
						IF exist_data_num > 0 THEN
								DELETE FROM legion_city_bid_info WHERE `territory_id` = in_territory_id AND `index_in_terr` = in_index_in_terr;
						END IF;
            SET @sql_cmd = concat('INSERT INTO `legion_city_bid_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_info_load`()
BEGIN
	SELECT * FROM `legion_city_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_info_save`(in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
            delete from `legion_city_info`;
            SET @sql_cmd = concat('INSERT INTO `legion_city_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_war_member_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_war_member_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_war_member_info_clear`(IN `_type` bigint)
BEGIN
	DELETE FROM `legion_city_war_member_info` WHERE use_type = `_type` ;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_war_member_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_war_member_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_war_member_info_load_all`(in in_legion_guid bigint)
BEGIN
	SELECT * FROM `legion_city_war_member_info` WHERE `legion_guid` = in_legion_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_city_war_member_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_city_war_member_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_city_war_member_info_save`(in in_role_guid bigint,
 in in_use_type bigint,in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN

						SELECT COUNT(*) INTO exist_data_num FROM legion_city_war_member_info WHERE `role_guid` = in_role_guid AND use_type =  in_use_type;
						IF exist_data_num > 0 THEN
								DELETE FROM legion_city_war_member_info WHERE `role_guid` = in_role_guid AND use_type =  in_use_type;
						END IF;

            SET @sql_cmd = concat('INSERT INTO `legion_city_war_member_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_event_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_event_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_event_clear`(in in_legion_guid bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_event WHERE `legion_guid` = in_legion_guid;
	IF exist_data_num > 0 THEN
			DELETE FROM legion_event WHERE `legion_guid` = in_legion_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_event_del_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_event_del_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_event_del_one`(in in_legion_guid bigint, in in_event_idx int)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_event WHERE `legion_guid` = in_legion_guid AND `event_idx` = in_event_idx;
	IF exist_data_num > 0 THEN
		DELETE FROM `legion_event` WHERE `legion_guid` = in_legion_guid AND `event_idx` = in_event_idx;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_event_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_event_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_event_load_all`(in in_legion_guid bigint)
BEGIN
	SELECT * FROM `legion_event` WHERE `legion_guid` = in_legion_guid ORDER BY happen_time;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_event_save_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_event_save_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_event_save_one`(in in_legion_guid bigint,
 in in_event_idx int, in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN

						SELECT COUNT(*) INTO exist_data_num FROM legion_event WHERE `legion_guid` = in_legion_guid AND `event_idx` = in_event_idx;
						IF exist_data_num > 0 THEN
							DELETE FROM legion_event WHERE `legion_guid` = in_legion_guid AND `event_idx` = in_event_idx;
						END IF;
				
            SET @sql_cmd = concat('INSERT INTO `legion_event` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_get_boss_award_log_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_get_boss_award_log_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_get_boss_award_log_load_all`(in in_role_guid bigint)
BEGIN
    SELECT * FROM `legion_get_boss_award_log` WHERE `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_get_boss_award_log_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_get_boss_award_log_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_get_boss_award_log_save`(in in_role_guid bigint,
    in in_boss_map_id int)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
		
		SELECT COUNT(*) INTO exist_data_num FROM legion_get_boss_award_log WHERE `role_guid` = in_role_guid AND `boss_map_id` = in_boss_map_id;
		IF exist_data_num > 0 THEN
				DELETE FROM legion_get_boss_award_log WHERE `role_guid` = in_role_guid AND `boss_map_id` = in_boss_map_id;
		END IF;
	
		SET @sql_cmd = concat('INSERT INTO `legion_get_boss_award_log` VALUES (', in_role_guid, ',', in_boss_map_id, ');');
		PREPARE stmt FROM @sql_cmd;
		EXECUTE stmt;
		DEALLOCATE PREPARE stmt;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_info_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_info_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_info_delete`(in in_legion_guid bigint)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_info WHERE legion_guid = in_legion_guid;
	IF exist_data_num > 0 THEN
    DELETE FROM `legion_info` WHERE legion_guid = in_legion_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_info_load_all`()
BEGIN
	SELECT * FROM `legion_info`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_info_save`(in in_legion_guid bigint,
    in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
             SET @sql_cmd = concat('INSERT INTO `legion_info` VALUES ', in_sql_cmd_str, 'ON DUPLICATE KEY UPDATE 
						`legion_name` = VALUES(legion_name),
						`announcement` = VALUES(announcement),
						`gs_value` = VALUES(gs_value),
						`ELegionInfo_construction_level_main` = VALUES(ELegionInfo_construction_level_main),
						`ELegionInfo_construction_level_1` = VALUES(ELegionInfo_construction_level_1),
						`ELegionInfo_construction_level_2` = VALUES(ELegionInfo_construction_level_2),
						`ELegionInfo_construction_level_3` = VALUES(ELegionInfo_construction_level_3),
						`ELegionInfo_asset_money` = VALUES(ELegionInfo_asset_money),
						`ELegionInfo_asset_item_1` = VALUES(ELegionInfo_asset_item_1),
						`ELegionInfo_asset_item_2` = VALUES(ELegionInfo_asset_item_2),
						`ELegionInfo_asset_item_3` = VALUES(ELegionInfo_asset_item_3),
						`ELegionInfo_asset_item_4` = VALUES(ELegionInfo_asset_item_4),
 						`ELegionInfo_fighting_power` = VALUES(ELegionInfo_fighting_power),
						`ELegionInfo_auto_accept_new_member` = VALUES(ELegionInfo_auto_accept_new_member),
						`ELegionInfo_last_refresh_time` = VALUES(ELegionInfo_last_refresh_time),
						`ELegionInfo_glory_glow` = VALUES(ELegionInfo_glory_glow),
						`ELegionInfo_act_score_raid` = VALUES(ELegionInfo_act_score_raid),
						`ELegionInfo_act_score_bonfire` = VALUES(ELegionInfo_act_score_bonfire),
						`ELegionInfo_legion_welfare_num` = VALUES(ELegionInfo_legion_welfare_num);');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_member_info_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_member_info_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_member_info_delete`(in in_role_guid bigint)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM legion_member_info WHERE role_guid = in_role_guid;
	IF exist_data_num > 0 THEN
    DELETE FROM `legion_member_info` WHERE role_guid = in_role_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_member_info_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_member_info_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_member_info_load_all`(in in_legion_guid bigint)
BEGIN
	SELECT * FROM `legion_member_info` WHERE `legion_guid` = in_legion_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_member_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_member_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_member_info_save`(in in_role_guid bigint,
    in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN

						SELECT COUNT(*) INTO exist_data_num FROM legion_member_info WHERE `role_guid` = in_role_guid;
						IF exist_data_num > 0 THEN
								DELETE FROM legion_member_info WHERE `role_guid` = in_role_guid;
						END IF;

            SET @sql_cmd = concat('INSERT INTO `legion_member_info` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_clear`(IN `in_legion_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;

	DECLARE t_error INTEGER DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;

	START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM legion_warehouse WHERE legion_guid = in_legion_guid;
		IF exist_data_num > 0 THEN
			DELETE FROM `legion_warehouse` WHERE legion_guid = in_legion_guid;
			DELETE FROM `legion_warehouse_log` WHERE legion_guid = in_legion_guid;
		END IF;

		IF t_error = 1 THEN    
      ROLLBACK;    
    ELSE    
      COMMIT;    
    END IF;   
    SELECT t_error;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_destroy`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_destroy`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_destroy`(IN `_sql` mediumtext,IN `_log_sql` mediumtext)
BEGIN
	DECLARE _del_sql mediumtext DEFAULT 'DELETE FROM legion_warehouse WHERE item_guid IN ';
	DECLARE t_error INTEGER DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;

	START TRANSACTION;
    IF length(_sql)>0 THEN
			SET @sql = concat(_del_sql,'(',_sql,')',';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;

		IF length(_log_sql)>0 THEN
			SET @sql = _log_sql;
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;

    IF t_error = 1 THEN    
      ROLLBACK;    
    ELSE    
      COMMIT;    
    END IF;   
    SELECT t_error;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_load_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_load_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_load_all`(IN `in_legion_guid` bigint)
BEGIN
  SELECT * from legion_warehouse WHERE legion_guid = `in_legion_guid` LIMIT 100;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_load_log`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_load_log`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_load_log`(IN `in_legion_guid` bigint)
BEGIN
	SELECT * from legion_warehouse_log WHERE legion_guid = `in_legion_guid`
  ORDER BY add_time DESC limit 20;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_log`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_log`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_log`(IN `_log_sql` mediumtext)
BEGIN
	DECLARE _insert_log_sql mediumtext DEFAULT 'insert into `legion_warehouse_log` values ';

  DECLARE t_error INTEGER DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;
  START TRANSACTION;
  
		IF length(_log_sql)>0 THEN
			SET @sql = concat(_insert_log_sql,`_log_sql`,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
    
    
  IF t_error = 1 THEN    
    ROLLBACK;    
  ELSE    
    COMMIT;    
  END IF;   
  SELECT t_error;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_role_get_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_role_get_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_role_get_item`(IN `in_item_guid` bigint,IN `in_legion_guid` bigint,IN `in_role_guid` bigint,IN `_sql` mediumtext,IN `_log_sql` mediumtext)
BEGIN
  DECLARE exist_data_num1 INT DEFAULT 0;
	DECLARE exist_data_num2 INT DEFAULT 0;
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_item` values ';
  DECLARE _insert_log_sql mediumtext DEFAULT 'insert into `legion_warehouse_log` values ';

	DECLARE t_error INTEGER DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num2 FROM legion_warehouse WHERE item_guid = `in_item_guid` AND legion_guid = `in_legion_guid`;
		IF exist_data_num2 > 0 THEN
			DELETE from legion_warehouse WHERE item_guid = `in_item_guid` AND legion_guid = `in_legion_guid`;
		END IF;

		IF length(_log_sql)>0 THEN
			SET @sql = concat(_insert_log_sql,`_log_sql`,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;

    IF t_error = 1 THEN    
      ROLLBACK;    
    ELSE    
      COMMIT;    
    END IF;   
    SELECT t_error;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_legion_warehouse_store_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_legion_warehouse_store_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_legion_warehouse_store_item`(IN `in_item_guid` bigint,IN `in_legion_guid` bigint,IN `_sql` mediumtext)
BEGIN
  DECLARE exist_legion_data_num INT DEFAULT 0;
	DECLARE exist_item_data_num INT DEFAULT 0;
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `legion_warehouse` values ';

  DECLARE t_error INTEGER DEFAULT 0;
  DECLARE CONTINUE HANDLER FOR SQLEXCEPTION SET t_error=1;
  START TRANSACTION;

    SELECT COUNT(*) INTO exist_legion_data_num FROM `legion_warehouse` WHERE item_guid = `in_item_guid`;
    IF exist_legion_data_num > 0 THEN
        DELETE FROM `legion_warehouse` WHERE item_guid = `in_item_guid`;
    END IF;

		SELECT COUNT(*) INTO exist_item_data_num FROM `role_item` WHERE item_guid = `in_item_guid`;
		IF exist_item_data_num > 0 THEN
			DELETE FROM `role_item` WHERE  item_guid = `in_item_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,`_sql`,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;

  IF t_error = 1 THEN    
    ROLLBACK;    
  ELSE    
    COMMIT;    
  END IF;   
  SELECT t_error;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_all_account`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_all_account`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_all_account`(in in_begin_index int,in in_end_index int)
BEGIN
	SELECT account FROM account ORDER BY id LIMIT in_begin_index, in_end_index;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_all_legion_guid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_all_legion_guid`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_all_legion_guid`()
BEGIN
	SELECT legion_guid FROM legion_info;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_all_role_guid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_all_role_guid`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_all_role_guid`(in in_begin_index int,in in_end_index int)
BEGIN
	SELECT role_guid FROM role_info ORDER BY role_guid LIMIT in_begin_index, in_end_index;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_appearance_info_by_id`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_appearance_info_by_id`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_appearance_info_by_id`(in in_appearance_id int)
BEGIN
	SELECT * FROM `role_apppearance_info` WHERE appearance_id = in_appearance_id  limit 1;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_marry_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_marry_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_marry_info`(in `in_row` int, in `in_load_max` int)
BEGIN
	SELECT * FROM `role_marry` LIMIT in_row, in_load_max;
 
 END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_other_server_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_other_server_mail`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_other_server_mail`(in `in_server_id` int, in `in_mail_guid` bigint, in `in_select_count` int)
BEGIN
	SELECT * FROM role_mail WHERE server_id=in_server_id OR mail_guid=in_mail_guid ORDER BY mail_guid=in_mail_guid, EMailInfo_SendTime_num LIMIT in_select_count;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_other_server_mail_record`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_other_server_mail_record`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_other_server_mail_record`()
BEGIN
	SELECT * FROM other_server_mail_record;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_load_role_appearance_id`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_load_role_appearance_id`;
DELIMITER ;;
CREATE PROCEDURE `sp_load_role_appearance_id`(IN `_role_guid` bigint)
BEGIN
	SELECT e_role_info_appearance_id from role_info where role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_create_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_create_role`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_create_role`(in in_role_guid bigint,
    in in_role_name varchar(100),
    in in_account varchar(100),
    in in_create_level_requirementint int,   in in_role_info varchar(5096),
    in in_item_list mediumtext,
    in in_skill_list mediumtext,   in in_guide_list mediumtext)
begin
	declare _count integer default 0;
	declare _max_level integer default 0;
	declare _res integer default 0;
	DECLARE _temp_name VARCHAR(100);
	DECLARE _insert_sql VARCHAR(6000) DEFAULT 'insert into `role_info` values (';
	declare exit handler for sqlexception
	begin
    rollback;
		set _res = -1;		
		select _res;
	end;

	start transaction;
	
		IF LENGTH(_temp_name) < 2 THEN
			set _res = -3;
		ELSE
			SELECT role_name INTO _temp_name FROM `role_info` WHERE role_name = in_role_name;
			IF ISNULL(_temp_name) THEN
				SELECT count(1), MAX(e_role_info_exp_level) INTO _count,_max_level FROM `role_info` WHERE account = in_account AND (e_role_info_del_time > unix_timestamp() - 86400 OR e_role_info_del_time = 0);
				IF _count >= 3 THEN
					set _res = -3;
				ELSE
						IF _max_level < in_create_level_requirementint THEN
							set _res = -4;
						ELSE
							SET @sql = concat(_insert_sql,in_role_info,');');
							PREPARE stmt FROM @sql;
							EXECUTE stmt;
							DEALLOCATE PREPARE stmt;
								INSERT INTO role_time(role_guid, e_time_type_create_time) VALUES(in_role_guid, unix_timestamp());
								CALL sp_role_item_save(in_role_guid, 0, in_item_list);
								CALL sp_role_skill_save(in_role_guid, in_skill_list);
								CALL sp_create_role_init_guide_save(in_role_guid, in_guide_list);
								INSERT INTO ranking_role_worship values (in_role_guid, 0, unix_timestamp());
								INSERT INTO role_att(role_guid, e_base_att_info_pk_mode) VALUES(in_role_guid, 112);
							SET _res = 1;
						END IF;
					END IF;
				ELSE
					set _res = -2;
				END IF;
			END IF;
	COMMIT;
	SELECT _res;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_del_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_del_role`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_del_role`(in `in_account` varchar(100), in `in_role_guid` bigint, in `in_del_type` int)
begin
	declare _del_time integer default 0;
	IF in_del_type = 0 THEN
		UPDATE role_info SET e_role_info_del_time = unix_timestamp() WHERE role_guid = in_role_guid;
		SET _del_time = unix_timestamp();
	ELSEIF in_del_type = 1 THEN
		UPDATE role_info SET e_role_info_del_time = 0 WHERE role_guid = in_role_guid;
		SET _del_time = 0;
	ELSE
		UPDATE role_info SET e_role_info_del_time = 1 WHERE role_guid = in_role_guid;
		SET _del_time = 1;
	END IF;
	SELECT _del_time;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_enum_role_buff`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_enum_role_buff`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_enum_role_buff`(in `in_role_guid` bigint)
BEGIN
		SELECT * FROM `role_buff` WHERE role_guid = `in_role_guid` AND 
				(EBuffInfo_series = 221100 			-- Upgrade Weapon
					OR EBuffInfo_series = 231101 	-- Upgrade Equip
					OR EBuffInfo_series = 240018	-- Addon Weapon
					OR EBuffInfo_series = 240019	-- Addon Equip
					OR EBuffInfo_series = 990289	-- Rune 1
					OR EBuffInfo_series = 990290	-- Rune 2
					OR EBuffInfo_series = 990291	-- Rune 3
					OR EBuffInfo_series = 990292	-- Rune 4
					OR EBuffInfo_series = 990293	-- Rune 5
					OR EBuffInfo_series = 990294	-- Rune 6
					OR EBuffInfo_series = 990295	-- Rune 7
					OR EBuffInfo_series = 990296	-- Rune 8
					OR EBuffInfo_series = 990297	-- Rune 9
					OR EBuffInfo_series = 990298	-- Rune 10
					OR EBuffInfo_series = 990299	-- Rune 11
					OR EBuffInfo_series = 990300	-- Rune 12
					OR EBuffInfo_series = 990301	-- Rune 13
					OR EBuffInfo_series = 990302	-- Rune 14
					OR EBuffInfo_series = 990303	-- Rune 15
					OR EBuffInfo_series = 990304	-- Rune 16
					OR EBuffInfo_series = 990306	-- Succinct Weapon
					OR EBuffInfo_series = 990307	-- Succinct Equip
					OR EBuffInfo_series = 992005	-- first pay weapon zs
					OR EBuffInfo_series = 992006	-- first pay weapon fs
					OR EBuffInfo_series = 992007	-- first pay weapon qs
					OR EBuffInfo_series = 992008
					OR EBuffInfo_series = 240020
					OR EBuffInfo_series = 240024
					OR EBuffInfo_series = 240025
					OR EBuffInfo_series = 240026);-- first pay weapon dz
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_enum_role_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_enum_role_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_enum_role_info`(IN `_account` varchar(100),IN `_area_id` int)
BEGIN
	SELECT * FROM role_info WHERE account = _account  AND (e_role_info_del_time > unix_timestamp() - 86400 OR e_role_info_del_time = 0);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_enum_role_item`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_enum_role_item`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_enum_role_item`(in `in_role_guid` bigint,in `in_wing_id` int,in `in_mount_id` int,in `in_wing_illusion_id` int,in `in_mount_illusion_id` int)
BEGIN
		-- SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND (e_item_info_container_type = 1 OR e_item_info_container_type = 12 OR e_item_info_info_id = in_wing_id OR e_item_info_info_id = in_mount_id);
				SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND (e_item_info_container_type = 1 OR e_item_info_container_type = 12 OR e_item_info_info_id = in_wing_id OR e_item_info_info_id = in_mount_id OR e_item_info_info_id = in_wing_illusion_id OR e_item_info_info_id = in_mount_illusion_id);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_enum_role_spirit`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_enum_role_spirit`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_enum_role_spirit`(in `in_role_guid` bigint)
BEGIN
		SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND e_item_info_container_type = 19 AND e_item_info_activate > 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_login_enum_role_time`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_login_enum_role_time`;
DELIMITER ;;
CREATE PROCEDURE `sp_login_enum_role_time`(in `in_role_guid` bigint)
BEGIN
		SELECT e_time_type_logout_time FROM `role_time` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_lucky_draw_record_ws_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_lucky_draw_record_ws_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_lucky_draw_record_ws_add`(in in_info_guid bigint,  in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `ws_lucky_draw_record` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM ws_lucky_draw_record WHERE info_guid = in_info_guid;
		IF exist_data_num > 0 THEN
				DELETE FROM ws_lucky_draw_record WHERE info_guid = in_info_guid ;
		END IF;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_lucky_draw_record_ws_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_lucky_draw_record_ws_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_lucky_draw_record_ws_del`(in in_info_guid bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM ws_lucky_draw_record WHERE info_guid = in_info_guid;
	IF exist_data_num > 0 THEN
			DELETE FROM ws_lucky_draw_record WHERE info_guid = in_info_guid;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_lucky_draw_record_ws_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_lucky_draw_record_ws_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_lucky_draw_record_ws_load`()
BEGIN
	SELECT * FROM ws_lucky_draw_record;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_globel_mail_item_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_globel_mail_item_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_globel_mail_item_load`()
BEGIN
	SELECT * FROM `mail_globel_mail_item`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_globel_mail_item_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_globel_mail_item_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_globel_mail_item_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `mail_globel_mail_item` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		-- DELETE FROM `server_mail` WHERE mail_guid = `in_mail_guid`;
		DELETE FROM mail_globel_mail_item;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_globel_mail_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_globel_mail_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_globel_mail_load`()
BEGIN
	SELECT * FROM `mail_globel_mail`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_globel_mail_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_globel_mail_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_globel_mail_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `mail_globel_mail` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		-- DELETE FROM `server_mail` WHERE mail_guid = `in_mail_guid`;
		DELETE FROM mail_globel_mail;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_id_mail_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_id_mail_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_id_mail_del`(in in_role_guid bigint, in in_cur_time int, in in_check_num int)
BEGIN
	DELETE FROM role_mail_by_id 
		WHERE in_role_guid = role_guid AND show_send_time < in_cur_time
		ORDER BY show_send_time LIMIT in_check_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_mail_id_mail_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_mail_id_mail_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_mail_id_mail_load`(in in_role_guid bigint, in in_cur_time int, in in_check_num int)
BEGIN
	SELECT * FROM role_mail_by_id 
		WHERE in_role_guid = role_guid AND show_send_time < in_cur_time
		ORDER BY show_send_time LIMIT in_check_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_marry_load_last_login_time`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_marry_load_last_login_time`;
DELIMITER ;;
CREATE PROCEDURE `sp_marry_load_last_login_time`(in `in_role_guid` bigint)
BEGIN
	SELECT  e_time_type_login_time FROM `role_time` WHERE in_role_guid = role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_marry_load_marry_info_cross_server`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_marry_load_marry_info_cross_server`;
DELIMITER ;;
CREATE PROCEDURE `sp_marry_load_marry_info_cross_server`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_marry` where role_guid = in_role_guid or mate_guid = in_role_guid  ;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_merge_server_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_merge_server_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_merge_server_clear`()
BEGIN
	delete from role_ranking;
	delete from croos_world_boss;
	delete from cross_server_harry_info;
	delete from cross_server_overload_city_info;
	delete from cross_server_pk_info;
	delete from cross_world_boss;
	delete from last_server_list;
		
	UPDATE server_info SET begin_first_merge_server_time = UNIX_TIMESTAMP(NOW()) WHERE begin_first_merge_server_time = 0 OR ISNULL(begin_first_merge_server_time);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_on_dp_started`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_on_dp_started`;
DELIMITER ;;
CREATE PROCEDURE `sp_on_dp_started`(
        p_dp_app_id int unsigned
    )
begin
        declare temp_conn_id int unsigned;
        declare dp_conn_count int unsigned;

        declare exit handler for sqlexception rollback;

        set temp_conn_id = connection_id();

        start transaction;
        select count(*) into dp_conn_count from dp_list where dp_app_id = p_dp_app_id lock in share mode;
        if dp_conn_count = 0 then
             call `sp_on_servercluster_started`(p_dp_app_id);
        end if;
        insert into dp_list(dp_app_id, conn_id) values(p_dp_app_id, temp_conn_id);
        call sp_dp_ping();
        commit;
end
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_oracle_trial_player_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_oracle_trial_player_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_oracle_trial_player_info_save`(IN `_role_guid` bigint,IN`_tier_num` int,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `oracle_trial_player_info` values ';

	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM oracle_trial_player_info WHERE role_guid = `_role_guid` AND tier_num = `_tier_num`;
				IF exist_data_num > 0 THEN
		DELETE FROM oracle_trial_player_info WHERE role_guid = `_role_guid` AND tier_num = `_tier_num`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_overlord_legion_clear_all`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_overlord_legion_clear_all`;
DELIMITER ;;
CREATE PROCEDURE `sp_overlord_legion_clear_all`(in `in_mount_id` int, in `in_wing_id` int, in `in_title_id` int, in `in_buff_id` int)
BEGIN
DECLARE exist_data_num INT DEFAULT 0;
IF in_wing_id != 0	THEN
	UPDATE role_info SET e_role_info_wing_showd_template_id = 0  WHERE e_role_info_wing_showd_template_id = in_wing_id ; 
	UPDATE role_item SET e_item_info_activate = 0  WHERE e_item_info_info_id = in_wing_id ;
END IF;

IF in_mount_id != 0 THEN
	UPDATE role_info SET e_role_info_mount_showd_template_id = 0  WHERE e_role_info_mount_showd_template_id = in_mount_id ;
	UPDATE role_item SET e_item_info_activate = 0  WHERE e_item_info_info_id = in_mount_id ;
END IF;

IF in_title_id != 0	THEN
	UPDATE role_att SET e_base_att_info_equip_title_id = 0 WHERE e_base_att_info_equip_title_id = in_title_id;
	SELECT COUNT(*) INTO exist_data_num FROM role_title WHERE e_title_info_titletemplate_id = in_title_id;
	IF exist_data_num > 0 THEN
			DELETE FROM role_title WHERE e_title_info_titletemplate_id = in_title_id;
	END IF;
END IF;

IF in_buff_id != 0	THEN
	SELECT COUNT(*) INTO exist_data_num FROM role_buff WHERE EBuffInfo_template_id = in_buff_id;
	IF exist_data_num > 0 THEN
		DELETE FROM role_buff WHERE EBuffInfo_template_id = in_buff_id;
	END IF;
END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_overlord_legion_clear_one_member_stuff`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_overlord_legion_clear_one_member_stuff`;
DELIMITER ;;
CREATE PROCEDURE `sp_overlord_legion_clear_one_member_stuff`(in`in_role_guid` bigint, in `in_mount_id` int, in `in_wing_id` int, in `in_title_id` int, in `in_buff_id` int)
BEGIN
DECLARE exist_data_num INT DEFAULT 0;
IF in_role_guid != 0 THEN
IF in_wing_id != 0	THEN
	UPDATE `role_info` SET e_role_info_wing_showd_template_id = 0  WHERE e_role_info_wing_showd_template_id = in_wing_id AND role_guid = in_role_guid; 
	UPDATE `role_item` SET e_item_info_activate = 0  WHERE e_item_info_info_id = in_wing_id AND role_guid = in_role_guid;
END IF;

IF in_mount_id != 0 THEN
	UPDATE `role_info` SET e_role_info_mount_showd_template_id = 0  WHERE e_role_info_mount_showd_template_id = in_mount_id AND role_guid = in_role_guid;
	UPDATE `role_item` SET e_item_info_activate = 0  WHERE e_item_info_info_id = in_mount_id AND role_guid = in_role_guid;
END IF;

IF in_buff_id != 0	THEN
	SELECT COUNT(*) INTO exist_data_num FROM role_buff WHERE EBuffInfo_template_id = in_buff_id AND role_guid = in_role_guid;
	IF exist_data_num > 0 THEN
		DELETE FROM `role_buff` WHERE EBuffInfo_template_id = in_buff_id AND role_guid = in_role_guid;
	END IF;
END IF;

IF in_title_id != 0	THEN
	UPDATE `role_att` SET e_base_att_info_equip_title_id = 0 WHERE e_base_att_info_equip_title_id = in_title_id AND role_guid = in_role_guid;
	SELECT COUNT(*) INTO exist_data_num FROM role_title WHERE e_title_info_titletemplate_id = in_title_id AND role_guid = in_role_guid;
	IF exist_data_num > 0 THEN
		DELETE FROM `role_title` WHERE e_title_info_titletemplate_id = in_title_id AND role_guid = in_role_guid;
	END IF;
END IF;

END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ranking_arena_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ranking_arena_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_ranking_arena_info_load`(IN `in_ranking_type` bigint, in  `in_load_num` int)
BEGIN
	SELECT * FROM role_ranking WHERE ERankingInfo_ranking_type = `in_ranking_type` AND   ERankingInfo_ranking_value != -1
	ORDER BY ERankingInfo_ranking_value ASC , ERankingInfo_ranking_time ASC LIMIT in_load_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ranking_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ranking_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_ranking_info_load`(IN `in_ranking_type` bigint, in  `in_load_num` int)
BEGIN
	SELECT * FROM role_ranking WHERE ERankingInfo_ranking_type = `in_ranking_type` ORDER BY ERankingInfo_peak DESC ,ERankingInfo_ranking_value DESC , ERankingInfo_ranking_time ASC LIMIT in_load_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ranking_info_load_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ranking_info_load_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_ranking_info_load_one`(IN `in_role_guid` bigint, in `in_ranking_type` bigint)
BEGIN
	SELECT * FROM `role_ranking` WHERE role_guid = `in_role_guid` AND ERankingInfo_ranking_type = `in_ranking_type` ;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ranking_update_vip_level`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ranking_update_vip_level`;
DELIMITER ;;
CREATE PROCEDURE `sp_ranking_update_vip_level`(IN `in_ranking_guid` bigint, in  `in_vip_level` int)
BEGIN
	 UPDATE role_ranking  set ERankingInfo_vip_level = in_vip_level WHERE role_guid = `in_ranking_guid` ;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_challenge_log_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_challenge_log_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_challenge_log_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM ranking_arena_challenge_log WHERE role_guid = in_role_guid
		ORDER BY date DESC LIMIT 0,100; 
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_challenge_log_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_challenge_log_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_challenge_log_save`(in _role_guid bigint,in role_name varchar(100),in in_role_rank int,in _target_guid bigint ,in target_name varchar(100),in in_target_rank int,in _result int,in _date int)
BEGIN
	-- DECLARE _insert_sql mediumtext DEFAULT 'insert into rank_arena_challenge_log(role_guid) value ';
	DECLARE _target_result int DEFAULT 0 ;
	
  START TRANSACTION;
		INSERT INTO ranking_arena_challenge_log(role_guid,role_name,role_rank,target_guid,target_name,target_rank,initiative,result,date) VALUE(_role_guid,role_name,in_role_rank,_target_guid,target_name,in_target_rank,1,_result,_date );

		IF _result > 0 THEN
				set _target_result = 0;
		ELSE
				set _target_result = 1;
		END IF;
		
		INSERT INTO ranking_arena_challenge_log(role_guid,role_name,role_rank,target_guid,target_name,target_rank,initiative,result,date) VALUE(_target_guid,target_name,in_target_rank,_role_guid,role_name,in_role_rank,0,_target_result,_date);
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_del_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_del_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_del_one`(in `in_role_guid` bigint)
BEGIN
		DECLARE exist_data_num INT DEFAULT 0;
		SELECT COUNT(*) INTO exist_data_num FROM ranking_arena WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
			DELETE FROM ranking_arena WHERE role_guid = `in_role_guid`;
		END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_load`(in _from_index int, in _load_num int)
BEGIN
	SELECT * FROM `ranking_arena` ORDER BY position LIMIT _from_index,_load_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_save`(in _is_first bool, in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `ranking_arena` values ';
  START TRANSACTION;

		IF _is_first THEN
			DELETE FROM ranking_arena;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_arena_save_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_arena_save_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_arena_save_one`(in `in_role_guid` bigint,  in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `ranking_arena` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM ranking_arena WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM ranking_arena WHERE  role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_worship_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_worship_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_worship_add`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `ranking_role_worship` values ';
  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_worship_player_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_worship_player_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_worship_player_add`(in `in_role_guid` bigint ,in `in_role_name` varchar(100) ,in `in_ranking_type` bigint , in `add_value` bigint , in `in_time` bigint, in`in_supporting_guid` bigint,in `in_supporting_name` varchar(100),in`in_vip` int,in`in_peak` int,in`in_server_id` int,in `in_game_channel` varchar(128),in `in_standby_parameter` bigint)
BEGIN
		INSERT INTO role_ranking VALUES(in_role_guid, in_role_name,in_ranking_type,add_value, in_time, in_supporting_guid, in_supporting_name,in_vip,in_peak,in_server_id,in_game_channel,in_standby_parameter) 
			ON DUPLICATE KEY UPDATE ERankingInfo_ranking_value=ERankingInfo_ranking_value+add_value, ERankingInfo_ranking_time = in_time , ERankingInfo_supporting_guid = in_supporting_guid,ERankingInfo_supporting_name = in_supporting_name,ERankingInfo_vip_level =  in_vip,ERankingInfo_peak =  in_peak,ERankingInfo_server_id = in_server_id,ERankingInfo_game_channel = in_game_channel,ERankingInfo_stanby_parameter = in_standby_parameter;
		SELECT * FROM role_ranking WHERE (role_guid = in_role_guid AND ERankingInfo_ranking_type = in_ranking_type );
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_rank_worship_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_rank_worship_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_rank_worship_save`(in `in_role_guid` bigint, in _sql mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `ranking_role_worship` values ';
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM ranking_role_worship WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM ranking_role_worship WHERE  role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_recharge_process_sucess`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_recharge_process_sucess`;
DELIMITER ;;
CREATE PROCEDURE `sp_recharge_process_sucess`(in in_order_id varchar(40),in in_payment_type int,in in_end_time bigint)
BEGIN										
	#Routine body goes here...									
	UPDATE recharge_record SET is_sucess = 1,end_time=in_end_time 
		WHERE order_id = in_order_id AND payment_type = in_payment_type;									
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_add`(in `in_red_package_guid` bigint, in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `red_package_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  SELECT COUNT(*) INTO exist_data_num FROM red_package_info WHERE red_package_guid = `in_red_package_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM red_package_info WHERE red_package_guid = `in_red_package_guid`;
	END IF;

  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_del`(in `in_red_package_guid` bigint)
BEGIN
		DECLARE exist_data_num INT DEFAULT 0;
		SELECT COUNT(*) INTO exist_data_num FROM red_package_info WHERE red_package_guid = `in_red_package_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM red_package_info WHERE red_package_guid = `in_red_package_guid`;
		END IF;
		
		SELECT COUNT(*) INTO exist_data_num FROM red_package_receiver_info WHERE red_package_guid = `in_red_package_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM red_package_receiver_info WHERE red_package_guid = `in_red_package_guid`;
		END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_load`()
BEGIN
	-- SELECT TOP(50) * FROM `role_mail` WHERE  role_guid = `in_role_guid`;
			
	SELECT * FROM `red_package_info`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_receiver_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_receiver_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_receiver_add`(in `in_red_package_guid` bigint, in `in_receiver_guid` bigint,   in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `red_package_receiver_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
	SELECT COUNT(*) INTO exist_data_num FROM red_package_receiver_info WHERE red_package_guid = `in_red_package_guid`
																				AND receiver_guid = `in_red_package_guid`;
	IF exist_data_num > 0 THEN
	DELETE FROM red_package_receiver_info WHERE red_package_guid = `in_red_package_guid`
																				AND receiver_guid = `in_red_package_guid`;
	END IF;

  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_receiver_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_receiver_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_receiver_load`(IN `in_red_package_guid` bigint)
BEGIN
	-- SELECT TOP(50) * FROM `role_mail` WHERE  role_guid = `in_role_guid`;
			
	SELECT * FROM `red_package_receiver_info` WHERE red_package_guid = `in_red_package_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_receiver_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_receiver_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_receiver_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `red_package_receiver_info` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		DELETE FROM red_package_receiver_info;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_redpack_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_redpack_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_redpack_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `red_package_info` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		DELETE FROM red_package_info;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_refresh_role_time_limit_guid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_refresh_role_time_limit_guid`;
DELIMITER ;;
CREATE PROCEDURE `sp_refresh_role_time_limit_guid`(IN `_old_act_guid` int,IN `_new_act_guid` int)
BEGIN
	UPDATE role_time_limit_activity SET act_guid = _new_act_guid where act_guid = _old_act_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_achievement_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_achievement_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_achievement_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_achievement` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_achievement_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_achievement_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_achievement_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_achievement` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_achievement WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_achievement WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_active_degree_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_active_degree_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_active_degree_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_active_degree` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_active_degree_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_active_degree_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_active_degree_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_active_degree` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_active_degree WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_active_degree WHERE  role_guid = `in_role_guid`;
		END IF;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_arena_skill_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_arena_skill_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_arena_skill_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_skill` WHERE  role_guid = `in_role_guid` 
		AND e_skill_info_use_pos IN(0,1,2,3);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_att_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_att_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_att_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_att` WHERE role_guid = `in_role_guid`;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_att_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_att_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_att_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_att` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_att WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_att WHERE  role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_belief_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_belief_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_belief_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_belief` WHERE role_guid = `in_role_guid`;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_belief_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_belief_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_belief_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_belief` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_belief WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_belief WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_boss_island_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_boss_island_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_boss_island_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_boss_island` WHERE role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_boss_island_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_boss_island_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_boss_island_info_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_boss_island` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_boss_island WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_boss_island WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_buff_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_buff_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_buff_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_buff` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_buff_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_buff_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_buff_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_buff` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_buff WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_buff WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_chat_record_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_chat_record_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_chat_record_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_chat_record` WHERE  role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_chat_record_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_chat_record_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_chat_record_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_chat_record` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_chat_record WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_chat_record WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_convert_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_convert_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_convert_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_convert` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_convert_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_convert_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_convert_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_convert` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_convert WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_convert WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_daily_must_do_count_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_daily_must_do_count_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_daily_must_do_count_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_daily_must_do_count` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_daily_must_do_count_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_daily_must_do_count_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_daily_must_do_count_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_daily_must_do_count` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_daily_must_do_count WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_daily_must_do_count WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_data_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_data_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_data_load`(in `in_role_guid` bigint,in `in_table_name` varchar(256))
BEGIN
  DECLARE _sql1 mediumtext DEFAULT 'SELECT * FROM ';
  DECLARE _sql2 mediumtext DEFAULT ' WHERE  role_guid = ';
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  START TRANSACTION;
		SET @sql = concat(_sql1, in_table_name, _sql2,in_role_guid,";");
		PREPARE stmt FROM @sql;
	 	EXECUTE stmt;
	 	DEALLOCATE PREPARE stmt;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_data_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_data_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_data_save`(in `in_role_guid` bigint,
 in `in_table_name` varchar(256), in _sql mediumtext)
BEGIN
  DECLARE _sql1 mediumtext DEFAULT 'INSERT INTO ';
  DECLARE _sql2 mediumtext DEFAULT '(role_guid, data_array) VALUES(';
  DECLARE _sql3 mediumtext DEFAULT ') ON DUPLICATE KEY UPDATE data_array = ';
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  START TRANSACTION;
		SET @sql = concat(_sql1, in_table_name, _sql2,in_role_guid,",",_sql,_sql3, _sql,";");
		-- SELECT @sql;
		PREPARE stmt FROM @sql;
	 	EXECUTE stmt;
	 	DEALLOCATE PREPARE stmt;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_demons_tower_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_demons_tower_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_demons_tower_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM `role_demons_tower` WHERE `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_demons_tower_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_demons_tower_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_demons_tower_save`(in in_role_guid bigint,
    in in_sql_cmd_str mediumtext)
BEGIN
		DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN

						SELECT COUNT(*) INTO exist_data_num FROM role_demons_tower WHERE `role_guid` = in_role_guid;
						IF exist_data_num > 0 THEN
								DELETE FROM role_demons_tower WHERE `role_guid` = in_role_guid;
						END IF;

            SET @sql_cmd = concat('INSERT INTO `role_demons_tower` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_dragontrip_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_dragontrip_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_dragontrip_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_dragontrip` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_dragontrip_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_dragontrip_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_dragontrip_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_dragontrip` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_dragontrip WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_dragontrip WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_element_heart_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_element_heart_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_element_heart_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_element_heart` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_element_heart_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_element_heart_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_element_heart_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_element_heart` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_element_heart WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_element_heart WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_event_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_event_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_event_load`(in `in_role_guid` bigint)
BEGIN
	SELECT *FROM `role_event` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_event_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_event_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_event_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_event` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_event WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_event WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_feather_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_feather_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_feather_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM role_item WHERE role_guid = `in_role_guid`  AND (role_item.e_item_info_info_id = 31040045 OR role_item.e_item_info_info_id = 31040046 OR role_item.e_item_info_info_id = 31040047 OR role_item.e_item_info_info_id = 31040048);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_fighting_spirit_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_fighting_spirit_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_fighting_spirit_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_spirit` WHERE  role_guid = `in_role_guid` AND ESpiritInfo_spirit_is_fighting > 0;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_fight_att_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_fight_att_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_fight_att_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_fight_att` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_fight_att_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_fight_att_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_fight_att_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_fight_att` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_fight_att WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_fight_att WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_first_time_do_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_first_time_do_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_first_time_do_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM `role_first_time_do` WHERE `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_first_time_do_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_first_time_do_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_first_time_do_save`(in in_role_guid bigint,
    in in_do_type int,
    in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_first_time_do` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_first_time_do WHERE role_guid = in_role_guid AND do_type = in_do_type;
		IF exist_data_num > 0 THEN
				DELETE FROM role_first_time_do WHERE role_guid = in_role_guid AND do_type = in_do_type;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_gain_treasure_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_gain_treasure_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_gain_treasure_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_gain_treasure` WHERE role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_gain_treasure_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_gain_treasure_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_gain_treasure_info_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_gain_treasure` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_gain_treasure WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_gain_treasure WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_goods_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_goods_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_goods_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_goods` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_goods_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_goods_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_goods_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_goods` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_goods WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_goods WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_guide_record_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_guide_record_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_guide_record_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM `role_guide_record` where `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_guide_record_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_guide_record_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_guide_record_save`(in in_role_guid bigint,
    in in_guide_record_id int,
	in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN

						SELECT COUNT(*) INTO exist_data_num FROM role_guide_record WHERE `role_guid` = in_role_guid AND `guide_record_id` = in_guide_record_id;
						IF exist_data_num > 0 THEN
								DELETE FROM role_guide_record WHERE `role_guid` = in_role_guid AND `guide_record_id` = in_guide_record_id;
						END IF;

            SET @sql_cmd = concat('INSERT INTO `role_guide_record` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_harry_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_harry_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_harry_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_harry_record` WHERE role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_harry_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_harry_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_harry_info_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_harry_record` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_harry_record WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_harry_record WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_highest_record_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_highest_record_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_highest_record_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_history_highest_record` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_highest_record_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_highest_record_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_highest_record_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_history_highest_record` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_history_highest_record WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_history_highest_record WHERE  role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_info` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_info_load_by_acount`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_info_load_by_acount`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_info_load_by_acount`(in `in_role_account` varchar(100))
BEGIN
	SELECT * FROM `role_info` WHERE account = `in_role_account`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_info_load_by_name`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_info_load_by_name`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_info_load_by_name`(in `in_role_name` varchar(100))
BEGIN
	SELECT * FROM `role_info` WHERE role_name = `in_role_name`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_info_save`(in _sql mediumtext)
BEGIN
 -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  START TRANSACTION;
		IF length(_sql) > 0 THEN
			SET @sql = concat(_sql,'');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_item_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_item_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_item_add`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_item` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_item_del_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_item_del_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_item_del_one`(in `in_item_guid` bigint,in `in_role_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM role_item WHERE item_guid = `in_item_guid` and role_guid = `in_role_guid` ;
	IF exist_data_num > 0 THEN
			DELETE FROM role_item WHERE item_guid = `in_item_guid` and role_guid = `in_role_guid` ;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_item_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_item_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_item_load`(in `in_role_guid` bigint, in _bag_type int)
BEGIN
	SELECT * FROM `role_item` WHERE role_guid = `in_role_guid` AND e_item_info_container_type = _bag_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_item_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_item_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_item_save`(in `in_role_guid` bigint, in _bag_type int, 
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_item` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_item WHERE role_guid = `in_role_guid` AND e_item_info_container_type = _bag_type;
		IF exist_data_num > 0 THEN
				DELETE FROM role_item WHERE role_guid = `in_role_guid` AND e_item_info_container_type = _bag_type;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_legion_skill_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_legion_skill_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_legion_skill_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM role_legion_skill WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_legion_skill_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_legion_skill_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_legion_skill_save`(in `in_role_guid` bigint, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql MEDIUMTEXT DEFAULT 'insert into `role_legion_skill` values ';
	DECLARE exist_data_num INT DEFAULT 0;

	START TRANSACTION;
		
		SELECT COUNT(*) INTO exist_data_num FROM role_legion_skill WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_legion_skill WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_add`(in in_role_guid bigint,    in in_mail_guid bigint,   in _sql mediumtext)
BEGIN
	DECLARE _count integer default 0;
	DECLARE exist_data_num INT DEFAULT 0;
	DECLARE _oldestGuid BIGINT default 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_mail` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
/*
		IF length(_sql)>0 THEN		
			SELECT count(1) INTO _count FROM `role_mail` WHERE in_role_guid = role_guid;
			IF _count>=50 THEN
				SELECT mail_guid INTO _oldestGuid  FROM `role_mail` WHERE in_role_guid  = role_guid ORDER BY EMailInfo_SendTime_num LIMIT 1;
				CALL sp_role_mail_delete(_oldestGuid);

				DELETE FROM role_mail WHERE mail_guid = in_mail_guid ;
				
				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;
			
			ELSE

				DELETE FROM role_mail WHERE mail_guid = in_mail_guid ;

				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

			END IF;
		END IF;
*/
				SELECT COUNT(*) INTO exist_data_num FROM role_mail WHERE mail_guid = in_mail_guid;
				IF exist_data_num > 0 THEN
						DELETE FROM role_mail WHERE mail_guid = in_mail_guid ;
				END IF;

				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_delete`(in in_mail_guid bigint)
BEGIN
	DECLARE _itemGuid1 BIGINT default 0;
	DECLARE _itemGuid2 BIGINT default 0;
	DECLARE _itemGuid3 BIGINT default 0;
	DECLARE _itemGuid4 BIGINT default 0;
	DECLARE _itemGuid5 BIGINT default 0;
	DECLARE exist_data_num INT DEFAULT 0;

	SELECT EMailInfo_ItemGuid1,  EMailInfo_ItemGuid2,   EMailInfo_ItemGuid3,   EMailInfo_ItemGuid4 ,EMailInfo_ItemGuid5
			INTO _itemGuid1,_itemGuid2,_itemGuid3,_itemGuid4,_itemGuid5 FROM `role_mail` WHERE mail_guid = in_mail_guid;

	SELECT COUNT(*) INTO exist_data_num FROM role_mail_item WHERE item_guid IN (_itemGuid1,_itemGuid2,_itemGuid3,_itemGuid4,_itemGuid5);
	IF exist_data_num > 0 THEN
			DELETE FROM role_mail_item WHERE item_guid IN (_itemGuid1,_itemGuid2,_itemGuid3,_itemGuid4,_itemGuid5);
	END IF;

	SELECT COUNT(*) INTO exist_data_num FROM role_mail WHERE mail_guid = in_mail_guid;
	IF exist_data_num > 0 THEN
			DELETE FROM role_mail WHERE mail_guid = in_mail_guid;
	END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_delete_batch`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_delete_batch`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_delete_batch`(in _sql mediumtext)
BEGIN
	DECLARE _param mediumtext;
	DECLARE _source_param mediumtext;
	DECLARE _max_loop_num int DEFAULT 100;
	DECLARE _loop_count int DEFAULT 0;
	SET _source_param = _sql;
	
	-- fist set "_param",or can't enter while loop
	SELECT SUBSTRING_INDEX(_source_param, ',', 1) INTO _param;
	SELECT SUBSTRING(_source_param, LENGTH(_param) + 2) INTO _source_param;
	CALL sp_role_mail_delete(_param);

	WHILE LENGTH(_param) > 0 DO
			SELECT SUBSTRING_INDEX(_source_param, ',', 1) INTO _param;
			SELECT SUBSTRING(_source_param, LENGTH(_param) + 2) INTO _source_param;

			IF LENGTH(_param) > 0 THEN
				CALL sp_role_mail_delete(_param);
			END IF;
	END WHILE;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_del_id_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_del_id_mail`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_del_id_mail`(in in_role_guid bigint,  in in_cur_time int,  in deal_num int)
BEGIN
	DELETE FROM role_mail_by_id
		WHERE role_guid = in_role_guid AND show_send_time < in_cur_time
		ORDER BY create_time LIMIT deal_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_item_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_item_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_item_add`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_mail_item` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_item_add_batch`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_item_add_batch`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_item_add_batch`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_mail_item` values ';
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_item_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_item_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_item_delete`(in in_mail_item_guid bigint)
BEGIN
DECLARE exist_data_num INT DEFAULT 0;

SELECT COUNT(*) INTO exist_data_num FROM role_mail_item WHERE item_guid = in_mail_item_guid;
IF exist_data_num > 0 THEN
		DELETE FROM role_mail_item WHERE item_guid = in_mail_item_guid ;
END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_item_delete_batch`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_item_delete_batch`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_item_delete_batch`(in del_item_guid_array mediumtext)
BEGIN
	DECLARE _param mediumtext;
	DECLARE _source_param mediumtext;
	DECLARE _max_loop_num int DEFAULT 100;
	DECLARE _loop_count int DEFAULT 0;
	SET _source_param = del_item_guid_array;
	
	-- fist set "_param",or can't enter while loop
	SELECT SUBSTRING_INDEX(_source_param, ',', 1) INTO _param;
	SELECT SUBSTRING(_source_param, LENGTH(_param) + 2) INTO _source_param;
	IF LENGTH(_param) > 0 THEN
		CALL sp_role_mail_item_delete(_param);
	END IF;
	WHILE LENGTH(_param) > 0 DO
			SELECT SUBSTRING_INDEX(_source_param, ',', 1) INTO _param;
			SELECT SUBSTRING(_source_param, LENGTH(_param) + 2) INTO _source_param;

			IF LENGTH(_param) > 0 THEN
				CALL sp_role_mail_item_delete(_param);
			END IF;
	END WHILE;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_item_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_item_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_item_load`(in `in_item_guid_array` mediumtext)
BEGIN
	DECLARE _select_sql mediumtext DEFAULT 'SELECT * FROM `role_mail_item` WHERE item_guid IN (';

	IF length(in_item_guid_array)>0 THEN
			SET @sql = concat(_select_sql,in_item_guid_array,')',';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_load`(in `in_role_guid` bigint)
BEGIN
	-- SELECT TOP(50) * FROM `role_mail` WHERE  role_guid = `in_role_guid`;
			
	SELECT * FROM `role_mail` WHERE in_role_guid  = role_guid ORDER BY EMailInfo_SendTime_num DESC LIMIT 0,50;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mail_select_id_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mail_select_id_mail`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mail_select_id_mail`(in in_role_guid bigint,  in in_cur_time int,  in deal_num int)
BEGIN
	SELECT * FROM role_mail_by_id 
		WHERE role_guid = in_role_guid AND show_send_time < in_cur_time
		ORDER BY create_time LIMIT deal_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mission_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mission_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mission_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM `role_mission` WHERE `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_mission_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_mission_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_mission_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_mission` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_mission WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_mission WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_money_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_money_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_money_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_money` WHERE role_guid = `in_role_guid` ORDER BY e_money_save_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_money_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_money_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_money_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_money` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_money WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_money WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_offline_chat_eliminate`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_offline_chat_eliminate`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_offline_chat_eliminate`(IN `role_guid` bigint)
BEGIN
	#Routine body goes here...
	DELETE FROM role_offline_chat where role_offline_chat.role_guid = role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_offline_chat_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_offline_chat_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_offline_chat_load`(IN `_role_guid` bigint)
BEGIN
	SELECT* FROM role_offline_chat WHERE role_guid = `_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_offline_chat_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_offline_chat_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_offline_chat_save`(IN `sender_guid` bigint,IN `address_guid` bigint,IN `sql_text` mediumtext)
BEGIN
	#Routine body goes here...	
	DECLARE	_ret  INT DEFAULT 0;
	DECLARE _chat_count INT DEFAULT 0;
	DECLARE _all_memeber_chat_count INT DEFAULT 0;
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_offline_chat` values ';
	START TRANSACTION;
		select COUNT(1) INTO _all_memeber_chat_count FROM role_offline_chat WHERE role_offline_chat.role_guid = address_guid;
		IF _all_memeber_chat_count < 1000 THEN
		
			select COUNT(1) INTO _chat_count FROM role_offline_chat WHERE role_offline_chat.role_guid = address_guid AND role_offline_chat.sender_guid = sender_guid;
			IF _chat_count < 100 THEN
					SET @sql = concat(_insert_sql,sql_text,';');
					PREPARE stmt FROM @sql;
					EXECUTE stmt;
					DEALLOCATE PREPARE stmt;
			ELSE
					SET _ret = 1;
			END IF;

		ELSE
			SET _ret = 1;
		END IF;
	COMMIT;

	SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_oracle_trial_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_oracle_trial_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_oracle_trial_info_load`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM oracle_trial_player_info WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_phantom_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_phantom_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_phantom_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_phantom` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_phantom_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_phantom_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_phantom_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_phantom` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_phantom WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_phantom WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_pokedex_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_pokedex_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_pokedex_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_pokedex` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_pokedex_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_pokedex_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_pokedex_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_pokedex` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_pokedex WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_pokedex WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_ranking_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_ranking_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_ranking_save`(in `in_role_guid` bigint, in `in_role_name` varchar(100), in `in_ranking_type` bigint, in `in_ranking_value`bigint,in `in_ranking_time` bigint, in `in_supporting_guid` bigint,in `in_supporting_name`varchar(100),in  `in_vip` int,in  `in_peak` int,in  `in_server_id` int,in  `in_game_channel` varchar(128), in `in_intstandby_parameter` bigint)
BEGIN
        INSERT INTO role_ranking VALUES (in_role_guid,in_role_name,in_ranking_type,in_ranking_value,in_ranking_time,in_supporting_guid,in_supporting_name, in_vip,in_peak, in_server_id, in_game_channel, in_intstandby_parameter)
				ON DUPLICATE KEY UPDATE role_name=in_role_name,  ERankingInfo_ranking_value = in_ranking_value, ERankingInfo_ranking_time = in_ranking_time, ERankingInfo_supporting_guid = in_supporting_guid,  ERankingInfo_supporting_name = in_supporting_name,  ERankingInfo_vip_level = in_vip,ERankingInfo_peak = in_peak,ERankingInfo_server_id = in_server_id, ERankingInfo_game_channel = in_game_channel, ERankingInfo_stanby_parameter = in_intstandby_parameter;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_ranking_save_one_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_ranking_save_one_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_ranking_save_one_list`(in `in_ranking_type` bigint, in _sql mediumtext)
BEGIN

  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_ranking` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_ranking WHERE ERankingInfo_ranking_type = `in_ranking_type`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_ranking WHERE ERankingInfo_ranking_type = `in_ranking_type`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_add_invited`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_add_invited`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_add_invited`(IN `in_inviter_guid` bigint, IN `in_invited_guid` bigint,IN `_sql` mediumtext)
BEGIN
  DECLARE v_exist_count integer default 0;
	DECLARE v_total_count integer default 0;
	DECLARE v_ret integer DEFAULT 0;
	DECLARE v_insert_sql mediumtext DEFAULT 'insert into `role_invited` values ';

  START TRANSACTION;
	SELECT count(role_invited.role_guid) into v_exist_count  FROM role_invited WHERE role_invited.role_guid = in_inviter_guid and role_invited.target_guid = in_invited_guid;
	if  v_exist_count = 0 THEN
			SELECT count(role_invited.role_guid) into v_total_count  from role_invited WHERE role_invited.role_guid = in_inviter_guid;
			IF v_total_count  < 100 THEN
					SET @sql = concat(v_insert_sql,_sql,';');
					PREPARE stmt FROM @sql;
					EXECUTE stmt;
					DEALLOCATE PREPARE stmt;
					SET v_ret = 1;
			END if;
  END IF;
	COMMIT;

	SELECT v_ret;
	 
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_get_inviter_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_get_inviter_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_get_inviter_info`(IN `in_role_guid` bigint)
BEGIN
	SELECT role_guid, e_role_info_exp_level FROM `role_info` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_invited_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_invited_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_invited_load`(IN `in_role_guid` bigint)
BEGIN
	  SELECT  role_guid, e_role_info_exp_level,e_role_info_template_id, e_role_info_class_type, role_name  from  role_info where role_guid in ( SELECT  target_guid  FROM `role_invited` WHERE  role_guid = `in_role_guid`);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_recycle` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_recycle` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_recycle WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_recycle WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_task_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_task_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_task_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_recycle_task` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_recycle_task_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_recycle_task_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_recycle_task_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_recycle_task` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_recycle_task WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_recycle_task WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_relation_add`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_relation_add`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_relation_add`(IN `role_guid` bigint, IN `target_guid` bigint, IN `relation_type` bigint,IN `_sql` mediumtext)
BEGIN
	#Routine body goes here..
	DECLARE _relation_count integer default 0;
	DECLARE _relation_type_count integer default 0;
	DECLARE _ret integer DEFAULT 0;
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_relation` values ';

  START TRANSACTION;
	SELECT count(role_relation.role_guid) into _relation_type_count FROM role_relation WHERE role_relation.role_guid = role_guid AND role_relation.relation_guid = target_guid AND role_relation.e_relation_data_type = relation_type;
	if _relation_type_count = 0 THEN
			SELECT count(role_relation.role_guid) into _relation_count  from role_relation where role_relation.role_guid = role_guid AND role_relation.e_relation_data_type = relation_type;

			IF _relation_count  < 99 THEN
					SET @sql = concat(_insert_sql,_sql,';');
					PREPARE stmt FROM @sql;
					EXECUTE stmt;
					DEALLOCATE PREPARE stmt;
					SET _ret = 1;
			END if;
  END IF;
	COMMIT;

	SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_relation_del`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_relation_del`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_relation_del`(IN `role_guid` bigint,IN `relation_guid` bigint, IN  `relation_type` bigint)
BEGIN
	#Routine body goes here...
	  START TRANSACTION;
		
		IF relation_type > 0 THEN
			DELETE FROM role_relation WHERE role_relation.role_guid = role_guid AND role_relation.relation_guid = relation_guid AND role_relation.e_relation_data_type = relation_type;
		ELSE
      DELETE FROM role_relation WHERE role_relation.role_guid = role_guid AND role_relation.relation_guid = relation_guid;
		END IF;
		COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_relation_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_relation_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_relation_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_relation` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_relation_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_relation_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_relation_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_relation` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_relation WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_relation WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_service_goal_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_service_goal_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_service_goal_load`(in `in_role_guid` bigint)
BEGIN
	#Routine body goes here...
	SELECT * FROM `role_service_goal` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_service_goal_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_service_goal_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_service_goal_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_service_goal` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
	 START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_service_goal WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_service_goal WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_service_rank_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_service_rank_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_service_rank_info_load`(IN `in_ranking_type` int, in  `in_load_num` int)
BEGIN
	SELECT * FROM role_service_rank WHERE rank_type = `in_ranking_type` ORDER BY role_num DESC LIMIT in_load_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_service_rank_reward_state_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_service_rank_reward_state_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_service_rank_reward_state_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_service_rank_reward_state` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_service_rank_reward_state_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_service_rank_reward_state_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_service_rank_reward_state_save`(in `in_role_guid` bigint ,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_service_rank_reward_state` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_service_rank_reward_state WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_service_rank_reward_state WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_seven_day_goal_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_seven_day_goal_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_seven_day_goal_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM role_seven_day_goal WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_seven_day_goal_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_seven_day_goal_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_seven_day_goal_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_seven_day_goal` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
	 START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_seven_day_goal WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_seven_day_goal WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_show_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_show_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_show_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_show_info` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_show_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_show_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_show_info_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_show_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_show_info WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_show_info WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_skill_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_skill_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_skill_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_skill` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_skill_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_skill_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_skill_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_skill` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_skill WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_skill WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_skytreasure_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_skytreasure_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_skytreasure_load`(in in_role_guid bigint)
BEGIN
	SELECT * FROM `role_skytreasure` where `role_guid` = in_role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_skytreasure_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_skytreasure_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_skytreasure_save`(in in_role_guid bigint,  in in_skytreasure_layer_num int,  in in_skytreasure_pos int,  in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;     

			SELECT COUNT(*) INTO exist_data_num FROM role_skytreasure WHERE `role_guid` = in_role_guid AND `skytreasure_layer_num` = in_skytreasure_layer_num AND `skytreasure_pos` = in_skytreasure_pos;
			IF exist_data_num > 0 THEN
					DELETE FROM role_skytreasure WHERE `role_guid` = in_role_guid AND `skytreasure_layer_num` = in_skytreasure_layer_num AND `skytreasure_pos` = in_skytreasure_pos;
			END IF;

			IF length(in_sql_cmd_str) > 0 THEN
         SET @sql_cmd = concat('INSERT INTO `role_skytreasure` VALUES ', in_sql_cmd_str, ';');
         PREPARE stmt FROM @sql_cmd;
         EXECUTE stmt;
         DEALLOCATE PREPARE stmt;
      END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_special_name_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_special_name_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_special_name_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_special_name` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_special_name_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_special_name_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_special_name_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_special_name` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_special_name WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_special_name WHERE  role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_spirit_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_spirit_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_spirit_clear`()
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM role_spirit WHERE (spirit_guid IN (SELECT item_guid FROM role_item));
	DELETE FROM role_spirit WHERE spirit_guid IN (SELECT item_guid FROM role_item);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_spirit_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_spirit_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_spirit_load`(in `in_role_guid` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	
	SELECT * FROM `role_spirit` WHERE  role_guid = `in_role_guid`;

	SELECT COUNT(*) INTO exist_data_num FROM role_spirit WHERE role_guid = `in_role_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM role_spirit WHERE role_guid = `in_role_guid`;
	END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_spirit_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_spirit_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_spirit_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_spirit` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_spirit WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_spirit WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_starark_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_starark_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_starark_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_starark` WHERE  role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_starark_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_starark_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_starark_save`(IN `in_role_guid` bigint,IN `in_sql` mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_starark` values ';
	DECLARE exist_data_num INT DEFAULT 0;

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_starark WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_starark WHERE role_guid = `in_role_guid`;
		END IF;
		
		IF length(in_sql)>0 THEN
			SET @sql = concat(_insert_sql,in_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_talent_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_talent_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_talent_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_talent` WHERE role_guid = `in_role_guid`;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_talent_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_talent_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_talent_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_talent` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_talent WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_talent WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_time_limit_activity_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_time_limit_activity_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_time_limit_activity_load`(in in_role_id bigint)
BEGIN
	SELECT * from `role_time_limit_activity` WHERE `role_guid`=in_role_id ORDER BY type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_time_limit_activity_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_time_limit_activity_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_time_limit_activity_save`(in in_role_id bigint,
	in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_time_limit_activity` values ';
  DECLARE exist_data_num INT DEFAULT 0;

  START TRANSACTION;
    SELECT COUNT(*) INTO exist_data_num FROM `role_time_limit_activity` WHERE `role_guid`=in_role_id;
    IF exist_data_num > 0 THEN
        DELETE FROM `role_time_limit_activity` WHERE `role_guid`=in_role_id;
    END IF;

    IF length(_sql)>0 THEN
      SET @sql = concat(_insert_sql,_sql,';');
      PREPARE stmt FROM @sql;
      EXECUTE stmt;
      DEALLOCATE PREPARE stmt;
    END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_time_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_time_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_time_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_time` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_time_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_time_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_time_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_time` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_time WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_time WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_tinder_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_tinder_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_tinder_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_tinder` WHERE role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_tinder_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_tinder_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_tinder_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_tinder` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_tinder WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_tinder WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_title_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_title_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_title_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_title` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_title_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_title_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_title_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_title` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_title WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_title WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_treasure_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_treasure_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_treasure_load`(IN `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_treasure` WHERE role_guid = `in_role_guid`;	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_treasure_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_treasure_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_treasure_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_treasure` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_treasure WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_treasure WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_welfare_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_welfare_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_welfare_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_welfare` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_welfare_random_get_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_welfare_random_get_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_welfare_random_get_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_welfare_random_get` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_welfare_random_get_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_welfare_random_get_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_welfare_random_get_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_welfare_random_get` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_welfare_random_get WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_welfare_random_get WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_welfare_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_welfare_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_welfare_save`(in `in_role_guid` bigint,
in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_welfare` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_welfare WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_welfare WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_worship_target_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_worship_target_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_worship_target_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_worship_target` WHERE  role_guid = `in_role_guid`;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_worship_target_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_worship_target_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_worship_target_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_worship_target` values ';
	DECLARE exist_data_num INT DEFAULT 0;
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_worship_target WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_worship_target WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_yesterday_must_do_remain_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_yesterday_must_do_remain_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_yesterday_must_do_remain_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_yesterday_must_do_remain` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_role_yesterday_must_do_remain_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_role_yesterday_must_do_remain_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_role_yesterday_must_do_remain_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_yesterday_must_do_remain` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_yesterday_must_do_remain WHERE role_guid = `in_role_guid` ;
		IF exist_data_num > 0 THEN
				DELETE FROM role_yesterday_must_do_remain WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_account_online_time`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_account_online_time`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_account_online_time`(_account varchar(100), _online_duration int, _online_time int, _month_recharge_num int, _month_recharge_time int)
BEGIN					

	START TRANSACTION;
		If Exists (Select account From `account` Where `account` = _account) Then
			UPDATE account SET online_duration=_online_duration, online_time=_online_time, month_recharge_num=_month_recharge_num, month_recharge_time=_month_recharge_time WHERE account=_account;
		End If;
	COMMIT;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_appearance_info_to_id`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_appearance_info_to_id`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_appearance_info_to_id`(IN `_in_appearance_info` varchar(1000))
BEGIN
	#Routine body goes here...
	declare _count integer default 0;

	start transaction;

		SELECT count(*) INTO _count FROM `role_apppearance_info` WHERE appearance_info = _in_appearance_info;
		IF _count = 0 THEN
			INSERT INTO role_apppearance_info(appearance_info) VALUES (_in_appearance_info);
		END IF;

	COMMIT;
	SELECT * FROM `role_apppearance_info` WHERE appearance_info = _in_appearance_info limit 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_marry_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_marry_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_marry_info`(in `in_role_guid` bigint, in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_marry` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_marry WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
		DELETE FROM role_marry WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_other_server_mail_record`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_other_server_mail_record`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_other_server_mail_record`(in in_source_server_id int, in in_target_server_id int,   in _sql mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `other_server_mail_record` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;

  START TRANSACTION;
				SELECT COUNT(*) INTO exist_data_num FROM other_server_mail_record WHERE source_server_id=in_source_server_id AND target_server_id=in_target_server_id;
				IF exist_data_num > 0 THEN
						DELETE FROM other_server_mail_record WHERE source_server_id=in_source_server_id AND target_server_id=in_target_server_id;
				END IF;

				SET @sql = concat(_insert_sql,_sql,';');
				PREPARE stmt FROM @sql;
				EXECUTE stmt;
				DEALLOCATE PREPARE stmt;

  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_person_information`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_person_information`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_person_information`(in in_role_guid bigint,in in_sql_cmd_str mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_person_information WHERE role_guid = in_role_guid;
			IF exist_data_num > 0 THEN
				DELETE  FROM role_person_information WHERE role_guid = in_role_guid;
			END IF;
			SET @sql_cmd = concat('INSERT INTO `role_person_information` VALUES ', in_sql_cmd_str, ';');
			PREPARE stmt FROM @sql_cmd;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_recharge`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_recharge`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_recharge`(in in_order_id varchar(40),in in_order_token varchar(32),in in_role_id bigint,in in_game_goods_id int,in in_payment_type int,in in_direct_diamond int,in in_begin_time bigint)
BEGIN					
	DECLARE	_success INT DEFAULT 0;			
	DECLARE	_order_id VARCHAR(40) DEFAULT '';			
	#Routine body goes here...				
	start transaction;				
		SELECT order_id INTO _order_id FROM `recharge_record` WHERE order_id=in_order_id OR (LENGTH(in_order_token) > 0 AND order_token=in_order_token);			
		 IF _order_id='' THEN			
			SET _success=1;		
			INSERT INTO `recharge_record` VALUES(in_order_id,in_role_id,in_game_goods_id,0,in_begin_time,0,in_payment_type,in_direct_diamond,in_order_token);		
		 END IF;			
	COMMIT;				
	SELECT _success;				
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_role_competition`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_role_competition`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_role_competition`(in in_role_guid bigint,in in_type int,in in_sql_cmd_str mediumtext)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
	START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM role_competition WHERE role_guid = in_role_guid AND have_type = in_type;
			IF exist_data_num > 0 THEN
				DELETE  FROM role_competition WHERE role_guid = in_role_guid AND have_type = in_type;
			END IF;
			SET @sql_cmd = concat('INSERT INTO `role_competition` VALUES ', in_sql_cmd_str, ';');
			PREPARE stmt FROM @sql_cmd;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_info_begin_time`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_info_begin_time`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_info_begin_time`(IN _server_id int, IN `_begin_time` int)
BEGIN
	UPDATE server_info SET begin_time = _begin_time WHERE big_group_id = _server_id div 10000 AND server_group_id =_server_id mod 10000;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_info_load`(_server_id int)
BEGIN
	if not exists (select big_group_id from `server_info` where big_group_id =_server_id div 10000 AND server_group_id =_server_id mod 10000) then
			INSERT INTO `server_info`(big_group_id,server_group_id,begin_time) 
					VALUES(_server_id div 10000,_server_id mod 10000, 2082729600);
	end if;
	SELECT * FROM server_info WHERE big_group_id =_server_id div 10000 AND server_group_id =_server_id mod 10000;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_info_need_begin_cross`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_info_need_begin_cross`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_info_need_begin_cross`(in _sql mediumtext)
BEGIN
 -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  START TRANSACTION;
		IF length(_sql) > 0 THEN
			SET @sql = concat(_sql,'');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_info_query_gm`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_info_query_gm`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_info_query_gm`()
BEGIN
	DECLARE acc_num INT;
	DECLARE role_num INT;
	SELECT count(*) INTO acc_num FROM (SELECT count(*) FROM `role_info` GROUP BY account) t;
	SELECT count(*) INTO role_num FROM `role_info`;
	SELECT acc_num, role_num;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_auction`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_auction`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_auction`(in in_main_db varchar(30))
BEGIN
	DECLARE s_tablename VARCHAR(100);   -- 变量名 s_tablename
	DECLARE cur_table_structure CURSOR  -- 游标名  cur_table_structure
	FOR
		SELECT table_name              -- 此为查询所有表 可以单独执行看结果
    FROM
		information_schema.TABLES
    WHERE
		table_schema = DATABASE ( )
    AND (table_name LIKE 'auction%' or table_name LIKE 'legion_info'
		or table_name LIKE 'legion_member_info'or table_name LIKE 'legion_applicant'
		or table_name LIKE 'legion_city_war_member_info'or table_name LIKE 'legion_event');

	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET s_tablename = NULL;    -- 此为固定写法 非null判断

	OPEN cur_table_structure;  -- 开启游标

	FETCH cur_table_structure INTO s_tablename;
	WHILE ( s_tablename IS NOT NULL) DO
		SET @MyQuery=CONCAT("INSERT INTO ",in_main_db,".`",s_tablename,"` SELECT * FROM ",DATABASE ( ),".",s_tablename,";"); 
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		SET @MyQuery=CONCAT("DELETE FROM ",s_tablename,";");
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		FETCH cur_table_structure INTO s_tablename;
	END WHILE; 
	CLOSE cur_table_structure; -- 关闭游标
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_clear_old_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_clear_old_role`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_clear_old_role`(in in_is_keep_old_recharged_role bool)
BEGIN
	DECLARE keeped_vip_point_value INTEGER DEFAULT 2147483647;
	DECLARE invalid_time BIGINT DEFAULT (UNIX_TIMESTAMP() - 60 * 24 * 3600);
	
	SELECT "sp_server_merge_clear_old_role begin";
	DROP TABLE IF EXISTS `server_merge_clear_role_guid`;
	CREATE TABLE `server_merge_clear_role_guid` (
		`role_guid` bigint(20) NOT NULL DEFAULT '0',
		PRIMARY KEY (`role_guid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	IF in_is_keep_old_recharged_role THEN
		SET keeped_vip_point_value = 1;
	END IF;

	INSERT INTO server_merge_clear_role_guid(role_guid) 
		SELECT a.role_guid FROM role_info a, role_time b WHERE 
			(a.role_guid = b.role_guid 
			AND a.e_role_info_recharge_num < keeped_vip_point_value 
			AND a.e_role_info_exp_level < 80 
			AND b.e_time_type_logout_time < invalid_time
			AND b.e_time_type_login_time < invalid_time);


	UPDATE role_info SET e_role_info_del_time = 1 WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);

	DELETE FROM auction_bid_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM auction_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM best_record_info WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	-- DELETE FROM cloud_shop_role_record WHERE player_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM cross_role_info WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM ranking_arena WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM ranking_arena_challenge_log WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM ranking_role_worship WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_achievement WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_active_degree WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_att WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_belief WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_boss_island WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_buff WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_convert WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_daily_must_do_count WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_demons_tower WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_element_heart WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_event WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_fight_att WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_first_time_do WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_gain_treasure WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_goods WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_guide_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_harry_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_history_highest_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_item WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_logic WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_mail WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_mail_by_id WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_mail_item WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_map_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_marry WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_mission WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_money WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_offline_chat WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_passive_skill WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_pokedex WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_ranking WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);

	DELETE FROM role_relation WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_relation WHERE relation_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);

	DELETE FROM role_service_goal WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_service_rank WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_service_rank_reward_state WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_seven_day_goal WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_show_info WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_skill WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_special_name WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_spirit WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_talent WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_time WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_time_limit_activity WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_tinder WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_title WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_treasure WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_welfare WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_welfare_random_get WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_worship_target WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM role_yesterday_must_do_remain WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);
	DELETE FROM ws_lucky_draw_record WHERE role_guid IN (SELECT role_guid FROM server_merge_clear_role_guid);



	DROP TABLE IF EXISTS `server_merge_clear_role_guid`;
	SELECT "sp_server_merge_clear_old_role end";
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_clear_up_legion`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_clear_up_legion`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_clear_up_legion`(in in_max_legion_per_server int)
BEGIN
	DECLARE data_index INT DEFAULT 0;
	DECLARE max_data_num INT DEFAULT 1;
	DECLARE temp_data BIGINT DEFAULT 0;
	DECLARE exist_data_num INT DEFAULT 0;

	SELECT "clear legion begin", in_max_legion_per_server;

	DROP TABLE IF EXISTS tmp_table;

	CREATE TEMPORARY TABLE tmp_table(
    SELECT legion_guid FROM legion_info 
		ORDER BY gs_value DESC LIMIT in_max_legion_per_server,99999999);


	SELECT COUNT(legion_guid) FROM tmp_table INTO max_data_num;
	WHILE data_index < max_data_num DO
		
		SELECT legion_guid FROM tmp_table ORDER BY legion_guid LIMIT data_index,1 INTO temp_data;

		SELECT COUNT(*) INTO exist_data_num FROM auction WHERE seller_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM auction WHERE seller_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM auction_record WHERE seller_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM auction_record WHERE seller_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_applicant WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_applicant WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_bonus_info WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_bonus_info WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_boss_record WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_boss_record WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_city_bid_info WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_city_bid_info WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_event WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_event WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_info WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_info WHERE legion_guid = temp_data;
		END IF;

		SELECT COUNT(*) INTO exist_data_num FROM legion_member_info WHERE legion_guid = temp_data;
		IF exist_data_num > 0 THEN
			DELETE FROM legion_member_info WHERE legion_guid = temp_data;
		END IF;

		SET data_index = data_index + 1;
	END WHILE;

	DROP TABLE IF EXISTS tmp_table;
	
	SELECT "clear legion end";
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_clear_up_role`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_clear_up_role`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_clear_up_role`(in max_role_per_account int)
BEGIN
	DECLARE max_data_num INT DEFAULT 0;
	DECLARE data_index INT DEFAULT 0;
	DECLARE temp_data MEDIUMTEXT DEFAULT '';
	DROP TABLE IF EXISTS tmp_table;
	SELECT "clear role begin";
	CREATE TEMPORARY TABLE tmp_table(
    SELECT account
    FROM role_info WHERE account IN
			(SELECT account FROM role_info GROUP BY account HAVING count(account) > max_role_per_account)
			AND 
			(e_role_info_del_time = 0));

	SELECT COUNT(*) FROM tmp_table INTO max_data_num;
	WHILE data_index < max_data_num DO
		SELECT account FROM tmp_table LIMIT data_index,1 INTO temp_data;
		CALL sp_server_merge_clear_up_role_one(max_role_per_account, temp_data);
		SET data_index = data_index + 1;
	END WHILE;

	DROP TABLE IF EXISTS tmp_table;
	SELECT "clear role end";

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_clear_up_role_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_clear_up_role_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_clear_up_role_one`(in max_role_per_account int, in process_account mediumtext)
BEGIN
	DECLARE max_data_num INT DEFAULT 0;
	-- SELECT "clear role one begin", max_role_per_account, process_account;
	DROP TABLE IF EXISTS del_one_role_temp_table;
	CREATE TEMPORARY TABLE del_one_role_temp_table(
    SELECT role_guid FROM role_info WHERE account = process_account AND e_role_info_del_time = 0
		ORDER BY e_role_info_exp_level DESC, e_role_i64_info_gs_value DESC LIMIT max_role_per_account,99999);

	SELECT COUNT(*) FROM del_one_role_temp_table INTO max_data_num;

	IF max_data_num > 0 THEN
		UPDATE role_info SET e_role_info_del_time = 1 WHERE role_guid IN 
			(SELECT role_guid FROM del_one_role_temp_table);
	END IF;
	
	DROP TABLE IF EXISTS del_one_role_temp_table;
	-- SELECT "clear role one end";
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_do_merge_process`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_do_merge_process`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_do_merge_process`()
BEGIN
	DECLARE max_legion_per_server INTEGER DEFAULT 999; 
	DECLARE max_role_per_account INTEGER DEFAULT 3; 
	DECLARE max_name_size INTEGER DEFAULT 101; 
	DECLARE normal_mail_id INTEGER DEFAULT 0; 
	DECLARE legion_del_mail_id INTEGER DEFAULT 0; 
	DECLARE role_name_postfix MEDIUMTEXT DEFAULT '#';
	DECLARE role_name_last MEDIUMTEXT DEFAULT '';
	DECLARE old_server_id INTEGER DEFAULT 99999;
	DECLARE new_server_id INTEGER DEFAULT 99999;

		SELECT e_role_info_server_id INTO old_server_id FROM sub_game_schema_1.role_info LIMIT 0,1;
		SELECT e_role_info_server_id INTO new_server_id FROM role_info LIMIT 0,1;
		SET role_name_postfix = concat(role_name_postfix,old_server_id,role_name_last);

		START TRANSACTION;
			CALL sp_server_merge_step_1(max_name_size, normal_mail_id, role_name_postfix, new_server_id);
			CALL sp_server_merge_step_2();
			CALL sp_server_merge_step_3(max_role_per_account, max_legion_per_server, legion_del_mail_id);
			CALL sp_server_merge_clear_old_role(true);
		COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_fix_name_and_clear_schema`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_fix_name_and_clear_schema`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_fix_name_and_clear_schema`(in max_name_size int, in `postfix` mediumtext, in new_server_id int)
BEGIN
	DECLARE source_len_max INT DEFAULT 0;
	DECLARE account_max INT DEFAULT 0;
	DECLARE sub_account_max INT DEFAULT 0;
	SET source_len_max = max_name_size - LENGTH(postfix);

	SELECT "change merge name begin";

-- check length

	UPDATE sub_game_schema_1.role_info SET e_role_info_server_id = new_server_id;
	UPDATE role_info SET e_role_info_server_id = new_server_id;

	UPDATE sub_game_schema_1.auction SET seller_name = LEFT(seller_name, source_len_max);
	UPDATE sub_game_schema_1.auction SET buyer_name = LEFT(buyer_name, source_len_max) WHERE LENGTH(buyer_name) > 0;

	UPDATE sub_game_schema_1.auction_record SET seller_name = LEFT(seller_name, source_len_max);
	UPDATE sub_game_schema_1.auction_record SET buyer_name = LEFT(buyer_name, source_len_max) WHERE LENGTH(buyer_name) > 0;
	
	UPDATE sub_game_schema_1.cross_ladder_ranking SET role_name = LEFT(role_name, source_len_max);
	UPDATE sub_game_schema_1.cross_role_info SET role_name = LEFT(role_name, source_len_max);

	UPDATE sub_game_schema_1.legion_applicant SET role_name = LEFT(role_name, source_len_max);
	UPDATE sub_game_schema_1.legion_boss_record SET killer_name = LEFT(killer_name, source_len_max);
	UPDATE sub_game_schema_1.legion_member_info SET role_name = LEFT(role_name, source_len_max);
	UPDATE sub_game_schema_1.role_info SET role_name = LEFT(role_name, source_len_max);

	UPDATE sub_game_schema_1.role_marry SET role_name = LEFT(role_name, source_len_max);
	UPDATE sub_game_schema_1.role_marry SET mate_name = LEFT(mate_name, source_len_max);

	-- UPDATE game_schema_1.role_ranking SET role_name = LEFT(role_name, source_len_max);
	-- UPDATE game_schema_1.role_ranking SET ERankingInfo_supporting_name = LEFT(ERankingInfo_supporting_name, source_len_max);
	
	UPDATE sub_game_schema_1.world_boss SET killer_name = LEFT(killer_name, source_len_max);
	UPDATE sub_game_schema_1.world_boss SET first_killer_name = LEFT(first_killer_name, source_len_max);

	UPDATE sub_game_schema_1.ws_lucky_draw_record SET role_name = LEFT(role_name, source_len_max);
	UPDATE sub_game_schema_1.legion_info SET legion_name = LEFT(legion_name, source_len_max);



-- do fix name

	UPDATE sub_game_schema_1.auction SET seller_name = concat(seller_name, postfix);
	UPDATE sub_game_schema_1.auction SET buyer_name = concat(buyer_name, postfix) WHERE LENGTH(buyer_name) > 0;

	UPDATE sub_game_schema_1.auction_record SET seller_name = concat(seller_name, postfix);
	UPDATE sub_game_schema_1.auction_record SET buyer_name = concat(buyer_name, postfix) WHERE LENGTH(buyer_name) > 0;

	UPDATE sub_game_schema_1.cloud_shop_role_record SET role_name = concat(role_name, postfix);
	
	UPDATE sub_game_schema_1.cross_ladder_ranking SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.cross_role_info SET role_name = concat(role_name, postfix);

	UPDATE sub_game_schema_1.legion_applicant SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.legion_boss_record SET killer_name = concat(killer_name, postfix);
	UPDATE sub_game_schema_1.legion_member_info SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.legion_warehouse_log SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.role_info SET role_name = concat(role_name, postfix);

	UPDATE sub_game_schema_1.role_marry SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.role_marry SET mate_name = concat(mate_name, postfix);
	
	UPDATE sub_game_schema_1.role_relation SET relation_name = concat(relation_name, postfix);

	-- UPDATE sub_game_schema_1.role_ranking SET role_name = concat(role_name, postfix);
	-- UPDATE sub_game_schema_1.role_ranking SET ERankingInfo_supporting_name = concat(ERankingInfo_supporting_name, postfix);
	
	UPDATE sub_game_schema_1.world_boss SET killer_name = concat(killer_name, postfix);
	UPDATE sub_game_schema_1.world_boss SET first_killer_name = concat(first_killer_name, postfix);

	UPDATE sub_game_schema_1.ws_lucky_draw_record SET role_name = concat(role_name, postfix);
	UPDATE sub_game_schema_1.legion_info SET legion_name = concat(legion_name, postfix);

	UPDATE sub_game_schema_1.gm_role_lock SET role_mark = LEFT(role_mark, source_len_max);

	-- UPDATE sub_game_schema_1.role_map_record SET e_map_record_info_fastest_time = 0;
	-- UPDATE sub_game_schema_1.role_map_record SET e_map_record_info_score_record = 0;
	
	SELECT "change merge name end";
	


	SELECT "clear table begin";
	DELETE FROM sub_game_schema_1.auction_share_list;
	DELETE FROM sub_game_schema_1.big_player;
	DELETE FROM sub_game_schema_1.best_record_info;
	DELETE FROM sub_game_schema_1.gain_treasure_record;
	DELETE FROM sub_game_schema_1.gm_role_lock;
	DELETE FROM sub_game_schema_1.ranking_arena;
	DELETE FROM sub_game_schema_1.ranking_arena_challenge_log;
	DELETE FROM sub_game_schema_1.red_package_info;
	DELETE FROM sub_game_schema_1.red_package_receiver_info;
	DELETE FROM sub_game_schema_1.role_ranking;
	DELETE FROM sub_game_schema_1.role_service_rank;
	DELETE FROM sub_game_schema_1.legion_city_bid_info;
	DELETE FROM sub_game_schema_1.legion_city_info;
	DELETE FROM sub_game_schema_1.mail_globel_mail;
	DELETE FROM sub_game_schema_1.mail_globel_mail_item;
	DELETE FROM sub_game_schema_1.server_refresh;
	DELETE FROM sub_game_schema_1.time_limit_activity_ws_info;
	DELETE FROM sub_game_schema_1.transfer_channel;
	DELETE FROM sub_game_schema_1.world_boss;
	DELETE FROM sub_game_schema_1.server_info;
	DELETE FROM sub_game_schema_1.other_server_mail_record;
	DELETE FROM sub_game_schema_1.croos_world_boss;
	DELETE FROM sub_game_schema_1.cross_server_harry_info;
	DELETE FROM sub_game_schema_1.cross_server_overload_city_info;
	DELETE FROM sub_game_schema_1.cross_server_pk_info;
	DELETE FROM sub_game_schema_1.cross_world_boss;
	DELETE FROM sub_game_schema_1.last_server_list;
	DELETE FROM sub_game_schema_1.ws_lucky_draw_record;
	-- DELETE FROM sub_game_schema_1.role_time_limit_activity;
	-- DELETE FROM role_time_limit_activity;
	DELETE FROM role_ranking;
	DELETE FROM croos_world_boss;
	DELETE FROM cross_server_harry_info;
	DELETE FROM cross_server_overload_city_info;
	DELETE FROM cross_server_pk_info;
	DELETE FROM cross_world_boss;
	DELETE FROM last_server_list;

	DELETE FROM sub_game_schema_1.role_relation WHERE e_relation_data_type = 3;
	DELETE FROM sub_game_schema_1.role_relation WHERE role_guid NOT IN(SELECT role_guid FROM sub_game_schema_1.role_info);
	DELETE FROM sub_game_schema_1.role_relation WHERE relation_guid NOT IN(SELECT role_guid FROM sub_game_schema_1.role_info);
	DELETE FROM sub_game_schema_1.legion_event WHERE legion_guid = 0;
	DELETE FROM sub_game_schema_1.account WHERE account IN (SELECT account FROM account);
	select max(id) into account_max from account;
	select max(id) into sub_account_max from sub_game_schema_1.account;
	update sub_game_schema_1.account set id = id + account_max + sub_account_max;
	DELETE FROM sub_game_schema_1.recharge_record WHERE order_id IN (SELECT order_id FROM recharge_record);
	DELETE FROM sub_game_schema_1.role_relation WHERE e_relation_data_type = 6;
	DELETE FROM sub_game_schema_1.time_limit_template WHERE activity_type IN(26,27,33,34,35,36,37,40,72);
	DELETE FROM sub_game_schema_1.time_limit_branch_template WHERE activity_type IN(26,27,33,34,35,36,37,40,72);
	UPDATE sub_game_schema_1.time_limit_template SET create_time = 0;

	SELECT "clear table end";
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_fix_time_limit_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_fix_time_limit_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_fix_time_limit_info`()
BEGIN
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_marry_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_marry_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_marry_info`(in in_main_db varchar(30))
BEGIN
		SET @MyQuery=CONCAT("INSERT INTO ",in_main_db,".`role_marry` SELECT * FROM ",DATABASE ( ),".role_marry;"); 
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		SET @MyQuery=CONCAT("DELETE FROM role_marry;");
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_send_compensate_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_send_compensate_mail`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_send_compensate_mail`(in in_mail_id int)
BEGIN
	
	DECLARE data_index INT DEFAULT 0;
	DECLARE max_data_num INT DEFAULT 1;
	DECLARE exist_data_num INT DEFAULT 0;
	DECLARE temp_data BIGINT DEFAULT 0;
	SELECT "send mail begin";
	SELECT COUNT(role_guid) FROM sub_game_schema_1.role_info INTO max_data_num;
	WHILE data_index < max_data_num DO

		SELECT role_guid FROM sub_game_schema_1.role_info ORDER BY role_guid LIMIT data_index,1 INTO temp_data;

		SELECT COUNT(*) INTO exist_data_num FROM sub_game_schema_1.role_mail_by_id 
			WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;

		IF exist_data_num > 0 THEN
			DELETE FROM sub_game_schema_1.role_mail_by_id WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;
		END IF;

		INSERT INTO sub_game_schema_1.role_mail_by_id VALUES 
			(temp_data, UNIX_TIMESTAMP() * 1000, 0, in_mail_id);

		SET data_index = data_index + 1;
	END WHILE;
	SELECT "send mail end";

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_send_legion_del_compensate_mail`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_send_legion_del_compensate_mail`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_send_legion_del_compensate_mail`(in in_max_legion_per_server int, in in_mail_id int)
BEGIN
	DECLARE data_index INT DEFAULT 0;
	DECLARE max_data_num INT DEFAULT 1;
	DECLARE temp_data BIGINT DEFAULT 0;
	SELECT "send legion mail begin", in_max_legion_per_server, in_mail_id;
	DROP TABLE IF EXISTS tmp_table;

	CREATE TEMPORARY TABLE tmp_table(
    SELECT legion_guid FROM legion_info 
		ORDER BY gs_value DESC LIMIT in_max_legion_per_server,99999999);


	SELECT COUNT(legion_guid) FROM tmp_table INTO max_data_num;
	WHILE data_index < max_data_num DO
		SELECT legion_guid FROM tmp_table ORDER BY legion_guid LIMIT data_index,1 INTO temp_data;
		CALL sp_server_merge_send_legion_del_compensate_mail_one(temp_data, in_mail_id);
		SET data_index = data_index + 1;
	END WHILE;

	DROP TABLE IF EXISTS tmp_table;
	SELECT "send legion mail end";
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_send_legion_del_compensate_mail_one`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_send_legion_del_compensate_mail_one`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_send_legion_del_compensate_mail_one`(in in_legion_guid bigint, in in_mail_id int)
BEGIN
	
	DECLARE data_index INT DEFAULT 0;
	DECLARE max_data_num INT DEFAULT 1;
	DECLARE exist_data_num INT DEFAULT 0;
	DECLARE temp_data BIGINT DEFAULT 0;

	SELECT "send legion mail one begin", in_legion_guid, in_mail_id;
	SELECT COUNT(role_guid) FROM legion_member_info 
		WHERE legion_guid = in_legion_guid INTO max_data_num;
		
	WHILE data_index < max_data_num DO
			
		SELECT role_guid FROM legion_member_info WHERE legion_guid = in_legion_guid  ORDER BY role_guid LIMIT data_index,1 INTO temp_data;

		SELECT COUNT(*) INTO exist_data_num FROM role_mail_by_id 
			WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;

		IF exist_data_num > 0 THEN
			DELETE FROM role_mail_by_id WHERE role_guid = temp_data AND create_time = UNIX_TIMESTAMP() * 1000 AND mail_id = in_mail_id ;
		END IF;

		INSERT INTO role_mail_by_id VALUES 
			(temp_data, UNIX_TIMESTAMP() * 1000, 0, in_mail_id);

		SET data_index = data_index + 1;

	END WHILE;
	SELECT "send legion mail one end";

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_step_1`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_step_1`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_step_1`(in max_name_size int, in compensate_mail_id int, in `postfix` mediumtext, in new_server_id int)
BEGIN

	CALL sp_server_merge_fix_time_limit_info();
	CALL sp_server_merge_fix_name_and_clear_schema(max_name_size, postfix, new_server_id);
	CALL sp_server_merge_send_compensate_mail(compensate_mail_id);
	
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_step_2`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_step_2`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_step_2`()
BEGIN
	DECLARE s_tablename VARCHAR(100);   -- 变量名 s_tablename
	DECLARE cur_table_structure CURSOR  -- 游标名  cur_table_structure
	FOR
		SELECT table_name              -- 此为查询所有表 可以单独执行看结果
    FROM
		information_schema.TABLES
    WHERE
		table_schema = DATABASE ( )
    AND table_name NOT LIKE 'ACT%'
    AND table_name NOT LIKE 'QRTZ%';

	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET s_tablename = NULL;    -- 此为固定写法 非null判断

	OPEN cur_table_structure;  -- 开启游标

	FETCH cur_table_structure INTO s_tablename;
	WHILE ( s_tablename IS NOT NULL) DO
		SELECT CONCAT("insert ",s_tablename," begin");
		SET @MyQuery=CONCAT("INSERT INTO `",s_tablename,"` SELECT * FROM sub_game_schema_1.",s_tablename); 
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		SELECT CONCAT("insert ",s_tablename," end");
		FETCH cur_table_structure INTO s_tablename;
	END WHILE; 
	CLOSE cur_table_structure; -- 关闭游标
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_step_3`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_step_3`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_step_3`(in max_role_per_account int, in max_legion_per_server int, in legion_del_compensate_mail_id int)
BEGIN

	CALL sp_server_merge_clear_up_role
		(max_role_per_account);

	CALL sp_server_merge_send_legion_del_compensate_mail
		(max_legion_per_server, legion_del_compensate_mail_id);

	CALL sp_server_merge_clear_up_legion
		(max_legion_per_server);
		
	UPDATE server_info SET begin_first_merge_server_time = UNIX_TIMESTAMP(NOW()) WHERE begin_first_merge_server_time = 0 OR ISNULL(begin_first_merge_server_time);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_merge_time_limit_temp`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_merge_time_limit_temp`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_merge_time_limit_temp`(in in_main_db varchar(30))
BEGIN
		DELETE FROM time_limit_template WHERE activity_type IN(26,27,33,34,35,36,37,40,72);
		DELETE FROM time_limit_branch_template WHERE activity_type IN(26,27,33,34,35,36,37,40,72);
		UPDATE time_limit_template SET create_time = 0;
		SET @MyQuery=CONCAT("INSERT INTO ",in_main_db,".`time_limit_template` SELECT * FROM ",DATABASE ( ),".time_limit_template;"); 
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		SET @MyQuery=CONCAT("DELETE FROM time_limit_template;");
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;

		SET @MyQuery=CONCAT("INSERT INTO ",in_main_db,".`time_limit_branch_template` SELECT * FROM ",DATABASE ( ),".time_limit_branch_template;"); 
		-- 业务逻辑 拼接字符串然后执行
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
		SET @MyQuery=CONCAT("DELETE FROM time_limit_branch_template;");
		PREPARE msql FROM @MyQuery;
		EXECUTE msql ;#USING @c;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_refresh_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_refresh_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_refresh_load`()
BEGIN
	SELECT * FROM `server_refresh`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_server_refresh_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_server_refresh_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_server_refresh_save`(in in_time_type int , in in_sql_cmd_str mediumtext)
BEGIN
	-- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE exist_data_num INT DEFAULT 0;
    START TRANSACTION;
        IF length(in_sql_cmd_str) > 0 THEN
						SELECT COUNT(*) INTO exist_data_num FROM server_refresh WHERE `time_type` = in_time_type ;
						IF exist_data_num > 0 THEN
								DELETE FROM server_refresh WHERE `time_type` = in_time_type ;
						END IF;

            SET @sql_cmd = concat('INSERT INTO `server_refresh` VALUES ', in_sql_cmd_str, ';');
            PREPARE stmt FROM @sql_cmd;
            EXECUTE stmt;
            DEALLOCATE PREPARE stmt;
        END IF;
    COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_service_ranking_save_one_list`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_service_ranking_save_one_list`;
DELIMITER ;;
CREATE PROCEDURE `sp_service_ranking_save_one_list`(in `in_ranking_type` bigint, in _sql mediumtext)
BEGIN

  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_service_rank` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_service_rank WHERE rank_type = `in_ranking_type`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_service_rank WHERE rank_type = `in_ranking_type`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_set_all_player_check_guid`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_set_all_player_check_guid`;
DELIMITER ;;
CREATE PROCEDURE `sp_set_all_player_check_guid`()
BEGIN
	UPDATE role_info SET e_role_info_need_check_guide = 1;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_subscribe_daily_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_subscribe_daily_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_subscribe_daily_info_load`(in in_role_id bigint)
BEGIN
	SELECT * from `role_subscribe_daily_info` WHERE `role_guid`=in_role_id;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_subscribe_daily_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_subscribe_daily_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_subscribe_daily_info_save`(in `in_role_guid` bigint,in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `role_subscribe_daily_info` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM role_subscribe_daily_info WHERE role_guid = `in_role_guid`;
		IF exist_data_num > 0 THEN
				DELETE FROM role_subscribe_daily_info WHERE role_guid = `in_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_feed_back_info_clear`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_feed_back_info_clear`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_feed_back_info_clear`(IN `_role_guid` bigint)
BEGIN
	DELETE FROM time_feed_back_info WHERE role_guid = `_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_feed_back_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_feed_back_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_feed_back_info_load`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM time_feed_back_info WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_feed_back_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_feed_back_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_feed_back_info_save`(IN `_role_guid` bigint,IN`_is_first` int,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `time_feed_back_info` values ';

	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		START TRANSACTION;
		IF _is_first = 0 THEN
				DELETE FROM time_feed_back_info WHERE role_guid = `_role_guid`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_branch_temp_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_branch_temp_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_branch_temp_delete`(in `in_act_idex` int)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	SELECT COUNT(*) INTO exist_data_num FROM time_limit_branch_template WHERE act_guid = `in_act_idex` ;
	IF exist_data_num > 0 THEN
	DELETE FROM time_limit_branch_template WHERE act_guid = `in_act_idex` ;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_branch_temp_load_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_branch_temp_load_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_branch_temp_load_info`(in in_row int, 
  in in_load_max int)
BEGIN
  select * from time_limit_branch_template LIMIT in_row, in_load_max;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_branch_temp_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_branch_temp_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_branch_temp_save`(in `in_act_idex` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `time_limit_branch_template` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM time_limit_branch_template WHERE act_guid = `in_act_idex` ;
		IF exist_data_num > 0 THEN
		DELETE FROM time_limit_branch_template WHERE act_guid = `in_act_idex` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_gift_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_gift_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_gift_info_load`(IN `_role_guid` bigint)
BEGIN
	SELECT * FROM time_limit_gift_info WHERE role_guid = _role_guid;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_gift_info_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_gift_info_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_gift_info_save`(IN `_role_guid` bigint,IN`_main_id` int,in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `time_limit_gift_info` values ';

	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		START TRANSACTION;
		SELECT COUNT(*) INTO exist_data_num FROM time_limit_gift_info WHERE role_guid = `_role_guid` AND template_id = `_main_id`;
		IF exist_data_num > 0 THEN
			DELETE FROM time_limit_gift_info WHERE role_guid = `_role_guid` AND template_id = `_main_id`;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');	
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_temp_delete`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_temp_delete`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_temp_delete`(in `in_act_idex` int)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	SELECT COUNT(*) INTO exist_data_num FROM time_limit_template WHERE act_guid = `in_act_idex` ;
	IF exist_data_num > 0 THEN
	DELETE FROM time_limit_template WHERE act_guid = `in_act_idex` ;
	END IF;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_temp_load_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_temp_load_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_temp_load_info`(in in_row int, 
  in in_load_max int)
BEGIN
  select * from time_limit_template LIMIT in_row, in_load_max;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_temp_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_temp_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_temp_save`(in `in_act_idex` int, in _sql mediumtext)
BEGIN
	DECLARE _insert_sql mediumtext DEFAULT 'insert into `time_limit_template` values ';
	DECLARE exist_data_num INT DEFAULT 0;
  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;

		SELECT COUNT(*) INTO exist_data_num FROM time_limit_template WHERE act_guid = `in_act_idex` ;
		IF exist_data_num > 0 THEN
		DELETE FROM time_limit_template WHERE act_guid = `in_act_idex` ;
		END IF;

		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_ws_activity_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_ws_activity_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_ws_activity_load`()
BEGIN
	SELECT * from `time_limit_activity_ws_info`ORDER BY act_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_time_limit_ws_activity_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_time_limit_ws_activity_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_time_limit_ws_activity_save`(in in_act_type int,
	in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `time_limit_activity_ws_info` values ';
  DECLARE exist_data_num INT DEFAULT 0;

  START TRANSACTION;
    SELECT COUNT(*) INTO exist_data_num FROM `time_limit_activity_ws_info` WHERE `act_type`=in_act_type  ;
    IF exist_data_num > 0 THEN
        DELETE FROM `time_limit_activity_ws_info` WHERE `act_type`=in_act_type;
    END IF;

    IF length(_sql)>0 THEN
      SET @sql = concat(_insert_sql,_sql,';');
      PREPARE stmt FROM @sql;
      EXECUTE stmt;
      DEALLOCATE PREPARE stmt;
    END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_transfer_append_channel`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_transfer_append_channel`;
DELIMITER ;;
CREATE PROCEDURE `sp_transfer_append_channel`(IN `_obj_type` tinyint,IN `_obj_id` bigint,IN `_ordinal` int,IN `_data` mediumtext)
BEGIN
	INSERT INTO `transfer_channel` VALUES(_obj_type,_obj_id,_ordinal,_data);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_transfer_commit`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_transfer_commit`;
DELIMITER ;;
CREATE PROCEDURE `sp_transfer_commit`(IN `_obj_type` tinyint,IN `_obj_id` bigint,IN `_preprocess` mediumtext)
BEGIN
DECLARE _ret INTEGER DEFAULT 0;
  DECLARE done INTEGER DEFAULT 0;
  DECLARE _ordinal INTEGER UNSIGNED DEFAULT 0;
  DECLARE _obj_data MEDIUMTEXT;
  DECLARE _start_pos INTEGER UNSIGNED DEFAULT 1;
  DECLARE _comma_pos INTEGER UNSIGNED DEFAULT 0;
  DECLARE _pre_one VARCHAR(512);
  DECLARE _pre_one_length INTEGER UNSIGNED DEFAULT 0;

  DECLARE cur CURSOR FOR SELECT `ordinal`,`data` FROM `transfer_channel`
    WHERE `obj_type`=_obj_type AND `obj_id`=_obj_id ORDER BY `ordinal` ASC;
  DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = 1;

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
  BEGIN
    ROLLBACK;
    SET _ret = -1;
    SELECT _ret,_ordinal;
  END;

  START TRANSACTION;

  SET _comma_pos = LOCATE(';',_preprocess, _start_pos);
  preloop: WHILE (_comma_pos<>0) DO
    SET _pre_one_length = _comma_pos - _start_pos + 1;
    SET _pre_one = SUBSTRING(_preprocess, _start_pos, _pre_one_length);

    SET @SQL = _pre_one;
    PREPARE stmt FROM @SQL;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;

    SET _start_pos = _comma_pos +1;
    SET _comma_pos = LOCATE(';',_preprocess, _start_pos);
  END WHILE preloop;

  OPEN cur;
  curloop: WHILE (NOT done) DO
    FETCH cur INTO _ordinal,_obj_data;
    IF done THEN
      LEAVE curloop;
    END IF;
    SET @SQL = _obj_data;
    PREPARE stmt FROM @SQL;
    EXECUTE stmt;
    DEALLOCATE PREPARE stmt;
  END WHILE curloop;

  CLOSE cur;

  DELETE FROM `transfer_channel` WHERE `obj_type`=_obj_type AND `obj_id`=_obj_id;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_transfer_in_begin`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_transfer_in_begin`;
DELIMITER ;;
CREATE PROCEDURE `sp_transfer_in_begin`(IN `_obj_type` tinyint,IN `_obj_id` bigint)
BEGIN
	DECLARE exist_data_num INT DEFAULT 0;

	SELECT COUNT(*) INTO exist_data_num FROM transfer_channel WHERE `obj_type`=_obj_type AND `obj_id`=_obj_id;
	IF exist_data_num > 0 THEN
			DELETE FROM `transfer_channel` WHERE `obj_type`=_obj_type AND `obj_id`=_obj_id;
	END IF;

END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_update_role_time_limit_info`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_update_role_time_limit_info`;
DELIMITER ;;
CREATE PROCEDURE `sp_update_role_time_limit_info`(IN `_act_guid` int,IN `_act_type` int)
BEGIN
	UPDATE role_time_limit_activity SET act_guid = _act_guid where type = _act_type;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_USER_DELETE_USER`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_USER_DELETE_USER`;
DELIMITER ;;
CREATE PROCEDURE `SP_USER_DELETE_USER`(
	IN _account_id INTEGER UNSIGNED
)
BEGIN
  DECLARE _ret TINYINT DEFAULT 0;
  DECLARE _count INTEGER UNSIGNED DEFAULT 0;
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
      ROLLBACK;
      SET _ret = 0;
      SELECT _ret;
    END;

  START TRANSACTION;
  SELECT COUNT(1) INTO _count FROM `low_account`.`account` WHERE accountid=_account_id;
  IF ( _count <> 1 ) THEN
    SET _ret = 0;
  ELSE
    DELETE FROM `low_account`.`account` WHERE accountid=_account_id;
    SET _ret = 1;
  END IF;
  COMMIT;
  SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_USER_LOGIN`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_USER_LOGIN`;
DELIMITER ;;
CREATE PROCEDURE `SP_USER_LOGIN`(
	in name varchar(64),
	in word varchar(64)
)
BEGIN
  select accountid,username,groupid from `low_account`.`account` where username = name and password = word;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_USER_REGISTER`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_USER_REGISTER`;
DELIMITER ;;
CREATE PROCEDURE `SP_USER_REGISTER`(
	in _account_name varchar(100),
	in _password varchar(100),
  in _group_id int(10)
)
BEGIN
  DECLARE _ret TINYINT DEFAULT 0;
  DECLARE _count INTEGER UNSIGNED DEFAULT 0;

  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
      ROLLBACK;
      SET _ret = -1;
      SELECT _ret;
    END;
  START TRANSACTION;
  SELECT COUNT(1) INTO _count FROM `low_account`.`account` WHERE username=_account_name;
  IF ( _count > 0 ) THEN
    SET _ret = 0;
  ELSE
    INSERT INTO `low_account`.`account`(username,password,groupid) VALUES(_account_name,_password,_group_id);
    SET _ret = 1;
  END IF;
  COMMIT;
  SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_USER_UPDATE_GROUP_ID`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_USER_UPDATE_GROUP_ID`;
DELIMITER ;;
CREATE PROCEDURE `SP_USER_UPDATE_GROUP_ID`(
	IN _account_id INTEGER UNSIGNED,
	IN _old_group_id INTEGER UNSIGNED,
	IN _new_group_id INTEGER UNSIGNED
)
BEGIN
  DECLARE _ret TINYINT DEFAULT 0;
  DECLARE _count INTEGER UNSIGNED DEFAULT 0;
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
      ROLLBACK;
      SET _ret = 0;
      SELECT _ret;
    END;

  START TRANSACTION;
  SELECT COUNT(1) INTO _count FROM `low_account`.`account` WHERE accountid=_account_id AND groupid=_old_group_id;
  IF ( _count <> 1 ) THEN
    SET _ret = 0;
  ELSE
    UPDATE `low_account`.`account` SET groupid=_new_group_id WHERE accountid=_account_id AND groupid=_old_group_id;
    SET _ret = 1;
  END IF;
  COMMIT;
  SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `SP_USER_UPDATE_PWD`
-- ----------------------------
DROP PROCEDURE IF EXISTS `SP_USER_UPDATE_PWD`;
DELIMITER ;;
CREATE PROCEDURE `SP_USER_UPDATE_PWD`(
	in _account_name varchar(100),
	in _old_password varchar(100),
	in _new_password varchar(100)
)
BEGIN
  DECLARE _ret TINYINT DEFAULT 0;
  DECLARE _count INTEGER UNSIGNED DEFAULT 0;
  
  DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
      ROLLBACK;
      SET _ret = 0;
      SELECT _ret;
    END;

  START TRANSACTION;
  SELECT COUNT(1) INTO _count FROM `low_account`.`account` WHERE username=_account_name AND password=_old_password;
  IF ( _count <> 1 ) THEN
    SET _ret = 0;
  ELSE
    UPDATE `low_account`.`account` SET password=_new_password WHERE username=_account_name AND password=_old_password;
    SET _ret = 1;
  END IF;
  COMMIT;
  SELECT _ret;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_world_boss_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_world_boss_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_world_boss_load`()
BEGIN
	SELECT * FROM `world_boss`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_world_boss_save`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_world_boss_save`;
DELIMITER ;;
CREATE PROCEDURE `sp_world_boss_save`(in _sql mediumtext)
BEGIN
  DECLARE _insert_sql mediumtext DEFAULT 'insert into `world_boss` values ';

  -- DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
  
  START TRANSACTION;
		-- DELETE FROM `server_mail` WHERE mail_guid = `in_mail_guid`;
		DELETE FROM world_boss;
		
		IF length(_sql)>0 THEN
			SET @sql = concat(_insert_sql,_sql,';');
			PREPARE stmt FROM @sql;
			EXECUTE stmt;
			DEALLOCATE PREPARE stmt;
		END IF;
  COMMIT;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ws_big_player_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ws_big_player_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_ws_big_player_info_load`(in `in_role_guid` bigint)
BEGIN
	SELECT * FROM `role_info` WHERE role_guid = `in_role_guid`;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_ws_role_info_load`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_ws_role_info_load`;
DELIMITER ;;
CREATE PROCEDURE `sp_ws_role_info_load`(in `in_role_guid` bigint)
BEGIN
	DECLARE _account VARCHAR(100);
	DECLARE _role_name VARCHAR(100);
	DECLARE _role_guid BIGINT;
	DECLARE _login_lock INT;
	DECLARE _send_chat INT;
	DECLARE _worship_value INT;
	DECLARE _role_ws_guid BIGINT;
	DECLARE _save_time INT;
	DECLARE _red_package_free_count INT;
	DECLARE _end_login_lock INT;
	DECLARE _end_send_chat INT;
	SELECT account, role_name, role_guid INTO _account, _role_name, _role_guid FROM role_info WHERE role_guid = `in_role_guid`;
	
	SET _end_login_lock = 0;
	SET _end_send_chat = 0;

	IF  ISNULL(_account) = FALSE THEN
		SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = _account;
	END IF;
	IF _login_lock = 1 THEN
			set _end_login_lock = 1;
	END IF;
	IF _send_chat > 0 THEN
			set _end_send_chat = _send_chat;
	END IF;
	
	IF  ISNULL(_role_name) = FALSE THEN
		SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = _role_name;
	END IF;
	IF _login_lock = 1 THEN
			set _end_login_lock = 1;
	END IF;
	IF _send_chat > 0 THEN
			set _end_send_chat = _send_chat;
	END IF;

	IF  ISNULL(_role_guid) = FALSE THEN
		SELECT is_login, is_send_chat INTO _login_lock, _send_chat FROM gm_role_lock WHERE role_mark = CONCAT(_role_guid);
	END IF;
	IF _login_lock = 1 THEN
			set _end_login_lock = 1;
	END IF;
	IF _send_chat > 0 THEN
			set _end_send_chat = _send_chat;
	END IF;

	SELECT worship INTO _worship_value FROM ranking_role_worship WHERE role_guid = `in_role_guid`;
	IF ISNULL(_worship_value) THEN
		SET _worship_value = 0;
	END IF;
	SET _save_time = 0;
	SET _red_package_free_count = 0;
	SELECT a.*, _save_time, _red_package_free_count ,_end_login_lock, _end_send_chat,_worship_value FROM `role_info` a WHERE a.role_guid = `in_role_guid`;
END
;;
DELIMITER ;
