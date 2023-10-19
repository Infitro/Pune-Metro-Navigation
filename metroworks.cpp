//run using g++ -std=c++11 Dijkstras.cpp
#include<bits/stdc++.h>
//#include <graphviz/gvc.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
using namespace std;
template<typename T>
class Graph
{

    map<T, list<pair<T,float> > > m;
public:
    list<T> path;
    void addEdge(T u,T v,float dist,bool bidir=true)
    {
        m[u].push_back(make_pair(v,dist));
        if(bidir)
        {
            m[v].push_back(make_pair(u,dist));
        }
    }

    void DijkstraGetShortestPathTo(T destination, map<T,T> &prev)
    {

        for( ; destination!=""; destination = prev[destination])
        {
            path.push_back(destination);
        }
        path.reverse();
        cout<<"\t\t\t";
        copy(path.begin(), path.end(), ostream_iterator<T>(cout, "\n\t\t\t"));
    }
    bool printAdj(string,string);
    void dijsktraSSSP(T,map<T,float> &dist, map<T,T> &prev);
    void makedotfile();
    vector<string> split (const string &s, char delim)
    {
        vector<string> result;
        stringstream ss (s);
        string item;

        while (getline (ss, item, delim))
        {
            result.push_back (item);
        }

        return result;
    }
    bool check(string, string);
    void calcPrice(string, string);
};

// FARE CALC FUNCTION STARTS

template<typename T>
void Graph<T>::calcPrice(string srstn,string dstn)
{
    string line;
    int i=0,j=0,k=0,len,last=0;
    string num = "";
    long arr[235][235];
									    ifstream infile ("fare1.csv");

    if(infile.is_open())
    {
        while(getline(infile,line) )
        {

            // Takes complete row
            // cout << line<< '\n';

            k=0,last=0,j=0;
            len=line.length();
            while(k!=len)
            {
                if(line[k]==','||k==len-1)
                {

                    //for converting string into number
                    arr[i][j]=atof(num.append(line,last,k).c_str());
                    //cout<<"new entry -> "<<arr[i][j]<<" endl \n";

                    //Emtying string for getting next data
                    num="";

                    //increasing column number after saving data
                    j++;

                    if(k!=len-1)
                        last=k+1;
                }
                k++;
            }
            //increase row number for array
            i++;
        }
        //close the file
        infile.close();
    }
    else cout << "Unable to open file";

    string line1;
    int a,b;
    a=0;
    b=0;
							    ifstream file("stations.txt");
    int f=0;
    while(!file.eof())
    {
        getline(file,line1);
        vector<string> v1=split(line1,'\t');
        if(v1[1]==srstn)
        {
            f++;
            std::istringstream(v1[0]) >> a;

        }
        if(v1[1]==dstn)
        {
            f++;
            std::istringstream(v1[0]) >> b;
        }
        if(f==2)
            break;
    }
    //cout<<"Id of "<<srstn<<" is "<<a<<endl;
    //cout<<"Id of "<<dstn<<" is "<<b<<endl;
    cout<<endl<<"\t\t\t";
    cout<<"--> Fare is: र"<<arr[a-1][b-1]<<endl;
}

// FARE CALCULATION ENDS HERE

template<typename T>
bool Graph<T>::check(string src, string dest)
{
    int f=0;
    list <string> :: iterator it;
    for (it = path.begin(); it != path.end(); ++it)
    {
        if(*it==src)
        {
            f++;
        }
        else if(*it==dest)
        {
            f++;
        }
    }
    if(f==2)
        return true;
    else
        return false;
}



template<typename T>
void Graph<T>::makedotfile()
{
    string a,b,clr;
    string labl;
    int f;
    f=0;
    clr="red";
    char filename[] = "finaltest.dot";
    string delimiter=",";
    ofstream fout(filename);
    fout<<"graph G {"<<endl;
    fout<<"node [shape=rect,dpi=600] margin=0.75"<<endl;
    fout<<"\n//"<<clr<<endl;
    string x;
									    ifstream file("data.txt");
    while (getline(file,x))
    {
        
        vector<string> v = split(x,',');
        a=v[0];
        b=v[1];
        labl=v[2];
        if(f==1)
        {
            fout<<"\n//"<<clr<<endl;
            f=0;
        }

        if(check(a,b)==true)
            fout<<"\""<<a<<"\""<<" -- "<<"\""<<b<<"\""<<" "<<"[label=\""<<labl<<"\",color="<<clr<<" "<<",penwidth=\"5\"];"<<endl;
        else
            fout<<"\""<<a<<"\""<<" -- "<<"\""<<b<<"\""<<" "<<"[label=\""<<labl<<"\",color="<<clr<<" "<<",penwidth=\"2\"];"<<endl;
        if(a=="Mandai" && f==0)
        {
            f=1;
            clr="blue";
        }
        else if(a=="Kalyani Nagar" && f==0)
        {
            f=1;
            clr="yellow";
        }
    }
    fout<<"}";
}

