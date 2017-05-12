#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H

#include <vector>

namespace tools
{
	template<class C>
	class ObjectPool : public std::vector<C>
	{
	public:
		ObjectPool(size_t n=5, size_t maxSz=20);
		~ObjectPool();
		C* grab();
		void release(C*);
	private:
		C* allocate();
		size_t _batchSz;
		size_t _maxSz;
		std::mutex _mtx;
		std::vector<C*> _ptrVec; 
	};

	template <class C>
	ObjectPool<C>::ObjectPool(size_t n, size_t maxSz)
		: std::vector<C>(), _batchSz(n), _maxSz(maxSz)
	{
		this->reserve(maxSz);

		_ptrVec.push_back(allocate());
		std::cout<<"cap MemVec:"<< this->capacity() <<"/siz MemVec:"<<this->size()<<std::endl;
		std::cout<<"cap PtrVec:"<< _ptrVec.capacity() <<"/siz PtrVec:"<<_ptrVec.size()<<"\n"<<std::endl;
	}

	template <class C>
	ObjectPool<C>::~ObjectPool()
	{
	}

	template <class C>
	C* ObjectPool<C>::grab()
	{
		std::lock_guard<std::mutex> lock(_mtx);
		C* obj = nullptr;
		
		if (_ptrVec.size() == 0)
		{	
			if ((this->size()+_batchSz) <= _maxSz)
			{
				std::cout<<"allocating..."<<std::endl;
				obj = allocate();
			}
		}
		else
		{
			obj = _ptrVec.back();
			_ptrVec.pop_back();
		}

		std::cout<<"cap MemVec:"<< this->capacity() <<"/siz MemVec:"<<this->size()<<std::endl;
		std::cout<<"cap PtrVec:"<< _ptrVec.capacity() <<"/siz PtrVec:"<<_ptrVec.size()<<"\n"<<std::endl;
		
		return obj;
	}

	template <class C>
	void ObjectPool<C>::release(C* obj)
	{
		std::lock_guard<std::mutex> lock(_mtx);
		_ptrVec.push_back(obj);

		std::cout<<"cap MemVec:"<< this->capacity() <<"/siz MemVec:"<<this->size()<<std::endl;
		std::cout<<"cap PtrVec:"<< _ptrVec.capacity() <<"/siz PtrVec:"<<_ptrVec.size()<<"\n"<<std::endl;
	}

	template <class C>
	C* ObjectPool<C>::allocate()
	{
		C* obj;
		for (size_t i = 0; i < _batchSz-1; ++i)
		{
			obj = new C();
			this->push_back(*obj);
			_ptrVec.push_back(obj);	
		}
		
		obj = new C();
		this->push_back(*obj);
		return obj;	
	}


} // ns tools
#endif
