#ifndef ___Misc___
#define ___Misc___

#include <utility>
#include <tuple>
#include <random>
#include <iostream>

class PairCompare {
public:

	bool operator()(const std::pair<double,size_t>& lhs, const std::pair<double,size_t>& rhs) const {
		return lhs.first > rhs.first;
	}

};
//
//class MessageChainTopicCompare {
//
//    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
//    typedef std::vector<Message> MessageChain;
//    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
//
//public:
//    bool operator()(const MessageChainTopic& lhs, const MessageChainTopic& rhs) const {
//        return lhs.first.back().first > rhs.first.back().first;
//    }
//};

// templated functor to generator parameters using the distributions T, Ts, ...
template <typename... Ts>
class ParamGenerator {
public:
	template <typename... Ss>
    ParamGenerator(Ss&&... ss) : distributions(std::make_tuple<Ts...>(Ts(std::forward<Ss>(ss))...)) {}

    template <typename Gen>
	auto operator()(Gen& gen) {
		return createTuple(std::index_sequence_for<Ts...>{},gen);
	}
private:

	template <size_t... Is, typename Gen>
	auto createTuple(std::index_sequence<Is...>, Gen& gen) {
		return std::make_tuple(std::get<Is>(distributions)(gen)...);
	}

	std::tuple<Ts...> distributions;
};

template <template <typename...> class, typename...>
class is_instance_of : public std::false_type {};

template <template <typename...> class Te, typename... Ts>
class is_instance_of<Te,Te<Ts...>> : public std::true_type {};

template <template <typename...> class Te, typename T>
inline constexpr bool is_instance_of_v = is_instance_of<Te, T>::value;


#endif
