int open(char *name, int flags);
int close(int fd);
int read(int fd, char *buf, int size);
int write(int fd, char *buf, int size);
int strlen(char *s);

void utoa_s(int i, char *s);
int atou_s(char *s);

/* print a string on stdout */
void print(char *s);

int main(int argc, char **argv) {
	char printing_buffer[16];
	utoa_s(123, printing_buffer);
	print(printing_buffer);
	print("\n");
	int num = atou_s("5678");
	utoa_s(num, printing_buffer);
	print(printing_buffer);
	print("\n");
}

void print(char *s) {
	write(1, s, strlen(s));
}
