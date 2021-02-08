#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>


std::string charstar_to_str(char*);
std::vector<double> break_comma(std::string, double*, double*, std::string);
std::vector<std::vector<double>> read_file(std::string, double*, double*, std::string);
std::vector<std::vector<double>> normalize(std::vector<std::vector<double>>, double*, double*);
int predict(std::vector<double> , std::vector<std::vector<double>>);
std::vector<int> get_predictions(std::vector<std::vector<double>>, std::vector<std::vector<double>>);
