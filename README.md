# Programmes simulant un double pendule

> [!WARNING]
> Le programme "double_pendule" necessite la librairie graphique Raylib

Programmes permettant de résoudre numériquement l'équation du double pendule. 
Le programme embarque plusieurs solveur d'EDO comme: RK4, Verlet, Euler et DOPRI45.
La visualisation de cette simulation se fait de plusieurs manière selon les programmes.
Le programme double\_pendule utilise la librairie graphique Raylib.
Le programme double\_pendule\_

## Pour commencer

```console
$ make all
$ ./project/double_pendule
$ ./project/double_pendule_console
$ make create_picture
$ create_video
$ play_video
```
## Exemple en image du double pendule avec Raylib

[![Double-pendule](./picture/Double-pendule.png)](/picture)

## Exemple en image du double pendule en mode console

[![Double-pendule-console](./picture/Double-pendule-console.png)](/picture)

<!-- TODO: -->
<!-- ## Exemple en image du double pendule en mode vidéo -->

<!-- [![Double-pendule](./Double-pendule.png)](/picture) -->


# Infos interessantes

## Source d'inspiration
> Site Simulatant un double pendule ultra réaliste
https://www.myphysicslab.com/pendulum/double-pendulum-en.html
(Formule dispo en bas de page)

> Code du site
https://github.com/myphysicslab/myphysicslab/blob/master/src/lab/model/AdaptiveStepSolver.ts

## Aide pour l'établissement des équations
> Résolution théorique: Résoudre les équations différentielles en utilisant la transformée de Laplace
> [Résolution celon_Laplace_](https://www.mathforengineers.com/french/Laplace-transform/solve-differential-equations-using-Laplace-transform.html)

> simplification trigonométrique
> [Formule trigo](https://www.mathforu.com/hors-programme/formulaire-de-trigonometrie/)


## visualisation de la Série de Fourier
https://isaacvr.github.io/coding/fourier_transform/

## Equation rugeuse (stiff) qui sont les pires equations pour solveurs explicites
https://en.wikipedia.org/wiki/Backward_differentiation_formula
https://en.wikipedia.org/wiki/Stiff_equation

## Programme solveur explicite DOP853
https://www.unige.ch/~hairer/software.html
