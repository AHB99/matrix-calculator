#include "matrix.h"


using std::cout;
using std::cin;
using std::endl;

std::ostream& Matrix::print(std::ostream& os) const {
	for (auto & rows : matrix) {
		for (auto & el : rows) {
			os << std::fixed << std::setprecision(3) << el << "\t\t";
		}
		os << endl;
	}
	return os;
}
//for user input of matrix, as constructor makes zero matrices only
std::istream& Matrix::modifyMatrixByInput(std::istream& is) {
	for (auto & rows : matrix) {
		for (auto & el : rows) {
			is >> el;
		}
	}
	return is;
}

size_t Matrix::getRows() const {
	return matrix.size();
}

size_t Matrix::getColumns() const {
	if (matrix.size() == 0) {
		return 0;
	}
	else {
		return (matrix[0]).size();
	}
}

void Matrix::setMatrixValue(size_t row, size_t column, double value) {
	matrix[row][column] = value;
}

double Matrix::getMatrixValue(size_t row, size_t column) const {
	return matrix[row][column];
}



Matrix Matrix::scalarMultiplication(double scalar) const {
	Matrix result(*this);
	for (auto & row : result.matrix) {
		for (auto & el : row) {
			el *= scalar;
		}
	}
	return result;
}


Matrix addMatrices(const Matrix& mat1, const Matrix& mat2) {
	Matrix result(mat1.getRows(), mat1.getColumns());
	for (size_t i = 0; i < mat1.getRows(); ++i) {
		for (size_t j = 0; j < mat1.getColumns(); ++j) {
			result.setMatrixValue(i, j, (mat1.getMatrixValue(i, j) + mat2.getMatrixValue(i, j)));
		}
	}
	return result;
}

Matrix subtractMatrices(const Matrix& mat1, const Matrix& mat2) {
	//uses addMatrices and scalarMultiplication of RHS with -1 for same effect
	Matrix rhs = mat2.scalarMultiplication(-1);
	Matrix result = addMatrices(mat1, rhs);
	return result;
}

Matrix multiplyMatrices(const Matrix& mat1, const Matrix& mat2) {
	//because result has same number of rows as first, and columns as second
	Matrix result(mat1.getRows(), mat2.getColumns());
	for (size_t i = 0; i < mat1.getRows(); ++i) {
		for (size_t j = 0; j < mat2.getColumns(); ++j) {
			double tempSum = 0;
			for (size_t k = 0; k < mat1.getColumns(); ++k) {
				tempSum += (mat1.getMatrixValue(i, k) * mat2.getMatrixValue(k, j));
			}
			result.setMatrixValue(i, j, tempSum);
		}
	}
	return result;
}

//sums diagonal
double Matrix::trace() const {
	double result = 0;
	for (size_t i = 0; i < this->getRows(); ++i) {
		result += this->getMatrixValue(i, i);
	}
	return result;
}

//extracts the submatrix by excluding the given row and column
Matrix Matrix::squareMatrixCutter(size_t rowpos, size_t colpos) const {
	//size will be current-1 due to exclusion
	Matrix result((this->getRows()) - 1, (this->getRows()) - 1);
	//Indices of result are kept seperate from indices of source matrix (i and j) to search and fill correctly
	size_t resultRowIndex = 0;

	for (size_t i = 0; i < (this->getRows()); ++i) {
		if (i != rowpos) {
			size_t resultColumnIndex = 0;
			for (size_t j = 0; j < (this->getRows()); ++j) {
				if (j != colpos) {
					result.setMatrixValue(resultRowIndex, resultColumnIndex, this->getMatrixValue(i, j));
					++resultColumnIndex;
				}
			}
			++resultRowIndex;
		}
	}
	return result;
}

double Matrix::findDeterminant() const {
	if ((this->getRows()) == 2) {
		//ad-bc for trivial 2x2 matrix
		double result = (((this->getMatrixValue(0, 0)) * (this->getMatrixValue(1, 1))) - ((this->getMatrixValue(0, 1)) * (this->getMatrixValue(1, 0))));
		return result;
	}
	else {
		double finalResult = 0;
		//arbitrarily chose first row, and finding sum of cofactors using it.
		for (size_t j = 0; j < (this->getRows()); ++j) {
			int cofactorCoefficient = 1;
			//since first row is set arbitrarily, can deduce that odd index columns have negative coefficient
			if (j % 2 != 0) {
				cofactorCoefficient = -1;
			}
			Matrix cutMatrix = this->squareMatrixCutter(0, j);
			finalResult += (cutMatrix.findDeterminant())*(this->getMatrixValue(0, j))*cofactorCoefficient;
		}
		return finalResult;
	}
}

Matrix Matrix::transpose() const {
	Matrix result((this->getColumns()), (this->getRows()));
	for (size_t i = 0; i < (this->getRows()); ++i) {
		for (size_t j = 0; j < (this->getColumns()); ++j) {
			result.setMatrixValue(j, i, (this->getMatrixValue(i, j)));
		}
	}
	return result;
}

