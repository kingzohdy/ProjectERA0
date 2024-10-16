#pragma once


template<typename _Ty>
class locked
{
public:
    locked(const _Ty& _value) :
        value(_value)
    {
        ::InitializeCriticalSection(&cs);
    }
    ~locked()
    {
        ::DeleteCriticalSection(&cs);
    }

    _Ty get()
    {
        this->enter();
        _Ty _value = value;
        this->leave();
        return _value;
    }
    void set(const _Ty& _value)
    {
        this->enter();
        value = _value;
        this->leave();
    }
    operator _Ty ()
    {
        this->enter();
        _Ty _value = value;
        this->leave();
        return _value;
    }
    _Ty& operator = (const _Ty& _value)
    {
        this->enter();
        value = _value;
        this->leave();
        return value;
    }

private:
    void enter() { ::EnterCriticalSection(&cs); }
    void leave() { ::LeaveCriticalSection(&cs); }

    _Ty value;
    CRITICAL_SECTION cs;
};
