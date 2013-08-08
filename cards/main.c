#include <stdio.h>
#include <stdlib.h>

/* How many cards to pick? The story says 26 */
#define NUM_PICKED 26

/* Pick a random number from 0 to 51 */
#define RAND_CARD (rand() % 52)

int main(int argc, char **argv) {
  int num_times = 1; /* Number of times to do the big loop */

  short cur_card;    /* Card we're working on */
  int   num_aces;    /* Number of aces in set */

  short i;           /* Re-used var */

  long  stats[5];    /* Number of times 0 aces occured, and 1, 2, 3, 4 */
  short cards[52];   /* A deck of cards 0 = not picked, 1 = flipped */

  /* Get number of times to run test from input */
  if (argc == 2)
    num_times = atoi(argv[1]);

  printf("Running test %d time(s)\n", num_times);

  /* Initialize seed. Not all that random, but simple. */
  srand(num_times);

  /* Set stats[] to 0 */
  for(i = 0; i < 5; stats[i++] = 0);

  while(num_times--) {

    /* Clear the deck of cards */
    for(i = 0; i < 52; cards[i++] = 0);

    num_aces = 0;

    /* Select one card at a time. */
    for(i = 0; i < NUM_PICKED; i++) {

      /* Pick one that hasn't been picked already */
      while(cards[cur_card = RAND_CARD]);

      /* Set it to "picked" */
      cards[cur_card] = 1;

      /* Add to num_aces if it's an ace. 0, 1, 2, and 3 are "aces" */
      if (cur_card < 4)
        num_aces++;

    } /* for(i = 0; i < NUM_PICKED; i++) */

    /* Add to the stats */
    stats[num_aces]++;

  } /* while(num_times--) */

  /* Print stats */
  for(i = 0; i < 5; i++)
    printf("Number of times %d aces were picked: %d\n", i, stats[i]);

  return 0;
}
