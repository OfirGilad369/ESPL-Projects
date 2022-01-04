#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void total_differences(node* diff_list, FILE * output);
void first_n_differences(node* diff_list, FILE * output, long int n);
void restore_n_differences(node* diff_list, FILE * new_file, long int n);

int main(int argc, char **argv) {
    FILE * file1 = NULL;
    FILE * file2 = NULL;
    FILE * output = stdout;
    node *diff_list = NULL;
    int option_number = 0;
    long int k_option_n = 0;
    long int r_option_n = -1;
    char * file_name;
    
    for(int i = 1; i < argc; i++) {
		if(strcmp(argv[i],"-o")==0) {
			output = fopen(argv[i + 1], "w");
			i = i + 1;
		}
		else if(strcmp(argv[i],"-t")==0) {
			option_number = 1;
		}
		else if(strcmp(argv[i],"-k")==0) {
			option_number = 2;
			k_option_n = atoi(argv[i + 1]);
			i = i + 1;
		}
		else if(strcmp(argv[i],"-r")==0) {
			option_number = 3;
			if (i != argc - 1) {
				r_option_n = atoi(argv[i + 1]);
			}
			i = i + 1;
		}
		else {
			if(file1 == NULL) 
			{
				file1 = fopen(argv[i], "rb");
			}
			else {
				file2 = fopen(argv[i], "rb");
				file_name = argv[i];
			}
		}
	}
    
    fseek(file1, 0L, SEEK_END);
    long int input_size1 = ftell(file1);
    rewind(file1);
    
    fseek(file2, 0L, SEEK_END);
    long int input_size2 = ftell(file2);
    rewind(file2);
    
    long int size_of_shortest_file = (input_size1 < input_size2) ? input_size1 : input_size2;
    for(long int i = 0; i < size_of_shortest_file; i++) {
		unsigned char orig_value[1];
		unsigned char new_value[1];
		fread(orig_value, 1, 1, file1);
		fread(new_value, 1, 1, file2);
		if(orig_value[0] != new_value[0]) {
			diff *data = malloc(sizeof(diff));
			data->offset = i;
			data->orig_value = orig_value[0];
			data->new_value = new_value[0];
			diff_list = list_append(diff_list, data);
		}
	}
	
	//No option selected
	if(option_number == 0) {
		list_print(diff_list, output);
	}
	//Option -t selected
	else if(option_number == 1) {
		total_differences(diff_list, output);
	}
	//Option -k selected
	else if(option_number == 2) {
		first_n_differences(diff_list, output, k_option_n);
	}
	//Option -r selected
	else if(option_number == 3) {
		fclose(file2);
		if(r_option_n != -1) {
			file2 = fopen(file_name, "r+b");
			rewind(file2);
			restore_n_differences(diff_list, file2, r_option_n);
		}
		else {
			file2 = fopen(file_name, "wb");
			rewind(file1);
			rewind(file2);
			for(long int i = 0; i < input_size1; i++) {
				unsigned char orig_value[1];
				fread(orig_value, 1, 1, file1);
				fwrite(orig_value, 1, 1, file2);
			}
		}
	}
	
    list_free(diff_list);
    fclose(file1);
    fclose(file2);
	fclose(output);
    
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

void total_differences(node* diff_list, FILE * output) {
	long int count = 0L;
	node* next_node = diff_list;
	while(next_node != NULL) {
		count++;
		next_node = next_node->next;
	}
	fprintf(output, "%ld\n", count);
}

void first_n_differences(node* diff_list, FILE * output, long int n) {
	if(diff_list != NULL && n != 0L) {
		fprintf(output, "byte %ld %02X %02X\n", diff_list->diff_data->offset, diff_list->diff_data->orig_value, diff_list->diff_data->new_value);
		first_n_differences(diff_list->next, output, n - 1);
	}
}

void restore_n_differences(node* diff_list, FILE * new_file, long int n) {
	if(diff_list != NULL && n != 0L) {
		fseek(new_file, diff_list->diff_data->offset, SEEK_SET);
		unsigned char orig_value[1];
		orig_value[0] = diff_list->diff_data->orig_value;
		fwrite(orig_value, 1, 1, new_file);
		rewind(new_file);
		restore_n_differences(diff_list->next, new_file, n - 1);
	}
}
