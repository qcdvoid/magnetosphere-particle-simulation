# magnetosphere-project
Simulation of the trajectory taken by charged particles entering in the Earth's magnetosphere

Initial conditions of the particle are editable in `prep_analyse.cpp`

Code won't run because of the missing dependency `<bibli_fonctions.h>` it only affects allocation\deallocation intructions in the `rk4_modif()` function of `integration.cpp`
