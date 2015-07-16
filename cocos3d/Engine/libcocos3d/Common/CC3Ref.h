#ifndef _CC3_REF_H_
#define _CC3_REF_H_

NS_COCOS3D_BEGIN

template<class T>
class CC3Ref : public CC3WeakRef<T>
{
public:
	CC3Ref() 
	{
        CC3WeakRef<T>::m_object = NULL;
	}

	CC3Ref( T* object ) : CC3WeakRef<T>( object )
	{
        if ( CC3WeakRef<T>::m_object )
            CC3WeakRef<T>::m_object->retain();
	}

	CC3Ref<T>& operator = ( T* object )
	{
		CC3WeakRef<T>::set( object );
		return *this;
	}
};

NS_COCOS3D_END

#endif
