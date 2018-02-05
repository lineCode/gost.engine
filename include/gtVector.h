/*!	GOST
	\file gtVector.h
	\brief vector

	\attention Для ускоренной разработки используется шаблонный вариант, с указанием типа и размера. Из за неизвестной длинны приходится использовать массив для инициализации и т.д. Возможно он будет переписан.
*/

#pragma once
#ifndef __GT_VECTOR_H__
#define __GT_VECTOR_H__ ///< include guard

namespace gost{
		
#define x_ at(0u)
#define y_ at(1u)
#define z_ at(2u)
#define w_ at(3u)
#define a_ at(4u)
#define b_ at(5u)
#define c_ at(6u)
#define d_ at(7u)

#define GT_INITIALIZER_LIST
#ifdef GT_INITIALIZER_LIST
#define IL_BEGIN {
#define IL_END }
#else
#define IL_BEGIN
#define IL_END
#endif

		///	Вектор
	template<typename T, u32 i >
	class gtVector{
	
			///	массив с данными
		T	m_data[ i ];

	public:

			///	ctor по умолчанию
		gtVector( void ){
			GT_STATIC_ASSERT( i > 1u, gtError::GT_VECTOR_BAD_SIZE );
			fill( 0u );
		}

			/// создаёт из готового вектора
			/// \param v: вектор для копирования
		gtVector( const gtVector<T,i>& v ){
			*this = v;
		}

			///	такая инициализация
			///	vector<f32, 4> v = {1.f,2.1f,1.3f,5.14f};
			///	или vector<f32, 4>{21.f,32.1f,41.3f,55.14f}
			/// \param l: список значений
		gtVector( const std::initializer_list<T>& l ){
			set( l );
		}

			///	создаёт с указанием одной величины (все компоненты будут равнятся этой величине)
			/// \param v: значение которым должен инициализироваться вектор
		gtVector( T v ){
			fill( v );
		}

			///	установит каждый компонент равным 0
		void zero( void ){
			fill( 0u );
		}

			///	заполнит вектор указанным значением
			/// \param val: значение которым должен инициализироваться вектор
		void fill( T val ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] = static_cast<T>( val );
		}

			///	установить значение компонента
			/// \param id: номер компонента
			/// \param value: новое значение компонента
		void setComponent( u32 id, T value ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			m_data[ id ] = value;
		}

