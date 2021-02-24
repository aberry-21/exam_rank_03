/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aberry <aberry@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/24 15:38:03 by aberry            #+#    #+#             */
/*   Updated: 2021/02/24 15:38:49 by aberry           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

//WIDTH HEIGHT BACKGROUND_CHAR
typedef struct	s_map
{
	int			width;
	int			height;
	char		back_ch;
}				t_map;

//c X Y RADIUS CHAR
typedef struct	s_circle
{
	char		fill_or_empty;
	float		x;
	float		y;
	float		radius;
	char		draw_ch;
}				t_circle;

t_map			map;
t_circle		*array_circle;
FILE			*file;
int				size_array;

int				ft_strlen(char *str)
{
	int			counter;

	counter = 0;
	while(str[counter])
		counter++;
	return (counter);
}

void			ft_clean(void)
{
	if(file)
		fclose(file);
	if(array_circle)
		free(array_circle);
}

void			ft_error_message(char *message)
{
	ft_clean();
	write(1, message, ft_strlen(message));
	exit (1);
}

void			ft_init_map(void)
{
	int			size_fscanf;

	size_fscanf = fscanf(file, "%d %d %c\n", &map.width, &map.height, &map.back_ch);
	if(size_fscanf != 3)
		ft_error_message("Error: Operation file corrupted\n");
	if(map.width <= 0 || map.width > 300)
		ft_error_message("Error: Operation file corrupted\n");
	if(map.height <= 0 || map.height > 300)
		ft_error_message("Error: Operation file corrupted\n");
}

int				ft_init_circle(int index)
{
	int			size_fscanf;

	array_circle = (t_circle *)realloc(array_circle, (index + 1) * sizeof(t_circle));
	if(!array_circle)
		ft_error_message("Error: malloc\n");
	size_fscanf = fscanf(file, "%c %f %f %f %c\n", &array_circle[index].fill_or_empty, &array_circle[index].x,\
							&array_circle[index].y, &array_circle[index].radius, &array_circle[index].draw_ch);
	if(size_fscanf > 0 && size_fscanf < 5)
		ft_error_message("Error: Operation file corrupted\n");
	if(size_fscanf != -1)
	{
		if(array_circle[index].radius <= 0.0)
			ft_error_message("Error: Operation file corrupted\n");
		if(array_circle[index].fill_or_empty != 'C' && array_circle[index].fill_or_empty != 'c')
			ft_error_message("Error: Operation file corrupted\n");
	}
	return(size_fscanf);
}

int				ft_check_coord(int index, float x, float y)
{
	float		dist;

	dist = sqrtf(powf((x - array_circle[index].x), 2) + powf((y - array_circle[index].y), 2));
	if(dist <= array_circle[index].radius)
	{
		if(array_circle[index].radius - dist < 1.0)
			return (2);
		return (1);
	}
	return (0);
}

void			ft_drawing_map(void)
{
	int			x;
	int			y;
	int			res;
	int			index;
	int			flag;

	flag = 0;
	y = 0;
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
				if((res && array_circle[index].fill_or_empty == 'C') ||
					(res == 2 && array_circle[index].fill_or_empty == 'c'))
				{
					flag = write(1, &array_circle[index].draw_ch, 1);
					break;
				}
				index--;
			}
			if(!flag)
				write(1, &map.back_ch, 1);
			x++;
		}
		write(1, "\n", 1);
		y++;
	}
}

int				main(int argc, char **argv)
{
	file = NULL;
	array_circle = NULL;
	if(argc != 2)
		ft_error_message("Error: argument\n");
	if(!(file = fopen(argv[1], "r")))
		ft_error_message("Error: argument\n");
	ft_init_map();
	size_array = 0;
	while(ft_init_circle(size_array) != -1)
		size_array++;
	ft_drawing_map();
	ft_clean();
	return (0);
}
