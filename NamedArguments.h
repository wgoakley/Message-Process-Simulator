//
//  NamedArguments.h
//  NetworkSim
//
//  Created by William Oakley on 12/18/18.
//  Copyright © 2018 William Oakley. All rights reserved.
//

#ifndef NamedArguments_h
#define NamedArguments_h


template <typename Base, unsigned D>
class Discriminator : public Base {};

namespace namedexperiment {
    
    // todo: need to enter in defaults!!!
    class DefaultTemplateTypes {
    public:
        using nodeType = NodeType;
        using graphProcess = GraphProcess;
        using dynamicalProcess = DynamicalProcess;
        using recorderType = Recorder;
    };
    
    template <typename Type>
    class NodeTypeIs : virtual public DefaultTemplateTypes {
    public:
        using nodeType = Type;
    };
    
    // may want to change the name
    template <typename Type>
    class GraphProcessTypeIs : virtual public DefaultTemplateTypes {
    public:
        using graphProcess = Type;
    };
    
    template <typename Policy>
    class DynamicalProcessTypeIs : virtual public DefaultTemplateTypes {
    public:
        using dynamicalProcess = Policy;
    };
    
    template <typename Policy>
    class RecorderTypeIs : virtual public DefaultTemplateTypes {
    public:
        using recorderType = Policy;
    };
    
    class DefaultParams : virtual public DefaultTemplateTypes {};
    
    template <typename Setter0, typename Setter1, typename Setter2, typename Setter3>
    class PolicySelector : public Discriminator<Setter0,0>, public Discriminator<Setter1,1>, public Discriminator<Setter2,2>, public Discriminator<Setter3,3> {};
    
    
    
    
    // Node Constructor Stuff
    
    
    
    
    class DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<> const*;
        using arrivalPolicyParamsPointerType = std::tuple<> const*;
        using sendPolicyParamsPointerType = std::tuple<> const*;
        using servicePolicyParamsPointerType = std::tuple<> const*;
        using activeStartPolicyParamsPointerType = std::tuple<> const*;
        using activeEndPolicyParamsPointerType = std::tuple<> const*;
        using exogeneousArrivalParamsPointerType = std::tuple<> const*;
        
        virtual void const* getStorageParams() const {
            return nullptr;
        }
        virtual void const* getArrivalPolicyParams() const {
            return nullptr;
        }
        virtual void const* getSendPolicyParams() const {
            return nullptr;
        }
        virtual void const* getServicePolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveStartPolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveEndPolicyParams() const {
            return nullptr;
        }
        virtual void const* getExogeneousArrivalParams() const {
            return nullptr;
        }
    };
    
    template <typename... Args>
    class StorageParamsAre : public virtual DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<Args...> const*;
        StorageParamsAre(std::tuple<Args...> const& _storage_params) : storage_params(std::move(_storage_params)) {}
        void const* getStorageParams() const override { return &storage_params; }
    private:
        std::tuple<Args...> storage_params;
    };
    template <typename... Args>
    class ArrivalPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using arrivalPolicyParamsPointerType = std::tuple<Args...> const*;
        ArrivalPolicyParamsAre(std::tuple<Args...> const& _arrival_policy_params) : arrival_policy_params(std::move(_arrival_policy_params)) {}
        void const* getArrivalPolicyParams() const override { return &arrival_policy_params; }
    private:
        std::tuple<Args...> arrival_policy_params;
    };
    template <typename... Args>
    class SendPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using sendPolicyParamsPointerType = std::tuple<Args...> const*;
        SendPolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : send_policy_params(std::move(_send_policy_params)) {}
        void const* getSendPolicyParams() const override { return &send_policy_params; }
    private:
        std::tuple<Args...> send_policy_params;
    };
    template <typename... Args>
    class ServicePolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using servicePolicyParamsPointerType = std::tuple<Args...> const*;
        ServicePolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : service_policy_params(std::move(_service_policy_params)) {}
        void const* getServicePolicyParams() const override { return &service_policy_params; }
    private:
        std::tuple<Args...> service_policy_params;
    };
    template <typename... Args>
    class ActiveStartPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeStartPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveStartPolicyParamsAre(std::tuple<Args...> const& _active_start_policy_params) : active_start_policy_params(std::move(_active_start_policy_params)) {}
        void const* getActiveStartPolicyParams() const override { return &active_start_policy_params; }
    private:
        std::tuple<Args...> active_start_policy_params;
    };
    template <typename... Args>
    class ActiveEndPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeEndPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveEndPolicyParamsAre(std::tuple<Args...> const& _active_end_policy_params) : active_end_policy_params(std::move(_active_end_policy_params)) {}
        void const* getActiveEndPolicyParams() const override { return &active_end_policy_params; }
    private:
        std::tuple<Args...> active_end_policy_params;
    };
    template <typename... Args>
    class ExogeneousArrivalParamsAre : public virtual DefaultConstructorTypes {
    public:
        using exogeneousArrivalParamsPointerType = std::tuple<Args...> const*;
        ExogeneousArrivalParamsAre(std::tuple<Args...> const& _exogeneous_arrival_params) : exogeneous_arrival_params(std::move(_exogeneous_arrival_params)) {}
        void const* getExogeneousArrivalParams() const override { return &exogeneous_arrival_params; }
    private:
        std::tuple<Args...> exogeneous_arrival_params;
    };

}


