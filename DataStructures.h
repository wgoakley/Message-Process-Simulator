#ifndef ___DataStructures___
#define ___DataStructures___

#include <vector>
#include <tuple>
#include <queue>
#include <unordered_map>
#include "StochasticProcesses.h"
#include "Misc.h"
#include <algorithm>
#include <initializer_list>
#include "ModelParts.h"
#include "Storage.h"

template <typename DatumType, typename StorageType, typename HistoryType, typename ArrivalPolicyType, typename SendPolicyType, typename ServicePolicyType, typename ActiveStartPolicyType, typename ActiveEndPolicyType>
class Node {
public:
    
    using datumType = DatumType;
    
    template <typename... StorageTupleParams, typename... ArrivalTupleParams, typename... SendTupleParams, typename... ServiceTupleParams, typename... ActiveStartTupleParams, typename... ActiveEndTupleParams>
    Node(size_t _node_id, std::tuple<StorageTupleParams...>&& _storage_params, std::tuple<ArrivalTupleParams...>&& _arrival_policy_params, std::tuple<SendTupleParams...>&& _send_policy_params, std::tuple<ServiceTupleParams...>&& _service_policy_params, std::tuple<ActiveStartTupleParams...>&& _active_start_policy_params, std::tuple<ActiveEndTupleParams...>&& _active_end_policy_params) : Node(_node_id, std::move(_storage_params), std::index_sequence_for<StorageTupleParams...>(), std::move(_arrival_policy_params), std::index_sequence_for<ArrivalTupleParams...>(), std::move(_send_policy_params), std::index_sequence_for<SendTupleParams...>(), std::move(_service_policy_params), std::index_sequence_for<ServiceTupleParams...>(), std::move(_active_start_policy_params), std::index_sequence_for<ActiveStartTupleParams...>(), std::move(_active_end_policy_params), std::index_sequence_for<ActiveEndTupleParams...>())
    {}
    
    std::pair<double,size_t> update(double _current_time) {
        current_time = _current_time;
        // case when you try to become active but there are no new messages
        if (storage.empty()) {
            current_time = active_start_policy(current_time);
            activeStartUpdate();
            return std::make_pair(current_time,0);
        }
        
        // otherwise you try to send a message and generate the next update
        auto send = send_policy(current_time);
        if (send == 2) {
            msgIdIncrement();
        }
        
        current_msg_id = 0;
        if (send == 1 || send == 2) {
            current_msg_id = msg_id_counter;
            serviceUpdate();
        }
        
        if (active_end_policy(current_time)) {
            activeEndUpdate();
            return std::make_pair(active_start_policy(current_time),msg_id_counter);
        } else {
            return std::make_pair(service_policy(current_time),msg_id_counter);
        }
    }
    
    // add arrivals to node
    void pushArrival(datumType _datum) {
        storage.put(_datum);
    }
    
    double initialUpdateTime() {
        current_time = active_start_policy(0);
        activeStartUpdate();
        return current_time;
    }
    
    HistoryType const& getHistory() const {
        return history;
    }
    
    static size_t getMsgId() {
        return msg_id_counter;
    }

    
private:
    
    template <typename... StorageTupleParams, size_t... StorageTupleIndices, typename... ArrivalTupleParams, size_t... ArrivalTupleIndices, typename... SendTupleParams, size_t... SendTupleIndices, typename... ServiceTupleParams, size_t... ServiceTupleIndices, typename... ActiveStartTupleParams, size_t... ActiveStartTupleIndices, typename... ActiveEndTupleParams, size_t... ActiveEndTupleIndices>
    Node(size_t _node_id, std::tuple<StorageTupleParams...>&& _storage_tuple, std::index_sequence<StorageTupleIndices...>, std::tuple<ArrivalTupleParams...>&& _arrival_tuple, std::index_sequence<ArrivalTupleIndices...>, std::tuple<SendTupleParams...>&& _send_tuple, std::index_sequence<SendTupleIndices...>, std::tuple<ServiceTupleParams...>&& _service_tuple, std::index_sequence<ServiceTupleIndices...>, std::tuple<ActiveStartTupleParams...>&& _active_start_tuple, std::index_sequence<ActiveStartTupleIndices...>, std::tuple<ActiveEndTupleParams...>&& _active_end_tuple, std::index_sequence<ActiveEndTupleIndices...>) : node_id(_node_id), current_msg_id(0), history(), storage(std::get<StorageTupleIndices>(_storage_tuple)...), arrival_policy(history,storage,std::get<ArrivalTupleIndices>(_arrival_tuple)...), send_policy(history,storage,std::get<SendTupleIndices>(_send_tuple)...), service_policy(history,storage,std::get<ServiceTupleIndices>(_service_tuple)...), active_start_policy(history,storage,std::get<ActiveStartTupleParams>(_active_start_tuple)...), active_end_policy(history,storage,std::get<ActiveEndTupleIndices>(_active_end_tuple)...) {}
    
