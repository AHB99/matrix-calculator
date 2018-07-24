#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <iostream>
#include <iomanip>

class Matrix {
public:
	friend Matrix addMatrices(const Matrix&, const Matrix&);
	friend Matrix subtractMatrices(const Matrix&, const Matrix&);
	friend Matrix multiplyMatrices(const Matrix&, const Matrix&);

	Matrix() = default;
	//constructs zero matrix of size rows x columns
	Matrix(size_t rows, size_t columns) : matrix(std::vector<std::vector<double>>(rows, std::vector<double>(columns))) { }

	std::ostream& print(std::ostream&) const;
	std::istream& modifyMatrixByInput(std::istream&);
	size_t getRows() const;
	size_t getColumns() const;
	void setMatrixValue(size_t, size_t, double);
	double getMatrixValue(size_t, size_t) const;
	Matrix scalarMultiplication(double) const;
	Matrix transpose() const;
	Matrix findEchelonForm() const;
	Matrix findReducedRowEchelonForm() const;
	//methods for square matrices
	double trace() const;
	double findDeterminant() const;
	Matrix findInverse() const;

private:
	//Echelon Form helper functions
	void swapRows(size_t, size_t);
	void multiplyRowByCoefficient(size_t, double);
	size_t findLeftmostNonzeroColumnInSubmatrix(size_t) const;
	size_t findFirstNonzeroRowInColumnInSubmatrixFromTop(size_t, size_t) const;
	void addRowTimesCoefficientToRow(size_t, size_t, double);
	//Reduced Row Echelon Form helper functions
	size_t findColumnOfLeadingOneInRow(size_t) const;
	bool isNonzeroRow(size_t) const;
	size_t findFirstNonzeroRowFromBottom() const;

	//Inverse helper function
	Matrix addIdentityToRightSide() const;
	//Determinant helper function
	Matrix squareMatrixCutter(size_t, size_t) const;

	std::vector<std::vector<double>> matrix;
};

#endif