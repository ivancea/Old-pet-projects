#include <iostream>
#include <map>

using namespace std;

struct State{
	map<char, string> transitions;
	
	string onEnter;
	string onEnd;
	
	State(){}
	
	State(map<char, string> _transitions, string _onEnter="", string _onEnd="")
	:transitions(_transitions),onEnter(_onEnter),onEnd(_onEnd){}
};

void start(const map<string, State>& states, string first, string input){
	string name = first;
	auto it = states.find(name);
	bool error = true;
	int i = 0;
	while(it != states.end()){
		error = false;
		const State& state = it->second;
		
		if(state.onEnter != ""){
			cout << '[' << name << "]";
			if(i>0)
				cout << '(' << input[i-1] << ')';
			cout << " entered: " << state.onEnter << endl;
		}
		
		if(input.size()<=i){
			cout << "Ended: No more input" << endl;
			break;
		}
		
		auto next = state.transitions.find(input[i]);
		
		if(next == state.transitions.end()){
			if(state.onEnd == "")
				cout << "Error at [" << name << "]. Found (" << input[i] << ")" << endl;
			else
				cout << '[' << name << "] ended: " << state.onEnd << endl;
			break;
		}
		
		++i;
		name = next->second;
		it = states.find(name);
		error = true;
	}
	if(error){
		cout << "Error: State [" << name << "] not exists" << endl;
	}
}

int main(){
	map<string, State> states;
	states["s"] = State({
		{'a', "s"},
		{'b', "s"},
		{'/', "a"},
		{'*', "s"}
	});
	states["a"] = State({
		{'a', "s"},
		{'b', "s"},
		{'/', "a"},
		{'*', "b"}
	}, "OUTPUT");
	states["b"] = State({
		{'a', "b"},
		{'b', "b"},
		{'/', "b"},
		{'*', "c"}
	}, "OUTPUT");
	states["c"] = State({
		{'a', "b"},
		{'b', "b"},
		{'/', "d"},
		{'*', "c"}
	}, "OUTPUT");
	states["d"] = State({
		{'a', "s"},
		{'b', "s"},
		{'/', "a"},
		{'*', "s"}
	}, "COMMENT FINISHED");
	
	start(states, "s", "abbabaabab/*abba**/bb/**/");
}