#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <ctime>
#include <thread>

using namespace std;

template<typename T>
void quickSort(vector<T>& l, int start, int count){
	if(count <= 1){
		return;
	}
	
	T pivot = l[start];
	int n = start,
		m = start + count - 1;

	for(int i = start + 1; i < start + count; i++){
		if(l[i] < pivot){
			l[n++] = l[i];
		}else{
			l[m--] = l[i];
		}
	}

	l[n] = pivot;

	quickSort(l, start, n - start);
	quickSort(l, n + 1, count - n - start - 1);
}

/*template<typename T, template <typename T> typename Container>
Container<T> quickSort(const Container<T>& l){
	if(l.size()<=1){
		return l;
	}
	
	T pivot = l.front();
	Container<T> a,b;
	
	auto it = l.begin(); it++;
	for(; it!=l.end(); it++){
		if(*it<pivot)
			a.push_front(*it);
		else
			b.push_front(*it);
	}
	
	a = quickSort(a);
	b = quickSort(b);
	a.push_back(pivot);
	a.insert(a.end(), b.begin(),b.end());
	return a;
}

template<typename T, template <typename T> typename Container>
Container<T> bubbleSort(Container<T> l){
	for(auto it1 = l.begin(); it1!=l.end(); it1++){
		auto it2 = it1; it2++;
		for(; it2!=l.end(); it2++)
			if(*it1>*it2){
				T temp = *it1;
				*it1 = *it2;
				*it2 = *it1;
			}
	}
	return l;
}

template<typename T, template <typename T> typename Container>
Container<T> mergeSort(const Container<T>& l){
	if(l.size()<=1){
		return l;
	}
	Container<T> a,b, li;
	
	auto it1 = l.begin();
	for(int i=0; i<l.size()/2; i++)
		it1++;
	
	a.insert(a.begin(), l.begin(),it1);
	b.insert(b.begin(), it1,l.end());
	
	a = mergeSort(a);
	b = mergeSort(b);
	
	it1 = a.begin();
	auto it2 = b.begin();
	while(it1 != a.end() && it2 != b.begin()){
		if(*it1<*it2){
			li.push_back(*it1);
			it1++;
		}else{
			li.push_back(*it2);
			it2++;
		}
	}
	while(it1 != a.end()){
		li.push_back(*it1);
		it1++;
	}
	while(it2 != b.end()){
		li.push_back(*it2);
		it2++;
	}
	
	return li;
}*/

template<typename Container>
void coutContainer(const Container& l){
	for(const auto& it : l){
		cout << it << " ";
	}

	cout << endl;
}

int main(){
	srand(time(0));
	
	vector<int> l, li;
	for(int i=0; i<10000; i++)
		l.push_back(rand());
	
	clock_t cl = clock();
	quickSort(l, 0, l.size());
	cout << "QuickSort : " << (clock()-cl) << "ms" << endl;
	
	/*clock_t cl = clock();
	li = quickSort(l);
	cout << "QuickSort : " << (clock()-cl) << "ms" << endl;
	
	cl = clock();
	li = mergeSort(l);
	cout << "MergeSort : " << (clock()-cl) << "ms" << endl;
	
	cl = clock();
	li = bubbleSort(l);
	cout << "BubbleSort : " << (clock()-cl) << "ms" << endl;*/
}