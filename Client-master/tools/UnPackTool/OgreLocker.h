
#ifndef __Locker_H__
#define __Locker_H__

class LockSection
{
public:
	LockSection();
	~LockSection();

	void Lock();
	void Unlock();

private:
	void *m_pSysObj;
};

class LockFunctor
{
public:
	LockFunctor( LockSection *pSection ) : m_pSection(pSection)
	{
		if(m_pSection == NULL) return;
		m_pSection->Lock();
	}

	~LockFunctor()
	{
		m_pSection->Unlock();
	}

private:
	LockSection *m_pSection;
};


#endif