template<typename T>
bool Graph<T>::printAdj(string src,string dest)
{
    int f;f=0;
    //Let try to print the adj list
    //Iterate over all the key value pairs in the map
    for(auto j:m)
    {
    if(j.first==src)
        f++;
        //Iterater over the list of cities
        for(auto l:j.second)
        {

            if(l.first==dest)
            {
                f++;
            }
        }
    }
    if(f>1)
        return true;
    else
        return false;

}

template<typename T>
void Graph<T>::dijsktraSSSP(T src, map<T,float> &dist, map<T,T> &prev)
{

    set<pair<float, T> > s;
    //Set all distance to infinity
    prev.clear();
    for(auto j:m)
    {
        dist[j.first] = numeric_limits<float>::infinity();
        prev[j.first] = "";
    }

    //Make a set to find a out node with the minimum distance

    dist[src] = 0;
    s.insert(make_pair(0,src));

    while(!s.empty())
    {

        //Find the pair at the front.
        auto p =   *(s.begin());
        T node = p.second; // NODE

        float nodeDist = p.first; //NODEDIST
        s.erase(s.begin());
        //Iterate over neighbours/children of the current node
        for(auto childPair: m[node])
        {
            T dest = childPair.first;
            float weight = childPair.second;
            float distance_through_node = nodeDist + childPair.second;

            if(distance_through_node < dist[childPair.first])
            {
                //In the set updation of a particular is not possible
                // we have to remove the old pair, and insert the new pair to simulation updation

                auto f = s.find( make_pair(dist[dest],dest));
                if(f!=s.end())
                {
                    s.erase(f);
                }
                //Insert the new pair
                dist[dest] = distance_through_node;
                prev[dest] = node;
                s.insert(make_pair(dist[dest],dest));
            }
        }
    }
    //Lets print distance to all other node from src
    /*for(auto d:dist)
    {
        cout<<d.first<<",is located at distance of  "<<d.second<<endl;
    }*/
}

