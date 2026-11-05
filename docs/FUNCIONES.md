# Defensa del Reino · Funciones del proyecto

Lista de todas las funciones que hay hoy en el código: en qué archivo y clase están, y qué hace cada una.
Estado al 04/11/2026 (actualizado en la noche).

---

## Enemigo
**Archivo:** `src/enemigos/Enemigo.h` · Clase abstracta, base de todos los enemigos.

| Función | Qué hace |
|---|---|
| `Enemigo(nombre, vida, velocidad, danio, recompensa)` | Constructor: guarda los datos del enemigo. La posición arranca en 0. |
| `~Enemigo()` | Destructor `virtual`, para que el `delete` desde un `Enemigo*` borre bien a la hija. |
| `avanzar()` | Suma la velocidad a la posición: mueve al enemigo por el camino. |
| `estaVivo()` | Devuelve `true` si su vida es mayor que 0. |
| `operator-(int)` | Le resta daño a la vida, sin bajar de 0. Se usa como `*enemigo - danio`. |
| `operator<<` (friend) | Imprime símbolo, nombre y vida: `cout << *enemigo`. |
| `getNombre()` | Devuelve el nombre. |
| `getVida()` | Devuelve la vida actual. |
| `getVelocidad()` | Devuelve la velocidad (casillas que avanza por turno). |
| `getDanio()` | Devuelve el daño que le hace a la torre. |
| `getRecompensa()` | Devuelve el oro que da al morir. |
| `getPosicion()` | Devuelve la casilla del camino en la que va. |
| `setPosicion(int)` | Cambia la posición. La Oleada la usa para que los enemigos entren en fila (0, −2, −4...). |
| `simbolo()` | Virtual pura: cada hija devuelve su letra para el mapa. |

---

## Goblin, Orco y Troll
**Archivos:** `src/enemigos/Goblin.h`, `src/enemigos/Orco.h`, `src/enemigos/Woodtroll.h` · Heredan de `Enemigo`.

| Clase | Función | Qué hace |
|---|---|---|
| `Goblin` | `Goblin()` | Crea un Goblin: vida 30, velocidad 2, daño 5, recompensa 10. |
| `Goblin` | `simbolo()` | Devuelve `'G'`. |
| `Orco` | `Orco()` | Crea un Orco: vida 60, velocidad 1, daño 10, recompensa 40. |
| `Orco` | `simbolo()` | Devuelve `'O'`. |
| `Troll` | `Troll()` | Crea un Troll: vida 120, velocidad 1, daño 25, recompensa 40. |
| `Troll` | `simbolo()` | Devuelve `'W'`. |

---

## Torre
**Archivo:** `src/torre/Torre.h`

| Función | Qué hace |
|---|---|
| `Torre(vida)` | Constructor: guarda la vida actual y la máxima. |
| `recibirDanio(int)` | Resta daño a la vida, sin bajar de 0. |
| `operator-=(int)` | Lo mismo con operador: `torre -= 10;`. |
| `getVida()` | Devuelve la vida actual. |
| `estaDestruida()` | `true` si la vida llegó a 0. Es la condición de derrota. |
| `mostrarEstado()` | Imprime "Vida de la torre: X/Y". |

---

## Defensa
**Archivo:** `src/defensas/Defensa.h` · Clase abstracta, base de todas las defensas.

| Función | Qué hace |
|---|---|
| `Defensa(nombre, fila, columna, danio, rango, nivel, costo)` | Constructor: guarda los datos de la defensa. |
| `~Defensa()` | Destructor `virtual`. |
| `atacar(enemigos, mapa)` | Virtual pura: cada hija ataca a su manera. |
| `simbolo()` | Virtual pura: la letra de la defensa en el mapa. |
| `operator+=(int)` | Mejora la defensa: +1 de nivel, +10 de daño y +1 de rango por cada nivel. |
| `operator<<` (friend) | Imprime símbolo, nombre, nivel, daño, rango y posición. |
| `getNombre()` | Devuelve el nombre. |
| `getDanio()` | Devuelve el daño por ataque. |
| `getRango()` | Devuelve el alcance. |
| `getNivel()` | Devuelve el nivel actual. |
| `getCosto()` | Devuelve cuánto oro cuesta colocarla. |
| `getFila()` | Devuelve la fila donde está en el mapa. |
| `getColumna()` | Devuelve la columna donde está en el mapa. |

---

