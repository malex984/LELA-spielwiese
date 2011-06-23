/* linbox/matrix/transpose.h
 * Copyright 2002 Bradford Hovinen,
 *
 * Written by Bradford Hovinen <hovinen@cis.udel.edu>,
 *
 * Evolved from dense-base.h by Bradford Hovinen
 *
 * --------------------------------------------------------
 *
 * See COPYING for license information
 */

#ifndef __LINBOX_matrix_transpose_H
#define __LINBOX_matrix_transpose_H

#include <iostream>
#include <vector>
#include <fstream>

#include "linbox/vector/subiterator.h"
#include "linbox/vector/subvector.h"
#include "linbox/vector/stream.h"
#include "linbox/matrix/traits.h"
#include "linbox/matrix/submatrix.h"

#undef _A

namespace LinBox
{

/** Matrix transpose
 * 
 * This class takes a matrix meeting the @ref{DenseMatrixBase} archetype and
 * switches the row and column iterators, giving the transpose of the original
 * matrix. It is generic with respect to the matrix given.
 * 
 * If the matrix given has limited iterators, then its transpose will have
 * limited iterators as well. In particular, if the matrix given has only row
 * iterators, then the transpose object will have only column iterators, and
 * vice versa.
 * 
 * This class differs from @ref{Transpose} in that it constructs a full matrix
 * representation, with row and/or column iterators. It does not include any
 * logic for matrix-vector products, and does not meet the
 * @ref{BlackboxArchetype} interface. Nor does it make such assumptions about
 * the matrix given.
 *
 * This class gives a constant matrix as output. It provides no iterators for
 * modification of the data in the matrix.
 * 
 * The input/output functionality of this class passes requests directly through
 * to the underlying matrix. In particular, the output will be the transpose of
 * the matrix expected and the input will expect the transpose of the matrix
 * given. Thus, it is not recommended to use TransposeMatrix for reading and
 * writing matrices, except for testing purposes.
 */
  
template <class Matrix, class Trait = typename MatrixIteratorTypes<typename MatrixTraits<Matrix>::MatrixCategory>::MatrixCategory>
class TransposeMatrix
{
    public:

	typedef typename Matrix::Element Element;
	typedef TransposeMatrix<Matrix, Trait> Self_t;

	typedef typename Matrix::ColIterator RowIterator;
	typedef typename Matrix::RowIterator ColIterator;
	typedef typename Matrix::RawIterator RawIterator;
	typedef typename Matrix::RawIndexedIterator RawIndexedIterator;
	typedef typename Matrix::ConstColIterator ConstRowIterator;
	typedef typename Matrix::ConstRowIterator ConstColIterator;
	typedef typename Matrix::ConstRawIterator ConstRawIterator;
	typedef typename Matrix::ConstRawIndexedIterator ConstRawIndexedIterator;

	typedef typename Matrix::Row Column;
	typedef typename Matrix::Row Col;
	typedef typename Matrix::Col Row;

	typedef Submatrix<Self_t> SubmatrixType;
	typedef Submatrix<const Matrix> ConstSubmatrixType;
	typedef SubmatrixType AlignedSubmatrixType;
	typedef ConstSubmatrixType ConstAlignedSubmatrixType;

	static const size_t rowAlign = 1;
	static const size_t colAlign = 1;

	typedef Matrix ContainerType;

	/** Constructor.
	 * @param  A  Underlying matrix of which to construct the transpose
	 */
	TransposeMatrix (Matrix &A)
		: _A (A)
	{}

	/** Copy constructor
	 */
	TransposeMatrix (const TransposeMatrix &M)
		: _A (M._A)
	{}

	/** Get the number of rows in the matrix
	 * @return Number of rows in matrix
	 */
	inline size_t rowdim () const
		{ return _A.coldim (); }

	/** Get the number of columns in the matrix
	 * @return Number of columns in matrix
	 */
	inline size_t coldim () const
		{ return _A.rowdim (); }

