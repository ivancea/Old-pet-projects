#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <exception>
#include <future>

using namespace std;


using ActivationFunction = double(*)(double);

class NeuronalException : public exception{
	const char* _message;

public:
	NeuronalException(const char* message) : _message(message){}

	const char* what() const noexcept{
		return _message;
	}
};

class Node{
public:
	virtual ~Node(){}

	virtual double getOutput() = 0;

	virtual Node* clone() = 0;
};

class InputNode : public Node {
	double _data;

public:

	InputNode():_data(0){}
	InputNode(double data):_data(data){}
	InputNode(const InputNode& node) : _data(node._data) {}

	virtual Node* clone(){
        return new InputNode(_data);
	}

	void setData(double data){
		_data = data;
	}

	virtual double getOutput(){
		return _data;
	}
};

enum class NeuronalNodeState {
	NoValue,
	CalculatingValue,
	Ready
};

class NeuronalNode : public Node {
	vector<Node*> _sourceNodes;
	vector<double> _weights;

	double _savedValue;
	NeuronalNodeState _state;

	ActivationFunction _activationFunction;

public:

	NeuronalNode()
		: _savedValue(0.0)
		, _state(NeuronalNodeState::NoValue)
		, _activationFunction(nullptr){

	}
	NeuronalNode(NeuronalNode&& node){
		_sourceNodes = std::move(node._sourceNodes);
		_weights = std::move(node._weights);
		_state = node._state;
		_activationFunction = node._activationFunction;
	}

	virtual Node* clone(){
	    NeuronalNode* node = new NeuronalNode();
	    node->setActivationFunction(_activationFunction);
        return node;
	}

	void setSource(Node* node, double weight = 0){
		_state = NeuronalNodeState::NoValue;
		for(int i=0; i<_sourceNodes.size(); i++){
			if(_sourceNodes[i] == node){
				_weights[i] = weight;
				return;
			}
		}

		_sourceNodes.push_back(node);
		_weights.push_back(weight);
	}

	void removeSource(Node* node){
		for(int i=0; i<_sourceNodes.size(); i++){
			if(_sourceNodes[i] == node){
				_state = NeuronalNodeState::NoValue;
				_sourceNodes.erase(_sourceNodes.begin() + i);
				_weights.erase(_weights.begin() + i);
				break;
			}
		}
	}

	void resetValue(){
		_state = NeuronalNodeState::NoValue;
	}

	void clear(){
		_sourceNodes.clear();
		_weights.clear();
		_state = NeuronalNodeState::NoValue;
	}

	size_t getSourceNodesCount() const {
		return _sourceNodes.size();
	}

	double getWeight(Node* node) const {
		for(int i=0; i<_sourceNodes.size(); i++){
			if(_sourceNodes[i] == node){
				return _weights[i];
			}
		}

		return -1;
	}

	vector<double>& getWeights(){
		return _weights;
	}

	bool existsSource(Node* node) const {
		for(int i=0; i<_sourceNodes.size(); i++){
			if(_sourceNodes[i] == node){
				return true;
			}
		}

		return false;
	}

	void setActivationFunction(ActivationFunction activationFunction){
		_activationFunction = activationFunction;
	}

	ActivationFunction getActivationFunction() const {
		return _activationFunction;
	}

	virtual double getOutput(){
		if(_state == NeuronalNodeState::CalculatingValue){
			throw NeuronalException("NeuronalNode#getOutput: Infinite loop");
			// Maybe, return _savedValue (old value)
		}else if(_state == NeuronalNodeState::NoValue){
			_state = NeuronalNodeState::CalculatingValue;
			double newValue = 0;
			for(int i=0; i<_sourceNodes.size(); i++){
				newValue += _sourceNodes[i]->getOutput() * _weights[i];
			}

			if(_activationFunction != nullptr){
				_savedValue = _activationFunction(newValue);
			}else{
				_savedValue = newValue;
			}

			_state = NeuronalNodeState::Ready;
		}

		return _savedValue;
	}
};

class NeuronalNetwork { // Fully connected columns
	vector< vector<Node*> > _columns;  //  I -> C0 -> C1 -> C2(output)

	ActivationFunction _activationFunction;

	int _weightCount;

public:

	NeuronalNetwork(const vector<int>& columns,
					ActivationFunction activationFunction)
		: _columns(columns.size())
		, _activationFunction(activationFunction)
		, _weightCount(0){
		if(columns.size() == 0){
			throw NeuronalException("NeuronalNetwork#constructor: 0 columns");
		}

		for(int i=0; i<columns.size(); i++){
			if(columns[i] <= 0){
				throw NeuronalException("NeuronalNetwork#constructor: column with 0 elements");
			}

			vector<Node*>& column = _columns[i];
			column.resize(columns[i]);

			for(Node*& n : column){
				if(i == 0){
					n = new InputNode();
				}else{
					NeuronalNode* node = new NeuronalNode();
					node->setActivationFunction(_activationFunction);
					n = node;
				}
			}
		}
	}

