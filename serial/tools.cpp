#include "tools.h"

std::string charstar_to_str(char* inadr)
{
  int len = strlen(inadr);
  std::string adr;
  for(int i = 0; i < len; i++)
    adr += inadr[i];
  return adr;
}

std::vector<double> break_comma(std::string line, double min[], double max[], std::string address)
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
      if(address == "train")
      {
        if(num > max[l])
          max[l] = num;
        if(num < min[l])
          min[l] = num;
      }
      str = "";
      l += 1;
    }
    else
      str += line[i];
    if(i == size - 1)
      data.push_back(std::stof(str));
  }
  return data;
}

std::vector<std::vector<double>> read_file(std::string path, double min[], double max[], std::string address)
{
  std::ifstream fin;
  fin.open(path);
  std::vector<std::vector<double>> data;
  std::string line;
  getline(fin, line);
  while(getline(fin, line))
    data.push_back(break_comma(line, min, max, address));
  return data;
}

std::vector<std::vector<double>> normalize(std::vector<std::vector<double>> data, double max[], double min[])
{
  int columns = data[0].size();
  int rows = data.size();
  for(int i = 0; i < columns - 1; i++)
    for(int j = 0; j < rows; j++)
      data[j][i] = (data[j][i] - min[i]) / (max[i] - min[i]);
  return data;
}

int predict(std::vector<double> phone, std::vector<std::vector<double>> weights)
{
  int columns = phone.size() - 1;
  int size = weights.size();
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

std::vector<int> get_predictions(std::vector<std::vector<double>>data, std::vector<std::vector<double>> weights)
{
  int rows = data.size();
  std::vector<int> predictions;
  for(int i = 0; i < rows; i++)
    predictions.push_back(predict(data[i], weights));
  return predictions;
}
