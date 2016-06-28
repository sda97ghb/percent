#ifndef MATRICES_H
#define MATRICES_H

#include <iostream>

#include <cmath>

#include <algorithm>

#include <vector>
template <typename T>
using Array = std::vector<T>;

template <typename T>
class Matrix {
protected:
    Array<T> _matrix;

    size_t _rows;
    size_t _columns;

public:
    Matrix() {}

    Matrix(size_t size) {
        _matrix = Array<T>(size * size);
        _rows = size;
        _columns = size;
    }

    Matrix(size_t rows, size_t columns) {
        _matrix = Array<T>(rows * columns);
        _rows = rows;
        _columns = columns;
    }

    Matrix(const Matrix<T> &m) :
        Matrix<T>(m.rows(), m.columns())
    {
        for (size_t i = 0; i < m.rows(); ++i)
            for (size_t j = 0; j < m.columns(); ++j)
                set(i, j, m.get(i, j));
    }

    typename Array<T>::iterator
    begin()
    {
        return _matrix.begin();
    }

    typename Array<T>::iterator
    end()
    {
        return _matrix.end();
    }

    void resize(size_t size) {
        _matrix.clear();
        _matrix = Array<T>(size * size);
        _rows = size;
        _columns = size;
    }

    void resize(size_t rows, size_t columns) {
        _matrix.clear();
        _matrix = Array<T>(rows * columns);
        _rows = rows;
        _columns = columns;
    }

    size_t rows() const { return _rows; }
    size_t columns() const { return _columns; }
    size_t size() const { return _rows == _columns ? _rows : 0; }
    size_t minSize() const { return _rows < _columns ? _rows : _columns; }
    size_t maxSize() const { return _rows > _columns ? _rows : _columns; }

    bool isEmpty() const { return _rows == 0 && _columns == 0; }

    T get(size_t i, size_t j) const { return _matrix[i + _rows * j]; }
    void set(size_t i, size_t j, T value) { _matrix[i + _rows * j] = value; }
//    T &at (size_t i, size_t j) { return _matrix[i + _rows * j]; }
//    const T &at (size_t i, size_t j) const { return _matrix[i + _rows * j]; }

    const T *toCArray() const {
        return &_matrix[0];
    }

    Matrix<T> submatrix(const Array<size_t> &rowsIndeces,
                        const Array<size_t> &columnsIndeces) const {
        Matrix<T> m(rowsIndeces.size(), columnsIndeces.size());
        for (size_t i = 0; i < rowsIndeces.size(); ++ i)
            for (size_t j = 0; j < columnsIndeces.size(); ++ j)
                m.at(i, j) = _matrix[rowsIndeces[i] +
                        _rows * columnsIndeces[j]];
        return m;
    }

#undef minor
    Matrix<T> minor(size_t i, size_t j) const {
        if (i >= _rows || j >= _columns)
            return Matrix<T>(0);
        Array<size_t> rowsIndeces = genRange(0, _rows - 1);
        Array<size_t> columnsIndeces = genRange(0, _columns - 1);
        rowsIndeces.erase(rowsIndeces.begin() + i);
        columnsIndeces.erase(columnsIndeces.begin() + j);
        return submatrix(rowsIndeces, columnsIndeces);
    }

    Matrix<T> horizcat(const Matrix<T> &m) const {
        if (_rows != m.rows())
            return Matrix<T>(0);
        if (_columns == 0 && m.columns() == 0)
            return Matrix<T>(0);
        if (_columns == 0)
            return m;
        if (m.columns() == 0)
            return *this;
        Matrix<T> mBar(_rows, _columns + m.columns());
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < _columns; ++ j)
                mBar.set(i, j, get(i, j));
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < m.columns(); ++ j)
                mBar.at(i, j + _columns) = m.at(i, j);
        return mBar;
    }

    Matrix<T> vertcat(const Matrix<T> &m) const {
        if (_columns != m.columns())
            return Matrix<T>(0);
        if (_rows == 0 && m.rows() == 0)
            return Matrix<T>(0);
        if (_rows == 0)
            return m;
        if (m.rows() == 0)
            return *this;
        Matrix<T> mBar(_rows + m.rows(), _columns);
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < _columns; ++ j)
                mBar.set(i, j, get(i, j));
        for (size_t i = 0; i < m.rows(); ++ i)
            for (size_t j = 0; j < _columns; ++ j)
                mBar.set(i + _rows, j, m.get(i, j));
        return mBar;
    }

    Matrix<T> transpose() const {
        if (_rows == 0 || _columns == 0)
            return Matrix<T>(0);
        Matrix<T> m(_columns, _rows);
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < _columns; ++ j)
                m.set(j, i, get(i, j));
        return m;
    }

    Matrix<T> inverse() const {
        if (_rows != _columns || _rows == 0)
            return Matrix<T>(0);
        Matrix<T> mBar(_rows, _columns);
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < _columns; ++ j)
                mBar.at(j, i) = pow(-1, i + j) *
                        minor(i, j).det();
        double d = det();
        return mBar / d;
    }

    Matrix<T> abs() const {
        if (_rows == 0 || _columns == 0)
            return *this;
        Matrix<T> mBar(_rows, _columns);
        for (size_t i = 0; i < _rows; ++ i)
            for (size_t j = 0; j < _columns; ++ j) {
                const T &e = _matrix[i + _rows * j];
                mBar.at(i, j) = e >= 0 ? e : -e;
            }
        return mBar;
    }

    double det() const {
        if (_rows != _columns || _rows == 0)
            return 0;
        Array<size_t> indeces;
        for (size_t i = 0; i < _rows; ++ i)
            indeces.push_back(i);
        double S = 0.0f;
        do {
            // calculate product
            double p = 1;
            for (size_t i = 0; i < _rows; ++ i)
                p *= get(i, indeces[i]);
            // calculate permutation's sign
            bool b = false;
            for (size_t i = 0; i < _rows - 1; ++ i)
                for (size_t j = i + 1; j < _rows; ++ j)
                    if (indeces[i] > indeces[j])
                        b = !b;
            if (b)
                p *= -1;
            // adding
            S += p;

        } while (std::next_permutation(indeces.begin(), indeces.end()));
        return S;
    }