void Matrix::swapRows(size_t row1, size_t row2) {
	std::vector<double> temp = matrix[row1];
	matrix[row1] = matrix[row2];
	matrix[row2] = temp;
}
//for EF and RREF usage
void Matrix::multiplyRowByCoefficient(size_t row, double coefficient) {
	for (auto & el : matrix[row]) {
		el *= coefficient;
	}
}
//to avoid floating point arithmetic errors, -10 chosen arbitrarily
bool isZero(double num) {
	if (abs(num) < exp(-10)) {
		return true;
	}
	else {
		return false;
	}
}
//for EF and RREF usage
size_t Matrix::findLeftmostNonzeroColumnInSubmatrix(size_t upperLim) const {
	for (size_t j = 0; j < (this->getColumns()); ++j) {
		for (size_t i = upperLim; i < (this->getRows()); ++i) {
			if (!isZero(this->getMatrixValue(i, j))) {
				return j;
			}
		}
	}
	//implies a full zero submatrix;
	return (this->getColumns());
}
//for EF and RREF usage
size_t Matrix::findFirstNonzeroRowInColumnInSubmatrixFromTop(size_t upperLim, size_t colNum) const {
	for (size_t i = upperLim; i < (this->getRows()); ++i) {
		if (!isZero(this->getMatrixValue(i, colNum))) {
			return i;
		}
	}
	//implies a zero column.
	return (this->getRows());
}
//for EF and RREF usage
void Matrix::addRowTimesCoefficientToRow(size_t sourceRow, size_t destinationRow, double coefficient) {
	for (size_t j = 0; j < (this->getColumns()); ++j) {
		this->setMatrixValue(destinationRow, j, ((this->getMatrixValue(destinationRow, j)) + ((this->getMatrixValue(sourceRow, j))*coefficient)));
	}
}

Matrix Matrix::findEchelonForm() const {
	Matrix result(*this);
	//moving from top to bottom
	size_t upperLim = 0;
	while (upperLim != (this->getRows())) {
		size_t currColumn = result.findLeftmostNonzeroColumnInSubmatrix(upperLim);
		//if currColumn was matrix's columns, it implies a full zero submatrix (from that method's definition), so do not calculate
		if (currColumn != result.getColumns()) {
			//swaps to ensure pivot is non-zero. may swap with self.
			size_t firstNonZeroRow = result.findFirstNonzeroRowInColumnInSubmatrixFromTop(upperLim, currColumn);
			result.swapRows(upperLim, firstNonZeroRow);
			//sets pivot to 1
			result.multiplyRowByCoefficient(upperLim, (1.0 / (result.getMatrixValue(upperLim, currColumn))));
			//sets cells below pivot to 0
			for (size_t i = upperLim + 1; i < (this->getRows()); ++i) {
				result.addRowTimesCoefficientToRow(upperLim, i, (-1.0)*(result.getMatrixValue(i, currColumn)));
			}
		}
		++upperLim;
	}
	return result;
}

//for RREF usage
size_t Matrix::findColumnOfLeadingOneInRow(size_t row) const {
	for (size_t j = 0; j < (this->getColumns()); ++j) {
		//checks if value == 1. floating point arithmetic errors workaround.
		if (isZero((this->getMatrixValue(row, j)) - 1.0)) {
			return j;
		}
	}
	//implies could not find a leading one.
	return (this->getColumns());
}

//for RREF usage
bool Matrix::isNonzeroRow(size_t row) const {
	for (size_t j = 0; j < (this->getColumns()); ++j) {
		if (!isZero(this->getMatrixValue(row, j))) {
			return true;
		}
	}
	return false;
}

//assuming non-zero matrix
size_t Matrix::findFirstNonzeroRowFromBottom() const {
	for (size_t i = (this->getRows()) - 1; i >= 0; --i) {
		//need not check if top row reached since assuming non-zero matrix
		if ((i == 0) || (this->isNonzeroRow(i))) {
			return i;
		}
	}
	return 0;
}


Matrix Matrix::findReducedRowEchelonForm() const {
	//first send to Echelon form
	Matrix result = this->findEchelonForm();
	//then work from bottom to top, starting at first pivot
	for (size_t lowerLim = result.findFirstNonzeroRowFromBottom(); lowerLim > 0; --lowerLim) {

		size_t currColumn = result.findColumnOfLeadingOneInRow(lowerLim);
		for (size_t i = lowerLim - 1; i >= 0; --i) {
			//set cells above pivot to 0
			result.addRowTimesCoefficientToRow(lowerLim, i, (-1.0)*(result.getMatrixValue(i, currColumn)));
			//to avoid decrementing a size_t below 0
			if (i == 0) {
				break;
			}
		}

	}
	return result;
}

//for inverse usage
Matrix buildIdentity(size_t n) {
	Matrix result(n,n);
	for (size_t i = 0; i < n; ++i) {
		result.setMatrixValue(i, i, 1);
	}
	return result;
}
//for inverse usage
Matrix Matrix::addIdentityToRightSide() const {
	Matrix result((this->getRows()), (this->getRows()) * 2);
	Matrix tempIdentity(buildIdentity((this->getRows())));
	for (size_t i = 0; i < (this->getRows()); ++i) {
		for (size_t j = 0; j < (this->getRows()); ++j) {
			result.setMatrixValue(i, j, this->getMatrixValue(i, j));
		}
		for (size_t j = (this->getRows()); j < (this->getRows()) * 2; ++j) {
			result.setMatrixValue(i, j, tempIdentity.getMatrixValue(i, j - (this->getRows())));
		}
	}
	return result;
}
//for inverse usage
Matrix extractRightSideSquareMatrix(const Matrix& mat1) {
	Matrix result(mat1.getRows(), mat1.getRows());
	for (size_t i = 0; i < mat1.getRows(); ++i) {
		for (size_t j = 0; j < mat1.getRows(); ++j) {
			result.setMatrixValue(i, j, mat1.getMatrixValue(i, j + mat1.getRows()));
		}
	}
	return result;
}

Matrix Matrix::findInverse() const {
	//adds identity to right side of matrix. performs RREF. extracts inverse from right side.
	Matrix matrixWithIdentity = this->addIdentityToRightSide();
	Matrix solvedReducedRowEchelonForm = matrixWithIdentity.findReducedRowEchelonForm();
	Matrix result = extractRightSideSquareMatrix(solvedReducedRowEchelonForm);
	return result;
}
