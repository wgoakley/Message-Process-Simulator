#ifndef ___GraphProcesses___
#define ___GraphProcesses___

#include <random>
#include "Graph.h"
#include <tuple>
#include <unordered_map>
#include <numeric>
#include <sstream>
#include <cmath>

namespace graphProcess {

	class ConstDirectedCircleGraph {
	public:
		ConstDirectedCircleGraph(const size_t _nodes) : nodes(_nodes), g(_nodes) {
			size_t i = 0;
			for (; i < nodes-1 ; ++i) {
				g.insert_edge(i,i+1);
			}
			g.insert_edge(i,0);
		}

        const Graph& operator()(const double t) const {
            return g;
        }
	private:
		size_t nodes;
		Graph g;
	};

	class ConstCircleGraph {
	public:
		ConstCircleGraph(const size_t _nodes) : nodes(_nodes), g(_nodes) {
			size_t i = 0;
			for (; i < nodes-1 ; ++i) {
				g.insert_edge(i,i+1);
				g.insert_edge(i+1,i);
			}
			g.insert_edge(i,0);
			g.insert_edge(0,i);
		}

		const Graph& operator()(const double t) const {
			return g;
		}
	private:
		size_t nodes;
		Graph g;
	};

	class ConstDirectedLineGraph {
	public:
		ConstDirectedLineGraph(const size_t _nodes) : nodes(_nodes), g(_nodes) {
			size_t i = 0;
			for (; i < nodes-1 ; ++i) {
				g.insert_edge(i,i+1);
			}
		}

        const Graph& operator()(const double t) const {
            return g;
        }
	private:
		size_t nodes;
		Graph g;
	};


	class ConstLineGraph {
	public:
		ConstLineGraph(const size_t _nodes) : nodes(_nodes), g(_nodes) {
			size_t i = 0;
			for (; i < nodes-1 ; ++i) {
				g.insert_edge(i,i+1);
				g.insert_edge(i+1,i);
			}
		}

        const Graph& operator()(const double t) const {
            return g;
        }
	private:
		size_t nodes;
		Graph g;
	};

	class ConstCompleteGraph {
	public:
		ConstCompleteGraph(const size_t _nodes) : nodes(_nodes), g(_nodes) {
			size_t j = 0;
			for (size_t i = 0 ; i < nodes ; ++i) {
				for (j = 0 ; j < i ; ++j) {
					g.insert_edge(i,j);
				}
				++j;
				for (; j < nodes ; ++j) {
					g.insert_edge(i,j);
				}
			}
		}

        const Graph& operator()(const double t) const {
            return g;
        }
	private:
		size_t nodes;
		Graph g;
	};
    
    class ConstRegularGraph {
    public:
        ConstRegularGraph(const size_t _nodes, const size_t _k) : nodes(_nodes), k(_k), g(_nodes) {
            std::vector<size_t> vin(nodes*k);
            for (size_t i = 0 ; i < nodes ; ++i) {
                for (size_t j = 0 ; j < k ; ++j) {
                    vin[i*k + j] = i;
                }
            }
            std::vector<size_t> vout(vin);
            std::mt19937 gen((std::random_device())());
            std::shuffle(vin.begin(), vin.end(), gen);
            for (size_t i = 0 ; i < nodes*k ; ) {
                if (vout[i] != vin[i]) {
                    g.insert_edge(vout[i],vin[i]);
                    ++i;
                } else {
                    std::uniform_int_distribution<size_t> dis(i+1,nodes*k-1);
                    std::swap(vin[i],vin[dis(gen)]);
                }
            }
        }
        
        const Graph& operator()(const double t) const {
            return g;
        }
    private:
        size_t nodes;
        size_t k;
        Graph g;
    };

	class ConstDirectedERGraph {
	public:
		ConstDirectedERGraph(const size_t _nodes, const double _p) : nodes(_nodes), p(_p), gen((std::random_device())()), geo(_p), g(_nodes) {
			size_t n = 0;
			size_t old = 0;
			size_t multiples = 0;
			while (true) {
				n += geo(gen);
				multiples = ((n - n%(nodes+1))-(old-old%(nodes+1)))/(nodes+1); // number of self-edges between n and old == number of multiples of nodes+1
				while (multiples > 0) {
					old = n;
					n += multiples;
					multiples = ((n - n%(nodes+1))-(old-old%(nodes+1)))/(nodes+1);
				}
				if (n < nodes*nodes) {
					g.insert_edge(n/nodes,n%nodes);
				} else {
					break;
				}
			}
		}

