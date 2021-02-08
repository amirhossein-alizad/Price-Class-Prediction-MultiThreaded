#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <pthread.h>

#define NUM_OF_THREADS 4

std::string charstar_to_str(char*);
std::vector<double> break_comma(std::string);
void read_weight(std::string);
void normalize(int);
int predict(std::vector<double>);
void* get_predictions(void*);
void* read_file(void*);
void set_address(std::string);
std::vector<double> train_data_break(std::string, long);
void calc_accuracy(void);
void find_min_max(void);
void multi_thread_read(void);
void multi_thread_predict(void);
