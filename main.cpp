#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

void print_title(string title);
void get_questions(vector<string> *questions);
void get_question_name(string *question_name, vector<string> *questions);
void read_file_data(const string &file_path, vector<vector<string>> *data);
void get_inputs_outputs(const string &question_folder_path, vector<vector<string>> *inputs, vector<vector<string>> *outputs);

const string QUESTIONS_PATH = "./questions/";
const string PROGRAM_NAME = "program.cpp";

int main(void) {
	// Get Question Name
	vector<string> questions;
	get_questions(&questions);

	string question_name;
	get_question_name(&question_name, &questions);
	const string QUESTION_FOLDER_PATH = QUESTIONS_PATH + question_name;

	vector<vector<string>> inputs, outputs;
	get_inputs_outputs(QUESTION_FOLDER_PATH, &inputs, &outputs);

	cout << "Hello, world!\n";

	cout << "Inputs: \n";
	for (int i = 0; i < inputs.size(); i++) {
		vector<string> line_data = inputs.at(i);
		cout << "[";
		for (int j = 0; j < line_data.size(); j++) {
			cout << line_data.at(j) << " ";
		}
		cout << "]\n";
	}

	cout << "Outputs: \n";
	for (int i = 0; i < outputs.size(); i++) {
		vector<string> line_data = outputs.at(i);
		cout << "[";
		for (int j = 0; j < line_data.size(); j++) {
			cout << line_data.at(j) << " ";
		}
		cout << "]\n";
	}

	return 0;
}

void print_title(string title) {
	cout << "--------------------\n" << title << '\n';
}

void get_questions(vector<string> *question_files) {
	print_title("QUESTIONS");
	for (const auto &entry : fs::directory_iterator(QUESTIONS_PATH)) {
		if (entry.is_directory()) {
			string file_name = entry.path().filename().string();
			cout << file_name << "\t";
			question_files->push_back(file_name);
		}
	}
	cout << '\n';
}

void get_question_name(string* question_name, vector<string> *questions) {
	bool valid = false;
	while (!valid) {
		cout << "Choose a question: ";
		getline(cin, *question_name);

		auto location = find(questions->begin(), questions->end(), *question_name);

		if (location != questions->end()) {
			valid = true;
		} else {
			cout << "Invalid file name. Try again.\n";
		}
	}
}

void read_file_data(const string& file_path, vector<vector<string>> *data) {
	ifstream file(file_path);
	if (!file) {
		cerr << "Unable to open file with path " << file_path << '\n';
		return;
	}

	string line;
	while (getline(file, line)) {
		int opening_bracket = line.find('[');
		int closing_bracket = line.find(']');

		if (opening_bracket == string::npos || closing_bracket == string::npos) {
			return;
		}

		vector<string> line_data;

		// Start at once since character 0 is [
		int left_ptr = 1;

		// Although last character is ], include it
		// so we can include everything up to it as
		// another substring in line_data.
		for (int i = 1; i < line.length(); i++) {
			char c = line.at(i);

			// FIXME trim all spaces!
			if (c == ',' || i == line.length() - 1) {
				int length = i - left_ptr;
				if (length <= 0)
					cerr << "Malformed input in file " << file_path << '\n';
				
				line_data.push_back(line.substr(left_ptr, length));
				left_ptr = i + 1;
			}
		}

		data->push_back(line_data);
	}
}

void get_inputs_outputs(const string &question_folder_path, vector<vector<string>> *inputs, vector<vector<string>> *outputs) {
	// FIXME check if the files exist
	read_file_data(question_folder_path + "/inputs.txt", inputs);
	read_file_data(question_folder_path + "/outputs.txt", outputs);
}