#include <iostream>
#include <memory>
#include <optional>
#include <cassert>

using namespace std;

template<typename T>
class Enumerator{
	class IterableEnumerator{
	
		class Iterator{
			Enumerator<T>* _enumerator;
			std::optional<T> _current;
			
		public:
		
			Iterator(Enumerator<T>* enumerator) : _enumerator(enumerator) {
				if(_enumerator != nullptr && _enumerator->moveNext()){
					_current = _enumerator->current();
				}
			}
			
			Iterator(const Iterator&) = delete;
			
			Iterator(Iterator&& iterator){
				_enumerator = iterator._enumerator;
				_current = iterator._current;
				
				_enumerator = nullptr;
				_current.reset();
			}
			
			Iterator& operator++() {
				if(_enumerator->moveNext()){
					_current = _enumerator->current();
				}else{
					_current.reset();
				}
				
				return *this;
			}
		
			T operator*() const {
				return _current.value();
			}
			
			bool operator==(const Iterator& iterator) const {
				return _current.has_value() == iterator._current.has_value();
			}
			
			bool operator!=(const Iterator& iterator) const {
				return !operator==(iterator);
			}
		};
		
		Enumerator<T>* _enumerator;
		mutable bool _wasBeginCalled;
		
	public:

		IterableEnumerator(Enumerator<T>* enumerator) : _enumerator(enumerator), _wasBeginCalled(false) {}
			
		IterableEnumerator(const IterableEnumerator&) = delete;
		
		IterableEnumerator(IterableEnumerator&& iterableEnumerator){
			_enumerator = iterableEnumerator._enumerator;
			_wasBeginCalled = iterableEnumerator._wasBeginCalled;
			
			_enumerator = nullptr;
		}

		Iterator begin() const {
			if(_enumerator == nullptr){
				throw std::logic_error("IterableEnumerator with nullptr Enumerator");
			}
			
			if(_wasBeginCalled){
				throw std::logic_error("Begin called twice on an IterableEnumerator");
			}
			
			_wasBeginCalled = true;
			
			return Iterator(_enumerator);
		}
		
		Iterator end() const {
			return Iterator(nullptr);
		}
	};
	
public:

	virtual T current() const = 0;
	
	virtual bool moveNext() = 0;
	
	virtual void reset() = 0;
	
	IterableEnumerator toIterable(){
		return IterableEnumerator(this);
	}
};



class Range{
	struct RangeData{
		int from;
		int to;
	};
	
	class Iterator{
		int _n;
		
	public:
		
		Iterator(int n) : _n(n) {}
		
		Iterator& operator++() {
			++_n;
			return *this;
		}
	
		int operator*() const {
			return _n;
		}
		
		bool operator==(const Iterator& iterator) const {
			return _n == iterator._n;
		}
		
		bool operator!=(const Iterator& iterator) const {
			return !operator==(iterator);
		}
	};
	
	class RangeEnumerator : public Enumerator<int> {
		RangeData _range;
		int _current;
		
	public:
		
		RangeEnumerator(RangeData range) : _range(std::move(range)) {
			reset();
		}
		
		int current() const override {
			return _current;
		}
		
		bool moveNext() override {
			++_current;
			return _current < _range.to;
		}
		
		void reset() override {
			_current = _range.from - 1;
		}
	};
	
	RangeData _range;
	
public:

	Range(int from, int to) : _range({from, to}){
		if(from > to){
			throw std::invalid_argument("From can´t be greater than To");
		}
	}

	Iterator begin() const {
		return Iterator(_range.from);
	}
	
	Iterator end() const {
		return Iterator(_range.to);
	}
	
	RangeEnumerator getEnumerator() const {
		return RangeEnumerator(_range);
	}
	
	const RangeData& getRange() const {
		return _range;
	}
};

int main(){
	Range range(5, 11);
	
	cout << "C++ Iterator Style" << "\n\n";
	
	for(int n : range){
		cout << n << endl;
	}
	
	cout << "\n\nC# Enumerator Style" << "\n\n";
	
	auto enumerator = range.getEnumerator();
	
	while(enumerator.moveNext()){
		cout << enumerator.current() << endl;
	}
	
	enumerator.reset();
	
	assert(enumerator.moveNext() && enumerator.current() == 5);
	
	cout << "\n\nC++ Iterator from Enumerator" << "\n\n";
	
	enumerator.reset();
	
	auto iterable = enumerator.toIterable();
	
	for(int n : iterable){
		cout << n << endl;
	}
	
	try{
		iterable.begin(); // EnumeratorIterable should be iterated at most once
		
		assert(false);
	}catch(...){}
}