#ifndef _WRITETOFILE_HEADER_
#define _WRITETOFILE_HEADER_

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

void write_to_file(int num, std::vector<std::pair<double,double>> data){
	std::ofstream output;
	output.open("results.txt", std::ios::out);
	for(int i = 0; i< num; i++){
		output << i << "\t" << data.at(i).first << "\t" << data.at(i).second << std::endl;
	}
}

#endif