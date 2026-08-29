/*
Navicat MySQL Data Transfer

Source Server         : localhost_3306
Source Server Version : 50628
Source Host           : localhost:3306
Source Database       : low_log_schema

Target Server Type    : MYSQL
Target Server Version : 50628
File Encoding         : 65001

Date: 2019-11-12 21:17:59
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `account_create_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `account_create_log` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of account_create_log
-- ----------------------------

-- ----------------------------
-- Table structure for `auction_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `auction_log` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT '0',
  `log_time` datetime NOT NULL,
  `seller_account_id` varchar(101) DEFAULT '',
  `seller_role_guid` bigint(20) DEFAULT '0',
  `seller_role_name` varchar(101) DEFAULT '',
  `seller_role_level` int(11) DEFAULT '0',
  `item_guid` bigint(20) NOT NULL DEFAULT '0',
  `item_id` int(11) DEFAULT '0',
  `item_num` int(11) DEFAULT '0',
  `auction_type` int(11) DEFAULT '0',
  `buyer_account_id` varchar(101) DEFAULT '',
  `buyer_role_guid` bigint(20) DEFAULT '0',
  `buyer_role_name` varchar(101) DEFAULT '',
  `buyer_role_level` int(11) DEFAULT '0',
  `money_type` int(11) DEFAULT '0',
  `money_value` int(11) DEFAULT '0',
  `auction_state` int(11) DEFAULT '0',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`seller_account_id`),
  KEY `log_time` (`log_time`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of auction_log
-- ----------------------------

-- ----------------------------
-- Table structure for `boss_kill_drop_log`
-- ----------------------------
DROP TABLE IF EXISTS `boss_kill_drop_log`;
CREATE TABLE `boss_kill_drop_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `boss_id` int(11) DEFAULT NULL,
  `drop_item_id1` int(11) DEFAULT NULL,
  `drop_item_num1` int(11) DEFAULT NULL,
  `drop_item_id2` int(11) DEFAULT NULL,
  `drop_item_num2` int(11) DEFAULT NULL,
  `drop_item_id3` int(11) DEFAULT NULL,
  `drop_item_num3` int(11) DEFAULT NULL,
  `drop_item_id4` int(11) DEFAULT NULL,
  `drop_item_num4` int(11) DEFAULT NULL,
  `drop_item_id5` int(11) DEFAULT NULL,
  `drop_item_num5` int(11) DEFAULT NULL,
  `drop_item_id6` int(11) DEFAULT NULL,
  `drop_item_num6` int(11) DEFAULT NULL,
  `drop_item_id7` int(11) DEFAULT NULL,
  `drop_item_num7` int(11) DEFAULT NULL,
  `drop_item_id8` int(11) DEFAULT NULL,
  `drop_item_num8` int(11) DEFAULT NULL,
  `drop_item_id9` int(11) DEFAULT NULL,
  `drop_item_num9` int(11) DEFAULT NULL,
  `drop_item_id10` int(11) DEFAULT NULL,
  `drop_item_num10` int(11) DEFAULT NULL,
  `drop_item_id11` int(11) DEFAULT NULL,
  `drop_item_num11` int(11) DEFAULT NULL,
  `drop_item_id12` int(11) DEFAULT NULL,
  `drop_item_num12` int(11) DEFAULT NULL,
  `drop_item_id13` int(11) DEFAULT NULL,
  `drop_item_num13` int(11) DEFAULT NULL,
  `drop_item_id14` int(11) DEFAULT NULL,
  `drop_item_num14` int(11) DEFAULT NULL,
  `drop_item_id15` int(11) DEFAULT NULL,
  `drop_item_num15` int(11) DEFAULT NULL,
  `drop_item_id16` int(11) DEFAULT NULL,
  `drop_item_num16` int(11) DEFAULT NULL,
  `drop_item_id17` int(11) DEFAULT NULL,
  `drop_item_num17` int(11) DEFAULT NULL,
  `drop_item_id18` int(11) DEFAULT NULL,
  `drop_item_num18` int(11) DEFAULT NULL,
  `drop_item_id19` int(11) DEFAULT NULL,
  `drop_item_num19` int(11) DEFAULT NULL,
  `drop_item_id20` int(11) DEFAULT NULL,
  `drop_item_num20` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT NULL,
  `appid` varchar(50) DEFAULT NULL,
  `version` varchar(50) DEFAULT NULL,
  `stepnumid` varchar(200) DEFAULT NULL,
  `normversion` varchar(50) DEFAULT NULL,
  `gamechannel` varchar(50) DEFAULT NULL,
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(50) DEFAULT NULL,
  `cross_service` varchar(50) DEFAULT NULL,
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of boss_kill_drop_log
-- ----------------------------

-- ----------------------------
-- Table structure for `boss_kill_log`
-- ----------------------------
DROP TABLE IF EXISTS `boss_kill_log`;
CREATE TABLE `boss_kill_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `boss_id` int(11) DEFAULT NULL,
  `map_id` int(11) DEFAULT NULL,
  `player_num` int(11) DEFAULT NULL,
  `battle_begin_time` datetime NOT NULL,
  `battle_end_time` datetime NOT NULL,
  `battle_continued_time` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT NULL,
  `appid` varchar(50) DEFAULT NULL,
  `version` varchar(50) DEFAULT NULL,
  `stepnumid` varchar(200) DEFAULT NULL,
  `normversion` varchar(50) DEFAULT NULL,
  `gamechannel` varchar(50) DEFAULT NULL,
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(50) DEFAULT NULL,
  `cross_service` varchar(50) DEFAULT NULL,
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of boss_kill_log
-- ----------------------------

-- ----------------------------
-- Table structure for `conf_area`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `conf_area` (
  `confID` int(11) NOT NULL AUTO_INCREMENT,
  `areaID` varchar(32) DEFAULT '',
  `areaName` varchar(32) DEFAULT '',
  `startTime` datetime DEFAULT NULL,
  PRIMARY KEY (`confID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of conf_area
-- ----------------------------
INSERT INTO `conf_area` VALUES ('1', '11003', '11003',now());

-- ----------------------------
-- Table structure for `conf_module`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `conf_module` (
  `confID` int(11) NOT NULL AUTO_INCREMENT,
  `moduleID` varchar(32) DEFAULT NULL,
  `moduleName` varchar(32) DEFAULT '',
  `type` int(11) DEFAULT NULL,
  PRIMARY KEY (`confID`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of conf_module
-- ----------------------------

-- ----------------------------
-- Table structure for `conf_userinfo`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `conf_userinfo` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `userName` varchar(32) DEFAULT '',
  `password` varchar(32) DEFAULT '',
  `userCode` varchar(32) DEFAULT '',
  `moduleIDs` varchar(1024) DEFAULT '',
  `buttonIDs` varchar(1024) DEFAULT NULL,
  PRIMARY KEY (`userID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of conf_userinfo
-- ----------------------------
INSERT INTO `conf_userinfo` VALUES ('1', 'faith', 'faith', 'faith', 'adminAreaInfo,adminModule,adminUser,logout,register,guide,levelChange,task,moneyChange,recharge,playerCount,dayDevicesAdd,dayPlayersAdd,dayActives,stayPercent,levelUpAvg,levelStay,newGuidePass,dayTimeCost,dayPlayCount,dayEndLevel','');

-- ----------------------------
-- Table structure for `create_role_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `create_role_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `career` varchar(64) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(32) DEFAULT NULL,
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `account_id` (`account_id`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of create_role_log
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_server_boss_kill_log`
-- ----------------------------
DROP TABLE IF EXISTS `cross_server_boss_kill_log`;
CREATE TABLE `cross_server_boss_kill_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `boss_id` int(11) DEFAULT NULL,
  `kill_legion_id` varchar(64) DEFAULT NULL,
  `kill_legion_name` varchar(64) DEFAULT NULL,
  `top1_id` bigint(11) DEFAULT NULL,
  `top1_name` varchar(64) DEFAULT NULL,
  `top1_server_id` int(11) DEFAULT NULL,
  `top2_id` bigint(11) DEFAULT NULL,
  `top2_name` varchar(64) DEFAULT NULL,
  `top2_server_id` int(11) DEFAULT NULL,
  `top3_id` bigint(11) DEFAULT NULL,
  `top3_name` varchar(64) DEFAULT NULL,
  `top3_server_id` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT NULL,
  `appid` varchar(50) DEFAULT NULL,
  `version` varchar(50) DEFAULT NULL,
  `stepnumid` varchar(200) DEFAULT NULL,
  `normversion` varchar(50) DEFAULT NULL,
  `gamechannel` varchar(50) DEFAULT NULL,
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(50) DEFAULT NULL,
  `cross_service` varchar(50) DEFAULT NULL,
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`)
) ENGINE=InnoDB AUTO_INCREMENT=1410 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_server_boss_kill_log
-- ----------------------------

-- ----------------------------
-- Table structure for `cross_server_harry_log`
-- ----------------------------
DROP TABLE IF EXISTS `cross_server_harry_log`;
CREATE TABLE `cross_server_harry_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `oper_type` int(11) DEFAULT NULL,
  `harry_type` int(11) DEFAULT NULL,
  `harry_server_id` varchar(64) DEFAULT NULL,
  `normal_harry_count` int(11) DEFAULT NULL,
  `sepcail_harry_count` int(11) DEFAULT NULL,
  `login_type` int(50) DEFAULT NULL,
  `appid` varchar(50) DEFAULT NULL,
  `version` varchar(50) DEFAULT NULL,
  `stepnumid` varchar(200) DEFAULT NULL,
  `normversion` varchar(50) DEFAULT NULL,
  `gamechannel` varchar(50) DEFAULT NULL,
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(50) DEFAULT NULL,
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cross_server_harry_log
-- ----------------------------

-- ----------------------------
-- Table structure for `cycle_statistics`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `cycle_statistics` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `cycle_week_month` varchar(64) DEFAULT NULL,
  `active_number` bigint(11) DEFAULT NULL,
  `pay_number` bigint(11) DEFAULT NULL,
  `rate` decimal(14,2) DEFAULT NULL,
  `pay_amount` decimal(14,2) DEFAULT NULL,
  `arpu` decimal(14,2) DEFAULT NULL,
  `arppu` decimal(14,2) DEFAULT NULL,
  `daily_register` bigint(11) DEFAULT NULL,
  `new_reg_rec_num` bigint(14) DEFAULT NULL,
  `new_reg_rate` decimal(14,2) DEFAULT NULL,
  `new_reg_rec_mon` decimal(14,2) DEFAULT NULL,
  `cycle_type` int(11) DEFAULT NULL,
  `channel_id` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of cycle_statistics
-- ----------------------------

-- ----------------------------
-- Table structure for `daily_statistics`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `daily_statistics` (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime DEFAULT NULL,
  `daily_register` int(11) DEFAULT NULL,
  `active_number` int(11) DEFAULT NULL,
  `pay_amount` decimal(14,2) DEFAULT NULL,
  `pay_number` int(11) DEFAULT NULL,
  `frist_number` int(11) DEFAULT NULL,
  `frist_amount` decimal(14,2) DEFAULT NULL,
  `sumpersonnel` decimal(14,2) DEFAULT NULL,
  `frist_sum_amount` decimal(14,2) DEFAULT NULL,
  `rate` decimal(14,2) DEFAULT NULL,
  `arpu` decimal(14,2) DEFAULT NULL,
  `arppu` decimal(14,2) DEFAULT NULL,
  `day_retain` decimal(14,2) DEFAULT NULL,
  `day_account_count` decimal(14,2) DEFAULT NULL,
  `three_retain` decimal(14,2) DEFAULT NULL,
  `three_account_count` decimal(14,2) DEFAULT NULL,
  `week_retain` decimal(14,2) DEFAULT NULL,
  `week_account_count` decimal(14,2) DEFAULT NULL,
  `fifteen_retain` decimal(14,2) DEFAULT NULL,
  `fifteen_retain_count` decimal(14,2) DEFAULT NULL,
  `thirty_retain` decimal(14,2) DEFAULT NULL,
  `thirty_retain_count` decimal(14,2) DEFAULT NULL,
  `fortyfive_retain` decimal(14,2) DEFAULT NULL,
  `fortyfive_retain_count` decimal(14,2) DEFAULT NULL,
  `sixty_retain` decimal(14,2) DEFAULT NULL,
  `sixty_retain_count` decimal(14,2) DEFAULT NULL,
  `ninety_retain` decimal(14,2) DEFAULT NULL,
  `ninety_retain_count` decimal(14,2) DEFAULT NULL,
  `max_online_number` decimal(14,2) DEFAULT NULL,
  `game_number` int(11) DEFAULT NULL,
  `ave_gametime` decimal(14,2) DEFAULT NULL,
  `ave_gamenumber` decimal(14,2) DEFAULT NULL,
  `new_players` int(11) DEFAULT NULL,
  `day_ltv` decimal(14,2) DEFAULT NULL,
  `two_ltv` decimal(14,2) DEFAULT NULL,
  `three_ltv` decimal(14,2) DEFAULT NULL,
  `four_ltv` decimal(14,2) DEFAULT NULL,
  `five_ltv` decimal(14,2) DEFAULT NULL,
  `six_ltv` decimal(14,2) DEFAULT NULL,
  `seven_ltv` decimal(14,2) DEFAULT NULL,
  `fifteen_ltv` decimal(14,2) DEFAULT NULL,
  `thirty_ltv` decimal(14,2) DEFAULT NULL,
  `fortyfive_ltv` decimal(14,2) DEFAULT NULL,
  `sixty_ltv` decimal(14,2) DEFAULT NULL,
  `ninety_ltv` decimal(14,2) DEFAULT NULL,
  `new_reg_rec_mon` decimal(14,2) DEFAULT NULL,
  `new_reg_rec_num` int(14) DEFAULT NULL,
  `double_ply_num` int(14) DEFAULT NULL,
  `new_reg_rate` decimal(14,2) DEFAULT NULL,
  `new_reg_arpu` decimal(14,2) DEFAULT NULL,
  `new_reg_arppu` decimal(14,2) DEFAULT NULL,
  `cum_register` int(14) DEFAULT NULL,
  `cum_recharge` decimal(14,2) DEFAULT NULL,
  `cum_rec_num` int(14) DEFAULT NULL,
  `cum_ltv` decimal(14,2) DEFAULT NULL,
  `channel_id` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of daily_statistics
-- ----------------------------

-- ----------------------------
-- Table structure for `growth_fund`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `growth_fund`  (
  `log_id` bigint(20) NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `purchase_growth_fund_id` int(11) DEFAULT NULL,
  `purchase_cost` bigint(20) DEFAULT NULL,
  `purchase_quantity` int(20) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`),
  KEY `account_id` (`account_id`),
  KEY `log_time` (`log_time`)
) ENGINE=InnoDB AUTO_INCREMENT=6062 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of growth_fund
-- ----------------------------

-- ----------------------------
-- Table structure for `item_change_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `item_change_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `item_id` int(4) DEFAULT NULL,
  `change_num` bigint(20) DEFAULT NULL,
  `left_num` bigint(20) DEFAULT NULL,
  `op_type` varchar(64) DEFAULT NULL,
  `cause_id` varchar(64) DEFAULT NULL,
  `location` varchar(64) DEFAULT NULL,
  `item_guid` bigint(20) DEFAULT '0',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of item_change_log
-- ----------------------------

-- ----------------------------
-- Table structure for `legion_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `legion_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT '0',
  `log_time` datetime NOT NULL,
  `legion_id` bigint(20) DEFAULT '0',
  `role_id` bigint(20) DEFAULT '0',
  `legion_name` varchar(101) DEFAULT '',
  `operate_type` int(11) DEFAULT '0',
  `param_string` varchar(401) DEFAULT '',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`legion_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of legion_log
-- ----------------------------

-- ----------------------------
-- Table structure for `level_change_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `level_change_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `old_level` int(11) DEFAULT NULL,
  `new_level` int(11) DEFAULT NULL,
  `online_time` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of level_change_log
-- ----------------------------

-- ----------------------------
-- Table structure for `map_change_log`
-- ----------------------------
DROP TABLE IF EXISTS `map_change_log`;
CREATE TABLE `map_change_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `map_template_id` int(11) DEFAULT NULL,
  `map_type` int(11) DEFAULT NULL,
  `map_sub_type` int(11) DEFAULT NULL,
  `change_type` int(11) DEFAULT NULL,
  `stage_type` int(11) DEFAULT NULL,
  `login_type` int(50) DEFAULT NULL,
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of map_change_log
-- ----------------------------

-- ----------------------------
-- Table structure for `money_change_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `money_change_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `money_type` int(4) DEFAULT NULL,
  `old_money` bigint(20) DEFAULT NULL,
  `new_money` bigint(20) DEFAULT NULL,
  `op_type` varchar(64) DEFAULT NULL,
  `cause_id` varchar(64) DEFAULT NULL,
  `location` varchar(64) DEFAULT NULL,
  `param2` int(64) DEFAULT NULL,
  `login_type` int(11) DEFAULT NULL,
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `money_type` (`money_type`),
  KEY `op_type` (`op_type`),
  KEY `device_id` (`device_id`),
  KEY `cause_id` (`cause_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of money_change_log
-- ----------------------------

-- ----------------------------
-- Table structure for `new_guide_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `new_guide_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `step_id` varchar(32) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `account_id` (`account_id`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of new_guide_log
-- ----------------------------

-- ----------------------------
-- Table structure for `occupation_pk_log`
-- ----------------------------
DROP TABLE IF EXISTS `occupation_pk_log`;
CREATE TABLE `occupation_pk_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `class_type` int(11) DEFAULT NULL,
  `state_value` int(11) DEFAULT NULL,
  `oppo_guid` varchar(64) DEFAULT NULL,
  `pk_result` int(11) DEFAULT NULL,
  `item_id_1` int(11) DEFAULT NULL,
  `item_num_1` int(11) DEFAULT NULL,
  `item_id_2` int(11) DEFAULT NULL,
  `item_num_2` int(11) DEFAULT NULL,
  `item_id_3` int(11) DEFAULT NULL,
  `item_num_3` int(11) DEFAULT NULL,
  `item_id_4` int(11) DEFAULT NULL,
  `item_num_4` int(11) DEFAULT NULL,
  `item_id_5` int(11) DEFAULT NULL,
  `item_num_5` int(11) DEFAULT NULL,
  `item_id_6` int(11) DEFAULT NULL,
  `item_num_6` int(11) DEFAULT NULL,
  `item_id_7` int(11) DEFAULT NULL,
  `item_num_7` int(11) DEFAULT NULL,
  `item_id_8` int(11) DEFAULT NULL,
  `item_num_8` int(11) DEFAULT NULL,
  `item_id_9` int(11) DEFAULT NULL,
  `item_num_9` int(11) DEFAULT NULL,
  `item_id_10` int(11) DEFAULT NULL,
  `item_num_10` int(11) DEFAULT NULL,
  `login_type` int(50) DEFAULT NULL,
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(50) DEFAULT NULL,
  `model` varchar(50) DEFAULT NULL,
  `idfa` varchar(200) DEFAULT NULL,
  `imei` varchar(200) DEFAULT NULL,
  `androidId` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `sn` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of occupation_pk_log
-- ----------------------------

-- ----------------------------
-- Table structure for `player_count_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `player_count_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `player_count` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT NULL,
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of player_count_log
-- ----------------------------

-- ----------------------------
-- Table structure for `rank_list_log`
-- ----------------------------
DROP TABLE IF EXISTS `rank_list_log`;
CREATE TABLE `rank_list_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `ranking_type` bigint(20) NOT NULL DEFAULT '0',
  `ranking_value` bigint(20) DEFAULT '0',
  `ranking_time` bigint(20) DEFAULT '0',
  `supporting_guid` bigint(20) DEFAULT '0',
  `supporting_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '0',
  `vip_level` int(11) DEFAULT NULL,
  `peak` int(11) unsigned zerofill DEFAULT '00000000000',
  `game_channel` varchar(129) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of rank_list_log
-- ----------------------------

-- ----------------------------
-- Table structure for `recharge_step_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `recharge_step_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `order_id` varchar(1024) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `goods_id` varchar(64) DEFAULT NULL,
  `step_num` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `role_id` (`role_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of recharge_step_log
-- ----------------------------

-- ----------------------------
-- Table structure for `recharge_success_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `recharge_success_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `game_order_id` varchar(64) DEFAULT NULL,
  `game_channel_order_id` varchar(64) DEFAULT NULL,
  `order_amount` decimal(14,2) DEFAULT NULL,
  `share_amount` decimal(14,2) DEFAULT NULL,
  `no_share_amount` decimal(14,2) DEFAULT NULL,
  `pay_id` int(11) DEFAULT NULL,
  `recharge_channel` varchar(64) DEFAULT NULL,
  `add_jewel` bigint(11) DEFAULT NULL,
  `total_jewel` bigint(11) DEFAULT NULL,
  `currency_type` int(11) DEFAULT NULL,
  `item_id` int(11) DEFAULT NULL,
  `is_first_recharge` int(4) DEFAULT NULL,
  `device_model` varchar(64) DEFAULT NULL,
  `android_id` varchar(64) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of recharge_success_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_buy_goods_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_buy_goods_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT '0',
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT '',
  `role_id` bigint(20) DEFAULT '0',
  `role_name` varchar(101) DEFAULT '',
  `role_level` int(11) DEFAULT '0',
  `goods_id` int(11) DEFAULT '0',
  `buy_num` int(11) DEFAULT '0',
  `item_id` int(11) DEFAULT '0',
  `item_num_per_goods` int(11) DEFAULT '0',
  `cost_money_type` int(11) DEFAULT '0',
  `cost_money_num` int(11) DEFAULT '0',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_buy_goods_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_chat_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_chat_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `role_id` bigint(20) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `target_id` bigint(20) DEFAULT NULL,
  `chat_type` int(11) DEFAULT NULL,
  `chat_content` varchar(300) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_chat_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_get_mail_content_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_get_mail_content_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT '0',
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT '',
  `role_id` bigint(20) DEFAULT '0',
  `role_name` varchar(101) DEFAULT '',
  `role_level` int(11) DEFAULT '0',
  `reward_id` int(11) DEFAULT '0',
  `reward_num` bigint(20) DEFAULT '0',
  `mail_guid` bigint(20) DEFAULT '0',
  `mail_title` varchar(65) DEFAULT '',
  `text_contents` varchar(401) DEFAULT '',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_get_mail_content_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_jewel_count`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_jewel_count`  (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) NOT NULL DEFAULT '',
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) NOT NULL DEFAULT '',
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `total_recharge_jewel` int(11) DEFAULT NULL,
  `total_consume_jewel` int(11) DEFAULT NULL,
  `total_recharge_bind_jewel` int(11) DEFAULT '0',
  `total_consume_bind_jewel` int(11) DEFAULT '0',
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`,`account_id`,`role_id`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `money_type` (`total_recharge_jewel`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_jewel_count
-- ----------------------------

-- ----------------------------
-- Table structure for `role_logout_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_logout_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `channel_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `op_type` int(4) DEFAULT NULL,
  `create_time` datetime DEFAULT NULL,
  `online_time` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`),
  KEY `device_id` (`device_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
 

-- ----------------------------
-- Records of role_logout_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_lucky_draw_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_lucky_draw_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `role_id` bigint(20) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `item_id` int(11) DEFAULT NULL,
  `item_num` int(11) DEFAULT NULL,
  `treasure_type` int(11) DEFAULT NULL,
  `cost_type` int(11) DEFAULT NULL,
  `is_first_time_do` int(11) DEFAULT NULL,
  `old_lucky_value` int(11) DEFAULT NULL,
  `new_lucky_value` int(11) DEFAULT NULL,
  `is_free` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_lucky_draw_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_task_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_task_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` varchar(64) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) DEFAULT NULL,
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) DEFAULT NULL,
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `op_type` int(4) DEFAULT NULL,
  `task_type` int(11) DEFAULT NULL,
  `task_id` varchar(16) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`),
  KEY `account_id` (`account_id`),
  KEY `server_id` (`server_id`),
  KEY `role_name` (`role_name`),
  KEY `role_id` (`role_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of role_task_log
-- ----------------------------

-- ----------------------------
-- Table structure for `role_time_limit_activity_log`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `role_time_limit_activity_log` (
  `log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `server_id` int(11) DEFAULT NULL,
  `log_time` datetime NOT NULL,
  `account_id` varchar(101) NOT NULL DEFAULT '',
  `device_id` varchar(64) DEFAULT NULL,
  `role_id` varchar(64) NOT NULL DEFAULT '',
  `role_name` varchar(101) DEFAULT NULL,
  `role_level` int(11) DEFAULT NULL,
  `role_power` int(11) DEFAULT NULL,
  `role_vip` int(11) DEFAULT NULL,
  `activity_id` int(11) DEFAULT NULL,
  `activity_type` int(11) DEFAULT NULL,
  `cur_value` int(11) DEFAULT NULL,
  `target_value` int(11) DEFAULT NULL,
  `reward_id` int(11) DEFAULT NULL,
  `reward_num` int(11) DEFAULT NULL,
  `login_type` int(11) DEFAULT '0',
  `appid` varchar(101) DEFAULT '',
  `version` varchar(101) DEFAULT '',
  `stepnumid` varchar(101) DEFAULT '',
  `normversion` varchar(101) DEFAULT '',
  `gamechannel` varchar(101) DEFAULT '',
  `ip` varchar(64) DEFAULT '',
  `adchannel` varchar(101) DEFAULT '',
  `cross_service` varchar(101) DEFAULT '',
  `os_version` varchar(101) DEFAULT '',
  `model` varchar(101) DEFAULT '',
  `idfa` varchar(101) DEFAULT '',
  `imei` varchar(101) DEFAULT '',
  `androidId` varchar(101) DEFAULT '',
  `mac` varchar(101) DEFAULT '',
  `sn` varchar(101) DEFAULT '',
  PRIMARY KEY (`log_id`,`log_time`,`account_id`,`role_id`),
  KEY `account_id` (`account_id`),
  KEY `role_id` (`role_id`),
  KEY `role_name` (`role_name`),
  KEY `money_type` (`activity_id`),
  KEY `device_id` (`device_id`),
  KEY `server_id` (`server_id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
-- ----------------------------
-- Records of role_time_limit_activity_log
-- ----------------------------

-- ----------------------------
-- Table structure for `type_field`
-- ----------------------------
CREATE TABLE IF NOT EXISTS `type_field` (
  `fid` int(11) NOT NULL AUTO_INCREMENT,
  `id` varchar(11) NOT NULL,
  `name` varchar(64) DEFAULT NULL,
  `pid` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `remarks` varchar(64) DEFAULT NULL,
  `sort` int(11) DEFAULT NULL,
  PRIMARY KEY (`fid`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of type_field
-- ----------------------------

-- ----------------------------
-- Procedure structure for `insert_cycle`
-- ----------------------------
DROP PROCEDURE IF EXISTS `insert_cycle`;
DELIMITER ;;
CREATE PROCEDURE `insert_cycle`(daily_date VARCHAR(64),serverid int,cycletype int,channel VARCHAR(300))
label:BEGIN 
DECLARE iscount INT;
DECLARE active_number INT;
DECLARE pay_number INT;
DECLARE firstweek VARCHAR(64);
DECLARE lastweek VARCHAR(64);
DECLARE firstmoon VARCHAR(64);
DECLARE lastmoon VARCHAR(64);
DECLARE cycle_week_month1 VARCHAR(64);
DECLARE cyclemonth VARCHAR(64);
DECLARE rate DECIMAL (14, 2);
DECLARE pay_amount DECIMAL (14, 2);
DECLARE arpu DECIMAL (14, 2);
DECLARE arppu DECIMAL (14, 2);
DECLARE daily_register INT;
DECLARE new_reg_rec_num INT;
DECLARE new_reg_rate DECIMAL (14, 2);
DECLARE new_reg_rec_mon DECIMAL (14, 2);


set firstweek = (select date_sub(daily_date,INTERVAL WEEKDAY(daily_date) + 7 DAY));
set lastweek = (select date_sub(daily_date,INTERVAL WEEKDAY(daily_date) + 1 DAY));
set firstmoon = (select date_sub(date_sub(date_format(daily_date,'%y-%m-%d'),interval extract(day from daily_date)-1 day),interval 1 month));
set lastmoon = (select date_sub(date_sub(date_format(daily_date,'%y-%m-%d'),interval extract(day from daily_date) day),interval 0 month));
 
IF(cycletype=0)THEN
set cycle_week_month1 = (select CONCAT((select YEAR(daily_date) ),'-',(select month(daily_date)),'-',(SELECT (DAY(daily_date)+WEEKDAY(daily_date-INTERVAL DAY(daily_date) DAY)) DIV 7 + 1)));
SET iscount = (SELECT count(*) FROM cycle_statistics WHERE cycle_week_month = cycle_week_month1 AND server_id = serverid  and channel_id = channel);
IF(iscount!=0)THEN
leave label;
END IF;

SET active_number = (SELECT count(DISTINCT account_id) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET pay_number = (SELECT active_number FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET rate = (SELECT (SELECT count(DISTINCT account_id) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS account_id FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek  AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) * 100 AS account_id 
FROM 	recharge_success_log 	WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET pay_amount = (SELECT  sum(order_amount) from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET arpu = (SELECT SUM(order_amount) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek  AND server_id = serverid  AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS arpu
 from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET arppu = (SELECT (SELECT sum(order_amount) / (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d')  between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
 from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET daily_register = (SELECT count(DISTINCT account_id) FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek  AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rec_num = (SELECT count(0) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rate = (SELECT 	count(DISTINCT account_id) / (SELECT 	(CASE WHEN count(DISTINCT account_id) = 0 THEN NULL ELSE count(DISTINCT account_id) END) AS account_id
		FROM 	account_create_log 	WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek 
		AND server_id = serverid  AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) * 100 AS account_id FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek  AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rec_mon = (SELECT sum(order_amount) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') between firstweek and lastweek AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));

ELSE
set cycle_week_month1 = (select CONCAT((select YEAR(daily_date) ),'-',(select month(daily_date))));
SET iscount = (SELECT count(*) FROM cycle_statistics WHERE cycle_week_month = cycle_week_month1 AND server_id = serverid and channel_id = channel);
IF(iscount!=0)THEN
leave label;
END IF;
SET active_number = (SELECT count(DISTINCT account_id) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET pay_number = (SELECT active_number FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET rate = (SELECT (SELECT count(DISTINCT account_id) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS account_id FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) * 100 AS account_id 
FROM 	recharge_success_log 	WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET pay_amount = (SELECT  sum(order_amount) from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET arpu = (SELECT SUM(order_amount) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon  AND server_id = serverid  AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS arpu
 from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET arppu = (SELECT (SELECT sum(order_amount) / (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d')  between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
 from recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET daily_register = (SELECT count(DISTINCT account_id) FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon  AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rec_num = (SELECT count(0) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rate = (SELECT 	count(DISTINCT account_id) / (SELECT 	(CASE WHEN count(DISTINCT account_id) = 0 THEN NULL ELSE count(DISTINCT account_id) END) AS account_id
		FROM 	account_create_log 	WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon
		AND server_id = serverid  AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) * 100 AS account_id FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));
SET new_reg_rec_mon = (SELECT sum(order_amount) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') between firstmoon and lastmoon AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));

END IF;
INSERT INTO cycle_statistics (log_id,server_id,cycle_week_month,active_number,pay_number,rate,
pay_amount,arpu,arppu,daily_register,new_reg_rec_num,new_reg_rate,new_reg_rec_mon,cycle_type,channel_id) VALUES (
 null,
serverid,
cycle_week_month1,
active_number,
pay_number,
rate,
pay_amount,
arpu,
arppu,
daily_register,
new_reg_rec_num,
new_reg_rate,
new_reg_rec_mon,
cycletype,
channel
);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `insert_daily`
-- ----------------------------
DROP PROCEDURE IF EXISTS `insert_daily`;
DELIMITER ;;
CREATE PROCEDURE `insert_daily`(daily_date VARCHAR(64),serverid int,channel VARCHAR(300))
BEGIN
	DECLARE
		iscount INT;

DECLARE
	daily_register INT;

DECLARE
	active_number INT;

DECLARE
	pay_amount DECIMAL (14, 2);

DECLARE
	frist_amount DECIMAL (14, 2);

DECLARE
	frist_sum_amount DECIMAL (14, 2);

DECLARE
	day_retain DECIMAL (14, 2);

DECLARE
	three_retain DECIMAL (14, 2);

DECLARE
	week_retain DECIMAL (14, 2);

DECLARE
	fifteen_retain DECIMAL (14, 2);

DECLARE
	thirty_retain DECIMAL (14, 2);

DECLARE
	fortyfive_retain DECIMAL (14, 2);


DECLARE
	sixty_retain DECIMAL (14, 2);

DECLARE
	ninety_retain DECIMAL (14, 2);

DECLARE
	max_online_number INT;

DECLARE
	game_number INT;

DECLARE
	ave_gamenumber DECIMAL (14, 2);

DECLARE
	new_reg_rec_num INT;

DECLARE
	new_reg_rec_mon DECIMAL (14, 2);

DECLARE
	double_ply_num DECIMAL (14, 2);

DECLARE
	new_reg_rate DECIMAL (14, 2);

DECLARE
	new_reg_arpu DECIMAL (14, 2);

DECLARE
	new_reg_arppu DECIMAL (14, 2);

DECLARE
	new_players INT;

 DECLARE
	cum_recharge DECIMAL (14, 2);
DECLARE
	cum_register INT;
 DECLARE
	cum_ltv DECIMAL (14, 2);
DECLARE
	cum_rec_num INT;



DECLARE
	yesterday VARCHAR(64);
DECLARE
	days_ago_3 VARCHAR(64);
DECLARE
	days_ago_4 VARCHAR(64);

DECLARE
	days_ago_5 VARCHAR(64);

DECLARE
	days_ago_6 VARCHAR(64);

DECLARE
	days_ago_7 VARCHAR(64);

DECLARE
	days_ago_8 VARCHAR(64);

DECLARE
	days_ago_15 VARCHAR(64);
DECLARE
	days_ago_30 VARCHAR(64);
DECLARE
	days_ago_45 VARCHAR(64);
DECLARE
	days_ago_60 VARCHAR(64);
DECLARE
	days_ago_90 VARCHAR(64);
DECLARE
	days_ago_16 VARCHAR(64);

DECLARE
	days_ago_31 VARCHAR(64);
DECLARE
	days_ago_46 VARCHAR(64);
DECLARE
	days_ago_61 VARCHAR(64);
DECLARE
	days_ago_91 VARCHAR(64);


DECLARE
	daysltv_1 VARCHAR(64);
DECLARE
	daysltv_2 VARCHAR(64);
DECLARE
	daysltv_3 VARCHAR(64);
DECLARE
	daysltv_4 VARCHAR(64);
DECLARE
	daysltv_5 VARCHAR(64);
DECLARE
	daysltv_6 VARCHAR(64);
DECLARE
	daysltv_7 VARCHAR(64);
DECLARE
	daysltv_15 VARCHAR(64);
DECLARE
	daysltv_30 VARCHAR(64);
DECLARE
	daysltv_45 VARCHAR(64);
DECLARE
	daysltv_60 VARCHAR(64);
DECLARE
	daysltv_90 VARCHAR(64);
 

SET yesterday 		= DATE_SUB(daily_date,INTERVAL 1 day );
SET days_ago_3 		= DATE_SUB(daily_date,INTERVAL 2 day );
SET days_ago_4 		= DATE_SUB(daily_date,INTERVAL 3 day );
SET days_ago_5 		= DATE_SUB(daily_date,INTERVAL 4 day );
SET days_ago_6 		= DATE_SUB(daily_date,INTERVAL 5 day );
SET days_ago_7 		= DATE_SUB(daily_date,INTERVAL 6 day );
SET days_ago_8 		= DATE_SUB(daily_date,INTERVAL 7 day );
SET days_ago_15 		= DATE_SUB(daily_date,INTERVAL 14 day );
SET days_ago_16 		= DATE_SUB(daily_date,INTERVAL 15 day );
SET days_ago_30 		= DATE_SUB(daily_date,INTERVAL 29 day );
SET days_ago_31 		= DATE_SUB(daily_date,INTERVAL 30 day );
SET days_ago_45 		= DATE_SUB(daily_date,INTERVAL 44 day );
SET days_ago_46 		= DATE_SUB(daily_date,INTERVAL 45 day );
SET days_ago_60 		= DATE_SUB(daily_date,INTERVAL 59 day );
SET days_ago_61 		= DATE_SUB(daily_date,INTERVAL 60 day );
SET days_ago_90 		= DATE_SUB(daily_date,INTERVAL 89 day );
SET days_ago_91 		= DATE_SUB(daily_date,INTERVAL 90 day );




SET iscount = (SELECT count(*) FROM daily_statistics WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid and channel_id = channel);

 

IF (iscount = 0) THEN

SET daily_register = (SELECT count(DISTINCT account_id) FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid and  find_in_set(login_type,channel));


SET active_number = (SELECT count(DISTINCT account_id) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
;


SET frist_amount = (SELECT count(DISTINCT d.account_id) FROM recharge_success_log d WHERE DATE_FORMAT(d.log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d') AND d.server_id = serverid and 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));


SET frist_sum_amount = (
	SELECT
		sum(order_amount)
	FROM
		recharge_success_log
	WHERE
		account_id IN (SELECT DISTINCT account_id FROM recharge_success_log d WHERE is_first_recharge = 1 AND DATE_FORMAT(d.log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d') AND d.server_id = serverid)
	AND DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d') and server_id = serverid and 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
);


UPDATE daily_statistics
SET day_retain = (
		SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = yesterday AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),day_account_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = yesterday AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = yesterday
AND server_id = serverid;


UPDATE daily_statistics
SET three_retain = (
	SELECT
	x.stay_count AS day_retain
	FROM
				(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_3 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),three_account_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_3 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_3
AND server_id = serverid;


UPDATE daily_statistics
SET week_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_7 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),week_account_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_7 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_7
AND server_id = serverid;


UPDATE daily_statistics
SET fifteen_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_15 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),fifteen_retain_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_15 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_15
AND server_id = serverid;

UPDATE daily_statistics
SET thirty_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_30 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),thirty_retain_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_30 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_30
AND server_id = serverid;

UPDATE daily_statistics
SET fortyfive_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_45 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),fortyfive_retain_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_45 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_45
AND server_id = serverid;

UPDATE daily_statistics
SET sixty_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_60 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),sixty_retain_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_60 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_60
AND server_id = serverid;

UPDATE daily_statistics
SET ninety_retain = (
	SELECT
		x.stay_count AS day_retain
	FROM
		(SELECT COUNT(DISTINCT a.account_id) AS stay_count FROM role_logout_log b, account_create_log a WHERE b.account_id = a.account_id AND
		DATE_FORMAT(b.log_time, '%Y-%m-%d') = daily_date and DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_90 AND b.server_id = serverid AND
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) x
),ninety_retain_count = (SELECT count(DISTINCT account_id) AS account_count FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_90 AND server_id = serverid AND
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
WHERE
	DATE_FORMAT(log_time, "%Y-%m-%d") = days_ago_90
AND server_id = serverid;

UPDATE daily_statistics
SET day_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = yesterday AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 1 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = yesterday
		AND a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  yesterday 
 );

UPDATE daily_statistics
SET two_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_3  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 2 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_3 AND 
	  a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_3
 );

UPDATE daily_statistics
SET three_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_4  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 3 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_4 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_4
 );

UPDATE daily_statistics
SET four_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_5  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 4 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_5 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_5
 );

UPDATE daily_statistics
SET five_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_6  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 5 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_6 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_6
 );

UPDATE daily_statistics
SET six_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_7  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 6 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_7 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_7
 );

UPDATE daily_statistics
SET seven_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_8  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 7 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_8 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_8
 );


UPDATE daily_statistics
SET fifteen_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_16  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 15 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_16 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_16
 );

UPDATE daily_statistics
SET thirty_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_31  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 30 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_31 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_31
 );

UPDATE daily_statistics
SET fortyfive_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_46  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 45 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_46 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_46
 );

UPDATE daily_statistics
SET sixty_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_61  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 60 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_61 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_61
 );

UPDATE daily_statistics
SET ninety_ltv =(SELECT
	order_amount/(SELECT count(DISTINCT account_id) FROM account_create_log b WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = days_ago_91  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS account_count
FROM
	(
		SELECT
			DATE_FORMAT(a.log_time, '%Y-%m-%d') as log_time,
			SUM(b.order_amount) AS order_amount
		FROM
			recharge_success_log b,
			account_create_log a
		WHERE
			b.account_id = a.account_id
		AND b.log_time BETWEEN DATE_FORMAT(a.log_time, '%Y-%m-%d')
		AND DATE_ADD(DATE_FORMAT(a.log_time, '%Y-%m-%d'), INTERVAL 90 DAY)
		AND DATE_FORMAT(a.log_time, '%Y-%m-%d') = days_ago_91 AND 
a.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	 
	) AS x
  where DATE_FORMAT(log_time, "%Y-%m-%d") =  days_ago_91
 );


SET max_online_number = (SELECT max(player_count) FROM player_count_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid);


SET game_number = (SELECT count(*) FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND op_type !=1 AND server_id = serverid  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));


SET ave_gamenumber = (SELECT avg(cou) FROM(SELECT count(*) AS cou FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND op_type !=1 AND server_id = serverid  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)) GROUP BY account_id) a);

SET new_players = (SELECT count(DISTINCT c.account_id) AS new_players FROM role_logout_log c, account_create_log d WHERE DATE_FORMAT(c.log_time, '%Y-%m-%d') = DATE_FORMAT(d.log_time, '%Y-%m-%d') AND c.account_id = d.account_id AND c.server_id = d.server_id AND DATE_FORMAT(c.log_time, '%Y-%m-%d') = daily_date AND d.server_id = serverid and
d.account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));


SET new_reg_rec_num = (SELECT count(0) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid) and DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid  AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));

SET new_reg_rec_mon = (SELECT sum(order_amount) FROM recharge_success_log where account_id in (SELECT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid) and DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));


SET double_ply_num = (SELECT
	count(distinct account_id)-(SELECT count(0) FROM account_create_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND  server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
FROM
	recharge_success_log
WHERE
  DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));

SET new_reg_rate = (SELECT
	count(DISTINCT account_id) / (
		SELECT
			(CASE WHEN count(DISTINCT account_id) = 0 THEN NULL ELSE count(DISTINCT account_id) END) AS account_id
		FROM
			account_create_log
		WHERE
			DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
		AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
	) * 100 AS account_id
FROM
	recharge_success_log
WHERE
	DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)));

SET new_reg_arpu = (SELECT
	SUM(order_amount) / (SELECT count(DISTINCT account_id) FROM account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)))
FROM
	recharge_success_log where account_id in (SELECT DISTINCT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
);

SET new_reg_arppu = (SELECT
	SUM(order_amount) / count(distinct account_id)
FROM
	recharge_success_log where account_id in (SELECT DISTINCT account_id from account_create_log where DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) and DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')
AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
);
set cum_register = (SELECT count(DISTINCT account_id) FROM account_create_log WHERE  server_id = serverid);
set cum_recharge = (SELECT sum(order_amount) FROM recharge_success_log WHERE  server_id = serverid);
set cum_rec_num = (SELECT count(DISTINCT account_id) FROM recharge_success_log WHERE  server_id = serverid);
set cum_ltv = cum_recharge/cum_register;

INSERT INTO daily_statistics (
	SELECT
		NULL,
		serverid,
		DATE_FORMAT(daily_date, '%Y-%m-%d') AS log_time1,
		daily_register,
		active_number,
		sum(order_amount) AS totalmoney,
		count(DISTINCT account_id) AS totalnum,
		count(DISTINCT CASE WHEN is_first_recharge = 1 THEN account_id END) AS fristnum,
		sum(CASE WHEN is_first_recharge = 1 THEN order_amount ELSE 0 END) AS fristmoney,
		(SELECT count(DISTINCT d.account_id) FROM recharge_success_log d WHERE DATE_FORMAT(d.log_time, '%Y-%m-%d') <= DATE_FORMAT(daily_date, '%Y-%m-%d') AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) sumpersonnel,
		frist_sum_amount,
		(
			SELECT
				count(DISTINCT account_id) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS account_id FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')   AND server_id = serverid) * 100 AS account_id
			FROM
				recharge_success_log
			WHERE
				DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d')  AND server_id = serverid AND 
		account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
		) AS Rate,
		SUM(order_amount) / (SELECT (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) AS sum_players FROM role_logout_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d') AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel)) ) AS arpu,
		(SELECT sum(order_amount) / (case when count(DISTINCT account_id)=0 then null else count(DISTINCT account_id) end ) FROM recharge_success_log WHERE DATE_FORMAT(log_time, '%Y-%m-%d') = DATE_FORMAT(daily_date, '%Y-%m-%d') AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))) AS payarpu,
		0,
		0,
		three_retain,
		0,
		week_retain,
		0,
fifteen_retain,
0,
thirty_retain,
0,
fortyfive_retain,
0,
sixty_retain,
0,
ninety_retain,
0,
		max_online_number,
		game_number,
		0,
		ave_gamenumber,
		new_players,
		0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
new_reg_rec_mon,
new_reg_rec_num,
double_ply_num,
new_reg_rate,
new_reg_arpu,
new_reg_arppu,
cum_register,
cum_recharge,
cum_rec_num,
cum_ltv,
channel
	FROM
		recharge_success_log
	WHERE
		DATE_FORMAT(log_time, '%Y-%m-%d') = daily_date AND server_id = serverid AND 
account_id in (select DISTINCT account_id from account_create_log where find_in_set(login_type,channel))
);


END
IF;


END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `partitionByTable`
-- ----------------------------
DROP PROCEDURE IF EXISTS `partitionByTable`;
DELIMITER ;;
CREATE PROCEDURE `partitionByTable`(IN tableName varchar(50),IN columName varchar(50))
begin
    set @s0 = concat('select max(',columName,') into @a from ',tableName);
    prepare stmt0 from @s0;
    execute stmt0;
    deallocate prepare stmt0;

    set @s1 = concat('select min(',columName,') into @b from ',tableName);
    prepare stmt1 from @s1;
    execute stmt1;
    deallocate prepare stmt1;

    set @maxDate = @a;
    set @minDate = @b;
    set @s2 = concat('alter table ',tableName,' partition by RANGE(to_days(',columName,'))(');

    set @endDate = now();
    if @minDate is null
    then set @minDate = @endDate;
         set @maxDate = @endDate;
    end if;
    set @minDate = date(date(@minDate)-day(@minDate)+1);
    set @maxDate = date(date(@maxDate)-day(@maxDate)+1);

    while to_days(@minDate)<to_days(@maxDate)
      do
      set @s2= concat(@s2,' PARTITION ',tableName,'_',date_format(@minDate,'%Y%m'),' VALUES less than (to_days("',date_add(@minDate,interval 1 month),'")),');
      set @minDate=  date_add(@minDate,interval 1 month);
    end while;

    set @s2=concat(@s2,' PARTITION ',tableName,'_',date_format(@minDate,'%Y%m'),' VALUES less than (to_days("',date_add(@minDate,interval 1 month),'")));');
    prepare stmt2 from @s2;
    execute stmt2;
    deallocate prepare stmt2;
end
;;
DELIMITER ;

 -- ----------------------------
-- Procedure structure for `partitionNextMonth`
-- ----------------------------
DROP PROCEDURE IF EXISTS `partitionNextMonth`;
DELIMITER ;;
CREATE PROCEDURE `partitionNextMonth`(IN tableName varchar(50))
begin
	DECLARE i INT;
  SET i = 2;
WHILE i < 120 DO
    set @enddate = date_add(date(now())-day(now())+1,interval i month);
		   
    set @s=concat('alter table ',tableName,' add partition (partition ',tableName,'_',date_format(date_add(date(now())-day(now())+1,interval i-1 month),'%Y%m'),' VALUES less than (to_days("',@enddate,'")));');
    prepare stmt from @s;execute stmt;deallocate prepare stmt;
		SET i = i + 1;
END
WHILE;
end
;;
DELIMITER ;
-- ----------------------------
-- Procedure structure for `create_partition`
-- ----------------------------
DROP PROCEDURE IF EXISTS `create_partition`;
DELIMITER ;;
CREATE PROCEDURE `create_partition`()
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
		AND table_name NOT LIKE 'QRTZ%'
		AND table_name NOT LIKE 'conf_%'
		AND table_name NOT LIKE 'type_%'
		AND table_name NOT LIKE '%_statistics'
		AND table_name NOT LIKE 'growth_fund';
	DECLARE CONTINUE HANDLER FOR SQLSTATE '02000' SET s_tablename = NULL;    -- 此为固定写法 非null判断

	OPEN cur_table_structure;  -- 开启游标

	FETCH cur_table_structure INTO s_tablename;
	WHILE ( s_tablename IS NOT NULL) DO
    call partitionByTable(s_tablename,'log_time');
		call partitionNextMonth(s_tablename);
	
		FETCH cur_table_structure INTO s_tablename;
	END WHILE; 
	CLOSE cur_table_structure; -- 关闭游标
END
;;
DELIMITER ;
-- ----------------------------
-- Event structure for `event_daily_analyze`
-- ----------------------------
DROP EVENT IF EXISTS `event_daily_analyze`;
DELIMITER ;;
CREATE EVENT `event_daily_analyze` ON SCHEDULE EVERY 1 DAY STARTS '2017-08-28 04:00:00' ON COMPLETION PRESERVE ENABLE DO call daily_analyze()
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_init_ranking_list_log_table`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_init_ranking_list_log_table`;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `sp_init_ranking_list_log_table`(IN `max_ranking_num` int)
BEGIN
	#Routine body goes here...
	DECLARE i INT DEFAULT 0;
	SET i = 0;
	WHILE i < max_ranking_num DO

		#此处仅处理不存在表格时新增表格
		SET @sql_create_table = concat(
		'CREATE TABLE IF NOT EXISTS rank_list_log_', i,"(
		`log_id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
		`server_id` varchar(64) DEFAULT NULL,
		`log_time` datetime NOT NULL,
		`account_id` varchar(101) DEFAULT NULL,
		`device_id` varchar(64) DEFAULT NULL,
		`role_id` varchar(64) DEFAULT NULL,
		`role_name` varchar(101) DEFAULT NULL,
		`role_level` int(11) DEFAULT NULL,
		`role_power` int(11) DEFAULT NULL,
		`role_vip` int(11) DEFAULT NULL,
		`ranking_type` bigint(20) NOT NULL DEFAULT '0',
		`ranking_value` bigint(20) DEFAULT '0',
		`ranking_time` bigint(20) DEFAULT '0',
		`supporting_guid` bigint(20) DEFAULT '0',
		`supporting_name` varchar(101) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT '0',
		`vip_level` int(11) DEFAULT NULL,
		`peak` int(11) unsigned zerofill DEFAULT '00000000000',
		`game_channel` varchar(129) DEFAULT '',
		`ranking_index` int(11) DEFAULT NULL,
		PRIMARY KEY (`log_id`,`log_time`),
		KEY `account_id` (`account_id`),
		KEY `role_id` (`role_id`),
		KEY `role_name` (`role_name`),
		KEY `device_id` (`device_id`),
		KEY `server_id` (`server_id`)
		) ENGINE=InnoDB AUTO_INCREMENT=0 DEFAULT CHARSET=utf8;");

		PREPARE sql_create_table FROM @sql_create_table;   
		EXECUTE sql_create_table;

		SET i = i + 1;
	END WHILE;
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_chat_log`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_chat_log`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_chat_log`(in in_server_id int, in in_log_time datetime,in in_role_account varchar(100),in in_role_guid varchar(64) ,in in_role_name varchar(100), in in_role_level int, in in_target_guid varchar(64), in in_chat_type int,in in_chat_content varchar(500), in in_login_type int, in in_gamechannel varchar(101), in in_ip varchar(64))
BEGIN
/*
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM `auction` WHERE item_guid = `in_item_guid`;
	END IF;
*/
	INSERT INTO role_chat_log(server_id, log_time, account_id, role_id, role_name, role_level, target_id, chat_type, chat_content, login_type, gamechannel, ip) VALUES (in_server_id, in_log_time, in_role_account, in_role_guid, in_role_name, in_role_level, in_target_guid, in_chat_type, in_chat_content, in_login_type, in_gamechannel, in_ip);