	/** @name Access to matrix elements
	 */

	//@{

	/** Set the entry at the (i, j) position to a_ij.
	 * @param i Row number, 0...rowdim () - 1
	 * @param j Column number 0...coldim () - 1
	 * @param a_ij Element to set
	 */
	void setEntry (size_t i, size_t j, const Element &a_ij)
		{ _A.setEntry (j, i, a_ij); }

	/** Erase an individual entry from the matrix.
	 * If the entry doesn't exist, then takes no action. If the underlying
	 * matrix is dense, then takes no action.
	 * @param i Row index of entry
	 * @param j Column index of entry
	 */
	void eraseEntry (size_t i, size_t j)
		{ _A.eraseEntry (j, i); }

	/** Copy the (i, j) entry into x
	 * If the entry does not exist in the matrix, x is left unchanged and false is returned.
	 *
	 * @param x Element in which to store result
	 * @param i Row index
	 * @param j Column index
	 * @return true if entry exists in matrix, false otherwise
	 */
	inline bool getEntry (Element &x, size_t i, size_t j) const
		{ return _A.getEntry (x, j, i); }

	/** @name Column of rows iterator
	 * The column of rows iterator traverses the rows of the
	 * matrix in ascending order. Dereferencing the iterator yields
	 * a row vector in dense format
	 */

	inline RowIterator rowBegin () { return _A.colBegin (); }
	inline RowIterator rowEnd () { return _A.colEnd (); }
	inline ConstRowIterator rowBegin () const { return _A.colBegin (); }
	inline ConstRowIterator rowEnd () const { return _A.colEnd (); }

	/** @name Row of columns iterator
	 * The row of columns iterator traverses the columns of the
	 * matrix in ascending order. Dereferencing the iterator yields
	 * a column vector in dense format
	 */

	inline ColIterator colBegin () { return _A.rowBegin (); }
	inline ColIterator colEnd () { return _A.rowEnd (); }
	inline ConstColIterator colBegin () const { return _A.rowBegin (); }
	inline ConstColIterator colEnd () const { return _A.rowEnd (); }

	/** @name Raw iterator
	 *
	 * The raw iterator is a method for accessing all entries in the matrix
	 * in some unspecified order. This can be used, e.g. to reduce all
	 * matrix entries modulo a prime before passing the matrix into an
	 * algorithm.
	 */

	inline RawIterator rawBegin () { return _A.rawBegin (); }
	inline RawIterator rawEnd () { return _A.rawEnd (); }
	inline ConstRawIterator rawBegin () const { return _A.rawBegin (); }
	inline ConstRawIterator rawEnd () const { return _A.rawEnd (); }

	/** @name Raw Indexed iterator
	 * Like the raw iterator, the indexed iterator is a method for 
	 * accessing all entries in the matrix in some unspecified order. 
	 * At each position of the the indexed iterator, it also provides 
	 * the row and column indices of the currently referenced entry.
	 * This is provided through it's rowIndex() and colIndex() functions.
	 */

	inline RawIndexedIterator rawIndexedBegin() { return _A.rawIndexedBegin (); }
        inline RawIndexedIterator rawIndexedEnd() { return _A.rawIndexedEnd (); }
	inline ConstRawIndexedIterator rawIndexedBegin() const { return _A.rawIndexedBegin (); }
        inline ConstRawIndexedIterator rawIndexedEnd() const { return _A.rawIndexedEnd (); }

	//@}

    protected:

	Matrix &_A;
};

// Specialization for matrices that have both row and column iterators

template <class Matrix>
class TransposeMatrix<Matrix, MatrixCategories::RowColMatrixTag>
{
    public:

	typedef typename Matrix::Element Element;
	typedef TransposeMatrix<Matrix, MatrixCategories::RowColMatrixTag> Self_t;
	typedef typename MatrixCategories::RowColMatrixTag MatrixCategory;

