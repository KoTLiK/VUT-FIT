/**
 * @file   proj3.h
 * @author Milan Augustin (xaugus09@stud.fit.vutbr.cz)
 * @date   December, 2015
 * @brief  Documentation of simple cluster analysis
 */

/**
 * @addtogroup Sctructures
 * @{
 */

/**
 * @brief    Structure of object
 * 
 * @param 	id 	Identification number
 * @param 	x 	Coordinate X
 * @param 	y 	Coordinate Y
 */
struct obj_t {
    int id;
    float x;
    float y;
};

/**
 * @brief    Structure of cluster
 * 
 * @param 	size 		Current size of cluster
 * @param 	capacity 	Current capacity of cluster
 * @param 	obj 		Pointer at the first object in cluster
 */
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};
/** @} */

/**
 * @addtogroup Cluster_functions
 * @{
 */

/**
 * @brief    Initializes cluster[c] and allocates memory for capacity[cap]
 * @details  This function initializes cluster[c] with allocated memory (capacity[cap]).
 * 	Pointer NULL means, cluster capacity is equaled zero and allocation failed.
 * 
 * @param 		c 		Cluster address
 * @param[in] 	cap 	Value of capacity
 * 
 * @pre      Pointer 'c' is not pointed to NULL
 * @pre      Value of capacity is not negative
 * 
 * @post     Inicialization was successful, cluster is initialized with allocated memory 
 * @post     Else cluster is initialized to empty cluster
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief    Deletes cluster[c] and inicializes to an empty cluster
 * @details  This function deletes all cluster objects and inicializes to an empty cluster.
 * 
 * @param 	c 	Cluster address
 * 
 * @post     Cluster is inicialized to empty cluster
 */
void clear_cluster(struct cluster_t *c);

/// Chunk of cluster objects. Value recommended for reallocation.
extern const int CLUSTER_CHUNK;

/**
 * @brief    Changes the capacity of cluster[c] to new capacity[new_cap]
 * 
 * @param 		c 			Cluster address
 * @param[in] 	new_cap 	Value of new capacity
 * 
 * @pre      Pointer 'c' is not pointed to NULL
 * @pre      Cluster has inicialized to zero or more
 * @pre      Value of new capacity is not negative
 * 
 * @return   The new address, otherwise NULL is returned
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief    Appends the object[obj] at the end of a cluster[c]
 * @details  This function appends the object[obj] at the end of a cluster[c].
 * 	If is not enough capacity for appending, cluster is resized.
 * 
 * @param 		c 		Cluster address
 * @param[in] 	obj 	An object, which will be appended
 * 
 * @post     If it is successful, object is appended, otherwise nothing happened
 */
void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief    Adds all objects from cluster[c2] to cluster[c1]
 * @details  This function adds all objects from cluster[c2] to cluster[c1].
 * 	If is not enough capacity, cluster is resized.
 * 	At the end, cluster[c1] is sorted and cluster[c2] is untouched.
 * 
 * @param 	c1 	Destination, where will be added objects
 * @param 	c2 	Source of objects for copying
 * 
 * @pre      Addresses of both clusters are not pointed to NULL
 * 
 * @post     Cluster[c1] is sorted
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief    Removes cluster[idx] from array[narr] of clusters[carr]
 * @details  This function removes cluster[idx] from array of clusters[carr].
 * 	Array of clusters consist of [narr] clusters.
 * 	Function returns the new number of clusters in array.
 * 
 * @param 		carr 	Pointer at first cluster in array
 * @param[in] 	narr 	Size of array of clusters
 * @param[in] 	idx 	Cluster index, which will be deleted
 * 
 * @return   Reduced size of array
 */
int remove_cluster(struct cluster_t *carr, int narr, int idx);
/** @} */

/**
 * @addtogroup Object_functions
 * @{
 */

/**
 * @brief    Calculates Euclidean distance between two objects
 * 
 * @param[in] 	o1 	Object address
 * @param[in] 	o2 	Object address
 * 
 * @pre      Clusters are not empty
 * 
 * @return   Value of distance
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief    Calculates distance between two clusters
 * @details  This function calculates distance between two clusters.
 * 	Distance of two clusters depends at the minimal distance between their objects.
 * 
 * @param[in] 	o1 	Cluster address
 * @param[in] 	o2 	Cluster address
 * 
 * @pre      Clusters are not empty
 * @pre      Clusters sizes are more then zero
 * 
 * @return   Value of distance
 */
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief    Finds the nearest neighbours in array[narr] of clusters[carr]
 * @details  This function finds the nearest clusters.
 * 	In array[narr] of clusters[carr] finds two the nearest clusters (by the nearest neighbours).
 * 	Found clusters identifies and their indexes are saved in [c1, c2].
 * 
 * @param 		carr 	Pointer at first cluster in array
 * @param[in] 	narr 	Size of array
 * @param[out] 	c1 		Address one of the neighbours
 * @param[out] 	c2 		Address another one of the neighbours
 * 
 * @pre      Size of array is more than zero
 * 
 * @post     Indexes of the nearest neighbours are saved in variables 'c1' and 'c2'
 */
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief    Sorts objects in the cluster by their IDs
 * 
 * @param 	c 	Cluster address
 * 
 * @post     Objects in cluster are sorted by their IDs
 */
void sort_cluster(struct cluster_t *c);

/**
 * @brief    Prints cluster[c] on the standard output
 * 
 * @param 	c 	Cluster address
 */
void print_cluster(struct cluster_t *c);

/**
 * @brief    Loads the objects from the file and every object recieve one cluster
 * @details  This function loads the objects from the file[filename].
 * 	For every object creates one cluster and
 * 	these clusters are saved in allocated memory for array of clusters[*arr].
 * 	Function returns the number of loaded objects.
 * 	When error occurs, the pointer of array of clusters is pointed at NULL.
 * 
 * @param[in] 	filename 	File stream
 * @param 		arr 		Array of clusters address
 * 
 * @return   The number of loaded objects
 * @return   When error occurs, negative value is returned
 * 				[-1: File opening problem], [-2: Problems with syntax in file], [-3: Malloc error], [-4: Problem with loading objects]
 */
int load_clusters(char *filename, struct cluster_t **arr);

/**
 * @brief    Prints clusters on the standard output
 * @details  This function prints clusters on the standard output,
 * 	where [carr] is pointer at the first cluster
 * 	and function prints first [narr] clusters.
 * 
 * @param 		carr 	Array of clusters address
 * @param[in] 	narr 	size of array
 */
void print_clusters(struct cluster_t *carr, int narr);
/** @} */