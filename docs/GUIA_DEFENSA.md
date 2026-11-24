# Guía de explicación del código y preguntas para la defensa

**Proyecto:** Defensa del Reino · Programación II · UTEC 2026-2

**Integrantes:** Cristopher Aron Lopez Garay, Wesley Rivaldo Laura Choquejahua, Ernesto Sebastian Davila Quichca, Gonzalo Alejandro Ugaz Espinoza y Santiago Nicolás Villacorta Nacurena.

Esta guía sirve para que **todos** los integrantes puedan explicar cualquier parte del código. El docente puede preguntarle a cualquiera por cualquier clase, así que no basta con dominar solo la parte propia.

> Cómo estudiarla: lee la parte 1 con el código abierto al lado. Después tápate las respuestas de la parte 3 y respóndelas en voz alta. Si una respuesta no te sale en 30 segundos, vuelve al archivo correspondiente.

---

## 1. Mapa del proyecto

```
src/
├── main.cpp                 Solo crea Juego y llama a menu()
├── juego/Juego.h/.cpp       Controlador: menú, partida, oleadas, tienda
├── mapa/Mapa.h/.cpp         Matriz char**, camino aleatorio, dibujo ASCII
├── torre/Torre.h/.cpp       Vida de la torre, operator-=, barra de vida
├── jugador/Jugador.h/.cpp   Nombre y oro, operator+= / -=
├── oleada/Oleada.h/.cpp     Crea/destruye enemigos, turnos de la oleada
├── enemigos/                Enemigo (abstracta) → Goblin, Orco, Troll
├── defensas/                Defensa (abstracta) → Arquero, Mago, Canon
├── estadisticas/            Partida (un registro) y Estadisticas (archivo)
└── consola/Consola.h/.cpp   Entrada validada, colores, pausas (static)
```

### 1.1 `main.cpp`
```cpp
int main() {
    Juego juego;
    juego.menu();
    return 0;
}
```
Cumple la regla del enunciado: **en el main solo se llama al menú dentro de un objeto**. No hay funciones sueltas: todo está en clases. `Consola` agrupa funciones de utilidad como métodos `static`.

### 1.2 `Juego`: el controlador
- **Atributos por valor:** `Mapa mapa`, `Torre torre`, `Jugador jugador`, `Estadisticas estadisticas` → **composición** (nacen y mueren con `Juego`).
- **`vector<Defensa*> defensas`:** `Juego` las crea con `new` en `colocarDefensa()` y las borra con `delete` en `liberarDefensas()` → también es **composición**.
- `menu()` es un bucle `do-while` con las 3 opciones del enunciado.
- `jugar()` recorre las 10 oleadas. En cada una crea un objeto **local** `Oleada oleada(numero);` (dependencia), llama a `faseDePreparacion()` y luego a `ejecutarOleada()`.
- `ejecutarOleada()` contiene el **bucle de turnos** (ver 1.8).
- `crearDefensa(tipo, f, c)` es una pequeña **fábrica**: devuelve un `Defensa*` que apunta a un `Arquero`, `Mago` o `Canon`.
- En la tienda hay un **arreglo estático**: `const int costos[] = {0, Arquero::COSTO, Mago::COSTO, Canon::COSTO};`.
- `Juego(const Juego&) = delete;` evita copiar un objeto que es dueño de punteros.

### 1.3 `Mapa`: matriz dinámica y camino
- `char** celdas` es una **matriz dinámica**: primero `new char*[filas]` y luego cada fila `new char[columnas]`.
- `int* caminoFila`, `int* caminoCol`: **arreglos dinámicos paralelos** con las coordenadas de cada casilla del camino. El enemigo solo guarda un índice (`posicion`) y el mapa lo traduce a (fila, columna).
- `generarCamino()`: empieza en una fila aleatoria de la columna 0, avanza de 2 a 5 casillas a la derecha y sube o baja de 1 a 3, hasta la última columna. **Nunca va a la izquierda**, así que no se cruza y siempre termina. Marca `E` (entrada) y `T` (torre).
- `vector<Defensa*> defensas`: **agregación**. Las usa para dibujarlas y para saber si una casilla está ocupada (`esLibre`), pero **no las borra** en su destructor.
- El destructor libera en orden inverso: primero cada fila (`delete[] celdas[i]`) y luego el arreglo de punteros (`delete[] celdas`).
- `dibujar()` copia el terreno en un `vector<string> vista`, pinta encima defensas y enemigos, y la imprime con números de fila y columna.
- `llegoAlFinal(pos)` devuelve `pos >= largoCamino - 1` porque la última casilla del camino **es** la torre.

