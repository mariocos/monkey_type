#include "monkey_type.h"

dispatch_semaphore_t match_sem;
dispatch_semaphore_t word_sem;

// Function to read words from file
int read_words(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        return -1;
    }

    // Count words first
    int num_words = 0;
    char temp[WORD_LENGTH];
    while (fscanf(file, "%s", temp) != EOF) {
        num_words++;
    }
    monkey_call()->num_words = num_words;

    // Allocate space for words and reset file pointer
    monkey_call()->target_words = malloc(num_words * sizeof(char *));
    fseek(file, 0, SEEK_SET); // Move file pointer back to the start

    // Read words into array
    for (int i = 0; i < num_words; i++) {
        monkey_call()->target_words[i] = malloc(WORD_LENGTH * sizeof(char));
        fscanf(file, "%s", monkey_call()->target_words[i]);
    }
    fclose(file);
    return 0;
}

// Function to generate a random character
char generate_random_char() {
    return 'a' + rand() % 26;
}

// Thread function for monkeys
void *monkey_typing(void *arg) {
    int monkey_id = (int)(long)arg;
    char generated_word[WORD_LENGTH];
    int word_len;
    int generated_len = 0;

    while (1) {
        // Get access to the current word index
        dispatch_semaphore_wait(match_sem, DISPATCH_TIME_FOREVER);
        if (monkey_call()->target_word_index >= monkey_call()->num_words) {
            dispatch_semaphore_signal(match_sem);
            break; // All words are completed
        }
        word_len = strlen(monkey_call()->target_words[monkey_call()->target_word_index]);
        dispatch_semaphore_signal(match_sem);

        // Monkey adds letters one by one
        generated_word[generated_len++] = generate_random_char();

        // Check if generated_word length matches the target word length
        if (generated_len == word_len) {
            generated_word[generated_len] = '\0'; // Null-terminate generated word

            // Check if generated word matches the target word
            dispatch_semaphore_wait(match_sem, DISPATCH_TIME_FOREVER);
            if (strcmp(generated_word, monkey_call()->target_words[monkey_call()->target_word_index]) == 0) {
                printf("Monkey %d typed the correct word: %s\n", monkey_id, generated_word);
				celebrate();

                // Move to the next word
                monkey_call()->target_word_index++;
                generated_len = 0; // Reset for next word
            } else {
                // Reset generated length for this word attempt
                generated_len = 0;
            }
            dispatch_semaphore_signal(match_sem);
        }
    }
    return NULL;
}

const char *celebration_frames[] = {
    "   \\(o_o)/   \n"
    "     ( )     \n"
    "    /   \\    \n",

    "   \\(^_^)   \n"
    "     ( )  \\   \n"
    "    /   \\    \n",

    "    (^_^)/   \n"
    "   / ( )     \n"
    "    /   \\    \n",

    "   \\(^_^)    \n"
    "     ( )     \n"
    "    /   \\    \n",
};

void celebrate() {
    for (int i = 0; i < 10; i++) { // Loop through animation frames a few times
        // Display each frame
        printf(CLEAR_SCREEN); // Clear the screen
        printf("%s", celebration_frames[i % NUM_CELEBRATION_FRAMES]); // Show the frame
        printf(RESET_CURSOR); // Reset cursor to the top
        fflush(stdout); // Force immediate output
        usleep(200000); // Delay between frames (200 ms)
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s {filename} {num_monkeys}\n", argv[0]);
        return 1;
    }

    int num_monkeys = atoi(argv[2]);

    monkey_init();

    if (read_words(argv[1]) != 0) {
        return 1;
    }

    // Initialize dispatch semaphores
    match_sem = dispatch_semaphore_create(1);
    // sem_init(&match_sem, 0, 1);


    pthread_t monkeys[num_monkeys];
    srand(time(NULL)); // Seed random number generator

    // Create monkey threads
    for (long i = 0; i < num_monkeys; i++) {
        pthread_create(&monkeys[i], NULL, monkey_typing, (void *)i);
    }

    // Join monkey threads
    for (int i = 0; i < num_monkeys; i++) {
        pthread_join(monkeys[i], NULL);
    }

    // Clean up
    dispatch_release(match_sem);
    // sem_destroy(&match_sem);
    for (int i = 0; i < monkey_call()->num_words; i++) {
        free(monkey_call()->target_words[i]);
    }
    free(monkey_call()->target_words);

    return 0;
}
