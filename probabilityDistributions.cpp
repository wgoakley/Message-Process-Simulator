//
//  ProbabilityDistributions.cpp
//  NetworkSim
//
//  Created by William Oakley on 5/29/18.
//  Copyright © 2018 William Oakley. All rights reserved.
//

#include "ProbabilityDistributions.h"
#include <stddef.h>
#include <iostream>
#include <fstream>

double newCoupon::operator()(const long N, const long n) const {
    assert(n<n_MAX);
    assert(N >= N_MIN && N <= N_MAX);
    if (n == 1) {
        return 1;
    }
    double sum = 0;
    for (long k = 1 ; k < (n<N?n:N) ; ++k) {
        sum += q[N-(N_MIN-1)-1][n-1-1][k-1]*(N-k)/static_cast<const double>(N);
    }
    return sum;
}

double newCoupon::operator()(const long n) const {
    assert(n<n_MAX);
    const long N = N_MIN;
    if (n == 1) {
        return 1;
    }
    double sum = 0;
    for (long k = 1 ; k < (n<N?n:N) ; ++k) {
        sum += q[N-(N_MIN-1)-1][n-1-1][k-1]*(N-k)/static_cast<const double>(N);
    }
    return sum;
}

void newCoupon::initialize() {

    for(size_t i = 0 ; i < (N_MAX-N_MIN+1) ; ++i) {
        for (size_t j = 0 ; j < n_MAX ; ++j) {
            for (size_t k = 0 ; k < k_MAX ; ++k) {
                q[i][j][k] = 0;
            }
        }
    }
    
    // boundary data
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        q[N-1][1-1][1-1] = 1;
    }
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            q[N-1][n-1][1-1] = 1/static_cast<double>(N+(N_MIN-1))*q[N-1][n-1-1][1-1];
        }
    }
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            for (long k = ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) + 1 ; k <= k_MAX ; ++k) {
                q[N-1][n-1][k-1] = 0;
            }
        }
    }
    
    // recurrence
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            for (long k = 2 ; k <= ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) ; ++k) {
                q[N-1][n-1][k-1] = q[N-1][n-1-1][k-1]*k/static_cast<double>(N+(N_MIN-1)) + q[N-1][n-1-1][k-1-1]*(N+(N_MIN-1)-k+1)/static_cast<double>(N+(N_MIN-1));
            }
        }
    }
    
    std::ofstream fout("newCoupon.txt");
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            fout << '{';
            for (long k = 2 ; k <= ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) ; ++k) {
                fout << q[N-1][n-1][k-1] << ", ";
            }
            fout << '}';
        }
    }
}

double newCoupon::q[N_MAX-N_MIN+1][n_MAX][k_MAX] = {};

// complexCoupon

double complexCoupon::operator()(const long N, const long M, const long n, const long k) const {
    assert(n<n_MAX);
    assert(N >= N_MIN && N <= N_MAX);
    if (n == 1) {
        return 1;
    }
    double sum = 0;
    for (long k = 1 ; k < (n<N?n:N) ; ++k) {
        sum += q[N-(N_MIN-1)-1][n-1-1][k-1]*(N-k)/static_cast<const double>(N);
    }
    return static_cast<const double>(k)/M*sum;
}

double complexCoupon::operator()(const long M, const long n, const long k) const {
    assert(n<n_MAX);
    const long N = N_MIN;
    if (n == 1) {
        return 1;
    }
    double sum = 0;
    for (long k = 1 ; k < (n<N?n:N) ; ++k) {
        sum += q[N-(N_MIN-1)-1][n-1-1][k-1]*(N-k)/static_cast<const double>(N);
    }
    return static_cast<const double>(k)/M*sum;
}

void complexCoupon::initialize() {
    
    for(size_t i = 0 ; i < (N_MAX-N_MIN+1) ; ++i) {
        for (size_t j = 0 ; j < n_MAX ; ++j) {
            for (size_t k = 0 ; k < k_MAX ; ++k) {
                q[i][j][k] = 0;
            }
        }
    }
    
    // boundary data
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        q[N-1][1-1][1-1] = 1;
    }
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            q[N-1][n-1][1-1] = 1/static_cast<double>(N+(N_MIN-1))*q[N-1][n-1-1][1-1];
        }
    }
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            for (long k = ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) + 1 ; k <= k_MAX ; ++k) {
                q[N-1][n-1][k-1] = 0;
            }
        }
    }
    
    // recurrence
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            for (long k = 2 ; k <= ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) ; ++k) {
                q[N-1][n-1][k-1] = q[N-1][n-1-1][k-1]*k/static_cast<double>(N+(N_MIN-1)) + q[N-1][n-1-1][k-1-1]*(N+(N_MIN-1)-k+1)/static_cast<double>(N+(N_MIN-1));
            }
        }
    }
    
    std::ofstream fout("complexCoupon.txt");
    for (long N = N_MIN-(N_MIN-1) ; N <= N_MAX-(N_MIN-1) ; ++N) {
        for (long n = 2 ; n <= n_MAX ; ++n) {
            fout << '{';
            for (long k = 2 ; k <= ((n<(N+(N_MIN-1)))?n:(N+(N_MIN-1))) ; ++k) {
                fout << q[N-1][n-1][k-1] << ", ";
            }
            fout << '}';
        }
    }
}

double complexCoupon::q[N_MAX-N_MIN+1][n_MAX][k_MAX] = {};

