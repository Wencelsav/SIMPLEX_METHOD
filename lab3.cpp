#include<iostream>
#include<istream>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


// Метод для вывода симплекс-таблицы
void print(double ** simplex, vector<double> matrix_c, vector<double> matrix_b) {
	std::cout << setw(11) << " " << "      ";
	std::cout << setw(10) << "C"  << "      ";
	for(unsigned int i = 2; i <= matrix_c.size() + 1; ++i) {
		std::cout << setw(9) << "x" << simplex[0][i] << "      ";
	}
	std::cout << endl;
	for (unsigned int j = 1; j <= matrix_b.size() + 1; ++j) {
		if (j == matrix_b.size() + 1) std::cout << setw(11) << "F"  << "      ";
		else std::cout << setw(10) << "x" << simplex[j][0] << "      ";
		for (unsigned int i = 1; i <= matrix_c.size() + 1; ++i) {
			std::cout << setw(10) << simplex[j][i] << "      ";
		}
		std::cout << endl;
	}
	std::cout << endl;
}


// Метод, реализующий жордановы исключения
void JordanExeptions(double ** &simplex, unsigned int r, unsigned int k, vector<double> matrix_c, vector<double> matrix_b) {
	//changing all exept r and k
	for (unsigned int i = 1; i <= matrix_b.size() + 1; ++i) {
		if (i != r) {
			for (unsigned int j = 1; j <= matrix_c.size() + 1; ++j) {
				if (j != k) {
					simplex[i][j] = simplex[i][j] - (simplex[i][k] * simplex[r][j]) / simplex[r][k];
				}
			}
		}
	}
	//changing r and k
	for (unsigned int j = 1; j <= matrix_c.size() + 1; ++j) {
		if (j != k) {
			simplex[r][j] = simplex[r][j] / simplex[r][k];
		}
	}
	for (unsigned int i = 1; i <= matrix_b.size() + 1; ++i) {
		if (i != r) {
			simplex[i][k] = -simplex[i][k] / simplex[r][k];
		}
	}
	//changing rk
	simplex[r][k] = 1 / simplex[r][k];
	//change vars
	swap(simplex[0][k], simplex[r][0]);;
}


// Метод создания симплекс-таблицы из исходных данных
void create_simplex(double ** &simplex_matrix, vector<double> matrix_c, vector<double> matrix_b, vector<vector<double>> matrix_a) {
	//filling with 0s
	for (unsigned int i = 0; i <= matrix_c.size() + 1; ++i) {
		for (unsigned int j = 0; j <= matrix_b.size() + 1; ++j) {
			simplex_matrix[j][i] = 0;
		}
	}
	//filing hat
	for (unsigned int i = 1; i <= matrix_a[0].size() + 1; ++i) {
		simplex_matrix[0][i] = i - 1;
	}
	for (unsigned int j = 1; j <= matrix_b.size(); ++j) {
		simplex_matrix[j][0] = matrix_c.size() + j;
	}
	//filling B
	for (unsigned int i = 0; i < matrix_b.size(); ++i) {
		simplex_matrix[i + 1][1] = matrix_b[i];
	}
	//filling C
	for (unsigned int i = 1; i <= matrix_c.size(); ++i) {
		simplex_matrix[matrix_b.size() + 1][i + 1] = matrix_c[i - 1];
	}
	//Filling A
	for (unsigned int i = 0; i < matrix_b.size(); ++i) {
		for (unsigned int j = 1; j <= matrix_c.size(); ++j) {
			simplex_matrix[i + 1][j + 1] = matrix_a[i][j - 1];
		}
	}
}


bool checking_b(double ** simplex_matrix, vector<double> matrix_b) {
	for (unsigned i = 1; i <= matrix_b.size(); ++i) {
		if (simplex_matrix[i][1] < 0) {
			return true;
		}
	}
	return false;
}


