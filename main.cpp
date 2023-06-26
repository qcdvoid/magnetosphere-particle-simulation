#include<stdlib.h>

/*launches all necessary files*/

int main(){
  int status = system("ccc integration.cpp; ccc prep_analyse.cpp; ccc analyse.cpp; python3 visualisation.py");
  return 0;
}
