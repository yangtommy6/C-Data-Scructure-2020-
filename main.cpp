#include <sstream>
#include <fstream>
#include <iostream>
#include "AVL.h"

using namespace std;

int main(int argc, char* argv[]) {

	AVL<int> numbers;

	ifstream in(argv[1]);
	ofstream out(argv[2]);

	for (string line; getline(in, line);) {
		string command, item;
		int integer = 0;
		out << line << " ";
		istringstream iss(line);
		iss >> item;
		if (item == "INT") {
			out << "true " << endl;
		}
		else if (item == "add") {
			iss >> integer;
			if (numbers.addNode(integer)) {
				out << "true " << endl;
			}
			else out << "false " << endl;
		}
		else if (item == "print") {
			out << ": " << numbers.toString() << endl;
		}
		else if (item == "remove") {
			iss >> integer;
			if (numbers.removeNode(integer)) {
				out << "true " << endl;
			}
			else out << "false " << endl;
		}

		else if (item == "clear") {
			if (numbers.deleteTree() == true) {
				out << "true" << endl;
			}
		}
		else if (item == "size") {
			out << numbers.sizeTree() << endl;
		}
		else if (item == "find") {
			iss >> integer;
			out << numbers.findNode(integer) << endl;
		}
		else if (item == "tree") {
			out << ": ";
			if (numbers.sizeTree() == 0) {
				out << "empty" << endl;
			}
			else out << "" << endl;
		}
		else break;
	}
	return 0;
}