    static void msgIdIncrement() {
        ++msg_id_counter;
    }
    
    void activeStartUpdate() {
        history.putActiveStart(current_time);
    }
    
    void activeEndUpdate() {
        history.putActiveEnd(current_time);
    }
    
    void serviceUpdate() {
        history.putService(datumType(current_msg_id,node_id,current_time),storage.get());
    }
    
    HistoryType history;
    StorageType storage;
    ArrivalPolicyType arrival_policy;
    SendPolicyType send_policy;
    ServicePolicyType service_policy;
    ActiveStartPolicyType active_start_policy;
    ActiveEndPolicyType active_end_policy;
    static size_t msg_id_counter;
    double current_time;
    size_t current_msg_id;
    size_t node_id;
    
};

template <typename DatumType, typename StorageType, typename HistoryType, typename ArrivalPolicyType, typename SendPolicyType, typename ServicePolicyType, typename ActiveStartPolicyType, typename ActiveEndPolicyType>
size_t Node<DatumType, StorageType, HistoryType, ArrivalPolicyType, SendPolicyType, ServicePolicyType, ActiveStartPolicyType, ActiveEndPolicyType>::msg_id_counter = 1;

// consists of node id of sender, time of sent, and id of message
// does not contain information about previous messages that created the message
// Things to do to a simple message:
//   1. Create new simple message
//   2.
//class SimpleMessage {
//
//    std::tuple<size_t,size_t,double> msg_data; // (id,sender,time)
//    static size_t msg_id_counter;
//};
//
//class MessageChain {
//
//};
//
//template <typename... Ts>
//class CoreDatumType : public std::tuple<size_t,size_t,double,Ts...> {
//public:
//    CoreDatumType(size_t const _msg_id, size_t const _sender_id, double const _time, Ts... ts) : std::tuple<size_t,size_t,double,Ts...>(_msg_id,_sender_id,_time,ts...) {}
//};
//
//typedef CoreDatumType<> DefaultDatumType;
//
//template <typename CRTP, typename DatumType>
//class DatumEqualityType {
//public:
//
//    bool operator()(DatumType const& _datum) const {
//        return static_cast<CRTP const&>(*this)(_datum);
//    }
//    void init(DatumType _datum) {
//        std::swap(datum,_datum);
//    }
//
//private:
//
//    DatumType datum;
//};
//
//template <typename DatumType>
//class TopicalDatumEqualityType : public DatumEqualityType<TopicalDatumEqualityType<DatumType>,DatumType> {
//public:
//
//    bool operator()(DatumType const& _datum) const {
//        return std::get<0>(_datum) == std::get<0>(datum);
//    }
//
//private:
//
//    DatumType datum;
//};
//
//template <typename DatumType>
//class TopicalSenderDatumEqualityType : public DatumEqualityType<TopicalSenderDatumEqualityType<DatumType>, DatumType> {
//public:
//
//    bool operator()(DatumType const& _datum) const {
//        return (std::get<0>(_datum) == std::get<0>(datum)) && (std::get<1>(_datum) == std::get<1>(datum));
//    }
//
//private:
//
//    DatumType datum;
//};
//
//template <typename CRTP, typename DatumType>
//class Storage {
//public:
//    // add to storage
//    void put(DatumType _datum) {
//        static_cast<CRTP const&>(*this).put(_datum);
//    }
//
//    // get from storage and remove from storage
//    DatumType pop() {
//        DatumType const& datum = get();
//        DatumType datum_copy = datum;
//        erase(datum);
//        return datum_copy;
//    }
//
//    DatumType const& get() const {
//        return static_cast<CRTP const&>(*this).get();
//    }
//
//    template <typename IteratorType>
//    void copy(IteratorType it1, IteratorType it2) {
//        static_cast<CRTP const&>(*this).copy(it1,it2);
//    }
//
//private:
//
//    void erase(DatumType const& _datum) {
//        static_cast<CRTP const&>(*this).erase(_datum);
//    }
//
//};
//
//// node generator will generate the storage params.
//template <typename DatumType, typename CompareType>
//class Feed : public Storage<Feed<DatumType,CompareType>,DatumType> {
//
//    Feed(const size_t _functional_length) : functional_length(_functional_length), functional_size(0) {}
//
//    void put(DatumType _datum) {
//        ++functional_size;
//        data.push(_datum);
//    }
//
//    DatumType const& get() const {
//        return data.top();
//    }
//
//    template <typename IteratorType>
//    void copy(IteratorType it1, IteratorType it2) {
//        while (it1 < it2) {
//            data.push(*it1);
//        }
//    }
//
//private:
//
//    void erase(DatumType const& _datum) {
//        if (functional_size == 0) {
//            throw std::out_of_range("functional size is 0");
//        }
//        --functional_size;
//        data.pop();
//    }
//
//    size_t const functional_length;
//    size_t functional_size;
//    std::priority_queue<DatumType,std::vector<DatumType>,CompareType> data;
//
//};
//
//// infer the keytype and datumtype from datatype!
//template <typename DatumType, typename DataType>
//class Pool : public Storage<Pool<DatumType,DataType>, DatumType> {
//public:
//
//    void put(DatumType _datum) {
//        data.emplace(std::get<0>(_datum),_datum); // assumes a datum is a tuple with identifier in pos 0
//    }
//
//    DatumType const& get() const {
//        size_t sz = data.size();
//        std::uniform_int_distribution<size_t> dis(0,sz-1);
//        std::mt19937 gen((std::random_device())());
//        size_t pos = dis(gen);
//        auto it = data.begin();
//        for (size_t j = 0 ; j < pos ; ++j, ++it) {}
//        return *it;
//    }
//
//    template <typename IteratorType>
//    void copy(IteratorType it1, IteratorType const it2) {
//        data.insert(it1,it2);
//    }
//
//private:
//
//    // erase all messages of the same topic
//    void erase(DatumType const& _datum) {
//        data.erase(std::get<0>(_datum));
//    }
//
//    DataType data; // assume key is a size_t
//
//};
//
//template <typename DatumType>
//using UniquePool = Pool<DatumType,std::unordered_map<size_t,DatumType>>;
//
//template <typename DatumType>
//using MultiPool = Pool<DatumType,std::unordered_multimap<size_t,DatumType>>;
//
//// Contains service history and history of messages received
//template <typename DatumType>
//class History {
//public:
//
//    typedef DatumType datumType;
//
//    void putService(DatumType _datum_sent, DatumType _datum_received) {
//        service_history.push_back(std::make_pair(_datum_sent,_datum_received));
//    }
//
//    void putArrival(DatumType _datum) {
//        arrival_history.push_back(_datum);
//    }
//
//    void putActiveStart(double _time) {
//        if (active_period_history.rbegin()->second != 0) {
//            active_period_history.push_back(std::make_pair(_time,0));
//        } else {
//            active_period_history.rbegin()->first = _time;
//        }
//    }
//
//    void putActiveEnd(double _time) {
//        active_period_history.rbegin()->second = _time;
//    }
//
//   std::vector<std::pair<DatumType,DatumType>> const& getService() const {
//        return service_history;
//    }
//
//    std::vector<DatumType> const& getArrivals() const {
//        return arrival_history;
//    }
//
//    std::vector<std::pair<double,double>> const& getActivePeriods() const {
//        return active_period_history;
//    }
//
//private:
//
//    std::vector<std::pair<DatumType,DatumType>> service_history;
//    std::vector<DatumType> arrival_history;
//    std::vector<std::pair<double,double>> active_period_history;
//};

