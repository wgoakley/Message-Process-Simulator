#ifndef ___Updaters___
#define ___Updaters___
#include <queue>
#include <typeinfo>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include "Graph.h"
#include "NodeGenerators.h"
#include "DataStructures.h"
#include "Misc.h"

// template <typename NodeGenerator, typename GraphProcess>
// class ConstGraphUpdater {
// public:
// 	template <typename S, typename T, typename U, typename V, typename W, typename X>
// 	ConstGraphUpdater(const size_t _sz, const S& _arrival_distribution_param, const T& _service_distribution_param,
// 		              const U& _login_distribution_param, const V& _send_distribution_param,
// 		              const W& _size_distribution_param, const X& _graph_process_param)
// 	 : sz(_sz),
// 	   node_gen(_sz,_arrival_distribution_param,_service_distribution_param,_login_distribution_param, _send_distribution_param, _size_distribution_param),
// 	   nodes(node_gen()),
// 	   graph_process(std::move(std::make_from_tuple<GraphProcess,X>(_graph_process_param))) {}

// 	virtual std::pair<const std::vector<Node>&,const double> operator()() = 0;

// 	void reset() {
// 		update_times.clear();
// 		nodes.clear();
// 	}

// 	virtual std::string info() const = 0;

// protected:

// 	virtual void node_initializer(const size_t) = 0;
// 	virtual void update_times_initializer() = 0;
// 	virtual void update(const size_t, const size_t) = 0;
// 	// virtual void node_data_update(const size_t i) = 0;

// 	size_t sz;
// 	NodeGenerator node_gen;
// 	GraphProcess graph_process;
// 	std::priority_queue<std::pair<double,size_t>,std::vector<std::pair<double,size_t>>,PairCompare<double,size_t>> update_times;  // queue of pairs (update time, node_id to update)
// 	std::vector<Node> nodes; // vector of Node objects
// };


// at highest level just tells the nodes that they need to execute an update step.
//template <typename NodeType>
//class NodeProcess {
//public:
//
//    NodeProcess(std::vector<NodeType>& _nodes, double _T) : nodes(_nodes) : T(_T) {
//        initialize();
//    }
//
//private:
//
//    std::vector<NodeType>& nodes;
//    double T;
//
//    void initialize() {
//        exogeneousArrivalsInitializer();
//        updateTimesInitializer();
//    }
//
//    void exogeneousArrivalsInitializer() {
//        double current_time = 0;
//        for (size_t i = 0 ; i < sz ; ++i) {
//            current_time = 0;
//            while (current_time < T) {
//                current_time = nodes[i].genArrival(current_time,sz,arrival_init_time,topic);
//            }
//        }
//    }
//    void updateTimesInitializer() {
//
//    }
//};


template <typename NodeGeneratorType, typename GraphProcessType>
class MessageUpdater {
public:
    
    using nodeType = typename NodeGeneratorType::nodeType;
    using datumType = typename nodeType::datumType;

    MessageUpdater(NodeGeneratorType&& _node_generator, GraphProcessType&& _graph_process) : node_generator(std::forward<NodeGeneratorType>(_node_generator)), graph_process(std::forward<GraphProcessType>(_graph_process))
	{
        nodes = node_generator.generate();
		updateTimesInitializer();
	}

    double operator()(){
        static bool print_graph_info = true;
		auto [current_time, node_id] = update_times.top();
        update_times.pop();
        Graph const& graph = graph_process(current_time);
		auto [new_update_time,msg_id] = nodes[node_id].update(current_time);
		if (msg_id > 0) {
			auto [neighbor_iter,neighbor_num] = graph.range(node_id);
			for (size_t i = 0 ; i < neighbor_num ; ++neighbor_iter, ++i) {
				update(neighbor_iter->second,datumType(msg_id,node_id,current_time));
			}
		}
		update_times.push(std::make_pair(new_update_time,node_id));
        if (print_graph_info) {
            print_graph_info = false;
            std::cout << graph.info() << std::endl;
        }
        std::tie(current_time, node_id) = update_times.top();
        return current_time;
	}
 
