
#ifndef __SIM_OBJECT_POOL_H__
#define __SIM_OBJECT_POOL_H__ 

#include "logic/type_def.hpp"

namespace faith
{
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

	template<typename element_type, uint32 max_size>
	class sim_stack
	{
	public:
		sim_stack() : corser_(0) {}

		bool push(element_type elem)
		{
			if (corser_ < max_size)
			{
				element_array_[corser_] = elem;
				corser_++;
				return true;
			}
			return false;
		}

		void pop()
		{
			if (corser_ >= 1)
			{
				corser_--;
			}
		}

		element_type top()
		{
			if (corser_ >= 1)
			{
				return element_array_[corser_ - 1];
			}
			return default_elem;
		}

		element_type get(int index) const
		{
			if (index >= 0 && index < max_size)
			{
				return element_array_[index];
			}
			return default_elem;
		}

		int count() const { return corser_; }

		void reset() { corser_ = 0; }
	private:
		uint32 corser_;
		element_type element_array_[max_size];
		static element_type default_elem;
	};


	template<typename element_type, uint32 max_size>
	class sim_object_collection
	{
	public:
		sim_object_collection() : corser_(0) {}

		element_type* spawn()
		{
			element_type* ret = nullptr;
			if (corser_ < max_size)
			{
				ret = &element_array_[corser_];
				corser_++;
			}
			return ret;
		}

		void pop()
		{
			if (corser_ >= 1)
			{
				corser_--;
			}
		}

		element_type* top()
		{
			if (corser_ >= 1)
			{
				return &element_array_[corser_ - 1];
			}
			return nullptr;
		}

		element_type* get(int index)
		{
			if (index >= 0 && index < max_size)
			{
				return &element_array_[index];
			}
			return nullptr;
		}

		int count() const { return corser_; }

		void reset() { corser_ = 0; }
	private:
		uint32 corser_;
		element_type element_array_[max_size];
	};

	template<typename object_type, int32 max_size>
	class sim_object_pool
	{
		typedef sim_stack<int32, max_size> recycle_stack_type;
	public:
		sim_object_pool() : length_(max_size)
		{
			for (int i = length_ - 1; i >= 0; --i)
			{
				object_array_[i].set_sim_object_pool_id(i);
				recycle_pool_.push(i);
			}
		};

		~sim_object_pool() {};

		object_type* spawn()
		{
			if (recycle_pool_.count() > 0)
			{
				int32 idx = recycle_pool_.top();
				recycle_pool_.pop();
				if (idx < length_)
				{
					return &object_array_[idx];
				}
			}

			return nullptr;
		}

		bool recycle(object_type* obj)
		{
			if (obj)
			{
				int32 idx = obj->get_sim_object_pool_id();
				if (idx >= 0 && idx < length_)
				{
					object_array_[idx].on_recycle();
					recycle_pool_.push(idx);
					return true;
				}
			}
			return false;
		}

		void reset()
		{
			recycle_pool_.reset();
			for (int i = 0; i < length_; ++i)
			{
				recycle_pool_.push(i);
			}
		}

		int count() const { return length_ - recycle_pool_.count(); }

		object_type* get(int32 idx) 
		{
			if (idx >=0 && idx <max_size)
			{
				return &object_array_[idx];
			}
			return nullptr;
		}

	private:
		object_type object_array_[max_size];
		int32 length_;
		recycle_stack_type recycle_pool_;
	};

#define DEF_SIM_OBJECT_POOL_PROPERTY()\
	public: \
	int32 get_sim_object_pool_id() const { return sim_object_pool_id_;} \
	void set_sim_object_pool_id(int32 v) { sim_object_pool_id_ = v; } \
	private: \
	int32 sim_object_pool_id_; 
}



#endif //__SIM_OBJECT_POOL_H__