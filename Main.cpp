#include <iostream>
#include "huffman.h"
#include<string>
using namespace std;

int main()
{
    int choice;
	string source_file_path, target_file_path;
	cout << "choose 1 for compression and 2 for decompression: ";
    cin >> choice;
	char c; 
	cin.get(c);
    if ( choice == 1 )
		{
            cout << "enter the path of the input file: ";
			getline(cin, source_file_path);
            cout << "enter the path of the output file: ";
            getline(cin,target_file_path);

	huffman h(source_file_path,target_file_path);
	h.create_pq();
	h.create_huffman_tree();
	h.calculate_huffman_codes();
	h.compress();
	h.calculate_compression_ratio();
	h.calculate_coding_efficiency();
	cout << endl;
        }
        else if (choice ==2 )
        {
            
            cout << "enter the path of the input file: ";
            getline(cin,source_file_path);
            cout << "enter the path of the output file: ";
			getline(cin, target_file_path);

   huffman h(source_file_path,target_file_path);
	h.recreate_huffman_tree();
	h.decompress();
	cout << endl;
	
        }

        system("pause");
	return 0;
}
