#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

//==========================================================================

typedef std::map<std::string, std::vector<int> > MiniblastMap;

std::string readGenome(const std::string& filename);
void mapKmers(const std::string& genome, int k, MiniblastMap& index_map);
void searchQuery(const std::string& genome, const MiniblastMap& index_map, const std::string& query, int max_mismatch, int k);

//==========================================================================
//==========================================================================

int main(int argc, char const *argv[]){
	MiniblastMap index_map;
	std::string genome;
	int k;

	std::string command;
	while (std::cin >> command) {
		if (command == "genome") {
			std::string file;
			std::cin >> file;
			genome = readGenome(file);
			index_map.clear();
		} else if (command == "kmer") {
			std::cin >> k;
			mapKmers(genome, k, index_map);
		} else if (command == "query") {
			int max_mismatch;
			std::string query;
			std::cin >> max_mismatch;
			std::cin >> query;
			searchQuery(genome, index_map, query, max_mismatch, k);
		} else if (command == "quit") {
			break;
		} 
		/*else {
		  cerr << "error unknown command " << command << endl;
		  return EXIT_FAILURE;
		}*/
	}
	return EXIT_SUCCESS;
}

//==========================================================================
//==========================================================================

std::string readGenome(const std::string& filename){
	std::ifstream infile;
	infile.open(filename);
	if (!infile.good()){
		std::cerr << "Can't open" << filename << "to read.\n";
		exit(1);
	}
	std::string genome;
	std::string line;
	while(infile >> line){
		genome.append(line);
	}
	return genome;
}

//==========================================================================

void mapKmers(const std::string& genome, int k, MiniblastMap& index_map){
	index_map.clear();
	for(int i = 0; i+k <= genome.length(); ++i){
		std::string kmer = genome.substr(i, k);
		index_map[kmer].push_back(i);
	}
}

//==========================================================================

void searchQuery(const std::string& genome, const MiniblastMap& index_map, const std::string& query, int max_mismatch, int k){
	std::cout << "Query: " << query << std::endl;
	std::string query_kmer = query.substr(0, k);
	bool match = false;
	MiniblastMap::const_iterator itr = index_map.find(query_kmer);
	if(itr != index_map.end()){
		const std::vector<int>& locs = itr->second;
		for(int idx = 0; idx < locs.size(); ++idx){
			int mismatches = 0;
			int ln = 0;
			for(ln = k; ln < query.length(); ++ln){
				int g_len = locs[idx] + ln;
				if(query[ln] != genome[g_len]){
					++mismatches;
				}
				if(mismatches > max_mismatch){ break; }
			}
			if(ln == query.length()){
				match = true;
				std::cout << locs[idx] << " " << mismatches << " " << genome.substr(locs[idx], ln) << std::endl;
			}
		}
	}

	if(!match){
		std::cout << "No Match" << std::endl;
	}
	
}

//==========================================================================
//==========================================================================


















