#include "tools.h"

using namespace std;

vector<std::vector<double>> weights;

int main(int argc, char* argv[])
{
  std::string adr = charstar_to_str(argv[1]);
  double min[50];
  fill_n(min, 50, 100000);
  double max[50] = {0};
  string data_adr = adr + "/train.csv";
  string weight_adr = adr + "/weights.csv";
  weights = read_file(weight_adr, min, max, "weight");
  std::vector<std::vector<double>> data = read_file(data_adr, min, max, "train");
  data = normalize(data, max, min);
  int columns = data[0].size() - 1;
  double predict = 0;
  std::vector<int> predictions = get_predictions(data, weights);
  for(int i = 0; i < predictions.size(); i++)
  {
    if(predictions[i] == data[i][columns])
      predict+=1;
  }
  printf("Accuracy: %.2f%%\n", predict * 100/data.size());
}
