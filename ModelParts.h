//
//  ModelParts.h
//  NetworkSim
//
//  Created by William Oakley on 1/9/19.
//  Copyright © 2019 William Oakley. All rights reserved.
//

#ifndef ModelParts_h
#define ModelParts_h

// shouldn't have history as a variable but i want the class construction to be the same for all arrival policies...
template <typename CRTP, typename HistoryType, typename StorageType>
class ArrivalPolicy {
public:
    
    typedef typename HistoryType::datumType datumType;
    
    ArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
    
    bool operator()() {
        return static_cast<CRTP const&>(*this)();
    }
    
protected:
    
    HistoryType const& history;
    StorageType const& storage;
    
};

// inherit HistoryInterpreter as well!
template <typename HistoryType, typename StorageType>
class NullArrivalPolicy : public ArrivalPolicy<NullArrivalPolicy<HistoryType,StorageType>,HistoryType,StorageType> {
public:
    typedef ArrivalPolicy<NullArrivalPolicy<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    NullArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
    
    bool operator()() {
        return true;
    }
    
};

template <typename HistoryType, typename StorageType, typename DatumEqualityType>
class NoneArrivalPolicy : public ArrivalPolicy<NoneArrivalPolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType>  {
public:
    typedef NoneArrivalPolicy<NoneArrivalPolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    NoneArrivalPolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
    
    bool operator()() {
        eq.init(parent::storage.get());
        return std::count_if(parent::history.getArrivals().begin(),parent::history.getArrivals().end(),eq)==0;
    }
    
private:
    DatumEqualityType eq;
};

template <typename HistoryType, typename StorageType, typename DatumEqualityType>
class NoneServicePolicy : public NoneServicePolicy<NoneServicePolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType>  {
public:
    typedef NoneServicePolicy<NoneServicePolicy<HistoryType,StorageType,DatumEqualityType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    NoneServicePolicy(HistoryType const& _history, StorageType const& _storage) : parent(_history,_storage) {}
    
    bool operator()() {
        eq.init(parent::storage.get());
        return std::count_if(parent::history.getService().begin(),parent::history.getService().end(),eq)==0;
    }
    
private:
    DatumEqualityType eq;
};

// Sender & creator template: class to decide whether to send a message or to create a message
template <typename CRTP, typename HistoryType, typename StorageType>
class Sender {
public:
    
    typedef typename HistoryType::datumType datumType;
    
    Sender(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage){}
    
    size_t operator()(double _current_time) {
        return static_cast<CRTP const&>(*this)(_current_time);
    }
    
protected:
    
    HistoryType const& history;
    StorageType const& storage;
    
private:
    
};

class DiscreteThreeDistribution {
public:
    DiscreteThreeDistribution(double _p_resend, double _p_create) : p_resend(_p_resend), p_create(_p_create), dis({1-_p_resend-_p_create,_p_resend,_p_create}) {}
    
    unsigned operator()() {
        switch (dis(gen)) {
            case 0 : return 0; break;
            case 1 : return 1; break;
            case 2 : return 2; break;
        }
        return 0; // stupid
    }
    
private:
    
    static std::mt19937 gen;
    std::discrete_distribution<unsigned> dis;
    double p_resend;
    double p_create;
};

std::mt19937 DiscreteThreeDistribution::gen((std::random_device())());

template <typename HistoryType, typename StorageType>
class DefaultSender : public Sender<DefaultSender<HistoryType,StorageType>,HistoryType,StorageType> {
public:
    
    typedef Sender<DefaultSender<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    DefaultSender(HistoryType const& _history, StorageType const& _storage, double _p_resend, double _p_create) : parent(_history,_storage), send_process(_p_resend, _p_create) {}
    
    size_t operator()(double _current_time) {
        auto result = send_process();
        if (result == 0) {
            return 0;
        } else if (result == 1) {
            return 1;
        } else { // == 2
            return 2;
        }
    }
    
private:
    
    DiscreteThreeDistribution send_process;
};


// should generate the next service time
// should be able to use the history...
template <typename CRTP, typename HistoryType, typename StorageType>
class Service {
public:
    
    typedef typename HistoryType::datumType datumType;
    
    template <typename... Args>
    Service(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
    
    double operator()(double _current_time) const {
        return static_cast<CRTP const&>(*this)(_current_time);
    }
    
protected:
    
    HistoryType const& history;
    StorageType const& storage;
};

//////////

// ServiceTimesProcess should be a Markov type process... Concept?
// Going to need ANOTHER class in Node to decide how to load up some of these functions... such as nextService... with data from the process.  Just one piece of data or multiple?  Save for another day!
template <typename HistoryType, typename StorageType>
class MarkovService : public Service<MarkovService<HistoryType,StorageType>,HistoryType,StorageType>
{
public:
    
    typedef Service<MarkovService<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    MarkovService(HistoryType const& _history, StorageType const& _storage, double _r) : parent(_history,_storage), r(_r), dis(_r) {}
    
