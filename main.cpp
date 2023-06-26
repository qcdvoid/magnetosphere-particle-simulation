#include<stdlib.h>

/*launches all necessary files*/

int main(){
  int status = system("ccc integration.cpp; ccc prep_analyse.cpp; ccc analyse.cpp; python3 vis.py");
  return 0;
}