namespace namednode {
    
    class DefaultTemplateTypes {
    public:
        using datumType = DefaultDatumType;
        using storageType = Feed<datumType>;
        using arrivalPolicy = NullArrival<datumType>;
        using sendPolicy = DefaultSender<datumType>;
        using servicePolicy = MarkovService<datumType>;
        using activeStartPolicy = MarkovStart<datumType>;
        using activeEndPolicy = ExhaustiveEnd<datumType>;
        using exogeneousArrivalType = NullExogeneousArrival<datumType>;
    };

    template <typename Type>
    class DatumTypeIs : virtual public DefaultTemplateTypes {
    public:
        using datumType = Type;
    };
    
    // may want to change the name
    template <typename Type>
    class StorageTypeIs : virtual public DefaultTemplateTypes {
    public:
        using storageType = Type;
    };
    
    template <typename Policy>
    class ArrivalPolicyIs : virtual public DefaultTemplateTypes {
    public:
        using arrivalPolicy = Policy;
    };
    
    template <typename Policy>
    class SendPolicyIs : virtual public DefaultTemplateTypes {
    public:
        using sendPolicy = Policy;
    };
    
    template <typename Policy>
    class ServicePolicyIs : virtual public DefaultTemplateTypes {
    public:
        using servicePolicy = Policy;
    };
    
    template <typename Policy>
    class ActiveStartPolicyIs : virtual public DefaultTemplateTypes {
    public:
        using activeStartPolicy = Policy;
    };
    
    template <typename Policy>
    class ActiveEndPolicyIs : virtual public DefaultTemplateTypes {
    public:
        using activeEndPolicy = Policy;
    };
    
    template <typename Type>
    class ExogeneousArrivalTypeIs : virtual public DefaultTemplateTypes {
    public:
        using exogeneousArrivalType = Type;
    };
    
    class DefaultParams : virtual public DefaultTemplateTypes {};
    
    template <typename Setter0, typename Setter1, typename Setter2, typename Setter3, typename Setter4, typename Setter5, typename Setter6, typename Setter7>
    class PolicySelector : public Discriminator<Setter0,0>, public Discriminator<Setter1,1>, public Discriminator<Setter2,2>, public Discriminator<Setter3,3>, public Discriminator<Setter4,4>, public Discriminator<Setter5,5>, public Discriminator<Setter6,6>, public Discriminator<Setter7,7> {};
    
    
    
    
    // Node Constructor Stuff
    
    
    template <typename Setter0 = namednode::DefaultParams, typename Setter1 = namednode::DefaultParams, typename Setter2 = namednode::DefaultParams, typename Setter3 = namednode::DefaultParams, typename Setter4 = namednode::DefaultParams, typename Setter5 = namednode::DefaultParams, typename Setter6 = namednode::DefaultParams, typename Setter7 = namednode::DefaultParams>
    class Node {
    private:
        
        using policies = namednode::PolicySelector<Setter0,Setter1,Setter2,Setter3,Setter4,Setter5,Setter6,Setter7>;
        using policies::datumType;
        using policies::storageType;
        using policies::arrivalPolicy;
        using policies::sendPolicy;
        using policies::servicePolicy;
        using policies::activeStartPolicy;
        using policies::activeEndPolicy;
        using policies::exogeneousArrivalType;
        
