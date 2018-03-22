#include <bits/stdc++.h>
using namespace std;
#define vvs vector<vector<string> >

class Node{

private:
	string name;
	vector <string> values;
	int num_values;
	vector<Node* > parents;
	vector<Node* > childrens;
	vector<float> cpt;;
    int index;
    map<string, int> values_n;
public:
	Node(string name, vector<string> values){
		this->name = name;
		this->values = values;
		this->num_values = values.size();
        for(int i=0; i<this->values.size();i++)
            values_n[values[i]] = i;
	}

    int get_value_index(string v)
    {
        return values_n[v];
    }

    string get_name(){
        return this->name;
    }

    int get_index(){
        return this->index;
    }


    int get_num_values(){
        return this->num_values;
    }

    void set_Parents(vector<Node*> parents){
        this->parents = parents;
    }

    vector<Node *>& get_Parents()
    {
        return parents;
    }

    void add_child(Node* child){
        (this->childrens).push_back(child);
    }

    void set_cpt(vector<float> cpt){
        this->cpt = cpt;
    }
    void print(){
        cout<<this->name<<":";
        for(int i=0; i< parents.size(); i++)
            cout<<parents[i]->get_name()<<" ";
        cout<<endl;;
    }
    void set_index(int index){
        this->index = index;
    }

    void print_cpt(){
        for(int i=0;i<cpt.size(); i++)
            cout<<cpt[i]<<" ";
        cout<<endl;;
    }
	// TODO: add functions as needed
};

class Network{
private:
	vector<Node> nodes;
public:
    void addNode(Node node)
    {
        (this->nodes).push_back(node);
        nodes[nodes.size()-1].set_index(nodes.size()-1);
    }

    Node* search_node(string name){
        for(int i=0 ;i < nodes.size(); i++)
            if(nodes[i].get_name() == name)
                return &nodes[i];

        cout<< "Can't find the node";
    }

    void print(){
        for(int i=0; i<nodes.size(); i++)
            nodes[i].print();
    }
};

class Data{

private:
    vvs data;
public:
    Data(string file)
    {
        cout<<"Reading Data\n";
        ifstream infile(file);
        string line;
        if(infile.is_open())
        {
            while(getline(infile, line))
            {
                  stringstream ss;
                  ss.str(line);
                  vector<string> temp;
                  string value;
                  while(ss >> value)
                    temp.push_back(value);

                  (this->data).push_back(temp);

            }
            infile.close();
        }
    }
    vvs& get_data(){
        return this->data;
    }
    void print()
    {
        for(int i=0; i<data.size();i++)
        {
            for(int j=0; j<data[i].size(); j++)
                cout<<this->data[i][j];
            cout<<endl;
        }
    }

};


Network read_network()
{
	Network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
    vector<string> values;
  	vector<Node*> evidence;

    if(myfile.is_open())
    {
        cout<<"Reading Network\n";
    	while(!myfile.eof())
    	{
    		stringstream ss;
      		getline (myfile,line);
      		ss.str(line);
     		ss>>temp;
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
                    Node new_node(name, values);
     				Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
     				Node* target = Alarm.search_node(temp);
                    ss>>temp;
                    evidence.clear();
     				while(temp.compare(")")!=0)
     				{
                        evidence.push_back(Alarm.search_node(temp));
     					ss>>temp;
    				}

                    target->set_Parents(evidence);

                    for(int i=0; i < evidence.size(); i++)
                        evidence[i]->add_child(target);

    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> cpt;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
     					cpt.push_back(atof(temp.c_str()));
     					ss2>>temp;
    				}
                    target->set_cpt(cpt);
     		}		
    	}
    	myfile.close();
  	}
  	
  	return Alarm;
}

// void calc_prob_for_missing()
// {
//     return 0.5;
// }
void update_cpt(Node& node, Data& data, float c)
{
    vvs& d = data.get_data();

    vector<int> parents_index;
    vector<int> parents_num_values;

    vector<Node*>& parents = node.get_Parents();
    
    int cpt_size = node.get_num_values();
    for(int i=0; i<parents.size(); i++)
    {
        parents_index.push_back(parents[i]->get_index());
        parents_num_values.push_back(parents[i]->get_num_values());
        cpt_size *= parents_num_values[i];
    }

    vector<float> new_cpt(cpt_size, 0.0);
    
    for(int i=0; i<d.size(); i++)
    {
        int index = 0;
        int old = 1;
        for(int j=parents_num_values.size()-1; j>=0; j--)
        {
            // Todo what if value_of_parj = ?
            string value_of_parj = d[i][parents_index[j]];
            index += parents[j]->get_value_index(value_of_parj)*old;
            old *= parents_num_values[j];
            
            cout<<parents[j]->get_name()<<" : "<<parents_num_values[j]<<" : "<<parents[j]->get_value_index(value_of_parj)<<endl;
        }

        string my_value = d[i][node.get_index()];
        index += node.get_value_index(my_value)*old;
        cout<<node.get_name()<<" : "<<node.get_index()<<" : "<<node.get_value_index(my_value)<<endl;

        new_cpt[index] ++;
    }
    int total = d.size();
    for(int i=0; i<new_cpt.size(); i++)
        new_cpt[i] = (new_cpt[i] + c)/(c*total);

    node.set_cpt(new_cpt);

}
// void maximization(Network &alarm, Data &data)
// {

// }
int main()
{
    Network alarm = read_network();
    Data data("records.dat");
    Node* test = (alarm.search_node("\"LVEDVolume\""));
    update_cpt(*test, data, 1.0);
    test->print_cpt();
}