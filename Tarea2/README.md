# Tarea 2: Algoritmo Dijkstra

## Integrantes:
- Juan Luis Cid Lagos
- Daniel Ortuño J.
- Antonio Vitalic
  
## Profesor: 
Gonzalo Navarro<br>
Sección 2

## Configuracion:
El proceso de compilacion y ejecución fue llevado a cabo en Windows 10, y Linux.

### Compilación:
$ javac .\experiments\Experiments.java (Windows)
$ javac ./experiments/Experiments.java (Linux)

### Ejecución
$ java experiments.Experiments

### Resultado:
La ejecución del programa genera 3 archivos csv que contienen la cantidad de vertices en el nombre, y dentro se encuentra el tiempo de ejecucion de cada algoritmo, para cada cantidad de aristas por esa cantidad de vertices.

## Estructuras
Se definen las siguientes clases para trabajar con los grafos en el paquete struct.graph:
- Graph: clase que representa un grafo con la ayuda de Node (vertices).
- Node: clase de representación de los vertices.

Se definen 2 estructuras para trabajar con las colas de prioridad, en el paquete structs.q:
- QHeap: clase que representa una cola de prioridades usando heap.
- QFib: clase que representa una cola de prioridades utilizando heaps de fibonacci.

Se define una clase para realizar el algoritmo con ambas metodologias, en el paquete algoritmo:
- Dijkstra: posee 2 metodos estaticos, correspondiente a cada metodologia a usar.
  - heapHeap: realiza el algoritmo utilizando heaps.
  - heapFib: realiza el algoritmo utilizado colas de fibonacci.
  
Se define tambien la clase Experiments (nuestro main en este caso), en el paquete experiments. En esta clase se encuentra la metodologia a llevar a cabo para realizar los experimentos iterativos, realizando tambien la creación de archivos que contienen los resultados finales a cada experimentación realizada.