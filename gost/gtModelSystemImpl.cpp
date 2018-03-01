#include "common.h"

gtModelSystemImpl::gtModelSystemImpl(){
	m_pluginSystem = (gtPluginSystemImpl*)gtMainSystem::getInstance()->getPluginSystem();
#ifdef GT_DEBUG
	m_debugName.assign(u"gtModelSystemImpl");
#endif
}

gtModelSystemImpl::~gtModelSystemImpl(){
}

gtModel*	gtModelSystemImpl::createEmpty( u32 stride, gtVertexType* vt ){
	gtPtr<gtModelImpl> m = gtPtrNew<gtModelImpl>(new gtModelImpl( stride, vt ) );

	if( m.data() )
		m->addRef();

	return m.data();
}

gtModel*	gtModelSystemImpl::createPlane( f32 x, f32 y, gtSide side ){

	gtVertexType vt[ 4 ] = {
		gtVertexType::position,
		gtVertexType::uv,
		gtVertexType::normal,
		gtVertexType::end
	};

	gtPtr<gtModel> model = gtPtrNew<gtModel>( createEmpty( gtStrideStandart, &vt[ 0u ] ) );
	if( !model.data() ){
		gtLogWriter::printWarning( u"Can not allocate memory for gtModel");
		return nullptr;
	}
	
	const u32 i_count = 6U;
	const u32 v_count = 4U;
	gtSubModel* subModel = model->addSubModel( v_count, i_count, gtStrideStandart );

	if( !subModel )
		return nullptr;

	gtQuaternion q;

	switch( side ){
		case gost::gtSide::LEFT:
		q.set( v3f( 0.f, 0.f, PI/2.f ) );
		break;
		case gost::gtSide::RIGHT:
		q.set( v3f( 0.f, 0.f, -PI/2.f ) );
		q = q * gtQuaternion( v3f( 0.f, PI, 0.f ) );
		break;
		case gost::gtSide::UP:
		q.set( v3f( PI, 0.f, 0.f ) );
		break;
		default:
		case gost::gtSide::DOWN:
		q.set( v3f( 0.f, 0.f, 0.f ) );
		break;
		case gost::gtSide::FRONT:
		q.set( v3f( -PI/2.f, 0.f, 0.f ) );
		q = q * gtQuaternion( v3f( 0.f, -PI/2.f, 0.f ) );
		break;
		case gost::gtSide::BACK:
		q.set( v3f( PI/2.f, 0.f, 0.f ) );
		q = q * gtQuaternion( v3f( 0.f, PI/2.f, 0.f ) );
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

	model->updateAabb();

	model->addRef();
	return model.data();
}

gtModel*	gtModelSystemImpl::createCube( f32 sz ){
	gtPtr_t(gtModel,up,createPlane( sz, sz, gtSide::DOWN ));
	gtPtr_t(gtModel,down,createPlane( sz, sz, gtSide::UP ));
	gtPtr_t(gtModel,right,createPlane( sz, sz, gtSide::LEFT ));
	gtPtr_t(gtModel,left,createPlane( sz, sz, gtSide::RIGHT ));
	gtPtr_t(gtModel,front,createPlane( sz, sz, gtSide::BACK ));
	gtPtr_t(gtModel,back,createPlane( sz, sz, gtSide::FRONT ));

	up->getSubModel( 0u )->move(   v3f( 0.f, sz, 0.f ) );
	down->getSubModel( 0u )->move( v3f( 0.f, -sz, 0.f ) );
	left->getSubModel( 0u )->move( v3f( sz, 0.f, 0.f ) );
	right->getSubModel( 0u )->move( v3f( -sz, 0.f, 0.f ) );
	front->getSubModel( 0u )->move( v3f( 0.f, 0.f, sz ) );
	back->getSubModel( 0u )->move( v3f( 0.f, 0.f, -sz ) );

	gtVertexType vt[ 4 ] = {
		gtVertexType::position,
		gtVertexType::uv,
		gtVertexType::normal,
		gtVertexType::end
	};

	gtPtr_t(gtModel,cube,createEmpty( gtStrideStandart, vt ));
	
	auto * sub = up->getSubModel(0u);
	sub->append(down->getSubModel(0u));
	sub->append(right->getSubModel(0u));
	sub->append(left->getSubModel(0u));
	sub->append(front->getSubModel(0u));
	sub->append(back->getSubModel(0u));

	cube->addSubModel( sub );
	/*cube->addSubModel( down->getSubModel(0u) );
	cube->addSubModel( right->getSubModel(0u) );
	cube->addSubModel( left->getSubModel(0u) );
	cube->addSubModel( front->getSubModel(0u) );
	cube->addSubModel( back->getSubModel(0u) );*/

	cube->updateAabb();

	cube->addRef();

	return cube.data();
}

gtModel*	gtModelSystemImpl::createFromFile( const gtString& fileName ){
	return m_pluginSystem->importModel( fileName );
}