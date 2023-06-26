#include<stdlib.h>

/*Lance tous les fichiers a la suite*/

int main(){
  int status = system("ccc integration.cpp; ccc prep_analyse.cpp; ccc analyse.cpp; python3 vis.py");
  return 0;
}
