#include "fft.h"

#include <cassert>
#include <iostream>

namespace AudioLib
{
    static void applyfft(std::vector<complex>& a, bool inverse)
    {
        int n = a.size();
        if (n == 1)
        {
            return;
        }

        std::vector<complex> even(n/2), odd(n/2);
        for (int i = 0; i<n/2; ++i)
        {
            even[i] = a[2*i];
            odd[i] = a[2*i+1];
        }
        applyfft(even, inverse);
        applyfft(odd, inverse);

        float theta = 2.0f*3.1415926f/n * (inverse ? 1.0f : -1.0f);
        complex w(1), wn(std::cosf(theta), std::sinf(theta));
        for (int i = 0; i < n/2; ++i)
        {
            a[i] = even[i] + w*odd[i];
            a[i+n/2] = even[i] - w*odd[i];
            if (inverse)
            {
                a[i] /= 2;
                a[i + n/2] /= 2;
            }
            w *= wn;
        }
    }

    std::vector<complex> fft(const std::vector<complex>& samples, bool inverse)
    {
        size_t n = samples.size();

        if (n == 0 || (n & (n-1)) != 0)
        {
            std::cout << "Error: number of samples must be a power of 2.";
            assert(false && "Number of samples must be a power of 2.");
        }

        std::vector<complex> result = samples;
        applyfft(result, inverse);
        return result; 
    }

    std::vector<complex> fft(const std::vector<float>& samples, bool inverse)
    {
        size_t n = samples.size();

        if (n == 0 || (n & (n-1)) != 0)
        {
            std::cout << "Error: number of samples must be a power of 2.";
            assert(false && "Number of samples must be a power of 2.");
        }

        std::vector<complex> result(n);
        for (int i = 0; i < n; ++i)
        {
            result[i] = samples[i];
        }
        applyfft(result, inverse);
        return result; 
    }
}