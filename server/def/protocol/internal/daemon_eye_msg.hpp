#ifndef _DAEMON_EYE_MSG_HPP_
#define _DAEMON_EYE_MSG_HPP_

#include "logic/type_def.hpp"
#include "base.hpp"
#include "eye_proto.pb.h"

#define	PROTOCOL_MSG_TYPE	ui16
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))

namespace faith
{
	// server -> daemon -> eye protocol
	enum daemon2eye_proto_def {
		d2e_proto_begin			= 0,
		d2e_ping_def = d2e_proto_begin,
		d2e_getcpubaseinfo_def,
		d2e_getcpuload_def,
		d2e_getmeminfo_def,
		d2e_getdiskinfo_def,
		d2e_getnetinfo_def,
		d2e_getcardload_def,
		d2e_getprocinfo_def,
		d2e_serverpid_def,
		d2e_gameproc_info_def,
		d2e_get_portinfo_def,

		d2e_file_upload_def,
		d2e_file_download_def,
		d2e_exe_syscmd_def,

		s2d_proto_begin			= 200,
		s2d_get_server_info_def = s2d_proto_begin,

		s2e_proto_begin			= 400,
		s2e_proto_def			= s2e_proto_begin,

		s2e_proto_end,
	};

	// server to eye sub protocol
	enum s2e_sub_proto_def {
		s2e_sub_ping_def = 0,
	};

	// server -> daemon -> eye protocol
	enum eye2daemon_proto_def {
		e2d_proto_begin			= 0,
		e2d_ping_def = e2d_proto_begin,
		e2d_getcpubaseinfo_def,
		e2d_getcpuload_def,
		e2d_getmeminfo_def,
		e2d_getdiskinfo_def,
		e2d_getnetbaseinfo_def,
		e2d_getcardload_def,
		e2d_getprocinfo_def,
		e2d_serverid_def,
		e2d_serverpid_def,
		e2d_get_portinfo_def,

		e2d_file_upload_def,
		e2d_file_download_def,
		e2d_exe_syscmd_def,
		e2d_updateserver_def,
		e2d_ftpdownload_def,
		e2d_update_daemon_def,

		d2s_proto_begin			= 200,

		e2s_proto_begin			= 400,
		e2s_proto_def			= e2s_proto_begin,
	};

	enum eye2daemon_sub_proto_def {
		e2d_sub_proto_begin = 0,
	};

	// Server To Eye SubProtocol
	enum e2s_sub_proto_def {
		e2s_exit_def = 1,
		e2s_gm_cmd_def,
	};

	enum file_sub_proto_def {
		file_sub_open_def = 0,
		file_sub_read_def,
		file_sub_write_def,
		file_sub_seek_def,
		file_sub_close_def,
	};

	enum eye_result {
		fseye_success = 0,
		guard_err,
		guard_create_client_err,
		guard_startup_client_err,
		guard_client_invalidhadle,
		guard_client_send_err,
		plug_opendll_err,
		plug_getproc_err,
		plug_creat_err,
		filetran_opening_err,
		filetran_app_err,
		filetran_cre_err,
		filetran_seek_err,
		filetran_close_err,
		mydb_err_opendb,
		mydb_err_query,
		mydb_err_dbuncon,
		servicestate_stopped,
		servicestate_starting,
		servicestate_running,
		servicestate_stopping,
		servicestate_restarting,
		db_err,
		db_transaction_started_err,
		db_transaction_not_started_err,
		db_rebuild_table_err,
		db_delete_table_err,
		db_table_not_exist_err,
		db_table_exist_err,
		db_begin_transaction_err,
		db_commit_transaction_err,
		db_rollback_transaction_err,
		db_get_table_data_err,
		db_adapter_not_init_err,
		db_add_table_data_err,
		db_update_table_data_err,
		as_err,
		as_bad_argument_err,
		as_user_not_exist_err,
		as_user_already_login_err,
		as_fm_task_complete,
		sec_err,
		sec_allow,
		sec_deny,
		sec_unknown,
		sec_not_found_in_cache,
		sec_not_enough_privilege_err,
		sec_ace_already_exist_err,
		sec_ace_not_exist_err,
		sec_user_already_login_err,
		sec_user_not_exist_err,
		sec_user_not_login_err,
		l2e_ExeGMCmd_err,
		l2e_ExeGMCmd_player_not_found_err,
		g2e_ExeSysCmd_done,
		g2e_ExeSysCmd_busy,
		g2e_ExeSysCmd_result,
		//--> ZhangZe Edit Start [2010/12/28] for 角色数据导出
		g2e_Role_Export_err,
		g2e_Role_Import_err,
		//<-- ZhangZe Edit End [2010/12/28]
		//--> ZhangZe Edit Start [2011/3/21] for 删除角色恢复
		g2e_recover_role_err,
		//<-- ZhangZe Edit End [2011/3/21]
		// --> xiaojiaquan Edit Start [7/1/2011] for 数据清洗
		g2e_compiledataclean_bussy,
		g2e_compilecreatethread_err,
		g2e_compiledataclean_err,
		g2e_compileabnomalexit_err,
		g2e_compilefail_err,
		g2e_cleanroledata_err,
		g2e_loadcleanprj_err,
		g2e_getcleaninterface_err,
		// --< xiaojiaquan Edit End [7/1/2011]
		mydb_more_result,
	};
}

#endif _DAEMON_EYE_MSG_HPP_