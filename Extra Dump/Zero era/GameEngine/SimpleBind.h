#pragma once

template <class R>
class SimpleCaller
{
public:
	virtual ~SimpleCaller() {}
	virtual R operator () () = 0;
	virtual SimpleCaller *	clone() = 0;
};

template <class R>
class SimpleCallerF : public SimpleCaller<R>
{
private:
	R (*ptr)();

public:
	SimpleCallerF(R (*pf)()) : ptr(pf) {}
	virtual R operator () () {return (*ptr)();}
	virtual SimpleCaller *	clone() {return new SimpleCallerF<R>(*this);}
};

template <>
class SimpleCallerF<void> : public SimpleCaller<void>
{
private:
	void (*ptr)();

public:
	SimpleCallerF(void (*pf)()) : ptr(pf) {}
	virtual void operator () () {(*ptr)();}
	virtual SimpleCaller *	clone() {return new SimpleCallerF<void>(*this);}
};

template <class R, class T>
class SimpleCallerC : public SimpleCaller<R>
{
private:
	T object;

public:
	SimpleCallerC(const T &t) : object(t) {}
	virtual R operator () () {return object();}
	virtual SimpleCaller *	clone() {return new SimpleCallerC<R, T>(*this);}
};

template <class T>
class SimpleCallerC<void, T> : public SimpleCaller<void>
{
private:
	T object;

public:
	SimpleCallerC(const T &t) : object(t) {}
	virtual void operator () () {object();}
	virtual SimpleCaller *	clone() {return new SimpleCallerC<void, T>(*this);}
};

//////////////////////////////////////////////////////////////////////////

template <class R = void>
class SimpleFunction
{
private:
	SimpleCaller<R> *caller;

public:
	template <class T>
	SimpleFunction(T (*pf)()){caller = new SimpleCallerF<T>(pf);}
	template <class T>
	SimpleFunction(const T &t){caller = new SimpleCallerC<R, T>(t);}

	SimpleFunction(const SimpleFunction<R> &other)
	{caller = other.caller->clone();}

	~SimpleFunction(){delete caller;}

public:
	R operator () (){return (*caller)();}

	const SimpleFunction & operator = (const SimpleFunction<R> &other)
	{delete caller;caller = other.caller->clone();return *this;}
};

template <>
class SimpleFunction<void>
{
private:
	SimpleCaller<void> *caller;

public:
	template <class T>
	SimpleFunction(void (*pf)()){caller = new SimpleCallerF<T>(pf);}
	template <class T>
	SimpleFunction(const T &t){caller = new SimpleCallerC<void, T>(t);}

	SimpleFunction(const SimpleFunction<void> &other)
	{caller = other.caller->clone();}

	~SimpleFunction(){delete caller;}

public:
	void operator () (){(*caller)();}

	const SimpleFunction & operator = (const SimpleFunction<void> &other)
	{delete caller;caller = other.caller->clone();return *this;}
};


//////////////////////////////////////////////////////////////////////////

template <class R, class T1>
class SimpleBind1_0
{
private:
	R (*ptr)(T1);
	T1		p1;

public:
	SimpleBind1_0(R (*pf)(T1), const T1 &t1) : ptr(pf), p1(t1) {}

	R operator () () {return ptr(p1);}
};

template <class T1>
class SimpleBind1_0<void, T1>
{
private:
	void (*ptr)(T1);
	T1		p1;

public:
	template <class T1>
	SimpleBind1_0(void (*pf)(T1), const T1 &t1) : ptr(pf), p1(t1) {}

	void operator () () {ptr(p1);}
};

template <class R, class T1>
SimpleBind1_0<R, T1> SimpleBind(R (*pf)(T1), const T1 &p1)
{
	return SimpleBind1_0<R, T1>(pf, p1);
}