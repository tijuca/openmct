#define PROCESS_COMMAND "ps aux | awk '{ print $1\";\"$2\";\"$3\";\"$4\";\"$5\";\"$6\";\"$7\";\"$8\";\"$9\";\"$10\";\"$11; }'"
#define PROCESS_SEPARATOR ";"

int process_main(int argc, char **);
void process_list(struct file_t *f);