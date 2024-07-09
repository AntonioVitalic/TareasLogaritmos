package bloom;
import java.util.ArrayList;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Bloom {
    // definimos el arreglo M de m bits inicializado en 0,
    // junto a las k funciones de hash distintas h1, ... hk
    private int[] M;  // Arreglo M de m bits
    private List<HashFunction> hashFunctions;
    
    // Constructor de la clase Bloom
    public Bloom(int m, int k) {
        M = new int[m];
        hashFunctions = new ArrayList<>(k);
        for (int i = 0; i < k; i++) {
            hashFunctions.add(new HashFunction(i));
        }
    }

    // definimos método para insertar() y busqueda() ?????

    // Cuando se ***inserta*** un nuevo elemento a nuestro universo U, se aplica cada una de las k funciones de hash, lo que
    // devolverá distintas posiciones j en el arreglo M. Luego modificamos M[j] = 1, para cada j retornado
    // por cada función de hash.
    public void insertar(String element) {
        for (Tipo hash : Hashes) {
            M[hash.hash(element)] = 1;
        }
    }

    // Luego, cuando ***buscamos*** un elemento y, aplicamos el filtro de Bloom: calculamos hi(y), con i ∈ [1, k].
    // Si cada una de las funciones de hash retorna 1, entonces buscamos el elemento en la base de datos.
    // Si al menos una de las funciones de hash retorna 0, entonces podemos decir con seguridad que el
    // elemento no se encuentra en la base de datos.
    public boolean buscar(String element) {
        for (Tipo hash : Hashes) {
            if (M[hash.hash(element)] == 0) {
                return false;
            }
        }
        return true;
    }

    private int hash(String element, int seed) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update((byte) seed);
            byte[] bytes = md.digest(element.getBytes(StandardCharsets.UTF_8));
            int hash = 0;
            for (byte b : bytes) {
                hash = (hash << 8) | (b & 0xFF);
            }
            return hash;
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("SHA-256 algorithm not found");
        }
    }
}
