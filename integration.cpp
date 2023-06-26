#include<math.h>
#include<fstream>
#include<bibli_fonctions.h>

/*Numerical integration of the equations of movement, outputs in "particle.dat"*/

/*
Conventions used for directions
ux -> Earth-Sun
uz -> geographic "north"
uy -> such that (ux,uy,uz) is directly oriented
*/


/*---Constants---*/

const double mu0 = 1.0e-7;//(mu0/4pi)
const double eV = 1.6e-19;
const double mp = 1.67e-27;
const double charg = 1.6e-19;
//earth's magnetic dipole
double d[3]={0.,0.,-8.1e22};


/*---Standard Functions---*/

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

/*---Resolution of the differential system---*/

void B_update(double* q, double* B){//computes B(x,y,z)
  double r=n(q);
  double dr=sp(d,q);
  for(int i=0; i<3; i++){
    double r2 = r*r;
    B[i] = mu0*(3*dr*q[i]/r2-d[i])/r2/r;
  }
}

double* particle_init(double E, double m, double angle){//initial conditions for the particle (here a typical proton emitted by the Sun)
  double* q = (double*)malloc(9*sizeof(double));
  //x,y,z initial
  q[0] = 3.0e7;
  q[1] = 0;
  q[2] = 0;
  //vx,vy,vz initial
  double v = -sqrt(2*E/m);
  q[3] = cos(angle)*v;
  q[4] = 0;
  q[5] = sin(angle)*v;
  //B
  q[6] = 0;
  q[7] = 0;
  q[8] = 0;
  return q;
}

void syst_diff(double* q, double t, double* qp, int n, double* B){//differential system
  //q=(x,y,z,vx,vy,vz)
  //qp=(vx,vy,vz,ax,ay,az)
  qp[0]=q[3];//vx
  qp[1]=q[4];//vy
  qp[2]=q[5];//vz
  qp[3]=charg/mp*(qp[1]*B[2]-qp[2]*B[1]);//ax
  qp[4]=charg/mp*(qp[2]*B[0]-qp[0]*B[2]);//ay
  qp[5]=charg/mp*(qp[0]*B[1]-qp[1]*B[0]);//az
}

void rk4_modif(void(*sd)(double *,double,double *,int, double*),double *q,double t,double dt,int n){
  /*USING RUNGE-KUTTA-4 for numerical integration*/
  int i,k,p,PM=4;
  static const double c2=1./2,c3=1./3,c6=1./6;
  /* Allocations and initialisations */
  double **a=(double **)malloc(PM*sizeof(double *)); 
  for(i=0;i<PM;i++) a[i]=(double *)malloc(PM*sizeof(double));
  ini_D_2(a,PM,PM,c2,0.,0.,0.,0.,c2,0.,0.,0.,0.,1.,0.,c6,c3,c3,c6);
  double *b=(double *)malloc(PM*sizeof(double));
  ini_D_1(b,PM,0.,c2,c2,1.);
  double **y=(double **)malloc((PM+1)*sizeof(double *)); 
  for(i=0; i<PM+1; i++) y[i]=(double *)malloc(n*sizeof(double));
  double **z=(double **)malloc(PM*sizeof(double *)); 
  for(i=0; i<PM; i++) z[i]=(double *)malloc(n*sizeof(double));
  /* Champ B */
  double B[3];
  B_update(q,B);
  /* Calcul */
  for(i=0; i<n; i++) y[0][i]=q[i];
  for(p=1; p<=PM; p++)
    {
      sd(y[p-1],t+b[p-1]*dt,z[p-1],n,B);
      for(i=0; i<n; i++) y[p][i]=q[i];
      for(k=0; k<p; k++) {for(i=0; i<n; i++) y[p][i]=y[p][i]+dt*a[p-1][k]*z[k][i];}
    }   
  for(i=0; i<n; i++) q[i]=y[PM][i];
  q[n] = B[0];
  q[n+1] = B[1];
  q[n+2] = B[2];
  /* Deallocations */
  f_D_2(a,PM,PM); f_D_1(b,PM); f_D_2(y,PM+1,n); f_D_2(z,PM,n); 
}

int main(){
  int np=700000;     //iterations
  double dt=1.0e-3; //time step
  double E = 1e6*eV; //particle energy
  int n=6;          //dimension of phase space (3D movement)
  double t=0;

  fstream res;
  res.open("particle.dat", ios::out);

  res << np << " " << dt << " "  << 0 << " " << 0 << " " << 0 << " " << 0  << " " << 0  << " " << 0 << " " << 0 << " " << 0 << std::endl;

  double* q = particle_init(E,mp,0.7853981);//initial conditions
  
  for(int i=1; i<np; i++){//itérations de rk4
    res << t << " " << q[0] << " " << q[1] << " " << q[2] << " "  << q[3] << " "  << q[4] << " " << q[5] << " " << q[6] << " " << q[7] << " " << q[8] << std::endl;
    rk4_modif(syst_diff,q,t,dt,n);
    t+=dt;
  }
  res.close();
  return 0;
}
