# Diseño y analisis de algoritmos
# Tarea 1

### Integrantes:
- Juan Luis Cid Lagos
- Daniel Ortuño J.
- Antonio Vitalic

### Profesor: Gonzalo Navarro, sección 2.


## Configuración:

Para llevar a cabo el proceso de compilación se utilizó el compilador g++ obtenido del conjunto de compiladores de GCC. Fue probado tanto en Windows como en Linux.

A continuación se presenta el proceso de compilación y ejecución.

### Compilación: 

$ g++ main.cpp -o [main.exe (windows) | main.out (linux)]
### Ejecución: $ .\main.[exe | out]

## Estructura del proyecto

La estructura se separa entre los archivos preeliminares y las versiones finales para el experimento.

### Versiones preeliminares:

Los programas Mtree.cpp, cp.cpp y ss.cpp, corresponden a las estructuras ocupadas, el algoritmo CP, y el algoritmo SS respectivamente. Estos fueron desarrollados porseparado para mantener un orden y mejor compresion de lo que se hace.

- Mtree.cpp se definen las clases: 
  - Point para trabajar con puntos y operaciones de distancia.
  - Entry para las tuplas {punto, radio, puntero}.
  - MTree para guardar una lista de Entrys (Nodo).
  - Query para representar una busqueda de un punto con un radio.
  - Cluster para realizar procesos de clusters en vectores de puntos.
  - 
    Las busquedas se implementan en la clase de MTree y ademas, se sobrecarga el operador * en Point para calcular en lo posible la distancia al cuadrado, con el fin de optimizar los calculos que solo requieren comparar y no guardar la distancia como tal.
  
- cp.cpp presenta el algoritmo Ciaccia Patella utilizando estructuras.
- ss.cpp presenta el algoritmo Sexton-Swingbank.
- En ambos archivos se indica los tamaños de B = 4096 / sizeof(Entry), con un Entries de 32bytes, consiguiendo un B de 128 y b de 64. Estas versiones de los programas fueros dejados para realizar tests por separado, con su propio main cada uno.
- Para tratar con los numeros aleatorios se utiliza la semilla time(NULL)

## Versiones finales:

Para la versión final, junto con los experimentos, se unificaron algoritmos y estructuras en un solo archivo (algoritmos.cpp) para evitar errores de importación, así como el desconocimiento del lenguaje y el uso de encabezados para proyectos que importan un mismo paquete en varias instancias. En este mismo archivo se definen las constantes B y b igual que en la forma anterior, utilizando la misma semilla para los números aleatorios. Adicionalmente, se crea el archivo main.cpp que contiene las funciones que ayudan a calcular lo solicitado en la tarea, como los intervalos de confianza. También se implementan 2 métodos que ayudan a la experimentación iterativa, generando registros de tiempos de ejecución de construcción y búsqueda, dejando un registro en archivos por cada potencia de 2 en un formato CSV para facilitar la lectura de estos. En el main se lleva a cabo este experimento, definiendo iterativamente un vector de puntos y queries con las estructuras anteriormente descritas, realizando ambos algoritmos de forma consecutiva por iteración.