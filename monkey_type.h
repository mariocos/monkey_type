#ifndef MONKEY_TYPE_H
# define MONKEY_TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>
//#include <dispatch/dispatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CLEAR_SCREEN "\033[H\033[J"
#define RESET_CURSOR "\033[H"
#define NUM_CELEBRATION_FRAMES (sizeof(celebration_frames) / sizeof(celebration_frames[0]))

#define WORD_LENGTH 20

typedef struct s_monkey {
    char    **target_words;
    int     target_word_index;
    int     num_words;
}   t_monkey;

t_monkey    *monkey_call(void);
void        monkey_init(void);
int         read_words(const char *filename);
char        generate_random_char();
void        *monkey_typing(void *arg);

// extern sem_t match_sem;
// extern sem_t word_sem;
extern dispatch_semaphore_t match_sem;
extern dispatch_semaphore_t word_sem;

#endif
