#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>


pthread_mutex_t ticket;       // Ticket assignment lock
pthread_mutex_t queue;        // Queue lock
pthread_cond_t wait;          // Synchronizer
int tickets;
int passed;


void help(){
  printf("Usage:\n./program <N> <M>\n");
  printf("<N> - integer number of threads\n" );
  printf("<M> - integer number of allowed passes of the critical section\n");
  exit(EXIT_FAILURE);
}


void err(int code, char* err){
  if (code) {
    fprintf(stderr, "Err: %s, code:%d\n", err, code );
    exit(EXIT_FAILURE);
  }
}


int get_num_arg(char* arg){
  char *str = NULL;
  int n = (int) strtod(arg, &str);
  if (strlen(str) != 0) {
    fprintf(stderr, "Parsing failure: %s\n", str);
    return -1;
  }
  return n;
}


void asleep(int id){
  struct timeval t;
  gettimeofday(&t, NULL);   // get current time of the day
  // obtain unique seed from time and unique thread ids
  unsigned int seed = (t.tv_sec * (int)1e6 + t.tv_usec)*pthread_self()*id;

  struct timespec req;
  req.tv_sec = 0;
  req.tv_nsec = rand_r(&seed) % 500000000;  // number has to less than 0.5s
  nanosleep(&req, NULL);
}

/*
* Used mutex to ensure that evert thread has unique ticket
*/
int getticket(void){
  pthread_mutex_lock(&ticket);
  int tmp = tickets;
  tickets++;
  pthread_mutex_unlock(&ticket);
  return tmp;
}

/*
* Lock queue until process is the next in the queue
*/
void await(int aenter){
  pthread_mutex_lock(&queue);
  while (aenter != passed) {
    pthread_cond_wait(&wait, &queue);
  }
}

/*
* End of Critical section, condition is signalized, mutex is unlocked
*/
void advance(void){
  passed++;
  pthread_cond_broadcast(&wait);
  pthread_mutex_unlock(&queue);
}

/*
* Fuction called upon thread spawn
*/
void* serialize(void* argv){
  int* data = (int *) argv;
  int ticket;
  while ((ticket = getticket()) < data[0]) {
    asleep(data[1]);
    await(ticket);
    printf("%d\t(%d)\n", ticket, data[1]);
    advance();
    asleep(data[1]);
  }
  return NULL;
}


void init () {
  int res = 0;
  if ((res = pthread_mutex_init(&ticket, NULL)) != 0)
    err(res, "pthread_mutex_init");
  if ((res = pthread_mutex_init(&queue, NULL)) != 0)
    err(res, "pthread_mutex_init");
  if ((res = pthread_cond_init(&wait, NULL)) != 0)
    err(res, "pthread_cond_init");
  tickets = 0;
  passed = 0;
}


void deinit(){
  int res = 0;
  if ((res = pthread_mutex_destroy(&ticket)) != 0)
    err(res, "pthread_mutex_destroy");
  if ((res = pthread_mutex_destroy(&queue)) != 0)
    err(res, "pthread_mutex_destroy");
  if ((res = pthread_cond_destroy(&wait)) != 0)
    err(res, "pthread_cond_destroy");
}


int main(int argc, char** argv) {
  if (argc !=3) {       // Check number of args
    help();
  }
  int res, n, m;
  // Check if args are numbers
  if ((m = get_num_arg(argv[2])) < 0 || (n = get_num_arg(argv[1])) < 0) {
    help();
  }
  init();
  pthread_t threads[n];
  int param[n][2];
  // Initialize params
  for (int i = 0; i < n; i++) {
    param[i][0] = m;
    param[i][1] = i;
  }
  for (int id = 0; id < n; id++) {
    res = pthread_create(&threads[id], NULL, serialize, &param[id]);
    err(res, "pthread_create\n");
  }
  for (int id = 0; id < n; id++) {
    res = pthread_join(threads[id], NULL);
    err(res, "pthread_join\n");
  }
  deinit();
  return 0;
}
