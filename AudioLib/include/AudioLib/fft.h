#pragma once 

#include <complex>
#include <vector>

namespace AudioLib
{
    using complex = std::complex<float>;

    std::vector<complex> fft(const std::vector<complex>& samples, bool inverse = false);
    std::vector<complex> fft(const std::vector<float>& samples, bool inverse = false);
};