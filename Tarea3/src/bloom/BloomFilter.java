package bloom;

import java.nio.charset.Charset;

import com.google.common.hash.HashFunction;
import com.google.common.hash.Hashing;

/*
 * Clase que implementa un filtro de Bloom
 * @param m: cantidad de bits
 * @param k: cantidad de funciones hash
 */
public class BloomFilter {
    private int m; // Cantidad de bits
    private int k; // Cantidad de funciones hash
    private int[] M; // Arreglo M de bits
    private HashFunction[] hfs; // Funciones hash

    public BloomFilter(int m, int k) {
        this.m = m;
        this.k = k;
        this.M = new int[m];
        this.hfs = new HashFunction[k];
        for (int i = 0; i < k; i++) {
            hfs[i] = Hashing.murmur3_128(i);
        }
    }

    /*
     * Método para insertar una llave en el filtro de Bloom
     * @param key: llave a insertar
     */
    public void insert(String key) {
        for (int i = 0; i < k; i++) {
            int hash = hfs[i].hashString(key, Charset.defaultCharset()).asInt();
            int pos = Math.abs(hash) % m;
            M[pos] = 1;
        }
    }

    /*
     * Método para verificar si una llave está en el filtro de Bloom
     * @param key: llave a buscar
     * @return true si la llave está en el filtro, false en caso contrario
     */
    public boolean contains(String key) {
        for (int i = 0; i < k; i++) {
            int hash = hfs[i].hashString(key, Charset.defaultCharset()).asInt();
            int pos = Math.abs(hash) % m;
            if (M[pos] == 0) {
                return false;
            }
        }
        return true;
    }
}
