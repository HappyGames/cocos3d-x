#ifndef _CC3_WEAK_REF_H_
#define _CC3_WEAK_REF_H_

NS_COCOS3D_BEGIN

template<class T>
class CC3WeakRef
{
public:
	CC3WeakRef() 
	{
		m_object = NULL; 
	}

	CC3WeakRef( T* object ) 
	{
		m_object = NULL; 
		set( object );
	}

	virtual ~CC3WeakRef() 
	{
		release();
	}

	CC3WeakRef<T>& operator = ( T* object )
	{
		set( object );
		return *this;
	}

	operator T* ()
	{
		return m_object;
	}

	T* operator -> ()
	{
		return m_object;
	}

	void    release()
	{
		if ( m_object )
			m_object->release();
	}

protected:
	void	set( T* object )
	{
		if ( object != m_object )
		{
			m_object = object;
		}
	}

protected:
	T*		m_object;
};

NS_COCOS3D_END

#endif
