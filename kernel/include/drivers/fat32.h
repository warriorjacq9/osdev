#include <stdbool.h>

/* structure used by the functions of this file. */
struct fat32_driver;

/* structure used to represent a folder. */
struct fat32_node;

/* A linked list of nodes. */
struct fat32_node_list {
    struct fat32_node *node; 
    struct fat32_node_list *next; /* The next link (or NULL) */
};
void fat32_node_list_free(struct fat32_node_list *list);

struct fat32_node* copy_node(const struct fat32_node *node);

/* Create a new structure "fat32_driver" from the name of a
 * file containing a FAT32 image. */
struct fat32_driver* fat32_driver_new(const char *image_name);

/* Releases the memory used by a "fat32_driver" structure. */
void fat32_driver_free(struct fat32_driver *driver);

/* Returns the fat32_node corresponding to the root folder of the disk image. */
struct fat32_node* fat32_driver_get_root_dir(const struct fat32_driver *driver);

const char* fat32_node_get_name(const struct fat32_node *node);
bool fat32_node_is_dir(const struct fat32_node *node);

/* If node 'node' is a folder, return a list of fat32_node,
 * which correspond to the child nodes of it. */
struct fat32_node_list* fat32_node_get_children(const struct fat32_node *node);

/* Get the node corresponding to a relative path to another node.
 * Returns NULL if no node matches. */
struct fat32_node* fat32_node_get_path(const struct fat32_node *node, const char *path);

/* Read the contents of a file and write it to a file descriptor (eg stdout) */
void fat32_node_read_to_fd(const struct fat32_node *node, FILE *fd);

/* Releases the memory used by a "fat32_node" structure. */
void fat32_node_free(struct fat32_node *dir);