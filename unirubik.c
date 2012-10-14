/*
    This file is part of unirubik
    Copyright (C) 1988-2012 Mark Longridge <cubeman@vector.nothing.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* Unirubik is an attempt to make a universal Rubik's Cube simulation 
   which can be compiled with any ANSI C compiler
   
   First version made on August 1st, 1988 
   Compiled on Linux Redhat 8.0 on March 17th, 2003
   
   Compiles on:
   
     QuickC (Intel X86 DOS)  1988
     Lattice C (Amiga)       1988
     gcc (Intel X86 Linux)   2003
     gcc 4.0.1 (OS X 10.5)   2012

     The program appends .q to the end of saved files

     One can enter a string of cube operators to be processed,
     e.g. uuffrruuulldbrrrbrrrbrrrdddlluuu gives 'cube in a cube'

     Singmaster (befuddler) notation used: 
     u = up
     d = down
     f = front
     b = back
     l = left
     r = right
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define D 3    /* Cube is size '3' */
#define SIDES 6

#define TOP   0
#define DOWN  1
#define FRONT 2
#define BACK  3
#define LEFT  4
#define RIGHT 5

unsigned short c;
short d;
short s1, s2, s3, s4;
char k;

/* Define 6 sides of Cube's surface and image */

short q[6][D][D], q1[6][D][D];

void rotate_face(short);
void help();
void initial_sides();
void refresh_image();
void display_sides();

void top();
void down();
void front();
void back();
void left();
void right();

void load();
void load_file( FILE *input);
void save();
void save_file ( FILE *output);

void mix();

int main()
{
	d=D;
	
	printf ("\nunirubik 1.0  Rubik's Cube Simulation\n");
	printf ("\nDeveloped by Mark Longridge  April 1988\n\n");

	initial_sides ();
	refresh_image ();
	display_sides ();

	while (( k = getchar()) !='q')
	{
		switch (k)
			{
				case 'u':  top();  break;
				case 'd':  down();  break;
				case 'f':  front();  break;
				case 'b':  back();  break;
				case 'l':  left();  break;
				case 'r':  right();  break;
			        case 'm':  mix();   break;
			        case '*':  initial_sides ();
					   refresh_image();  break;
				case '[':  load(); break;
			        case ']':  save(); break;
			        case '?':  help(); break;
			        case '\n': display_sides();  break;
			}
	}
	return(1); 
}

void initial_sides()
{
	short x,y,z;
	for (z=0; z<=SIDES-1; z++)
		for (x=0; x<=d-1; x++)
			for (y=0; y<=d-1; y++)
				q[z][x][y] = z;
}

void refresh_image()
{
	short x,y,z;
	for (z=0; z<=SIDES-1; z++)
		for (x=0; x<=d-1; x++)
			for (y=0; y<=d-1; y++)
				q1[z][x][y] = q[z][x][y];
}

void display_sides()
{
	short x,y;
	printf ("              TOP\n");
	
	for (x=0; x<=d-1; x++)
	{
		printf ("          ");
		for (y=0; y<=2; y++)
		{
			printf ("%3d", q[TOP][x][y]);
		}
		printf ("\n");
	}
	
	printf ("    LEFT     FRONT   RIGHT      BACK\n");
	
	for (x=0; x<=d-1; x++)
	{
		for (y=0; y<=d-1; y++)
		{
			printf ("%3d", q[LEFT][x][y]);
		}
		for (y=0; y<=d-1; y++)
		{
			printf ("%3d", q[FRONT][x][y]);
		}
		for (y=0; y<=d-1; y++)
		{
			printf ("%3d", q[RIGHT][x][y]);
		}
		printf ("  ");
		for (y=0; y<=d-1; y++)
		{
			printf ("%3d", q[BACK][x][y]);
		}
		printf ("\n");
	}
	printf ("             DOWN\n");
        for (x=0; x<=d-1; x++)
	{
		printf ("          ");
	        for (y=0; y<=d-1; y++)
	        {
		     printf ("%3d", q[DOWN][x][y]);
	        }
		printf ("\n");
	}
	printf ("\n");
}

void top()
{
	short y,z;
	z = TOP;
	rotate_face(z);

	for (y=0; y<=d-1; y++)
	{
		q[LEFT][0][y] = q1[FRONT][0][y];
		q[BACK][0][y] = q1[LEFT][0][y];
		q[RIGHT][0][y]= q1[BACK][0][y];
		q[FRONT][0][y]= q1[RIGHT][0][y];
	}
	refresh_image();
}