//// DatumCompare is a Unary operator that is constructed from a datum to be used for equality comparison.
//// Datum EqualityType requires an initializer function to seed the EqualityType with a value to compare with.
//template <typename CRTP, typename HistoryType, typename StorageType>
//class Interpreter {
//public:
//
//    typedef HistoryType historytype;
//    typedef StorageType storagetype;
//
//    typedef typename HistoryType::datumtype datumtype;
//
//    Interpreter() = default;
//
//    Interpreter(std::shared_ptr<HistoryType>&& _history_ptr, std::shared_ptr<StorageType>&& _storage_ptr) : history_ptr(std::move(_history_ptr)), storage_ptr(std::move(_storage_ptr)) {}
//
//    template <typename... Ts>
//    auto info(Ts&&... _ts) {
//        return static_cast<CRTP const&>(*this).info(_ts...);
//    }
//
//protected:
//
//    std::shared_ptr<HistoryType> history_ptr;
//    std::shared_ptr<StorageType> storage_ptr;
//
//};
//
//template <typename HistoryType = std::nullptr_t, typename StorageType = std::nullptr_t, typename DatumEqualityType = std::nullptr_t>
//class NullInterpreter : public Interpreter<NullInterpreter<>,HistoryType,StorageType> {
//
//public:
//
//    typedef Interpreter<NullInterpreter<>,HistoryType,StorageType> parent;
//
//    template <typename HistoryTypeForget, typename StorageTypeForget, typename DatumEqualityTypeForget>
//    NullInterpreter(HistoryTypeForget const& _history,StorageTypeForget const& _storage, DatumEqualityTypeForget&& _eq) : parent(std::make_shared<HistoryTypeForget>(nullptr),std::make_shared<StorageTypeForget>(nullptr)) {}
//
//    template <typename... Ts>
//    auto info(Ts&&... _ts) {
//        return false;
//    }
//
//};
//
//using DefaultInterpreter = NullInterpreter<>;
//
//template <typename HistoryType, typename StorageType, typename DatumEqualityType>
//class CountArrivalsInterpreter : public Interpreter<CountArrivalsInterpreter<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> {
//
//public:
//
//    typedef Interpreter<CountArrivalsInterpreter<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> parent;
//
//    CountArrivalsInterpreter(HistoryType const& _history, StorageType const& _storage, DatumEqualityType&& _eq) : parent(std::make_shared<HistoryType>(_history),std::make_shared<StorageType>(_storage)), eq(std::move(_eq)) {}
//
//    auto info() {
//        return parent::history_ptr->getArrivals().size();
//    }
//
//    template <typename... Ts>
//    auto info(Ts&&... _ts) {
//        eq.init(_ts...);
//        auto arrival_history = parent::history_ptr->getArrivals();
//        return (std::count_if(arrival_history.begin(),arrival_history.end(),eq));
//    }
//
//private:
//
//    DatumEqualityType eq;
//
//};
//
//
//
//template <typename HistoryType, typename StorageType, typename DatumEqualityType>
//class CountServiceInterpreter : public Interpreter<CountServiceInterpreter<HistoryType, StorageType, DatumEqualityType>,HistoryType,StorageType> {
//public:
//
//    typedef Interpreter<CountServiceInterpreter<HistoryType, StorageType, DatumEqualityType>,HistoryType,StorageType> parent;
//
//    CountServiceInterpreter(HistoryType const& _history, StorageType const& _storage, DatumEqualityType&& _eq) : parent(std::make_shared<HistoryType>(_history),std::make_shared<StorageType>(_storage)), eq(std::move(_eq)) {}
//
//    auto info() {
//        return parent::history_ptr->getService().size();
//    }
//
//    template <typename... Ts>
//    auto info(Ts&&... _ts) {
//        eq.init(_ts...);
//        auto service_history = parent::history_ptr->getService();
//        return (std::count_if(service_history.begin(),service_history.end(),eq));
//    }
//private:
//
//    DatumEqualityType eq;
//
//};
//
//// shouldn't have history as a variable but i want the class construction to be the same for all arrival policies...
//template <typename CRTP, typename HistoryType, typename StorageType>
//class ArrivalPolicy {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    ArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
//
//    bool operator()() {
//        return static_cast<CRTP const&>(*this)();
//    }
//
//protected:
//
//    HistoryType const& history;
//    StorageType const& storage;
//
//};
//
//// inherit HistoryInterpreter as well!
//template <typename HistoryType, typename StorageType>
//class NullArrivalPolicy : public ArrivalPolicy<NullArrivalPolicy<HistoryType,StorageType>,HistoryType,StorageType> {
//public:
//    typedef ArrivalPolicy<NullArrivalPolicy<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    NullArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
//
//    bool operator()() {
//        return true;
//    }
//
//};
//
//template <typename HistoryType, typename StorageType, typename DatumEqualityType>
//class NoneArrivalPolicy : public ArrivalPolicy<NoneArrivalPolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType>  {
//public:
//    typedef NoneArrivalPolicy<NoneArrivalPolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    NoneArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
//
//    bool operator()() {
//        eq.init(parent::storage.get());
//        return std::count_if(parent::history.getArrivals().begin(),parent::history.getArrivals().end(),eq)==0;
//    }
//
//private:
//    DatumEqualityType eq;
//};
//
//template <typename HistoryType, typename StorageType, typename DatumEqualityType>
//class NoneServicePolicy : public NoneServicePolicy<NoneServicePolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType>  {
//public:
//    typedef NoneServicePolicy<NoneServicePolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    NoneServicePolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
//
//    bool operator()() {
//        eq.init(parent::storage.get());
//        return std::count_if(parent::history.getService().begin(),parent::history.getService().end(),eq)==0;
//    }
//
//private:
//    DatumEqualityType eq;
//};
//
//// Sender & creator template: class to decide whether to send a message or to create a message
//template <typename CRTP, typename HistoryType, typename StorageType>
//class Sender {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    Sender(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage){}
//
//    typename std::tuple_element<0,datumType>::type operator()(double _current_time) {
//        return static_cast<CRTP const&>(*this)(_current_time);
//    }
//
//protected:
//
//    HistoryType const& history;
//    StorageType const& storage;
//
//private:
//
//};
//
//class DiscreteThreeDistribution {
//public:
//    DiscreteThreeDistribution(double _p_resend, double _p_create) : p_resend(_p_resend), p_create(_p_create), dis({1-_p_resend-_p_create,_p_resend,_p_create}) {}
//
//    unsigned operator()() {
//        switch (dis(gen)) {
//            case 0 : return 0; break;
//            case 1 : return 1; break;
//            case 2 : return 2; break;
//        }
//        return 0; // stupid
//    }
//
//private:
//
//    static std::mt19937 gen;
//    std::discrete_distribution<unsigned> dis;
//    double p_resend;
//    double p_create;
//};
//
//std::mt19937 DiscreteThreeDistribution::gen((std::random_device())());
//
//template <typename HistoryType, typename StorageType>
//class DefaultSender : public Sender<DefaultSender<HistoryType,StorageType>,HistoryType,StorageType> {
//public:
//
//    typedef Sender<DefaultSender<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    DefaultSender(HistoryType const& _history, double _p_resend, double _p_create) : parent(_history), send_process(_p_resend, _p_create) {}
//
//    typename std::tuple_element<0,datumType>::type operator()(double _current_time) {
//        auto datum = parent::storage.get();
//        auto result = send_process();
//        if (result == 0) {
//            return 0;
//        } else if (result == 1) {
//            return 1;
//        } else { // == 2
//            return 2;
//        }
//    }
//
//private:
//
//    DiscreteThreeDistribution send_process;
//};
//
//
//// should generate the next service time
//// should be able to use the history...
//template <typename CRTP, typename HistoryType, typename StorageType>
//class Service {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    template <typename... Args>
//    Service(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
//
//    double operator()(double _current_time) const {
//        return static_cast<CRTP const&>(*this)(_current_time);
//    }
//
//protected:
//
//    HistoryType const& history;
//    StorageType const& storage;
//};
//
////////////
//
//// ServiceTimesProcess should be a Markov type process... Concept?
//// Going to need ANOTHER class in Node to decide how to load up some of these functions... such as nextService... with data from the process.  Just one piece of data or multiple?  Save for another day!
//template <typename HistoryType, typename StorageType>
//class MarkovService : public Service<MarkovService<HistoryType,StorageType>,HistoryType,StorageType>
//{
//public:
//
//    typedef Service<MarkovService<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    MarkovService(HistoryType const& _history, StorageType const& _storage, double _r) : parent(_history,_storage), r(_r), dis(_r) {}
//
//    double operator()(double _current_time) const {
//        return _current_time + dis(gen);
//    }
//
//private:
//
//    double r;
//    std::exponential_distribution<double> dis;
//    static std::mt19937 gen;
//};
//
//template <typename HistoryType, typename StorageType>
//std::mt19937 MarkovService<HistoryType,StorageType>::gen((std::random_device())());
//
//
//// decides when next active period begins, decides when active period ends
//template <typename CRTP, typename HistoryType, typename StorageType>
//class ActiveStart {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    ActiveStart(HistoryType const& _history) : history(_history) {}
//
//    double operator()(double _current_time) {
//        return static_cast<CRTP const&>(*this)(_current_time);
//    }
//
//protected:
//
//    HistoryType const& history;
//    StorageType const& storage;
//};
//
//template <typename HistoryType, typename StorageType>
//class MarkovActiveStart : public ActiveStart<MarkovActiveStart<HistoryType,StorageType>,HistoryType,StorageType>
//{
//public:
//
//    typedef ActiveStart<MarkovActiveStart<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    MarkovActiveStart(HistoryType const& _history, StorageType const& _storage, double _r) : parent(_history,_storage), r(_r), dis(_r) {}
//
//    double operator()(double _current_time) const {
//        return _current_time + dis(gen);
//    }
//
//private:
//
//    double r;
//    std::exponential_distribution<double> dis;
//    static std::mt19937 gen;
//};
//
//template <typename HistoryType, typename StorageType>
//std::mt19937 MarkovActiveStart<HistoryType,StorageType>::gen((std::random_device())());
//
//
//// decides when next active period begins, decides when active period ends
//template <typename CRTP, typename HistoryType, typename StorageType>
//class ActiveEnd {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    ActiveEnd(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
//
//    bool operator()(double _current_time) {
//        return static_cast<CRTP const&>(*this)(_current_time);
//    }
//
//protected:
//
//    HistoryType const& history;
//    StorageType const& storage;
//};
//
//// decides when next active period begins, decides when active period ends
//template <typename HistoryType, typename StorageType>
//class ActiveEndExhaustive : public ActiveEnd<ActiveEndExhaustive<HistoryType,StorageType>,HistoryType,StorageType> {
//public:
//
//    typedef ActiveEnd<ActiveEndExhaustive<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    ActiveEndExhaustive(HistoryType const& _history, StorageType const& _storage) : parent(_history, _storage) {}
//
//    bool operator()(double _current_time) {
//        return parent::storage.size()==0;
//    }
//};
//
//template <typename HistoryType, typename StorageType>
//class ActiveEndBernoulli : public ActiveEnd<ActiveEndBernoulli<HistoryType,StorageType>,HistoryType,StorageType> {
//public:
//
//    typedef ActiveEnd<ActiveEndBernoulli<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    ActiveEndBernoulli(HistoryType const& _history, StorageType const& _storage, double _p) : parent(_history, _storage), p(_p), dis(_p)  {}
//
//    bool operator()(double _current_time) {
//        return parent::storage.size()==0 || dis(gen)==0;
//    }
//
//private:
//
//    double p;
//    std::bernoulli_distribution dis;
//    static std::mt19937 gen;
//};
//
//template <typename CRTP, typename NodeType>
//class ExogeneousArrival {
//public:
//
//    typedef typename NodeType::datumType datumType;
//
//    double operator()(std::vector<NodeType>& _nodes) {
//        return static_cast<CRTP const&>(*this)(_nodes);
//    }
//
//};
//
//template <typename NodeType>
//class ExponentialExogeneousArrival : public ExogeneousArrival<ExponentialExogeneousArrival<NodeType>,NodeType> {
//public:
//
//    typedef ExogeneousArrival<ExponentialExogeneousArrival<NodeType>,NodeType> parent;
//    typedef typename parent::datumType datumType;
//
//    ExponentialExogeneousArrival(double _r, double _T, double _p) : r(_r), T(_T), exp_dis(r), p(_p), bern_dis(p) {}
//
//    double operator()(std::vector<NodeType>& _nodes) {
//        for (auto it = _nodes.begin(); it != _nodes.end() ; ++it) {
//            if (bern_dis(gen)) {
//                it->pushArrival(datumType(NodeType::msg_id_counter,0,exp_dis(gen)));
//            }
//        }
//    }
//
//protected:
//    double r;
//    double T;
//    double p;
//    std::exponential_distribution<double> exp_dis;
//    std::bernoulli_distribution bern_dis;
//    static std::mt19937 gen;
//};
//
//template <typename NodeType>
//std::mt19937 ExponentialExogeneousArrival<NodeType>::gen((std::random_device())());

