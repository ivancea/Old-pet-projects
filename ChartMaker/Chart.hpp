#ifndef CHART_HPP
#define CHART_HPP

#include <fstream>
#include <vector>
#include <cstdarg>


class Chart{

	std::vector< std::vector<unsigned int> > _data;

public:

	Chart(unsigned int series) : _data(series){}

	const std::vector< std::vector<unsigned int> >& getData() const {
	    return _data;
	}

	void addValues(unsigned int firstValue, ...){
	    if(_data.size() > 0){
            va_list li;
            va_start(li, firstValue);
			_data[0].push_back(firstValue);
            for(int i=1; i<_data.size(); i++){
                _data[i].push_back(va_arg(li, unsigned int));
            }
            va_end(li);
	    }
	}

	void addValues(const std::vector<unsigned int>& values){
		for(int i=0; i<_data.size(); i++){
			_data[i].push_back(values[i]);
		}
	}

	bool saveToFile(const std::string& fileName) const {
		std::ofstream file(fileName, std::ios::trunc);
		if(!file)
			return false;

		file << _data.size() << '\n';
		if(_data.size() > 0){
			for(int i=0; i<_data[0].size(); i++){
				file << _data[0][i];
				for(int j=1; j<_data.size(); j++){
					file << ',' << _data[j][i];
				}
				file << '\n';
			}
		}

		return true;
	}

	static Chart loadFromFile(const std::string& fileName){
		std::ifstream file(fileName);
		if(!file)
			throw "Cannot open file";
		std::string line;
		getline(file, line);
		int count = strtol(line.c_str(), nullptr, 10);
		if(count <= 0)
			throw "Invalid format: series count";
		Chart chart(count);
		while(file){
			getline(file, line);
			size_t pos1 = 0,
				   pos2 = 0;
			std::vector<unsigned int> values(count);
			for(int i=0; i<count && pos2 != std::string::npos; i++){
				pos2 = line.find(',', pos1);

				values[i] = strtol(line.c_str() + pos1, nullptr, 10);
				pos1 = pos2 + 1;
			}
			chart.addValues(values);
		}
		return chart;
	}

};

#endif // CHART_HPP
