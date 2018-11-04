#include <map>
#include <vector>
#include <iostream>


/*!
  This is a network of nodes with bidirectional links: if <b>(a, b)</b> is a link then <b>(b, a)</b> is also a link.

  Each node supports a *double* value, and the list of nodes is in fact defined by the list of their values.
 */
 
 // the degree is the nubmer of links each is BIDIRECTIONAL 
 // links is nb of connections 

// as the random connection systems only support int and not size_t they will be several comparisons between int and size_t 
class Network {

public:
    Network() ;//{}

/*  Resizes the list of nodes (\ref values)
    void resize(const size_t&);
	Adds a bidirectional link between two nodes
	@param[in] a,b the indexes Of the two nodes 
	@param[out] success the link was succesfully inserted
 */

 /*! 
    Resizes the list of nodes (\ref values) and also resets all values.
    After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
 */
    void resize(const size_t &n);
/*! 
  Adds a bidirectional link between two nodes 
    @param[in] a,b the indexes Of the two nodes  
   @param[out] success the link was succesfully inserted (true if both nodes exist and the link did not exist yet)
    */
    bool add_link(const size_t&, const size_t&);

/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.

  All previous links are ERASED first.
  @param[in] mean_deg the average of the Poisson distribution.
  @param[out] decided to return the sum of the degrees of the nodes
 */
    size_t random_connect(const double&);
/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
    size_t set_values(const std::vector<double>&);
/*! Number of nodes */
    size_t size() const;
/*! Degree (number of links) of node no *n* */
    size_t degree(const size_t &_n) const;
/*! Value of node no *n* */
    double value(const size_t &_n) const;
/*! All node values in descending order */
    std::vector<double> sorted_values() const;
/*! All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> neighbors(const size_t&) const;

private:
    std::vector<double> values;
    std::multimap<size_t, size_t> links; // chaque clé à une valeur 
    
	bool is_number_in_values (const size_t& nb) ;
	bool is_link (const size_t& a , const size_t& b) ;
	size_t bind_randomely_nodes (std::multimap<size_t,size_t> nodes_to_bind) ;
	

};

bool way_to_sort (double i, double j ) ;