	typedef typename Matrix::ColIterator RowIterator;
	typedef typename Matrix::RowIterator ColIterator;
	typedef typename Matrix::RawIterator RawIterator;
	typedef typename Matrix::RawIndexedIterator RawIndexedIterator;
	typedef typename Matrix::ConstColIterator ConstRowIterator;
	typedef typename Matrix::ConstRowIterator ConstColIterator;
	typedef typename Matrix::ConstRawIterator ConstRawIterator;
	typedef typename Matrix::ConstRawIndexedIterator ConstRawIndexedIterator;

	typedef typename Matrix::Row Column;
	typedef typename Matrix::Row Col;
	typedef typename Matrix::Col Row;

	typedef Submatrix<Self_t> SubmatrixType;

	TransposeMatrix (Matrix &A) : _A (A) {}
	TransposeMatrix (const TransposeMatrix &M) : _A (M._A) {}

	inline size_t rowdim () const { return _A.coldim (); }
	inline size_t coldim () const { return _A.rowdim (); }

	inline void setEntry (size_t i, size_t j, const Element &a_ij) { _A.setEntry (j, i, a_ij); }
	inline bool getEntry (Element &x, size_t i, size_t j) const { return _A.getEntry (x, j, i); }

	inline RowIterator rowBegin () { return _A.colBegin (); }
	inline RowIterator rowEnd () { return _A.colEnd (); }
	inline ConstRowIterator rowBegin () const { return _A.colBegin (); }
	inline ConstRowIterator rowEnd () const { return _A.colEnd (); }

	inline ColIterator colBegin () { return _A.rowBegin (); }
	inline ColIterator colEnd () { return _A.rowEnd (); }
	inline ConstColIterator colBegin () const { return _A.rowBegin (); }
	inline ConstColIterator colEnd () const { return _A.rowEnd (); }

	inline RawIterator rawBegin () { return _A.rawBegin (); }
	inline RawIterator rawEnd () { return _A.rawEnd (); }
	inline ConstRawIterator rawBegin () const { return _A.rawBegin (); }
	inline ConstRawIterator rawEnd () const { return _A.rawEnd (); }

	inline RawIndexedIterator rawIndexedBegin() { return _A.rawIndexedBegin (); }
        inline RawIndexedIterator rawIndexedEnd() { return _A.rawIndexedEnd (); }
	inline ConstRawIndexedIterator rawIndexedBegin() const { return _A.rawIndexedBegin (); }
        inline ConstRawIndexedIterator rawIndexedEnd() const { return _A.rawIndexedEnd (); }

    protected:

	Matrix &_A;
};

// Specialization for matrices that have only row iterators

template <class Matrix>
class TransposeMatrix<Matrix, MatrixCategories::RowMatrixTag>
{
    public:

	typedef typename Matrix::Element Element;
	typedef TransposeMatrix<Matrix, MatrixCategories::RowMatrixTag> Self_t;
	typedef typename MatrixCategories::ColMatrixTag MatrixCategory;

	typedef typename Matrix::RowIterator ColIterator;
	typedef typename Matrix::RawIterator RawIterator;
	typedef typename Matrix::RawIndexedIterator RawIndexedIterator;
	typedef typename Matrix::ConstRowIterator ConstColIterator;
	typedef typename Matrix::ConstRawIterator ConstRawIterator;
	typedef typename Matrix::ConstRawIndexedIterator ConstRawIndexedIterator;

	typedef typename Matrix::Row Column;
	typedef typename Matrix::Row Col;

	typedef Submatrix<Self_t> SubmatrixType;

	//TransposeMatrix () {}
	TransposeMatrix (Matrix &A) : _A (A) {}
	TransposeMatrix (const TransposeMatrix &M) : _A (M._A) {}

	inline size_t rowdim () const { return _A.coldim (); }
	inline size_t coldim () const { return _A.rowdim (); }

	inline void setEntry (size_t i, size_t j, const Element &a_ij) { _A.setEntry (j, i, a_ij); }
	inline bool getEntry (Element &x, size_t i, size_t j) const { return _A.getEntry (x, j, i); }

