#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <unordered_map>
#include <set>
#include <math.h>

using namespace std;


class TrainingSet{
	public:
		int totalMessage;
		double* MessageClassCount;
		double* ProbClass;
		map<string,double>* ProbWordClass;
		unordered_map<string,int>* WordCountClass;
	public:
        TrainingSet(){
		totalMessage=0;
		ProbClass = new double[4];
		MessageClassCount = new double[4];
		WordCountClass = new unordered_map<string,int>[4];
		ProbWordClass = new map<string,double>[4];
		for(int i = 0 ; i < 4; i++){
			MessageClassCount[i] = 0;
		}
	}

	void increaseTotalMessage(){
		totalMessage++;
	}

	void increaseMessageClassCount(int i){
		MessageClassCount[i]++;
	}

	void setProbClass(int i, double prob){
		ProbClass[i-1] = prob;
	}

	int getTotalM(){
		return totalMessage;
	}

	unordered_map<string,int> getWordCountClass(int i){
		return WordCountClass[i];
	}

	double getProbClass(int i){
		return ProbClass[i];
	}

	double getMessageClassCount(int i ){
		return MessageClassCount[i];
	}

	map<string,double> getProbWordClass(int i){
		return ProbWordClass[i];
	}


};


string ProcessWord(string s)
{
	string t;
	
	// Remove punctuation.
    for (int i = 0; i < s.size(); i++)
        if (!ispunct(s[i]))
			t += s[i];

	// Convert to lower case.
	std::transform(t.begin(), t.end(), t.begin(), ::tolower);
	
	return t;
}

void ParseFile(string filename, int classNum, TrainingSet* trainingSet)
{
	set<string> setBuffer;
	set<string>::iterator it;
	// Open file.

	ifstream in;
	in.open(filename.c_str());
	if (!in.is_open())
	{
		cout<<"File not found: "<<filename<<endl;
		return;
	}
	
	// Find the end of the header.
	string line;
	while (getline(in, line))
	{
		if (line == "")
			break;
	}

	// Read the rest of the file word by word.
	string word;
	while (in >> word)
	{
		// Strip the word of punctuation and convert to lower case.
		word = ProcessWord(word);
		
		if (word != "")
		{		
			// Do stuff here (for instance, check if the word is in the dictionary).
			// You might also like to make this function return something.
			it = setBuffer.find(word);
			if(it==setBuffer.end()){
				//cout<<"here in the parse file function"<<endl;
				unordered_map<string,int>::const_iterator currentCounter = trainingSet->WordCountClass[classNum].find(word);
				//cout<<word<<endl;
				if ( currentCounter != trainingSet->WordCountClass[classNum].end() ){
				//cout << currentCounter->first << " is " << currentCounter->second<<endl;
				trainingSet->WordCountClass[classNum][word]=currentCounter->second+1;
				}
				setBuffer.insert(word);
			}			
		}
	}
}

void PopulateDictionary(string filename, TrainingSet* trainingSet)
	{
		// Open file.
		ifstream in;
		in.open(filename.c_str());
		if (!in.is_open())
		{
			cout<<"File not found: "<<filename<<endl;
			return;
		}
		// Read the rest of the file word by word.
		string word;
		while (in >> word)
		{
			// Strip the word of punctuation and convert to lower case.
			word = ProcessWord(word);
		
			if (word != "")
			{		
				for(int i = 0; i< 4; i++){
					//pair<std::string,double> newWord (word,0);
					//trainingSet->getWordCountClass(i).insert(newWord);
					trainingSet->WordCountClass[i][word]=0;
					trainingSet->ProbWordClass[i][word]=0;
				}
			}
		}
		in.close();
}


void TrainTheModel(TrainingSet* trainingSet, string trainingFileList ){

	PopulateDictionary("dictionary", trainingSet);

	ifstream in;
	in.open(trainingFileList.c_str());

	if (!in.is_open())
	{
		cout<<"traning file list not found: "<<trainingFileList<<endl;
		return;
	}
	
	// Find the end of the header.
	string line;
	while (getline(in, line))
	{
		istringstream iss(line);

        string filename;
        iss >> filename;

        string classname;
        iss >> classname;

        int classNum = stoi(classname);
        trainingSet->increaseMessageClassCount(classNum);
        trainingSet->increaseTotalMessage();

        ParseFile(filename,classNum,trainingSet);
	}

		//store the value of ProbClass Array
		for(int a = 0 ; a < 4 ; a++){
			double prob = trainingSet->getMessageClassCount(a)/(double)trainingSet->getTotalM();
			trainingSet->setProbClass(a,prob);
			cout<<"ProbClass " << setprecision(3)<<a << " " <<trainingSet->getProbClass(a)<<endl;
		}

		//calculate the probability for each word
		for(int b = 0; b<4; b++){
        	for (unordered_map<string,int>::iterator it=trainingSet->WordCountClass[b].begin(); it!=trainingSet->WordCountClass[b].end(); ++it){
        		trainingSet->ProbWordClass[b][it->first] = ((double)trainingSet->WordCountClass[b][it->first]+1)/((double)trainingSet->MessageClassCount[b]+2);
        	}
        }


		//output the network.txt 
		ofstream networkfile;
		networkfile.open ("solution/network.txt");

        ifstream Dic;
        string dictionaryName = "dictionary";
		Dic.open(dictionaryName.c_str());
		if (!Dic.is_open())
		{
			cout<<"File not found: "<<dictionaryName<<endl;
			return;
		}
		// Read the rest of the file word by word.
		string word;
		while (Dic >> word)
		{
			// Strip the word of punctuation and convert to lower case.
			word = ProcessWord(word);
			if (word != "")
			{		
				for(int i = 0; i< 4; i++){
					unordered_map<string,int>::iterator it = trainingSet->WordCountClass[i].find(word);
					networkfile << it->first<<"     "<<i<<"      "<<trainingSet->ProbWordClass[i][it->first]<< "\n";
				}
			}
		}
		Dic.close();
        networkfile.close();
}


