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

	//Radius of Earth in kilometers, R = 6371
	long double R = 6371;

	ans = ans * R;

	return ans;
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

	ll g_id;
	double g_lat, g_lon;
	cout << "Enter the id of the node : ";
	cin >> g_id;

	int k;
	cout << "Enter the value of k : ";
	cin >> k;

	bool flag = true;

	while (flag)
	{
		for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
		{
			string str = _node->name();

			if (str == "node")
			{
				ll id = stoll(_node->first_attribute("id")->value());
				if (id == g_id)
				{
					g_lat = stod(_node->first_attribute("lat")->value());
					g_lon = stod(_node->first_attribute("lon")->value());
					flag = false;
					break;
				}
			}
		}
		if (flag)
		{
			cout << "\nNo node with given id found. Please enter a valid Node ID.\n\n";
			cout << "Enter the id of the node : ";
			cin >> g_id;
			cout << "Enter the value of k : ";
			cin >> k;
		}
	}
	//We declare a vector of pairs to store distance and id of the node from given node.
	vector<pair<double, ll>> v;
	for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
	{
		string str = _node->name();

		if (str == "node")
		{
			ll id = stoll(_node->first_attribute("id")->value());
			if (id == g_id)
				continue;

			double lat = stod(_node->first_attribute("lat")->value());
			double lon = stod(_node->first_attribute("lon")->value());

			// We calculate the distance ans store it in the vector
			double cfDist = distance(g_lat, g_lon, lat, lon);

			v.push_back(make_pair(cfDist, id));
		}
	}

	//Now we sort the vector and after that we will print the first k distanced nodes
	sort(v.begin(), v.end());

	cout << "\n"<< k << " nearest nodes to the given node are : \n\n";
	for (int i = 0; i < k; i++)
	{
		cout << i + 1 << ">";
		cout << "\nId of node is : " << v[i].second << "\n";
		cout << "Distance is : " << v[i].first << "\n";
		cout << endl;
	}
	return 0;
}
