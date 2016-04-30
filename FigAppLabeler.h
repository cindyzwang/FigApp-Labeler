#ifndef FIGGAPPLABELER_H
#define FIGGAPPLABELER_H


void changeFiles(string path_in);
bool isFig(const string& str, ifstream& infile);
bool isApp(const string& str, ifstream& infile);
bool nextIsInt(ifstream& infile);
void compareStrings();
bool ciCompareChar(char a, char b);
bool csCompareChar(char a, char b);
bool compareStr(const string& str1, const string& str2);
void writeWhiteSpaces(ifstream& ifile, ofstream& ofile);
int searchNStoreOldLabels(int oldLabels[], int oldLabel, int& numLabels);
//*void convertToLetters();


#endif
