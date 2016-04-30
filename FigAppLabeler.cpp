/*
"old label" refers to a label in the input file
"new label" refers to a label in the output file

Problem:
	for large, collaborative reports, relabeling figures and appendices 
	in the correct order takes so much more time than it's worth 
	but you have to do it

How to use:
	users label figures/appendices with any integer (-2^16 < label < 2^16-1)
	set input file
	this program:
		1) reads and copies file
		2) as it reads, it looks for labels
		3) renumbers in order for the output file
			a) if it is referenceing a new fig/app, stores the old label
			b) if it is referencing a previous fig/app, finds that old label, 
			   returns the new label (index) used for that fig/app

*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>

//#ifndef NOMINMAX
//#define NOMINMAX
//#endif
//#include <windows.h>

//#include "WordAutomation.h"
//#include "EzWordAutomation.h"

using namespace std;


#include "FigAppLabeler.h"
const int N = 100;							// max number of figures and labels


int main() {
	
	char path_in[200];
	cout << "input file path: ";
	cin.getline(path_in, N);
	changeFiles(path_in);
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// read the entire file
// find labels for figures and appendices
// search for "figure," "fig.," "appendix," "app."

// still to do: 
// x 1) ignore cases where the word "figure" or "appendix" is used as part of a sentence
//      x: only looks at "fig" or "app" when followed by [' ', digit]
//   2) let users choose between labeling with numbers or letters
//   3) use .doc files. will this read text boxes/other special formatting?

void changeFiles(char path_in[]) {

	
	// get old file name
	ifstream ifile(path_in);
	if (!ifile) {
		cout << "404 File Not Found \n";
		return;
	}


	// create new file name "* RELABELED.xxx"
	// I KNOW that using strings would have been easier/better/shorter
	ofstream ofile;
	char path_out_suf[] = " RELABELED";
	char path_out[200];
	char ext[10];
	char* pchp;
	// check for a file extension and split
	pchp = (char*) strchr(path_in, '.');
	if (pchp != NULL) {
		int split_pos = pchp - path_in;
		for (int i = strlen(path_in), j = 0; i > split_pos-1; i--, j++)
			ext[j] = path_in[j+split_pos];
		strncat(path_out, path_in, split_pos);
		strcat(path_out, path_out_suf);
		strcat(path_out, ext);
		ofile.open(path_out);
	} else {
		cout << "404 File Not Found \n";
		return;
	}	

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

		if (isFig(str, ifile)) {
			// replace oldlabel with newlabel, store oldlabel
			ifile >> oldFigLabel;
			newLabel = searchNStoreOldLabels(oldFigLabels, oldFigLabel, figcount) + 1;
			ofile << newLabel;

		} else if (isApp(str, ifile)) {
			// replace oldlabel with newlabel, store oldlabel
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

bool isFig(const string& str, ifstream& infile) {
	if (compareStr(str, "Figure") || compareStr(str, "Fig.") ||	compareStr(str, "Fig")) {
		if (nextIsInt(infile))
			return true;
	}
	return false;
}

bool isApp(const string& str, ifstream& infile) {
	if (compareStr(str, "Appendix") || compareStr(str, "App.") || compareStr (str, "App")) {
		if (nextIsInt(infile)) 
			return true;
	}
	return false;
}

bool nextIsInt(ifstream& infile) {
	string line, next;
	int pos = infile.tellg();
	getline(infile, line);

	istringstream sline(line);
	sline >> next;
	infile.seekg(pos, infile.beg);

	if (next.empty() || ((!isdigit(next[0])) && (next[0] != '-') && (next[0] != '+'))) 
		return false;

   char* p;
   strtol(next.c_str(), &p, 10);

   return (*p == 0) ;

}


// end of figure/appendix label checking
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// string compare functions

// case insensitive
bool ciCompareChar(char a, char b) {
	return tolower(a) == tolower(b);
}

bool compareStr(const string& str1, const string& str2) {
	if (str1.size() == str2.size()) {
		return equal(str1.begin(), str1.end(), str2.begin(), ciCompareChar);
	} else {
		return false;
	}
}

// end of string compare functions
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// get whitespaces

void writeWhiteSpaces(ifstream& infile, ofstream& outfile) {

	if (infile.eof())
		return;

	char ws;
	char next = infile.peek();

	while (next < 33 || next > 126) {
		ws = infile.get();
		outfile << ws;

		next = infile.peek();

		if (infile.eof())
			return;

	}
}

// end of whitespace function
//////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////////
// search and store oldlabels, return index

int searchNStoreOldLabels(int oldLabels[], int oldLabel, int& numLabels) {
	
	int index;

	if (numLabels == 0) {
		oldLabels[0] = oldLabel;
		index = 0;
		numLabels++;
	} else {
		int* p = find(oldLabels, oldLabels+numLabels, oldLabel);
		if (p == oldLabels + numLabels)
			numLabels++;
		index = distance(oldLabels, p);
	}

	return index;
}

// end of search and store function
//////////////////////////////////////////////////////////////////////////////////////////////////