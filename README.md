# magnetosphere-project
Simulation of charged particles in Earth's magnetosphere using the magnetic dipole model.

Initial conditions of the particle are editable in `prep_analyse.cpp`

Code **will not** run as-is because of a missing dependency `<bibli_fonctions.h>` it only affects allocation\deallocation intructions in the `rk4_modif()` function of `integration.cpp`
