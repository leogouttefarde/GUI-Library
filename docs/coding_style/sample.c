/*
 * Programme de demonstration
 */

#include <stdio.h>
#include <assert.h>

#include "fonctions.h"

#define TOTO 0

typedef struct exemple_type {
	struct exemple_type *prev, *next;
	uint32_t vertex_x;
	uint32_t vertex_y;
	uint32_t vertex_z;
} exemple_type_t;

/*
 * Procedure exemple_fonction
 * Commentaires longs
 */
void exemple_fonction(exemple_type_t parametre, uint32_t size_x, uint32_t size_y)
{
	uint32_t nb_lig, nb_col;
	/* Commentaires courts */
	<instruction...>;

	if (<...>) {
		<instruction...>;
	}

	while (<...>) {
		<instruction...>;
	}

	do {
		<instruction...>;
	} while (<...>);

	for (nb_col = 0; nb_col < size_y; nb_col++) {
		for (nb_lig = 0; nb_lig < size_x; nb_lig++) {
			<instruction...>;
			<instruction...>;
			if (<...>) {
				if (<...>) {
					<instruction...>;
					<instruction...>;
				} else if (<...>) {
					<instruction...>;
					<instruction...>;
				}
			}
		}
	}

	switch(<...>) {
	case <...>:
		<instruction...>;
		break;
	case <...>:
		<instruction...>;
		break;
	default :
		<instruction...>;
		break;
	}

}

/*
 * Programme principal
 * ...
 */
int main(int argc, char **argv)
{
	<instruction...>;
	return 0;
}