        const Graph& operator()(const double t) const {
            return g;
        }
	private:
		size_t nodes;
		double p;
		std::mt19937 gen;
		std::geometric_distribution<size_t> geo;
		Graph g;
	};

    class ConstPeriodicFourLattice {
    public:
        ConstPeriodicFourLattice(const size_t _nodes) : g(_nodes) {
            for (size_t i = 1 ; i < _nodes-1 ; ++i) {
                for (size_t j = 1 ; j < _nodes-1 ; ++j) {
                    g.insert_edge(i*_nodes + j, i*_nodes + j + 1);
                    g.insert_edge(i*_nodes + j, i*_nodes + j - 1);
                    g.insert_edge(i*_nodes + j, (i+1)*_nodes + j);
                    g.insert_edge(i*_nodes + j, (i-1)*_nodes + j);
                }
            }
            for (size_t i = 1 ; i < _nodes-1 ; ++i) {
                g.insert_edge(i,i-1);
                g.insert_edge(i,i+1);
                g.insert_edge(i,i*_nodes + i);
                g.insert_edge(i,(_nodes-1)*_nodes+i);
            }
            for (size_t i = 1 ; i < _nodes-1 ; ++i) {
                g.insert_edge((_nodes-1)*_nodes + i,(_nodes-1)*_nodes + i - 1);
                g.insert_edge((_nodes-1)*_nodes + i,(_nodes-1)*_nodes + i + 1);
                g.insert_edge((_nodes-1)*_nodes + i,(_nodes-2)*_nodes + i);
                g.insert_edge((_nodes-1)*_nodes + i,i);
            }
            for (size_t i = 1 ; i < _nodes-1 ; ++i) {
                g.insert_edge(_nodes*i,_nodes*i+(_nodes-1));
                g.insert_edge(_nodes*i,_nodes*i+1);
                g.insert_edge(_nodes*i,_nodes*(i+1));
                g.insert_edge(_nodes*i,_nodes*(i-1));
            }
            for (size_t i = 1 ; i < _nodes-1 ; ++i) {
                g.insert_edge(_nodes*i+(_nodes-1),_nodes*i+(_nodes-1)-1);
                g.insert_edge(_nodes*i+(_nodes-1),_nodes*i);
                g.insert_edge(_nodes*i+(_nodes-1),_nodes*(i+1)+(_nodes-1));
                g.insert_edge(_nodes*i+(_nodes-1),_nodes*(i-1)+(_nodes-1));
            }
            //bl
            g.insert_edge(0,_nodes-1);
            g.insert_edge(0,1);
            g.insert_edge(0,_nodes);
            g.insert_edge(0,(_nodes-1)*_nodes);
            //br
            g.insert_edge(_nodes-1,0);
            g.insert_edge(_nodes-1,(_nodes-1)-1);
            g.insert_edge(_nodes-1,_nodes + (_nodes-1));
            g.insert_edge(_nodes-1,_nodes*_nodes);
            //tl
            g.insert_edge((_nodes-1)*_nodes,_nodes*_nodes);
            g.insert_edge((_nodes-1)*_nodes,(_nodes-1)*_nodes + 1);
            g.insert_edge((_nodes-1)*_nodes,((_nodes-1)-1)*_nodes);
            g.insert_edge((_nodes-1)*_nodes,0);
            //tr
            g.insert_edge(_nodes*_nodes,_nodes*_nodes-1);
            g.insert_edge(_nodes*_nodes,(_nodes-1)*_nodes);
            g.insert_edge(_nodes*_nodes,(_nodes-1));
            g.insert_edge(_nodes*_nodes,(_nodes-1)*_nodes+(_nodes-1));

        }
        
        const Graph& operator()(const double t) const {
            return g;
        }
    private:
        size_t nodes; // half the number of nodes
        Graph g;
    };
    
