#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

enum Answer{
	None = 0,
	Yes = 1,
	No = 2
};

struct Item{
	string name;

	map<int, Answer> validAnswers;
	
	Item(){}
	Item(string _name, const map<int, Answer>& _validAnswers)
	:name(_name),validAnswers(_validAnswers){}
	
	int getFitness(const map<int, Answer>& answers){
		int temp = 0;
		for(auto valid:validAnswers){
			auto it = answers.find(valid.first);
			if(it!=answers.end()){
				if(it->second!=None){
					if(it->second == valid.second)
						++temp;
					else
						--temp;
				}
			}
		}
		return temp;
	}
};

map<int, string> questions;
map<int, Answer> answeredQuestions;
vector<Item> items;

Answer requestAnswer(const string& question){
	cout << question << endl
	     << "1 - S¡" << endl
		 << "2 - No" << endl
		 << "(otro) - No s‚" << endl
		 << " >> ";
	string answer;
	getline(cin, answer);
	if(answer=="1")
		return Yes;
	if(answer=="2")
		return No;
	return None;
}

bool getUnansweredQuestion(const map<int, Answer>& validAnswers, int* ret){
	auto itValid = validAnswers.begin();
	auto itQuestion = answeredQuestions.begin();
	
	if(itValid!=validAnswers.end())
		while(itQuestion!=answeredQuestions.end()
		   && itValid->first > itQuestion->first)
			++itQuestion;
	while(itValid!=validAnswers.end()
	   && itQuestion!=answeredQuestions.end()
       && itValid->first == itQuestion->first){
		++itValid, ++itQuestion;
		if(itValid!=validAnswers.end())
			while(itQuestion!=answeredQuestions.end()
			   && itValid->first > itQuestion->first)
				++itQuestion;
	}
	
	if(itValid==validAnswers.end())
		return false;
	*ret = itValid->first;
	return true;
}

bool tryItem(const Item& item){
	cout << "Creo que es: " << item.name << endl
		 << "¨He acertado? (1 - S¡ / otro - No)" << endl
		 << " >> ";
	string answer;
	getline(cin, answer);
	if(answer=="1")
		return true;
	// Question if still want to play
	return false;
}

void playGame(){
	if(items.size()==0)
		return;
	vector<pair<int,int>> sortedItems; // Item index, fitness
	for(int i=0; i<items.size(); i++){
		if(items[i].validAnswers.size()>0)
			sortedItems.push_back(make_pair(i, 0));
	}
	bool finished = false;
	while(!finished){
		// Maybe do an algorithm for find most usefull questions first
		if(sortedItems.size()==0){
			cout << "Lo siento, no s‚ en qu‚ est s pensando" << endl;
			finished = true;
			break;
		}
		
		int nextQuestion = -1;
		int itemQuestionIndex = 0;
		for(; itemQuestionIndex<sortedItems.size(); itemQuestionIndex++)
			if(getUnansweredQuestion(items[sortedItems[itemQuestionIndex].first].validAnswers, &nextQuestion))
				break;
		if(nextQuestion < 0 || itemQuestionIndex > 0){ // If no more questions (or best item have no questions)
			finished = tryItem(items[sortedItems[0].first]);
			if(!finished){
				sortedItems.erase(sortedItems.begin());
				continue;
			}else
				break;
		}
		
		answeredQuestions[nextQuestion] = requestAnswer(questions[nextQuestion]);

		for(pair<int,int>& it:sortedItems)
			it.second = items[it.first].getFitness(answeredQuestions);
		
		sort(sortedItems.begin(), sortedItems.end(),
		     [](const pair<int,int>& a, const pair<int,int>& b)->bool{return a.second>b.second;});
		for(int i=0; i<sortedItems.size(); i++){
			if(items[sortedItems[i].first].validAnswers.size() == sortedItems[i].second){
				finished = tryItem(items[sortedItems[i].first]);
				if(!finished){
					sortedItems.erase(sortedItems.begin() + i);
					--i; // Fast for increment fix
				}else
					break;
			}
		}
	}
}

bool loadFromFile(string fileName){
	ifstream file(fileName);
	if(!file)
		return false;
	questions.clear();
	items.clear();
	
	string line;
	do{
		getline(file, line);
		size_t pos = line.find(':');
		if(pos==string::npos)
			break;
		try{
			size_t next;
			questions[stoi(line.substr(0,pos), &next)] = line.substr(pos+1);
			if(next!=pos)
				return false;
		}catch(const exception&){
			return false;
		}
	}while(file);
	do{
		getline(file, line);
		Item item;
		item.name = line;
		while(file){
			getline(file, line);
			size_t pos = line.find(':');
			if(pos==string::npos)
				break;
			try{
				size_t next;
				int ans = stoi(line.substr(pos+1), &next);
				if(next != line.size()-pos-1)
					return false;
				switch(ans){
				case 0: item.validAnswers[stoi(line.substr(0,pos), &next)] = None; break;
				case 1: item.validAnswers[stoi(line.substr(0,pos), &next)] = Yes; break;
				case 2: item.validAnswers[stoi(line.substr(0,pos), &next)] = No; break;
				default: return false;
				}
				if(next != pos)
					return false;
			}catch(const exception&){
				return false;
			}
		}
		items.push_back(item);
	}while(file);
	
	
	return true;
}

bool saveToFile(string fileName){
	ofstream file(fileName, ios::trunc);
	if(!file)
		return false;
	for(auto it:questions)
		file << it.first << ':' << it.second << '\n';
	
	for(int i=0; i<items.size(); i++){
		const Item& item = items[i];
		if(i>0)
			file << '\n';
		file << '\n' << item.name;
		for(auto it:item.validAnswers)
			file << '\n' << it.first << ':' << it.second;
	}
	return true;
}

int main(){
	/*questions = {
		{1, "¨Vive en el agua?"},
		{2, "¨Es grande?"},
		{3, "¨Es carn¡voro?"},
		{4, "¨Tiene cola?"},
		{5, "¨Anda a cuatro patas?"}
	};
	items = {
		Item("Vaca", {
			{1, No},
			{2, Yes},
			{3, No},
			{4, Yes},
			{5, Yes}
		}),
		Item("Gato", {
			{3, No},
			{4, Yes},
			{5, Yes}
		}),
		Item("Hormiga", {
			{2, No},
			{4, No}
		}),
		Item("Le¢n", {
			{2, Yes},
			{3, Yes},
			{4, Yes},
			{5, Yes}
		}),
		Item("Camale¢n", {
			{1, No},
			{2, No},
			{3, No},
			{4, Yes}
		}),
		Item("Tibur¢n", {
			{1, Yes},
			{2, Yes},
			{4, Yes}
		}),
		Item("Pececito", {
			{1, Yes},
			{2, No}
		})
	};
	cout << "Elige uno entre estos, y adivinar‚ cu l es:";
	for(const Item& item:items)
		cout << ' ' << item.name;
	cout << endl << endl;*/
	//saveToFile("test.tst");
	if(loadFromFile("test.tst")){
		cout << "Test cargado." << endl << "Comenzando..." << endl << endl;
		playGame();
	}else{
		cout << "Error al cargar el test" << endl;
	}
}
