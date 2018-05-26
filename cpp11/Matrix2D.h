#pragma once
template <typename T, template<typename U, typename Allocator = std::allocator<U>> typename CONT>  // Allocator is required to compile
class Matrix2D : public CONT<CONT<T>>  // derive from a standard container
{
public:
	Matrix2D() = default;
	~Matrix2D() = default;

	typedef typename CONT<CONT<T>>::size_type size_type;

	class ColIterator : public std::iterator<std::random_access_iterator_tag, T>  // value_type is T!, not Matrix2D
	{
		friend class Matrix2D;  // allow aceess to constructor
		// Matrix2D& matrix;  // no copy assignment constructor due to reference data member
		Matrix2D *matrix;
		//int m, n;  // matrix dimension
		int col;   // col to iterate
		int row;   // row, [0, m]
		ColIterator(Matrix2D & mx, size_type col, bool end=false) :matrix(&mx), col(col), row(0) 
		{ if (end) row = mx.size(); }  // constructor for begin() and end()
	public:
		ColIterator() = default; // needed by stable_sort
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


	class const_ColIterator : public std::iterator<std::forward_iterator_tag, T> //random_access_iterator_tag
	{
		friend class Matrix2D;
		const Matrix2D *matrix;
		int col;   // col to iterate
		int row;   // row, [0, m]
		const_ColIterator(const Matrix2D & mx, size_type col, bool end = false) :matrix(&mx), col(col), row(0)
		{
			if (end) row = mx.size();
		}  // constructor for begin() and end()
	public:
		const_ColIterator(const const_ColIterator&) = default;  // copy constructor
		const_ColIterator& operator=(const const_ColIterator& v) = default;  // lower_bound require copy assignment
		~const_ColIterator() = default;

		friend bool operator==(const const_ColIterator& it1, const const_ColIterator&it2) {
			return it1.row == it2.row;
		}
		friend bool operator!=(const const_ColIterator& it1, const const_ColIterator&it2) {
			return it1.row != it2.row;
		}
		const_ColIterator& operator+=(const ptrdiff_t& movement) { row += movement; return (*this); }
		const_ColIterator& operator-=(const ptrdiff_t& movement) { row -= movement; return (*this); }
		const_ColIterator& operator++() { ++row; return (*this); }  // prefix
		const_ColIterator& operator--() { --row; return (*this); }
		const_ColIterator operator++(ptrdiff_t) { auto temp(*this); ++row; return temp; } // postfix
		const_ColIterator operator--(ptrdiff_t) { auto temp(*this); --row; return temp; }
		const_ColIterator operator+(const ptrdiff_t& movement) {
			auto temp(*this); temp.row = row + movement; return temp;
		}
		const_ColIterator operator-(const ptrdiff_t& movement) {
			auto temp(*this); temp.row = row - movement; return temp;
		}
		ptrdiff_t operator-(const const_ColIterator& rhs) { return row - rhs.row; }

		const T& operator*() const { return (*matrix)[row][col]; }
		const T* operator->() const { return &(*matrix)[row][col]; }

		operator bool() { return row < matrix->size(); }
		friend bool operator<(const const_ColIterator& lhs, const const_ColIterator& rhs) { return lhs.row < rhs.row; } // required by count
	};


	typedef const_ColIterator const_col_iter;
	const_col_iter cbegin(size_type col) {
		return const_ColIterator(*this, col);
	}
	const_col_iter cend(size_type col) {
		return const_ColIterator(*this, col, true);
	}
};


