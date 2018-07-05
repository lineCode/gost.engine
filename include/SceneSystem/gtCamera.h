#pragma once
#ifndef __GT_CAMERA_H__
#define __GT_CAMERA_H__

namespace gost{

	enum class gtCameraType{
		LookAt,
		Free,
		FPS,
		Camera_2D
	};


	struct gtCameraFrustum{
		v4f m_planes[ gtConst6U ];
	};

	class gtCamera : public gtGameObject{
	public:

		virtual gtAabb*				getAabb() = 0;
		virtual f32					getAspect() = 0;
		virtual gtCameraType		getCameraType() = 0;
		virtual f32					getFar() = 0;
		virtual f32					getFOV() = 0;
		virtual f32					getNear() = 0;
		virtual gtObb*				getObb() = 0;
		virtual const gtMatrix4&	getProjectionMatrix() = 0;
		virtual const v4f&			getTarget() = 0;
		virtual const v4f&			getUpVector() = 0;
		virtual const gtMatrix4&	getViewMatrix() = 0;

		virtual void				setAspect( f32 aspect ) = 0;
		virtual void				setCameraType( gtCameraType type ) = 0;
		virtual void				setFar( f32 Far ) = 0;
		virtual void				setFOV( f32 fov ) = 0;
		virtual void				setNear( f32 Near ) = 0;
		virtual void				setTarget( const v4f& target ) = 0;
		virtual void				setUpVector( const v4f& up ) = 0;
		virtual void				setViewPort( const v4f& v ) = 0;
		
	};
}

#endif

/*
Copyright (c) 2017-2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/