import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

## TRAJECTORIES

l = np.loadtxt('particle.dat')
nit=l[0,0]
dt=l[0,1]

t=l[1:,0]
x=l[1:,1]
y=l[1:,2]
z=l[1:,3]
vx=l[1:,4]
vy=l[1:,5]
vz=l[1:,6]

extrema = np.loadtxt('ex_z.dat')
tz=extrema[:,1]
exz=extrema[:,2]

#3d plot
fig3d = plt.figure()
ax = fig3d.add_subplot(projection='3d')
ax.plot(x,y,z)
ax.set_title('Trajectory')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z')

#x(t),y(t)
fig, axs = plt.subplots(2,1)

axs[0].plot(t,x)
axs[0].set_title('x(t)')
axs[0].set_xlabel('t (in s)')
axs[0].set_ylabel('x (in m)')

axs[1].plot(t,y)
axs[1].set_title('y(t)')
axs[1].set_xlabel('t (in s)')
axs[1].set_ylabel('y (in m)')

##INVARIANTS AND PERIODS

#invariants
idp = np.loadtxt('inv_dip.dat')
ilong = np.loadtxt('inv_long.dat')

tdp = idp[1:,0]
inv_dp = idp[1:,1]

inv_long = ilong[1:-1]
nbper = range(len(inv_long))

fig2, axs2 = plt.subplots(1,2)

axs2[0].plot(tdp,inv_dp)
axs2[0].set_title(r'$\mu(t)=\frac{m{u_\perp}\!^2}{\|\vec{B}\|}$')
axs2[0].set_xlabel('t (in s)')
axs2[0].set_ylabel(r'$\mu$ (in C.m)')

avg = ilong[-1]
avg = '{:.4e}'.format(avg)

axs2[1].scatter(nbper,inv_long, label=r'$<J> = $'+str(avg))
axs2[1].set_title(r'$J = \int_{C_1}^{C_2}u_\parallel ds$')
axs2[1].set_xlabel('Half-periods')
axs2[1].set_ylabel('J (in m^2/s)')
axs2[1].legend()

#periods Tx Ty

Tx = np.loadtxt('Tx.dat')
Ty = np.loadtxt('Ty.dat')
Tz = np.loadtxt('Tz.dat')

fig3, axs3 = plt.subplots(2,1)

axs3[0].plot(Tx[:-1,0],Tx[:-1,1], label='<Tx> = '+str('{:.4e}'.format(Tx[-1,0])))
axs3[0].set_title('Tx(t), over a period')
axs3[0].set_xlabel('t (in s)')
axs3[0].set_ylabel('Tx (in s)')
axs3[0].legend()

axs3[1].plot(Ty[:-1,0],Ty[:-1,1], label='<Ty> = '+str('{:.4e}'.format(Ty[-1,0])))
axs3[1].set_title('Ty(t), over a period')
axs3[1].set_xlabel('t (in s)')
axs3[1].set_ylabel('Ty (in s)')
axs3[1].legend()


#z(t), period Tz
fig4, axs4 = plt.subplots(2,1)

axs4[0].plot(t,z)
axs4[0].scatter(tz,exz, marker='*', color='red', label="extrema of z(t)")
axs4[0].set_title('z(t)')
axs4[0].set_xlabel('t (in s)')
axs4[0].set_ylabel('z (in m)')
axs4[0].legend()

axs4[1].scatter(Tz[:-1,0],Tz[:-1,1], label='<Tz> = '+str('{:.4e}'.format(Tz[-1,0])))
axs4[1].set_title('Tz(t), over a period')
axs4[1].set_xlabel('t (in s)')
axs4[1].set_ylabel('Tz (in s)')
axs4[1].legend()

plt.show()
