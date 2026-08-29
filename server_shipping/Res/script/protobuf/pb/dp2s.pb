
²(

dp2s.protohldst.proto"”(

dp2s_proto'
test_dp2s_test_msg
	role_guid (m
dp2ws_get_game_info
	server_id (

begin_time (
server_info_arr (
gm_commond_arr (»
s_unit_info
	role_guid (
account (	
	role_name (	
role_appearance (	"
role_first_kill_npc_id_arr (	
map_guid (
data_i64_ary (
data_ary («
dp2ws_load_character.
	role_info (2.hld.dp2s_proto.s_unit_info
ws_data_ary (
is_login (
is_send_chat (
worship_value (
eresult (Ó
s_arena_rank
rank_pos (
	role_guid (
	role_name (	
gs_value (
role_template_id (
robot_template_id (
military_rank_level (
	vip_level (
game_channel	 (	V
s_arena_rank_all
	row_count (/
	load_data (2.hld.dp2s_proto.s_arena_ranke
dp2ws_load_arena_rank
start_index (7
load_data_all (2 .hld.dp2s_proto.s_arena_rank_all'
dp2ws_create_robot
	is_create (,
dp2ws_arena_load_finish
	is_finish (”
s_ranking_player_info
	role_guid (
	role_name (	
ranking_type (
ranking_value (
	save_time (
supporting_guid (
supporting_name (	
	vip_level (
peak	 (
	server_id
 (
game_channel (	
standby_parameter (_
s_ranking_top_info7
data_ary (2%.hld.dp2s_proto.s_ranking_player_info
data_num (j
dp2ws_load_all_ranking_info
ranking_type (5
	data_info (2".hld.dp2s_proto.s_ranking_top_infoÇ
dp2ws_load_one_ranking_info
sender_guid (
ranking_type (
worship_add_value (
worship_type (

end_result (8
	data_info (2%.hld.dp2s_proto.s_ranking_player_infou
dp2ws_get_worship_value_end
sender (5
target (2%.hld.dp2s_proto.s_ranking_player_info
is_load (T
s_service_rank_player_info
	role_guid (
	rank_type (
role_num (e
s_service_rank_info<
data_ary (2*.hld.dp2s_proto.s_service_rank_player_info
data_num (Y
 dp2ws_load_all_service_rank_info5
all_data (2#.hld.dp2s_proto.s_service_rank_infoJ
dp2ws_load_last_server_list
	server_id (
last_server_list (	i
dp2ws_gm_set_result
listen_port (
handle_index (
result_code (

result_msg (	ß
dp2ws_save_recharge
order_id (	
	role_guid (
game_goods_id (
	pay_price (
success (
	order_num (
payment_type (
direct_diamond (

client_uid	 (
order_token
 (	­
dp2ws_send_recharge_end_result
result_type (
	order_num (
payment_type (
goods_id (
direct_diamond (
	role_guid (
order_id (	?
dp2ws_query_server_info_gm
acc_num (
role_num (X
dp2ws_gm_change_account

change_res (
listen_port (
handle_index (^
dp2ws_gm_get_role_info_arr.
	role_info (2.hld.dp2s_proto.s_unit_info
data_num (š
dp2ws_gm_get_role_info
listen_port (
handle_index (A
role_info_arr (2*.hld.dp2s_proto.dp2ws_gm_get_role_info_arr

check_type (Y
dp2ws_gm_modify_role_info
	role_guid (
listen_port (
handle_index (æ
s_red_package_base
red_package_guid (
red_package_template_id (
sender_guid (
create_time (
sender_name (	
text (	
sender_vip_level (
remain_times (
remain_money_num	 (f
load_red_package_end
data_num (<
red_package_list (2".hld.dp2s_proto.s_red_package_baseo
s_red_package_receiver
red_package_guid (
receiver_guid (
	role_name (	
	money_num (p
load_red_package_receiver_end
data_num (=
receiver_list (2&.hld.dp2s_proto.s_red_package_receiverY
aution_time_out_end
data_num (0
	data_info (2.hld.st_proto.st_auction_info½
auction_req_share_list_end

buyer_guid (

money_type (
money_value (
item_template_id (
data_num (6
	data_info (2#.hld.st_proto.st_auction_share_info`
auction_clear_old_item_end
data_num (0
	data_info (2.hld.st_proto.st_auction_info­
auction_find_bid_target_end
	role_guid (
	item_guid (
price_money_type (
price_money_value (3
auction_info (2.hld.st_proto.st_auction_infoÔ
auction_sell_end
operate_result (+
info (2.hld.st_proto.st_auction_info-
	role_info (2.hld.st_proto.st_unit_info
is_add_to_log (5

third_info (2!.hld.st_proto.st_login_fixed_data]
auction_cancel_sell
	role_guid (3
auction_info (2.hld.st_proto.st_auction_infox
auction_find_buy_target_end
	role_guid (
	item_guid (3
auction_info (2.hld.st_proto.st_auction_infoY
auction_show_sell_end
result (0
	data_info (2.hld.st_proto.st_auction_info
"load_all_gain_treasure_record_info
data_num (G
record_infos (21.hld.st_proto.st_gain_treasure_record_info_one_dbf
load_all_best_record
db_record_num (7
db_record_list (2.hld.st_proto.st_db_best_recordH
load_big_player_end1
	data_info (2.hld.st_proto.st_big_player_db