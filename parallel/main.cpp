#include "tools.h"

using namespace std;


int main(int argc, char* argv[])
{
  std::string adr = charstar_to_str(argv[1]);
  set_address(adr);
  read_weight(adr);
  multi_thread_read();
  find_min_max();
  multi_thread_predict();
  calc_accuracy();
}
