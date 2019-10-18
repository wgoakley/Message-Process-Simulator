#ifndef ___Distributions___
#define ___Distributions___

#include "ProbabilityDistributions.h"
#include <cmath>

template <typename T>
class ConstantDistribution {
public:
	ConstantDistribution(T const& _t) : t(_t) {}

	template<typename Gen>
	const T operator()(Gen&) {
		return t;
	}
private:
	T t;
};

class PowerLawDistribution {
public:
    PowerLawDistribution(const double _r, const size_t _N) : r(_r), N(_N), dis(N,0,N,[_r](long i)->double{return 1/std::pow(static_cast<double>(i + 1), _r);}) {}
    
    template<typename Gen>
    double operator()(Gen& gen) {
        return dis(gen);
    }
private:
    const double r;
    const size_t N;
    std::discrete_distribution<> dis;
};

#endif