int TestFile(string filename,TrainingSet* trainingSet,map<string, int>* testResult){
	double* probability = new double[4];


	for(int i =0 ; i<4; i++){
		probability[i] = trainingSet->getProbClass(i);
		cout<<probability[i]<<endl;
	}



	set<string> MessageWords;
	set<string>::iterator message_it;

	map<string,double>::iterator it;

	ifstream in;
	in.open(filename.c_str());
	if (!in.is_open())
	{
		cout<<"File not found: "<<filename<<endl;
		return -1;
	}
	
	// Find the end of the header.
	string line;
	while (getline(in, line))
	{
		if (line == "")
			break;
	}

	// Read the rest of the file word by word.
	string word;

	//Store all words find in the message into the set
	while (in >> word)
	{
		// Strip the word of punctuation and convert to lower case.
		word = ProcessWord(word);
		if (word != "")
		{		
			// Do stuff here (for instance, check if the word is in the dictionary).
			// You might also like to make this function return something.
			message_it = MessageWords.find(word);
			if(message_it==MessageWords.end()){
				MessageWords.insert(word);
			}			
		}
	}

	in.close();


	ifstream Dic;
	string dictionary = "dictionary";
	Dic.open(dictionary.c_str());
	if (!Dic.is_open())
	{
		cout<<"File not found: "<<dictionary<<endl;
		return -1;
	}

	//Store all words find in the message into the set
	while (Dic >> word)
	{
		// Strip the word of punctuation and convert to lower case.
		word = ProcessWord(word);
		if (word != "")
		{		
			// Do stuff here (for instance, check if the word is in the dictionary).
			// You might also like to make this function return something.
			message_it = MessageWords.find(word);
			for(int a = 0; a<4; a++){
				double pro = trainingSet->ProbWordClass[a][word];
				if(message_it==MessageWords.end()){	
				probability[a] += log(1-pro);
				}else{
				probability[a] += log(pro);
				}	
			}
					
		}
	}

	double max = probability[0];

	for(int i =0 ; i<4; i++){
		if(max<probability[i]){
			max = probability[i];
		}
	}

	for(int i =0; i<4 ; i++){
		if(max==probability[i]){
			return i;
		}
	}

	cout<<"probability"<<endl;

	Dic.close();
	return -1;
}


//Deal with test file
void TestModel(TrainingSet* trainingSet, string testFileList, map<string, int>* testResult,int** array){

	ifstream in;
	in.open(testFileList.c_str());

	if (!in.is_open())
	{
		cout<<"tets file list not found: "<<testFileList<<endl;
		return;
	}
	
	// Find the end of the header.
	string line;
	while (getline(in, line))
	{
		istringstream iss(line);

        string filename;
        iss >> filename;

        string classname;
        iss >> classname;

        int classNum = stoi(classname);
        int resultNum = TestFile(filename,trainingSet,testResult);

        cout<<"resultNum is here  outside function "<<resultNum<<endl;
        cout<<"classNum is here outside function "<<classNum<<endl;

        array[classNum][resultNum]+=1;

	}

		ofstream summaryfile;
		summaryfile.open ("solution/classification-summary.txt");

       	for(int b = 0; b< 4; b++){
			for(int c= 0; c<4;c++){
				summaryfile<<array[b][c]<<" ";
			}
			summaryfile<<"\n";
		}

        summaryfile.close();		
}







int main()
{
	map<string, int>* testResult = new map<string,int>();

	
	//initialize print out array
	int width = 4;
	int height = 4;
	int** arr = new int*[width];
	
	for(int i = 0; i < width; ++i){
			arr[i] = new int[height];
			for(int a = 0; a<height ; a++){
				arr[i][a]=0;
			}
	}
   			

	TrainingSet* trainingSet = new TrainingSet();
	TrainTheModel(trainingSet,"training_list");


	TestModel(trainingSet,"test_list", testResult,arr);

	// ParseFile("training/comp.graphics/37914",0,trainingSet);
	// ParseFile("training/comp.graphics/37915",0,trainingSet);
	

	// map<string,int>* W = new map<string,int>();
	// W[0]["elliscope"] = 1;
	// W[0]["steven"] = 2;

	// auto it = W->find("elliscope");
	// if (it != W->end()) cout << "x: " << it->second << "\n";

	return 0;
}