        template <typename... Params>
        class DefaultArgs : public virtual namednode::DefaultConstructorTypes {};
        
        template <typename ArgsSetter0, typename ArgsSetter1, typename ArgsSetter2, typename ArgsSetter3, typename ArgsSetter4, typename ArgsSetter5, typename ArgsSetter6>
        class ParamSelector final: public Discriminator<ArgsSetter0,0>, public Discriminator<ArgsSetter1,1>, public Discriminator<ArgsSetter2,2>, public Discriminator<ArgsSetter3,3>, public Discriminator<ArgsSetter4,4>, public Discriminator<ArgsSetter5,5>, public Discriminator<ArgsSetter6,6> {
            ParamSelector(ArgsSetter0&& _args_setter_0, ArgsSetter1&& _args_setter_1, ArgsSetter2&& _args_setter_2, ArgsSetter3&& _args_setter_3, ArgsSetter4&& _args_setter_4, ArgsSetter5&& _args_setter_5, ArgsSetter6&& _args_setter_6) : Discriminator<Setter0,0>(_args_setter_0), Discriminator<Setter1,1>(_args_setter_1), Discriminator<Setter2,2>(_args_setter_2), Discriminator<Setter3,3>(_args_setter_3), Discriminator<Setter4,4>(_args_setter_4), Discriminator<Setter5,5>(_args_setter_5), Discriminator<Setter6,6>(_args_setter_6) {}
        };
        
    public:
        
        // general constructor with named template arguments as you do
        template <typename ArgsSetter0 = DefaultArgs<>, typename ArgsSetter1 = DefaultArgs<>, typename ArgsSetter2 = DefaultArgs<>, typename ArgsSetter3 = DefaultArgs<>, typename ArgsSetter4 = DefaultArgs<>, typename ArgsSetter5 = DefaultArgs<>, typename ArgsSetter6 = DefaultArgs<>, typename = std::enable_if<!is_instance_of_v<ParamSelector,ArgsSetter0>&&!is_instance_of_v<std::tuple,ArgsSetter0>>>
        Node(ArgsSetter0&& _args_setter_0 = DefaultArgs<>(), ArgsSetter1&& _args_setter_1 = DefaultArgs<>(), ArgsSetter2&& _args_setter_2 = DefaultArgs<>(), ArgsSetter3&& _args_setter_3 = DefaultArgs<>(), ArgsSetter4&& _args_setter_4 = DefaultArgs<>(), ArgsSetter5&& _args_setter_5 = DefaultArgs<>(), ArgsSetter6&& _args_setter_6 = DefaultArgs<>()) : Node(ParamSelector<ArgsSetter0,ArgsSetter1,ArgsSetter2,ArgsSetter3,ArgsSetter4,ArgsSetter5,ArgsSetter6>(std::forward<ArgSetter0>(_args_setter_0),std::forward<ArgSetter1>(_args_setter_1),std::forward<ArgSetter2>(_args_setter_2),std::forward<ArgSetter3>(_args_setter_3),std::forward<ArgSetter4>(_args_setter_4),std::forward<ArgSetter5>(_args_setter_5),std::forward<ArgSetter6(_args_setter_6))) {}
        
    private:
        
        template <typename ParamSelectorType, typename = std::enable_if<is_instance_of_v<ParamSelector,ParamSelectorType>>>
        Node(ParamSelectorType&& _param_selector) : Node((_param_selector.getStorageParams()!=nullptr)?(*static_cast<ParamSelectorType::storageParamsPointerType>(_param_selector.getStorageParams())):std::make_tuple(),(_param_selector.getArrivalPolicyParams()!=nullptr)?(*static_cast<ParamSelectorType::arrivalPolicyParamsPointerType>(_param_selector.getArrivalPolicyParams())):std::make_tuple(),(_param_selector.getSendPolicyParams()!=nullptr)?(*static_cast<ParamSelectorType::sendPolicyParamsPointerType>(_param_selector.getSendPolicyParams())):std::make_tuple(),(_param_selector.getServicePolicyParams()!=nullptr)?(*static_cast<ParamSelectorType::servicePolicyParamsPointerType>(_param_selector.getServicePolicyParams())):std::make_tuple(),(_param_selector.getActiveStartPolicyParams()!=nullptr)?(*static_cast<ParamSelectorType::activeStartPolicyParamsPointerType>(_param_selector.getActiveStartPolicyParams())):std::make_tuple(),(_param_selector.getActiveEndPolicyParams()!=nullptr)?(*static_cast<ParamSelectorType::activeEndPolicyParamsPointerType>(_param_selector.getActiveEndPolicyParams())):std::make_tuple(),(_param_selector.getExogeneousArrivalParams()!=nullptr)?(*static_cast<ParamSelectorType::exogeneousArrivalPolicyPointerType>(_param_selector.getExogeneousArrivalParams())):std::make_tuple()) {}
        