int main()
{					   system("notify-send -t 5000 -i /home/nightwarrior-xxx/Documents/MetroWorksDS/train1.png \"Metro Works\"");
 								   system("gnome-terminal -x sh -c \"fim --autowindow punegraph.png\"");
    system("clear");
    //system("printf '\e[45;5;196m'");
    //system("echo  \"\e[96m\"");
    system("sl -alfe");
    system("clear");

    cout<<"\t\t"<<" __  __      _              __        __         _ "<<endl;
    cout<<"\t\t"<<"|  \\/  | ___| |_ _ __ ___   \\ \\      / /__  _ __| | _____"<<endl;
    cout<<"\t\t"<<"| |\\/| |/ _ \\ __| '__/ _ \\   \\ \\ /\\ / / _ \\| '__| |/ / __|"<<endl;
    cout<<"\t\t"<<"| |  | |  __/ |_| | | (_) |   \\ V  V / (_) | |  |   <\\__ \\ " <<endl;
    cout<<"\t\t"<<"|_|  |_|\\___|\\__|_|  \\___/     \\_/\\_/ \\___/|_|  |_|\\_\\___/"<<endl;
    system("echo  \"\e[96m\"");
    //system("figlet Metro Works");
    string source, destination;
    Graph<string> Metro;
	Metro.addEdge("PCMC","Sant Tukaram Nagar",2.4);
	Metro.addEdge("Sant Tukaram Nagar","Nashik Phata",3.3);
	Metro.addEdge("Nashik Phata","Kasarwadi",2.1);
	Metro.addEdge("Kasarwadi","Phugewadi",1.1);
	Metro.addEdge("Phugewadi","Dapodi",2.2);
	Metro.addEdge("Dapodi","Bopodi",1.6);
	Metro.addEdge("Bopodi","Khadki",1);
	Metro.addEdge("Khadki","Range Hill",2);
	Metro.addEdge("Range Hill","Shivaji Nagar",1.5);
	Metro.addEdge("Shivaji Nagar","Civil Court",2);
	Metro.addEdge("Civil Court","Budhwar Peth",1.1);
	Metro.addEdge("Budhwar Peth","Mandai",1.3);
	Metro.addEdge("Mandai","Swargate",1.7);

	Metro.addEdge("Vanaz","Anand Nagar",1);
	Metro.addEdge("Anand Nagar","Ideal Colony",1.1);
	Metro.addEdge("Ideal Colony","Nal Stop",1.1);
	Metro.addEdge("Nal Stop","Garware College",1.5);
	Metro.addEdge("Garware College","Deccan Gymkhana",1.2);
	Metro.addEdge("Deccan Gymkhana","Chhatrapati Sambhaji Udyan",1.3);
	Metro.addEdge("Chhatrapati Sambhaji Udyan","PMC",1);
	Metro.addEdge("PMC","Civil Court",0.8);
	Metro.addEdge("Civil Court","Mangalwar Peth",1.9);
	Metro.addEdge("Mangalwar Peth","Pune Railway Station",0.7);
	Metro.addEdge("Pune Railway Station","Ruby Hall Clinic",0.5);
	Metro.addEdge("Ruby Hall Clinic","Bund Garden",0.9);
	Metro.addEdge("Bund Garden","Yerawada",1);
	Metro.addEdge("Yerawada","Kalyani Nagar",2.4);
	Metro.addEdge("Kalyani Nagar","Ramwadi",1.4);

	Metro.addEdge("Quadron","Infosys Phase II",1.9);
	Metro.addEdge("Infosys Phase II","Dohler",1.2);
	Metro.addEdge("Dohler","Wipro Technologies",2.5);
	Metro.addEdge("Wipro Technologies","Pall India",2);
	Metro.addEdge("Pall India","Shivaji Chowk",1.6);
	Metro.addEdge("Shivaji Chowk","Hinjawadi",0.5);
	Metro.addEdge("Hinjawadi","Wakad Chowk",3.1);
	Metro.addEdge("Wakad Chowk","Balewadi Stadium",3.9);
	Metro.addEdge("Balewadi Stadium","NICMAR",0.6);
	Metro.addEdge("NICMAR","Ramnagar",0.5);
	Metro.addEdge("Ramnagar","Laxmi Nagar",2);
	Metro.addEdge("Laxmi Nagar","Balewadi Phata",2);
	Metro.addEdge("Balewadi Phata","Baner Gaon",0.7);
	Metro.addEdge("Baner Gaon","Baner",1.5);
	Metro.addEdge("Baner","IARI",4);
	Metro.addEdge("IARI","Sakal Nagar",1.5);
	Metro.addEdge("Sakal Nagar","SPPU",1);
	Metro.addEdge("SPPU","RBI",1.1);
	Metro.addEdge("RBI","Agriculture College",1.1);
	Metro.addEdge("Agriculture College","Shivaji Nagar",1.6);

    map<string,float> dist;
    map<string,string> prev;
    string sourcestn, deststn;
    cout<<endl<<endl<<endl;
    cout<<"\t\t";
    cout<<"Enter source station in capital case: ";
    getline(cin,sourcestn);
    //system("echo \"\e[92m\"");
    cout<<endl;
    cout<<"\t\t";
    cout<<"Enter destination station in capital case: ";
    //system("echo \"\e[96m\"");
    getline(cin,deststn);
    bool res=Metro.printAdj(sourcestn,deststn);
    if(res==false)
    {
      system("zenity --error --title \"Error Occured\" --text='Invalid Station Entered'");
      system("clear");
      return 0;
    }

    //system("echo \"\e[92m\"");
    Metro.dijsktraSSSP(sourcestn, dist, prev);
    //system("echo \"\e[96m\"");
    cout<<endl<<"\t\t";
    cout<<"Distance from "<<sourcestn<<" to "<<deststn<<" - "<<dist[deststn]<<" Kms"<<endl;
    cout<<endl<<"\t\tPath: "<<endl;
    Metro.DijkstraGetShortestPathTo(deststn,prev);
    Metro.makedotfile();
    Metro.calcPrice(sourcestn,deststn);
											    system("dot -Tpng finaltest.dot -o punepath.png");
										    system("gnome-terminal  -- sh -c \"fim --autowindow punepath.png\"");
    return 0;
}
