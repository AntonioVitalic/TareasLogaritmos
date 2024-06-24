# Tarea 2: Algoritmo Dijkstra

## Integrantes:
- Juan Luis Cid Lagos
- Daniel Ortuño J.
- Antonio Vitalic
  
## Profesor: 
Gonzalo Navarro<br>
Sección 2

## Configuración:
El proceso de compilación y ejecución fue llevado a cabo en Windows 11, y Linux.

### Compilación:
$ javac .\experiments\Experiments.java (Windows)

$ javac ./experiments/Experiments.java (Linux)

### Ejecución
$ java experiments.Experiments

### Resultado:
La ejecución del programa genera 3 archivos CSV que contienen la cantidad de vértices en el nombre, y dentro se encuentra el tiempo de ejecución de cada algoritmo, para cada cantidad de aristas por esa cantidad de vértices.

## Estructuras
Se definen las siguientes clases para trabajar con los grafos en el paquete struct.graph:
- Graph: clase que representa un grafo con la ayuda de Node (vértices).
- Node: clase de representación de los vértices.

Se definen 2 estructuras para trabajar con las colas de prioridad, en el paquete structs.q:
- QHeap: clase que representa una cola de prioridades usando heap.
- QFib: clase que representa una cola de prioridades utilizando heaps de fibonacci.

Se define una clase para realizar el algoritmo con ambas metodologías, en el paquete algoritmo:
- Dijkstra: posee 2 métodos estáticos, correspondiente a cada metodología a usar.
  - heapHeap: realiza el algoritmo utilizando heaps.
  - heapFib: realiza el algoritmo utilizado colas de fibonacci.
  
Se define también la clase Experiments (nuestro main en este caso), en el paquete experiments. En esta clase se encuentra la metodología a llevar a cabo para realizar los experimentos iterativos, realizando también la creación de archivos que contienen los resultados finales a cada experimentación realizada.
