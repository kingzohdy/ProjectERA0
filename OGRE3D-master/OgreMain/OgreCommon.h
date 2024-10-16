/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef __Common_H__
#define __Common_H__
// Common stuff

namespace Ogre {

	template< typename T > struct TRect
	{
		T left, top, right, bottom;
		TRect() {}
		TRect( T const & l, T const & t, T const & r, T const & b )
			: left( l ), top( t ), right( r ), bottom( b )
		{
		}
		TRect( TRect const & o )
			: left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom )
		{
		}
		TRect & operator=( TRect const & o )
		{
			left = o.left;
			top = o.top;
			right = o.right;
			bottom = o.bottom;
			return *this;
		}
		T width() const
		{
			return right - left;
		}
		T height() const
		{
			return bottom - top;
		}
	};

	/** Structure used to define a rectangle in a 2-D floating point space.
	*/
	typedef TRect<float> FloatRect;

	/** Structure used to define a rectangle in a 2-D integer space.
	*/
	typedef TRect< long > Rect;

	/** Structure used to define a box in a 3-D integer space.
	Note that the left, top, and front edges are included but the right, 
	bottom and back ones are not.
	*/
	struct _OgreExport Box
	{
		size_t left, top, right, bottom, front, back;
		/// Parameterless constructor for setting the members manually
		Box()
		{
		}
		/** Define a box from left, top, right and bottom coordinates
		This box will have depth one (front=0 and back=1).
		@param	l	x value of left edge
		@param	t	y value of top edge
		@param	r	x value of right edge
		@param	b	y value of bottom edge
		@note Note that the left, top, and front edges are included 
		but the right, bottom and back ones are not.
		*/
		Box( size_t l, size_t t, size_t r, size_t b ):
		left(l),
			top(t),   
			right(r),
			bottom(b),
			front(0),
			back(1)
		{
			assert(right >= left && bottom >= top && back >= front);
		}
		/** Define a box from left, top, front, right, bottom and back
		coordinates.
		@param	l	x value of left edge
		@param	t	y value of top edge
		@param  ff  z value of front edge
		@param	r	x value of right edge
		@param	b	y value of bottom edge
		@param  bb  z value of back edge
		@note Note that the left, top, and front edges are included 
		but the right, bottom and back ones are not.
		*/
		Box( size_t l, size_t t, size_t ff, size_t r, size_t b, size_t bb ):
		left(l),
			top(t),   
			right(r),
			bottom(b),
			front(ff),
			back(bb)
		{
			assert(right >= left && bottom >= top && back >= front);
		}

		/// Return true if the other box is a part of this one
		bool contains(const Box &def) const
		{
			return (def.left >= left && def.top >= top && def.front >= front &&
				def.right <= right && def.bottom <= bottom && def.back <= back);
		}

		/// Get the width of this box
		size_t getWidth() const { return right-left; }
		/// Get the height of this box
		size_t getHeight() const { return bottom-top; }
		/// Get the depth of this box
		size_t getDepth() const { return back-front; }
	};

}

#endif
