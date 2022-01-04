#include <stdio.h>
#include <stdlib.h>

typedef struct diff {
    long offset; /* offset of the difference in file starting from zero*/
    unsigned char orig_value;     /* value of the byte in ORIG */
    unsigned char new_value;     /* value of the byte in NEW */
} diff;

typedef struct node node;

struct node {
    diff *diff_data; /* pointer to a struct containing the offset and the value of the bytes in each of the files*/
    node *next;
};

void list_print(node *diff_list, FILE* output); 
     /* Print the nodes in diff_list in the following format: byte POSITION ORIG_VALUE NEW_VALUE. 
Each item followed by a newline character. */
 
node* list_append(node* diff_list, diff* data); 
     /* Add a new node with the given data to the list,
        and return a pointer to the list (i.e., the first node in the list).
        If the list is null - create a new entry and return a pointer to the entry.*/

void list_free(node *diff_list); /* Free the memory allocated by and for the list. */

int main(int argc, char **argv) {
    FILE * input = stdin;
    FILE * output = stdout;
    node *list = NULL;
    
    diff *data1 = malloc(sizeof(diff));
    data1->offset = 0;
    data1->orig_value = 'a';
    data1->new_value = 'b';
    
    diff *data2 = malloc(sizeof(diff));
    data2->offset = 1;
    data2->orig_value = 'c';
    data2->new_value = 'd';
    
    list = list_append(list, data1);
    list = list_append(list, data2);
    list_print(list, output);
    list_free(list);
    
    return 0;
}

void list_print(node *diff_list, FILE* output) {
	if(diff_list != NULL) {
		fprintf(output, "byte %ld %02X %02X\n", diff_list->diff_data->offset, diff_list->diff_data->orig_value, diff_list->diff_data->new_value);
		list_print(diff_list->next, output);
	}
}
 
node* list_append(node* diff_list, diff* data) {
	if(diff_list == NULL) {
		node* new_node = (node*)calloc(1, sizeof(node));
		new_node->diff_data = data;
		diff_list = new_node;
	}
	else {
		diff_list->next = list_append(diff_list->next, data);
	}
	return diff_list;
}

void list_free(node *diff_list) {
	if(diff_list != NULL) {
		free(diff_list->diff_data);
		list_free(diff_list->next);
	}
	free(diff_list);
}
