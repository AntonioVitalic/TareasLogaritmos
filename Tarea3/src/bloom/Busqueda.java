package bloom;

import java.util.ArrayList;

/*
 * Clase que realiza la búsqueda de palabras en un diccionario
 * @param dict: diccionario de palabras
 */
public class Busqueda {
    private ArrayList<String> dict; // Diccionario de palabras para buscar

    public Busqueda(ArrayList<String> dict){
        this.dict = dict;
    }

    /*
     * Método para realizar una búsqueda secuencial de una palabra en el diccionario
     * @param key: palabra a buscar
     */
    public void secuencial(String key){
        for (String word : dict) {
            if (word.equals(key)) {
                return;
            }
        }
    }

    /*
     * Método para realizar una búsqueda de una palabra en el diccionario utilizando un filtro de Bloom
     * @param bf: filtro de Bloom
     * @param key: palabra a buscar
     */
    public void bloom(BloomFilter bf, String key){
        if (bf.contains(key)) {
            secuencial(key);
        }
    }

    /*
     * Método para calcular la tasa de falsos positivos de un filtro de Bloom
     * @param bf: filtro de Bloom
     * @param fpDict: diccionario de palabras que no están en el diccionario
     * @return tasa de falsos positivos
     */
    public float falsosPositivos(BloomFilter bf, ArrayList<String> fpDict){
        int fp = 0;
        for (String word : dict) {
            if (bf.contains(word)) {
                fp++;
            }
        }
        return (float) fp / (float) fpDict.size();
    }
}
