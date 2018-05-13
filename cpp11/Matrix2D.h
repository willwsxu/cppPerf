#pragma once
template <typename T, template<typename U, typename Allocator = std::allocator<U>> typename CONT>  // Allocator is required to compile
class Matrix2D : public CONT<CONT<T>>  // derive from a standard container
{
public:
	Matrix2D() = default;
	~Matrix2D() = default;

	typedef typename CONT<CONT<T>>::size_type size_type;

	class ColIterator : public std::iterator<std::random_access_iterator_tag,
		Matrix2D,
		ptrdiff_t,
		Matrix2D*,
		Matrix2D&>
	{
		// Matrix2D& matrix;  // no copy assignment constructor due to reference data member
		Matrix2D *matrix;
		int m, n;  // matrix dimension
		int col;   // col to iterate
		int row;   // row, [0, m]
	public:
		ColIterator(Matrix2D & mx, size_type col):matrix(&mx), col(col), row(0) { }  // begin()
		ColIterator(Matrix2D & mx, size_type col, bool end) :matrix(&mx), col(col) { row = mx.size(); }  // end()
		ColIterator(const ColIterator&) = default;  // copy constructor
		ColIterator& operator=(const ColIterator& v) = default;  // lower_bound require copy assignment
		~ColIterator() = default;

		friend bool operator==(const ColIterator& it1, const ColIterator&it2) {
			return it1.row == it2.row;
		}
		friend bool operator!=(const ColIterator& it1, const ColIterator&it2) {
			return it1.row != it2.row;
		}
		ColIterator& operator+=(const ptrdiff_t& movement) { row += movement; return (*this); }
		ColIterator& operator-=(const ptrdiff_t& movement) { row -= movement; return (*this); }
		ColIterator& operator++() { ++row; return (*this); }  // prefix
		ColIterator& operator--() { --row; return (*this); }
		ColIterator operator++(ptrdiff_t) { auto temp(*this); ++row; return temp; } // postfix
		ColIterator operator--(ptrdiff_t) { auto temp(*this); --row; return temp; }
		ColIterator operator+(const ptrdiff_t& movement) { 
			auto temp(*this); temp.row=row + movement; return temp; }
		ColIterator operator-(const ptrdiff_t& movement) {
			auto temp(*this); temp.row = row - movement; return temp;
		}
		ptrdiff_t operator-(const ColIterator& rhs) {return row-rhs.row; }

		T& operator*() { return (*matrix)[row][col]; }
		const T& operator*() const { return (*matrix)[row][col]; }
		T* operator->() const { return &(*matrix)[row][col]; }

		friend bool operator<(const ColIterator& lhs, const ColIterator& rhs) {	return lhs.row < rhs.row;	} // required by count
	};


	typedef ColIterator col_iter;
	col_iter begin(size_type col) {
		return col_iter(*this, col);
	}
	col_iter end(size_type col) {
		return col_iter(*this, col, true);
	}
};


