#ifndef ___StochasticProcesses___
#define ___StochasticProcesses___
#include <random>
#include "ProbabilityDistributions.h"

class HomogeneousPoissonProcess {
public:
    
	HomogeneousPoissonProcess(const double);
    HomogeneousPoissonProcess(const HomogeneousPoissonProcess&) = default;
    HomogeneousPoissonProcess(HomogeneousPoissonProcess&&) = default;
    HomogeneousPoissonProcess& operator=(const HomogeneousPoissonProcess&) = default;
    HomogeneousPoissonProcess& operator=(HomogeneousPoissonProcess&&) = default;

	double getParam() const;

	double operator()();

private:
	double lambda;
	std::exponential_distribution<double> exp;
	static std::mt19937 gen;
};

class CouponMessageProcess {

    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
    typedef std::vector<Message> MessageChain;
    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
    
public:
    CouponMessageProcess(const double p_low, const double p_high, const size_t N);
    CouponMessageProcess(const CouponMessageProcess&) = default;
    CouponMessageProcess(CouponMessageProcess&&) = default;
    CouponMessageProcess& operator=(const CouponMessageProcess&) = default;
    CouponMessageProcess& operator=(CouponMessageProcess&&) = default;
    
    bool operator()(const MessageChain&) const;

private:
    double p_low;
    double p_high;
	size_t N;
	static newCoupon coupon_dist;
	static std::mt19937 gen;
};

// class ComplexCouponProcess {
    
//     typedef std::vector<std::pair<double,size_t>> Message;
    
// public:
//     ComplexCouponProcess(const double p_low, const double p_high, const size_t N);
//     ComplexCouponProcess(const ComplexCouponProcess&) = default;
//     ComplexCouponProcess(ComplexCouponProcess&&) = default;
//     ComplexCouponProcess& operator=(const ComplexCouponProcess&) = default;
//     ComplexCouponProcess& operator=(ComplexCouponProcess&&) = default;
    
//     bool operator()(const std::vector<Message>&, const size_t _M) const;
    
// private:
//     double p_low;
//     double p_high;
//     size_t N;
//     static complexCoupon coupon_dist;
//     static std::mt19937 gen;
// };

//class BernoulliMessageProcess {
//
//    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
//    typedef std::vector<Message> MessageChain;
//    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
//
//public:
//    BernoulliMessageProcess(const double);
//    BernoulliMessageProcess(const BernoulliMessageProcess&) = default;
//    BernoulliMessageProcess(BernoulliMessageProcess&&) = default;
//    BernoulliMessageProcess& operator=(const BernoulliMessageProcess&) = default;
//    BernoulliMessageProcess& operator=(BernoulliMessageProcess&&) = default;
//
//    bool operator()(const std::vector<MessageChainTopic>&);
//
//private:
//    double p;
//    std::bernoulli_distribution dis;
//    static std::mt19937 gen;
//};
//
//class NewMessageProcess {
//
//    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
//    typedef std::vector<Message> MessageChain;
//    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
//
//public:
//    NewMessageProcess(const double, const double, const double);
//    NewMessageProcess(const NewMessageProcess&) = default;
//    NewMessageProcess(NewMessageProcess&&) = default;
//    NewMessageProcess& operator=(const NewMessageProcess&) = default;
//    NewMessageProcess& operator=(NewMessageProcess&&) = default;
//
//    long operator()(const std::vector<MessageChainTopic>&);
//
//    bool servicedTopic(const std::vector<MessageChainTopic>&) const;
//
//
//private:
//    static size_t topic_counter;
//    double p_spread;
//    double p_resend;
//    double p_new;
//    std::bernoulli_distribution dis_spread;
//    std::bernoulli_distribution dis_resend;
//    std::bernoulli_distribution dis_new;
//    static std::mt19937 gen;
//};


#endif
