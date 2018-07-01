#include "common.h"

gtModelSystemImpl::gtModelSystemImpl(){
	m_pluginSystem = (gtPluginSystemImpl*)gtMainSystem::getInstance()->getPluginSystem();
}

gtModelSystemImpl::~gtModelSystemImpl(){}

gtPtr<gtModel>	gtModelSystemImpl::createEmpty( u32 stride, gtVertexType* vt ){
	return gtPtrNew<gtModel>(new gtModelImpl( stride, vt ) );
}

gtPtr<gtModel>	gtModelSystemImpl::createPlane( f32 x, f32 y, gtSide side ){

	gtVertexType vt[ 4 ] = {
		gtVertexType::Position,
		gtVertexType::UV,
		gtVertexType::Normal,
		gtVertexType::End
	};

	gtPtr<gtModel> model = createEmpty( gtStrideStandart, &vt[ 0u ] );
	if( !model.data() ){
        const char16_t* msg = u"Can not allocate memory for gtModel";
		gtLogWriter::printWarning( msg );
		return nullptr;
	}

	const u32 i_count = 6U;
	const u32 v_count = 4U;
	gtSubModel* subModel = model->addSubModel( v_count, i_count, gtStrideStandart );

	if( !subModel )
		return nullptr;

	gtQuaternion q;



	// down
	// up
	switch( side ){
		case gost::gtSide::Left:
		q.set( v3f( 0.f, math::PI, math::PI/2.f ) );
		break;
		case gost::gtSide::Right:
		q.set( v3f( 0.f, 0.f, -math::PI/2.f ) );
		q = q * gtQuaternion( v3f( 0.f, math::PI, 0.f ) );
		break;
		case gost::gtSide::Up:
		q.set( v3f( math::PI, 0.f, 0.f ) );
		break;
		default:
		case gost::gtSide::Down:
		q.set( v3f( 0.f, 0.f, 0.f ) );
		break;
		case gost::gtSide::Front:
		q.set( v3f( math::PI/2.f, 0.f, 0.f ) );
		q = q * gtQuaternion( v3f( 0.f, 0.f, -math::PI/2.f ) );
		break;
		case gost::gtSide::Back:
		q.set( v3f( math::PI/2.f, 0.f, 0.f ) );
		q = q * gtQuaternion( v3f( 0.f, math::PI, 0.f ) );
		break;
	}

	f32 x1 = -x,
		y1 = 0.f,
		z1 = -y;

	f32 x2 =  x,
		y2 = 0.f,
		z2 = -y;

	f32 x3 =  x,
		y3 = 0.f,
		z3 =  y;

	f32 x4 = -x,
		y4 = 0.f,
		z4 =  y;



	u8 * v = &subModel->m_vertices[ 0u ];

	struct vert_t{
		v4f pos;
		v2f uv;
		v3f nor;
	};

	vert_t vert[ 4u ];

	vert[ 0u ].pos.set( x1, y1, z1, 1.f );
	vert[ 1u ].pos.set( x2, y2, z2, 1.f );
	vert[ 2u ].pos.set( x3, y3, z3, 1.f );
	vert[ 3u ].pos.set( x4, y4, z4, 1.f );

	vert[ 0u ].nor.set( 0.f, 1.f, 0.f );
	vert[ 1u ].nor.set( 0.f, 1.f, 0.f );
	vert[ 2u ].nor.set( 0.f, 1.f, 0.f );
	vert[ 3u ].nor.set( 0.f, 1.f, 0.f );

	vert[ 0u ].uv.set( 0.f, 1.f );
	vert[ 1u ].uv.set( 0.f, 0.f );
	vert[ 2u ].uv.set( 1.f, 0.f );
	vert[ 3u ].uv.set( 1.f, 1.f );

	memcpy( v, &vert[0u], gtStrideStandart * 4u );

	subModel->rotate( q );

	const u16 u[i_count] = {0U,1U,2U,0U,2U,3U};

	subModel->fillIndices( u );

	model->updateBoundingVolume();

	return model;
}

gtPtr<gtModel>	gtModelSystemImpl::createCube( f32 sz ){
	auto up = createPlane( sz, sz, gtSide::Down );
	auto down = createPlane( sz, sz, gtSide::Up );
	auto right = createPlane( sz, sz, gtSide::Left );
	auto left = createPlane( sz, sz, gtSide::Right );
	auto front = createPlane( sz, sz, gtSide::Back );
	auto back = createPlane( sz, sz, gtSide::Front );

	up->getSubModel( 0u )->move(   v3f( 0.f, sz, 0.f ) );
	down->getSubModel( 0u )->move( v3f( 0.f, -sz, 0.f ) );
	left->getSubModel( 0u )->move( v3f( sz, 0.f, 0.f ) );
	right->getSubModel( 0u )->move( v3f( -sz, 0.f, 0.f ) );
	front->getSubModel( 0u )->move( v3f( 0.f, 0.f, sz ) );
	back->getSubModel( 0u )->move( v3f( 0.f, 0.f, -sz ) );

	gtVertexType vt[ 4 ] = {
		gtVertexType::Position,
		gtVertexType::UV,
		gtVertexType::Normal,
		gtVertexType::End
	};

	auto cube = createEmpty( gtStrideStandart, vt );

	auto * sub = up->getSubModel(0u);
	sub->append(down->getSubModel(0u));
	sub->append(right->getSubModel(0u));
	sub->append(left->getSubModel(0u));
	sub->append(front->getSubModel(0u));
	sub->append(back->getSubModel(0u));

	cube->addSubModel( sub );

	cube->updateBoundingVolume();

	return cube;
}

gtPtr<gtModel>	gtModelSystemImpl::createFromFile( const gtString& fileName ){
	return gtPtrNew<gtModel>(m_pluginSystem->importModel( fileName ));
}

/*
Copyright (c) 2018 532235

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