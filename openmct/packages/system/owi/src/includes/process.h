#define PROCESS_FILE "/tmp/process.tmp"

#define PROCESS_SEPARATOR ";"

#define PROCESS_COMMAND "ps | awk '{ if ($1 != \"PID\") print \
                         substr($0,0,5)  \"" PROCESS_SEPARATOR "\" \
			 substr($0,7,8)  \"" PROCESS_SEPARATOR "\" \
			 substr($0,16,6) \"" PROCESS_SEPARATOR "\" \
			 substr($0,23,4) \"" PROCESS_SEPARATOR "\" \
			 substr($0,27) }' >" PROCESS_FILE

int process_main(struct owi_t *owi);
