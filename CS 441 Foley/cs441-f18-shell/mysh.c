/*
 * TEAM : 亀
 *　Wanqian Huang, Ian E. Neville
 * CS441/541: Project 3
 *
 */
#include "mysh.h"
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
//BATCH MODE FILE COUNT
int file_cnt;
int total_builts;

/*
 * Main method execute this project.
 * It's about shell. It can execute binary file
 * and some built-in methods(wait, fg, exit, jobs, history)
 *
 */
int main(int argc, char * argv[]) {
    int ret;
    if (0 != (ret = parse_args_main(argc, argv))) {
        fprintf(stderr, "Error: Invalid command line!\n");
        return -1;
    }
    if (TRUE == is_batch) {
        if (TRUE == is_debug) {
            fprintf(stdout,"Batch Mode!\n");
        }
        if (0 != (ret = batch_mode())) {
            fprintf(stderr, "Error: Batch mode returned a failure!\n");
        }
    } else if (FALSE == is_batch) {
        if (TRUE == is_debug) {
            fprintf(stdout,"Interactive Mode!\n");
        }
        if (0 != (ret = interactive_mode())) {
            fprintf(stderr, "Error: Interactive mode returned a failure!\n");
        }
    } else {
        fprintf(stderr, "Error: Unknown execution mode!\n");
        return -1;
    }
    return 0;
}

/*
 * It can parse arguments to get filename
 *
 */
int parse_args_main(int argc, char ** argv) {
    int i;
    if (argc > 1) {
        is_batch = TRUE;
        file_cnt = argc - 1;
        file_names = malloc(file_cnt * sizeof(char * ));
        for (i = 0; i < file_cnt; i++) {
            file_names[i] = malloc(sizeof(char) * strlen(argv[i + 1]));
            strcpy(file_names[i], argv[i + 1]);
        }
    } else if (argc > 1) {
        is_batch = FALSE;
    } else {
        return 0;
    }
    return 0;
}

/*
 * It can split strings based on input delim
 * and return split array.
 *
 */
char ** split_str(char * cmdgen, char * delim) {
    char * cmd = strdup(cmdgen);
    int len = MAX_COMMAND_LINE;
    char ** cmds = malloc(sizeof(char * ) * len);
    char * token = strtok(cmd, delim);
    int i = 0;
    while (token) {
        cmds[i++] = token;
        token = strtok(NULL, delim);
    }
    return cmds;
}

/*
 * Free string arrays.
 */
void free_stuff(char ** mem){
    while(*mem){
        free(*mem);
        mem = mem + 1;
    }
    free(mem);
}


/*
 * Free job address.
 */
void free_job(struct job_t * job){
    if(job){
        free(job->full_command);
        free(job->binary);
        free(job->path);
        free(job->bin);
    }
}
/*
 * User choose to use batcho mode.
 */
int batch_mode() {
    total_builts = 0;
    FILE * fp;
    char * buffer;
    size_t len = 0;
    ssize_t nread;
    int i, j;
    char ** cmds;
    int start_num, end_num;
    for (i = 0; i < file_cnt; i++) {
        fp = fopen(file_names[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "The file doesn't exist!\n");
        }
        while ((nread = getline( & buffer, & len, fp)) != EOF) {
            buffer[strlen(buffer) - 1] = '\0';
            cmds = split_str(buffer, ";");
                i = 0;
            //only need to loop until cmds[i] == NULL
            start_num = total_history;
            while (cmds[i]) {
                add_job(cmds[i]);
                i++;
            }
            end_num = total_history;
            for (j = start_num; j < end_num; j++) {
                exe( & job_list[j]);
            }
        }
        fclose(fp);
        fp = NULL;
    }
    builtin_exit();
    free_stuff(cmds);
    return 0;
}

/*
 * User choose to use inter
 */
int interactive_mode() {
    total_builts = 0;
    const int BUF_SIZE = 1000;
    char ** cmds;
    int i, j, start_num, end_num;
    char buffer[BUF_SIZE];
    char *fgets_ret;
    setbuf(stdout, NULL);
    fprintf(stdout, PROMPT);
    fgets_ret = fgets(buffer, BUF_SIZE, stdin);
    while (fgets_ret) {
        buffer[strlen(buffer) - 1] = '\0';
        cmds = split_str(buffer, ";");
        i = 0;
        //only need to loop until cmds[i] == NULL
        start_num = total_history;
        while (cmds[i]) {
            add_job(cmds[i]);
            i++;
        }
        end_num = total_history;
        for (j = start_num; j < end_num; j++) {
            exe( & job_list[j]);
        }
        fprintf(stdout, PROMPT);
        fgets_ret = fgets(buffer, BUF_SIZE, stdin);
    }
    builtin_exit();
    free_stuff(cmds);
    return 0;
}

/*
 *  Return argument length.
 */
int arg_len(char ** arg) {
    int i = 0;
    while (arg[i]) {
        i++;
    }
    return i;
}

/*
 * Remove all empty space.
 *
 */