//
//template <typename CRTP, typename HistoryType, typename StorageType>
//class DatumConstructor {
//public:
//
//    typedef typename HistoryType::datumType datumType;
//
//    DatumConstructor(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
//
//    datumType operator()(typename std::tuple_element<0,datumType>::type _sender_id, typename std::tuple_element<1,datumType>::type _msg_id, typename std::tuple_element<0,datumType>::type _time) {
//        return static_cast<CRTP const&>(*this)(_sender_id, _msg_id, _time);
//    }
//private:
//    HistoryType const& history;
//    StorageType const& storage;
//};
//
//template <typename HistoryType, typename StorageType>
//class CoreDatumConstructor : public DatumConstructor<CoreDatumConstructor<HistoryType,StorageType>,HistoryType,StorageType> {
//public:
//
//    typedef DatumConstructor<CoreDatumConstructor<HistoryType,StorageType>,HistoryType,StorageType> parent;
//    typedef typename parent::datumType datumType;
//
//    CoreDatumConstructor(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
//
//    datumType operator()(typename std::tuple_element<0,datumType>::type _sender_id, typename std::tuple_element<1,datumType>::type _msg_id, typename std::tuple_element<0,datumType>::type _time) {
//        return datumType(_sender_id,_msg_id,_time);
//    }
//
//};


