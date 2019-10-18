//
//  Datum.h
//  NetworkSim
//
//  Created by William Oakley on 1/9/19.
//  Copyright © 2019 William Oakley. All rights reserved.
//

#ifndef Datum_h
#define Datum_h

#include <stack>

template <typename... Ts>
class CoreDatumType : public std::tuple<size_t,size_t,double,Ts...> {
public:
    CoreDatumType(size_t const _msg_id, size_t const _sender_id, double const _time, Ts... ts) : std::tuple<size_t,size_t,double,Ts...>(_msg_id,_sender_id,_time,ts...) {}
    
};

typedef CoreDatumType<> DefaultDatumType;

template <typename... Ts>
std::ostream& operator<<(std::ostream& _os, const CoreDatumType<Ts...>& _datum) {
    _os << '{' << std::get<0>(_datum) << ',' << std::get<1>(_datum) << ',' << std::get<2>(_datum);
    coreDatumPrint(_os,_datum,std::index_sequence_for<Ts...>());
    _os << '}';
    return _os;
}

template <typename... Ts, size_t... Is>
std::ostream& coreDatumPrint(std::ostream& _os, const CoreDatumType<Ts...>& _datum, std::index_sequence<Is...>) {
    (..., (_os << ',' << std::get<Is+3>(_datum)));
    return _os;
}

template <typename CRTP, typename DatumType>
class DatumEqualityType {
public:
    
    bool operator()(DatumType const& _datum) const {
        return static_cast<CRTP const&>(*this)(_datum);
    }
    void init(DatumType _datum) {
        std::swap(datum,_datum);
    }
    
private:
    
    DatumType datum;
};

template <typename DatumType>
class TopicalDatumEqualityType : public DatumEqualityType<TopicalDatumEqualityType<DatumType>,DatumType> {
public:
    
    bool operator()(DatumType const& _datum) const {
        return std::get<0>(_datum) == std::get<0>(datum);
    }
    
private:
    
    DatumType datum;
};

template <typename DatumType>
class TopicalSenderDatumEqualityType : public DatumEqualityType<TopicalSenderDatumEqualityType<DatumType>, DatumType> {
public:
    
    bool operator()(DatumType const& _datum) const {
        return (std::get<0>(_datum) == std::get<0>(datum)) && (std::get<1>(_datum) == std::get<1>(datum));
    }
    
private:
    
    DatumType datum;
};

template <typename CRTP, typename DatumType>
class Storage {
public:
    // add to storage
    void put(DatumType _datum) {
        static_cast<CRTP const&>(*this).put(_datum);
    }
    
    // get from storage and remove from storage
    DatumType pop() {
        DatumType const& datum = get();
        DatumType datum_copy = datum;
        erase(datum);
        return datum_copy;
    }
    
    DatumType const& get() const {
        return static_cast<CRTP const&>(*this).get();
    }
    
    template <typename IteratorType>
    void copy(IteratorType it1, IteratorType it2) {
        static_cast<CRTP const&>(*this).copy(it1,it2);
    }
    
    bool empty() {
        return static_cast<CRTP const&>(*this).empty();
    }
    
    size_t size() const {
        return static_cast<CRTP const&>(*this).size();
    }
    
private:
    
    void erase(DatumType const& _datum) {
        static_cast<CRTP const&>(*this).erase(_datum);
    }
    
};

// node generator will generate the storage params.
template <typename DatumType>
class Feed : public Storage<Feed<DatumType>,DatumType> {
public:
    
    Feed(const size_t _functional_length) : functional_length(_functional_length), functional_size(0) {}
    
    void put(DatumType _datum) {
        ++functional_size;
        data.push(_datum);
    }
    
    DatumType const& get() const {
        return data.top();
    }
    
    template <typename IteratorType>
    void copy(IteratorType it1, IteratorType it2) {
        while (it1 < it2) {
            data.push(*it1);
        }
    }
    
    bool empty() {
        return data.empty();
    }
    
    size_t size() const {
        return functional_size;
    }
    
private:
    
    void erase(DatumType const& _datum) {
        if (functional_size == 0) {
            throw std::out_of_range("functional size is 0");
        }
        --functional_size;
        data.pop();
    }
    
    size_t const functional_length;
    size_t functional_size;
    std::stack<DatumType> data;
    
};

// infer the keytype and datumtype from datatype!
template <typename DatumType, typename DataType>
class Pool : public Storage<Pool<DatumType,DataType>, DatumType> {
public:
    
    void put(DatumType _datum) {
        data.emplace(std::get<0>(_datum),_datum); // assumes a datum is a tuple with identifier in pos 0
    }
    
    DatumType const& get() const {
        size_t sz = data.size();
        std::uniform_int_distribution<size_t> dis(0,sz-1);
        std::mt19937 gen((std::random_device())());
        size_t pos = dis(gen);
        auto it = data.begin();
        for (size_t j = 0 ; j < pos ; ++j, ++it) {}
        return *it;
    }
    
    template <typename IteratorType>
    void copy(IteratorType it1, IteratorType const it2) {
        data.insert(it1,it2);
    }
    
    bool empty() {
        return data.empty();
    }
    
    size_t size() const {
        return data.size();
    }
private:
    
    // erase all messages of the same topic
    void erase(DatumType const& _datum) {
        data.erase(std::get<0>(_datum));
    }
    
    DataType data; // assume key is a size_t
    
};

template <typename DatumType>
using UniquePool = Pool<DatumType,std::unordered_map<size_t,DatumType>>;

template <typename DatumType>
using MultiPool = Pool<DatumType,std::unordered_multimap<size_t,DatumType>>;

// Contains service history and history of messages received
template <typename DatumType>
class History {
public:
    
    typedef DatumType datumType;
    
    void putService(DatumType _datum_sent, DatumType _datum_received) {
        service_history.push_back(std::make_pair(_datum_sent,_datum_received));
    }
    
    void putArrival(DatumType _datum) {
        arrival_history.push_back(_datum);
    }
    
    void putActiveStart(double _time) {
        if (active_period_history.empty() || active_period_history.rbegin()->second != 0) {
            active_period_history.push_back(std::make_pair(_time,0));
        } else {
            active_period_history.rbegin()->first = _time;
        }
    }
    
    void putActiveEnd(double _time) {
        active_period_history.rbegin()->second = _time;
    }
    
    std::vector<std::pair<DatumType,DatumType>> const& getService() const {
        return service_history;
    }
    
    std::vector<DatumType> const& getArrivals() const {
        return arrival_history;
    }
    
    std::vector<std::pair<double,double>> const& getActivePeriods() const {
        return active_period_history;
    }
    
private:
    
    std::vector<std::pair<DatumType,DatumType>> service_history;
    std::vector<DatumType> arrival_history;
    std::vector<std::pair<double,double>> active_period_history;
};

#endif /* Datum_h */
