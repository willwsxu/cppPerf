#pragma once
template <typename T, template<typename U, typename Allocator = std::allocator<U>> typename CONT>  // Allocator is required to compile
class Matrix2D : public CONT<CONT<T>>
{
public:
	Matrix2D() = default;
	~Matrix2D() = default;

	class ColIterator : public std::iterator<std::random_access_iterator_tag,
		Matrix2D,
		ptrdiff_t,
		Matrix2D*,
		Matrix2D&>
	{
		Matrix2D& matrix;
		int m, n;  // matrix dimension
	public:
		ColIterator(Matrix2D* ptr = nullptr) { m_ptr = ptr; }

	};
};


