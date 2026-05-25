# Wasteland Waters

Wasteland Waters fue desarrollado en el lenguaje de programación C, hubo uso de Makefile, y de la librería Raylib.
Cada misión está enfocada a un término diferente o pilar diferente en el tema de plásticos e islas de plástico, enfocado en este caso a la Gran Mancha del Pacífico.

## Misiones e Interfáz

### Mejoras a desarrollar

- [x] Mejora de fuente.
- [x] Mejora de resolución.
- [x] Mejora de colores del fondo bajo soluciónn de contraste general.
- [x] Mejora de tiempo y accesibilidad.
- [x] Herramienta para brindar información a los estudiantes de forma educativa y amigable.
- [x] Pantallas de explicación a los estudiantes.
- [x] Corrección del RAE
- [x] Huella
- [ ] Trailer
- [ ] Presentación y Tutorial
- [ ] Manual general de uso
- [ ] Arreglo de Misión 2: Cadenas de Polímeros.

### Compilation instructions

1. Install the Raylib windows binaries ([LINK](https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_win64_mingw-w64.zip))
2. Extract the files and move them to `tmp/raylib-win/`
3. Run the next command:

```bash
make win # Builds the file for Windows as a .exe
```

4. Install Wine:

```bash
sudo pacman -S wine
```

5. Run the game. No DLLs needed.
