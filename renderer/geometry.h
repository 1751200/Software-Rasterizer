#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

template<size_t DimCols, size_t DimRows, typename T> class mat;

template <size_t DIM, typename T> struct vec {
	vec() { 
		for (size_t i = DIM; i; ) {
			i--;
			_data[i] = T();
		}
	}
	T& operator[](const size_t i) { 
		assert(i < DIM); 
		return _data[i]; 
	}
	const T& operator[](const size_t i) const { 
		assert(i < DIM); 
		return _data[i]; 
	}
private:
	T _data[DIM];
};

///////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<2, T> {
	vec() : x(T()), y(T()) {}
	vec(T X, T Y) : x(X), y(Y) {}
	template <class U> vec<2, T>(const vec<2, U> &v);
	T& operator[](const size_t i) { 
		assert(i < 2); 
		return i <= 0 ? x : y; 
	}
	const T& operator[](const size_t i) const { 
		assert(i < 2); 
		return i <= 0 ? x : y; 
	}
	float norm() { 
		return std::sqrt(x*x + y * y); 
	}

	T x, y;
};

/////////////////////////////////////////////////////////////////////////////////

template <typename T> struct vec<3, T> {
	vec() : x(T()), y(T()), z(T()) {}
	vec(T X, T Y, T Z) : x(X), y(Y), z(Z) {}
	template <class U> vec<3, T>(const vec<3, U> &v);
	T& operator[](const size_t i) { 
		assert(i < 3); 
		return i <= 0 ? x : (1 == i ? y : z); 
	}
	const T& operator[](const size_t i) const { 
		assert(i < 3); 
		return i <= 0 ? x : (1 == i ? y : z); 
	}
	float norm() { 
		return std::sqrt(x*x + y * y + z * z); 
	}
	vec<3, T> & normalize(T l = 1) { 
		*this = (*this)*(l / norm()); 
		return *this; 
	}

	T x, y, z;
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM, typename T> T operator*(const vec<DIM, T>& lVec, const vec<DIM, T>& rVec) {
	T ret = T();
	for (size_t i = DIM; i; ) {
		i--;
		ret += lVec[i] * rVec[i];
	}
	return ret;
}


template<size_t DIM, typename T>vec<DIM, T> operator+(vec<DIM, T> lVec, const vec<DIM, T>& rVec) {
	for (size_t i = DIM; i; ) {
		i--;
		lVec[i] += rVec[i];
	}
	return lVec;
}

template<size_t DIM, typename T>vec<DIM, T> operator-(vec<DIM, T> lVec, const vec<DIM, T>& rVec) {
	for (size_t i = DIM; i; ) {
		i--;
		lVec[i] -= rVec[i];
	}
	return lVec;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> operator*(vec<DIM, T> lVec, const U& rVec) {
	for (size_t i = DIM; i;) {
		i--;
		lVec[i] *= rVec;
	}
	return lVec;
}

template<size_t DIM, typename T, typename U> vec<DIM, T> operator/(vec<DIM, T> lVec, const U& rVec) {
	for (size_t i = DIM; i;) {
		i--;
		lVec[i] /= rVec;
	}
	return lVec;
}
/*
 * 如果增长向量，则向增广处填充1
 * 也可以用来截断向量
 */
template<size_t LEN, size_t DIM, typename T> vec<LEN, T> embed(const vec<DIM, T> &v, T fill = 1) {
	vec<LEN, T> ret;
	for (size_t i = LEN; i;) {
		i--;
		ret[i] = (i < DIM ? v[i] : fill);
	}
	return ret;
}
/*
 * 可用来截断向量
 */
template<size_t LEN, size_t DIM, typename T> vec<LEN, T> proj(const vec<DIM, T> &v) {
	vec<LEN, T> ret;
	for (size_t i = LEN; i; ) {
		i--;
		ret[i] = v[i];
	}
	return ret;
}

/*
 * 叉乘
 */
template <typename T> vec<3, T> cross(vec<3, T> v1, vec<3, T> v2) {
	return vec<3, T>(v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

template <size_t DIM, typename T> std::ostream& operator<<(std::ostream& out, vec<DIM, T>& v) {
	for (unsigned int i = 0; i < DIM; i++) {
		out << v[i] << " ";
	}
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<size_t DIM, typename T> struct dt {
	static T det(const mat<DIM, DIM, T>& src) {
		T ret = 0;
		for (size_t i = DIM; i; ) {
			i--;
			ret += src[0][i] * src.cofactor(0, i);
		}
		return ret;
	}
};

template<typename T> struct dt<1, T> {
	static T det(const mat<1, 1, T>& src) {
		return src[0][0];
	}
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DimRows, size_t DimCols, typename T> class mat {
	vec<DimCols, T> rows[DimRows];
public:
	mat() {}

	vec<DimCols, T>& operator[] (const size_t idx) {
		assert(idx < DimRows);
		return rows[idx];
	}

	const vec<DimCols, T>& operator[] (const size_t idx) const {
		assert(idx < DimRows);
		return rows[idx];
	}

	vec<DimRows, T> col(const size_t idx) const {
		assert(idx < DimCols);
		vec<DimRows, T> ret;
		for (size_t i = DimRows; i;) {
			i--;
			ret[i] = rows[i][idx];
		}
		return ret;
	}

	void set_col(size_t idx, vec<DimRows, T> v) {
		assert(idx < DimCols);
		for (size_t i = DimRows; i;) {
			i--;
			rows[i][idx] = v[i];
		}
	}

	static mat<DimRows, DimCols, T> identity() {
		mat<DimRows, DimCols, T> ret;
		for (size_t i = DimRows; i; ) {
			i--;
			for (size_t j = DimCols; j; ) {
				j--;
				ret[i][j] = (i == j);
			}
		}
		return ret;
	}

	T det() const {
		return dt<DimCols, T>::det(*this);
	}

	mat<DimRows - 1, DimCols - 1, T> get_minor(size_t row, size_t col) const {
		mat<DimRows - 1, DimCols - 1, T> ret;
		for (size_t i = DimRows - 1; i; ) {
			i--;
			for (size_t j = DimCols - 1; j; ) {
				j--;
				size_t row_index = i < row ? i : i + 1;
				size_t col_index = j < col ? j : j + 1;
				ret[i][j] = rows[row_index][col_index];
			}
		}
		return ret;
	}

	T cofactor(size_t row, size_t col) const {
		return get_minor(row, col).det()*((row + col) % 2 ? -1 : 1);
	}

	mat<DimRows, DimCols, T> adjugate() const {
		mat<DimRows, DimCols, T> ret;
		for (size_t i = DimRows; i; ) {
			i--;
			for (size_t j = DimCols; j; ) {
				j--;
				ret[i][j] = cofactor(i, j);
			}
		}
		return ret;
	}

	mat<DimRows, DimCols, T> invert_transpose() {
		mat<DimRows, DimCols, T> ret = adjugate();
		T tmp = ret[0] * rows[0];
		return ret / tmp;
	}

	mat<DimRows, DimCols, T> invert() {
		return invert_transpose().transpose();
	}

	mat<DimCols, DimRows, T> transpose() {
		mat<DimCols, DimRows, T> ret;
		for (size_t i = DimCols; i; ) {
			i--;
			ret[i] = this->col(i);
		}
		return ret;
	}
};

/////////////////////////////////////////////////////////////////////////////////

template<size_t DimRows, size_t DimCols, typename T> vec<DimRows, T> operator*(const mat<DimRows, DimCols, T>& lVec, const vec<DimCols, T>& rVec) {
	vec<DimRows, T> ret;
	for (size_t i = DimRows; i; ) {
		i--;
		ret[i] = lVec[i] * rVec;
	}
	return ret;
}

template<size_t Rows1, size_t Cols1, size_t Cols2, typename T>mat<Rows1, Cols2, T> 
	operator*(const mat<Rows1, Cols1, T>& lVec, const mat<Cols1, Cols2, T>& rVec) {
	mat<Rows1, Cols2, T> result;
	for (size_t i = Rows1; i; ) {
		i--;
		for (size_t j = Cols2; j; ) {
			j--;
			result[i][j] = lVec[i] * rVec.col(j);
		}
	}
		
	return result;
}

template<size_t DimRows, size_t DimCols, typename T>mat<DimCols, DimRows, T> operator/(mat<DimRows, DimCols, T> lVec, const T& rVec) {
	for (size_t i = DimRows; i; ) {
		i--;
		lVec[i] = lVec[i] / rVec;
	}
	return lVec;
}

template <size_t DimRows, size_t DimCols, class T> std::ostream& operator<<(std::ostream& out, mat<DimRows, DimCols, T>& m) {
	for (size_t i = 0; i < DimRows; i++) out << m[i] << std::endl;
	return out;
}

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2, float> Vec2f;
typedef vec<2, int>   Vec2i;
typedef vec<3, float> Vec3f;
typedef vec<3, int>   Vec3i;
typedef vec<4, float> Vec4f;
typedef mat<4, 4, float> Matrix;

#endif //__GEOMETRY_H__