bool first_stage(double ** &simplex_matrix, vector<double> matrix_c, vector<double> matrix_b) {
	while (checking_b(simplex_matrix, matrix_b)) {
		std::cout << "start 1 stage" << endl;
		for (unsigned i = 1; i <= matrix_b.size(); ++i) {
			if (simplex_matrix[i][1] < 0) {
				int j_;
				bool res = false;
				//finding разрешающий столбец
				for (unsigned j = 2; j <= matrix_c.size() + 1; ++j) {
					if (simplex_matrix[i][j] < 0) {
						j_ = j;
						res = true;
						break;
					}
				}
				if (!res) {
					std::cout << "No solution" << endl;
					return false;
				}
				//finding разрешающую строку
				double min = simplex_matrix[i][1]/ simplex_matrix[i][j_];
				int min_i = i;
				/*
				for (unsigned i = 1; i <= matrix_b.size(); ++i) {
					if (simplex_matrix[i][j_] != 0 && simplex_matrix[i][1] / simplex_matrix[i][j_] > 0) {
						min = simplex_matrix[i][1] / simplex_matrix[i][j_];
						min_i = i;
						break;
					}
				}*/
				for (unsigned i = 1; i <= matrix_b.size(); ++i) {
					if (simplex_matrix[i][j_] != 0 && simplex_matrix[i][1] / simplex_matrix[i][j_] > 0 && simplex_matrix[i][1] / simplex_matrix[i][j_] < min) {
						min = simplex_matrix[i][1] / simplex_matrix[i][j_];
						min_i = i;
					}
				}
				std::cout << "min = " << min << endl;
				std::cout << "start 1 stage Jordan with " << min_i << "," << j_ << endl;
						JordanExeptions(simplex_matrix, min_i, j_, matrix_c, matrix_b);
						print(simplex_matrix, matrix_c, matrix_b);
			}
		}
	}
	return true;
}


bool checking_F(double ** simplex_matrix, vector<double> matrix_c, vector<double> matrix_b) {
	for (unsigned i = 2; i <= matrix_c.size() + 1; ++i) {
		if (simplex_matrix[matrix_b.size()+1][i] > 0) {
			return true;
			break;
		}
	}
	return false;
}


void second_stage(double ** &simplex_matrix, vector<double> matrix_c, vector<double> matrix_b) {
	while (checking_F(simplex_matrix, matrix_c, matrix_b)) {
		for (unsigned i = 2; i <= matrix_c.size() + 1; ++i) {
			if (simplex_matrix[matrix_b.size() + 1][i] > 0) {
				double min = 0;
				int min_j = 0;
				for (unsigned j = 1; j <= matrix_b.size(); ++j) {
					if (simplex_matrix[j][i] > 0 && simplex_matrix[j][1] / simplex_matrix[j][i] >= 0) {
						min = simplex_matrix[j][1] / simplex_matrix[j][i];
						min_j = j;
						break;
					}
				}
				for (unsigned j = 1; j <= matrix_b.size(); ++j) {
					if (simplex_matrix[j][i] > 0 && simplex_matrix[j][1] / simplex_matrix[j][i] >= 0 && simplex_matrix[j][1] / simplex_matrix[j][i] < min) {
						min = simplex_matrix[j][1] / simplex_matrix[j][i];
						min_j = j;
					}
				}
				std::cout << "start 2 stage Jordan with " << min_j << "," << i << endl;
				JordanExeptions(simplex_matrix, min_j, i, matrix_c, matrix_b);
				print(simplex_matrix, matrix_c, matrix_b);
			}
		}
	}
}


bool simplex_method_PZ(double ** &simplex_matrix, vector<double> matrix_c, vector<double> matrix_b) {
	while (checking_b(simplex_matrix, matrix_b) || checking_F(simplex_matrix, matrix_c, matrix_b)) {
		if (first_stage(simplex_matrix, matrix_c, matrix_b)) {
			second_stage(simplex_matrix, matrix_c, matrix_b);
		}
		else return false;
	}
	return true;
}


bool checking_integer_b(double ** simplex_matrix, vector<double> matrix_b) {
	for (unsigned i = 1; i <= matrix_b.size(); ++i) {
		if (simplex_matrix[i][1] != (long long)simplex_matrix[i][1]) {
			return true;
		}
	}
	return false;
}

double new_fic_var(double ** simplex_matrix, vector<double> matrix_c, vector<double> matrix_b) {
	vector<double>  vars;
	for (unsigned int j = 2; j <= matrix_c.size() + 1; ++j) {
		vars.push_back(simplex_matrix[0][j]);
	}
	for (unsigned int i = 1; i <= matrix_b.size(); ++i) {
		vars.push_back(simplex_matrix[i][0]);
	}
	double max = vars[0];
	for (unsigned int i = 1; i < vars.size(); ++i) {
		if (vars[i] > max) max = vars[i];
	}
	return max;
}

