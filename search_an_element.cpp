#include <bits/stdc++.h>

#include "rapidxml.hpp"

using namespace rapidxml;
using namespace std;
typedef long long int ll;

class Node
{
public:
	ll id;
	string Name;
	double lat, lon;

	Node(ll id, string name, double lat, double lon)
	{
		this->id = id;
		this->Name = name;
		this->lat = lat;
		this->lon = lon;
	}
};

//Function to check substring
int Substr(string s2, string s1)
{
	int counter = 0;
	transform(s1.begin(), s1.end(), s1.begin(), ::tolower);
	int n = s1.length(), m = s2.length();
	int i;
	transform(s2.begin(), s2.end(), s2.begin(), ::tolower);
	for (i = 0; i < n; i++)
	{
		if (counter == m)
			break;
		if (s2[counter] != s1[i])
		{
			if (counter > 0)
				i -= counter;
			counter = 0;
		}
		else
			counter++;
	}

	if (counter < m)
		return -1;
	else
		return i - counter;
}
int main(void)
{
	cout << "Parsing data (map.xml)....." << endl;
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

	int nodes_cnt = 0, ways_cnt = 0;
	string str;

	for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
	{
		str = _node->name();

		if (str == "node")
			nodes_cnt++;

		if (str == "way")
			ways_cnt++;
	}

	cout << "Total number of nodes discovered : " << nodes_cnt << "\n";
	cout << "Total number of ways discovered : " << ways_cnt << "\n\n";

	string toFind;
	cout << "Enter an element to search : ";
	cin >> toFind;
	cout << "\n";

	vector<Node> v;
	for (xml_node<> *_node = root_node->first_node(); _node; _node = _node->next_sibling())
	{
		str = _node->name();

		if (str == "node" )
		{
			ll id = stoll(_node->first_attribute("id")->value());
			double lat = stod(_node->first_attribute("lat")->value());
			double lon = stod(_node->first_attribute("lon")->value());
			for (xml_node<> *p_node = _node->first_node(); p_node; p_node = p_node->next_sibling())
			{
				string temp = p_node->first_attribute("k")->value();
				if (temp == "name")
				{
					string n_name = p_node->first_attribute("v")->value();
					if (Substr(toFind, n_name) != -1)
					{
						Node newNode = Node(id, n_name, lat, lon);
						v.push_back(newNode);
					}
				}
			}
		}
	}

	int n = v.size();
	if(n==0)
	cout<<"No element found.\n";

	for (int i = 0; i < n; i++)
	{
		cout << "Name is : " << v[i].Name << "\n";
		cout << "ID is : " << v[i].id << "\n";
		cout << "Latitude is : " << v[i].lat << "\n";
		cout << "Longitude is : " << v[i].lon << "\n";
		cout << "\n";
	}
	return 0;
}
