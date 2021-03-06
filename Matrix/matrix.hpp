#ifndef MATRIX_MATRIX_HPP
#define MATRIX_MATRIX_HPP

#include <array>
#include <ostream>

template <typename Type, size_t _width, size_t _height>
class Matrix {
	using reference = Type&;
	using const_reference = const Type&;
	using pointer = Type*;
	using const_pointer = const Type*;
	using size_type = size_t;

public:
	/**
	 * @brief 	default ctor
	 */
	Matrix() = default;

	/**
	 * @brief	parametric ctor
	 * 			creates Matrix with copies of given element
	 * @param 	element
	 */
	Matrix(const Type& element) {
		for (Type& elem : _elements)
			elem = element;
	}

	/**
	 * @brief 	copy ctor
	 * @param 	m
	 */
	Matrix(const Matrix& m) : _elements(m._elements) {}

	/**
	 * @brief	move ctor
	 * @param 	m
	 */
	Matrix(Matrix&& m) noexcept : _elements(std::move(m._elements)) {}

	/**
	 * @brief	constructs Matrix from iterators of any container
	 * 			Node: doesn't check the lenght, for correct use
	 * 			make sure iterator sequence has the right length
	 * @param 	begin
	 * @param 	end
	 */
	template <typename Iter>
	Matrix(Iter begin, Iter end) {
		size_type i = 0;
		for (; begin != end; ++begin, ++i) {
			_elements[i] = *begin;
		}
	}

	/**
	 * @brief	initializer_list ctor
	 * @param 	init
	 * @throw	std::logic_error when wrong number of elements is given
	 */
	Matrix(std::initializer_list<Type> init) {
		if (init.size() != size())
			throw std::logic_error("Wrong number of arguments given.");
		size_type i = 0;
		for (auto&& e : init) {
			_elements[i] = e;
			++i;
		}
	}

	/**
	 * @brief	copy assigment operator
	 * @param 	m
	 * @return 	reference to this instance
	 */
	Matrix& operator=(const Matrix& m) {
		_elements = m._elements;
		return *this;
	}

	/**
	 * @brief	move assigment operator
	 * @param 	m
	 * @return 	reference to this instance
	 */
	Matrix& operator=(Matrix&& m) noexcept {
		_elements = std::move(m._elements);
		return *this;
	}

	/**
	 * @brief	initializer_list assigment operator
	 * @param 	init
	 * @return 	reference to this instance
	 */
	Matrix& operator=(std::initializer_list<Type> init) {
		*this = Matrix(init);
		return *this;
	}

	/**
	 * @brief	swaps with other Matrix
	 * @param 	m
	 */
	void swap(Matrix& m) noexcept {
		Matrix tmp = std::move(m);
		m = std::move(*this);
		*this = std::move(tmp);
	}

	/**
	 * @brief	size of matrix getter (width * height)
	 * @return 	size of matrix
	 */
	size_type size() const noexcept { return _elements.size(); }

	/**
	 * @brief	width of matrix getter
	 * @return 	widht of matrix
	 */
	size_type width() const noexcept { return _width; }

	/**
	 * @brief	height of matrix getter
	 * @return 	height of matrix
	 */
	size_type height() const noexcept { return _height; }

	/**
	* @brief	function returning reference to element of matrix
	* @param 	pos 		linear position of element
	* @return	reference to element
	*/
	reference operator()(size_type pos) noexcept {
		return _elements[pos];
	}

	/**
	* @brief	function returning const reference to element of matrix	
	* @param 	pos 		linear position of element
	* @return 	const reference to element
	*/
	const_reference operator()(size_type pos) const noexcept {
		return _elements[pos];
	}

	/**
	 * @brief	function returning reference to element of matrix
	 * @param 	x		position in column
	 * @param 	y		position in row
	 * @return	reference to element
	 */
	reference operator()(size_type x, size_type y) noexcept {
		return _elements[x + y * _width];
	}

	/**
	 * @brief	function returning const reference to element of matrix
	 * @param 	x 		position in column
	 * @param 	y 		position in row
	 * @return 	const reference to element
	 */
	const_reference operator()(size_type x, size_type y) const noexcept {
		return _elements[x + y * _width];
	}

	/**
	* @brief	function returning reference to element of matrix
	* @param 	pos 		linear position of element
	* @return	reference to element
	* @throw	std::out_of_range when given index isn't inside matrix boundaries
	*/
	reference at(size_type pos) {
		try {
			return _elements.at(pos);
		} catch (const std::out_of_range&) {
			throw std::out_of_range("Given index is outside the matrix boundaries.");
		}
	}

