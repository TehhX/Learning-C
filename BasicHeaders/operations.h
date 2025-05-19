#pragma once

// .h variables should be delcared with extern, and defined in .c files

extern int five;

extern const float pi;

// Or just defined

#define euler 2.71828

// Same declaration/definition in .h and .c file kinda thing as in C++

int add(int x, int y);

int subtract(int x, int y);

int multiply(int x, int y);

// No protection against div by 0
int divide(int x, int y);

void printThing(const char thing[]);
