#include <vector>
#include <tuple>
#include <random>
#include "Updaters.h"
#include "NodeGenerators.h"
#include "StochasticProcesses.h"
#include "Distributions.h"
#include "Misc.h"
#include "GraphProcesses.h"
#include "Experiments.h"
#include <limits>
#include "Storage.h"
#include "ModelParts.h"
#include "DataStructures.h"

int main() {
    
    size_t nodes = 1000;
   
    using DatumType = DefaultDatumType;
    using HistoryType = History<DatumType>;
    using StorageType = Feed<DatumType>;
    using ArrivalPolicyType = NullArrivalPolicy<HistoryType,StorageType>;
    using SendPolicyType = DefaultSender<HistoryType,StorageType>;
    using ServicePolicyType = MarkovService<HistoryType,StorageType>;
    using ActiveStartPolicyType = MarkovActiveStart<HistoryType,StorageType>;
    using ActiveEndPolicyType = ActiveEndExhaustive<HistoryType,StorageType>;
    
    using NodeType = Node<DatumType, StorageType, HistoryType, ArrivalPolicyType, SendPolicyType, ServicePolicyType, ActiveStartPolicyType, ActiveEndPolicyType>;
    
    using ExogeneousArrivalType = ExponentialExogeneousArrival<NodeType>;

    using StorageParamGen = ParamGenerator<ConstantDistribution<size_t>>;
    using ArrivalPolicyParamGen = ParamGenerator<>;
    using SendPolicyParamGen = ParamGenerator<ConstantDistribution<double>,ConstantDistribution<double>>;
    using ServicePolicyParamGen = ParamGenerator<ConstantDistribution<double>>;
    using ActiveStartPolicyParamGen = ParamGenerator<ConstantDistribution<double>>;
    using ActiveEndPolicyParamGen = ParamGenerator<>;
    using ExogeneousArrivalParamGen = ParamGenerator<ConstantDistribution<double>,ConstantDistribution<double>,ConstantDistribution<double>>;
    
    using NodeGeneratorType = NodeGenerator<Node,DatumType, StorageType, HistoryType, ArrivalPolicyType, SendPolicyType, ServicePolicyType, ActiveStartPolicyType, ActiveEndPolicyType, ExogeneousArrivalType, StorageParamGen, ArrivalPolicyParamGen, SendPolicyParamGen, ServicePolicyParamGen, ActiveStartPolicyParamGen, ActiveEndPolicyParamGen, ExogeneousArrivalParamGen>;

    NodeGeneratorType node_generator(nodes,StorageParamGen(std::numeric_limits<size_t>::max()), ArrivalPolicyParamGen(), SendPolicyParamGen(.05,.05), ServicePolicyParamGen(10), ActiveStartPolicyParamGen(.1), ActiveEndPolicyParamGen(), ExogeneousArrivalParamGen(.01,100,.1));
    
    using GraphProcessType = graphProcess::ConstDirectedERGraph;
    
    GraphProcessType graph_process(nodes,.1);
    
    MessageUpdater<NodeGeneratorType,GraphProcessType> updater(std::move(node_generator),std::move(graph_process));
    
    size_t trials = 1;
    double T = 1000;
    
    Experiment experiment(updater,trials,T);
    experiment();

}
//
//int main() {
//
////    constexpr long N_MIN = 10;
////    constexpr long N_MAX = 10;
////    constexpr long n_MAX = 10000;
//    // constexpr long k_MAX = (N_MAX < n_MAX)?N_MAX:n_MAX;
//
//    newCoupon::initialize();
//    complexCoupon::initialize();
//
//    const size_t nodes = 1000;
////    const double p = .001;
//    const double T = 1440;
//    const double S = 60;
//    const double arrival_param = 10./nodes;
//    const double service_param = 6;
//  const double login_param = 1/60.;
//  const double p_const = .04;
////    const double p_low = .03;
////    const double p_high = .03;
////    const double p_high_up = .9;
////    const double p_high_down = .05;
////    const double p_low_up = .05;
////    const double p_low_down = 0;
////    const size_t N = 10;
////    const size_t k = 10;
////    const double p = 10./nodes;
////    const size_t queue_size = std::numeric_limits<size_t>::max();
//  const size_t queue_size = 10;
//  const double init_logged_prob = 1;
//    auto arrival_params = std::make_tuple(std::make_tuple(arrival_param));
//    auto service_params = std::make_tuple(std::make_tuple(service_param));
//    auto login_params = std::make_tuple(std::make_tuple(login_param));
////    auto send_params = std::make_tuple(std::make_tuple(p_low_down,p_low_up),std::make_tuple(p_high_down,p_high_up),std::make_tuple(N));
////    auto send_params = std::make_tuple(std::make_tuple(p_low),std::make_tuple(p_high),std::make_tuple(N));
//    auto send_params = std::make_tuple(std::make_tuple(p_const));
//    auto size_params = std::make_tuple(std::make_tuple(queue_size));
//    auto logged_params = std::make_tuple(std::make_tuple(init_logged_prob));
////    auto graph_params = std::make_tuple(nodes,k);
//
//    size_t trials = 1;
//
//    /*
//        reciprocated power law graph generator stuff
////     **/
//    double in_exp = 2;
//    double out_exp = 2;
//    double recip_exp = 1.65;
//    std::tuple<double,size_t> in_params = std::make_tuple(in_exp,nodes);
//    std::tuple<double,size_t> out_params = std::make_tuple(out_exp,nodes);
//    std::tuple<double,size_t> recip_params = std::make_tuple(recip_exp,nodes);
//    auto graph_params = std::make_tuple(nodes,in_params,out_params,recip_params);
//
//    /*
//     power law graph generator stuff
//     **/
////    double in_exp = 1.5;
////    double out_exp = 1.5;
////    std::tuple<double,size_t> in_params = std::make_tuple(in_exp,nodes);
////    std::tuple<double,size_t> out_params = std::make_tuple(out_exp,nodes);
////    auto graph_params = std::make_tuple(nodes,in_params,out_params);
//
//    // process type alias declarations
//    using ArrivalProcess = HomogeneousPoissonProcess;
//    using ServiceProcess = HomogeneousPoissonProcess;
//    using LoginProcess = HomogeneousPoissonProcess;
//    using SendProcess = BernoulliMessageProcess;
//    using SizeProcess = ConstantDistribution<size_t>;
//    using LoggedProcess = std::bernoulli_distribution;
//    using CompareType = MessageChainTopicCompare;
//
//    // node type alias declarations
//    using NodeType = Node<ArrivalProcess, ServiceProcess, LoginProcess, SendProcess, SizeProcess, LoggedProcess, CompareType>;
//
//    // param generator type alias declarations
//    using ArrivalParamGenerator = ParamGenerator<ConstantDistribution<double>>;
//    using ServiceParamGenerator = ParamGenerator<ConstantDistribution<double>>;
//    using LoginParamGenerator = ParamGenerator<ConstantDistribution<double>>;
//    using SendParamGenerator = ParamGenerator<ConstantDistribution<double>>;
//    using SizeParamGenerator = ParamGenerator<ConstantDistribution<size_t>>;
//    using LoggedParamGenerator = ParamGenerator<ConstantDistribution<size_t>>;
//
//    // node generator type alias declarations
//    using NodeGeneratorType = NodeGenerator<NodeType,ArrivalParamGenerator,ServiceParamGenerator,LoginParamGenerator,SendParamGenerator,SizeParamGenerator,LoggedParamGenerator>;
////    using GraphType = graphProcess::ConstDirectedReciprocalChungLu<PowerLawDistribution,PowerLawDistribution,PowerLawDistribution>;
////    using GraphType = graphProcess::ConstDirectedChungLu<PowerLawDistribution,PowerLawDistribution>;
//
//    // graph type alias declaration
////    using GraphType = graphProcess::ConstDirectedChungLu<PowerLawDistribution, PowerLawDistribution>;
//    using GraphType = graphProcess::ConstDirectedReciprocalChungLu<PowerLawDistribution, PowerLawDistribution, PowerLawDistribution>;
////    using GraphType = graphProcess::ConstRegularGraph;
//
//    // updater type alias declaration
//    using UpdaterType = MessageUpdater<NodeType,NodeGeneratorType,GraphType,PairCompare>;
//
//    // construction of the updater
//    UpdaterType message_updater(nodes, S, arrival_params, service_params, login_params, send_params, size_params, logged_params, graph_params);
//
////    Experiment<NodeType,UpdaterType> experiment(message_updater,trials,T,"msgs-1.m","logins-1.m","arrivals-1.m");
//    // construction of the experiment
//    Experiment<NodeType,UpdaterType> experiment(message_updater,trials,T,"ex2-pl1-msgs.m","ex2-pl1-logins.m","ex2-pl1-arrivals.m");
//
//    experiment();
//}