char * trim_white(char * str) {
    int i;
    char * str2 = strdup(str);
    if (str[strlen(str) - 1] == ' ') {
        str2[strlen(str) - 1] = '\0';
    }
    if (str[0] == ' ') {
        for (i = 1; i < strlen(str2); i++) {
            str2[i - 1] = str[i];
        }
        str2[i - 1] = '\0';
    }
    return str2;
}

/*
 * Split one line commands to one or multiple jobs
 * Then create a job with all properties
 * Finally add job to job_list
 */
void add_job(char * cmd) {
    char ** split_cmd;
    char * full;
    int cmd_len;
    int i;
    char * c;
    char ** c_splited;
    // Split input by &
    char ** seqs = split_str(cmd, "&");
    if (strlen(seqs[0]) != strlen(cmd)) {
        //sequentials found! add them to the job_list!
        i = 0;
        while (seqs[i]) {
            split_cmd = split_str(seqs[i], " ");
            job_list[total_history].binary = split_cmd[0];
            job_list[total_history].argv = split_cmd;
            job_list[total_history].argc = arg_len(split_cmd);
            full = trim_white(seqs[i]);
            job_list[total_history].full_command = full;
            job_list[total_history].is_background = TRUE;
            job_list[total_history].done = 0;
            i++;
            job_list[total_history].seen_done = 0;
            // Check redirection.
            if (strchr(job_list[total_history].full_command, '<')) {
                c = strdup(full);
                c_splited = split_str(c, "< ");
                job_list[total_history].bin = strdup(c_splited[0]);
                job_list[total_history].path = strdup(c_splited[1]);
                job_list[total_history].redirection = 1;
            } else if (strchr(job_list[total_history].full_command, '>')) {
                c = strdup(full);
                c_splited = split_str(c, "> ");
                job_list[total_history].bin = strdup(c_splited[0]);
                job_list[total_history].path = strdup(c_splited[1]);
                job_list[total_history].redirection = 2;
            }
            // One job is added.
            total_history++;
        }
        // Check a process is a background process or foreground process.
        if (cmd[strlen(cmd) - 1] == '&') {
            job_list[total_history - 1].is_background = TRUE;
        } else {
            job_list[total_history - 1].is_background = FALSE;
        }
        return;
    }
    //add null to cmd length to store it in the fullcommand attribute
    cmd_len = strlen(cmd) + 1;
    job_list[total_history].full_command = malloc(sizeof(char) * cmd_len);
    split_cmd = split_str(cmd, " ");
    job_list[total_history].binary = split_cmd[0];
    job_list[total_history].argv = split_cmd;
    job_list[total_history].argc = arg_len(split_cmd);
    full = trim_white(cmd);
    job_list[total_history].full_command = full;
    job_list[total_history].is_background = FALSE;
    job_list[total_history].done = 0;
    job_list[total_history].seen_done = 0;
    // Create a job for an redirection.
    if (strchr(job_list[total_history].full_command, '<')) {
        c = strdup(full);
        c_splited = split_str(c, "< ");
        job_list[total_history].bin = strdup(c_splited[0]);
        job_list[total_history].path = strdup(c_splited[1]);
        job_list[total_history].redirection = 1;
    } else if (strchr(job_list[total_history].full_command, '>')) {
        c = strdup(full);
        c_splited = split_str(c, "> ");
        job_list[total_history].bin = strdup(c_splited[0]);
        job_list[total_history].path = strdup(c_splited[1]);
        job_list[total_history].redirection = 2;
    }
    total_history++;
}

/*
 * Update the done status of job with p_pid
 * Update a background job when it's done.
 * and change a background to foreground when fg is 1.
 */
void update_status(int p_pid, int fg) {
    int i;
    for (i = 0; i < total_history; i++) {
        if (job_list[i].pid == p_pid) {
            job_list[i].done = 1;
            if (fg) {
                job_list[i].is_background = 0;
                total_jobs_bg--;
            }
        }
    }
}

/*
 * It's for background process forking.
 */
void sigchld_handler(int sig) {
    int status;
    assert(sig == SIGCHLD);
    pid_t child_pid;
    //check every child for sucessful state change:
    //waitpid returns that child's pid: update the job with corresponding pid
    while ((child_pid = waitpid(-1, & status, WNOHANG)) > 0) {
        update_status(child_pid, 0);
    }
}

/*
 * Exe a job in job_list.
 *
 */
