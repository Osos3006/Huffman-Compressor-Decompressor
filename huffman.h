#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include<iostream>
#include<cmath>
using namespace std;

struct node
{
	char index;																				//character 
	int freq;																				//frequency of the character
	string huffman_code;																	//huffman code for the character
	node* left;
	node* right;

	node()
	{//constructrr
		left = right = NULL;
	}
};
typedef node* node_ptr;

class huffman
{
private:
	node_ptr node_array[128];																//array for 128characters in the Ascii Table
	fstream source_file, target_file;
	node_ptr child, parent, root;
	char index;
	float average_L = 0; 
	long msg_length = 1;
	string source_file_name, target_file_name;
	class compare
	{// comparing rule of priority queue operator (maximum heap)
	public:
		bool operator()(const node_ptr& c1, const node_ptr& c2) const
		{
			return c1->freq > c2->freq;
		}
	};
	priority_queue<node_ptr, vector<node_ptr>, compare> pq;									
	void create_node_array();																
	void traverse(node_ptr, string);														
	int binary_to_decimal(string&);															 
	string decimal_to_binary(int);															
	inline void build_branch(string&, char);											   

public:
	huffman(string, string);
	void create_pq();
	void create_huffman_tree();
	void calculate_huffman_codes();
	void compress();
	void decompress();
	void recreate_huffman_tree();
	void calculate_compression_ratio();
	void calculate_coding_efficiency();
};

#endif