### 1.4 `Enemigo` y sus hijas
- `Enemigo` es **abstracta**: `simbolo()` y `habilidad()` son virtuales puras (`= 0`).
- El constructor recibe los **valores base** y el **nivel** (= número de oleada) y los escala:
  - vida +35 % por nivel, daño +25 % por nivel, recompensa +1 por nivel y velocidad +1 desde la oleada 7.
- Estados (`enum class EstadoEnemigo`): `EnEspera → EnCamino → Eliminado | LlegoATorre`.
- Métodos virtuales que las hijas redefinen (**polimorfismo**):

| Clase | Redefine | Comportamiento |
|---|---|---|
| Goblin | `pasosPorTurno()` | Con menos del 50 % de vida corre +1 casilla (pánico) |
| Orco | `danioEfectivo(d)` | Armadura: recibe `max(1, d - armadura)` |
| Troll | `avanzar()` | Regenera el 4 % de su vida y luego llama a `Enemigo::avanzar()` |

- `operator-` aplica el daño **llamando a `danioEfectivo()`**, que es virtual. Por eso `*orco - 15` le quita menos vida que `*goblin - 15`.
- `operator>` compara posiciones: "está más adelantado". Se usa para ordenar los objetivos.
- `cobrarRecompensa()` devuelve el oro **una sola vez** gracias a `recompensaCobrada`.

### 1.5 `Defensa` y sus hijas
- `Defensa` es **abstracta**: `atacar()`, `simbolo()` y `descripcion()` son virtuales puras.
- `Enemigo* objetivo` es la **asociación**: la defensa conoce a su objetivo pero no es su dueña.
- `enemigosEnRango()` (protegido, reutilizado por las 3 hijas) filtra por **distancia Manhattan** `|Δfila| + |Δcol| <= rango` y ordena con `operator>` (el más adelantado primero).
- `operator+=(niveles)`: +10 de daño por nivel y +1 de rango en los niveles 2 y 4, con un máximo de nivel 4.
- `costoMejora() = costo * nivel * 3 / 4`.

| Clase | Costo | Daño | Rango | `atacar()` |
|---|---|---|---|---|
| Arquero | 50 | 15 | 4 | Conserva su `objetivo` mientras siga vivo y en rango; si no, elige al más adelantado |
| Mago | 90 | 20 | 3 | Golpea a los 2 más adelantados y los congela 1 turno |
| Canon | 150 | 30 | 2 | Daño en área a todos los que estén en rango y luego recarga 1 turno |

### 1.6 `Oleada`
- Crea a sus enemigos en el constructor (`generarEnemigos`) y los borra en el destructor → **composición**.
- Composición de la oleada *n*: `3+n` goblins, orcos desde la oleada 2 y trolls desde la oleada 4, intercalados.
- `liberarSiguiente()`: pone **un** enemigo en camino por turno, para que no salgan amontonados.
- `procesarLlegadas(mapa, torre)`: los que llegaron hacen `torre -= danio`.
- `operator[]` devuelve el i-ésimo enemigo y lanza `out_of_range` si el índice es inválido.
- `Oleada(const Oleada&) = delete;`: regla de tres.

### 1.7 `Torre`, `Jugador`, `Partida` y `Estadisticas`
- `Torre::operator-=` resta vida sin bajar de 0. `operator<<` dibuja la barra `[#####-----]` en verde, amarillo o rojo.
- `Jugador::operator+=` suma oro y `operator-=` lo resta solo si alcanza (nunca queda negativo).
- `Partida` es un registro con `operator<<` (escribe una línea `a;b;c...`), `operator>>` (la lee y la separa por `;`) y `operator<` (compara puntajes).
- `Estadisticas::cargar()` lee el archivo con `ifstream` y `getline`, salta las líneas con `#` y las corruptas. `registrar()` escribe con `ofstream(ruta, ios::app)`, es decir, agrega al final sin borrar. `mostrar()` imprime la tabla, el resumen y el Top 3 (con `sort` y `operator<`).

