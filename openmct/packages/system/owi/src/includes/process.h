/*
 #define PROCESS_COMMAND "ps aux | awk '{ print $1\";\"$2\";\"$3\";\"$4\";\"$5\";\"$6\";\"$7\";\"$8\";\"$9\";\"$10\";\"$11; }' | grep -v '^USER'"
*/

#define PROCESS_COMMAND "ps | awk '{ print $1\";\"$2\";\"$3\";\"$4\";\"$5; }' | grep -v PID"
#define PROCESS_SEPARATOR ";"

int process_main(struct owi_t *owi);