void integer_solution(double ** &simplex_matrix, vector<double> &matrix_c, vector<double> &matrix_b, vector<double> &solutions) {
	if (checking_integer_b(simplex_matrix, matrix_b)) {
		for (unsigned i = 1; i <= matrix_b.size(); ++i) {
			if (simplex_matrix[i][1] != (long long)simplex_matrix[i][1]) {
				std::cout << "Our branch variable: " << simplex_matrix[i][1] << endl;
				double branch = simplex_matrix[i][1];
				vector<double> matrix_b1 = matrix_b, matrix_b2 = matrix_b;
				matrix_b1.push_back(1);
				matrix_b2.push_back(1);
				//Создание новой матрицы с переменной ветвления
				double ** simplex_matrix1 = new double *[matrix_b.size() + 2];
				for (unsigned int i = 0; i <= matrix_b.size() + 2; ++i) {
					simplex_matrix1[i] = new double[matrix_c.size() + 1];
				}
				//filling until b
				for (unsigned int i = 0; i <= matrix_b.size(); ++i) {
					for (unsigned int j = 0; j <= matrix_c.size() + 1; ++j) {
						simplex_matrix1[i][j] = simplex_matrix[i][j];
					}
				}
				// filling new fic var
				simplex_matrix1[matrix_b.size() + 1][0] = new_fic_var(simplex_matrix, matrix_c, matrix_b) + 1;
				// filling last string
				for (unsigned int j = 0; j <= matrix_c.size() + 1; ++j) {
					simplex_matrix1[matrix_b.size() + 2][j] = simplex_matrix[matrix_b.size() + 1][j];
				}
				//for second branch
				double ** simplex_matrix2 = new double *[matrix_b.size() + 2];
				for (unsigned int i = 0; i <= matrix_b.size() + 2; ++i) {
					simplex_matrix2[i] = new double[matrix_c.size() + 1];
				}
				for (unsigned int j = 0; j <= matrix_b.size() + 2; ++j) {
					for (unsigned int i = 0; i <= matrix_c.size() + 1; ++i) {
						simplex_matrix2[j][i] = simplex_matrix1[j][i];
					}
				}
				// ветвим по branch <= int(branch)
				double branch1 = double(int(branch) - branch);
				std::cout << "Branch with " << branch << " <= " << int(branch) << endl;
				// filling string for new fic var
				simplex_matrix1[matrix_b.size() + 1][1] = branch1;
				for (unsigned int j = 2; j <= matrix_c.size() + 1; ++j) {
					simplex_matrix1[matrix_b.size() + 1][j] = -simplex_matrix[i][j];
				}
				print(simplex_matrix1, matrix_c, matrix_b1);
				if (simplex_method_PZ(simplex_matrix1, matrix_c, matrix_b1)) {
					if (checking_integer_b(simplex_matrix1, matrix_b1)) {
						integer_solution(simplex_matrix1, matrix_c, matrix_b1, solutions);
					}
				}
				// ветвим по branch >= int(branch)
				double branch2 = double(branch - (int(branch) + 1));
				std::cout << "Branch with " << branch << " >= " << int(branch) + 1 << endl;
				simplex_matrix2[matrix_b.size() + 1][1] = branch2;
				for (unsigned int j = 2; j <= matrix_c.size() + 1; ++j) {
					simplex_matrix2[matrix_b.size() + 1][j] = simplex_matrix[i][j];
				}
				print(simplex_matrix2, matrix_c, matrix_b2);
				//JordanExeptions(simplex_matrix2, 4, 2, matrix_c, matrix_b2);
				//print(simplex_matrix2, matrix_c, matrix_b2);
				if (simplex_method_PZ(simplex_matrix2, matrix_c, matrix_b2)) {
					if (checking_integer_b(simplex_matrix2, matrix_b2)) {
						integer_solution(simplex_matrix2, matrix_c, matrix_b2, solutions);
					}
				}
			}
		}
	}
}


int main()
{
	//создаем матрицу
	vector<vector<double>> matrix_a = { { 2,1,2 },{ 1,2,0 },{ 0,0.5,1 } };
	vector<double> matrix_b = { 6,6,2 };
	vector<double> matrix_c = { 2,5,3 };
	vector<double> solutions;
	double ** simplex_matrix;
	simplex_matrix = new double *[matrix_b.size() + 1];
	for (unsigned int i = 0; i <= matrix_b.size() + 1; ++i) {
		simplex_matrix[i] = new double[matrix_c.size() + 1];
	}
	create_simplex(simplex_matrix, matrix_c, matrix_b, matrix_a);

	print(simplex_matrix, matrix_c, matrix_b);
	if (simplex_method_PZ(simplex_matrix, matrix_c, matrix_b)) {
		integer_solution(simplex_matrix, matrix_c, matrix_b, solutions);

	}
	//удаление матрицы, так как мы выделяли память под нее
	for (unsigned int i = 0; i < matrix_b.size() + 1; ++i) {
		delete[] simplex_matrix[i];
	}
	delete[]simplex_matrix;
	std::cin.get();
}
