#include <bits/stdc++.h>

#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;
typedef long long int ll;

long double toRadians(const long double degree)
{
	//We use M_PI as the value of pi, cmath library in C++ defines it accurate to 1e-30
	long double one_deg = (M_PI) / 180;
	return (one_deg * degree);
}
long double distance(long double lat1, long double long1, long double lat2, long double long2)
{
	//Converting the latitudes and longitudes from degree to radians.
	lat1 = toRadians(lat1);
	long1 = toRadians(long1);
	lat2 = toRadians(lat2);
	long2 = toRadians(long2);

	// For Haversine Formula
	long double dlong = long2 - long1;
	long double dlat = lat2 - lat1;

	long double ans = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlong / 2), 2);

	ans = 2 * asin(sqrt(ans));

	//Radius of Earth is taken to be 6371 kilometers.
	long double R = 6371;

	ans = ans * R;

	return ans;
}

//id, <id, dist>
map<ll, vector<pair<ll, double>>> mp;

void addEdge(ll u, ll v, double dist)
{
	mp[u].push_back(make_pair(v, dist));
	mp[v].push_back(make_pair(u, dist));
	
}

void shortestPath(ll src,unordered_map<ll, double> &dist)
{	
	dist[src] = 0;
    //We create a set to store vertices that are being processed
    set< pair<double, ll> > st;
	
    //We insert source itself in Set and initialize its distance as 0.
    st.insert(make_pair(0, src));
   
    while (!st.empty())
    {	
        //We extract first vertex from the set which is the minimum distance vertex.
        pair<double, ll> tmp = *(st.begin());

		/* First element is distance so that all the vertices are stored in 
		   sorted order of distance */
        ll u = tmp.second;

        st.erase(st.begin());
  
		const vector<pair<ll, double>> &edges = mp[u];
		for (const pair<ll, double> &i : edges)
        {
            ll v = i.first;
            double weight = i.second;
  
            //We check If there is shorter path to v through u.
            if (dist[v] > dist[u] + weight)
            {
                /*  If distance of v is not INF then it is in our set, so we remove it and insert it 
					with updated shorter distance.  */
                if (dist[v] != LONG_MAX)
                    st.erase(st.find(make_pair(dist[v], v)));

                dist[v] = dist[u] + weight;
                st.insert(make_pair(dist[v], v));
            }
        }
    }
}

int main(void)
{
	cout << "Parsing data (map.xml).....\n\n";
	xml_document<> doc;
	xml_node<> *root_node;

	// Reading the xml file into a vector
	ifstream theFile("map.osm");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parsing the buffer using the xml file parsing library into doc
	doc.parse<0>(&buffer[0]);

	// Finding our root node
	root_node = doc.first_node("osm");

	ll s_id, d_id, p_id;
	double p_lat, p_lon;
	cout << "Enter the id of the first node : ";
	cin >> s_id;
	cout << "Enter the id of the second node : ";
	cin >> d_id;

	bool flag = true;

	unordered_map<ll, double> dist;
	unordered_map<ll, pair<double, double>> nodes_data;
	for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
	{
		string str = _node->name();

		if (str == "node")
		{
			ll id = stoll(_node->first_attribute("id")->value());

			double lat = stod(_node->first_attribute("lat")->value());
			double lon = stod(_node->first_attribute("lon")->value());

			nodes_data[id] = (make_pair(lat, lon));

			dist[id]=LONG_MAX;
		}
	}
	
	//cout<<nodes_data.size()<<endl;

	for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
	{
		string str = _node->name();
		//cout << "LINE : " << __LINE__ << " " << str << '\n';

		if (str == "way")
		{
			//cout<<"a: " << _node -> first_attribute("id") -> value() <<endl;
			for (xml_node<> *p_node = _node->first_node(); p_node; p_node = p_node->next_sibling())
			{	
				//cout<<cnt << " :: " << p_node -> name() <<endl;
				if(strcmp((p_node -> name()), "nd")){
					break;
				}
				ll id = stoll(p_node->first_attribute("ref")->value());
				double lat = nodes_data[id].first;
				double lon = nodes_data[id].second;

				if (flag)
				{
					p_id = id;
					p_lat = lat;
					p_lon = lon;
					flag = false;
					continue;
				}

				double cfDist = distance(p_lat, p_lon, lat, lon);


				addEdge(p_id, id, cfDist);
				p_id = id;
				p_lat = lat;
				p_lon = lon;
			}
		}
	}
	
	shortestPath(s_id,dist);
	if (dist.find(d_id) == dist.end())
		cout << "No road exists between the given nodes.\n";
	else {
		cout << "Shortest distance between the two nodes is "<<dist[d_id]<<" kms.\n";
	}
	return 0;
}