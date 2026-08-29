
’h
legion.protohld"üg
legion_proto¯
member_info
	role_guid (
	role_name (	
member_data (
add_fuel_times (
gs_value (	
contribution_value (	
week_contribution_value (	I
members_info_list4
mems_info_arr (2.hld.legion_proto.member_infoß
legion_info
legion_guid (
legion_name (	
announcement (	
legion_gs_value (	
legion_data (

member_num (2
member_list (2.hld.legion_proto.member_info
next_recruit_stamp (Ê
legion_brief_info
legion_guid (
legion_name (	

chief_guid (

chief_name (	
cur_member_num (
max_member_num (
legion_fighting_power (	
legion_level (m
create_legion
	role_guid (
legion_name (	
legion_level (
auto_accept_new_member (L
create_legion_end7
legion_info_data (2.hld.legion_proto.legion_info6
update_legion_info_all
legion_info_data_arr (N
update_legion_info_one
legion_info_index (
legion_info_value (*
update_legion_gs_value
gs_value (	•
update_legion_member_info_all
member_guid (
gs_value (	
contribution_value (	
data_arr (
week_contribution_value (	\
update_legion_member_info_one
member_guid (

info_index (

info_value (F
update_legion_member_gs_value
member_guid (
gs_value (	{
'update_legion_member_contribution_value
member_guid (
contribution_value (	
week_contribution_value (	$
dissolve_legion
	role_guid (
dissolve_legion_end!
leave_legion
	role_guid (
leave_legion_end
get_legion_list
	role_guid (
start_index (
base_fighting_power ('
only_get_auto_accept_new_member (h
get_legion_list_end
legion_list_len (8
legion_list (2#.hld.legion_proto.legion_brief_info>
apply_to_join_legion
	role_guid (
legion_guid (/
apply_to_join_legion_end
apply_stage (e
invite_player
	role_guid (
legion_guid (
new_member_name (	
invite_guid (£
invite_player_end
invite_role_guid (
invite_role_name (	
invite_role_level (?
invite_legion_info (2#.hld.legion_proto.legion_brief_info
invite_player_succeedj
applicant_info
applicant_guid (
applicant_name (	
gs_value (	
applicant_data (,
get_applicant_info_list
	role_guid (i
get_applicant_info_list_end
list_len (8
applicant_list (2 .hld.legion_proto.applicant_infoM
process_apply
	role_guid (
applicant_guid (
	is_accept (>
process_apply_end
applicant_guid (
	is_accept (
convenient_join_legion
convenient_join_legion_endH
add_member_end6
new_member_info (2.hld.legion_proto.member_info8

del_member
	role_guid (
del_member_guid ()
del_member_end
del_member_guid (F
construction_level_up
	role_guid (
construction_index (7
construction_level_up_end
construction_index (F
get_construction_buff
	role_guid (
construction_index (7
get_construction_buff_end
construction_index (7
donate_money
	role_guid (
donate_times (Q
donate_item
	role_guid (
donate_item_index (
donate_times (F

donate_end
add_legion_money (
add_battle_achievement (9
update_announcement
	role_guid (
content (	*
update_announcement_end
content (	J
legion_event
	event_idx (

extra_data (	
happen_time (D
get_legion_event_list
	role_guid (
from_happen_time (g
get_legion_event_list_end
event_list_len (2

event_list (2.hld.legion_proto.legion_event=
legion_bonus_info

mission_id (
finish_count (Q
get_legion_bonus_info

mission_id (
sub_type_id (
get_all (g
get_legion_bonus_info_end7

bonus_info (2#.hld.legion_proto.legion_bonus_info
	bonus_num (
get_legion_bonus_award_end*
send_stimulate_bonus

mission_id (?
kickout_legion_member
	role_guid (
leaver_guid (0
kickout_legion_member_end
leaver_guid (N
appoint_job_title
	role_guid (
member_guid (
	job_title (O
appoint_job_title_end
result (
	job_title (
is_to_chief (?
enter_legion_boss_map
	role_guid (
boss_map_id (E
kill_legion_boss
boss_map_template_id (
killer_name (	;
legion_boss_record
killer_name (	
data_ary (d
legion_boss_record_list

record_num (5
records (2$.hld.legion_proto.legion_boss_recordH
get_legion_boss_award
	role_guid (
boss_map_template_id (Y
get_legion_boss_award_end
boss_map_template_id (
get_battle_achievement (%
get_banquet_info
	role_guid (Ô
get_banquet_info_end
occupy_legion_guid (
is_banquet_holding (
request_hold_banquet (
banquet_level (
participant_num (
participant_max (
role_take_part_in_num (@
request_hold_banquet
	role_guid (
banquet_level (
request_hold_banquet_end)
take_part_in_banquet
	role_guid (k
take_part_in_banquet_end
participant_num (
participant_max (
role_take_part_in_num (ô
city_war_bid_info
territory_id (
index_in_terr (
legion_guid (
cur_bid_price (
is_prior_maintain (
legion_name (	
legion_chief_name (	 
legion_chief_template_id (
occupy_time_stamp	 (z
city_war_territory_info
territory_id (
	is_in_war (6
	bid_infos (2#.hld.legion_proto.city_war_bid_info&
get_city_war_info
	role_guid (Z
get_city_war_info_endA
territory_info (2).hld.legion_proto.city_war_territory_infoH
get_abstention_city_war_end
territory_id (
legion_name (	M
apply_city_war_bid
	role_guid (
terr_id (
in_terr_idx ((
apply_city_war_bid_end
result (8
req_territory_info
	role_guid (
terr_id (V
req_territory_info_end<
	terr_info (2).hld.legion_proto.city_war_territory_info©
city_war_rank_info
legion_guid (
legion_name (	

glory_glow (
occupy_terr_num (
occupy_terr_score (!
occupy_terr_highest_score (+
req_city_war_rank_info
	role_guid (U
req_city_war_rank_info_end7
	rank_info (2$.hld.legion_proto.city_war_rank_infoC
set_territory_prior_maintain
	role_guid (
terr_ids (G
 set_territory_prior_maintain_end
terr_ids (
	is_priors (8
entry_city_war_map
	role_guid (
terr_id ()
entry_city_war_map_end
terr_id (c
city_war_operate
	role_guid (
operate_type (
prop_npc_guid (
terr_id (M
city_war_operate_end
result (
prop_npc_guid (
npc_id (d
"city_war_raid_info_of_legion_score
legion_guid (
legion_name (	
legion_score (R
 city_war_raid_info_of_stronghold
stronghold_guid (
occupant_guid (z
)city_war_raid_info_of_legion_member_score
	role_guid (
	role_name (	
legion_guid (

role_score (é
city_war_raid_info
join_war_legion_num (O
legion_score_list (24.hld.legion_proto.city_war_raid_info_of_legion_score
stronghold_num (K
stronghold_list (22.hld.legion_proto.city_war_raid_info_of_stronghold
role_num (T
role_score_list (2;.hld.legion_proto.city_war_raid_info_of_legion_member_score
cur_territory_id (9
city_war_update_role_score
score (
rank (+
city_war_over
winner_legion_guid (O
"set_auto_accept_new_member_setting
	role_guid (
is_auto_accept (l
set_player_legion_aoi_info
	role_guid (
legion_guid (
legion_name (	
	job_title (1
clear_player_legion_aoi_info
	role_guid (a
$set_player_legion_boss_award_get_log
	role_guid (
get_logs_num (
get_logs ()
get_city_master_info
	role_guid (n
get_city_master_info_end
master_guid (
master_name (	
legion_guid (
legion_name (	@
get_occupation_daily_award
	role_guid (
terr_id (1
get_occupation_daily_award_end
terr_id ($
get_legion_rank
	role_guid (#
get_legion_rank_end
rank ("
legion_error

error_type ('
create_bonfire_npc
	role_guid („
bonfire_info
map_line_id (
create_stamp (
	end_stamp (
postion (
is_init (
bonfire_guid (%
add_fuel_to_fire
	role_guid (c
member_add_fuel_to_fire
fire_member_guid (

fire_times (
fire_times_history (7
recruit_in_world
	role_guid (
is_rm_cd (2
recruit_in_world_end
next_recruit_stamp (
draw_mem_daily_award
draw_bonfire_daily_award#
bonfire_dead
legion_guid (S
legion_call_operate
	role_guid (
	call_type (
operate_params (W
legion_call_operate_end
	role_guid (
	call_type (
operate_params (<
level_up_legion_skill
	role_guid (
skill_id (-
level_up_legion_skill_end
skill_id ((
auction_is_selling

is_selling (K
legion_territory_buff_operate
operate_type (
territory_id (>
legion_bonfire_map_info
map_info (
	cur_stage ( 
goto_next_stage_of_bonfire_map
transfer_to_bonfire_map#
!notice_new_item_in_legion_auction<
get_all_legion_skill_level_end
legion_skill_level (4
get_legion_answer_question_info
	role_guid (©
#get_legion_answer_question_info_end
question_template_id (
question_index (
	time_left (
succeed_num (
	is_answer (
	play_name (	
legion_answer_resultk
legion_warehouse_item_info
item_guid_A (
item_guid_B (
add_time (
	item_info ({
legion_warehouse_log
add_time (
job (
	role_name (	
operation_type (
EItemInfo_info_id ( 
req_legion_warehouse_item_info‰
"req_legion_warehouse_item_info_end?
	item_list (2,.hld.legion_proto.legion_warehouse_item_info
is_final (
is_clean (]
operation_legion_warehouse_item
	item_guid (
operation_type (
item_id (9
#operation_legion_warehouse_item_end

error_code (
req_legion_warehouse_logX
req_legion_warehouse_log_end8
log_list (2&.hld.legion_proto.legion_warehouse_log-
req_change_legion_name
legion_name (	)
req_change_legion_name_end
ret (K
req_confirm_change_legion_name
confirm_type (
legion_name (	¬
cross_city_info_one
legion_guid_a (
legion_guid_b (
	server_id (
chief_guid_a (
chief_guid_b (
legion_rank (
legion_name (	¦
cross_city_info_all<
city_info_all (2%.hld.legion_proto.cross_city_info_one
overload_chief_name (	
over_chief_guid_a (
over_chief_guid_b (
join_cross_city_war
legion_answer_end
get_legion_answer_rank^
legion_answer_info_list
legion_name (	
score (
m_rank (
m_score (0
legion_answer_award_refresh
	award_num (
go_to_legion_station_map
get_map_legion_player…
legion_dance_player_info
play_guid_a (
play_guid_b (
	play_name (	

play_level (
play_template_id (m
send_map_legion_playerA
play_info_arr (2*.hld.legion_proto.legion_dance_player_info
play_num (?
invite_legion_dance
play_guid_a (
play_guid_b (.
invite_legion_dance_end
error_index (ƒ
be_invite_legion_dance
play_guid_a (
play_guid_b (
	play_name (	

play_level (
play_template_id (\
be_invite_legion_dance_end
play_guid_a (
play_guid_b (
is_reception (
legion_dance_end 
legion_dance_interaction_begin
legion_dance_interaction_end
legion_Dance_clear_data
add_bonfire_end‚
city_war_member_info
	role_guid (
	role_name (	
player_temp_id (
week_contribution (	
	job_title (Ÿ
legion_welfare_member_info
role_guid_a (
role_guid_b (
	role_name (	
player_temp_id (
week_contribution (	
	job_title (m
city_war_members_info_list=
mems_info_arr (2&.hld.legion_proto.city_war_member_info
is_first (y
 legion_welfare_members_info_listC
mems_info_arr (2,.hld.legion_proto.legion_welfare_member_info
is_first (l
legion_strategy_world_boss_info
low_boss_id (
intermediate_boss_id (
senior_boss_id (R
legion_strategy_info
legion_guid (
	role_guid (

boss_level (U
one_world_boss_info

boss_level (
boss_id (
legion_player_num (’
all_world_boss_infoB
world_boss_info_arr (2%.hld.legion_proto.one_world_boss_info7
all_info (2%.hld.legion_proto.one_world_boss_infoQ
world_boss_score_info
	role_guid (
	role_name (	

role_score (È
world_boss_strategy_resultB
legion_score_info (2'.hld.legion_proto.world_boss_score_infoD
personal_score_info (2'.hld.legion_proto.world_boss_score_infoC
legion_extra_score (2'.hld.legion_proto.world_boss_score_infoE
personal_extra_score (2'.hld.legion_proto.world_boss_score_info
reward_level (