# Programmes simulant un double pendule

> [!WARNING]
> Le programme "double_pendule" necessite la librairie graphique Raylib

Programmes permettant de résoudre numériquement l'équation du double pendule. 
Le programme embarque plusieurs solveur d'EDO comme: RK4, Verlet, Euler et DOPRI45.
La visualisation de cette simulation se fait de plusieurs manière selon les programmes.
Le programme double\_pendule\_console fonctionne sans prérequis.
Le programme double\_pendule\_video créer un certains nombre de photo au format .ppm, nécessitera ffmpeg pour compiler toutes les photos en vidéo au format .mp4. il est aussi possible de lire la vidéo depuis une commande du Makefile avec mpv.
Le programme double\_pendule utilise la librairie graphique Raylib.h

## Pour commencer

_pour compiler tous les programmes_

```console
$ make -k all
```
_Pour lancer le programme en mode console_
```console
$ ./project/double_pendule_console
```

_Pour générer la vidéo_

```console
$ make create_video
$ make create_gif
$ make play_video
```

_Pour lancer le programme avec Raylib_

```console
$ ./project/double_pendule
```

_Pour supprimer toutes les photos générées et les exécutables_

```console
$ make clean_all
```

## Exemple en image du double pendule avec Raylib

[![Double-pendule](./picture/Double-pendule.png)](/picture)

## Exemple en image du double pendule en mode console

[![Double-pendule-console](./picture/Double-pendule-console.png)](/picture)

## Exemple en vidéo du programme de Double\_pendule\_video

[![Double-pendule-video](./picture/Double-pendule-video.gif)](/picture)


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

## Info pour Makefile
https://makefiletutorial.com/
