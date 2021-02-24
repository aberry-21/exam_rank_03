/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   micro_paint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberry <aberry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 15:37:21 by aberry            #+#    #+#             */
/*   Updated: 2021/02/24 15:37:22 by aberry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//WIDTH HEIGHT BACKGROUND_CHAR
typedef struct		s_map
{
	int				width;
	int				height;
	char			back_ch;
}					t_map;

//r X Y WIDTH HEIGHT CHAR
typedef struct		s_rect
{
	char			fill_or_empty;
	float			x;
	float			y;
	float			width;
	float			height;
	char			draw_ch;
}					t_rect;

int			size_array;
t_map		map;
FILE		*file;
t_rect		*array_rect;

size_t		ft_strlen(char *str)
{
	size_t		counter;

	counter = 0;
	while(str[counter])
		counter++;
	return (counter);
}

void		ft_clear(void)
{
	if(array_rect)
		free(array_rect);
	if(file)
		fclose(file);
}

void		ft_error_message(char *message)
{
	ft_clear();
	write(1, message, ft_strlen(message));
	exit(1);
}

void		ft_init_map(void)
{
	int		size_fscanf;

	size_fscanf = fscanf(file, "%d %d %c\n", &map.width, &map.height, &map.back_ch);
	if(size_fscanf != 3)
		ft_error_message("Error: Operation file corrupted\n");
	if(map.width <= 0 || map.width > 300)
		ft_error_message("Error: Operation file corrupted\n");
	if(map.height <= 0 || map.height > 300)
		ft_error_message("Error: Operation file corrupted\n");
}

int			ft_init_rect(int index)
{
	int		size_fscanf;

	array_rect = (t_rect *)realloc(array_rect, (index + 1) * sizeof(t_rect));
	if (!array_rect)
		ft_error_message("Error: malloc\n");
	size_fscanf = fscanf(file, "%c %f %f %f %f %c\n", &array_rect[index].fill_or_empty, &array_rect[index].x, &array_rect[index].y,\
		   												&array_rect[index].width, &array_rect[index].height, &array_rect[index].draw_ch);
	if(size_fscanf > 0 && size_fscanf < 6)
		ft_error_message("Error: Operation file corrupted\n");
	if(size_fscanf != -1)
	{
		if(array_rect[index].width <= 0.0 || array_rect[index].height <= 0.0)
			ft_error_message("Error: Operation file corrupted\n");
		if(array_rect[index].fill_or_empty != 'r' && array_rect[index].fill_or_empty != 'R')
			ft_error_message("Error: Operation file corrupted\n");
	}
	return (size_fscanf);
}

int			ft_check_coord(int index, float x, float y)
{
	if ((x < array_rect[index].x || x > (array_rect[index].x + array_rect[index].width)) ||
		(y < array_rect[index].y || y > (array_rect[index].y + array_rect[index].height)))
			return (0);
	if((x - array_rect[index].x < 1.0 || (array_rect[index].x + array_rect[index].width) - x < 1.0) ||
		(y - array_rect[index].y < 1.0 || (array_rect[index].y + array_rect[index].height) - y < 1.0))
			return (2);
	return (1);
}


void		ft_drawing_map(void)
{
	int		x;
	int		y;
	int		index;
	int		flag;
	int		res;

	y = 0;
	flag = 0;
	while(y < map.height)
	{
		x = 0;
		while(x < map.width)
		{
			index = size_array - 1;
			while(index > -1)
			{
				flag = 0;
				res = ft_check_coord(index, x, y);
				if((res && array_rect[index].fill_or_empty == 'R') ||
					(res == 2 && array_rect[index].fill_or_empty == 'r'))
				{
					flag = write(1, &array_rect[index].draw_ch, 1);
					break;
				}
				index--;
			}
			if (!flag)
				 write(1, &map.back_ch, 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

int			main(int argc, char **argv)
{
	file = NULL;
	array_rect = NULL;
	if(argc != 2)
		ft_error_message("Error: argument\n");
	if(!(file = fopen(argv[1], "r")))
		ft_error_message("Error: argument\n");
	ft_init_map();
	size_array = 0;
	while(ft_init_rect(size_array) != -1)
		size_array++;
	ft_drawing_map();
	ft_clear();
	return (0);
}