### 1.8 El bucle de un turno (`Juego::ejecutarOleada`)
```cpp
while (!oleada.terminada() && !torre.estaDestruida() && turno < MAX_TURNOS) {
    oleada.liberarSiguiente();                 // 1. entra un enemigo
    for (Defensa* d : defensas)                // 2. atacan (POLIMORFISMO)
        d->atacar(oleada.getEnemigos(), mapa);
    jugador += oleada.cobrarRecompensas(bajas); // 3. oro por eliminados
    oleada.avanzar();                          // 4. se mueven (POLIMORFISMO)
    oleada.procesarLlegadas(mapa, torre);      // 5. dañan la torre
}
for (Defensa* d : defensas) d->olvidarObjetivo();   // antes de destruir la oleada
```
`MAX_TURNOS` es solo una protección contra bucles infinitos.

### 1.9 Dónde está cada requisito técnico

| Requisito | Dónde |
|---|---|
| Clases | 16 clases, todo el código dentro de ellas |
| Punteros | `Enemigo*`, `Defensa*`, `char**`, `int*`, `objetivo` |
| Arreglos | `caminoFila`/`caminoCol` (dinámicos), `costos[]` (estático) |
| Matrices | `Mapa::celdas` (`char**` de 10 × 24) |
| Vectores | `vector<Enemigo*>`, `vector<Defensa*>`, `vector<Partida>`, `vector<string>` |
| Herencia | Enemigo → Goblin/Orco/Troll, Defensa → Arquero/Mago/Canon |
| Polimorfismo | `atacar`, `avanzar`, `pasosPorTurno`, `danioEfectivo`, `simbolo`, `habilidad`, `descripcion` |
| Composición | Juego◆Mapa/Torre/Jugador/Estadisticas/Defensa, Oleada◆Enemigo, Estadisticas◆Partida |
| Agregación | Mapa◇Defensa |
| Asociación | Defensa→Enemigo (`objetivo`) |
| Sobrecarga | `-`, `>`, `+=`, `-=`, `<<`, `>>`, `<`, `[]` |
| Archivos | `historial.txt` (`ifstream`, `ofstream` con `ios::app`) |

---

## 2. Guion de la demo (2 minutos)

1. Ejecutar el programa → **2. Ver estadísticas** para mostrar el historial (lectura de archivo).
2. **1. Jugar** → nombre → `s` (animado).
3. Opción **4** para mostrar los enemigos de la oleada (`operator<<` de Enemigo).
4. Opción **1** para colocar un Arquero junto al camino (elegir una casilla `.` a 1-2 de distancia del `#`). Luego un Mago.
5. Opción **5** para mostrar la animación: enemigos saliendo de `E`, la vida bajando y el oro subiendo.
6. En la siguiente preparación: opción **2** para mejorar el Arquero (`operator+=`) y opción **3** para listar defensas.
7. Para terminar rápido: opción **0** (rendirse) → mostrar que se guardó → **2. Ver estadísticas** con la nueva fila.

> Consejo: practica la demo antes. Ten el programa compilado y abierto antes de empezar a exponer.

---

## 3. Preguntas probables del docente (con respuestas)

### A. Conceptos de POO

**1. ¿Qué es una clase abstracta y cuáles tienen?**
Es una clase con al menos un método virtual puro (`= 0`), por lo que no se puede instanciar. Tenemos `Enemigo` (`simbolo()`, `habilidad()`) y `Defensa` (`atacar()`, `simbolo()`, `descripcion()`). Sirven como "contrato" para sus hijas.

**2. ¿Dónde está el polimorfismo? Muéstrame una línea.**
En `Juego::ejecutarOleada`: `d->atacar(oleada.getEnemigos(), mapa);`. `d` es `Defensa*`, pero en tiempo de ejecución se ejecuta el `atacar` de Arquero, Mago o Canon según el objeto real. También en `oleada.avanzar()` → `e->avanzar()` (el Troll regenera) y en `operator-`, que llama a `danioEfectivo()` (el Orco tiene armadura).

**3. ¿Qué pasaría si `atacar` no fuera `virtual`?**
Se ejecutaría siempre la versión de `Defensa` (enlace estático), que además es pura, así que ni compilaría. En general, sin `virtual` se pierde el polimorfismo: se llamaría al método del tipo del puntero y no al del objeto.

**4. ¿Por qué el destructor de `Enemigo` es virtual?**
Porque borramos enemigos a través de un `Enemigo*` (`delete e` en `~Oleada`). Si no fuera virtual, solo se ejecutaría el destructor de la base y eso es comportamiento indefinido. Con `virtual` se llama primero al de la hija y luego al de la base.

**5. ¿Qué significa `override`?**
Le indica al compilador que el método redefine uno virtual de la base. Si nos equivocamos en el nombre o la firma, da error en vez de crear un método nuevo sin avisar.