	NeuronalNetwork(const NeuronalNetwork& neuronalNetwork) {
		_columns = neuronalNetwork._columns;
		_activationFunction = neuronalNetwork._activationFunction;
		_weightCount = 0;

		for(int i=0; i<_columns.size(); i++){
			vector<Node*>& column = _columns[i];

			for(int j=0; j<column.size(); j++){
				column[j] = neuronalNetwork._columns[i][j]->clone();
			}
		}
	}

	NeuronalNetwork(NeuronalNetwork&& neuronalNetwork) {
		_columns = std::move(neuronalNetwork._columns);
		_activationFunction = neuronalNetwork._activationFunction;
		_weightCount = neuronalNetwork._weightCount;
	}

	NeuronalNetwork& operator=(const NeuronalNetwork& neuronalNetwork) {
		_columns = neuronalNetwork._columns;
		_activationFunction = neuronalNetwork._activationFunction;
		_weightCount = 0;

		for(int i=0; i<_columns.size(); i++){
			vector<Node*>& column = _columns[i];

			for(int j=0; j<column.size(); j++){
				column[j] = neuronalNetwork._columns[i][j]->clone();
			}
		}
		return *this;
	}

	NeuronalNetwork& operator=(NeuronalNetwork&& neuronalNetwork) {
		_columns = std::move(neuronalNetwork._columns);
		_activationFunction = neuronalNetwork._activationFunction;
		_weightCount = neuronalNetwork._weightCount;
		return *this;
	}

	~NeuronalNetwork(){
		for(vector<Node*>& v : _columns){
			for(Node* n : v){
				delete n;
			}
		}
	}

	void reconnectAll(){
		_weightCount = 0;
		for(int i=1; i<_columns.size(); i++){
			for(Node* node : _columns[i]){
				NeuronalNode* neuronalNode = dynamic_cast<NeuronalNode*>(node);
				if(neuronalNode){
					for(Node* n : _columns[i-1]){
						neuronalNode->setSource(n);
					}
					_weightCount += _columns[i-1].size();
				}
			}
		}
	}

	int getWeightCount() const {
		return _weightCount;
	}

	vector<double> getWeights() const {
		vector<double> weights(getWeightCount());
		int n = 0;
		for(int i=1; i<_columns.size(); i++){
			for(Node* node : _columns[i]){
				NeuronalNode* neuronalNode = dynamic_cast<NeuronalNode*>(node);
				if(neuronalNode){
					vector<double>& nodeWeights = neuronalNode->getWeights();
					for(int j=0; j<nodeWeights.size(); j++){
						weights[n++] = nodeWeights[j];
					}
				}
			}
		}
		return weights;
	}

	void setWeights(vector<double> weights){
		if(weights.size() != getWeightCount()){
			throw NeuronalException("NeuronalNetwork#setWeights: different weight sizes");
		}
		int n = 0;
		for(int i=1; i<_columns.size(); i++){
			for(Node* node : _columns[i]){
				NeuronalNode* neuronalNode = dynamic_cast<NeuronalNode*>(node);
				if(neuronalNode){
					vector<double>& nodeWeights = neuronalNode->getWeights();
					for(int j=0; j<nodeWeights.size(); j++){
						nodeWeights[j] = weights[n++];
					}
				}
			}
		}
	}

	void resetAllValues(){
		for(int i=1; i<_columns.size(); i++){
			for(Node* node : _columns[i]){
				NeuronalNode* neuronalNode = dynamic_cast<NeuronalNode*>(node);
				if(neuronalNode){
					neuronalNode->resetValue();
				}
			}
		}
	}

	ActivationFunction getActivationFunction() const {
		return _activationFunction;
	}

	int getColumnCount() const {
		return _columns.size();
	}

	vector<Node*>& getColumn(int n){
		return _columns[n];
	}

	vector<Node*>& getInputs(){
		return _columns.front();
	}

	vector<Node*>& getOutputs(){
		return _columns.back();
	}
};

double getRandomWeight(){
    return ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
}

void randomizeAll(vector<double>& weights){
	for(double& weight : weights){
		weight = getRandomWeight();
	}
}

vector<double> getChild(const vector<double>& parent1, const vector<double>& parent2){
	if(parent1.size() != parent2.size()){
		throw NeuronalException("global#getChild: different parent weight sizes");
	}

	vector<double> child(parent1.size());

	bool actualParent = rand()%2;
	int pos = 0;

	while(pos < child.size()){
		size_t count = rand()%(child.size() - pos + 1);

		for(int i = pos; i < pos + count; i++){
			if(actualParent){
				child[i] = parent1[i];
			}else{
				child[i] = parent2[i];
			}
		}

		pos += count;
		actualParent = !actualParent;
	}

	return child;
}

void mutateOnce(vector<double>& weights){
	weights[rand()%weights.size()] = getRandomWeight();
}