int exe(struct job_t * job) {
    int status = 0;
    pid_t child = 0;
    int p_pid;
    int file;
    char * binary;
    char ** split_cmd;
    if(!job->binary){
      // Execute empty job.
        fprintf(stdout,"Unexecutable job\n");
        return 0;
    }
    if (!strcmp(job->binary, "jobs")) {
      // Execute built in method jobs.
        total_builts++;
        builtin_jobs();
    } else if (!strcmp(job->binary, "history")) {
      // Execute built in method history.
        total_builts++;
        builtin_history();
    } else if (!strcmp(job->binary, "wait")) {
      // Execute built in method wait.
        total_builts++;
        builtin_wait();
    } else if (!strcmp(job->binary, "exit")) {
      // Execute built in method exit.
        total_builts++;
        builtin_exit();
    } else if (!strcmp(job->binary, "fg")) {
      // Execute built in method fg.
        total_builts++;
        if (job->argc > 1) {
            builtin_fg_aug(atoi(job->argv[1]));
        } else {
            builtin_fg();
        }
    } else {
      // Forking.
        child = fork();
        if (child == 0) {
            job->pid = getpid();
            if (job->redirection == 1) {
                binary = strdup(job->bin);
                split_cmd = split_str(binary, " ");
                file = open(job->path, O_RDONLY);
                dup2(file, STDIN_FILENO);
                close(file);
                execvp(split_cmd[0], split_cmd);
            } else if (job->redirection == 2) {
                binary = strdup(job->bin);
                split_cmd = split_str(binary, " ");
                file = open(job->path, O_CREAT | O_TRUNC | O_WRONLY);
                dup2(file, STDOUT_FILENO);
                close(file);
                execvp(split_cmd[0], split_cmd);
            } else {
                execvp(job->binary, job->argv);
            }
            fprintf(stderr, "Command does not exist\n");
            /* If execvp returns, it must have failed. */
            exit(1);
        } else if (child < 0) {
            fprintf(stdout,"Error: fork() failed!!!\n");
            return -1;
        } else {
            job->pid = child;
            //if foreground then wait
            if (!job->is_background) {
                //WUNTRACED: return if a child has stopped
                p_pid = waitpid(child, & status, WUNTRACED);
                //child exit status check: if exited normally update the corresponding job's "done" status
                if (WIFEXITED(status)) {
                    update_status(p_pid, 0);
                }
            } else {
                total_jobs_bg++;
                //signal for background process.
                signal(SIGCHLD, sigchld_handler);
            }
        }
    }
    return 0;
}

/*
 * Built in jobs.
 * It will print background job.
 * It only print done job once.
 */
int builtin_jobs() {
    char * result;
    int i;
    for (i = 0; i < total_history; i++) {
        if (job_list[i].is_background && !job_list[i].seen_done) {
            result = job_list[i].done ? "Done" : "Running";
            fprintf(stdout, "[%d]  %s %5s\n", i + 1, result, job_list[i].full_command);
            if (job_list[i].done) {
                job_list[i].seen_done = 1;
            }
        }
    }
    return 0;
}

/*
 * Built in history method.
 * Print all history jobs.
 */
int builtin_history(void) {
    int i;
    for (i = 0; i < total_history; i++) {
        if (job_list[i].is_background) {
            fprintf(stdout, "%4d  %s &\n", i + 1, job_list[i].full_command);
        } else {
            fprintf(stdout, "%4d  %s\n", i + 1, job_list[i].full_command);
        }
    }
    return 0;
}

/*
 * Build in exit method.
 * Print summary and exit shell.
 */
int builtin_exit(void) {
    int waitcnt = 0;
    int i;
    int bg = total_jobs_bg;
    for(i = 0; i < total_history; i++){
        if(job_list[i].is_background && job_list[i].done == 0){
            waitcnt++;
        }
    }
    if (waitcnt) {
        fprintf(stdout,"Waiting on %d jobs to finish\n", waitcnt);
        builtin_wait();
    }
    fprintf(stdout,"-------------------------------\n");
    fprintf(stdout,"Total number of jobs               = %d\n", total_history - total_builts);
    fprintf(stdout,"Total number of jobs in history    = %d\n", total_history);
    fprintf(stdout,"Total number of jobs in background = %d\n", bg);
    for(i = 0; i < total_history; i++){
        //free_job(&job_list[i]);
    }
    exit(0);
}

/*
 * Built in wait method.
 * Wait until all background job is done.
 *
 */
int builtin_wait(void) {
    pid_t wpid;
    int status = 0;
    while ((wpid = wait( & status)) > 0) {
        update_status(wpid, 0);
    }
    return 1;
}

/*
 * Build in fg method.
 * For fg method with more than 1 argument.
 */
int builtin_fg_aug(int job_id) {
    int status = 0;
    if (!job_list[job_id - 1].is_background) {
        fprintf(stdout,"Process [%d] is not a background job\n", job_id);
        return 0;
    }
    if (job_list[job_id - 1].done) {
        fprintf(stdout,"Process [%d] is already completed\n", job_id);
        return 0;
    }
    // Change WNOHANG to WUNTRACED for that job.
    waitpid(job_list[job_id - 1].pid, & status, WUNTRACED);
    if (WIFEXITED(status)) {
        update_status(job_list[job_id - 1].pid, 1);
        return 1;
    }
    return 0;
}

/*
 * Build in fg method.
 * It changes background process to foreground jobs.
 */
int builtin_fg(void) {
    int i;
    for (i = total_history - 1; i >= 0; i--) {
        if (job_list[i].is_background) {
            builtin_fg_aug(i + 1);
            return 1;
        }
    }
    return 0;
}