## Arquero, Mago y Cañón
**Archivos:** `src/defensas/Arquero.h`, `src/defensas/Mago.h`, `src/defensas/Canon.h` · Heredan de `Defensa`.

Los tres `atacar()` siguen los mismos pasos: saltar a los muertos, traducir la posición del enemigo a fila y columna con el mapa, medir la distancia Manhattan y compararla con el rango. Lo que cambia es a cuántos golpean.

| Clase | Función | Qué hace |
|---|---|---|
| `Arquero` | `Arquero(fila, columna)` | Crea un Arquero: daño 15, rango 4, costo 50. |
| `Arquero` | `atacar(enemigos, mapa)` | Golpea al **primero** que encuentra en rango. |
| `Arquero` | `simbolo()` | Devuelve `'A'`. |
| `Mago` | `Mago(fila, columna)` | Crea un Mago: daño 20, rango 3, costo 90. |
| `Mago` | `atacar(enemigos, mapa)` | Golpea **hasta 2** enemigos en rango. |
| `Mago` | `simbolo()` | Devuelve `'M'`. |
| `Canon` | `Canon(fila, columna)` | Crea un Cañón: daño 30, rango 2, costo 150. |
| `Canon` | `atacar(enemigos, mapa)` | Golpea a **todos** los que estén en rango. |
| `Canon` | `simbolo()` | Devuelve `'C'`. |

---

## Mapa
**Archivo:** `src/mapa/Mapa.h`

| Función | Qué hace |
|---|---|
| `Mapa(filas, columnas)` | Reserva la matriz dinámica, la llena de `.` y genera el camino al azar. |
| `~Mapa()` | Libera la matriz y los arreglos del camino. |
| `aleatorio(min, max)` *(privada)* | Sortea un número entre min y max. |
| `agregarCelda(f, c)` *(privada)* | Agrega una casilla al camino y la marca con `#`. |
| `avanzarTramo(dir, pasos)` *(privada)* | Avanza varias casillas en una dirección, agregando cada una. |
| `generarCamino()` *(privada)* | Arma el camino aleatorio de izquierda a derecha y pone la torre `T` al final. |
| `getFilas()` | Devuelve cuántas filas tiene el mapa. |
| `getColumnas()` | Devuelve cuántas columnas tiene el mapa. |
| `getLargoCamino()` | Devuelve cuántas casillas tiene el camino. |
| `getFilaCamino(pos)` | Traduce una posición del camino a su fila. Devuelve -1 si está fuera. |
| `getColCamino(pos)` | Traduce una posición del camino a su columna. Devuelve -1 si está fuera. |
| `llegoAlFinal(pos)` | `true` si esa posición ya alcanzó la torre. |
| `esLibre(f, c)` | `true` si la celda está dentro del mapa y vacía. |
| `colocarDefensa(f, c, simbolo)` | Si la celda está libre, pone ahí la defensa y devuelve `true`. |
| `dibujar(enemigos)` | Imprime el mapa con los enemigos vivos encima, sin ensuciar la matriz original. |

---

## Oleada
**Archivo:** `src/oleada/Oleada.h`

| Función | Qué hace |
|---|---|
| `Oleada(n)` | Guarda el número de oleada y crea 3 Goblins. |
| `~Oleada()` | Borra a los enemigos que queden. |
| `agregarEnemigo(e)` *(privada)* | Le da al enemigo su posición inicial (0, −2, −4...) para que entren en fila, y lo mete al vector. |
| `avanzarEnemigos()` | Hace avanzar a cada enemigo vivo según su velocidad. |
| `eliminarMuertos()` | Saca del vector a los enemigos muertos (con `delete`) y devuelve el oro que dieron. |
| `procesarLlegadas(torre, mapa)` | Los enemigos que llegaron al final le restan su daño a la torre (con `-=`) y se borran. Devuelve el daño total. |
| `terminada()` | `true` si ya no queda ningún enemigo vivo. |
| `getEnemigos()` | Devuelve el vector por referencia, para que las defensas y el mapa lo usen. |
| `getNumOleada()` | Devuelve el número de oleada. |

**Pendiente:** dificultad según el número de oleada (más enemigos, Orcos y Trolls, enemigos más fuertes).

---

## Pendientes

| Archivo | Estado |
|---|---|
| `src/juego/Juego.h` | Vacío. Mientras tanto, `src/main.cpp` tiene una prueba de una oleada completa con un Arquero. |
| `src/estadisticas/Estadisticas.h` | Vacío |
