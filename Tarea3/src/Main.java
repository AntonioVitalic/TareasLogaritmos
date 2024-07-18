import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Locale;
import java.util.Random;
import java.util.Scanner;

import bloom.BloomFilter;
import bloom.Busqueda;

/*
 * Clase principal que realiza experimentos para comparar la búsqueda secuencial con la búsqueda utilizando un filtro de Bloom
 */
public class Main {

    private static int seed = 123456789; // Seed para Random

    /*
     * Método para obtener el valor de m
     * @param n: cantidad de elementos
     * @param err: error
     */
    private static int get_m(int n, float err){
        return - (int) (n * Math.log(err) / Math.pow(Math.log(2), 2));
    }

    /*
     * Método para obtener el valor de k
     * @param m: cantidad de bits
     * @param n: cantidad de elementos
     */
    private static int get_k(int m, int n){
        return (int) (m / n * Math.log(2));
    }

    /*
     * Método para leer un archivo CSV (de una columna) y retornar una lista con los elementos
     * @param file: archivo a leer
     */
    private static ArrayList<String> readCSV(File file){
        ArrayList<String> list = new ArrayList<>();
        try {
            Scanner scanner = new Scanner(file);
            while (scanner.hasNextLine()) {
                list.add(scanner.nextLine());
            }
            scanner.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return list;
    
    }

    /*
     * Método para generar una muestra de n elementos con proporción p de nombres de películas
     * @param bName: lista de nombres de bebés
     * @param fName: lista de nombres de películas
     * @param n: cantidad de elementos
     * @param p: proporción de nombres de películas
     */
    private static ArrayList<String> sample(ArrayList<String> bName, ArrayList<String> fName, int n, float p) {
        int bSize = (int) (n * p);
        int fSize = n - bSize;

        // Verificación de límites
        if (bSize > bName.size()) {
            throw new IllegalArgumentException("No hay suficientes nombres de bebés para crear la muestra.");
        }
        if (fSize > fName.size()) {
            throw new IllegalArgumentException("No hay suficientes nombres de películas para crear la muestra.");
        }
        ArrayList<String> smpl = new ArrayList<>(n);

        // Copias para randomizar
        ArrayList<String> bCpy = new ArrayList<>(bName);
        ArrayList<String> fCpy = new ArrayList<>(fName);

        // Randomizar
        Collections.shuffle(bCpy, new Random(seed));
        Collections.shuffle(fCpy, new Random(seed));

        // Agregar al sample
        for (int i = 0; i < bSize; i++) {
            smpl.add(bCpy.get(i));
        }
        for (int i = 0; i < fSize; i++) {
            smpl.add(fCpy.get(i));
        }

        // Randomizar
        Collections.shuffle(smpl, new Random(seed));
        return smpl;
    }

    /*
     * Método para escribir los resultados en un archivo CSV
     * @param file: archivo a escribir
     * @param timeSeq: tiempo de búsqueda secuencial
     * @param timeBloom: tiempo de búsqueda con filtro de Bloom
     * @param fpRate: tasa de falsos positivos
     */
    private static void writeResults(File file, double timeSeq, double timeBloom, double fpRate){
        try (PrintWriter writer = new PrintWriter(new FileWriter(file, true))) {
            writer.printf(Locale.US, "%f,%f,%f\n", timeSeq, timeBloom, fpRate);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /*
     * Método para realizar un experimento
     * @param busqueda: objeto de búsqueda
     * @param bf: filtro de Bloom
     * @param sample: muestra de nombres
     * @param dict: diccionario de nombres
     * @param file: archivo para escribir los resultados
     */
    private static void experiment(Busqueda busqueda, BloomFilter bf, ArrayList<String> sample, ArrayList<String> dict, File file){
        double timeSeq = 0;
        double timeBloom = 0;
        double fpRate = 0;

        for (String key : sample) {
            long start = System.nanoTime();
            busqueda.secuencial(key);
            long end = System.nanoTime();
            timeSeq += (end - start) / 1e9;

            start = System.nanoTime();
            busqueda.bloom(bf, key);
            end = System.nanoTime();
            timeBloom += (end - start) / 1e9;
        }

        fpRate = busqueda.falsosPositivos(bf, dict);

        writeResults(file, timeSeq, timeBloom, fpRate);
    }

    /*
     * Método principal
     */
    public static void main(String[] args) throws Exception {
        File fNames = new File("./data/Film-Names.csv");
        File bNames = new File("./data/Popular-Baby-Names-Final.csv");

        ArrayList<String> films = readCSV(fNames);
        ArrayList<String> names = readCSV(bNames);

        // Print sizes
        System.out.println("Names: " + names.size());
        System.out.println("Films: " + films.size());

        int[] N = {1024, 4096, 16384, 65536};
        float[] p = {0f, 0.25f, 0.5f, 0.75f, 1.0f};

        // Error
        float err = 0.01f;

        Busqueda busqueda = new Busqueda(names);

        for (int n : N) {

            File fout = new File("results/" + n + "/" + err + ".csv");

            for (float prop : p) {
                // Asignacion de m y k segun n y err
                int m = get_m(n, err);
                int k = get_k(m, n);

                // Generacion de muestra
                ArrayList<String> sample = sample(names, films, n, prop);

                // Creacion del filtro y llenado
                BloomFilter bf = new BloomFilter(m, k);
                for (String name : names) {
                    bf.insert(name);
                }

                // Experimento
                experiment(busqueda, bf, sample, names, fout);
            }
        }
    }
}
