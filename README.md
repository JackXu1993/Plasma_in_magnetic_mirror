# Plasma-in-Magnetic-Bottle

## Principle of [Magnetic Bottle](https://en.wikipedia.org/wiki/Magnetic_mirror)
A magnetic bottle is two magnetic mirrors placed close together. For example, two parallel coils separated by a small distance, carrying the same current in the same direction will produce a magnetic bottle between them. Unlike the full mirror machine which typically had many large rings of current surrounding the middle of the magnetic field, the bottle typically has just two rings of current. Particles near either end of the bottle experience a magnetic force towards the center of the region; particles with appropriate speeds spiral repeatedly from one end of the region to the other and back. Magnetic bottles can be used to temporarily trap charged particles. It is easier to trap electrons than ions, because electrons are so much lighter This technique is used to confine very hot plasmas with temperatures of the order of 106 K.

![magnetic mirror](https://upload.wikimedia.org/wikipedia/commons/1/1e/Fields_in_magnetic_bottles.jpg)

## How to simulate a Magnetic Bottle

### Calculate the magnetic field of magnetic bottle in 3D by numeric integration
We can get the magnetic field which is generared by a coil(at the origin of the coordinate) by integrating
```
\begin{equation}
\begin{cases}
        B_{x0}(x,y,z)=\int_0^{2 \pi} \frac{Rzcos \phi}{(x^2+y^2+z^2+R^2-2xRcos \phi-2yRsin \phi)^{3/2}} d\phi\\
        B_{y0}(x,y,z)=\int_0^{2 \pi} \frac{Rzsin \phi}{(x^2+y^2+z^2+R^2-2xRcos \phi-2yRsin \phi)^{3/2}} d\phi\\
        B_{z0}(x,y,z)=\int_0^{2 \pi} \frac{R(R-xcos\phi-ysin\phi)}{(x^2+y^2+z^2+R^2-2xRcos \phi-2yRsin \phi)^{3/2}} d\phi
\end{cases}
\end{equation}
```

 Then we can get the magnetic field of a magnetic bottle (two coils) by a transformation
```
 \begin{equation}
 \begin{cases}
        B_{x}(x,y,z)=B_{x0}(x,y,z-d/2)+B_{x0}(x,y,z+d/2)\\
        B_{y}(x,y,z)=B_{y0}(x,y,z-d/2)+B_{y0}(x,y,z+d/2)\\
        B_{z}(x,y,z)=B_{z0}(x,y,z-d/2)+B_{z0}(x,y,z+d/2)
 \end{cases}
 \end{equation}
 ```
 d in these equations means the distance between two coils.

### Calculate the trace of an electron in a magnetic bottle by solving odes(newton equation and Lorentz force)
According to the Newton equation and Lorentz force equation
```
\begin{equation}
\begin{cases}
    F=ma\\
    F_{Lorentz}=qv \times x
\end{cases}
\end{equation}
```
Then we can get the equation,
```
\begin{equation}
    qv \times x=ma
\end{equation}
```
Then we need to solve the second order Ordinary Diﬀerential Equations, because we study this problem in 3D.
```
\begin{equation}
    \begin{cases}
        x''=-\frac{m}{q}(y'Bz(x,y,z)-z'By(x,y,z))\\
        y''=-\frac{m}{q}(z'Bx(x,y,z)-x'Bz(x,y,z))\\
        z''=-\frac{m}{q}(x'By(x,y,z)-y'Bx(x,y,z))
    \end{cases}
\end{equation}
```
We transfer the them into first order Ordinary Diﬀerential Equations,
```
\begin{equation}
    \begin{cases}
        vx'=-sc(vy*Bz(x,y,z)-vz*By(x,y,z))\\
        vy'=-sc(vz*Bx(x,y,z)-vx*Bz(x,y,z))\\
        vz'=-sc(vx*By(x,y,z)-vy*Bx(x,y,z))\\
        x'=vx\\
        y'=vy\\
        z'=vz
    \end{cases}
\end{equation}
```
Its intial values are
```
\begin{equation}
    \begin{cases}
        vx(0)=0\\
        vy(0)=0\\
        vz(0)=0.15e6\\
        x(0)=0\\
        y(0)=0.78R\\
        z(0)=-0.75d
    \end{cases}
\end{equation}
```

## Mathematica Program
It describes the movement of plasma in the magnetic field of magnetic mirror. This program is written by mathematica.

![Drift](http://ww3.sinaimg.cn/large/0062cUvtgw1f03md0p7z1g306s0c07el.gif)

![Magnetic Field](http://ww1.sinaimg.cn/large/0062cUvtgw1f03rwzk2z4j30pz0pen2x.jpg)

## C Program
