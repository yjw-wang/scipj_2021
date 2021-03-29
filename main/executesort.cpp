#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdio.h>
#include <map>
#include "writefile.h"
#include "judge.h"
#include "../insertion/insertion.h"
#include "../selection/selection.h"
using namespace std;

#define prl cout<<endl

map<string, long> choose_algo(string in_dir, string algoname, int multi, int cnt, Insertion& ins, Selection& slc) {
	map<string, long> m1;
	if (in_dir.compare("") == 0) {
		if (algoname.compare("insertion") == 0)
			m1 = ins.insertion("..\\insertion\\temp.txt", "..\\insertion\\out.txt", multi, cnt);
		else if (algoname.compare("selection") == 0)
			m1 = slc.selection("..\\selection\\temp.txt", "..\\selection\\out.txt", multi, cnt);	
	}
	else {
		if (algoname.compare("insertion") == 0)
			m1 = ins.insertion(in_dir, "..\\insertion\\out.txt", multi, cnt);
		else if (algoname.compare("selection") == 0)
			m1 = slc.selection(in_dir, "..\\selection\\out.txt", multi, cnt);	
	}
	return m1;
}

bool check_yes(string str) {
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {return std::tolower(c);});
	return (str.compare("y") == 0 || str.compare("yes") == 0);
}

bool in_array(const string &value, const vector<std::string> &array) {
    return std::find(array.begin(), array.end(), value) != array.end();
}

string map_to_string(map<string, long> m1) {
	string output = "";
	string convrt = "";
	string result = "";
	for (auto it = m1.cbegin(); it != m1.cend(); ++it) {
		convrt = to_string(it->second);
		output += "\"" + (it->first) + "\":" + (convrt) + ", ";
	}
	result = output.substr(0, output.size() - 2 );
    return result;
}

void json_parse_child(int k, ofstream& file, map<string, long> m1) {
	if (k != true)
		file << "\t\t\t,{" << map_to_string(m1) << "}" << endl;
	else
		file << "\t\t\t{" << map_to_string(m1) << "}" << endl;
}

int handle_sort_algo(string algoname, string ind, bool check, bool empty_json, ofstream& file, int& json_count) {
	string temp, temp2, dir_name, out_name;
	int v_result, cnt = 0, multicnt = 1;
	bool multirand = false;
	map<string, long> m1;
	Insertion ins;
	Selection slc;
	Writefile wrf;
	Judge jdg;

	while (check_yes(ind)) {
		++cnt;
		dir_name = "..\\" + algoname + "\\temp.txt";
		out_name = "..\\" + algoname + "\\out.txt";
		printf("\n------- CURRENT TOOL %s -------", algoname.c_str());
		cout << "\n>>> Create multirun process? (y/N) ";

		cin >> temp;
		if (check_yes(temp)) {
			cout << ">>> How many times to run? (positive whole number) ";
			cin >> multicnt;
			cout << ">>> Change list each time? Must write a new file after every run (y/N) ";
			cin >> temp2;
			if (check_yes(temp2))
				multirand = true;
			prl;
		}
		temp = "";

		if (multicnt == 1) {
			cout << "\n>>> Change sort test file? (y/N) ";
			cin >> temp; prl;
		}
		if (multicnt != 1)
			cout << "\n>>> Repeating process for " << multicnt << " times..." << endl;
		if (empty_json) {
			file << "\t\"" << json_count << "\": \n\t[" << endl;
			empty_json = false;
		}
		else
			file << "\t,\"" << json_count << "\": \n\t[" << endl;
		++json_count;

		if (check_yes(temp) || multirand == true) {
			cout << "\n------- CURRENT TOOL updatenum -------" << endl;
			printf(">>> Use default save location (..\\%s\\temp.txt)? (y/n -> create new file) ", algoname.c_str());
			cin >> temp;

			if (check_yes(temp)) {
				file << "\t\t{\"mode\": \"single\", \"algorithm\": \"" << algoname << "\"},\n\t\t[" << endl;
				for (int i = 1; i <= multicnt; ++i) {
					wrf.writefiledefaultdir(dir_name);
					printf("------- CONTINUE TO %s -------\n", algoname.c_str());
					m1 = choose_algo("", algoname, multicnt, i, ins, slc);
					json_parse_child(i, file, m1);
				}
			} else {
				cout << "\n------- CURRENT TOOL writefile -------" << endl;
				file << "\t\t{\"mode\": \"multi\", \"algorithm\": \"" << algoname << "\"},\n\t\t[" << endl;
				for (int i = 1; i <= multicnt; ++i) {
					string temp_dir = wrf.writefile();
					m1 = choose_algo(temp_dir, algoname, multicnt, i, ins, slc); // what is this tbh
					json_parse_child(i, file, m1);
				}
			}
			file << "\t\t]\n\t]" << endl;
		}
		else {
			printf(">>> Use default file (..\\%s\\temp.txt), not recommended for patched exe? (y/n) ", algoname.c_str());
			cin >> temp;
			transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) {return std::tolower(c);});
			if (check_yes(temp)) {
				printf(">>> Using \"..\\%s\\temp.txt\"\n", algoname.c_str());
				temp = "";
			}
			else {
				cout << ">>> Input text file directory: ";
				cin >> temp;
				printf(">>> Using \"%s\"\n", temp.c_str());
			}
			if (multicnt == 1)
				file << "\t\t{\"mode\": \"single\", \"algorithm\": \"" << algoname << "\"},\n\t\t[" << endl;
			else
				file << "\t\t{\"mode\": \"multi\", \"algorithm\": \"" << algoname << "\"},\n\t\t[" << endl;
			for (int i = 1; i <= multicnt; ++i) {
				m1 = choose_algo(temp, algoname, multicnt, i, ins, slc);
				json_parse_child(i, file, m1);
			}
			file << "\t\t]\n\t]" << endl; prl;
		}

		if (check) {
			cout << "------- REDIRECT verification -------" << endl;
			cout << ">>> Change verification settings by typing \"updateverify\"" << endl;
			v_result = jdg.judge(dir_name, out_name);
			if (v_result == 1)
				cout << ">>> Verification passed!\n" << endl;
			else
				cout << ">>> Verification failed!\n" << endl;
		}

		cout << ">>> Continue with same algorithm? (y/n) ";
		cin >> ind;
		multicnt = 1; multirand = false; prl;
	}

	return cnt;
}

