# magnetosphere-project
Simulation of charged particles moving in the Earth's magnetosphere using the magnetic dipole model.

Initial conditions of the particle are editable in `prep_analyse.cpp`.
Run `main.cpp` to build the simulation's data files that can then be visualised with `visualisation.py`.

⚠️ Code **will not** run as-is because of a missing dependency `<bibli_fonctions.h>`. It only affects allocation\deallocation intructions in the `rk4_modif()` function of `integration.cpp`.
