#include <bits/stdc++.h>
using namespace std;
#define vvs vector<vector<string> >
#define vf vector<float>
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
    void init_cpt(float c){
         float column_size = 1.0;
        for(int i=0;i< parents.size(); i++)
            column_size *= parents[i]->get_num_values();

        for (int i=0; i<column_size; i++)
        {
            int j=i;
            while(j<cpt.size())
            {

                cpt[j] = c/(c*num_values);
                j += column_size;
            }
        }
    }
    string get_name(){
        return this->name;
    }

    int get_index(){
        return this->index;
    }

    float get_cpt_value(int i){
        return this->cpt[i];
    }

    vector<string>& get_values(){
        return this->values;
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

    vector<Node *>& get_childs()
    {
        return childrens;
    }

    void add_child(Node* child){
        (this->childrens).push_back(child);
    }

    void set_cpt(vector<float> cpt){
        this->cpt = cpt;
    }
    vf& get_cpt()
    {
        return this->cpt;
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

    vf& getcpt()
    {
        return cpt;
    }
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
    Node* get_node(int i)
    {
        return &nodes[i];
    }
    void print(){
        for(int i=0; i<nodes.size(); i++)
        {
            nodes[i].print();
            nodes[i].print_cpt();
        }
    }
    vector<Node>& get_nodes()
    {
        return this->nodes;
    }
};

class Data{
private:
    vvs data;
    vf weights;
    vector<int> u_x;
    vector<int> u_y;
public:
    Data(vvs data, vf weights){
        this->data = data;
        this->weights = weights;
    }
    Data(string file)
    {
        cout<<"Reading Data\n";
        ifstream infile(file);
        string line;
        if(infile.is_open())
        {   
            int i=0;
            while(getline(infile, line))
            {
                  stringstream ss;
                  ss.str(line);
                  vector<string> temp;
                  string value;
                  int j = 0;
                  while(ss >> value)
                  {
                    if(value == "\"?\"")
                    {
                        u_x.push_back(i);
                        u_y.push_back(j);
                    }
                    temp.push_back(value);
                    j+=1;
                  }
                  (this->data).push_back(temp);
                  (this->weights).push_back(1);
                  i++;
            }
            infile.close();
        }
    }
    vvs& get_data(){
        return this->data;
    }  
    vf& get_weights(){
        return this->weights;
    }
    vector<int>& get_ux()
    {
        return this->u_x;
    }
    vector<int>& get_uy()
    {
        return this->u_y;
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


Network read_network(char *file)
{
	Network Alarm;
	string line;
	int find=0;
  	ifstream myfile(file); 
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

void write_network(Network& net, char* file)
{
    Network Alarm;
    string line;
    int find=0;
    ifstream myfile(file); 
    ofstream outfile("solved_alarm.bif");
    string temp;
    string name;

    if(myfile.is_open())
    {
        cout<<"Wrting Network\n";
        while(!myfile.eof())
        {
            stringstream ss;
            getline (myfile,line);
            ss.str(line);
            ss>>temp;
           
            if(temp.compare("probability")==0)
            {
                    ss>>name;
                    ss>>name;
                    outfile << line <<endl;
                    getline (myfile,line);
                    Node* node = net.search_node(name);
                    outfile<<"\ttable ";
                    vf &cpt = node->getcpt();
                    for(int i=0;i<cpt.size(); i++)
                        outfile<<setprecision(4)<<std::fixed<<cpt[i]<<" ";
                    outfile<<";\n";
            }       
            else if(line == "")
                outfile<<line;
            else
                outfile<<line<<endl;
        }
        myfile.close();
    }   
}

float get_prob_given_par(int line, Node& node, int value, Data& data)
{
    vvs& d = data.get_data();
    vector<int> parents_index;
    vector<int> parents_num_values;
    vector<Node*>& parents = node.get_Parents();
    for(int i=0; i<parents.size(); i++)
    {
        parents_index.push_back(parents[i]->get_index());
        parents_num_values.push_back(parents[i]->get_num_values());
    }

    int index = 0;
    int old = 1;
    for(int j=parents_num_values.size()-1; j>=0; j--)
    {
        string value_of_parj = d[line][parents_index[j]];
        index += parents[j]->get_value_index(value_of_parj)*old;
        old *= parents_num_values[j];
    }
    index += value*old;

    return node.get_cpt_value(index);
}

string probs(int line, Node& node, Data& data)
{
    vvs &d = data.get_data();
    vector<Node*> &childs = node.get_childs();
    vector<string>& vals = node.get_values();
    
    vector<float> weights;
    float sum = 0.0;
    for(int i=0; i<vals.size(); i++)
    {
        float prob = get_prob_given_par(line, node, i, data);
        d[line][node.get_index()] = vals[i];

        for(int j=0; j<childs.size(); j++)
        {
            Node child = *childs[j];
            string v = d[line][child.get_index()];
            prob = prob * get_prob_given_par(line, child, child.get_value_index(v), data);
        }
        weights.push_back(prob);
        sum += prob;
    }
    int max_i = -1;
    float max_prob = -1;
    for(int i=0; i<weights.size(); i++)
    {
        if(weights[i] > max_prob)
        {
            max_i = i;
            max_prob = weights[i];
        }
    }
    return vals[max_i];
}

void update_data(Data& data, Network &net)
{
    vvs& d = data.get_data();
    vector<int>& u_x = data.get_ux();
    vector<int>& u_y = data.get_uy();
    for(int i = 0 ;i<u_x.size();i++)
    {
        int x_u = u_x[i];
        int y_u = u_y[i];

        Node *node = net.get_node(y_u);
        vector<string>& vals = node->get_values();

        int max_i = -1;
        float max_prob = -1;     
        d[x_u][y_u] = probs(i, *node, data);
    }
}



vf calculate_probs(int i, Node& node, Data& data, int loc_mark, Network& net)
{
    vvs& d = data.get_data();
    vector<float> weights(0);
    float probs_sum = 0.0;
    
    vector<string> &values = node.get_values();
    
    for(int j=0; j< values.size();j++)
    {
        float prob = get_prob_given_par(i, node, j, data);
        
        vector<Node*> &child = node.get_childs();
        d[i][node.get_index()] = values[j];
        
        for(int k = 0; k<child.size(); k++)
        {
            string x = d[i][child[k]->get_index()];

            prob *= get_prob_given_par(i,*child[k], child[k]->get_value_index(x), data);
        }
        probs_sum += prob;
        weights.push_back(prob);
    }
    for(int j=0 ;j<weights.size(); j++)
        weights[j] = weights[j]/probs_sum;

    return weights;
}

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
    
    float column_size = 1.0;
    for(int i=0;i< parents_num_values.size(); i++)
        column_size *= parents_num_values[i];

    for(int i=0; i<d.size(); i++)
    {
        int index = 0;
        int old = 1;
        bool mark = false;
        for(int j=parents_num_values.size()-1; j>=0; j--)
        {
            string value_of_parj = d[i][parents_index[j]];
            index += parents[j]->get_value_index(value_of_parj)*old;
            old *= parents_num_values[j];
        }

        string my_value = d[i][node.get_index()];
        index += node.get_value_index(my_value)*old;
        new_cpt[index] ++;
    }
    
    vector<int> sum_counts(column_size, 0);
    for (int i=0; i<sum_counts.size(); i++)
    {
        int j=i;
        while(j<new_cpt.size())
        {
            sum_counts[i] += new_cpt[j];
            j += column_size;
        }
    }
    
    for (int i=0; i<sum_counts.size(); i++)
    {
        int j=i;
        while(j<new_cpt.size())
        {
            new_cpt[j] = (new_cpt[j] + c)/(sum_counts[i] + c*node.get_num_values());
            j += column_size;
        }
    }
    node.set_cpt(new_cpt);
}

void update_network(Network &alarm, Data &data)
{
    vector<Node>& nodes = alarm.get_nodes();

    for(int i=0; i<nodes.size(); i++)
        update_cpt(nodes[i], data, 0.001);
}

float max_change(Network &old_net, Network &new_net)
{
    vector<Node>& old_nodes = old_net.get_nodes();
    vector<Node>& new_nodes = new_net.get_nodes();
    float change = 0.0;

    for(int i=0; i<old_nodes.size(); i++)
    {
        vf& old_cpt = old_nodes[i].get_cpt();
        vf& new_cpt = new_nodes[i].get_cpt();

        for(int j=0; j<old_cpt.size(); j++)
        {
            if (abs(old_cpt[j] - new_cpt[j]) > change)
                change = abs(old_cpt[j] - new_cpt[j]);
        }
    }
    return change;
}

void em(Network &alarm, Data &data, float threshold)
{
    int i = 0;
    time_t begin_time = time(NULL);
    while(true )
    {
        i++;
        printf("Step: %d\n", i);
        update_data(data, alarm);
        Network old_alarm(alarm);
        update_network(alarm, data);
        float change = max_change(old_alarm, alarm);

        // run untill threshold reached or time expired
        time_t curr_time = time(NULL);
        if(change < threshold || (curr_time - begin_time) > 60*9.5)
        {
            break;
        }
    }
}

void init(Network &alarm, Data &data)
{
    vector<Node>& nodes = alarm.get_nodes();
    int n = data.get_data().size();
    for(int i=0; i<nodes.size(); i++)
    {
        nodes[i].init_cpt(0.001);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {        
        cout<<"Program expects two commandline arguments";
        exit(0);
    }
    Network alarm = read_network(argv[1]);
    Data data(argv[2]);
    init(alarm, data);
    cout<<"Running EM\n";
    em(alarm, data, 0.0000000001);
    write_network(alarm, argv[1]);
}