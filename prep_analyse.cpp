#include<math.h>
#include<fstream>
#include<bibli_fonctions.h>

/*
Goes through "particule.dat" a first time and find local extrema, useful for the remaining calculations
*/

void local_extrema(){//searches for local extrema of (x,y,z) in "particle.dat" and ouputs them to 3 distincts .dat files
    
    fstream dat;
    fstream ex[3];

    dat.open("particle.dat", ios::in);
    ex[0].open("ex_x.dat", ios::out);
    ex[1].open("ex_y.dat",ios::out);
    ex[2].open("ex_z_temp.dat",ios::out);

    int nit;
    double dt;

    double* q = (double*)malloc(9*sizeof(double));// q = (x,y,z,vx,vy,vz,Bx,By,Bz)
    double* qp = (double*)malloc(9*sizeof(double));// qp = q_preceding
    double* qpp = (double*)malloc(9*sizeof(double));// qpp = qp_preceding

    double n=2;//line number in "particle.dat"
    
    double t;
    double tp;// tp=t_precedent
    double* tp_ex = (double*)malloc(3*sizeof(double));// t_extrema_preceding, allows the erasure of duplicate extrema

    //useless variable during reading
    double d;

    //reads "particle.dat"
    dat >> nit >> dt >> d  >> d  >> d  >> d >> d  >> d >> d >> d;
    dat >> tp >> qpp[0] >> qpp[1] >> qpp[2] >> qpp[3] >> qpp[4] >> qpp[5] >> qpp[6] >> qpp[7] >> qpp[8];
    dat >> tp >> qp[0] >> qp[1] >> qp[2] >> qp[3] >> qp[4] >> qp[5] >> qp[6] >> qp[7] >> qp[8];
    while(dat >> t >> q[0] >> q[1] >> q[2] >> q[3] >> q[4] >> q[5] >> q[6] >> q[7] >> q[8]){
        n++;
        
        //searches for extrema
        for(int i=0;i<3;i++){
            if((qpp[i]<=qp[i])&&(q[i]<=qp[i])&&(tp-tp_ex[i]>2*dt)){
                ex[i] << n << " " << tp << " " << qp[i] << " " << 1 << std::endl; //(n_ligne,t,q,1) max local
                tp_ex[i] = tp;
            }
            if((qpp[i]>=qp[i])&&(q[i]>=qp[i])&&(tp-tp_ex[i]>2*dt)){
                ex[i] << n << " " << tp << " " << qp[i] << " " << 0 << std::endl; //(n_ligne,t,q,0) min local
                tp_ex[i] = tp;
            }
        }

        for(int i=0;i<9;i++){//buffer
            qpp[i] = qp[i];
            qp[i] = q[i];
        }
        tp = t;
    }

    //desallocation
    dat.close();
    ex[0].close();
    ex[1].close();
    ex[2].close();

    free(q);
    free(qp);
    free(qpp);
    free(tp_ex);
}

void z_extrema_filter(){//removes "noise" in the extrema of z (looks for a local maximum in the local maxima list and a local minimum in the local minima list)
    fstream exz_temp;
    fstream exz;

    exz_temp.open("ex_z_temp.dat", ios::in);
    exz.open("ex_z.dat", ios::out);
  
    double** z = (double**)malloc(2*sizeof(double*));
    z[0] = (double*)malloc(3*sizeof(double));
    z[1] = (double*)malloc(3*sizeof(double));

    int** n = (int**)malloc(2*sizeof(int*));
    n[0] = (int*)malloc(3*sizeof(int));
    n[1] = (int*)malloc(3*sizeof(int));

    double** t = (double**)malloc(2*sizeof(double*));
    t[0] = (double*)malloc(3*sizeof(double));
    t[1] = (double*)malloc(3*sizeof(double));

    int* remp = (int*)malloc(2*sizeof(int));
    remp[0]=0;
    remp[1]=1;
    
    //buffers for reading
    int n_buff;
    double t_buff;
    double z_buff;
    int cas;

    while(exz_temp >> n_buff >> t_buff >> z_buff >> cas){//on remplit les variables buffer
                
        if(remp[cas]<3){//writing to z[cas] if it is not yet full (ie there's less than 3 values)
            if((remp[cas]>1) && (z_buff == z[cas][remp[cas]-1])){
                continue;
            }
            z[cas][remp[cas]] = z_buff;
            n[cas][remp[cas]] = n_buff;
            t[cas][remp[cas]] = t_buff;
            remp[cas]++;
        }

        if((remp[0] != 3) || (remp[1] != 3)){// the two queues are not yet full, the next iteration starts
            continue;
        }

        //queues z[0] and z[1] are now full, looking for an extremum
        if((z[1][0]<=z[1][1])&&(z[1][2]<=z[1][1])){// cas = 1 <-> local max
            exz << n[1][1] << " " << t[1][1] << " " << z[1][1] << " " << 1 << std::endl;
        }
        if((z[0][0]>=z[0][1])&&(z[0][2]>=z[0][1])){// cas = 0 <-> local min
            exz << n[0][1] << " " << t[0][1] << " " << z[0][1] << " " << 0 << std::endl;
        }
        for(int i=0;i<2;i++){//updates files
            for(int j=0;j<2;j++){
                n[i][j]=n[i][j+1];
                t[i][j]=t[i][j+1];
                z[i][j]=z[i][j+1];
            }
            remp[i]=2;
        }   
    }

    exz_temp.close();
    exz.close();
    
    //remove("exz_temp.dat");

    //desallocations
    free(remp);
    for(int i=0;i<2;i++){
        free(z[i]);
        free(n[i]);
        free(t[i]);
    }
    free(z);
    free(n);
    free(t);  
}

int main(){
    local_extrema();
    z_extrema_filter();
    return 0;
}
