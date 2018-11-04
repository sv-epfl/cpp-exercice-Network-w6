#include "network.hpp"
#include <iostream> 
#include "random.hpp"
#include <map> 
#include <iostream> 
//class RandomNumbers ;

// using namespace Network ;

   Network :: Network() {
		values = std:: vector<double>();
	    links = std::multimap<size_t, size_t>() ;
	   }
		///Resizes the list of nodes (\ref values) and also resets all values.
		///After this function is called \ref values has size *n* and contains random numbers (normal distribution, mean=0, sd=1).
	void Network:: resize(const size_t& size_) {
		// even if it is a size_t let's check if it is positive 
		if ( size_ >= 0){
			//  peut pas comparer  un obj qui nexiste pas donc initilisé obj dans le constructeur 
			if (size_ >= values.size()) { // then there is no problem with the links // it willl juste add random nodes values to the vector of nodes
				 values.resize(size_) ;	
				
			}else { // we have a problem with the links !! 
					// why statement no effect if predeclare size_t nodes_to_delete (size_) ; 	
				
				for(size_t nodes_to_delete (size_); nodes_to_delete < values.size() ; ++ nodes_to_delete) {
					size_t nb_nodes_to_delete = links.count(nodes_to_delete);
					for (size_t u (0) ; u < nb_nodes_to_delete; ++ u) {
						std::multimap<size_t,size_t>::iterator it = links.find(nodes_to_delete); // will only take the first one found
						links.erase(it) ; 
						
						// need also to delete the reverse links b->a (as we have stored each link twice : a->b AND b->a) 
						// let's iterate over all multimap 
						std::multimap<size_t,size_t>::iterator iter ; 
						for (iter=links.begin(); iter!=links.end(); ++iter) {
							if ((*iter).first == (*it).second and (*iter).second == (*it).first) links.erase(iter) ; 
						}
					}
				}
				values.resize(size_) ; 
			}
			// we have also to put new values in the values => random numbers normal distribution 
			RandomNumbers rn ; 
			rn.normal(values, 0, 1); // will change the values of the vector values mean =0 and SD = 1
		}
	}
	bool Network::  add_link(const size_t& a, const size_t& b){
		// if the values a or b are not nodes it will not create a link ! 
		// if the links already exists it will not create another link 

		if (is_number_in_values(a) and is_number_in_values(b) and !(a==b)) {
			// check if there is no link already existing
			if (! is_link(a,b)) {
				links.insert({a,b}) ;
				links.insert({b,a}) ;
				return true ; 
			}
		}
		return false;  	
	}
	
	bool Network::  is_number_in_values (const size_t& nb) {
		 return (nb>=0 and nb < values.size()) ; 
	}
	bool Network::  is_link (const size_t& a , const size_t& b) {
		std::multimap<size_t,size_t>::iterator it ;
		if (links.size() != 0 ) { // otherwise the iterator will not work properly 
			for( it = links.begin() ; it!= links.end() ; ++ it) {
				
				if ( (*it).first == a and (*it).second== b ) { 
					return true ; // as I choose to put always for each pair of connected nodes a link between a and b plus a link between b and a it is fine 
					
				}
			}
		}
		return false ; 
	}
//each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.	
	// 	WHAT DO I HAVE TO RETURN ?!? => let's return the sum of all nodes 
