#ifndef ___ProbabilityDistributions___
#define ___ProbabilityDistributions___

constexpr long N_MIN = 10;
constexpr long N_MAX = 10;
constexpr long n_MAX = 50000;
constexpr long k_MAX = (N_MAX < n_MAX)?N_MAX:n_MAX;

class newCoupon {
public:

    double operator()(const long N, const long n) const;

    double operator()(const long n) const;

    static void initialize();
private:

	static double q[N_MAX-N_MIN+1][n_MAX][k_MAX];

};

class complexCoupon {
public:
    
    double operator()(const long N, const long M, const long n, const long k) const;
    
    double operator()(const long M, const long n, const long k) const;
    
    static void initialize();
private:
    
    static double q[N_MAX-N_MIN+1][n_MAX][k_MAX];
};
#endif
