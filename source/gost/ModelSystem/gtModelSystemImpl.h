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

		gtPtr<gtModel>	createEmpty( u32, gtVertexType* ) final;
		gtPtr<gtModel>	createPlane( f32 x = gtConst1F, f32 y = gtConst1F, gtSide side = gtSide::Down );
		gtPtr<gtModel>	createCube( f32 sz = gtConst1F );
		gtPtr<gtModel>	createFromFile( const gtString& fileName );
	};

}

#endif