    double operator()(double _current_time) {
        return _current_time + dis(gen);
    }
    
private:
    
    double r;
    std::exponential_distribution<double> dis;
    static std::mt19937 gen;
};

template <typename HistoryType, typename StorageType>
std::mt19937 MarkovService<HistoryType,StorageType>::gen((std::random_device())());


// decides when next active period begins, decides when active period ends
template <typename CRTP, typename HistoryType, typename StorageType>
class ActiveStart {
public:
    
    typedef typename HistoryType::datumType datumType;
    
    ActiveStart(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
    
    double operator()(double _current_time) {
        return static_cast<CRTP const&>(*this)(_current_time);
    }
    
protected:
    
    HistoryType const& history;
    StorageType const& storage;
};

template <typename HistoryType, typename StorageType>
class MarkovActiveStart : public ActiveStart<MarkovActiveStart<HistoryType,StorageType>,HistoryType,StorageType>
{
public:
    
    typedef ActiveStart<MarkovActiveStart<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    MarkovActiveStart(HistoryType const& _history, StorageType const& _storage, double _r) : parent(_history,_storage), r(_r), dis(_r) {}
    
    double operator()(double _current_time) {
        return _current_time + dis(gen);
    }
    
private:
    
    double r;
    std::exponential_distribution<double> dis;
    static std::mt19937 gen;
};

template <typename HistoryType, typename StorageType>
std::mt19937 MarkovActiveStart<HistoryType,StorageType>::gen((std::random_device())());


// decides when next active period begins, decides when active period ends
template <typename CRTP, typename HistoryType, typename StorageType>
class ActiveEnd {
public:
    
    typedef typename HistoryType::datumType datumType;
    
    ActiveEnd(HistoryType const& _history, StorageType const& _storage) : history(_history), storage(_storage) {}
    
    bool operator()(double _current_time) {
        return static_cast<CRTP const&>(*this)(_current_time);
    }
    
protected:
    
    HistoryType const& history;
    StorageType const& storage;
};

// decides when next active period begins, decides when active period ends
template <typename HistoryType, typename StorageType>
class ActiveEndExhaustive : public ActiveEnd<ActiveEndExhaustive<HistoryType,StorageType>,HistoryType,StorageType> {
public:
    
    typedef ActiveEnd<ActiveEndExhaustive<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    ActiveEndExhaustive(HistoryType const& _history, StorageType const& _storage) : parent(_history, _storage) {}
    
    bool operator()(double _current_time) {
        return parent::storage.size()==0;
    }
};

template <typename HistoryType, typename StorageType>
class ActiveEndBernoulli : public ActiveEnd<ActiveEndBernoulli<HistoryType,StorageType>,HistoryType,StorageType> {
public:
    
    typedef ActiveEnd<ActiveEndBernoulli<HistoryType,StorageType>,HistoryType,StorageType> parent;
    typedef typename parent::datumType datumType;
    
    ActiveEndBernoulli(HistoryType const& _history, StorageType const& _storage, double _p) : parent(_history, _storage), p(_p), dis(_p)  {}
    
    bool operator()(double _current_time) {
        return parent::storage.size()==0 || dis(gen)==0;
    }
    
private:
    
    double p;
    std::bernoulli_distribution dis;
    static std::mt19937 gen;
};

template <typename HistoryType,typename StorageType>
std::mt19937 ActiveEndBernoulli<HistoryType,StorageType>::gen((std::random_device())());

template <typename CRTP, typename NodeType>
class ExogeneousArrival {
public:
    
    typedef typename NodeType::datumType datumType;
    
    double operator()(std::vector<NodeType>& _nodes) {
        return static_cast<CRTP const&>(*this)(_nodes);
    }
    
};

template <typename NodeType>
class ExponentialExogeneousArrival : public ExogeneousArrival<ExponentialExogeneousArrival<NodeType>,NodeType> {
public:
    
    typedef ExogeneousArrival<ExponentialExogeneousArrival<NodeType>,NodeType> parent;
    typedef typename parent::datumType datumType;
    
    ExponentialExogeneousArrival(double _r, double _T, double _p) : r(_r), T(_T), exp_dis(r), p(_p), bern_dis(p) {}
    
    void operator()(std::vector<NodeType>& _nodes) {
        for (auto it = _nodes.begin(); it != _nodes.end() ; ++it) {
            if (bern_dis(gen)) {
                it->pushArrival(datumType(NodeType::getMsgId(),0,exp_dis(gen)));
            }
        }
    }
    
protected:
    double r;
    double T;
    double p;
    std::exponential_distribution<double> exp_dis;
    std::bernoulli_distribution bern_dis;
    static std::mt19937 gen;
};

template <typename NodeType>
std::mt19937 ExponentialExogeneousArrival<NodeType>::gen((std::random_device())());

#endif /* ModelParts_h */