//
//
//template <typename Arrival, typename Service, typename Login, typename Send, typename Size, typename Logged, typename CompareType>
//class Node {
//public:
//
//    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
//    typedef std::vector<Message> MessageChain;
//    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
//
//    template <typename ArrivalParams,typename ServiceParams, typename LoginParams, typename SendParams, typename SizeParams, typename LoggedParams>
//    Node<Arrival,Service,Login,Send>(
//                                    ArrivalParams _arrival_param, ServiceParams _service_param,
//                                     LoginParams _login_param, SendParams _send_param,
//                                     SizeParams _size_param, LoggedParams _logged_param)
//    : arrivalProcess(std::move(std::make_from_tuple<Arrival,ArrivalParams>(std::move(_arrival_param)))),
//    serviceProcess(std::move(std::make_from_tuple<Service,ServiceParams>>(std::move(_service_param)))),
//    loginProcess(std::move(std::make_from_tuple<Login,LoginParams>(std::move(_login_param)))),
//    sendProcess(std::move(std::make_from_tuple<Send,SendParams>(std::move(_send_param)))),
//    sizeProcess(std::move(std::make_from_tuple<Size,SizeParams>(std::move(_size_param)))),
//    loggedProcess(std::move(std::make_from_tuple<Logged,LoggedParams>(std::move(_logged_param)))),
//                                        feed(sizeProcess(gen)),
//                                        logged(loggedProcess(gen)) {}
//
//    std::pair<double,size_t> update(const double _current_time) {
//        double new_time = genUpdate(_current_time); // generate new update time
//        if (!feed.functionallyEmpty(_current_time)) { // check if feed is functionally empty
//            return std::make_pair(new_time,sendMessage(feed.top(),_current_time));
//        }
//        return std::make_pair(new_time,0);
//    }
//    bool sendMessage(const MessageChainTopic& _msg_chain_topic,const double _current_time) {
//        long sent = sendProcess(feed.getService(),_msg_chain_topic.second);
//        if (sent == -2) {
//            feed.pushService(_msg_chain_topic,_current_time,0);
//        } else if (sent == -1) {
//            feed.pushService(_msg_chain_topic,_current_time,1);
//        } else {
//            feed.pushService(_current_time,sent);
//        }
//        feed.pop();
//        return (sent>=-1);
//    }
//    const MessageChainTopic& getSentMessage() {
//        return feed.getLastService();
//    }
//    const std::vector<MessageChainTopic>& getService() const {
//        return feed.getService();
//    }
//    const std::vector<MessageChainTopic>& getArrivals() const {
//        return feed.getArrivals();
//    }
//    const std::vector<std::pair<double,double>>& getLogins() const {
//        return login_periods;
//    }
//    double genArrival(const double _current_time, const size_t _node_id, const size_t _topic, const double _max_time = std::numeric_limits<double>::max()) {
//        // generate arrival and return arrival time
//        const double new_time = _current_time + arrivalProcess();
//        if (new_time < _max_time) {
//            push(new_time, _node_id, _topic);
//        }
//        return new_time;
//    }
//    double genUpdate(const double _current_time) {
//        bool fun_empty = feed.functionallyEmpty(_current_time);
//        if (logged && !fun_empty) {
//            return genService(_current_time);
//        } else if (logged && fun_empty) {
//            logged = false;
//            return genLogin(_current_time);
//        } else if (!logged && !fun_empty) {
//            double tmp = genService(_current_time);
//            logged = true;
//            return tmp;
//        } else { // !logged && feed.functionallyEmpty(_current_time)
//            return genLogin(_current_time);
//        }
//    }
//    double genLogin(const double _current_time) {
//        const double new_time = _current_time + loginProcess();
//        return new_time;
//    }
//    double genService(const double _current_time) {
//        const double new_time = _current_time + serviceProcess();
//        if (!logged) {
//            login_periods.push_back(std::make_pair(_current_time,new_time));
//        } else if (login_periods.empty()){
//            login_periods.push_back(std::make_pair(0,new_time));
//        } else {
//            login_periods.back().second = new_time;
//        }
//        return new_time;
//    }
//    void push(const double _time, const double _node_id, const double _topic) {
//        feed.push(_time,_node_id,_topic);
//        feed.pushArrival(_time,_node_id,_topic);
//    }
//    void push(const Message& _msg_chain_topic) {
//        feed.push(_msg_chain_topic);
//        feed.pushArrival(_msg_chain_topic);
//    }
//    void push(const Message& _msg_chain_topic, const double _current_time, const size_t _node_id) {
//        feed.push(_msg_chain_topic,_current_time,_node_id);
//        feed.pushArrival(_msg_chain_topic,_current_time,_node_id);
//    }
//private:
//    std::vector<std::pair<double,double>> login_periods; // login periods of the user (logged in, logged out)
//    Arrival arrivalProcess;
//    Service serviceProcess;
//    Login loginProcess;
//    Send sendProcess;
//    Size sizeProcess;
//    Logged loggedProcess;
//    static std::mt19937 gen;
//    Feed<CompareType> feed;
//    bool logged; // is the user logged in or is the user not?
//};
//
//template <typename Arrival, typename Service, typename Login, typename Send, typename Size, typename Logged, typename CompareType>
//std::mt19937 Node< Arrival,  Service,  Login,  Send,  Size,  Logged, CompareType>::gen((std::random_device())());
//
//
//template <typename CompareType>
//class Feed {
//public:
//
//    typedef std::pair<double,size_t> Message; // (time sent, id of sender)
//    typedef std::vector<Message> MessageChain;
//    typedef std::pair<MessageChain,size_t> MessageChainTopic; // (MessageChain,topic)
//
//    Feed(const size_t _functional_length) : functional_length(_functional_length), functional_size(0) {}
//
//    void push(const double _time, const size_t _node_id, const size_t _topic) {
//        msg_queue.push(std::make_pair(MessageChain{std::make_pair(_time,_node_id)},_topic));
//        functional_size+=(functional_size<functional_length);
//    }
//    void push(const MessageChainTopic& _msg_chain_topic) {
//        msg_queue.push(_msg_chain_topic);
//        functional_size+=(functional_size<functional_length);
//    }
//    void push(const MessageChainTopic& _msg_chain_topic, const double _time, const size_t _node_id) {
//        msg_queue.push(_msg_chain_topic);
//        msg_queue.top().first.push_back(std::make_pair(_time,_node_id));
//        functional_size+=(functional_size<functional_length);
//    }
//
//    void pop() {
//        if (functional_size == 0) {
//            throw std::out_of_range("functional size is 0");
//        }
//        msg_queue.pop();
//        --functional_size;
//    }
//
//    const MessageChainTopic& top() {
//        return msg_queue.top();
//    }
//
//    const MessageChainTopic& top() const {
//        return msg_queue.top();
//    }
//
//    size_t functionalSize() const {
//        return functional_size;
//    }
//
//    bool functionallyEmpty(const double _current_time) const {
//        if (functional_size == 0) {
//            return true;
//        }
//        if (auto msg = top(); msg.back().first >= _current_time) {
//            return true;
//        }
//        return false;
//    }
//
//    void pushArrival(const double _time, const size_t _node_id, const size_t _topic) {
//        arrival_history.push_back(std::make_pair(MessageChain{std::make_pair(_time,_node_id)},_topic));
//    }
//    void pushArrival(const MessageChainTopic& _msg_chain_topic) {
//        arrival_history.push_back(_msg_chain_topic);
//    }
//    void pushArrival(const MessageChainTopic& _msg_chain_topic, const double _time, const size_t _node_id) {
//        arrival_history.push_back(_msg_chain_topic);
//        arrival_history.back().first.push_back(std::make_pair(_time,_node_id));
//    }
//    void pushService(const MessageChainTopic& _msg_chain_topic, const double _time, const bool _sent) {
//        service_history.push_back(_msg_chain_topic);
//        service_history.back().first.push_back(std::make_pair(_time,_sent));
//    }
//
//    void pushService(const double _time, const long _topic) {
//        service_history.push_back(std::make_pair(MessageChain{std::make_pair(_time,1)},_topic));
//    }
//
//    const std::vector<MessageChainTopic>& getArrivals() const {
//        return arrival_history;
//    }
//
//    const std::vector<MessageChainTopic>& getService() const {
//        return service_history;
//    }
//
//    const MessageChainTopic& getLastService() const {
//        return service_history.back();
//    }
//private:
//
//    const size_t functional_length;
//    size_t functional_size;
//    std::priority_queue<MessageChainTopic,std::vector<MessageChainTopic>,CompareType> msg_queue;
//    std::vector<MessageChainTopic> arrival_history;
//    std::vector<MessageChainTopic> service_history;
//};

#endif
