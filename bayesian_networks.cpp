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
    vector<int> u_x;
    vector<int> u_y;

public:
    Data(string file)
    {
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

        for(int i=0; i<data.size(); i++)
            for(int j=0; j<data.size(); j++)
            {
                u_x.push_back(i);
                u_x.push_back(j);
            }
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

int main()
{
    Network alarm = read_network();
    alarm.print();
    Data data("records.dat");
    data.print();
}