#include "StochasticProcesses.h"
#include <iostream>
#include <set>

/*
 * Homogeneous Poisson Process definitions
 **/

HomogeneousPoissonProcess::HomogeneousPoissonProcess(const double lambda) : lambda(lambda), exp(lambda)  {}

double HomogeneousPoissonProcess::getParam() const {
	return lambda;
}

double HomogeneousPoissonProcess::operator()() {
	return exp(gen);
}

std::mt19937 HomogeneousPoissonProcess::gen((std::random_device())());

/*
 *  Coupon Message Process definitions
 **/

CouponMessageProcess::CouponMessageProcess(const double _p_low, const double _p_high, const size_t _N) : p_low(_p_low), p_high(_p_high), N(_N) {}

bool CouponMessageProcess::operator()(const std::vector<Message>& _service_history) const {
	size_t sz = _service_history.size();
    std::bernoulli_distribution send(p_low*(1-coupon_dist(N,sz)) + p_high*(coupon_dist(N,sz)));    
    return send(gen);
}

newCoupon CouponMessageProcess::coupon_dist;

std::mt19937 CouponMessageProcess::gen((std::random_device())());

/*
 *  Complex Coupon Message Process definitions
 **/

// ComplexCouponProcess::ComplexCouponProcess(const double _p_low, const double _p_high, const size_t _N) : p_low(_p_low), p_high(_p_high), N(_N) {}

// bool ComplexCouponProcess::operator()(const std::vector<Message>& _service_history, const size_t _M) const {
//     size_t sz = _service_history.size();
//     // look through the second coordinate of each pair in service_history to count the number
//     // of neighbors who I've received a message from.
//     std::set<size_t> temp;
//     for (size_t i = 0 ; i < sz ; ++i) {
//         size_t sz = _service_history[i].size();
//         temp.insert(_service_history[i][sz-2].second);
//     }
//     size_t k = temp.size();
//     std::bernoulli_distribution send(p_low*(1-coupon_dist(N,_M,sz,k)) + p_high*(coupon_dist(N,_M,sz,k)));
//     return send(gen);
// }

// complexCoupon ComplexCouponProcess::coupon_dist;

// std::mt19937 ComplexCouponProcess::gen((std::random_device())());


/*
 *  BernoulliMessageProcess definitions
 **/

//BernoulliMessageProcess::BernoulliMessageProcess(const double _p) : p(_p), dis(_p) {}
//
//bool BernoulliMessageProcess::operator()(const std::vector<Message>&) {
//    return dis(gen);
//}
//
//std::mt19937 BernoulliMessageProcess::gen((std::random_device())());
//
///*
// * NewMessageProcess definitions
// **/
//
//NewMessageProcess::NewMessageProcess(const double _p_spread, const double _p_new, const double _p_resend) : p_spread(_p_spread), p_resend(_p_resend), p_new(_p_new), dis_spread(_p_spread), dis_resend(_p_resend), dis_new(_p_new) {}
//
//long newMessageProcess::NewMessageProcess(const std::vector<MessageChainTopic>& _service_history, const size_t _topic) {
//    if (!servicedTopic(_service_history,_topic)) {
//        if (dis_spread(gen)){
//            if (dis_resend(gen)) {
//                return -1; // -1 indicates send same topic
//            } else if (dis_new(gen)) {
//                return topic_counter++; // send different topic
//            }
//        } else if  {
//            return -2; // don't send
//        }
//    }
//}
//
//bool newMessageProcess::servicedTopic(const std::vector<MessageChainTopic>& _service_history, const size_t _topic) const {
//    for (const auto& x : _service_history) {
//        if (x.second == _topic) {
//            return true;
//        }
//    }
//    return false;
//}
//
//std::mt19937 newMessageProcess::gen((std::random_device())());
//
//size_t newMessageProcess::topic_counter = 1;
