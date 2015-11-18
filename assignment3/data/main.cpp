#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <unordered_map>
#include <set>

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
			
   				 
			// int counter = currentCounter->second;
			// cout<<counter<<endl;
			// if(currentCounter->second!=NULL){
			// 	cout<<word<<" here is the currentCounter "<<currentCounter->second <<endl;
			// }
			
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
        //do something withe file 

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
        		//cout<<trainingSet->ProbWordClass[a][it->first]<<endl;
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


	

		// cout<<"TotalMessageClassCount "<<trainingSet->getTotalM()<<endl;
		// cout<<"TotalMessageClass1 "<<trainingSet->getMessageClassCount(0)<<endl;
		// cout<<"TotalMessageClass2 "<<trainingSet->getMessageClassCount(1)<<endl;
		// cout<<"TotalMessageClass3 "<<trainingSet->getMessageClassCount(2)<<endl;
		// cout<<"TotalMessageClass4 "<<trainingSet->getMessageClassCount(3)<<endl;


}





int main()
{

	TrainingSet* trainingSet = new TrainingSet();
	TrainTheModel(trainingSet,"training_list");

	// ParseFile("training/comp.graphics/37914",0,trainingSet);
	// ParseFile("training/comp.graphics/37915",0,trainingSet);
	

	// map<string,int>* W = new map<string,int>();
	// W[0]["elliscope"] = 1;
	// W[0]["steven"] = 2;

	// auto it = W->find("elliscope");
	// if (it != W->end()) cout << "x: " << it->second << "\n";

	return 0;
}
