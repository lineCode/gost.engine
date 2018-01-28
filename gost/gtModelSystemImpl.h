#pragma once
#ifndef __GT_MODEL_SYSTEM_IMPL_H__
#define __GT_MODEL_SYSTEM_IMPL_H__

namespace gost{

	class gtPluginSystemImpl;
	class gtModelSystemImpl : public gtModelSystem{
		gtPluginSystemImpl* m_pluginSystem;
	public:

		gtModelSystemImpl();
		virtual ~gtModelSystemImpl();

		// גûהוכÿוע ןאלÿעü
		gtModel*	createEmpty( u32, gtVertexType* ) final;
	
	//	lptIModel*			CreateBox( f32 size = 1.f ) final;
		gtModel*	createPlane( f32 x = 1.f, f32 y = 1.f, gtSide side = gtSide::DOWN );

		gtModel*	createFromFile( const gtString& fileName );
	};

}

#endif