**6. ¿Para qué usan `protected`?**
Para que las clases hijas accedan a atributos como `vida`, `danio` o `rango` (por ejemplo, Goblin lee `vida` y `vidaMax`) sin que sean públicos para el resto del programa.

**7. ¿Qué es el encapsulamiento en su proyecto?**
Los atributos son privados o protegidos y se modifican solo por métodos u operadores que validan. Por ejemplo, `Jugador::operator-=` nunca deja el oro en negativo y `Torre::operator-=` nunca deja la vida por debajo de 0.

### B. Relaciones entre clases (pregunta casi segura)

**8. Explica la diferencia entre composición, agregación y asociación con su código.**
- **Composición** (◆): el todo crea y destruye a la parte. `Oleada` crea enemigos con `new` y los borra en `~Oleada()`. `Juego` tiene `Mapa`, `Torre`, `Jugador` y `Estadisticas` como atributos por valor, y también crea y borra las defensas.
- **Agregación** (◇): el todo referencia partes que no le pertenecen. `Mapa` tiene `vector<Defensa*>` para dibujarlas, pero no las borra: si el mapa se reinicia, las defensas siguen existiendo en `Juego`.
- **Asociación** (→): un objeto conoce a otro para trabajar con él, sin relación todo-parte. `Defensa` tiene `Enemigo* objetivo`.

**9. ¿Por qué `Juego` → `Defensa` es composición si es un vector de punteros?**
Porque lo que define la relación no es el puntero, sino quién controla el ciclo de vida. `Juego` hace `new` en `colocarDefensa()` y `delete` en `liberarDefensas()`; ninguna defensa sobrevive a `Juego`.

**10. ¿Y por qué `Mapa` → `Defensa` es agregación si también son punteros?**
Porque `Mapa` nunca hace `new` ni `delete` de defensas. Solo las referencia. Su destructor libera su matriz, pero no las defensas.

**11. ¿Por qué `Juego` → `Oleada` es dependencia y no composición?**
Porque `Oleada` no es un atributo de `Juego`: es una variable **local** dentro del `for` de `jugar()`. Se usa durante una iteración y se destruye sola al terminar.

**12. ¿Qué es la herencia en el diagrama y en el código?**
Es la flecha con triángulo hueco. En el código: `class Goblin : public Enemigo`. Es una relación "es un".

### C. Sobrecarga de operadores

**13. ¿Qué operadores sobrecargaron y por qué?**
`Enemigo::operator-` (daño), `operator>` (más adelantado), `Defensa::operator+=` (mejora), `Torre::operator-=` (daño), `Jugador::operator+=/-=` (oro), `Oleada::operator[]` (acceso), `Partida::operator<`, `<<` y `>>` (ranking y archivo), además de `<<` en casi todas las clases para mostrar información.

**14. ¿Por qué `operator<<` es `friend` y no un método?**
Porque el operando izquierdo es `std::ostream` (`cout << enemigo`), no nuestra clase. Un método miembro siempre tiene a `*this` a la izquierda. Al ser `friend`, la función libre puede acceder a los atributos privados.

**15. ¿Por qué devuelven `ostream&` o `Defensa&`?**
Para poder encadenar: `cout << a << b;` o `(*d += 1) += 1;`. Devolver por referencia evita copias y devuelve el mismo objeto.

**16. `operator-` normalmente no modifica el objeto. ¿Por qué el suyo sí?**
El enunciado sugiere usar `-` para aplicar daño, y así lo implementamos: `*enemigo - danio` modifica la vida y devuelve el propio enemigo para poder encadenar. Somos conscientes de que lo convencional sería que `-` devuelva un objeto nuevo y que `-=` modifique. Por eso en `Torre` y `Jugador` usamos `-=`, que es lo idiomático.

**17. ¿Cómo funciona `operator>>` de `Partida`?**
Lee una línea con `getline`, la separa por `;` con un `stringstream`, valida que tenga 8 campos y convierte los números con `stoi` dentro de un `try`. Si algo falla, activa el `failbit` y `Estadisticas` ignora esa línea.

### D. Memoria y punteros

**18. ¿Cómo se crea y destruye la matriz del mapa?**
`celdas = new char*[filas];` y luego, por cada fila, `celdas[i] = new char[columnas];`. Para destruirla se hace `delete[] celdas[i]` en cada fila y después `delete[] celdas`. Se usa `delete[]` porque son arreglos.

