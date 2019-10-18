#include "Graph.h"
#include <sstream>

Graph::Graph(size_t _sz) : sz(_sz) {}

Graph::Graph(Edgelist _el, size_t _sz) : el(std::move(_el)), sz(_sz) {}

size_t Graph::size() const { return sz;}

void Graph::clear() {
    el.clear();
    sz = 0;
}

Graph::Edgelist::iterator Graph::begin()
{
    return el.begin();
}

Graph::Edgelist::iterator Graph::end()
{
    return el.end();
}

Graph::Edgelist::const_iterator Graph::begin() const
{
    return el.begin();
}

Graph::Edgelist::const_iterator Graph::end() const
{
    return el.end();
}

size_t Graph::edges() const {
    return el.size();
}

size_t Graph::in_degree(size_t i) const {
    return range(i).second;
}

Graph::EdgeIteratorRange Graph::range(size_t i) const
{
    size_t num = 0;
    Edgelist::iterator lb = el.lower_bound(std::make_pair(i,0));
    while (lb!=end() && lb->first == i) {
        ++lb;
        ++num;
    }
    return std::make_pair(el.lower_bound(std::make_pair(i,0)),num);
}

Graph::EdgeIteratorRange Graph::erase(EdgeIteratorRange p)
{
    return std::make_pair(el.erase(p.first),--(p.second));
}

bool Graph::find_edge(const Edge& e) {
    return el.find(e) != el.end();
}

std::pair<Graph::Edgelist::iterator,bool> Graph::insert_edge(const Edge& e) {
    return el.insert(e);
}

std::pair<Graph::Edgelist::iterator,bool> Graph::insert_edge(size_t i, size_t j) {
    return el.insert(std::make_pair(i,j));
}    

size_t Graph::erase_edge(const Edge& e) {
    return el.erase(e);
}

std::string Graph::info() const {
    std::stringstream ss;
    ss << "Graph;" << "nodes=" << sz << "edges=" << edges();
    size_t num = 0;
    for (size_t i = 0 ; i < sz ; ++i) {
        if (range(i).second != 0) {
            ++num;
        }
    }
    ss << ";nonzeroNodes=" << num;
    return ss.str();
}
