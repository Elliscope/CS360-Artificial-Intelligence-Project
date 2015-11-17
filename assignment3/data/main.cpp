#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>

using namespace std;


class TrainingSet{
	private:
		int totalMessage;
		double* MessageClassCount;
		double* ProbClass;
		map<string,int>* ProbWordClass;

	public:
        TrainingSet(){
		totalMessage=0;
		ProbClass = new double[4];
		MessageClassCount = new double[4];
		ProbWordClass = new map<string,int>[4];
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

	double getProbClass(int i){
		return ProbClass[i];
	}

	double getMessageClassCount(int i ){
		return MessageClassCount[i];
	}

	map<string,int> getProbWordClass(int i){
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

void ParseFile(string filename)
{
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
			cout<<word<<endl;
		}
	}
}

void TrainTheModel(TrainingSet trainingSet, string trainingFileList ){

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
        trainingSet.increaseMessageClassCount(classNum);
        trainingSet.increaseTotalMessage();

        cout << "filename " << filename << endl;
        cout << "classname " << classNum << endl;
	}

		for(int a = 0 ; a < 4 ; a++){
			double prob = trainingSet.getMessageClassCount(a)/(double)trainingSet.getTotalM();
			trainingSet.setProbClass(a,prob);
			//cout<<"ProbClass " << setprecision(3)<<a << " " <<trainingSet.getProbClass(a)<<endl;
		}

		cout<<"TotalMessageClassCount "<<trainingSet.getTotalM()<<endl;
		cout<<"TotalMessageClass1 "<<trainingSet.getMessageClassCount(0)<<endl;
		cout<<"TotalMessageClass2 "<<trainingSet.getMessageClassCount(1)<<endl;
		cout<<"TotalMessageClass3 "<<trainingSet.getMessageClassCount(2)<<endl;
		cout<<"TotalMessageClass4 "<<trainingSet.getMessageClassCount(3)<<endl;
}

int main()
{

	TrainingSet trainingSet;
	TrainTheModel(trainingSet,"training_list");

	//ParseFile("training/comp.graphics/37914");
	
	return 0;
}