END
;;
DELIMITER ;

-- ----------------------------
-- Procedure structure for `sp_save_legion_log`
-- ----------------------------
DROP PROCEDURE IF EXISTS `sp_save_legion_log`;
DELIMITER ;;
CREATE PROCEDURE `sp_save_legion_log`(in in_server_id int, in in_log_time datetime,in in_legion_id bigint,in in_role_guid bigint,in in_legion_name varchar(100), in in_operate_type int, in in_param_string varchar(400),in in_login_type int, in in_channel_id varchar(101), in in_ip varchar(64))
BEGIN
/*
	DECLARE exist_data_num INT DEFAULT 0;
	SELECT COUNT(*) INTO exist_data_num FROM auction WHERE item_guid = `in_item_guid`;
	IF exist_data_num > 0 THEN
			DELETE FROM `auction` WHERE item_guid = `in_item_guid`;
	END IF;
*/
	INSERT INTO legion_log(server_id, log_time, legion_id, role_id, legion_name, operate_type, param_string, login_type, gamechannel, ip) VALUES (in_server_id, in_log_time, in_legion_id, in_role_guid, in_legion_name, in_operate_type, in_param_string, in_login_type, in_channel_id, in_ip);
END
;;
DELIMITER ;

-- ----------------------------
-- Event structure for `event_daily_analyze`
-- ----------------------------
DROP EVENT IF EXISTS `event_daily_analyze`;
DELIMITER ;;
CREATE EVENT `event_daily_analyze` ON SCHEDULE EVERY 1 DAY STARTS '2017-08-28 04:00:00' ON COMPLETION PRESERVE ENABLE DO call daily_analyze()
;;
DELIMITER ;

-- ----------------------------
-- open event   set global event_scheduler = on;
-- ----------------------------

call create_partition();