int main() {
	string parent, child, cmd = "", temp, ind = "yes";
	vector<string> avbcommands = {"insertion", "selection"}, usedcommands = {};
	Writefile wrf;
	bool check = false, empty_json = true;	
	int json_count = 0;

	ofstream file;
	file.open("test.json");
	file << "{" << endl;

	cout << "------ WARNING ------" << endl;
	cout << "Do NOT close the program using ctrl+C!\nThe JSON file depends on the last bracket, ";
	cout << "so exit using `q` or `quit.`\nBlame me for using my least familiarized language...\n";
	cout << "Take your time and enter the commands carefully. There's no return!\n";
	cout << "------ END WARNING ------\n" << endl;

	while (true) {
		if (cmd.compare("quit") == 0 || cmd.compare("q") == 0)
			break;
		else if (cmd.compare("updatenum") == 0) {
			cout << ">>> CURRENT TOOL updatenum;" << endl;
			wrf.writefile();
		}
		else if (cmd.compare("updateverify") == 0) {
			check = !check;
			printf(">>> Verify has been toggled from %d to %d\n\n", !check, check);
		}
		else if (cmd.compare("help") == 0) {
			cout << "\n------- CURRENT TOOL help -------\n" << endl;
			cout << ">>> Type `help` for help (you're here)" << endl;
			cout << ">>> Type `quit` to quit the program" << endl;
			cout << ">>> Type `updateverify` to toggle sort-judging process" << endl;
			cout << ">>> Type `env` or `environ` to show program configurations" << endl;
			cout << ">>> Type `writefile` or `updatenums` to update test list" << endl;
			cout << ">>> Available list of algorithm commands: " << endl;
			cout << "    insertion, selection" << endl;
			cout << "\n------- EXIT help -------\n" << endl;
		}
		else if (cmd.compare("environ") == 0 || cmd.compare("env") == 0) {
			cout << "\n------- CURRENT TOOL environ -------\n" << endl;
			cout << "   - Verify sort results: " << check << endl;
			cout << "   - Used algorithms, in sequence: " << endl;
			for (int i = 0; i < usedcommands.size(); ++i)
				cout << "      " << i+1 << ". " << usedcommands[i] << endl;
			cout << "\n------- EXIT environ -------\n" << endl;
		}
		else if (cmd.compare("writefile") == 0 || cmd.compare("updatenums") == 0) {
			cout << "\n------ CURRENT TOOL writefile -------" << endl;
			cout << ">>> Note: don't forget folder name i.e., \"..\\insertion\\temp.txt\" or \"..\\selection\\temp.txt\"" << endl;
			wrf.writefile();
			cout << "------ EXIT writefile -------\n" << endl;
		}
		else if (cmd.compare("") != 0) {
			if (!in_array(cmd, avbcommands))
				cout << ">>> Please input an available command; type `help` for help\n" << endl;
			else {
				int x = handle_sort_algo(cmd, ind, check, empty_json, file, json_count);
				empty_json = false;
				usedcommands.push_back(cmd + " (x" + to_string(x) + ")");
			}
		}
		cout << ">>> Input command [quit, help, *]: "; cin >> cmd;
		transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) {return std::tolower(c);});
	}
	
	file << "}" << endl;
	file.close();
	return 0;
}