bool isPrime(int n){
    if(n < 2)
        return false;
    if(n == 2)
        return true;
    if(n % 2 == 0)
        return false;
    for(int i=3; i*i <= n; i+=2)
        if(n % i == 0)
            return false;
    return true;
}

bool nextConwayState(bool neighbours[3][3]){
    int neighbourCount = 0;
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            if(i != 1 || j != 1)
                neighbourCount += neighbours[i][j];

    bool actualState = neighbours[1][1];
    if(!actualState && neighbourCount == 3)
        return true;
    if(actualState && (neighbourCount == 2 || neighbourCount == 3))
        return true;
    return false;
}

double getFitness(NeuronalNetwork& network){
    double fitness = 1000;

    bool state[3][3];

    for(int j=0; j<512; j++){
        network.resetAllValues();

        for(int i=0; i<9; i++){
            char value = (j >> i) & 1;
            state[i/3][i%3] = value;
            ((InputNode*)network.getInputs()[i])->setData(value);
        }

        bool nextState = nextConwayState(state);

        /*fitness += ((network.getOutputs()[0]->getOutput() * 2 - 1) * (nextState ? 1.0 : -1.0) > 0) * 10000;
        fitness += (network.getOutputs()[0]->getOutput() * 2 - 1) * (nextState ? 1.0 : -1.0);*/
		fitness += (network.getOutputs()[0]->getOutput() > 0.5 ^ nextState) * 10000;
		fitness += (network.getOutputs()[0]->getOutput() * 2 - 1) * (nextState ? 1.0 : -1.0);
    }

    return fitness;
}

void testAndSetFitnesses(vector<NeuronalNetwork>& networks, vector<double>& fitnesses){
	for(int i=0; i<networks.size(); i++){
		fitnesses[i] = getFitness(networks[i]);
	}
}

void testAndSetFitnessesAsync(vector<NeuronalNetwork>& networks, vector<double>& fitnesses){
    vector<future<double>> futures;

    futures.reserve(networks.size());

	for(int i=0; i<networks.size(); i++){
        futures.push_back(async(getFitness, ref(networks[i])));
	}

	for(int i=0; i<networks.size(); i++){
        fitnesses[i] = futures[i].get();
	}
}

double logisticFunction(double d){
    return 1 / (1 + exp(-d));
}

int main(){
	srand(time(0));

	NeuronalNetwork baseNetwork({9, 2, 1}, logisticFunction);

	baseNetwork.getColumn(0).push_back(new InputNode(1)); // Bias
	baseNetwork.getColumn(1).push_back(new InputNode(1));
	baseNetwork.reconnectAll();

	vector<NeuronalNetwork> networks(100, baseNetwork);
	vector<double> fitnesses(networks.size());

	{
		vector<double> weights = baseNetwork.getWeights();
		for(NeuronalNetwork& network : networks){
			network.reconnectAll();
			randomizeAll(weights);
			network.setWeights(weights);
		}
	}

    double higherFitness = -1e100;

	for(int generation=1; ; generation++){
		cout << "Generation " << generation << '\r' << flush;
		testAndSetFitnesses(networks, fitnesses);

        double actualHigherFitness = higherFitness;
        int actualHigherFitnessIndex = -1;

		for(int i=0; i<fitnesses.size(); i++){
			//cout << "# " << fitness << endl;

			if(fitnesses[i] > higherFitness){
                actualHigherFitness = fitnesses[i];
                actualHigherFitnessIndex = i;
			}

			/*if(fitness == 20){
                cout << "Ended on generation " << generation << endl;
                return 0;
			}*/
		}

		if(actualHigherFitness > higherFitness){
            cout << "New fitness " << actualHigherFitness << " on generation " << generation;
            cout << " (" << (actualHigherFitnessIndex < networks.size()/4*3 ? "Child" : "Random") << ")" << endl;
            higherFitness = actualHigherFitness;
		}

		actualHigherFitness = fitnesses[0];
		int father = 0,
            mother = 1;

		for(int i=1; i<fitnesses.size(); i++){
            if(fitnesses[i] >= higherFitness){
                higherFitness = fitnesses[i];
                mother = father;
                father = i;
            }else if(fitnesses[mother] < fitnesses[i]){
                mother = i;
            }
		}

		vector<double> fatherWeights = networks[father].getWeights(),
                       motherWeights = networks[mother].getWeights(),
                       randomNetWeights = networks[rand()%networks.size()].getWeights();

        networks[0].setWeights(fatherWeights);
        networks[1].setWeights(motherWeights);

		for(int i=2; i<networks.size(); i++){
            vector<double> weights;

            if(i < networks.size()/4*3){
                weights = getChild(fatherWeights, randomNetWeights);
            }else{
                weights.resize(networks[i].getWeightCount());
                randomizeAll(weights);
            }

            while(rand()%2 == 0){
                mutateOnce(weights);
            }

            networks[i].setWeights(move(weights));
		}
	}
}