void down()
{
	short y,z;
	z = DOWN;
	rotate_face (z);

	for (y=0; y<=d-1; y++)
	{
		q[LEFT][d-1][y] = q1[BACK][d-1][y];
		q[BACK][d-1][y] = q1[RIGHT][d-1][y];
		q[RIGHT][d-1][y]= q1[FRONT][d-1][y];
		q[FRONT][d-1][y]= q1[LEFT][d-1][y];
	}
	refresh_image();
}

void front()
{
	short x,z;
	z = FRONT;
	rotate_face (z);

	for (x=0; x<=d-1; x++)
	{
		z = 2-x;
		q[LEFT][z][d-1] = q1[DOWN][0][z];
		q[TOP][d-1][x] =  q1[LEFT][z][d-1];
		q[RIGHT][x][0] =  q1[TOP][d-1][x];
		q[DOWN][0][z]  =  q1[RIGHT][x][0];
	}
	refresh_image();
}

void back()
{
	short x,z;
	z = BACK;
	rotate_face (z);

	for (x=0; x<=d-1; x++)
	{
		z = 2-x;
		q[LEFT][x][0] = q1[TOP][0][z];
		q[TOP][0][x] =   q1[RIGHT][x][2];
		q[RIGHT][x][2]= q1[DOWN][d-1][z];
		q[DOWN][d-1][x]=q1[LEFT][x][0];
	}
	refresh_image();
}

void left ()
{
	short x,z;
	z = LEFT;
	rotate_face (z);

	for (x=0; x<=d-1; x++)
	{
		z = 2-x;
		q[TOP][x][0] =   q1[BACK][z][d-1];
		q[FRONT][x][0] = q1[TOP][x][0];
		q[DOWN][x][0] =  q1[FRONT][x][0];
		q[BACK][x][d-1] =q1[DOWN][z][0];
	}
	refresh_image();
}
	

void right ()
{
	short x,z;
	z = RIGHT;
	rotate_face (z);

	for (x=0; x<=d-1; x++)
	{
		z= 2-x;
		q[TOP][x][d-1]   = q1[FRONT][x][d-1];
		q[FRONT][x][d-1] = q1[DOWN][x][d-1];
		q[DOWN][x][d-1]  = q1[BACK][z][0];
		q[BACK][x][0]    = q1[TOP][z][2];
	}
	refresh_image();
}

void rotate_face (z)
short z;
{
	short x,y;
	for (x=0; x<=d-1; x++)
		for (y=0; y<=d-1; y++)
		{
			s1 = y;
			s2 = x;
			s3 = 2-x;
			s4 = y;
			q[z][s1][s2] = q1[z][s3][s4];
		}
}

void save()
{
	FILE *fopen(), *fp_out;
	char fn_out[30];
	printf ("Enter name of output file: ");
	scanf ("%s", fn_out);
	strcat (fn_out,".q");
	fp_out = fopen (fn_out, "w");
	save_file (fp_out);
	printf ("\nOutput in file: %s\n", fn_out);
	fclose (fp_out);
}

void save_file (output)
FILE *output;
{
	short x,y,z;
	for (z=0; z<=SIDES-1; z++)
		for (x=0; x<=d-1; x++)
			for (y=0; y<=d-1; y++)
				fputc (q[z][x][y], output);
	refresh_image();
}

void load()
{
	FILE *fopen(), *fp_in;
	char fn_in[30];
	printf ("Enter name of input file: ");
	scanf ("%s", fn_in);
	strcat (fn_in, ".q");
	
	fp_in = fopen (fn_in, "r");

	if (fp_in == NULL)
	{
		printf ("No input file %s\n", fn_in);
		goto out;
	}
	load_file (fp_in);
	printf ("\nInput in file: %s\n", fn_in);
	fclose (fp_in);
        out:;
}

void load_file (fp_in)
FILE *fp_in;
{
	short x,y,z;
	for (z=0; z<=SIDES-1; z++)
		for (x=0; x<=d-1; x++)
			for (y=0; y<=d-1; y++)
				q[z][x][y] = fgetc(fp_in);
}
	
void mix()
{
	short x;
	srand (time(NULL));
	for (x=0; x<=20; x++)
	{
		/* Choose a random number */
		c = rand() % SIDES;
		
	/*	printf ("%d ", c); */
		switch (c)
		{
			case 0:
				top();
				break;
			case 1:
				down();
				break;
			case 2:
				left();
				break;
			case 3:
				right();
				break;
			case 4:
				front();
				break;
			case 5: 
				back();
				break;
		}
	}
}	

void help()
{
	printf ("u up     d down   m mix\n");
	printf ("f front  b back   * reset\n");
	printf ("l left   r right\n");
	printf ("[ load   ] save\n");
	printf ("q quit\n\n");
}
