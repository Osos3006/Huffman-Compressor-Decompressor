#include "huffman.h"

void huffman::create_node_array()
{
	for (int i = 0; i < 128; i++)
	{
		node_array[i] = new node;
		node_array[i]->index = i;
		node_array[i]->freq = 0;
	}
}

void huffman::traverse(node_ptr node, string code)
{
	if (node->left == NULL && node->right == NULL)
	{
		node->huffman_code = code;
	}
	else
	{
		traverse(node->left, code + '0');
		traverse(node->right, code + '1');
	}
}

int huffman::binary_to_decimal(string& in)
{
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}

string huffman::decimal_to_binary(int in)
{
	string temp = "";
	string result = "";
	while (in)
	{
		temp += ('0' + in % 2);
		in /= 2;
	}
	result.append(8 - temp.size(), '0');													//append '0' ahead to let the result become fixed length of 8
	for (int i = temp.size() - 1; i >= 0; i--)												
	{
		result += temp[i];
	}
	return result;
}

inline void huffman::build_branch(string& path, char h_code)
{//build a new branch according to the input code and ignore the already existed branches
	node_ptr current = root;
	for (int i = 0; i < path.size(); i++)
	{
		if (path[i] == '0')
		{
			if (current->left == NULL)
				current->left = new node;
			current = current->left;
		}
		else if (path[i] == '1')
		{
			if (current->right == NULL)
				current->right = new node;
			current = current->right;														 
		}
	}
	current->index = h_code;																	//attach index to the leaf
}

huffman::huffman(string source, string target)
{
	source_file_name = source;
	target_file_name = target;
	create_node_array();
}
void huffman::create_pq()
{
	source_file.open(source_file_name, ios::in);
	source_file.get(index);
	while (!source_file.eof())
	{
		node_array[index]->freq++;
		msg_length++;
		source_file.get(index);

	}
	source_file.close();
	for (int i = 0; i < 128; i++)
	{
		if (node_array[i]->freq)
		{
			pq.push(node_array[i]);
		}
	}
}


//create the huffman tree with highest frequency character being leaf from bottom to top
void huffman::create_huffman_tree()
{
	priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
	while (temp.size() > 1)
	{
		root = new node;
		root->freq = 0;
		root->left = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		root->right = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		temp.push(root);
	}
}

void huffman::calculate_huffman_codes()
{
	traverse(root, "");
}

void huffman::compress()
{
	source_file.open(source_file_name, ios::in);
	target_file.open(target_file_name, ios::out | ios::binary);
	string in = "", s = "";

	in += (char)pq.size();																	//the first byte saves the size of the priority queue
	priority_queue<node_ptr, vector<node_ptr>, compare> temp(pq);
	while (!temp.empty())
	{//get all characters and their huffman codes for output
		node_ptr current = temp.top();
		in += current->index;
		s.assign(127 - current->huffman_code.size(), '0'); 									//set the codes with a fixed 128-bit string form[00000.. 1 + real code]
		s += '1';																			//'1' indicates the start of huffman code
		s.append(current->huffman_code);
		string sub = s.substr(0, 8);
		in += (char)binary_to_decimal(sub);										
		for (int i = 0; i < 15; i++)
		{//cut into 8-bit binary codes that can convert into saving char needed for binary file
			s = s.substr(8);
			sub = s.substr(0, 8);
			in += (char)binary_to_decimal(sub);
		}
		temp.pop();
	}
	s.clear();

	source_file.get(index);
	while (!source_file.eof())
	{//get the huffman code
		s += node_array[index]->huffman_code;
		while (s.size() > 8)
		{//cut into 8-bit binary codes that can convert into saving char needed for binary file
			string sub = s.substr(0, 8);
			in += (char)binary_to_decimal(sub);
			s = s.substr(8);
		}
		source_file.get(index);
	}
	int count = 8 - s.size();
	if (s.size() < 8)
	{//append number of 'count' '0' to the last few codes to create the last byte of text
		s.append(count, '0');
	}
	in += (char)binary_to_decimal(s);															//save number of 'count' at last
	in += (char)count;

	target_file.write(in.c_str(), in.size());
	source_file.close();
	target_file.close();
}

void huffman::recreate_huffman_tree()
{
	source_file.open(source_file_name, ios::in | ios::binary);
	unsigned char size;																			//unsigned char to get number of node of huffman tree
	source_file.read(reinterpret_cast<char*>(&size), 1);
	root = new node;
	for (int i = 0; i < size; i++)
	{
		char h_code;
		unsigned char h_code_c[16];																//16 unsigned char to obtain the binary code
		source_file.read(&h_code, 1);
		source_file.read(reinterpret_cast<char*>(h_code_c), 16);
		string h_code_s = "";
		for (int i = 0; i < 16; i++)
		{//obtain the oringinal 128-bit binary string
			h_code_s += decimal_to_binary(h_code_c[i]);
		}
		int j = 0;
		while (h_code_s[j] == '0')
		{//delete the added '0000.. 1' to get the real huffman code
			j++;
		}
		h_code_s = h_code_s.substr(j + 1);
		build_branch(h_code_s, h_code);
	}
	source_file.close();
}

void huffman::decompress()
{
	source_file.open(source_file_name, ios::in | ios::binary);
	target_file.open(target_file_name, ios::out);
	unsigned char size;																		//get the size of huffman tree
	source_file.read(reinterpret_cast<char*>(&size), 1);
	source_file.seekg(-1, ios::end);															//jump to the last one byte to get the number of '0' append to the string at last
	char count0;
	source_file.read(&count0, 1);
	source_file.seekg(1 + 17 * size, ios::beg);													//jump to the position where text starts

	vector<unsigned char> text;
	unsigned char textseg;
	source_file.read(reinterpret_cast<char*>(&textseg), 1);
	while (!source_file.eof())
	{//get the text byte by byte using unsigned char
		text.push_back(textseg);
		source_file.read(reinterpret_cast<char*>(&textseg), 1);
	}
	node_ptr current = root;
	string path;
	for (int i = 0; i < text.size() - 1; i++)
	{//translate the huffman code
		path = decimal_to_binary(text[i]);
		if (i == text.size() - 2)
			path = path.substr(0, 8 - count0);
		for (int j = 0; j < path.size(); j++)
		{
			if (path[j] == '0')
				current = current->left;
			else
				current = current->right;
			if (current->left == NULL && current->right == NULL)
			{
				target_file.put(current->index);
				current = root;
			}
		}
	}
	source_file.close();
	target_file.close();
}


void huffman::calculate_compression_ratio()

	{
		
		for (int i = 0 ; i < 128 ; i++)
		{
				average_L += (node_array[i]->freq/(double)msg_length) * (node_array[i]->huffman_code.size());
		}

	
		cout << "The compression ratio = " << average_L/8.0 << endl;


	}

	void huffman::calculate_coding_efficiency()
	{
		float entropy = 0;
		float l; 
		for (int i = 0 ; i < 128 ; i++)
		{
			if (node_array[i]->freq > 0)
			{
				l = -log2(node_array[i]->freq/(double)msg_length);
				entropy += (node_array[i]->freq/(double)msg_length) * l ;
			}
		}

		cout << "The efficiency = " << entropy/average_L  << endl;
	}



