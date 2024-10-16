#pragma once

template<class T>
class SmartPtr
{
public:
	SmartPtr(void) : ptr(0) {}
	SmartPtr(const SmartPtr<T> &other)
	{
		copy(other.ptr);
	}
	SmartPtr<T> & operator = (const SmartPtr<T> &other)
	{
		if (this != &other)
		{
			clear();
			copy(other.ptr);
		}
		return *this;
	}
	SmartPtr(T *other)
	{
		copy(other);
	}
	template<class S>
	explicit SmartPtr(const SmartPtr<S> &other)
	{
		copy((T *)&*other);
	}
	~SmartPtr(void)
	{
		clear();
	}
	operator T * () const
	{
		return ptr;
	}
	T & operator * () const
	{
		return *ptr;
	}
	T * operator -> () const
	{
		return ptr;
	}
	bool operator ! () const
	{
		return ptr == 0;
	}
	template<class S>
	SmartPtr<T> & operator = (const SmartPtr<S> &other)
	{
		if ((void *)this != (void *)&other)
		{
			clear();
			copy(&*other);
		}
		return *this;
	}
	template<class S>
	bool operator == (const SmartPtr<S> &other)
	{
		return ptr == &*other;
	}
	bool operator < (const SmartPtr<T> &other)
	{
		return (size_t)ptr < (size_t)&*other;
	}

private:
	mutable T *ptr;

	void copy(T *other)
	{
		if (other)
			other->ref();
		ptr = other;
	}

	void clear()
	{
		if (ptr)
		{
			ptr->unref();
		}
	}
};

template<class T>
class AutoRelease
{
public:
	AutoRelease() : m_ref(0) {}
	void ref() { m_ref++; }
	void unref()
	{
		if (--m_ref <= 0)
		{
			delete (T *)this;
		}
	}
private:
	int	m_ref;
};