//operators:
    Matrix<T> &operator += (const Matrix<T> &a) {
        if (_rows != a.rows() || _columns != a.columns() ||
                a.rows() == 0 || a.columns() == 0)
            return *this;
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] += a._matrix[i];
        return *this;
    }

    Matrix<T> &operator += (const T &a) {
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] += a;
        return *this;
    }

    friend Matrix<T> operator + (Matrix<T> a, const Matrix<T> &b) {
        if (a.rows() != b.rows() ||
                a.columns() != b.columns())
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j) {
                auto old = a.get(i, j);
                a.set(i, j, old + b.get(i, j));
            }
        return a;
    }

    friend Matrix<T> operator + (const T &a, Matrix<T> b) {
        if (b.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < b.rows(); ++ i)
            for (size_t j = 0; j < b.columns(); ++ j) {
                auto old = b.get(i, j);
                b.set(i, j, old + a);
            }
        return b;
    }

    friend Matrix<T> operator + (Matrix<T> a, const T &b) {
        if (a.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                a.at(i, j) += b;
        return a;
    }

    Matrix<T> &operator -= (const Matrix<T> &a) {
        if (_rows != a.rows() || _columns != a.columns() ||
                a.rows() == 0 || a.columns() == 0)
            return *this;
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] -= a._matrix[i];
        return *this;
    }

    Matrix<T> &operator -= (const T &a) {
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] -= a;
        return *this;
    }

    friend Matrix<T> operator - (Matrix<T> a, const Matrix<T> &b) {
        if (a.rows() != b.rows() ||
                a.columns() != b.columns())
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                a.at(i, j) -= b.at(i, j);
        return a;
    }

    friend Matrix<T> operator - (Matrix<T> a, const T &b) {
        if (a.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                a.at(i, j) -= b;
        return a;
    }

    Matrix<T> operator - () const {
        if (_rows == 0 || _columns == 0)
            return Matrix<T>(0);
        return (*this) * (-1);
    }

    Matrix<T> &operator *= (const Matrix<T> &a) {
        if (_rows == 0 || a.columns() == 0 ||
                _columns != a.rows())
            return *this;
        _matrix = ((*this) * a)._matrix;
        return *this;
    }

    Matrix<T> &operator *= (const T &a) {
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] *= a;
        return *this;
    }

    friend Matrix<T> operator * (const Matrix<T> &a,
                                 const Matrix<T> &b) {
        if (a.columns() != b.rows())
            return Matrix<T>(0);
        Matrix<T> m(a.rows(), b.columns());
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < b.columns(); ++ j)
                for (size_t k = 0; k < a.columns(); ++ k)
                    m.at(i, j) += a.at(i, k) * b.at(k, j);
        return m;
    }

    friend Matrix<T> operator * (const T &a, Matrix<T> b) {
        if (b.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < b.rows(); ++ i)
            for (size_t j = 0; j < b.columns(); ++ j)
                b.at(i, j) *= a;
        return b;
    }

    friend Matrix<T> operator * (Matrix<T> a, const T &b) {
        if (a.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                a.at(i, j) *= b;
        return a;
    }

    Matrix<T> &operator /= (const T &a) {
        for (size_t i = 0; i < _rows * _columns; ++ i)
            _matrix[i] /= a;
        return *this;
    }

    friend Matrix<T> operator / (Matrix<T> a, const T &b) {
        if (a.minSize() == 0)
            return Matrix<T>(0);
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                a.at(i, j) /= b;
        return a;
    }

    friend bool operator == (const Matrix<T> &a,
                             const Matrix<T> &b) {
        if (a.rows() != b.rows() ||
                a.columns() != b.columns())
            return false;
        for (size_t i = 0; i < a.rows(); ++ i)
            for (size_t j = 0; j < a.columns(); ++ j)
                if (a.get(i, j) != b.get(i, j))
                    return false;
        return true;
    }

//static:
    static Matrix<T> eye(size_t size) {
        Matrix<T> m = Matrix<T>(size);
        for (size_t i = 0; i < size; ++ i)
            m.set(i, i, 1);
        return m;
    }

    static Matrix<T> eye(size_t rows, size_t columns) {
        Matrix<T> m = Matrix<T>(rows, columns);
        size_t minSize = rows <= columns ? rows : columns;
        for (size_t i = 0; i < minSize; ++ i)
            m.set(i, i, 1);
        return m;
    }

    static Array<size_t> genRange(size_t a, size_t b) {
        if (b < a)
            return Array<size_t>(0);
        Array<size_t> range;
        for (size_t i = a; i <= b; ++ i)
            range.push_back(i);
        return range;
    }
};

#endif // MATRICES_H
