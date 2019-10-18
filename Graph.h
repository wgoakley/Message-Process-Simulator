#ifndef ___Graph___
#define ___Graph___

#include <set>
#include <utility>
#include <random>

class Graph
{

public:

    /**
     *  @typedef Edge data structure for edges
     *  @typedef Edgelist data structure for edgelist
     *  @typedef EdgeIteratorRange data structure for range of edges
     */
    typedef std::pair<size_t,size_t> Edge;
    typedef std::set<Edge> Edgelist;
    typedef std::pair<Edgelist::iterator,size_t> EdgeIteratorRange;

    /**
     *  Compiler generated null constructor
     */
    Graph() = default;

    /**
     *  @param el the edgelist
     *  @param sz the number of nodes
     */
    Graph(size_t);
    
    /**
     *  @param el the edgelist
     *  @param sz the number of nodes
     */
    Graph(Edgelist, size_t);
    
    /**
     *  @return the number of nodes in the graph
     */
    size_t size() const;

    /**
     *  resets the graph
     */
    void clear();
    
    /**
     *  @return iterator to beginning of the edgelist
     */
    Edgelist::iterator begin();
    
    /**
     *  @return iterator to end of the edgelist
     */
    Edgelist::iterator end();
    
    /**
     *  @return const_iterator to beginning of the edgelist
     */
    Edgelist::const_iterator begin() const;
    
    /**
     *  @return const_iterator to end of the edgelist
     */
    Edgelist::const_iterator end() const;

    /**
     *  @return the number of edges
     */
    size_t edges() const;

    /**
     *  @param i a node id
     *  @return in-degree of a node
     */
    size_t in_degree(size_t) const;
    
    /**
     *  @brief Gets the in-edges of a node.
     *  In-edges of a node i are edges (i,j).  This function returns
     *  an iterator to the first in-edge and the number of in-edges
     *  If a node has no neighbors, a pair containing an iterator
     *  to the first edge after where the node's edges should
     *  have been is returned, along with the value 0.
     *  @param i the node whose in-edges are wanted
     *  @return a pair denoting the in-edge range
     */
    EdgeIteratorRange range(size_t) const;
    
    /**
     *  Function to erase the first in-edge in an EdgeIteratorRange
     *  and return an EdgeIteratorRange with the next in-edge
     *  and the number of in-edges remaining.
     *  @param p the in-edges of a node
     *  @return the remaining in-edges of a node
     */
    EdgeIteratorRange erase(EdgeIteratorRange);
    
    /**
     *  @param e edge to be searched for
     *  @return true if edge is found; false if not
     */
    bool find_edge(const Edge&);
    
    /**
     *  @param e edge to be inserted
     *  @return iterator to inserted edge or existing edge and boolean for whether edge was inserted
     */
    std::pair<Edgelist::iterator,bool> insert_edge(const Edge&);

    /**
     *  @param e edge to be inserted
     *  @return iterator to inserted edge or existinge edge and boolean for whether edge was inserted
     */
    std::pair<Edgelist::iterator,bool> insert_edge(size_t, size_t);
    
    /**
     * @param e edge to be erased
     * @return the number of edges erased
     */
    size_t erase_edge(const Edge&);

    /**
     * @return info about the graph
     */
    std::string info() const;
    
private:

    Edgelist el; /// @var set of edges
    size_t sz; /// @var sz the number of nodes
};

#endif
