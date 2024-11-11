#include "monkey_type.h"
#include <stdlib.h>
#include <time.h>
#include <strings.h>
#include <stdbool.h>

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				ret;
	unsigned char	*str1;
	unsigned char	*str2;

	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	ret = 0;
	if (n == 0)
		return (0);
	while (*str1 == *str2 && *str1 != '\0' && --n > 0)
	{
		str1++;
		str2++;
	}
	ret = *str1 - *str2;
	return (ret);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] != '\0')
		i++;
	return (i);
}

int	get_letter(void)
{
	return ((rand() % ('z' - 'a' + 1)) + 'a');
}

void	monkey_loop(char *target)
{
	char	*attempt;
	bool	start = true;
	int	target_len = ft_strlen(target);
	int	i = 0;

	attempt = malloc(target_len);
	while (1){
		if (start){
			start = false;
			bzero(attempt, target_len);
			i = 0;
		}
		attempt[i] = get_letter();
		i++;
		if (i == target_len && !ft_strncmp(attempt, target, target_len)){
			break ;
		}
		else
			start = true;
	}
	return (1);
}


int	main(int argc, char **argv)
{
	srand(time(NULL));
}