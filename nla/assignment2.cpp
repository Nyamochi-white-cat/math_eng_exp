#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <string>

// LU分解
Eigen::VectorXd LU(const Eigen::MatrixXd& A, const Eigen::VectorXd& b) {
    const int n = A.rows();
    Eigen::MatrixXd U = A;
    Eigen::MatrixXd L = Eigen::MatrixXd::Identity(n, n);
    Eigen::PermutationMatrix<Eigen::Dynamic> pi(n);
    pi.setIdentity();

    for (int i = 0; i < n - 1; ++i) {
        Eigen::Index r, c;
        U.block(i, i, n - i, 1).cwiseAbs().maxCoeff(&r, &c);
        r += i;  // r is the index of the block.
        double pivot = U(r, i);
        if (pivot == 0) {
            throw std::runtime_error("Matrix is not regular.");
        }
        if (r != i) {
            U.row(i).swap(U.row(r));
            L.block(r, 0, 1, i).swap(L.block(i, 0, 1, i));
            std::swap(pi.indices()(i), pi.indices()(r));
        }
        L.block(i + 1, i, n - i - 1, 1) = U.block(i + 1, i, n - i - 1, 1) / pivot;
        for (int j = i + 1; j < n; ++j) {
            U.row(j) -= L(j, i) * U.row(i);
        }
    }
    Eigen::VectorXd y = pi.transpose() * b;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            y(i) -= L(i, j) * y(j);
        }
    }
    Eigen::VectorXd x(n);
    for (int i = n - 1; i >= 0; --i) {
        x(i) = y(i);
        for (int j = i + 1; j < n; ++j) {
            x(i) -= U(i, j) * x(j);
        }
        x(i) /= U(i, i);
    }
    return x;
}


int main() {
    std::vector<int> mat_sizes = {100, 200, 400, 800};
    for (int n : mat_sizes) {
        std::ofstream outfile("data/assignment2_" + std::to_string(n) + ".dat");
        outfile << "trial" << "\t" << "residual" << "\t" << "diff" << "\t" << "duration" << std::endl;

        for (int i = 0; i < 100; i++) {
            Eigen::MatrixXd A = Eigen::MatrixXd::Random(n, n);
            Eigen::VectorXd b = Eigen::VectorXd::Random(n);
            Eigen::VectorXd x_true = A.colPivHouseholderQr().solve(b);

            auto start = std::chrono::high_resolution_clock::now();
            Eigen::VectorXd x_eliminated = LU(A, b);
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
