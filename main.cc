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

int dijkstra(Path *path){
	vector<int> length(path->paths.size(), 0);
	for(int i = 0; i < path->paths.size(); ++i)
	{
		for(int j = 0; j < path->paths.at(0).size(); j++)
		{
			length.at(i) += path->paths.at(i).at(j)->flowval;
		}
	}
	return *min_element(length.begin(), length.end());
}; 

vector<string> node_name(Vertex* v)
{
	int vertices_num = 1;
	Vertex* a = v;
	while(a->next != NULL){
		vertices_num++;
		a = a->next;
	}

	vector<string> NN(vertices_num);
	for(int i = 0; i < vertices_num; ++i){
		NN.at(i) = v->name;
		v = v->next;
	}
	return NN;
};

vector<pair<string, int> > vertex_degree_check(NetworkManager* nm){
	vector<string> NN = node_name(nm->get_all_nodes());
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

void add_edge_to_be_balanced (NetworkManager* nm, vector<pair<string, int> > AA){
	vector<vector<Edge *> > WOW;
	vector<string> positive, negtive;
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
	//cout << positive.size() << "   " << negtive.size() << endl;
	
	Edge* abc;
	do{
		for (int i = 0; i < positive.size(); i++) {
			WOW.push_back( vector<Edge*>() );
			for (int j = 0; j <positive.size(); ++j){
			//	abc = nm->get_edge( nm->get_node(positive.at(j)), nm->get_node(negtive.at(j)) );
				WOW.at(i).push_back( abc );
			}
		}
	}while(next_permutation(positive.begin(), positive.end()));
	for (auto &e : WOW){
		for (auto &ee : e){
			cout << ee->head->name << ' ' << ee->tail->name << "      ";
		}
		cout << endl;
	}

};

int main(int argc, char** argv){

	nm->interpret("test.txt");
	ofstream ofs("fuckyou.txt");
	//vector<string> NN = node_name(nm->get_all_nodes());
	vector<pair<string, int> > AA = vertex_degree_check(nm);
//	for(auto &e: AA)
//		cout << e.first << ' ' << e.second << endl;
	add_edge_to_be_balanced(nm, AA);













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
