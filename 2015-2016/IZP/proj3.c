/**
 *      subor:          proj3.c
 *      autor:          Milan Augustín, xaugus09@stud.fit.vutbr.cz
 *      projekt, rok:   IZP - Projekt 3 (Jednoducha shlukova analyza), 2015
 * 
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/slsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <errno.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

    // vypise ladici retezec
    #define debug(s) printf("- %s\n", s)

    // vypise formatovany ladici vystup - pouziti podobne jako printf
    #define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

    // vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
    #define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

    // vypise ladici informaci o promenne typu float - pouziti
    // dfloat(identifikator_promenne)
    #define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)
#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct objt_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
 */
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->obj = malloc(sizeof(struct obj_t) *cap);
    if (c->obj == NULL)
        c->capacity = 0;
    else c->capacity = cap;

    c->size = 0;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    c->obj = (free(c->obj), NULL);
    c->size = 0;
    c->capacity = 0;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */ 
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // Checking if is any free space available
    if (c->size >= c->capacity)
    {
        c = resize_cluster(c, (c->capacity == 0) ? CLUSTER_CHUNK : (c->capacity + c->capacity) );
        if (c->obj == NULL)
            return;
    }

    // Appending object 'obj' and expanding size of cluster 'c' by ++
    c->obj[c->size++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // Appending every object into 'c1' cluster from 'c2' cluster
    for (int i = 0; i < c2->size; i++)
    {
        append_cluster(c1, c2->obj[i]);
        if (c1->obj == NULL) return;
    }

    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    clear_cluster(&carr[idx]);
    // carr[idx] = carr[narr -1];  // More effective than next FOR
    // Dragging the hole at the end of array
    for (int i = idx; i < (narr -1); i++)
        carr[i] = carr[i +1];
    return --narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return sqrtf(powf( ((o1->x) - (o2->x)), 2) + powf( ((o1->y) - (o2->y)), 2) );
}

/*
 Pocita vzdalenost dvou shluku. Vzdalenost je vypoctena na zaklade nejblizsiho
 souseda.
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float min = INFINITY;
    float p;

    // Finding the minimal distance between clusters
    for (int i1 = 0; i1 < c1->size; i1++)
        for (int i2 = 0; i2 < c2->size; i2++)
        {
            p = obj_distance(&c1->obj[i1], &c2->obj[i2]);
            if (p < min) min = p;
        }

    return min;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky (podle nejblizsiho souseda). Nalezene shluky
 identifikuje jejich indexy v poli 'carr'. Funkce nalezene shluky (indexy do
 pole 'carr') uklada do pameti na adresu 'c1' resp. 'c2'.
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float min = INFINITY;
    float p;

    // Finding the nearest two clusters in array of clusters
    for (int i = 0; i < (narr -1); i++)
        for (int k = (i +1); k < narr; k++)
        {
            p = cluster_distance(&carr[i], &carr[k]);
            if (p < min)
            {
                min = p;
                *c1 = i;
                *c2 = k;
            }
        }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 */
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
 */
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    errno = 0;

    FILE *f;
    f = fopen(filename, "r");   // Opening file in reading mode
    if (f == NULL)
    {
        *arr = NULL;
        return -1;
    }

    char r[12];
    if (fscanf(f, "count=%11s\n", r) != 1)    // Reading first line of file
    {
        fclose(f);
        return -2;
    }

    char *endprt;
    long long int range;
    range = strtoll(r, &endprt, 10);
    if (*endprt != '\0' || range > INT_MAX || range < 0)
    {
        fclose(f);
        return -2;
    }

    *arr = malloc(sizeof(struct cluster_t) *range); // Alloccing memmory for clusters by times of 'range'
    if (*arr == NULL)
    {
        fclose(f);
        return -3;
    }

    struct cluster_t *var = *arr;

    char str[2049];
    int id, riadok = 0;
    float x, y;

    while ((fgets(str, 2048, f) != NULL) && (riadok < range))  // Loading line data into array of clusters
    {
        errno = 0;
        if (sscanf(str, "%d %f %f", &id, &x, &y) != 3) break;
        if (errno == ERANGE || (0 > x || x > 1000) || (0 > y || y > 1000)) break;

        struct obj_t object;
        object.id = id;
        object.x = x;
        object.y = y;

        init_cluster(&var[riadok], 1);
        append_cluster(&var[riadok], object);
        if (var[riadok].obj == NULL) break;

        riadok++;
    }

    if (riadok < range)
    {
        fclose(f);
        // Destroying all allocated memmory
        while (riadok > 0)
            riadok = remove_cluster(var, riadok, (riadok -1));
        free(var);
        return -4;
    }

    fclose(f);
    return riadok;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
 */
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    if (narr == 0)
    {
        printf("No one cluster is loaded.\n");
        return;
    }
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])    /* ||||||||||||* ZACIATOK *|||||||||||| */
{
    if (argc < 2)
    {
        fprintf(stderr, "Program sa spusta ako: %s SUBOR [N]\n"
            "  SUBOR je meno suboru so vstupnymi datmi.\n"
            "  N je volitelny argument definujuci cielovy pocet zhlukov. N > 0\n", argv[0]);
        return 1;
    }
    
    long long int n;
    if (argc == 2) n = 1;
    else
    {
        char *endprt;
        n = strtoll(argv[2], &endprt, 10);
        if (*endprt != '\0' || n > INT_MAX || n <= 0)   // Checking the format of INT
        {
            fprintf(stderr, "ERROR: Incorrect form of INTEGER in argument: '%s'\n", argv[2]);
            return 1;
        }
    }

    struct cluster_t *clusters;
    char *file = argv[1];
    int count = load_clusters(file, &clusters);

    switch (count) {    // Types of error in load_clusters
        case -1:
            fprintf(stderr, "ERROR: File openning problem.\n  ");
            perror("fopen");
            return 1;
        case -2:
            fprintf(stderr, "ERROR: Reading problem. Incorrect syntax of file.\n");
            return 1;
        case -3:
            perror("ERROR malloc");
            return 1;
        case -4:
            fprintf(stderr, "ERROR: 'Count' number of lines is not equal"
                            " to number of successfully loaded lines. "
                            "Please, check your file.\n");
            return 1;
    }

    int c1, c2;
    while (count > n)   // Connecting clusters
    {
        errno = 0;
        find_neighbours(clusters, count, &c1, &c2);
        merge_clusters(&clusters[c1], &clusters[c2]);
        if (errno != 0) break;  // Checking for errors
        count = remove_cluster(clusters, count, c2);
    }

    if (errno != 0)
    {
        perror("ERROR realloc");

        // Destroying rest of allocated memmory
        while (count > 0)
            count = remove_cluster(clusters, count, (count -1));
        free(clusters);

        return 1;
    }
    else print_clusters(clusters, count);

    // Destroying rest of allocated memmory
    while (count > 0)
        count = remove_cluster(clusters, count, (count -1));
    free(clusters);

    if (argc > 3)
    {
        fprintf(stderr, "ERROR: Too many arguments.\n");
        return 1;
    }

    return 0;
}   /* |||||||||||||||||||||||||||||* KONIEC *||||||||||||||||||||||||||||| */