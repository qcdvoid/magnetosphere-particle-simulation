# magnetosphere-project
Simulation of charged particles moving in the Earth's magnetosphere using the magnetic dipole model.

Initial conditions of the particle are editable in `prep_analyse.cpp`.
Run `main.cpp` to build the simulation's data files that can then be visualised with `visualisation.py`.

>[!WARNING]
>Code **will not** run as-is because of missing dependency `<bibli_fonctions.h>`. It only affects pointer allocation\deallocation intructions in the `rk4_modif()` function defined in `integration.cpp`.