**19. ¿Qué es la regla de tres y cómo la aplican?**
Si una clase necesita un destructor propio (porque maneja memoria), también necesita definir o prohibir el constructor de copia y la asignación. Si no, la copia por defecto copia los punteros y ambos objetos harían `delete` de lo mismo (doble liberación). En `Mapa`, `Oleada` y `Juego` los prohibimos con `= delete`.

**20. ¿Puede quedar un puntero colgante en `objetivo`?**
No. Durante una oleada los enemigos muertos **no se borran**, solo cambian de estado, así que `objetivo` siempre apunta a memoria válida. Al terminar la oleada, `Juego` llama a `olvidarObjetivo()` en cada defensa **antes** de que `~Oleada` libere los enemigos.

**21. ¿Por qué `liberarDefensas()` llama primero a `mapa.quitarDefensas()`?**
Para que el mapa no se quede con punteros a defensas ya borradas. Primero se rompe la agregación y después se hace `delete`.

**22. ¿Cómo comprobaron que no hay fugas de memoria?**
Compilamos con `-fsanitize=address,undefined` (AddressSanitizer) y jugamos partidas completas: no reportó fugas ni accesos inválidos.

**23. ¿Por qué usan `Enemigo*` en el vector y no `Enemigo`?**
Porque `Enemigo` es abstracta (no se puede instanciar) y, aunque no lo fuera, guardar objetos por valor en un `vector<Enemigo>` "cortaría" la parte de la hija (*object slicing*) y se perdería el polimorfismo.

### E. Lógica del juego y algoritmo

**24. ¿Cómo se genera el camino y por qué siempre llega a la torre?**
Alterna tramos a la derecha (2-5) y verticales (1-3) sin salirse del mapa. Como cada vuelta del `while` avanza al menos una columna a la derecha, termina en la última columna. Como nunca va a la izquierda, no se cruza.

**25. ¿Cómo sabe una defensa si un enemigo está en rango?**
El enemigo solo tiene `posicion`, un índice del camino. El mapa lo traduce a fila y columna con `getFilaCamino` y `getColCamino`, y la defensa calcula la distancia Manhattan `|Δf| + |Δc|` y la compara con su `rango`.

**26. ¿A quién ataca primero una defensa?**
Al enemigo más adelantado en el camino, el más cercano a la torre. Para eso `enemigosEnRango` ordena con `sort` usando `operator>` de `Enemigo`. El Arquero además mantiene su objetivo mientras siga en rango.

**27. ¿Cómo afectan los niveles a los enemigos?**
El nivel es el número de oleada. En el constructor de `Enemigo` la vida sube 35 % por nivel, el daño 25 % y la recompensa 1, y desde la oleada 7 la velocidad aumenta en 1. Además, cada oleada trae más enemigos y tipos más fuertes.

**28. ¿Cuándo se gana y cuándo se pierde?**
Se gana si la torre sigue con vida al terminar la oleada 10. Se pierde si su vida llega a 0 (o si el jugador se rinde). En ambos casos se registra la partida.

**29. ¿Cómo evitan que se cobre dos veces el oro de un enemigo?**
`cobrarRecompensa()` solo devuelve el oro si el estado es `Eliminado` y `recompensaCobrada` es `false`; luego lo marca como `true`.

**30. ¿Cómo comprobaron que el juego está balanceado?**
Con un simulador que juega 300 partidas por estrategia. Al principio una estrategia sencilla ganaba el 100 % de las veces, así que ajustamos recompensas, vida y costos. Ahora, sin defensas se pierde en la oleada 4; con mezcla y mejoras óptimas se gana el 98 %; colocando al azar, el 60 %.

### F. Archivos y entrada

**31. ¿Dónde se guarda el historial y en qué formato?**
En `historial.txt`, en la carpeta desde donde se ejecuta el programa. Es una tabla con una fila por partida y columnas alineadas separadas por `|`: jugador, fecha, resultado, oleadas, eliminados, daño, oro y defensas. `operator<<` escribe cada columna con `setw` (ancho fijo), por eso se puede seguir agregando con `ios::app` sin que se descuadre. `operator>>` separa por `|` y quita los espacios. Las líneas con `#` (el encabezado) son comentarios.

**32. ¿Qué pasa si el archivo no existe o una línea está dañada?**
Si no existe, `cargar()` simplemente no carga nada y `registrar()` lo crea con su encabezado. Si una línea está dañada, `operator>>` marca el error y esa línea se salta, sin romper el programa.

**33. ¿Por qué `ios::app`?**
Para agregar al final del archivo sin borrar las partidas anteriores.

