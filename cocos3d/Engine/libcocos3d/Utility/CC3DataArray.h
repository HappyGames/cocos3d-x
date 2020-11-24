/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
 * Copyright (c) 2014-2015 Jason Wang
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#ifndef _CC3_DATA_ARRAY_H_
#define _CC3_DATA_ARRAY_H_

NS_COCOS3D_BEGIN
/** CC3DataArray manages data as an array of structure elements. */

class CC3DataArray : public CCObject
{
	DECLARE_SUPER( CCObject );
public:
	CC3DataArray();
	~CC3DataArray();
	
	/** 
	 * Indicates the size, in bytes, of each data element in this array. 
	 *
	 * In common use, an instance will contain an array of structs,
	 * and this value will indicate the size of the struct.
	 *
	 * Changing this property changes the value of the length property.
	 *
	 * This property cannot be set to zero. 
	 *
	 * The initial value of this property is 1, indicating that each element contains one byte.
	 */
	unsigned int				getElementSize();
	void						setElementSize( unsigned int size );

	/**
	 * Indicates the number of elements, each of the size indicated by the elementSize property,
	 * that can be held in this data array.
	 *
	 * Changing this property changes the value of the length property.
	 *
	 * The initial value of this property is zero.
	 */
	unsigned int				getElementCapacity();
	void						setElementCapacity( unsigned int capacity ); 

	/**
	 * Ensures that this data array can hold at least the specified number of elements,
	 * each of the size indicated by the elementSize property.
	 *
	 * If the specified element capacity is larger than the value returned by the elementCapacity
	 * property, the data array is expanded to the new capacity. if the specified element capacity
	 * is equal to or smaller than the value returned by teh elementCapacity property, no changes
	 * are made to the capacity of this data array.
	 *
	 * Changing this property may change the value of the length property.
	 */
	void						ensureElementCapacity( unsigned int elementCapacity );

	/**
	 * Returns a pointer to the element in this array at the specified index.
	 *
	 * The returned pointer will typically be cast to a structure whose size matches the
	 * elementSize property.
	 */
	void*						getElementAt( unsigned int index );

	/**
	 * Indicates that the data is ready to be used.
	 *
	 * This property has no internal use. The application can use this property to indicate
	 * that this data array has been populated with data and is ready for use.
	 */
	bool						isReady();
	void						setIsReady( bool isReady );

	/** 
	 * Initializes this instance to hold an array of data elements, where each element will
	 * be of the specified size in bytes. The initial element capacity is zero. This can be
	 * extended via the elementCapacity property.
	 */
	void						initWithElementSize( unsigned int elementSize );

	/**
	 * Allocates and initializes an autoreleased instance to hold an array of data elements,
	 * where each element will be of the specified size in bytes. The initial element capacity
	 * is zero. This can be extended via the elementCapacity property.
	 */
	static CC3DataArray*		dataArrayWithElementSize( unsigned int elementSize );

	void						init();

protected:
	std::vector<char>			m_data;
	unsigned int				m_elementSize;
	bool						m_isReady : 1;
};

NS_COCOS3D_END

#endif
