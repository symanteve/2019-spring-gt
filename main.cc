/* 
    Your main program goes here
*/
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <algorithm>
#include "network_manager.h"
#include "gplot.h"
#include "path.h"

using namespace std;

// create NetworkManager first
NetworkManager *nm = new NetworkManager();

void draw(NetworkManager *pic){
	Gplot *gp = new Gplot();
    gp->gp_add(pic->elist);
    gp->gp_dump(true);
    gp->gp_export("plot");
}

void BFS(NetworkManager* nm, const vector<string> &NN, string src, string dst){
	queue<vector<string> > q;
	vector<string> one_path;
	vector<Edge *> temp_path;
	vector<vector<Edge*> > pathss;
	bool isNotVisited = true;
	vector<string>  adjancy;



	one_path.push_back(src);
	q.push(one_path);
//	vector<vector<string> > adjancy(NN.size(), vector<string>());
	Edge* temp_edge;

	while(!q.empty()){
		one_path = q.front();
		q.pop();
		string last = one_path.at(one_path.size()-1);

		//find vertex connect to "last"
		temp_edge = nm->elist;
		adjancy.clear();
		while(temp_edge != NULL){
			if(temp_edge->head->name == last){
				adjancy.push_back(temp_edge->tail->name);
			}
			temp_edge = temp_edge->next;
		}
		if (last == dst){
			cout << "one_path :" << endl;
				for(auto & e : one_path)
					cout << e << ' ';
			cout << endl;
			for(int i = 0; i < one_path.size() - 1; ++i){
				temp_edge = nm->get_edge(one_path.at(i), one_path.at(i+1));
				temp_path.push_back(temp_edge);
			}
			pathss.push_back(temp_path);
			temp_path.clear();
		}
		for (int i = 0; i < adjancy.size(); ++i){
			for (int j = 0; j < one_path.size(); ++j){
				if (one_path.at(j) == adjancy.at(i))
					isNotVisited = false;
					break;
			}
			if(isNotVisited){
				vector<string> new_one_path(one_path);
				new_one_path.push_back(adjancy.at(i));
				q.push(new_one_path);
			}
			isNotVisited = true;
		}
	}
			cout << "pathss :" << endl;
			for(auto & e : pathss){
					for(auto & ee : e)
					cout << ee->head->name << ' ' << ee->tail->name << "   ";
cout << endl;
			}
}

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
} 

vector<string> node_name(NetworkManager* nm)
{
	Vertex* a = nm->get_all_nodes();
	vector<string> NN;
	while(a != NULL){
		NN.push_back(a->name);
		a = a->next;
	}
	return NN;
}

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
}

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
	//permutate the vector to the very front
	while(prev_permutation(positive.begin(), positive.end()));
	do{
			WOW.push_back( vector<string>() );
			for (int j = 0; j <positive.size(); ++j){
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

		for (int i = 0; i < path->paths.size(); ++i){
			for (int k = 0; k < path->paths.at(i).size(); ++k){
				dij_vec = dijkstra(path);
				temp_length += *min_element(dij_vec.begin(), dij_vec.end());
			}
			pair_length.push_back(temp_length);
  		}

		int _nth_pair = distance(pair_length.begin(), max_element(pair_length.begin(), pair_length.end()));


		for(int l = 0; l < path->paths.at(_nth_pair).size(); ++l){
			head = path->paths.at(_nth_pair).at(l)->head->name;
			tail = path->paths.at(_nth_pair).at(l)->tail->name;
			nm->connect(head , tail);
		}
		pair_length.clear();
	}
}

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
	tour.push_back(current_node);

		for(int i = 0; i < edge.size(); ++i){
			if(edge.at(i).first == current_node){
				current_node = edge.at(i).second;
				subtour.push_back(current_node);
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
			}
		}
		
	}	

	for(auto&e : tour)
		cout << e << endl;
}

int main(int argc, char** argv){
	
//	nm->interpret("test.txt");
	nm->interpret(argv[1]);
	vector<string> NN = node_name(nm);
	BFS(nm, NN, "0", "3");
	vector<pair<string, int> > AA = vertex_degree_check(nm, NN);
	add_edge_to_be_balanced(nm, AA);
//	nm->print_all_e();
//	hierholzer(nm, NN);


    return 0;

}