    template <typename InType, typename OutType, typename ReciprocalType>
    class ConstDirectedReciprocalChungLu {
    public:
        template <typename... S, typename... T, typename... U>
        ConstDirectedReciprocalChungLu(const size_t _nodes, std::tuple<S...> _in_params, std::tuple<T...> _out_params, std::tuple<U...> _reciprocal_params) : nodes(_nodes), gen((std::random_device())()), in(std::move(std::make_from_tuple<InType,std::tuple<S...>>(std::move(_in_params)))), out(std::move(std::make_from_tuple<OutType,std::tuple<T...>>(std::move(_out_params)))), reciprocal(std::move(std::make_from_tuple<ReciprocalType,std::tuple<U...>>(std::move(_reciprocal_params)))), g(_nodes) {initialize();}
        
        const Graph& operator()(const double t) const {
//            std::unordered_map<size_t, size_t> outdeg;
//            for (auto it = g.begin() ; it != g.end() ; ++it) {
//                auto nhbrs = g.range(it->first);
//                if (outdeg.find(nhbrs.second) != outdeg.end()) {
//                    outdeg[nhbrs.second]++;
//                } else {
//                    outdeg[nhbrs.second] = 1;
//                }
//            }
            
            return g;
        }

    private:

        void initialize() {
            
            std::vector<size_t> in_indices = vertexSelect(in,nodes,nodes);
            std::vector<size_t> out_indices = vertexSelect(out,nodes,nodes);
            std::vector<size_t> reciprocal_indices_1 = vertexSelect(reciprocal,nodes,nodes/2);
            std::vector<size_t> reciprocal_indices_2 = vertexSelect(reciprocal,nodes,nodes/2);
            
            for (size_t i = 0, sz = std::min(in_indices.size(),out_indices.size()) ; i < sz ; ++i) {
                if (out_indices[i]!=in_indices[i]) {
                    g.insert_edge(out_indices[i],in_indices[i]);
                }
            }
            
            for (size_t i = 0 , sz = std::min(reciprocal_indices_1.size(),reciprocal_indices_2.size()) ; i < sz ; ++i) {
                if (reciprocal_indices_1[i] != reciprocal_indices_2[i]) {
                    g.insert_edge(reciprocal_indices_1[i],reciprocal_indices_2[i]);
                    g.insert_edge(reciprocal_indices_2[i],reciprocal_indices_1[i]);
                }
            }
            
        }
        
        template <typename Distribution>
        std::vector<size_t> vertexSelect(Distribution& dis, size_t nodes, size_t k) {
            // initialization of "degree distribution"
            std::unordered_multimap<size_t,size_t> n_map;
            size_t i = 0;
            for (; i < k; ++i) {
                size_t index = dis(gen);
                n_map.insert(std::make_pair(index,i));
            }

            // initialization of in-weights
            // get largest key in the multimap
            size_t largest = 0;
            for (const auto& x : n_map) {
                if (x.first > largest) {
                    largest = x.first;
                }
            }

            // create vector to store the weight to choose nodes
            std::vector<size_t> weight(largest,0);
            size_t num_edges = 0;
            for (size_t i = 0, sz = weight.size(); i != sz ; ++i) {
                weight[i] = (i+1)*n_map.count(i);
                num_edges+=weight[i];
            }
            // initialization of integer distribution
            std::discrete_distribution<> index_dis(std::begin(weight),std::end(weight));
            // apply integer distribution to nodes
            
            std::vector<size_t> indices(num_edges);
            for (size_t i = 0 ; i < num_edges ; ++i) {
                size_t deg = index_dis(gen);
                auto range = n_map.equal_range(deg);
                auto count = std::distance(range.first,range.second);
                if (count > 0) {
                    std::uniform_int_distribution<> tmp(0,count-1);
                    size_t tmp_index = tmp(gen);
                    for (size_t j = 0 ; j < tmp_index ; ++j) {
                        ++range.first;
                    }
                    indices[i] = (range.first)->second;
                }
            }
            
            // create a permutation
            std::vector<size_t> permutation(nodes);
            std::iota(std::begin(permutation),std::end(permutation),0);
            std::shuffle(std::begin(permutation),std::end(permutation),gen);
            
            // shuffle up edge assignments
            for (size_t i = 0 ; i < num_edges ; ++i) {
                assert(indices[i] < nodes);
                indices[i] = permutation[indices[i]];
            }
            
            return indices;
            
        }
    
