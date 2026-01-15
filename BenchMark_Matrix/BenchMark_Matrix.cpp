
#include <chrono>
#include <DirectXMath.h>
#include <DirectXMathConvert.inl>
#include <DirectXMathMatrix.inl>
#include <iostream>
#include <ratio>
#include <vector>
using namespace DirectX;

struct ScalarMat4 {
	float m[4][4];
};

// Naïve scalar 4x4 multiply
ScalarMat4 mulScalar(const ScalarMat4& A, const ScalarMat4& B) {
	ScalarMat4 R = {};
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				R.m[i][j] += A.m[i][k] * B.m[k][j];
	return R;
}

int main() {
	const int N = 1'000'000; // big batch
	std::vector<ScalarMat4> As(N), Bs(N);
	// init with something deterministic
	for (int i = 0; i < N; ++i) {
		for (int r = 0; r < 4; ++r)
			for (int c = 0; c < 4; ++c) {
				As[i].m[r][c] = (r == c) ? 1.0f : 0.1f * (i % 7);
				Bs[i].m[r][c] = (r == c) ? 1.0f : 0.2f * (i % 5);
			}
	}
	std::vector< XMMATRIX> Cs(N), Ds(N);
	for (int i = 0; i < N; ++i) {
		Cs[i] = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&As[i]));
		Ds[i] = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&Bs[i]));
	}

	// Benchmark DirectXMath
	auto t0 = std::chrono::high_resolution_clock::now();
	XMMATRIX accDX = XMMatrixIdentity();
	for (int i = 0; i < N; ++i) {
		accDX = XMMatrixMultiply(accDX, XMMatrixMultiply(Cs[i], Ds[i]));
	}
	auto t1 = std::chrono::high_resolution_clock::now();

	// Benchmark scalar
	auto t2 = std::chrono::high_resolution_clock::now();
	ScalarMat4 accS = { { {1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1} } };
	for (int i = 0; i < N; ++i) {
		accS = mulScalar(As[i], Bs[i]);
	}
	auto t3 = std::chrono::high_resolution_clock::now();

	double dx_ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
	double sc_ms = std::chrono::duration<double, std::milli>(t3 - t2).count();
	double percent_slower = (sc_ms / dx_ms - 1.0) * 100.0;

	std::cout << "DirectXMath: " << dx_ms << " ms\n";
	std::cout << "Scalar:      " << sc_ms << " ms\n";
	std::cout << "Scalar is " << percent_slower << "% slower\n";
}
