#include<math.h>
#include<fstream>
#include<bibli_fonctions.h>

/*Final analysis of the different files*/

/*---Constants---*/
const double mp = 1.67e-27;

/*---Standard functions---*/

double sp(double* a, double* b){//dot product
  double res=0;
  for(int i=0;i<3;i++){
    res += a[i]*b[i];
  }
  return res;
}

double n(double* a){//norm
  return sqrt(sp(a,a));
}

double* unit(double* a){//unit vector
  double* u = (double*)malloc(3*sizeof(double));
  double na = n(a);
  for(int i=0;i<2;i++){
    u[i] = a[i]/na;  
  }
  return u;
}

/*---Computes invariants and movement periodicity---*/

void periodes(){//periods Tx,Ty,Tz
    
    fstream T[3];
    fstream ex[3];

    ex[0].open("ex_x.dat", ios::in);
    ex[1].open("ex_y.dat",ios::in);
    ex[2].open("ex_z.dat",ios::in);

    T[0].open("Tx.dat", ios::out);
    T[1].open("Ty.dat", ios::out);
    T[2].open("Tz.dat", ios::out);

    double t; 
    double tp;

    double T_local;
    int count;
    
    double T_moy;
    double count_moy;

    double d;//stores useless variables during lecture
    int dint;//idem

    for(int i=0;i<3;i++){
        T_local = 0.;
        count = 0;
        T_moy = 0.;
        count_moy = 0;
        tp = 0.;

        while(ex[i] >> dint >> t >> d >> dint){
            T_local += t-tp;
            count++;
            if(count==2){//saving T_local, updates T_moy
                T[i] << tp << " " << T_local << std::endl;
                count = 0;
                T_moy += T_local;
                count_moy ++;
                T_local=0.;
            }
            tp = t;
        }
        T_moy = T_moy/count_moy;
        T[i] << T_moy << " " << count_moy;//T_moy is inserted at the end of the file
    }

    for(int i=0; i<3; i++){
        ex[i].close();
        T[i].close();
    }
}

void invariants(){//computes the dipole invariant and the longitudinal invariant
    
    fstream dat;
    fstream exz;
    fstream inv_dip;
    fstream inv_long;
    
    dat.open("particle.dat",ios::in);
    exz.open("ex_z.dat", ios::in);
    inv_dip.open("inv_dip.dat", ios::out);
    inv_long.open("inv_long.dat", ios::out);

    double nit, dt;

    double* v = (double*)malloc(3*sizeof(double));//(vx,vy,vz)
    double* B = (double*)malloc(3*sizeof(double));//(Bx,By,Bz)
    double* uB;//unit vector aligned with B

    double normB;
    double normv;
    double vpar;
    double vperp_2;//vperp^2

    double t;

    double n_ex;
    double t_ex;
    double z_ex;
    double cas_ex; 

    double d;//buffer variable

    double inv_dip_val;
    double inv_long_val = 0.;

    double inv_long_moy = 0.;
    int count=0;

    dat >> nit >> dt >> d >> d >> d >> d >> d >> d >> d >> d;

    exz >> n_ex >> t_ex >> z_ex >> cas_ex;
    while (dat >> t >> d >> d >> d >>  v[0] >> v[1] >> v[2] >> B[0] >> B[1] >> B[2]){
        
        normB = n(B);
        normv = n(v);

        uB = unit(B);

        vpar = sp(v,uB);
        vperp_2 = normv*normv-vpar*vpar;

        if(t == t_ex){
            inv_long << inv_long_val << std::endl;
	    if(count != 0){
	      inv_long_moy += inv_long_val;
            }
	    count++;
            inv_long_val = 0.;
            exz >> n_ex >> t_ex >> z_ex >> cas_ex;
        }

        inv_long_val += abs(vpar);

        inv_dip_val = mp*vperp_2/normB;
        inv_dip << t << " " << dt*inv_dip_val << std::endl;
        
        free(uB);//locally created by sp()
    }

    inv_long << inv_long_moy/count << std::endl;

    //deallocations
    dat.close();
    exz.close();
    inv_long.close();
    inv_dip.close();

    free(v);
    free(B);
}

int main(){
    invariants();
    periodes();
    return 0;
}