        size_t nodes;
        std::mt19937 gen;
        InType in;
        OutType out;
        ReciprocalType reciprocal;
        Graph g;
    };
    
    template <typename InType, typename OutType>
    class ConstDirectedChungLu {
    public:
        template <typename... S, typename... T>
        ConstDirectedChungLu(const size_t _nodes, std::tuple<S...> _in_params, std::tuple<T...> _out_params) : nodes(_nodes), gen((std::random_device())()), in(std::move(std::make_from_tuple<InType,std::tuple<S...>>(std::move(_in_params)))), out(std::move(std::make_from_tuple<OutType,std::tuple<T...>>(std::move(_out_params)))), g(_nodes) {initialize();}
        
        const Graph& operator()(const double t) const {
            //            std::unordered_map<size_t, size_t> outdeg;
            //            for (auto it = g.begin() ; it != g.end() ; ++it) {
            //                auto nhbrs = g.range(it->first);
            //                if (outdeg.find(nhbrs.second) != outdeg.end()) {
            //                    outdeg[nhbrs.second]++;
            //                } else {
            //                    outdeg[nhbrs.second] = 1;
            //                }
            //            }
            
            return g;
        }
        
    private:
        
        void initialize() {
            
            std::vector<size_t> in_indices = vertexSelect(in,nodes,nodes);
            std::vector<size_t> out_indices = vertexSelect(out,nodes,nodes);
            
            for (size_t i = 0, sz = std::min(in_indices.size(),out_indices.size()) ; i < sz ; ++i) {
                if (out_indices[i]!=in_indices[i]) {
                    g.insert_edge(out_indices[i],in_indices[i]);
                }
            }
            
        }
        
        template <typename Distribution>
        std::vector<size_t> vertexSelect(Distribution& dis, size_t nodes, size_t k) {
            // initialization of "degree distribution"
            std::unordered_multimap<size_t,size_t> n_map;
            size_t i = 0;
            for (; i < k; ++i) {
                size_t index = dis(gen);
                n_map.insert(std::make_pair(index,i));
            }
            
            // initialization of in-weights
            // get largest key in the multimap
            size_t largest = 0;
            for (const auto& x : n_map) {
                if (x.first > largest) {
                    largest = x.first;
                }
            }
            
            // create vector to store the weight to choose nodes
            std::vector<size_t> weight(largest,0);
            size_t num_edges = 0;
            for (size_t i = 0, sz = weight.size(); i != sz ; ++i) {
                weight[i] = (i+1)*n_map.count(i);
                num_edges+=weight[i];
            }
            // initialization of integer distribution
            std::discrete_distribution<> index_dis(std::begin(weight),std::end(weight));
            // apply integer distribution to nodes
            
            std::vector<size_t> indices(num_edges);
            for (size_t i = 0 ; i < num_edges ; ++i) {
                size_t deg = index_dis(gen);
                auto range = n_map.equal_range(deg);
                auto count = std::distance(range.first,range.second);
                if (count > 0) {
                    std::uniform_int_distribution<> tmp(0,count-1);
                    size_t tmp_index = tmp(gen);
                    for (size_t j = 0 ; j < tmp_index ; ++j) {
                        ++range.first;
                    }
                    indices[i] = (range.first)->second;
                }
            }
            
            // create a permutation
            std::vector<size_t> permutation(nodes);
            std::iota(std::begin(permutation),std::end(permutation),0);
            std::shuffle(std::begin(permutation),std::end(permutation),gen);
            
            // shuffle up edge assignments
            for (size_t i = 0 ; i < num_edges ; ++i) {
                assert(indices[i] < nodes);
                indices[i] = permutation[indices[i]];
            }
            
            return indices;
            
        }
        
        size_t nodes;
        std::mt19937 gen;
        InType in;
        OutType out;
        Graph g;
    };
    
}

#endif
