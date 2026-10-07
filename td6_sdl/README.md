# td6_sdl

Simulateur du TD6 : même code applicatif (`app/`) que `td6_complete`,
tournant sur une BSP simulée avec SDL3 au lieu de la carte `bidule`.

## Prérequis

SDL3 doit être installable via `pkg-config` (c'est ce que `find_package
(SDL3)` utilise).

## Compiler

```sh
cmake -S . -B build
cmake --build build
```

## Exécuter

```sh
./build/td6_sdl
```

Une fenêtre s'ouvre avec les 3 LEDs, le neopixel, l'écran OLED, l'encodeur
et les 2 boutons poussoirs (SW1/SW2). Commandes :

- Touches `↑` / `↓` : rotation de l'encodeur (-1 / +1)
- Touche `Entrée` : appui sur l'encodeur
- Touches `A` / `Z` : appui sur SW1 / SW2
- Clic souris sur les rectangles `+`, `-`, `PUSH`, SW1, SW2 : idem
- Fermer la fenêtre pour quitter