	std::string info() const {
		std::stringstream ss;
		ss << "Updater=MessageUpdater;";
		return ss.str();
	}
    
    std::vector<nodeType>& getNodes() {
        return nodes;
    }
    
    void reset() {
        while (!update_times.empty()) {
            update_times.pop();
        }
        nodes = node_generator.generate();
        updateTimesInitializer();
    }

//    void reset() {
//        while (!update_times.empty()) {
//            update_times.pop();
//        }
//        nodes.clear();
//        nodes = node_gen();
//        exogeneousInitializer();
//        updateTimesInitializer();
//    }

private:

	void updateTimesInitializer() {
		for (size_t i = 0 ; i < nodes.size() ; ++i) {
			update_times.push(std::make_pair(nodes[i].initialUpdateTime(),i));
		}
	}

	void update(size_t _to, datumType _datum) {
		nodes[_to].pushArrival(_datum);
	}

    double arrival_init_time;
	NodeGeneratorType node_generator;
	GraphProcessType graph_process;
    std::vector<nodeType> nodes;
	std::priority_queue<std::pair<double,size_t>,std::vector<std::pair<double,size_t>>,PairCompare>  update_times;  // queue of pairs (update time, node_id to update)

};

#endif

// template <typename NodeGenerator, typename GraphProcess>
// class MessageUpdater : public ConstGraphUpdater {
// public:

// 	typedef std::vector<std::pair<double,size_t>> Message;
// 	typedef std::pair<std::vector<std::pair<double,size_t>>,bool> ServicedMessage;

// 	template <typename S, typename T, typename U, typename V, typename W, typename X>
// 	MessageUpdater(const size_t _sz, const double _S, const S& _arrival_distribution_param, const T& _service_distribution_param,
// 		              const U& _login_distribution_param, const V& _send_distribution_param,
// 		              const W& _size_distribution_param, const X& _graph_process_param)
// 	: ConstGraphUpdater(_sz, _arrival_distribution_param, _service_distribution_param, _login_distribution_param,
// 				   		_send_distribution_param, _size_distribution_param, _graph_generator_param), S(_S)
// 	{
// 		graph = graph_process(0);
// 		node_initializer();
// 		update_times_initializer();
// 	}

// 	std::pair<const std::vector<Node>&,const double> operator()(){
// 		auto current = update_times.top();
// 		update_times.pop();
// 		auto [current_time, node_id] = current;
// 		Node& node = nodes[node_id];
// 		// Feed& feed = node.feed;
// 		auto [new_update_time,sent] = node.update(current_time);
// 		if (sent) {
// 			auto [neighbor_iter,neighbor_num] = graph.range(node_id);
// 			for (size_t i = 0 ; i < neighbor_num ; ++neighbor_iter) {
// 				update(neighbor_iter->second,node_id);
// 			}
// 		}
// 		update_times.push(std::make_pair(new_update_time,node_id));
// 		return std::make_pair<const std::vector<Node>&,const double>(nodes,current_time);
// 	}

// 	std::string info() const{
// 		std::stringstream ss;
// 		ss << "Updater=MessageUpdater;" << std::string(typeid(NodeGenerator).name()).substr(1);
// 		return ss.str();
// 	}

// protected:

// 	// puts the exogeneous service times into the nodes
// 	virtual void node_initializer() {
// 		for (size_t i = 0 ; i < sz ; ++i) {
// 			double current_time = 0;
// 			while (current_time < S) {
// 				current_time += nodes[i].genArrival(current_time,sz); // graph.size() is id of exogeneous.
// 			}
// 			current_time = 0;
// 		}
// 	}
// 	virtual void update_times_initializer() {
// 		size_t current_time = 0;
// 		for (size_t i = 0 ; i < sz ; ++i) {
// 			update_times.push(nodes[i].genUpdate(current_time));
// 		}
// 	}
// 	virtual void update(const size_t _to, const size_t _from) {
// 		auto msg = nodes[_from].getSentMessage();
// 		msg.pop_back();
// 		nodes[_to].push(msg,current_time,_from);
// 	}
// 	// virtual void node_data_update(const size_t i) {};

// 	Graph graph;
// 	const size_t S;
// };
