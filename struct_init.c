#include "monkey_type.h"

t_monkey    *monkey_call(void) {
    static t_monkey monkey;
    return &monkey;
}

void    monkey_init(void) {
    t_monkey *monkey = monkey_call();
    monkey->target_words = NULL;
    monkey->target_word_index = 0;
    monkey->num_words = 0;
}
