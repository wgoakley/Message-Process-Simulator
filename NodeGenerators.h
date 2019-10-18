#ifndef ___NodeGenerators___
#define ___NodeGenerators___

#include <tuple>
#include "ProbabilityDistributions.h"
#include "DataStructures.h"
#include "Misc.h"

// generates an entire vector of Node objects
template <template <typename,typename,typename,typename,typename,typename,typename,typename> class NodeTemplate, typename DatumType, typename StorageType, typename HistoryType, typename ArrivalPolicyType, typename SendPolicyType, typename ServicePolicyType, typename ActiveStartPolicyType, typename ActiveEndPolicyType, typename ExogeneousArrivalType, typename StorageParamGen, typename ArrivalPolicyParamGen, typename SendPolicyParamGen, typename ServicePolicyParamGen, typename ActiveStartPolicyParamGen, typename ActiveEndPolicyParamGen, typename ExogeneousArrivalParamGen>
class NodeGenerator {
public:

    using nodeType = NodeTemplate<DatumType, StorageType, HistoryType, ArrivalPolicyType, SendPolicyType, ServicePolicyType, ActiveStartPolicyType, ActiveEndPolicyType>;
    
	NodeGenerator(size_t _sz, StorageParamGen&& _storage_param_gen, ArrivalPolicyParamGen&& _arrival_policy_param_gen,
									 SendPolicyParamGen&& _send_policy_param_gen, ServicePolicyParamGen&& _service_policy_param_gen,
									 ActiveStartPolicyParamGen&& _active_start_policy_param_gen, ActiveEndPolicyParamGen&& _active_end_policy_param_gen,
									 ExogeneousArrivalParamGen&& _exogeneous_arrival_param_gen)
    : sz(_sz),
    storage_param_gen(std::forward<StorageParamGen>(_storage_param_gen)),
    arrival_policy_param_gen(std::forward<ArrivalPolicyParamGen>(_arrival_policy_param_gen)),
    send_policy_param_gen(std::forward<SendPolicyParamGen>(_send_policy_param_gen)),
    service_policy_param_gen(std::forward<ServicePolicyParamGen>(_service_policy_param_gen)),
    active_start_policy_param_gen(std::forward<ActiveStartPolicyParamGen>(_active_start_policy_param_gen)),
    active_end_policy_param_gen(std::forward<ActiveEndPolicyParamGen>(_active_end_policy_param_gen)),
    exogeneous_arrival_param_gen(std::forward<ExogeneousArrivalParamGen>(_exogeneous_arrival_param_gen))  {}

    std::vector<nodeType> generate() {
		std::vector<nodeType> v;
		v.reserve(sz);
		for (size_t i = 0 ; i < sz ; ++i) {
            v.emplace_back(i,storage_param_gen(gen),arrival_policy_param_gen(gen),send_policy_param_gen(gen),service_policy_param_gen(gen),active_start_policy_param_gen(gen),active_end_policy_param_gen(gen));
		}
        ExogeneousArrivalType exogeneous_arrival(std::move(std::make_from_tuple<ExogeneousArrivalType>(exogeneous_arrival_param_gen(gen))));
        exogeneous_arrival(v);
		return v;
	}

private:
	size_t sz;
    StorageParamGen storage_param_gen;
    ArrivalPolicyParamGen arrival_policy_param_gen;
    SendPolicyParamGen send_policy_param_gen;
    ServicePolicyParamGen service_policy_param_gen;
    ActiveStartPolicyParamGen active_start_policy_param_gen;
    ActiveEndPolicyParamGen active_end_policy_param_gen;
    ExogeneousArrivalParamGen exogeneous_arrival_param_gen;
	static std::mt19937 gen;
};

template <template <typename,typename,typename,typename,typename,typename,typename,typename> class NodeTemplate, typename DatumType, typename StorageType, typename HistoryType, typename ArrivalPolicyType, typename SendPolicyType, typename ServicePolicyType, typename ActiveStartPolicyType, typename ActiveEndPolicyType, typename ExogeneousArrivalType, typename StorageParamGen, typename ArrivalPolicyParamGen, typename SendPolicyParamGen, typename ServicePolicyParamGen, typename ActiveStartPolicyParamGen, typename ActiveEndPolicyParamGen, typename ExogeneousArrivalParamGen>
std::mt19937 NodeGenerator<NodeTemplate, DatumType, StorageType, HistoryType, ArrivalPolicyType, SendPolicyType, ServicePolicyType, ActiveStartPolicyType, ActiveEndPolicyType, ExogeneousArrivalType, StorageParamGen, ArrivalPolicyParamGen, SendPolicyParamGen, ServicePolicyParamGen, ActiveStartPolicyParamGen, ActiveEndPolicyParamGen, ExogeneousArrivalParamGen>::gen((std::random_device())());

#endif
