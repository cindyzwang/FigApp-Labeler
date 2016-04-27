#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

void readFile();
bool isFig(const string& str);
bool isApp(const string& str);
void compareStrings();
bool ciCompareChar(char a, char b);
bool csCompareChar(char a, char b);
bool compareStr(const string& str1, const string& str2);
void writeWhiteSpaces(ifstream& ifile, ofstream& ofile);
int searchNStoreOldLabels(int oldLabels[], int oldLabel, int& numLabels);

const int N = 100;

int main() {
	readFile();
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// read the entire file (will this read text boxes after using .doc files?)
// find labels for figures and appendices
// search for "figure," "fig.," "appendix," "app."

// still to do: 
// 1) ignore cases where the word "figure" or "appendix" is used as part of a sentence
// 2) let users choose between labeling with numbers or letters

void readFile() {
	
	ifstream ifile("linked labels test.txt");
	ofstream ofile("new linked labels test.txt");
	
	string str;

	int oldFigLabels[N];
	int oldFigLabel;
	int figcount = 0;
	
	int oldAppLabels[N];
	int oldAppLabel;
	int appcount = 0;

	int newLabel;

	writeWhiteSpaces(ifile, ofile);

	while (ifile >> str) {

		ofile << str;
		writeWhiteSpaces(ifile, ofile);

		if (isFig(str)) {
			// replace oldlabel with newlabel, store oldlabel
			ifile >> oldFigLabel;
			newLabel = searchNStoreOldLabels(oldFigLabels, oldFigLabel, figcount) + 1;
			ofile << newLabel;

		} else if (isApp(str)) {
			// replace oldlabel with newlabel
			ifile >> oldAppLabel;
			newLabel = searchNStoreOldLabels(oldAppLabels, oldAppLabel, appcount) + 1;
			ofile << newLabel;
		}

		writeWhiteSpaces(ifile, ofile);
	} 

	ifile.close();
	ofile.close();
}

// end of reading file
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// functions to check for figure or appendix label

bool isFig(const string& str) {
	return (compareStr(str, "Figure") || 
		compareStr(str, "Fig.") || 
		compareStr(str, "Fig"));
}

bool isApp(const string& str) {
	return (compareStr(str, "Appendix") || 
		compareStr(str, "App.") || 
		compareStr (str, "App"));
}

// end of figure/appendix label checking
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
//string compare functions 

// case insensitive
bool ciCompareChar(char a, char b) {
	return tolower(a) == tolower(b);
}

// case sensitive
bool csCompareChar(char a, char b) {
	return a == b;
}

bool compareStr(const string& str1, const string& str2) {
	if (str1.size() == str2.size()) {
		return equal(str1.begin(), str1.end(), str2.begin(), ciCompareChar);
	} else {
		return false;
	}
}

// end of case insensitivs string compare functions
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// get whitespaces. take care of eof

void writeWhiteSpaces(ifstream& infile, ofstream& outfile) {

	if (infile.eof()) {
		return;
	}

	char ws;
	char next = infile.peek();

	while (next < 33 || next > 126) {
		ws = infile.get();
		outfile << ws;

		next = infile.peek();

		if (infile.eof()) {
			return;
		}	

	}
}

// end of whitespace function
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// search through oldlabels, return index

int searchNStoreOldLabels(int oldLabels[], int oldLabel, int& numLabels) {
	
	int index;

	if (numLabels == 0) {
		oldLabels[0] = oldLabel;
		index = 0;
		numLabels++;

	} else {

		int* p = find(oldLabels, oldLabels+numLabels, oldLabel);

		if (p == oldLabels + numLabels) {
			numLabels++;
		}

		index = distance(oldLabels, p);
	}

	return index;
}

// end of search and store function
//////////////////////////////////////////////////////////////////////////////////////////////////