        template <typename... StorageTupleParams, typename... ArrivalTupleParams, typename... SendTupleParams, typename... ServiceTupleParams, typename... ActiveStartTupleParams, typename... ActiveEndTupleParams, typename... ExogeneousArrivalTupleParams>
        Node(std::tuple<StorageTupleParams...>&& _storage_tuple, std::tuple<ArrivalTupleParams...>&& _arrival_tuple, std::tuple<SendTupleParams...>&& _send_tuple, std::tuple<ServiceTupleParams...>&& _service_tuple, std::tuple<ActiveStartTupleParams...>&& _active_start_tuple, std::tuple<ActiveEndTupleParams...>&& _active_end_tuple), std::tuple<ExogeneousArrivalTupleParams...>&& exogeneous_arrival_tuple) : Node(std::move(_storage_tuple),std::index_sequence_for<StorageTupleParams...>(),std::move(_arrival_tuple),std::index_sequence_for<ArrivalTupleParams...>(),std::move(_send_tuple,),std::index_sequence_for<SendTupleParams...>(),std::move(_service_tuple),std::index_sequence_for<ServiceTupleParams...>(),std::move(_active_start_tuple),std::index_sequence_for<ActiveStartTupleParams...>(),std::move(_active_end_tuple),std::index_sequence_for<ActiveEndTupleParams...>(),std::move(exogeneous_arrival_tuple),std::index_sequence_for<ExogeneousArrivalTupleParams...>()) {}
        
