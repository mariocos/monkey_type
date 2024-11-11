#include "monkey_type.h"

typedef struct s_monkey
{
	char	**target_words;
	int		target_word_index;
}		t_monkey;


t_monkey	*monkey_call(void)
{
	static t_monkey	monkey;

	return (&monkey);
}

void	monkey_init(void)
{
	monkey_call()->target_words = NULL;
	
}