			///	Сложение векторов
			/// \param v: вектор
		void operator+=( const gtVector<T,i>& v ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] += static_cast<T>( v[ o ] );
		}

			///	Вычитание векторов
			/// \param v: вектор
		void operator-=( const gtVector<T,i>& v ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] -= static_cast<T>( v[ o ] );
		}

			///	Умножение векторов
			/// \param v: вектор
		void operator*=( const gtVector<T,i>& v ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] *= static_cast<T>( v[ o ] );
		}

			///	Умножить компоненты вектора на заданное число
			/// \param v: число
		void operator*=( T v ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] *= v;
		}

			///	Разделить на вектор
			/// \param v: вектор
		void operator/=( const gtVector<T,i>& v ){
			for( u32 o{0u}; o < i; ++o )
				m_data[ o ] /= static_cast<T>( v[ o ] );
		}

		T summ( void ){
			T r = static_cast<T>(0);
			for( u32 o{0u}; o < i; ++o )
				r += static_cast<T>( m_data[ o ] );
			return r;
		}

			///	Сложение векторов
			/// \param v: вектор
			///	\return сумма двух векторов
		gtVector<T,i> operator+( const gtVector<T,i>& v ) const {
			gtVector<T,i> r(*this);
			for( u32 o{0u}; o < i; ++o )
				r.m_data[ o ] += v[ o ];
			return r;
		}

			///	Вычитание векторов
			/// \param v: вектор
			///	\return разность двух векторов
		gtVector<T,i> operator-( const gtVector<T,i>& v ) const {
			gtVector<T,i> r(*this);
			for( u32 o{0u}; o < i; ++o )
				r[ o ] -= v[ o ];
			return r;
		}

			///	Изменение знака векта
			///	\return Новый вектор с противоположным знаком
		gtVector<T,i> operator-( void ) const {
			gtVector<T,i> r(*this);
			for( u32 o{0u}; o < i; ++o )
				r.m_data[ o ] = -r.m_data[ o ];
			return r;
		}

			///	Умножение векторов
			/// \param v: вектор
			///	\return произведение двух векторов
		gtVector<T,i> operator*( const gtVector<T,i>& v ) const {
			gtVector<T,i> r(*this);
			for( u32 o{0u}; o < i; ++o )
				r.m_data[ o ] *= v[ o ];
			return r;
		}

			///	Вычитание векторов
			/// \param v: вектор
			///	\return отношение двух векторов
		gtVector<T,i> operator/( const gtVector<T,i>& v ) const {
			gtVector<T,i> r(*this);
			for( u32 o{0u}; o < i; ++o )
				r.m_data[ o ] /= v[ o ];
			return r;
		}

			///	даёт доступ к компоненту вектора
			///	\param id: номер компонента
			/// \return значение компонента
		T&	operator()( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			///	даёт доступ к компоненту вектора. только чтение
			///	\param id: номер компонента
			/// \return значение компонента
		const T&	operator()( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			///	даёт доступ к компоненту вектора
			///	\param id: номер компонента
			/// \return значение компонента
		T&	operator[]( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			///	даёт доступ к компоненту вектора. только чтение
			///	\param id: номер компонента
			/// \return значение компонента
		const T&	operator[]( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			///	даёт доступ к компоненту вектора
			///	\param id: номер компонента
			/// \return значение компонента
		T&	at( u32 id ){
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}

			///	даёт доступ к компоненту вектора. только чтение
			///	\param id: номер компонента
			/// \return значение компонента
		const T&	at( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}
			///	получить значение компонента
			///	\param id: номер компонента
			/// \return значение компонента
		const T	getComponent( u32 id ) const {
			GT_ASSERT( id < i, "Bad component id", "id < i", id, i );
			return m_data[ id ];
		}
		
			///	установка компонентов
			///	v.set( {3.14f, 6.28f, 9.81f} );
			///	\param l: список компонентов
		void	set( const std::initializer_list<T>& l ){
			GT_ASSERT2( l.size() <= i, "l.size() <= i" );
			auto * p = &m_data[0u];
			for each( auto var in l ){
				*p++ = var;
			}
		}

			///	возвратит размер вектора
			/// \return размер вектора
		u32	getSize( void ){
			return i;
		}

			///	сравнит вектор
			///	\param v: вектор для сравнение
			/// \return \b true если вектора равны
		bool	operator==( const gtVector<T, i>& v ) const {
			for( u32 o{ 0u }; o < i; ++o ){
				if( m_data[ o ] != v.m_data[ o ] ) return false;
			}
			return true;
		}

			///	сравнит вектор
			///	\param v: вектор для сравнение
			/// \return \b true если вектора \b не равны
		bool	operator!=( const gtVector<T, i>& v ) const {
			for( u32 o{ 0u }; o < i; ++o ){
				if( m_data[ o ] != v.m_data[ o ] ) return true;
			}
			return false;
		}

		

			///	получить длину вектора
			/// \return длина вектора (от центра координат)
		T		lengthSqrt( void ){
			T r = static_cast<T>(0u);
			for( u32 o{ 0u }; o < i; ++o )
				r += (m_data[ o ] * m_data[ o ]);
			return r;
		}

			///	нормализовать вектор - нормализация означает приведение значений в формат от 0 до 1, или от -1 до 1
		void	normalize( void ){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			for( u32 o{ 0u }; o < i; ++o )
				m_data[ o ] *= invLen;
		}

			///	установит первые три значения \b для \b трёхкомпонентного вектора. Например можно для v4f установть значения xyz из v3f
			///	\param v: трёхкомпонентный вектор
		void	setXYZ( const gtVector<T, 3u>& v ){
			GT_STATIC_ASSERT(i>2u,gtError::GT_VECTOR_BAD_SIZE);
			m_data[0u] = v[0u];
			m_data[1u] = v[1u];
			m_data[2u] = v[2u];
		}

		gtVector<T,3u> getXYZ( void ){
			return gtVector<T,3u>({m_data[0u],m_data[1u],m_data[2u]});
		}

	};

	using v2f = gtVector<f32, 2u>;	///< синоним для gtVector<f32, 2u>
	using v3f = gtVector<f32, 3u>;	///< синоним для gtVector<f32, 3u>
	using v4f = gtVector<f32, 4u>;	///< синоним для gtVector<f32, 4u>
	using v5f = gtVector<f32, 5u>;	///< синоним для gtVector<f32, 5u>
	using v6f = gtVector<f32, 6u>;	///< синоним для gtVector<f32, 6u>
	using v7f = gtVector<f32, 7u>;	///< синоним для gtVector<f32, 7u>
	using v8f = gtVector<f32, 8u>;	///< синоним для gtVector<f32, 8u>
	using v2i = gtVector<s32, 2u>;	///< синоним для gtVector<s32, 2u>
	using v3i = gtVector<s32, 3u>;	///< синоним для gtVector<s32, 3u>
	using v4i = gtVector<s32, 4u>;	///< синоним для gtVector<s32, 4u>
	using v5i = gtVector<s32, 5u>;	///< синоним для gtVector<s32, 5u>
	using v6i = gtVector<s32, 6u>;	///< синоним для gtVector<s32, 6u>
	using v7i = gtVector<s32, 7u>;	///< синоним для gtVector<s32, 7u>
	using v8i = gtVector<s32, 8u>;	///< синоним для gtVector<s32, 8u>
	using v2u = gtVector<u32, 2u>;	///< синоним для gtVector<u32, 2u>
	using v3u = gtVector<u32, 3u>;	///< синоним для gtVector<u32, 3u>
	using v4u = gtVector<u32, 4u>;	///< синоним для gtVector<u32, 4u>
	using v5u = gtVector<u32, 5u>;	///< синоним для gtVector<u32, 5u>
	using v6u = gtVector<u32, 6u>;	///< синоним для gtVector<u32, 6u>
	using v7u = gtVector<u32, 7u>;	///< синоним для gtVector<u32, 7u>
	using v8u = gtVector<u32, 8u>;	///< синоним для gtVector<u32, 8u>

/// \anchor ox_gtvector_v2f_t
#define v2f_t(x,y) v2f({(x),(y)})	///< скращённая версия записи с initializer_list
#define v2i_t(x,y) v2i({(x),(y)})   ///< \ref ox_gtvector_v2f_t "смотрите это"
#define v2u_t(x,y) v2u({(x),(y)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v3f_t(x,y,z) v3f({(x),(y),(z)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v3i_t(x,y,z) v3i({(x),(y),(z)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v3u_t(x,y,z) v3u({(x),(y),(z)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v4f_t(x,y,z,w) v4f({(x),(y),(z),(w)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v4i_t(x,y,z,w) v4i({(x),(y),(z),(w)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v4u_t(x,y,z,w) v4u({(x),(y),(z),(w)})	///< \ref ox_gtvector_v2f_t "смотрите это"
#define v8f_t(x,y,z,w,x2,y2,z2,w2) v8f({(x),(y),(z),(w),(x2),(y2),(z2),(w2)})	///< \ref ox_gtvector_v2f_t "смотрите это"

	namespace math{

			///	cross product of s with a /векторное произедение
			///	\param s: first vector
			///	\param a: second vector
			///	\return cross product
		GT_FORCE_INLINE v3f cross( const v3f& s, const v3f& a ){
			v3f r;
			r.x_ = (s.y_ * a.z_) - (s.z_ * a.y_);
			r.y_ = (s.z_ * a.x_) - (s.x_ * a.z_);
			r.z_ = (s.x_ * a.y_) - (s.y_ * a.x_);
			return r;
		}

			///	dot product of s with a /скалярное произедение
			///	\param s: first vector
			///	\param a: second vector
			///	\return dot product
		GT_FORCE_INLINE f32 dot( const v3f& s, const v3f& a ){
			f32 r;
			r =  (s.x_ * a.x_);
			r += (s.y_ * a.y_);
			r += (s.z_ * a.z_);
			return r;
		}

			///	dot product of s with a /скалярное произедение
			///	\param s: first vector
			///	\param a: second vector
			///	\return dot product
		GT_FORCE_INLINE f32 dot( const v4f& s, const v3f& a ){
			f32 r;
			r =  (s.x_ * a.x_);
			r += (s.y_ * a.y_);
			r += (s.z_ * a.z_);
			return r;
		}

			///	dot product of s with a /скалярное произедение
			///	\param s: first vector
			///	\param a: second vector
			///	\return dot product
		GT_FORCE_INLINE f32 dot( const v4f& s, const v4f& a ){
			f32 r;
			r =  (s.x_ * a.x_);
			r += (s.y_ * a.y_);
			r += (s.z_ * a.z_);
			return r;
		}
	}

	namespace util{

			///	print vector in output window
		template<typename T, u32 i>
		void printVector( const gtVector<T, i>& v ){
			gtLogWriter::printInfo( u"Vector:" );
			for( u32 o{ 0u }; o < i; ++o )
				gtLogWriter::printInfo( u"\t\t%f", v.getComponent( o ) );
		}

	}




}

#endif

/*
Copyright (c) 2017, 2018 532235

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