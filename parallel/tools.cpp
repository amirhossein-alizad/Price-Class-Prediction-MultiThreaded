#include "tools.h"

std::string address;
std::vector<std::vector<double>> weights;
std::vector<std::vector<std::vector<double>>> train_data(NUM_OF_THREADS);
std::vector<std::vector<double>> min_vec(NUM_OF_THREADS, std::vector<double>(20, 100000));
std::vector<std::vector<double>> max_vec(NUM_OF_THREADS, std::vector<double>(20, 0));
std::vector<double> final_min(20, 100000);
std::vector<double> final_max(20, 0);
double sum[NUM_OF_THREADS];

void calc_accuracy()
{
  double size = 0;
  double predicted = 0;
  for(int i = 0; i < NUM_OF_THREADS; i++)
  {
    size += train_data[i].size();
    predicted += sum[i];
  }
  printf("Accuracy: %.2f%%\n", predicted * 100/size);
}

std::vector<double> train_data_break(std::string line, long id)
{
  std::vector<double> data;
  std::string str;
  int size = line.size();
  int l = 0;
  for(int i = 0; i < size; i++)
  {
    if(line[i] == ',')
    {
      double num = std::stold(str);
      data.push_back(num);
      if(num < min_vec[id][l])
        min_vec[id][l] = num;
      if(num > max_vec[id][l])
        max_vec[id][l] = num;
      str = "";
      l += 1;
    }
    else
      str += line[i];
    if(i == size - 1)
      data.push_back(std::stold(str));
  }
  return data;
}

void set_address(std::string adr)
{
  address = adr + "/train_";
}

std::string charstar_to_str(char* inadr)
{
  int len = strlen(inadr);
  std::string adr;
  for(int i = 0; i < len; i++)
    adr += inadr[i];
  return adr;
}

std::vector<double> break_comma(std::string line)
{
  std::vector<double> data;
  std::string str;
  int size = line.size();
  for(int i = 0; i < size; i++)
  {
    if(line[i] == ',')
    {
      double num = std::stold(str);
      data.push_back(num);
      str = "";
    }
    else
      str += line[i];
    if(i == size - 1)
      data.push_back(std::stof(str));
  }
  return data;
}

void read_weight(std::string adr)
{
  std::ifstream fin;
  fin.open(adr+"/weights.csv");
  std::string line;
  getline(fin, line);
  while(getline(fin, line))
    weights.push_back(break_comma(line));
}

void find_min_max()
{
  for(int i = 0; i < 20; i++)
  {
    double max = 0;
    double min = 100000;
    for(int j = 0; j < NUM_OF_THREADS; j++)
    {
      if(max < max_vec[j][i])
        max = max_vec[j][i];
      if(min > min_vec[j][i])
        min = min_vec[j][i];
    }
    final_max[i] = max;
    final_min[i] = min;
  }
}

void normalize(int k)
{
  int columns = train_data[k][0].size();
  int rows = train_data[k].size();
  for(int i = 0; i < columns - 1; i++)
  {
    for(int j = 0; j < rows; j++)
      train_data[k][j][i] = (train_data[k][j][i] - final_min[i]) / (final_max[i] - final_min[i]);
  }
}

int predict(std::vector<double> phone)
{
  int columns = phone.size() - 1;
  double ans0 = 0, ans1 = 0, ans2 = 0, ans3 = 0;
  for(int j = 0; j < columns; j++)
  {
    ans0 += weights[0][j] * phone[j];
    ans1 += weights[1][j] * phone[j];
    ans2 += weights[2][j] * phone[j];
    ans3 += weights[3][j] * phone[j];
  }
  ans0 += weights[0][columns];
  ans1 += weights[1][columns];
  ans2 += weights[2][columns];
  ans3 += weights[3][columns];
  double max = ans0;
  max = (max < ans1) ? ans1 : max;
  max = (max < ans2) ? ans2 : max;
  max = (max < ans3) ? ans3 : max;
  if(max == ans0)
    return 0;
  if(max == ans1)
    return 1;
  if(max == ans2)
    return 2;
  return 3;

}

void* get_predictions(void* tid)
{
  long id = (long) tid;
  normalize(id);
  int rows = train_data[id].size();
  int columns = train_data[id][0].size() - 1;
  std::vector<int> predictions;
  for(int i = 0; i < rows; i++)
    if(predict(train_data[id][i]) == train_data[id][i][columns])
      sum[id] += 1;
  pthread_exit(NULL);
}

void* read_file(void* tid)
{
  long id = (long) tid;
  std::string adr = address + std::to_string(id)+ ".csv";
  std::ifstream fin;
  fin.open(adr);
  std::string line;
  getline(fin, line);
  while(getline(fin, line))
    train_data[id].push_back(train_data_break(line, id));
  pthread_exit(NULL);
}

void multi_thread_read()
{
  pthread_t ids[NUM_OF_THREADS];
  for(int i = 0; i < NUM_OF_THREADS; i++)
  {
    long id = i;
    int ret_code = pthread_create(&ids[i], NULL, read_file, (void*)id);
  }
  void* status;
  for(int i = 0; i < NUM_OF_THREADS; i++)
    int ret = pthread_join(ids[i], &status);
}

void multi_thread_predict()
{
  pthread_t ids[NUM_OF_THREADS];
  for(int i = 0; i < NUM_OF_THREADS; i++)
  {
    long id = i;
    int ret_code = pthread_create(&ids[i], NULL, get_predictions, (void*)id);
  }
  void* status;
  for(int i = 0; i < NUM_OF_THREADS; i++)
    int ret = pthread_join(ids[i], &status);
}
