#ifndef NODE_H
#define NODE_H

// Use this to declare a node of your own data type.
#define DEFINE_NODE(T) typedef struct node_##T##_t node_##T##_t; struct node_##T##_t { T val; node_##T##_t *next; };

#endif // NODE_H