	/**
	* @brief	function returning const reference to element of matrix
	* @param 	pos 		linear position of element
	* @return 	const reference to element
	* @throw	std::out_of_range when given index isn't inside matrix boundaries
	*/
	const_reference at(size_type pos) const {
		return const_cast<const_reference>(const_cast<Matrix*>(this)->at(pos));
	}

	/**
	 * @brief	function returning reference to element of matrix
	 * @param 	x		position in column
	 * @param 	y		position in row
	 * @return	reference to element
	 * @throw	std::out_of_range when given index isn't inside matrix boundaries
	 */
	reference at(size_type x, size_type y) {
		if (x >= _width || y >= _height)
			throw std::out_of_range("Given index is outside the matrix boundaries.");
		return _elements.at(x + y * _width);
	}

	/**
	 * @brief	function returning const reference to element of matrix
	 * @param 	x 		position in column
	 * @param 	y 		position in row
	 * @return 	const reference to element
	 * @throw	std::out_of_range when given index isn't inside matrix boundaries
	 */
	const_reference at(size_type x, size_type y) const {
		return const_cast<const_reference>(const_cast<Matrix*>(this)->at(x, y));
	}

	/**
	 * @brief	multiplication of matrices
	 * @param 	m 	other matrix
	 * @return 	result of multiplication
	 */
	template <size_type x>
	Matrix<Type, x, _height> operator*(const Matrix<Type, x, _width>& m) {
		Matrix<Type, x, _height> tmp;
		for (size_type j = 0; j < _height; ++j) {
			for (size_type k = 0; k < x; ++k) {
				Type sum{};
				for (size_type i = 0; i < _width; ++i) {
					sum += this->at(i, j) * m.at(k, i);
				}
				tmp.at(k, j) = sum;
			}
		}
		return tmp;
	}

	/**
	 * @brief	self multiplication by other matrix
	 * @param 	m
	 * @return 	self reference
	 */
	Matrix& operator*=(const Matrix& m) noexcept {
		*this = *this * m;
		return *this;
	}

	/**
	 * @brief	self multiplication by scalar
	 * @param 	scalar
	 * @return	self reference
	 */
	Matrix& operator*=(const Type& scalar) noexcept {
		for (auto& elem : _elements)
			elem *= scalar;
		return *this;
	}

	/**
	 * @brief	self addition with other matrix
	 * @param 	m
	 * @return 	self reference
	 */
	Matrix& operator+=(const Matrix& m) noexcept {
		*this = *this + m;
		return *this;
	}

	/**
	 * @brief	self substraction with other matrix
	 * @param 	m
	 * @return	self referecne
	 */
	Matrix& operator-=(const Matrix& m) noexcept {
		*this = *this - m;
		return *this;
	}

	/**
	 * @brief	addition of two matrices
	 * @param 	m
	 * @return 	result of addition
	 */
	Matrix operator+(const Matrix& m) const {
		Matrix tmp;
		for (size_type i = 0; i < size(); ++i)
			tmp._elements[i] = _elements[i] + m._elements[i];
		return tmp;
	}

	/**
	 * @brief	substraction of two matrices
	 * @param 	m
	 * @return 	resulf of substraction
	 */
	Matrix operator-(const Matrix& m) const {
		Matrix tmp;
		for (size_type i = 0; i < size(); ++i)
			tmp._elements[i] = _elements[i] - m._elements[i];
		return tmp;
	}

	/**
	 * @brief	matrix multiplication by scalar
	 * @param 	scalar
	 * @param 	m
	 * @return 	result of multiplication
	 */
	Matrix operator*(const Type& scalar) const {
		Matrix tmp;
		for (size_type i = 0; i < size(); ++i)
			tmp._elements[i] = scalar *  _elements[i];
		return tmp;
	}

	friend Matrix operator*(const Type& scalar, const Matrix& m) {
		return m * scalar;
	}

private:
	std::array<Type, _width * _height> _elements = std::array<Type, _width * _height>();
};

/**
 * @brief	function for printing matrix to ostream
 * @param 	os
 * @param 	m
 * @return 	reference to ostream aquired via params
 */
template <typename T, size_t w, size_t h>
std::ostream& operator<<(std::ostream& os, const Matrix<T, w, h>& m) {
	for (size_t i = 0; i < h; ++i) {
		for (size_t j = 0; j < w; ++j) {
			os << m.at(j, i) << "\t";
		}
		os << "\n";
	}
	return os;
}

#endif //MATRIX_MATRIX_HPP