/*! Creates random connections between nodes: each node *n* will be linked with *degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.
 All previous links are ERASED first.
 @param mean_deg the average of the Poisson distribution.
 */
 /// AS the return parameter was not well defined I decided to return the sur of the degree of the nodes = 2*number of links 
	size_t Network::  random_connect(const double& mean_poisson_law) {
		size_t count_how_many_not_suceeded (0) ;  
		links.clear() ; // leaves empty size = 0
		std::vector <int> degree_of_nodes (values.size()) ; 
		size_t sum (0); 
		RandomNumbers rand ; 
		rand.poisson(degree_of_nodes, mean_poisson_law); 
		std::multimap<size_t, size_t> nodes_still_want_connections;  // first column with node reference number and the second column with the number of nodes it wants 
		
		
		for (size_t i (0) ; i < values.size() ; ++i) {	 
			// checking the values
			if (degree_of_nodes[i]<0 ){
				 degree_of_nodes[i] = 0 ; // choice to put to zero and not to make the value in  absolute !!
			 }
			if (degree_of_nodes[i]> values.size()-1 ){	// ONE NODE CAN MAXIMUM BIND TO N-1 OTHER NODES (NOT TO HIMSELF) 
														// if the values is higher => consider an ABERRANT value so as not in the definition set it will be as zero ! 
 				 degree_of_nodes[i] = 0 ; // choice to put to zero and not to make the value in  absolute !!!
			 }

			 
			size_t nb_links_to_create ((degree_of_nodes[i])- (links.count(i))) ;  
			std::vector<int> new_links (nb_links_to_create); 
			rand.uniform_int(new_links, 0, (values.size()-1)); // last node reference is .size() -1 // as the min values is 0 it will always produce positive integers
		
			for (size_t u (0); u < new_links.size() ; ++u) {
				// ATTENTION IT WILL ONLY CREATE A LINK IF THE RECEIVER OF THE LINK *DOES NOT* HAS ALREADY THE NUMBER OF LINKS DEFINED BY THE POISSON
				// IF THE RECEIVER CAN'T ACCEPT A NEW LINK WE WILL GENERATE ANOTHER LINK WITH THE UNIFORM LAW // this will be repeated 3 times in order to really try to connect well the nodes 
				// at the end the nodes that couldn't connect to degree n will be put together and be randomelly connected with each other
				size_t receiver_node_number (new_links[u]) ; 	
				bool response (false) ; 
				
				if (degree_of_nodes[receiver_node_number] > links.count(receiver_node_number)) {
					response =  add_link (i,new_links[u]) ;
					if (response) sum += 1 ;
					//else : this could happen if the link already exists => very small probability....
				}else if (!response) { // if doesn't entered the if response is stil false !  // lets apply this algo. for the first half 
					// actually we can't link all the nodes with the degree they want if the sum of the degrees is not a multiple of two
					// number of links = sum degree nodes /2 
			
					std:: vector <int> trying_other_links (3) ; 
					rand.uniform_int(trying_other_links,0, values.size()-1);
					bool link_completed (false) ; 
					
					for ( size_t w(0) ; ((!link_completed) and (w < trying_other_links.size())) ; ++ w) {
						size_t receiver_node (trying_other_links[w]) ;
						if (degree_of_nodes[receiver_node] > links.count(receiver_node)) {
							link_completed =  add_link (i,receiver_node) ;
						}
					}	
					
					if (link_completed) sum += 1 ;
				}
			}
		
		}
		// let's bind together all the nodes that still want to be bound 
		 for (size_t k (0) ; k < values.size() ; ++k) {
			if ((degree_of_nodes[k]) > (links.count(k))) { // couldn't do all the links randomly choosen by a poisson law 
				size_t number_links_not_done ((degree_of_nodes[k])- (links.count(k))) ; 
				nodes_still_want_connections.insert({k,number_links_not_done}) ;
				count_how_many_not_suceeded +=number_links_not_done ;
			}
		}
		size_t links_done_between_nodes_needed_more_links (0); 
		links_done_between_nodes_needed_more_links = bind_randomely_nodes (nodes_still_want_connections) ; //74 msec sans 
		sum += links_done_between_nodes_needed_more_links ; 
		
	return sum*2 ; // the return for me is the sum of the degree of the nodes	
	}

	size_t Network:: bind_randomely_nodes (std::multimap<size_t,size_t> nodes_to_bind){

		//let's just shuffle all the vectors and then connect the first one with the second, the third,... until (the first node) has reached the quota of links he wants (second number of the multimap) 
		// same algorithm for each node : so they will only be shuffeled once in total
		
		size_t sum (0) ; 
		std:: vector<size_t> nodes (0); 

		std::multimap<size_t,size_t>::iterator it ;
		
		for( it = nodes_to_bind.begin() ; it!= nodes_to_bind.end() ; ++ it) {
			nodes.push_back((*it).first) ; 
		}
		std::vector < size_t> degree_nodes (nodes.size(),0) ;     	
    	
    	RandomNumbers rand ; 
		rand.shuffle(nodes) ;
		// now that we have shuffeled all nodes, we need to put in relation the degree of the nodes with the right node : 
		for (size_t k(0) ; k < nodes.size() ; ++k) {
			for(auto it = nodes_to_bind.begin() ; it!= nodes_to_bind.end() ; ++ it) {
				if ( nodes[k] == (*it).first) {
					degree_nodes[k] = (*it).second ;
					break; 
				}
			}
		} 

		size_t size_(nodes.size()) ; 
		for (size_t g(0) ; g < size_ ; ++ g) { // will erase from the list the ones done.. 
			for (size_t x (0) ; x <degree_nodes[0] and x < (nodes.size() -1) ; ++ x) { // will erase from the list the ones done so it will always take the first one : [0]
				
				bool added (false); 
				if ( degree_nodes[x+1] > 0) {
					added = add_link(nodes[0],nodes[x+1]) ;
				}
				if (added) {
					sum += 1 ;
					degree_nodes[x+1] -=1 ; // has got one link 
				}
			} 
			nodes.erase (nodes.begin());
			degree_nodes.erase (degree_nodes.begin()) ; 	
		}
		return sum ; 	
	} 

    size_t Network::  set_values(const std::vector<double>& new_values){
		// if want to change values of nodes : there should not be nodes added (? MAYBE NOT...) 
		if (new_values.size() == values.size() ){
			values.clear() ; //vector size 0 
			values = new_values ; // will change automatically size accordingly
			return values.size() ; 
		}
		else if (new_values.size() < values.size()){
			// will only change the first ones
			for (size_t i (0) ; i < new_values.size() ; ++i){
				values[i]= new_values[i] ; 
			}
		return new_values.size(); 
		}else {
			for (size_t i (0) ; i < values.size() ; ++i) {
				values[i]= new_values[i] ; 	
			}
			return values.size() ; 
		} 
	}

    size_t Network::  size() const {
		return values.size() ;  	
	}
