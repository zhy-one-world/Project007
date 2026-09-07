
ï¿

game.protofaith"ˆø

game_proto\

enter_game
	role_guid (RroleGuid
account (	Raccount
line_id (RlineIdI
enter_game_end
result (Rresult
server_type (R
serverType6

pos_vector
x (Rx
y (Ry
z (Rz5
	rot_angle
x (Rx
y (Ry
z (Rz

pawn_attE
enter_scene6
type (2".faith.game_proto.enter_scene_typeRtype)
enter_scene_end
result (Rresult§
transfer_scene&
map_template_id (RmapTemplateId
line_id (RlineId
group_id (RgroupId
map_guid (RmapGuid
	war_index (RwarIndex®
transfer_scene_pos&
map_template_id (RmapTemplateId
line_id (RlineId
map_guid (RmapGuid
location (Rlocation
rotation (Rrotation
transfer_scene_born3
reconnect_enter_scene_end
result (Rresult$
	line_list
line_id (RlineId≈
transfer_scene_end
result (Rresult
	scene_uid (RsceneUid8
position (2.faith.game_proto.pos_vectorRposition@
rotator_angle (2.faith.game_proto.rot_angleRrotatorAngle∞
trans_local_scene_end
	role_guid (RroleGuid8
position (2.faith.game_proto.pos_vectorRposition@
rotator_angle (2.faith.game_proto.rot_angleRrotatorAngle6
client2gateway_ping
client_time (R
clientTimeW
gateway2client_ping
client_time (R
clientTime
server_time (R
serverTimes
client2cs_ping

trace_time (R	traceTime 
trace_time_2 (R
traceTime2 
trace_time_3 (R
traceTime3s
cs2client_ping

trace_time (R	traceTime 
trace_time_2 (R
traceTime2 
trace_time_3 (R
traceTime3
version_wrongF
set_timekeeping
time (Rtime
prefix_text (R
prefixText0
wave_trigger_message
message (Rmessagei

game_state
state (Rstate

state_time (R	stateTime&
cur_server_time (RcurServerTimeq

boss_state
boss_id (RbossId&
boss_state_code (RbossStateCode"
is_final_boss (RisFinalBossS
bosses_stateC
created_bosses (2.faith.game_proto.boss_stateRcreatedBossesk
protect_npc_state
npc_id (RnpcId
npc_guid (RnpcGuid$
npc_state_code (RnpcStateCodee
update_map_record_end
	role_guid (RroleGuid/
map_record_info_data (RmapRecordInfoDataã
update_map_record_all_end1
map_record_info_array (RmapRecordInfoArray"
is_end_record (RisEndRecord
map_num (RmapNumê
update_demons_tower_info_end
	role_guid (RroleGuid*
demons_tower_info (RdemonsTowerInfo'
is_in_mopping_up (RisInMoppingUp^
score_record
	role_guid (RroleGuid
	role_name (	RroleName
score (Rscoreó
score_record_list$
score_list_len (RscoreListLen=

score_list (2.faith.game_proto.score_recordR	scoreList?
extra_score (2.faith.game_proto.score_recordR
extraScore

extra_rank (R	extraRank&
is_legion_score (RisLegionScore
npc_id (RnpcId«
temple_game_over
	npc_score (RnpcScore#
win_money_id_1 (RwinMoneyId1%
win_money_num_1 (RwinMoneyNum1#
win_money_id_2 (RwinMoneyId2%
win_money_num_2 (RwinMoneyNum28
get_temple_over_gift 
get_over_num (R
getOverNum
get_temple_over_gift_endE
pop_plot_tip
str_id (RstrId
str_id_type (R	strIdType9
get_best_record&
map_template_id (RmapTemplateIdú
get_best_record_end&
map_template_id (RmapTemplateId1
best_record_role_name (	RbestRecordRoleName*
best_record_value (RbestRecordValue/
transfer_error

error_code (R	errorCode
transfer_to_pk_king_mapQ
pk_king_game_over
	award_exp (RawardExp
award_money (R
awardMoneyP
transfer_to_broken_sky_map
map_id (RmapId
	role_guid (RroleGuid3
broken_sky_boss_dead
	boss_guid (RbossGuid
	leave_map:
get_one_map_info&
map_template_id (RmapTemplateId`
get_one_map_info_end&
map_template_id (RmapTemplateId 
map_info_arr (R
mapInfoArrv
server_exp_level
	exp_level (RexpLevel
exp_percent (R
expPercent$
exp_last_level (RexpLastLevel2
need_hot_update
update_type (R
updateTypeT
gain_treasure_info_one
	info_type (RinfoType

info_value (R	infoValue]
gain_treasure_info_allC
info_arr (2(.faith.game_proto.gain_treasure_info_oneRinfoArr0
add_exp_raid_buff
	add_count (RaddCount4
add_exp_raid_buff_end
	add_count (RaddCountw
add_legion_boss_buff_end
	add_count (RaddCount
demon_count (R
demonCount

gold_count (R	goldCount~
add_legion_boss_big_player_info

class_type (R	classType
	add_count (RaddCount
player_name (	R
playerName_
gain_treasure_req_kill_record>
boss_spawn_point_template_id (RbossSpawnPointTemplateId_
gain_treasure_kill_record_one!
killed_stamp (RkilledStamp
	role_name (	RroleNameØ
gain_treasure_kill_record_end>
boss_spawn_point_template_id (RbossSpawnPointTemplateIdN

record_arr (2/.faith.game_proto.gain_treasure_kill_record_oneR	recordArrG
"gain_treasure_req_gain_item_record!
record_index (RrecordIndexì
'gain_treasure_gain_item_record_info_one
	role_name (	RroleName>
boss_spawn_point_template_id (RbossSpawnPointTemplateId(
item_template_id (RitemTemplateId!
killed_stamp (RkilledStamp!
record_index (RrecordIndex
	role_guid (RroleGuid~
"gain_treasure_gain_item_record_endX

record_arr (29.faith.game_proto.gain_treasure_gain_item_record_info_oneR	recordArrê
gain_treasure_boss_info_one>
boss_spawn_point_template_id (RbossSpawnPointTemplateId
	boss_guid (RbossGuid,
next_refresh_stamp (RnextRefreshStamp*
first_player_name (	RfirstPlayerName
player_guid (R
playerGuid
map_type (RmapTypei
gain_treasure_boss_info_allJ
	boss_info (2-.faith.game_proto.gain_treasure_boss_info_oneRbossInfoM
buy_raid_count
map_temp_id (R	mapTempId
	buy_count (RbuyCountQ
buy_raid_count_end
map_temp_id (R	mapTempId
	buy_count (RbuyCountR
boss_island_info_one
	info_type (RinfoType

info_value (R	infoValue5
boss_island_info_all

info_value (R	infoValueñ
server_config_all
	server_id (RserverId&
is_cross_server (RisCrossServer
	server_ip (	RserverIp
server_name (	R
serverName\
gm_begin_cross$
is_begin_cross (RisBeginCross$
gate_server_id (RgateServerIdH
gm_begin_cross_one
gm_type (RgmType
is_begin (RisBegin6
gm_begin_cross_all 
is_begin_arr (R
isBeginArr.
is_close_exchange
is_close (RisClose9
gm_begin_cross_pk$
is_begin_cross (RisBeginCross<
gm_begin_cross_harry$
is_begin_cross (RisBeginCrossD
gm_begin_cross_territory_war$
is_begin_cross (RisBeginCrossA
gm_begin_cross_world_boss$
is_begin_cross (RisBeginCross=
gm_begin_cross_ladder$
is_begin_cross (RisBeginCrossÕ
pk_one_player_msg!
player_guidA (RplayerGuidA!
player_guidB (RplayerGuidB
player_name (	R
playerName
win_num (RwinNum
	server_id (RserverId
	is_winner (RisWinner£
cross_server_one_island_msg
war_idex (RwarIdex&
island_pk_state (RislandPkStateB

player_msg (2#.faith.game_proto.pk_one_player_msgR	playerMsg!
winner_guidA (RwinnerGuidA!
winner_guidB (RwinnerGuidB

is_end_war (RisEndWar
pk_count (RpkCountd
cross_server_all_msgL

island_msg (2-.faith.game_proto.cross_server_one_island_msgR	islandMsgs
cross_server_map_msg
	map_state (RmapState
	left_time (RleftTime!
is_in_the_war (R
isInTheWarñ
cross_server_pk_map_island_msg
	map_state (RmapState
island_idex (R
islandIdex
	left_time (RleftTime
war_idex (RwarIdex
cross_server_pk_map_transfer:
cross_server_pk_state_end_msg
pk_state (RpkState0
get_world_boss_damage
boss_id (RbossId[
template_table_param_string

param_name (	R	paramName

param_date (	R	paramDateX
template_table_param_int

param_name (	R	paramName

param_date (R	paramDate”
template_table_dataP
param_string (2-.faith.game_proto.template_table_param_stringRparamStringG
	param_int (2*.faith.game_proto.template_table_param_intRparamInt!
attribute_id (RattributeId£
set_template_tableJ
template_data (2%.faith.game_proto.template_table_dataRtemplateData

tabel_name (	R	tabelName"
if_first_send (RifFirstSend
get_world_level5
send_world_level!
server_level (RserverLevel≈
cross_server_harry_one_msg
	server_id (RserverId!
server_money (RserverMoney#
special_money (RspecialMoney,
first_player_guidA (RfirstPlayerGuidA,
first_player_guidB (RfirstPlayerGuidB.
second_player_guidA (RsecondPlayerGuidA.
second_player_guidB (RsecondPlayerGuidB,
third_player_guidA (RthirdPlayerGuidA,
third_player_guidB	 (RthirdPlayerGuidB*
first_player_name
 (	RfirstPlayerNamen
cross_server_harry_all_msgP
all_harry_msg (2,.faith.game_proto.cross_server_harry_one_msgRallHarryMsg
give_server_item_msgY
role_harry_msg_info_one

harry_idex (R	harryIdex
harry_value (R
harryValueA
role_harry_msg_info_all&
harry_value_arr (RharryValueArr4
transfer_to_harry_map
	server_id (RserverId-
random_array

random_num (R	randomNumJ
$cross_server_world_boss_map_transfer"
boss_map_type (RbossMapType
get_cross_boss_show_infoÜ
set_cross_boss_show_info
player_name (	R
playerName
legion_name (	R
legionName(
boss_residue_num (RbossResidueNum
get_cross_boss_damage_list
cross_boss_rank_info!
legion_value (RlegionValue
legion_name (	R
legionName#
legion_damage (	RlegionDamage»
cross_boss_legion_rank_listC
	rank_info (2&.faith.game_proto.cross_boss_rank_infoRrankInfo
legion_rank (R
legionRank#
legion_damage (	RlegionDamage
boss_tmp_id (R	bossTmpIdt
cross_boss_legion_rank_list_allQ
rank_info_all (2-.faith.game_proto.cross_boss_legion_rank_listRrankInfoAllV
cross_boss_is_dead
boss_tmp_id (R	bossTmpId 
boss_is_dead (R
bossIsDead3
send_boss_god_begin_time
boss_id (RbossId,
get_boss_god_time
boss_id (RbossIdH
send_boss_god_time
god_time (RgodTime
boss_id (RbossId>
get_map_boss_is_dead&
map_template_id (RmapTemplateIda
oracle_trial_customs_info#
customs_state (RcustomsState
reward_stat (R
rewardStat›
oracle_trial_info_all

hight_rank (R	hightRank
max_tier (RmaxTier!
max_commping (RmaxCommpingF
all_info (2+.faith.game_proto.oracle_trial_customs_infoRallInfo
show_prompt (R
showPrompt
send_oracle_trial_commpingup>
oracle_trial_commpingup_end
error_index (R
errorIndexX
get_oracle_trial_customs_award
tier_num (RtierNum
	award_num (RawardNumF
#send_oracle_trial_customs_award_end
error_index (R
errorIndex
get_oracle_trial_ranku
"send_oracle_trial_funben_over_info
map_id (RmapId
	old_statr (RoldStatr
	new_statr (RnewStatrY
send_server_name$
is_self_server (RisSelfServer
server_name (	R
serverName
send_transfer_new_map_errorô
boss_damage_list_item
	role_name (	RroleName
role_guid_a (R	roleGuidA
role_guid_b (R	roleGuidB!
damage_value (	RdamageValue
damage_rate (R
damageRate
is_team (RisTeam"
legion_guid_a (RlegionGuidA"
legion_guid_b (RlegionGuidB∞
send_boss_damage_list_msgJ
ranking_list (2'.faith.game_proto.boss_damage_list_itemRrankingListH
helper_list (2'.faith.game_proto.boss_damage_list_itemR
helperList
	my_damage (	RmyDamage
damage_rate (R
damageRate
is_end (RisEnd(
boss_template_id (RbossTemplateId!
send_leave_boss_damage_list_msgï
help_succes_list_item
	role_name (	RroleName
role_guid_a (R	roleGuidA
role_guid_b (R	roleGuidB
template_id (R
templateIdü
send_help_success_msgD
	help_list (2'.faith.game_proto.help_succes_list_itemRhelpList#
helper_reward (RhelperReward
	my_reward (RmyRewardÃ
send_the_attacker_info
	role_name (	RroleName
role_guid_a (R	roleGuidA
role_guid_b (R	roleGuidB
template_id (R
templateId

role_level (R	roleLevel
is_npc (RisNpc0
buy_simple_boss_num
map_type (RmapType:
buy_simple_boss_num_end
error_index (R
errorIndexC
is_can_transfer_other_line%
transfer_state (RtransferStatea
legion_auction_item_showE
item_all (2*.faith.game_proto.legion_aunction_item_oneRitemAllt
legion_aunction_item_one 
item_temp_id (R
itemTempId
item_num (RitemNum
	item_lock (RitemLock•
pk_king_player_info
kill_num (RkillNum"
high_kill_num (RhighKillNum
cur_rank (RcurRank
	cur_score (RcurScore
is_dead (RisDeadn
pk_king_kill_num_info
	role_name (	RroleName
kill_num (RkillNum

class_type (R	classType∏
pk_king_map_state_info
map_time (RmapTime(
map_time_remaing (RmapTimeRemaing$
map_time_state (RmapTimeState3
map_time_state_remaing (RmapTimeStateRemaing2
pk_king_map_game_over
end_type (RendTypep
pk_king_rank_list@
	rank_list (2#.faith.game_proto.pk_king_rank_infoRrankList
rank_num (RrankNum≠
pk_king_rank_info
guid_A (RguidA
guid_B (RguidB
	role_name (	RroleName
	score_num (RscoreNum
rank_num (RrankNum
is_sur (RisSure
send_need_break_line_login&
map_template_id (RmapTemplateId
active_type (R
activeType\
send_need_break_line_login_end
is_login (RisLogin
active_type (R
activeTypep
create_pk_end_time

begin_time (R	beginTime
end_time (RendTime 
end_str_type (R
endStrType=
send_to_server_special_msg
special_msg (	R
specialMsgt
pk_king_target_list_msg
guid_a_list (R	guidAList
guid_b_list (R	guidBList
data_num (RdataNum3
server_act_is_open_end
is_hiden (RisHiden@
element_war_send_upgrade_notify

upgrade_id (R	upgradeId
element_war_send_sign_upi
element_war_send_sign_up_end
	ret_index (RretIndex
guid_a (RguidA
guid_b (RguidB@
#element_war_send_sign_up_team_ready
is_ready (RisReadyt
'element_war_send_sign_up_team_ready_end
	ret_index (RretIndex
guid_a (RguidA
guid_b (RguidB 
element_war_send_close_sign_upo
"element_war_send_close_sign_up_end
	ret_index (RretIndex
guid_a (RguidA
guid_b (RguidB’
element_match_info
guid_a (RguidA
guid_b (RguidB
	role_name (	RroleName
is_robot (RisRobot
template_id (R
templateId
	score_num (RscoreNum
	vip_level (RvipLevel∫
"element_war_send_match_success_msgF
my_team_info (2$.faith.game_proto.element_match_infoR
myTeamInfoL
other_team_info (2$.faith.game_proto.element_match_infoRotherTeamInfo…
element_war_role_info
	info_data (RinfoData
	max_index (RmaxIndex!
cross_result (RcrossResult

last_score (R	lastScore
	is_online (RisOnline
is_sign (RisSign»
element_war_map_role_info"
cur_score_num (RcurScoreNum$
team_score_num (RteamScoreNum

team_index (R	teamIndex!
kill_monster (RkillMonster
kill_player (R
killPlayero
element_war_boss_info
boss_id (RbossId
pos_x (RposX
pos_y (RposY
pos_z (RposZ≠
element_war_map_info&
first_score_num (RfirstScoreNum(
secord_score_num (RsecordScoreNum
	map_state (RmapStateF

first_boss (2'.faith.game_proto.element_war_boss_infoR	firstBossH
secord_boss (2'.faith.game_proto.element_war_boss_infoR
secordBossH
target_boss (2'.faith.game_proto.element_war_boss_infoR
targetBoss$
buff_have_team (RbuffHaveTeam$
buff_have_name (	RbuffHaveNameõ
element_war_map_end_role_info
	role_name (	RroleName*
cur_element_score (RcurElementScore
cur_rank (RcurRank
	add_score (RaddScore
	score_num (RscoreNum
	is_escape (RisEscape 
is_kill_boss (R
isKillBoss

team_index (R	teamIndex”
element_war_map_end_infoN

first_team (2/.faith.game_proto.element_war_map_end_role_infoR	firstTeamP
secord_team (2/.faith.game_proto.element_war_map_end_role_infoR
secordTeam
is_win (RisWin‡
element_war_send_boss_be_attack
pos_x (RposX
pos_y (RposY
pos_z (RposZ
is_open (RisOpen
attack_name (	R
attackName!
attack_class (RattackClass!
attack_level (RattackLevelE
element_war_get_mission_reward#
mission_index (RmissionIndexA
"element_war_get_mission_reward_end
	ret_index (RretIndex
element_war_request_role_info
element_war_buy_ticket9
element_war_buy_ticket_end
	ret_index (RretIndex"
 element_war_send_create_ready_uiê
element_war_create_boss_dead

class_type (R	classType
	role_name (	RroleName
	string_id (RstringId
boss_id (RbossId§
boss_rank_item
	role_name (	RroleName
role_guid_a (R	roleGuidA
role_guid_b (R	roleGuidB!
damage_value (	RdamageValue
rank (Rrank÷
boss_result_rank_list_end;
top_list (2 .faith.game_proto.boss_rank_itemRtopList
top_num (RtopNum9
my_rank (2 .faith.game_proto.boss_rank_itemRmyRank(
boss_template_id (RbossTemplateId£
element_map_rank_role_info
	role_name (	RroleName
	score_num (RscoreNum

team_index (R	teamIndex
guid_a (RguidA
guid_b (RguidBb
element_map_rank_infoI
	rank_info (2,.faith.game_proto.element_map_rank_role_infoRrankInfoe
map_target_show_info
guid_a (RguidA
guid_b (RguidB
image_index (R
imageIndex`
map_target_show_info_listC
	info_list (2&.faith.game_proto.map_target_show_infoRinfoList
get_map_target_show_info_list«
refresh_lucky_card_info#
activity_guid (RactivityGuid

state_info (R	stateInfo 
is_have_free (R
isHaveFree
end_item_id (R	endItemId&
lucky_card_info (RluckyCardInfo3
refresh_lucky_card

card_index (R	cardIndexN
lucky_card_operate
touch_index (R
touchIndex
is_jump (RisJump9
lucky_card_operate_end
error_index (R
errorIndex
get_element_competitionY
get_competition_lv_reward
	manual_id (RmanualId
manual_type (R
manualTypeä
get_competition_lv_reward_end$
manual_id_list (RmanualIdList(
manual_type_list (RmanualTypeList
get_type (RgetType
get_competition_lv_reward_all*
buy_competition_lv
level (RlevelG
buy_competition_lv_end
level (Rlevel
cur_exp (RcurExp5
buy_speical_manual
manual_type (R
manualTypeP
buy_speical_manual_end
is_buy (RisBuy
manual_type (R
manualTypeè
sync_role_competition
is_buy (RisBuy
season (Rseason&
buy_manual_type (RbuyManualType4
competition_begin_time (RcompetitionBeginTime0
competition_end_time (RcompetitionEndTime
level (Rlevel
cur_exp (RcurExp#
normal_reward (RnormalReward%
special_reward	 (RspecialReward
	cur_state
 (RcurState
world_level (R
worldLevel!
send_new_server_treasure_finish
transfer_attack_city_mapD
get_attack_city_info
guid_a (RguidA
guid_b (RguidBç
attack_city_info_level_info
guid_a (RguidA
guid_b (RguidB
legion_name (	R
legionName
group_level (R
groupLevelÕ
attack_city_info_list!
legion_level (RlegionLevel
	tem_level (RtemLevel
gs_rank (RgsRank 
is_have_rank (R
isHaveRank-
win_num_legion_name (	RwinNumLegionName
win_num (RwinNumJ
	list_info (2-.faith.game_proto.attack_city_info_level_infoRlistInfo%
is_in_game_time (RisInGameTime
get_attack_city_rank_list
get_attack_city_map_rank_list√
attack_city_rank_info
guid_a (RguidA
guid_b (RguidB
	role_name (	RroleName
role_source (R
roleSource
kill_player (R
killPlayer
kill_statue (R
killStatue
	kill_door (RkillDoor#
repair_statue (RrepairStatue
	kill_boss	 (RkillBoss

show_index
 (R	showIndexî
attack_city_rank_info_list
legion_name (	R
legionName#
legion_source (RlegionSourceD
	info_list (2'.faith.game_proto.attack_city_rank_infoRinfoList
guid_a (RguidA
guid_b (RguidB

mode_index (R	modeIndex

show_index (R	showIndexj
get_attack_city_rank_list_endI
	rank_list (2,.faith.game_proto.attack_city_rank_info_listRrankListÄ
attack_city_map_legion_info
	server_id (RserverId
legion_name (	R
legionName#
legion_damage (	RlegionDamageÂ
 send_attack_city_map_legion_infoJ
	info_list (2-.faith.game_proto.attack_city_map_legion_infoRinfoList
	map_state (RmapState*
next_refresh_time (RnextRefreshTime
guid_a (RguidA
guid_b (RguidB\
attack_city_operate!
operate_type (RoperateType"
prop_npc_guid (RpropNpcGuidl
attack_city_operate_end
result (Rresult"
prop_npc_guid (RpropNpcGuid
npc_id (RnpcIdÆ
attack_city_npc_info
npc_type (RnpcType
npc_id (RnpcId
cur_life (RcurLife
	buff_pile (RbuffPile
guid_a (RguidA
guid_b (RguidB[
attack_city_npc_listC
	info_list (2&.faith.game_proto.attack_city_npc_infoRinfoListV
attack_city_camp_cut
attack_name (	R
attackName

guard_name (	R	guardName
transfer_to_bron_pos‘
attack_city_map_end_player_info
guid_a (RguidA
guid_b (RguidB
	role_name (	RroleName
role_source (R
roleSource

role_group (R	roleGroup
kill_1 (Rkill1
kill_2 (Rkill2
kill_3 (Rkill3
kill_statue	 (R
killStatue
	kill_door
 (RkillDoor#
repair_statue (RrepairStatuep
attack_city_map_end_item_info
item_id (RitemId
item_num (RitemNum
	show_type (RshowTypeˆ
attack_city_map_end_infoN
	play_list (21.faith.game_proto.attack_city_map_end_player_infoRplayListL
	item_list (2/.faith.game_proto.attack_city_map_end_item_infoRitemList
role_source (R
roleSource
	role_rank (RroleRank
attack_legion_call
reset_camera.
attack_transfer_info
result (Rresult
end_attack_transfer_infoJ
!attack_city_npc_state_change_info%
notice_content (	RnoticeContent
time_feed_back_operation%
operation_type (RoperationType
main_tem_id (R	mainTemId

sub_tem_id (RsubTemId6
time_feed_back_operation_end
result (RresultÙ
send_time_feed_back_info
template_id (R
templateId&
sub_template_id (RsubTemplateId

info_state (R	infoState
get_time (RgetTime
end_time (RendTime

money_type (R	moneyType
	money_num (RmoneyNumh
send_time_feed_back_info_listG
	info_list (2*.faith.game_proto.send_time_feed_back_infoRinfoListc
time_limit_gift_operation%
operation_type (RoperationType
template_id (R
templateIdX
time_limit_gift_operation_end
result (Rresult
template_id (R
templateId 
time_limit_gift_info
template_id (R
templateId

state_info (R	stateInfo

begin_time (R	beginTime
end_time (RendTime
trigger_num (R
triggerNum
buy_num (RbuyNumb
update_time_limit_gift_infoC
	info_list (2&.faith.game_proto.time_limit_gift_infoRinfoList=
initiative_time_limit_gift
template_id (R
templateIdX
limit_time_notice_info
template_id (R
templateId

weight_num (R	weightNumê
limit_time_notice_info_listE
	info_list (2(.faith.game_proto.limit_time_notice_infoRinfoList*
black_show_notice (RblackShowNotice_
fuben_vip_mopping*
fuben_template_id (RfubenTemplateId

settlement (R
settlement/
fuben_vip_mopping_end
result (Rresult
	kill_boss'
kill_boss_end
result (Rresult,

map_search

multiplier (R
multiplier(
map_search_end
result (Rresult7
upgrade_searche_level

multiplier (R
multiplier3
upgrade_searche_level_end
result (Rresult"+
enter_scene_type	
enter 
transferbproto3