        template <typename... StorageTupleParams, typename... StorageTupleIndices, typename... ArrivalTupleParams, typename... ArrivalTupleIndices, typename... SendTupleParams, typename... SendTupleIndices, typename... ServiceTupleParams, typename... ServiceTupleIndices, typename... ActiveStartTupleParams, typename... ActiveStartTupleIndices, typename... ActiveEndTupleParams, typename... ActiveEndTupleIndices, typename... ExogeneousArrivalTupleParams, typename... ExogeneousArrivalTupleIndices>
        Node(std::tuple<StorageTupleParams...>&& _storage_tuple, std::index_sequence<StorageTupleIndices...>, std::tuple<ArrivalTupleParams...>&& _arrival_tuple, std::index_sequence<ArrivalTupleIndices...>, std::tuple<SendTupleParams...>&& _send_tuple, std::index_sequence<SendTupleIndices...>, std::tuple<ServiceTupleParams...>&& _service_tuple, std::index_sequence<ServiceTupleIndices...>, std::tuple<ActiveStartTupleParams...>&& _active_start_tuple, std::index_sequence<ActiveStartTupleIndices...>, std::tuple<ActiveEndTupleParams...>&& _active_end_tuple), std::index_sequence<ActiveEndTupleIndices...>, std::tuple<ExogeneousArrivalTupleParams...>&& _exogeneous_arrival_tuple, std::index_sequence<ExogeneousArrivalTupleIndices...>) : storage(std::get<StorageTupleIndices>(_storage_tuple)...),     arrival_policy(std::get<ArrivalTupleIndices>(_arrival_tuple)...), send_policy(std::get<SendTupleIndices...>(_send_tuple)...), service_policy(std::get<ServiceTupleIndices>(_service_tuple)...), active_start_policy(std::get<ActiveStartTupleParams>(_active_start_tuple)...), active_end_policy(std::get<ActiveEndTupleIndices>(_active_end_tuple)...), exogeneous_arrival(std::get<ExogeneousArrivalTupleIndices>(_exogeneous_arrival_tuple)...) {}
        
        storageType storage;
        arrivalPolicy arrival_policy;
        sendPolicy send_policy;
        servicePolicy service_policy;
        activeStartPolicy active_start_policy;
        activeEndPolicy active_end_policy;
        exogeneousArrivalType exogeneous_arrival;
    };

    
    class DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<> const*;
        using arrivalPolicyParamsPointerType = std::tuple<> const*;
        using sendPolicyParamsPointerType = std::tuple<> const*;
        using servicePolicyParamsPointerType = std::tuple<> const*;
        using activeStartPolicyParamsPointerType = std::tuple<> const*;
        using activeEndPolicyParamsPointerType = std::tuple<> const*;
        using exogeneousArrivalParamsPointerType = std::tuple<> const*;
        
        virtual void const* getStorageParams() const {
            return nullptr;
        }
        virtual void const* getArrivalPolicyParams() const {
            return nullptr;
        }
        virtual void const* getSendPolicyParams() const {
            return nullptr;
        }
        virtual void const* getServicePolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveStartPolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveEndPolicyParams() const {
            return nullptr;
        }
        virtual void const* getExogeneousArrivalParams() const {
            return nullptr;
        }
    };
    
    template <typename... Args>
    class StorageParamsAre : public virtual DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<Args...> const*;
        StorageParamsAre(std::tuple<Args...> const& _storage_params) : storage_params(std::move(_storage_params)) {}
        void const* getStorageParams() const override { return &storage_params; }
    private:
        std::tuple<Args...> storage_params;
    };
    template <typename... Args>
    class ArrivalPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using arrivalPolicyParamsPointerType = std::tuple<Args...> const*;
        ArrivalPolicyParamsAre(std::tuple<Args...> const& _arrival_policy_params) : arrival_policy_params(std::move(_arrival_policy_params)) {}
        void const* getArrivalPolicyParams() const override { return &arrival_policy_params; }
    private:
        std::tuple<Args...> arrival_policy_params;
    };
    template <typename... Args>
    class SendPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using sendPolicyParamsPointerType = std::tuple<Args...> const*;
        SendPolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : send_policy_params(std::move(_send_policy_params)) {}
        void const* getSendPolicyParams() const override { return &send_policy_params; }
    private:
        std::tuple<Args...> send_policy_params;
    };
    template <typename... Args>
    class ServicePolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using servicePolicyParamsPointerType = std::tuple<Args...> const*;
        ServicePolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : service_policy_params(std::move(_service_policy_params)) {}
        void const* getServicePolicyParams() const override { return &service_policy_params; }
    private:
        std::tuple<Args...> service_policy_params;
    };
    template <typename... Args>
    class ActiveStartPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeStartPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveStartPolicyParamsAre(std::tuple<Args...> const& _active_start_policy_params) : active_start_policy_params(std::move(_active_start_policy_params)) {}
        void const* getActiveStartPolicyParams() const override { return &active_start_policy_params; }
    private:
        std::tuple<Args...> active_start_policy_params;
    };
    template <typename... Args>
    class ActiveEndPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeEndPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveEndPolicyParamsAre(std::tuple<Args...> const& _active_end_policy_params) : active_end_policy_params(std::move(_active_end_policy_params)) {}
        void const* getActiveEndPolicyParams() const override { return &active_end_policy_params; }
    private:
        std::tuple<Args...> active_end_policy_params;
    };
    template <typename... Args>
    class ExogeneousArrivalParamsAre : public virtual DefaultConstructorTypes {
    public:
        using exogeneousArrivalParamsPointerType = std::tuple<Args...> const*;
        ExogeneousArrivalParamsAre(std::tuple<Args...> const& _exogeneous_arrival_params) : exogeneous_arrival_params(std::move(_exogeneous_arrival_params)) {}
        void const* getExogeneousArrivalParams() const override { return &exogeneous_arrival_params; }
    private:
        std::tuple<Args...> exogeneous_arrival_params;
    };
}






namespace namedupdater {
    
    class DefaultTemplateTypes {
        using nodeType = DefaultDatumType;
        using nodeGeneratorType = Feed<datumType>;
        using graphProcessType = NullArrival<datumType>;
    };
    
    template <typename Type>
    class NodeTypeIs : virtual public DefaultTemplateTypes {
    public:
        using nodeType = Type;
    };
    
    // may want to change the name
    template <typename Type>
    class NodeGeneratorTypeIs : virtual public DefaultTemplateTypes {
    public:
        using nodeGeneratorType = Type;
    };
    
    template <typename Type>
    class GraphProcessTypeIs : virtual public DefaultTemplateTypes {
    public:
        using graphProcessType = Type;
    };
    
    class DefaultParams : virtual public DefaultTemplateTypes {};
    
