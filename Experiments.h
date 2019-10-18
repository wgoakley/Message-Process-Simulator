#ifndef ___Experiments___
#define ___Experiments___
#include "DataStructures.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>

template <typename UpdaterType>
class Experiment {
    
public:
    
    using nodeType = typename UpdaterType::nodeType;

    Experiment(UpdaterType _updater, size_t _trials, double _T, std::string const& _filename_messages = "msgs.m", std::string const& _filename_logins = "logins.m", std::string const& _filename_arrivals = "arrivals.m") : updater(_updater), T(_T), trials(_trials), fout_service(_filename_messages), fout_active(_filename_logins), fout_arrivals(_filename_arrivals) {}
    
    void operator()() {
        fout_service << info() << "\n";
        fout_active << info() << "\n";
        fout_arrivals << info() << "\n";
        fout_service << updater.info() << "\n";
        fout_active << updater.info() << "\n";
        fout_arrivals << updater.info() << "\n";
        for (size_t i = 0 ; i < trials ; ++i) {
            std::cout << "trial " << i+1 << std::endl;
            // need for loop to run at least twice.
            auto data = trial();
            generate_data(data);
            updater.reset();
        }
    }
    
private:
    
    std::string info(){
        std::stringstream ss;
        ss << "TRIALS:" << trials;
        return ss.str();
    }
    
    const auto& trial() {
        size_t j = 0;
        double current_time = updater();
        std::cout << "iteration " << ++j << std::endl;
        while (current_time < T) {
            current_time = updater();
            //            std::cout << current_time << std::endl;
            if ((++j)%10000 == 0) {
                std::cout << current_time << std::endl;
                std::cout << "iteration: " << j << std::endl;
            }
        }
        return updater.getNodes();
    }
    
    /**
     *  @brief the kind of measurement we want to take of the process
     */
    
    void generate_data(const std::vector<nodeType>& v) {
        fout_service << '{';
        for (size_t i = 0, sz = v.size() ; i < sz ; ++i) {
            const nodeType& node = v[i];
            auto const& history = node.getHistory();
            auto const& service_history = history.getService();
            auto const& arrival_history = history.getArrivals();
            auto const& active_period_history = history.getActivePeriods();
            fout_service << '{';
            for (const auto& datum : service_history) {
                fout_service << '{' << std::get<0>(datum) << ',' << std::get<1>(datum) << '}' << ',';
            }
            if (!service_history.empty()) {
                fout_service.seekp(static_cast<unsigned long long>(fout_service.tellp())-1);
            }
            fout_service << '}' << ',';
            fout_arrivals << '{';
            for (const auto& datum : arrival_history) {
                fout_arrivals << datum << ',';
            }
            if (!arrival_history.empty()) {
                fout_arrivals.seekp(static_cast<unsigned long long>(fout_arrivals.tellp())-1);
            }
            fout_arrivals << '}' << ',';
            fout_active << '{';
            for (const auto& range : active_period_history) {
                fout_active << '{' << std::get<0>(range) << ',' << std::get<1>(range) << '}' << ',';
            }
            if (!active_period_history.empty()) {
                fout_active.seekp(static_cast<unsigned long long>(fout_active.tellp())-1);
            }
            fout_active << '}' << ',';
        }
        fout_service.seekp(static_cast<unsigned long long>(fout_service.tellp())-1);
        fout_service << '}';
        fout_arrivals.seekp(static_cast<unsigned long long>(fout_arrivals.tellp())-1);
        fout_arrivals << '}';
        fout_active.seekp(static_cast<unsigned long long>(fout_active.tellp())-1);
        fout_active << '}';
    }
    
    std::ofstream fout_service; /// filestream for outputting data
    std::ofstream fout_active;
    std::ofstream fout_arrivals;
    size_t trials; /// the number of disinct trials
    double T;
    UpdaterType updater;
};


//
//class Recorder {
//public:
//    Recorder(std::string _filename_)
//private:
//
//};
//
//template <typename NodesType, typename GraphProcess, typename DynamicalProcess>
//class Experiment {
//public:
//
//    // need to go back and allow for named parameters!
//    Experiment(size_t _trials, NodesType&& _nodes, GraphProcess&& _graph_process, DynamicalProcess&& _dynamical_process, Recorder&& _recorder) : trials(_trials), nodes(std::forward<std::vector<nodeType>>(_nodes)), graph_process(std::forward<DynamicalProcess>(_graph_process)), dynamical_process(std::forward<DynamicalProcess>(_dynamical_process)), Recorder(std::forward<Recorder>(_recorder)) {}
//
//    void runSimulation() {
//
//        fout_service << info() << "\n";
//        fout_logins << info() << "\n";
//        fout_arrivals << info() << "\n";
//        fout_messages << updater.info() << "\n";
//        fout_logins << updater.info() << "\n";
//        fout_arrivals << updater.info() << "\n";
//        for (size_t i = 0 ; i < trials ; ++i) {
//            std::cout << "trial " << i+1 << std::endl;
//            // need for loop to run at least twice.
//            const std::vector<NodeType>& data = trial();
//            generate_data(data);
//            updater.reset();
//        }
//
//    }
//
//private:
//
//    std::string basicInfo(){
//        std::stringstream ss;
//        ss << "TRIALS:" << trials;
//        return ss.str();
//    }
//
//    void runTrial() {
//        return ...
//    }
//
//    size_t trials;
//    NodesType nodes;
//    GraphProcess graph_process;
//    DynamicalProcess dynamical_process;
//    Recorder recorder;
//};

/**
 *  @brief this class represents an experiment on a temporal process
 *  This class runs an experiment on a temporal process, where
 *  an experiment is defined by taking certain measurements of
 *  the temporal process.
 */

#endif
