#ifndef _MSGPROC_APPEARANCE_INFO_H_
#define _MSGPROC_APPEARANCE_INFO_H_

#include "db_manager.hpp"

namespace hld
{
	struct s_client_uid;

	void save_appearance_info_to_id(uint32 connindex, const void *data_ptr, size_t data_len);
	void save_appearance_info_to_id_end(db_result_type result, uint32 connindex, s_client_uid client_uid);

	void load_appearance_info_by_id(uint32 connindex, const void *data_ptr, size_t data_len);
	void load_appearance_info_by_id_end(db_result_type result, uint32 connindex, s_client_uid client_uid, int32 appearance_id);

}

#endif
