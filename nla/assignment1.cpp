#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <string>

// 消去法
Eigen::VectorXd elimination(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) {
    const int n = A.rows();
    Eigen::MatrixXd Ab(n, n + 1);
    Ab << A, b;

    // 前進消去
    for (int i = 0; i < n - 1; ++i) {
        Eigen::Index r, c;
        Ab.block(i, i, n - i, 1).cwiseAbs().maxCoeff(&r, &c);
        r += i;  // r is the index of the block.
        double pivot = Ab(r, i);
        if (pivot == 0) {
            throw std::runtime_error("Matrix is not regular.");
        }
        if (r != i) {
            Ab.row(i).swap(Ab.row(r));
        }
        for (int j = i + 1; j < n; ++j) {
            double factor = Ab(j, i) / pivot;
            Ab.row(j) -= factor * Ab.row(i);
        }
    }

    // 後進代入
    Eigen::VectorXd x(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = Ab(i, n);
        for (int j = i + 1; j < n; ++j) {
            sum -= Ab(i, j) * x(j);
        }
        x(i) = sum / Ab(i, i);
    }
    return x;
}


int main() {
    std::vector<int> mat_sizes = {100, 200, 400, 800};
    for (int n : mat_sizes) {
        std::ofstream outfile("data/assignment1_" + std::to_string(n) + ".dat");
        outfile << "trial" << "\t" << "residual" << "\t" << "diff" << "\t" << "duration" << std::endl;

        for (int i = 0; i < 100; i++) {
            Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, n);
            Eigen::VectorXd b = Eigen::VectorXd::Random(n);
            Eigen::VectorXd x_true = A.colPivHouseholderQr().solve(b);

            auto start = std::chrono::high_resolution_clock::now();
            Eigen::VectorXd x_eliminated = elimination(A, b);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            double residual_norm = (A * x_eliminated - b).norm();
            double diff = (x_true - x_eliminated).norm();
            outfile << i << "\t" << residual_norm << "\t" << diff << "\t" << duration.count() << std::endl;
        }
        outfile.close();
    }
    return 0;
}
