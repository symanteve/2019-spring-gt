/* 
    Your main program goes here
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();
NetworkManager *nm1 = new NetworkManager();

void draw(NetworkManager *pic){
	Gplot *gp = new Gplot();
    gp->gp_add(pic->elist);
    gp->gp_dump(true);
    gp->gp_export("plot");
};

vector<int> dijkstra(Path *path){
	vector<int> length(path->paths.size(), 0);
	for(int i = 0; i < path->paths.size(); ++i)
	{
		for(int j = 0; j < path->paths.at(i).size(); ++j)
		{
			length.at(i) += path->paths.at(i).at(j)->flowval;
		}
	}
	return length;//*min_element(length.begin(), length.end());
}; 

vector<string> node_name(NetworkManager* nm)
{
	Vertex* a = nm->get_all_nodes();
	vector<string> NN;
	while(a != NULL){
		NN.push_back(a->name);
		a = a->next;
	}
	return NN;
};

vector<pair<string, int> > vertex_degree_check(NetworkManager* nm, const vector<string> &NN){
	vector<pair<string, int> > vertex_degree(NN.size());
	Edge* EE = nm->elist;

	for(int i = 0; i < NN.size(); ++i){
		vertex_degree.at(i).first = NN.at(i);
		while(EE != NULL){
			if(EE->head->name == NN.at(i))
				vertex_degree.at(i).second++;
			else if(EE->tail->name == NN.at(i))
				vertex_degree.at(i).second--;
			EE = EE->next;
		}
		EE = nm->elist;
	}
	return vertex_degree;
};

void add_edge_to_be_balanced (NetworkManager* nm, const vector<pair<string, int> > &AA){
	vector<vector<string> > WOW;
	vector<string> negtive;
	vector<string> positive;
	for(auto &e : AA){
		if(e.second > 0){
			for(int i = 0; i < e.second; ++i)
				positive.push_back(e.first);
		}
		else if(e.second < 0){
			for(int i = 0; i < -e.second; ++i)
				negtive.push_back(e.first);
		}
	}
	
	int cba = 0; //the number of permutation
				 //three unblanced vertices has 6 permutation posible situations
				 //every time a permutation happen, WOW increase one more size to store paired vertices
	do{
			WOW.push_back( vector<string>() );
			for (int j = 0; j <positive.size(); ++j){
				//cout << negtive.at(j) << ' ' << positive.at(j) << endl;
			//	abc = nm->get_edge( nm->get_node(positive.at(j)), nm->get_node(negtive.at(j)) );
				WOW.at(cba).push_back(negtive.at(j));
				WOW.at(cba).push_back(positive.at(j));
			}
			cba++;
			
	}while(next_permutation(positive.begin(), positive.end()));


	Path *path;
	path = new Path();
	vector<int> pair_length, dij_vec;
	int temp_length;
	string head , tail;
	//find the shortest edge pair
	for(int i = 0; i < WOW.size(); ++i){
		temp_length = 0;
		for(int j = 0; j < WOW.at(i).size()/2; ++j){
			head = WOW.at(i).at(j*2);
			tail = WOW.at(i).at(j*2+1);
			path->append(nm->elist);
			path->find_paths(head , tail);
			dij_vec = dijkstra(path);
			temp_length += *min_element(dij_vec.begin(), dij_vec.end());
		}
		pair_length.push_back(temp_length);
	}
	
	int nth_pair;
	nth_pair = distance(pair_length.begin(), max_element(pair_length.begin(), pair_length.end()));

	pair_length.clear();
	//
	for(int j = 0; j < WOW.at(nth_pair).size()/2; ++j){
		temp_length = 0;
		head = WOW.at(nth_pair).at(j*2);
		tail = WOW.at(nth_pair).at(j*2+1);
		path->append(nm->elist);
		path->find_paths(head , tail);
cout << head <<' '<< tail<< ' ' << endl;

		for (int i = 0; i < path->paths.size(); ++i){
				cout  << i << endl;
				cout  <<  ' '  << path->paths.size() << endl;
			for (int k = 0; k < path->paths.at(i).size(); ++k){
				dij_vec = dijkstra(path);
				temp_length += *min_element(dij_vec.begin(), dij_vec.end());
			}
			pair_length.push_back(temp_length);
  		}
			cout << "done" << endl;

		int _nth_pair = distance(pair_length.begin(), max_element(pair_length.begin(), pair_length.end()));
			cout << _nth_pair << endl;
cout << path->paths.size() << endl;


		for(int l = 0; l < path->paths.at(_nth_pair).size(); ++l){
			head = path->paths.at(_nth_pair).at(l)->head->name;
			tail = path->paths.at(_nth_pair).at(l)->tail->name;
			cout << head <<' '<< tail<< ' ' << l << endl;
			nm->connect(head , tail);
			//nm->setlink(head , tail, 1, 0);
		}
		//	cout << head <<' '<< tail<< ' ' << dijkstra(path)<< endl;
		pair_length.clear();
	}
	//

//	for (auto &e : WOW){
//		for (auto &ee : e){
//			cout << ee << " ";
//		}
//		cout << endl;
//	}
};

void hierholzer(NetworkManager* nm, const vector<string> &NN){
	vector<string>  		subtour;
	vector<string> 			tour;
	vector<pair<string, string> > edge;
	int i = 0;
	string current_node;
	Edge* EE = nm->elist;
	while(EE != NULL){
		edge.push_back(pair<string, string>());
		edge.at(i).first = EE->head->name;
		edge.at(i).second = EE->tail->name;
		EE = EE->next;
		++i;
	}
	
	current_node = NN.at(0);
	subtour.push_back(current_node);
//
	i=0;
	bool if_dead_end = true;
	bool if_continue = true;
	int aaa = edge.size();
	while(aaa >= 0 ){
		aaa--;
		//		cout<< current_node <<endl;
	tour.push_back(current_node);

		for(int i = 0; i < edge.size(); ++i){
			if(edge.at(i).first == current_node){
				current_node = edge.at(i).second;
				subtour.push_back(current_node);
				cout << edge.at(i).first << ' ' << edge.at(i).second << endl;
				edge.erase(edge.begin()+i);
				if_dead_end = false;
				break;
			}
			if_dead_end = true;
		}
		if(if_dead_end){
			for(int i = subtour.size() - 1; i >= 0; --i){
				for(auto & e : edge){
					if(e.first == current_node){
						if_continue = true;	
						break;
					}
					if_continue = false;
				}
				if(if_continue)
					break;
				current_node = subtour.at(i);
				cout << current_node << endl;
			}
		}
		
//		if(current_node == NN.at(0)){
//			if(tour.size() != 0)
//				tour.erase(tour.begin());
//
//			tour.insert(tour.begin(), subtour.begin(), subtour.end());	
//			subtour.clear();
//		}
	}	
//


	for(auto&e : tour)
		cout << e << endl;
	//	cout << e.first << ' ' << e.second << endl;
};

int main(int argc, char** argv){

	
	nm->interpret("test.txt");
	vector<string> NN = node_name(nm);
	vector<pair<string, int> > AA = vertex_degree_check(nm, NN);
	add_edge_to_be_balanced(nm, AA);
	draw(nm);
	nm->print_all_e();
//	cout << nm->get_edge("e","a")->flowval << endl;
	hierholzer(nm, NN);















/*
	Path *path;
	path = new Path();
	path->append(nm->elist);
	vector<vector<Edge *>> avail_paths = path->find_paths(string("2"), string("1"));
	path->debug();
	for(int x=0;x<path->paths.at(0).size();x++)
	{
		ofs << path->paths.at(0).at(x)->head->name << ' ' <<  path->paths.at(0).at(x)->tail->name << ' '
			<< path->paths.at(0).at(x)->cap << ' ' << path->paths.at(0).at(x)-> flowval << endl;
	}
	ofs.close();
	cout << dijkstra(path)<<endl;
	nm->connect("2", "1");
	nm->setlink("2", "1", 1, dijkstra(path));
	nm->print_all_e();
*/
//	for(int i = 0; i<3; ++i)
//		nm1->add_edge(avail_paths.at(0).at(i));
/*
	row = avail_paths.begin();
	nm1->add_edge(*row->begin());
	
	for (col = row->begin(); col != row->end(); col++) {
//		nm1->add_edge(*col);
	}
	draw(nm1);
	*/
	

    /* start your program */

	
    return 0;
}