	inline ColIterator colBegin () { return _A.rowBegin (); }
	inline ColIterator colEnd () { return _A.rowEnd (); }
	inline ConstColIterator colBegin () const { return _A.rowBegin (); }
	inline ConstColIterator colEnd () const { return _A.rowEnd (); }

	inline RawIterator rawBegin () { return _A.rawBegin (); }
	inline RawIterator rawEnd () { return _A.rawEnd (); }
	inline ConstRawIterator rawBegin () const { return _A.rawBegin (); }
	inline ConstRawIterator rawEnd () const { return _A.rawEnd (); }

	inline RawIndexedIterator rawIndexedBegin() { return _A.rawIndexedBegin (); }
        inline RawIndexedIterator rawIndexedEnd() { return _A.rawIndexedEnd (); }
	inline ConstRawIndexedIterator rawIndexedBegin() const { return _A.rawIndexedBegin (); }
        inline ConstRawIndexedIterator rawIndexedEnd() const { return _A.rawIndexedEnd (); }

    protected:

	Matrix &_A;
};

// Specialization for matrices that have only column iterators

template <class Matrix>
class TransposeMatrix<Matrix, MatrixCategories::ColMatrixTag>
{
    public:

	typedef typename Matrix::Element Element;
	typedef TransposeMatrix<Matrix, MatrixCategories::ColMatrixTag> Self_t;
	typedef typename MatrixCategories::RowMatrixTag MatrixCategory;

	typedef typename Matrix::ColIterator RowIterator;
	typedef typename Matrix::RawIterator RawIterator;
	typedef typename Matrix::RawIndexedIterator RawIndexedIterator;
	typedef typename Matrix::ConstColIterator ConstRowIterator;
	typedef typename Matrix::ConstRawIterator ConstRawIterator;
	typedef typename Matrix::ConstRawIndexedIterator ConstRawIndexedIterator;

	typedef typename Matrix::Col Row;

	typedef Submatrix<Self_t> SubmatrixType;

	TransposeMatrix (Matrix &A) : _A (A) {}
	TransposeMatrix (const TransposeMatrix &M) : _A (M._A) {}

	inline size_t rowdim () const { return _A.coldim (); }
	inline size_t coldim () const { return _A.rowdim (); }

	inline void setEntry (size_t i, size_t j, const Element &a_ij) { _A.setEntry (j, i, a_ij); }
	inline bool getEntry (Element &x, size_t i, size_t j) const { return _A.getEntry (x, j, i); }

	inline RowIterator rowBegin () { return _A.colBegin (); }
	inline RowIterator rowEnd () { return _A.colEnd (); }
	inline ConstRowIterator rowBegin () const { return _A.colBegin (); }
	inline ConstRowIterator rowEnd () const { return _A.colEnd (); }

	inline RawIterator rawBegin () { return _A.rawBegin (); }
	inline RawIterator rawEnd () { return _A.rawEnd (); }
	inline ConstRawIterator rawBegin () const { return _A.rawBegin (); }
	inline ConstRawIterator rawEnd () const { return _A.rawEnd (); }

	inline RawIndexedIterator rawIndexedBegin() { return _A.rawIndexedBegin (); }
        inline RawIndexedIterator rawIndexedEnd() { return _A.rawIndexedEnd (); }
	inline ConstRawIndexedIterator rawIndexedBegin() const { return _A.rawIndexedBegin (); }
        inline ConstRawIndexedIterator rawIndexedEnd() const { return _A.rawIndexedEnd (); }

    protected:

	const Matrix &_A;
};

} // namespace LinBox

#endif // __LINBOX_matrix_transpose_H

// Local Variables:
// mode: C++
// tab-width: 8
// indent-tabs-mode: t
// c-basic-offset: 8
// End:

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s:syntax=cpp.doxygen:foldmethod=syntax
