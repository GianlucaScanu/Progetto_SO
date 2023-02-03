#pragma once
#include <stdint.h>

int total_nodes(int levels);

int first_of_level(int level);

int nodes_in_level(int level);

int buddy(int i);

int right(int i);

int left(int i);

int parent(int i);
