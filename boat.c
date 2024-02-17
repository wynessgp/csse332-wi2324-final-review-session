#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <pthread.h>

#include "prettyprint.h"

// global definitions
#define BOAT_CAPACITY 3

// useful enum, you don't have to use them
enum direction { NORTHBOUND = 0, SOUTHBOUND = 1, LAST_ONE };
const char *descs[LAST_ONE] = {"northbound", "southbound"};

// state of the world
#define NORTHBOUND 0
#define SOUTHBOUND 1
int dock = NORTHBOUND;

// the total number of trips made by the boat in either direction
int total_trips = 0;
// the total number of pasengers transported in either direction
int total_passengers = 0;

void *northbound(void *arg)
{
  unsigned long long tid = (unsigned long long)arg;

  pr_red("Northbound passenger %llu arrived.\n", tid);

  // TODO: add the lines below in the right spot to match my output
  // pr_magenta("Boat is full, leaving dock in the northbound direction\n");
  // pr_blue("Northbound passenger %llu on board.\n", tid);
  // pr_magenta("Boat is back at dock and switching directions to southbound.\n");

  // go on the trip
  sleep(1);

  pr_red("Northbound passenger %llu getting off the boat.\n", tid);

  return 0;
}

void *southbound(void *arg)
{
  unsigned long long tid = (unsigned long long)arg;

  pr_blue("Southbound passenger %llu arrived.\n", tid);
  
  // TODO: add the lines below in the right spot to match my output
  // pr_magenta("Boat is full, leaving dock in the southbound direction\n");
  // pr_blue("Southbound passenger %llu on board.\n", tid);
  // pr_magenta("Boat is back at dock and switching directions to northbound.\n");

  // go on the trip
  sleep(1);

  pr_red("Southbound passenger %llu getting off the boat.\n", tid);

  return 0;
}

// test functions
void scenario_one(void);
void scenario_two(void);
void scenario_three(void);
void scenario_four(void);

static void (*test_funcs[])(void) = {
  NULL,
  scenario_one,
  scenario_two,
  scenario_three,
  scenario_four,
  NULL,
};

const char *test_descs[] = {
  "invalid",
  "southbound passengers arrive first, wait for northbound passengers to take the trip.",
  "test the boat going back to forth to get two sets of passengers across.",
  "test that the boat waits until it is full before heading out in any direction.",
  "test that some passengers might have to wait two trips before they get a chance to go.",
  "invalid",
};

int main(int argc, char **argv)
{
  int scenario = 1;

  if(argc > 1)
    scenario = atoi(argv[1]);

  if(test_funcs[scenario]) {
    pr_green("Running scenario %3d: %s\n\n", scenario, test_descs[scenario]);
    test_funcs[scenario]();
  } else {
    pr_red("Usage: %s <scenarion number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  exit(EXIT_SUCCESS);
}

void scenario_one(void)
{
  const int num_direction_threads = 3;
  pthread_t nb_threads[num_direction_threads];
  pthread_t sb_threads[num_direction_threads];
  unsigned long long i;

  // first get the sb thread to all wait
  for(i = 0; i < num_direction_threads; i++) {
    pthread_create(&sb_threads[i], 0, southbound, (void*)(i+1));
  }

  // let those ones go on and wait
  sleep(1);

  // start the nb threads
  for(i = 0; i < num_direction_threads; i++) {
    pthread_create(&nb_threads[i], 0, northbound, (void*)(i+1));
  }

  // join them
  for(i = 0; i < num_direction_threads; i++) {
    pthread_join(nb_threads[i], 0);
    pthread_join(sb_threads[i], 0);
  }

  pr_green("\n[Scenario 1]: Everything finished. The boat made %3d trips "
           "transporting %3d passengers.\n",
           total_trips, total_passengers);
}

void scenario_two(void)
{
  const int num_direction_threads = 6;
  pthread_t nb_threads[num_direction_threads];
  pthread_t sb_threads[num_direction_threads];
  unsigned long long i;

  // first get the sb thread to all wait
  for(i = 0; i < num_direction_threads; i++) {
    pthread_create(&sb_threads[i], 0, southbound, (void*)(i+1));
  }

  // let those ones go on and wait
  sleep(1);

  // start the nb threads
  for(i = 0; i < num_direction_threads; i++) {
    pthread_create(&nb_threads[i], 0, northbound, (void*)(i+1));
  }

  // join them
  for(i = 0; i < num_direction_threads; i++) {
    pthread_join(nb_threads[i], 0);
    pthread_join(sb_threads[i], 0);
  }

  pr_green("\n[Scenario 2]: Everything finished. The boat made %3d trips "
           "transporting %3d passengers.\n",
           total_trips, total_passengers);
}

void scenario_three(void)
{
  unsigned long long i = 0;
  const int num_nb_passengers = 6;
  const int num_sb_passengers = 3;
  pthread_t threads[num_sb_passengers + num_nb_passengers];

  // first three nb passengers arrive and only one sb passenger
  pthread_create(&threads[i], 0, northbound, (void*)i+1); i++;
  pthread_create(&threads[i], 0, northbound, (void*)i+1); i++;
  pthread_create(&threads[i], 0, northbound, (void*)i+1); i++;

  pthread_create(&threads[i], 0, southbound, (void*)(i+1)); i++;
  pthread_create(&threads[i], 0, northbound, (void*)(i+1)); i++;

  // wait some time to make sure the boat is blocked, boat should be stuck in
  // southbound direction
  sleep(3);

  // release the boat in southboud direction
  pthread_create(&threads[i], 0, southbound, (void*)(i+1)); i++;
  pthread_create(&threads[i], 0, southbound, (void*)(i+1)); i++;

  // one more northbound
  pthread_create(&threads[i], 0, northbound, (void*)(i+1)); i++;

  // block in northbound direction until last one gets here
  sleep(5);

  // last one arrives, should go now
  pthread_create(&threads[i], 0, northbound, (void*)(i+1)); i++;

  // wait for all of them to finish
  for(i = 0; i < num_sb_passengers + num_nb_passengers; i++) {
    pthread_join(threads[i], 0);
  }

  pr_green("\n[Scenario 3]: Everything finished. The boat made %3d trips "
           "transporting %3d passengers.\n",
           total_trips, total_passengers);
}

void scenario_four(void)
{
  unsigned long long i = 0;
  const int num_nb_passengers = 12;
  const int num_sb_passengers = 9;
  pthread_t threads[num_nb_passengers + num_sb_passengers];

  // all southbound passengers arrive together (almost), they will be blocked
  for(i = 0; i < num_sb_passengers; i++) {
    pthread_create(&threads[i], 0, southbound, (void*)(i+1));
  }

  // insert some artificial delay
  usleep(100);

  // four nb passengers arrive
  for(; i < num_sb_passengers + 4; i++) {
    pthread_create(&threads[i], 0, northbound, (void*)(i+1));
  }

  // insert some artificial long delay
  sleep(5);

  // last batch of nb passengers arrive
  for(; i < num_sb_passengers + num_nb_passengers; i++) {
    pthread_create(&threads[i], 0, northbound, (void*)(i+1));
  }

  // there should be on more southbound trip before everything ends.

  for(i = 0; i < num_nb_passengers + num_sb_passengers; i++) {
    pthread_join(threads[i], 0);
  }

  pr_green("\n[Scenario 4]: Everything finished. The boat made %3d trips "
           "transporting %3d passengers.\n",
           total_trips, total_passengers);
}