**34. ¿Qué pasa si el usuario escribe letras en un menú?**
`Consola::leerEntero` lee la línea completa con `getline`, intenta convertirla con `stoi` dentro de un `try` y verifica el rango. Si falla, muestra "Valor inválido" y vuelve a pedir. Así `cin` nunca queda en estado de error.

**35. ¿Por qué `Consola` tiene todo `static`?**
Porque son utilidades que no necesitan estado por objeto, y así cumplimos la regla de "no funciones sueltas" sin crear objetos innecesarios.

### G. Preguntas "trampa" o de diseño

**36. Si quisiera agregar un enemigo "Dragón", ¿qué cambiarían?**
Crear `class Dragon : public Enemigo` con su `simbolo()`, `habilidad()` y el comportamiento que redefina (por ejemplo, `avanzar()`), y agregarlo en `Oleada::generarEnemigos()`. El bucle de turnos y las defensas no cambian: ese es el beneficio del polimorfismo.

**37. ¿Por qué separaron en `.h` y `.cpp`?**
El `.h` declara la interfaz y el `.cpp` la implementación. Así se compila cada archivo por separado, se reducen las dependencias y se evitan inclusiones circulares.

**38. ¿Qué es `class Mapa;` al inicio de `Defensa.h`?**
Una declaración anticipada (*forward declaration*). `Defensa.h` solo necesita saber que `Mapa` existe, porque la usa por referencia. Así se evita la dependencia circular `Defensa.h ↔ Mapa.h`. El `.cpp` sí incluye el `.h` completo.

**39. ¿Qué es `#pragma once`?**
Evita que un archivo de cabecera se incluya dos veces en la misma unidad de compilación.

**40. ¿Por qué `static constexpr` para `COSTO` o `NIVEL_MAXIMO`?**
Son constantes de la clase, no de cada objeto, y se conocen en tiempo de compilación. Así la tienda puede mostrar el precio sin crear una defensa.

**41. ¿Por qué `explicit` en algunos constructores?**
Para evitar conversiones implícitas raras, como que un `int` se convierta solo en una `Torre`.

**42. ¿Qué es `enum class` y por qué no `int`?**
Es una enumeración con tipo propio. Es más legible (`EstadoEnemigo::Eliminado`) y no se mezcla accidentalmente con enteros.

**43. ¿Usaron IA?**
Sí, como apoyo (Claude, de Anthropic) para revisar el código, proponer mejoras de diseño, generar el diagrama y estructurar el informe. Está declarado en el informe. Todo el equipo revisó y entiende el código; por eso podemos explicar cada parte.

**44. ¿Qué mejorarían con más tiempo?**
Enemigos voladores que ignoren el camino, un jefe final, vender defensas, guardar una partida en curso en un archivo binario y usar punteros inteligentes (`unique_ptr`) para automatizar la liberación de memoria.

**45. ¿Por qué no usaron `unique_ptr`?**
Porque el curso pide trabajar con punteros y manejar la memoria explícitamente con `new` y `delete`. Lo compensamos aplicando la regla de tres y verificando con AddressSanitizer.

---

## 4. Compilar y ejecutar (VS Code)

- **Ctrl+Shift+B** compila todo (tarea de `.vscode/tasks.json`).
- **Terminal → Ejecutar tarea… → "Ejecutar Defensa del Reino"** compila y abre el juego.
- El botón ▶ de **Code Runner** también sirve: en `.vscode/settings.json` está configurado para compilar **todos** los `.cpp`. Sin esa configuración, Code Runner compila solo el archivo abierto y da errores de "undefined reference".

Comando manual en PowerShell:
```powershell
g++ -std=c++17 -Wall -Wextra (Get-ChildItem -Recurse src -Filter *.cpp).FullName -o build/DefensaDelReino.exe
.\build\DefensaDelReino.exe
```
> En PowerShell no funciona `src/*/*.cpp`, porque Windows no expande ese comodín. Por eso se usa `Get-ChildItem`.

**Si preguntan por `CMakeLists.txt`:** es un archivo estándar que describe cómo compilar el proyecto en cualquier entorno (CLion, la extensión CMake Tools de VS Code o la terminal). Nosotros compilamos con g++ desde VS Code; el CMake está para que el docente lo pueda compilar con cualquier herramienta.

**Antes de entregar en Gradescope:** jugar 2 o 3 partidas reales y adjuntar el `historial.txt` generado en la carpeta del proyecto (es el "Archivo de Historial" que pide el enunciado).
