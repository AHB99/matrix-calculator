#include "matrix.h"


using std::cout;
using std::cin;
using std::endl;

//menu system. true indicates continuation. false indicates exit.
bool matrixCalculatorTurn() {
	cout << "Choose option: \n"
		<< "a) Addition\n"
		<< "b) Subtraction\n"
		<< "c) Multiplication\n"
		<< "d) Scalar multiplication\n"
		<< "e) Echelon form\n"
		<< "f) Reduced row echelon form\n"
		<< "g) Transpose\n"
		<< "h) Trace\n"
		<< "i) Determinant\n"
		<< "j) Inverse\n"
		<< "k) EXIT\n" << endl;
	cout << "Select: ";
	char selection;
	cin >> selection;
	if (selection == 'k') {
		cout << "Exiting calculator.\n";
		return false;
	}
	//addition, subtraction, or multiplication (binary operations)
	else if (selection == 'a' || selection == 'b' || selection == 'c') {
		size_t mat1Rows, mat1Columns, mat2Rows, mat2Columns;
		cout << "[Matrix #1] No. of Rows: ";
		cin >> mat1Rows;
		cout << "[Matrix #1] No. of Columns: ";
		cin >> mat1Columns;
		cout << "[Matrix #2] No. of Rows: ";
		cin >> mat2Rows;
		cout << "[Matrix #2] No. of Columns: ";
		cin >> mat2Columns;
		//multiplication validity check
		if (selection == 'c') {
			if (!(mat1Columns == mat2Rows)) {
				cout << "Error, no. of columns of Matrix #1 and no. of rows of Matrix #2 should match." << endl;
				return true;
			}
		}
		//addition/subtraction validity check
		else if (!((mat1Rows == mat2Rows) && (mat1Columns == mat2Columns))) {
			cout << "Error, matrices are of different dimensions." << endl;
			return true;
		}
		Matrix mat1(mat1Rows, mat1Columns), mat2(mat2Rows, mat2Columns);
		cout << "Enter Matrix #1 (" << mat1Rows << "x" << mat1Columns << "):\n";
		mat1.modifyMatrixByInput(cin);
		cout << "Enter Matrix #2 (" << mat2Rows << "x" << mat2Columns << "):\n";
		mat2.modifyMatrixByInput(cin);
		Matrix result;
		if (selection == 'a') {
			result = addMatrices(mat1, mat2);
			cout << "Matrix #1 + Matrix #2 = \n";
		}
		else if ((selection == 'b')) {
			result = subtractMatrices(mat1, mat2);
			cout << "Matrix #1 - Matrix #2 = \n";
		}
		else {
			result = multiplyMatrices(mat1, mat2);
			cout << "Matrix #1 * Matrix #2 = \n";
		}
		result.print(cout);
		cout << endl;
		return true;
	}
	//scalar, echelon form, reduced row echelon form, transpose (unary)
	else if (selection == 'd' || selection == 'e' || selection == 'f' || selection == 'g') {
		size_t matRows, matColumns;
		cout << "[Matrix] No. of Rows: ";
		cin >> matRows;
		cout << "[Matrix] No. of Columns: ";
		cin >> matColumns;
		double scalar;
		if (selection == 'd') {
			cout << "Scalar: ";
			cin >> scalar;
		}
		Matrix mat(matRows, matColumns);
		cout << "Enter Matrix (" << matRows << "x" << matColumns << "):\n";
		mat.modifyMatrixByInput(cin);
		Matrix result;
		if (selection == 'd') {
			result = mat.scalarMultiplication(scalar);
			cout << scalar << " * Matrix = \n";
		}
		else if (selection == 'e') {
			result = mat.findEchelonForm();
			cout << "Echelon form = \n";
		}
		else if (selection == 'f') {
			result = mat.findReducedRowEchelonForm();
			cout << "Reduced row echelon form = \n";
		}
		else {
			result = mat.transpose();
			cout << "Transpose = \n";
		}
		result.print(cout);
		cout << endl;
		return true;
	}
	// trace, determinant, inverse (square matrix operations)
	else if (selection == 'h' || selection == 'i' || selection == 'j') {
		size_t dim;
		cout << "[Matrix] Dimension: ";
		cin >> dim;
		Matrix mat(dim,dim);
		cout << "Enter Matrix (" << dim << "x" << dim << "):\n";
		mat.modifyMatrixByInput(cin);
		if (selection == 'h') {
			double result = mat.trace();
			cout << "Trace = " << result << endl;
			return true;
		}
		else if (selection == 'i') {
			double result = mat.findDeterminant();
			cout << "Determinant = " << result << endl;
			return true;
		}
		else if (selection == 'j') {
			double determinantTest = mat.findDeterminant();
			if (determinantTest != 0) {
				Matrix result = mat.findInverse();
				cout << "Inverse =\n";
				result.print(cout);
				cout << endl;
				return true;
			}
			else {
				cout << "Matrix is non-invertible." << endl;
				return true;
			}
		}
	}
	else {
		cout << "Invalid selection." << endl;
		return true;
	}

}

int main() {
	while (matrixCalculatorTurn()) {
		cout << "Continue? [Y/N]: ";
		char continueChoice;
		cin >> continueChoice;
		if (continueChoice == 'N') {
			break;
		}
	}
}