/* Degree (number of links) of node no *n* */
    size_t Network:: degree(const size_t & n) const{
		return links.count(n) ; 
	}
/* Value of node no *n* */ // beginning with node number 0 
    double Network:: value(const size_t & n) const{
		return (values[n]) ;	
	}
/* All node values in descending order */
    std::vector<double> Network:: sorted_values() const{
		std::vector<double> sorted = values; 
		std::sort (sorted.begin(),sorted.end(),way_to_sort) ; // WHILE CALLING WAY_TO_SORT YOU CAN CALL A FUNCTITON POINTER 
		
		// () execute au moment ou met mais sans () :  
		// algo va l'exécuter bc fois en interne executé plus tard avec paramètre => pointeur sur fonction !! 
		return sorted; 
	}
	bool way_to_sort (double i, double j ) {return i > j;}

/* All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> Network:: neighbors(const size_t& n) const{
		std::vector<size_t> neighbors_nb ;  // the size is links.count(n) 
		//std::multimap< size_t,size_t>::iterator iter ; // const_iterator itératuer vers valeur constante
		for (auto iter = links.begin(); iter!=links.end(); ++iter) { 
			if ((*iter).first == n) {
				neighbors_nb.push_back((*iter).second) ;
			}
		}
		if (neighbors_nb.size() != links.count(n)) std::cerr<< "THE COUNTING WENT WRONG" << std::endl ; 
		return neighbors_nb ; 
	}
	
	
	
	
	
