#pragma once
#pragma once
template<typename T> class Singleton
{
protected:
	static T* s_Inst;
public:
	Singleton(const Singleton&) = delete;

	Singleton()
	{
		s_Inst = static_cast<T*>(this);
	}

	~Singleton(void)
	{
		s_Inst = nullptr;
	}

	static T* Instance(void)
	{
		if (s_Inst == nullptr)
			s_Inst = new T;
		return (s_Inst);
	}
};

template<typename T>
T* Singleton<T>::s_Inst = nullptr;