    template <typename Setter0, typename Setter1, typename Setter2>
    class SetterSelector : public Discriminator<Setter0,0>, public Discriminator<Setter1,1>, public Discriminator<Setter2,2> {};
    
    // Node Constructor Stuff
    
    
    
    
    class DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<> const*;
        using arrivalPolicyParamsPointerType = std::tuple<> const*;
        using sendPolicyParamsPointerType = std::tuple<> const*;
        using servicePolicyParamsPointerType = std::tuple<> const*;
        using activeStartPolicyParamsPointerType = std::tuple<> const*;
        using activeEndPolicyParamsPointerType = std::tuple<> const*;
        using exogeneousArrivalParamsPointerType = std::tuple<> const*;
        
        virtual void const* getStorageParams() const {
            return nullptr;
        }
        virtual void const* getArrivalPolicyParams() const {
            return nullptr;
        }
        virtual void const* getSendPolicyParams() const {
            return nullptr;
        }
        virtual void const* getServicePolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveStartPolicyParams() const {
            return nullptr;
        }
        virtual void const* getActiveEndPolicyParams() const {
            return nullptr;
        }
        virtual void const* getExogeneousArrivalParams() const {
            return nullptr;
        }
    };
    
    template <typename... Args>
    class StorageParamsAre : public virtual DefaultConstructorTypes {
    public:
        using storageParamsPointerType = std::tuple<Args...> const*;
        StorageParamsAre(std::tuple<Args...> const& _storage_params) : storage_params(std::move(_storage_params)) {}
        void const* getStorageParams() const override { return &storage_params; }
    private:
        std::tuple<Args...> storage_params;
    };
    template <typename... Args>
    class ArrivalPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using arrivalPolicyParamsPointerType = std::tuple<Args...> const*;
        ArrivalPolicyParamsAre(std::tuple<Args...> const& _arrival_policy_params) : arrival_policy_params(std::move(_arrival_policy_params)) {}
        void const* getArrivalPolicyParams() const override { return &arrival_policy_params; }
    private:
        std::tuple<Args...> arrival_policy_params;
    };
    template <typename... Args>
    class SendPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using sendPolicyParamsPointerType = std::tuple<Args...> const*;
        SendPolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : send_policy_params(std::move(_send_policy_params)) {}
        void const* getSendPolicyParams() const override { return &send_policy_params; }
    private:
        std::tuple<Args...> send_policy_params;
    };
    template <typename... Args>
    class ServicePolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using servicePolicyParamsPointerType = std::tuple<Args...> const*;
        ServicePolicyParamsAre(std::tuple<Args...> const& _send_policy_params) : service_policy_params(std::move(_service_policy_params)) {}
        void const* getServicePolicyParams() const override { return &service_policy_params; }
    private:
        std::tuple<Args...> service_policy_params;
    };
    template <typename... Args>
    class ActiveStartPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeStartPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveStartPolicyParamsAre(std::tuple<Args...> const& _active_start_policy_params) : active_start_policy_params(std::move(_active_start_policy_params)) {}
        void const* getActiveStartPolicyParams() const override { return &active_start_policy_params; }
    private:
        std::tuple<Args...> active_start_policy_params;
    };
    template <typename... Args>
    class ActiveEndPolicyParamsAre : public virtual DefaultConstructorTypes {
    public:
        using activeEndPolicyParamsPointerType = std::tuple<Args...> const*;
        ActiveEndPolicyParamsAre(std::tuple<Args...> const& _active_end_policy_params) : active_end_policy_params(std::move(_active_end_policy_params)) {}
        void const* getActiveEndPolicyParams() const override { return &active_end_policy_params; }
    private:
        std::tuple<Args...> active_end_policy_params;
    };
    template <typename... Args>
    class ExogeneousArrivalParamsAre : public virtual DefaultConstructorTypes {
    public:
        using exogeneousArrivalParamsPointerType = std::tuple<Args...> const*;
        ExogeneousArrivalParamsAre(std::tuple<Args...> const& _exogeneous_arrival_params) : exogeneous_arrival_params(std::move(_exogeneous_arrival_params)) {}
        void const* getExogeneousArrivalParams() const override { return &exogeneous_arrival_params; }
    private:
        std::tuple<Args...> exogeneous_arrival_params;
    };
}



#endif /* NamedArguments_h */
