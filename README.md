# Defensa del Reino

Juego de estrategia (tower defense) en consola, hecho en C++17 con programación orientada a objetos.
Proyecto de Programación II, UTEC 2026-2.

## Compilar y ejecutar (VS Code)

El proyecto ya trae la configuración en `.vscode/` para g++ de MSYS2.

- **Ctrl+Shift+B**: compila todo el proyecto en `build/DefensaDelReino.exe`.
- **Terminal → Ejecutar tarea… → "Ejecutar Defensa del Reino"**: compila y abre el juego en la terminal.
- **Botón ▶ de Code Runner**: también funciona, porque está configurado para compilar todos los `.cpp` y no solo el archivo abierto.

Desde la terminal de PowerShell, en la carpeta del proyecto:
```powershell
g++ -std=c++17 -Wall -Wextra -static (Get-ChildItem -Recurse src -Filter *.cpp).FullName -o build/DefensaDelRseino.exe
.\build\DefensaDelReino.exe
```

En Linux o Git Bash: `g++ -std=c++17 -Wall -Wextra src/*.cpp src/*/*.cpp -o DefensaDelReino`.

`CMakeLists.txt` es opcional: sirve si alguien abre el proyecto en CLion o con la extensión CMake Tools.

`-static` incluye las librerías de C++ dentro del `.exe`, así funciona también en una PC sin MSYS2.

El historial de partidas se guarda en `historial.txt`, en la carpeta desde donde se ejecuta el juego (con la tarea de VS Code, la carpeta del proyecto). Es una tabla con columnas alineadas, legible en cualquier editor; el archivo de ejemplo incluido tiene 5 partidas de prueba (1 victoria, 3 derrotas y 1 rendición).

## Integrantes

- Cristopher Aron Lopez Garay
- Wesley Rivaldo Laura Choquejahua
- Ernesto Sebastian Davila Quichca
- Gonzalo Alejandro Ugaz Espinoza
- Santiago Nicolás Villacorta Nacurena

## Cómo se juega

1. **Jugar**: escribe tu nombre y elige si quieres ver las oleadas animadas.
2. Antes de cada oleada puedes colocar defensas (tipo, fila y columna sobre una casilla `.`) y mejorarlas con oro.
3. Los enemigos salen de `E`, recorren el camino `#` y atacan la torre `T`.
4. Resiste las 10 oleadas para ganar.

| Defensas | | Enemigos | |
|---|---|---|---|
| `A` Arquero (50) | un objetivo fijo, largo alcance | `G` Goblin | rápido; herido corre más |
| `M` Mago (90) | 2 objetivos y los congela | `O` Orco | armadura que reduce cada golpe |
| `C` Cañón (150) | daño en área, recarga | `W` Troll | mucha vida; se regenera |

## Documentación

- `docs/Informe_DefensaDelReino_v2.pdf` / `.docx`: informe en formato APA 7.
- `docs/DiagramaFlujo_Partida.png`: diagrama de flujo del algoritmo.
- `docs/DefensaDelReino_UML.png` y `.mmd`: diagrama de clases.
- `docs/presentacion/DefensaDelReino_Presentacion.html`: presentación (se abre en el navegador; usa ← → para avanzar, N para las notas y F para pantalla completa).
- `docs/GUIA_DEFENSA.md`: explicación del código y preguntas para la exposición.
