#include "src/abb.h"
#include "src/tp1.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ----------------------- Validaciones -----------------------
bool validando_formato_csv(const char *archivo)
{
	if (!archivo)
		return false;

	const char *ext = strrchr(archivo, '.');
	if (!ext || strcmp(ext, ".csv") != 0)
		return false;

	return true;
}

bool validando_params(int argc, char *argv[])
{
	if (argc < 5) {
		printf("Se deben de pasar los 5 parametros permitidos <ejecutable> <archivo_pokemones.csv> buscar nombre|id <valor>\n");
		return false;
	}
	if (argc > 5) {
		printf("Solo 5 parametros permitidos <ejecutable> <archivo_pokemones.csv> buscar nombre|id <valor>\n");
		return false;
	}
	if (!validando_formato_csv(argv[1])) {
		printf("El archivo pasado debe de estar en formato 'csv'\n");
		return false;
	}
	if (strcmp(argv[2], "buscar") != 0) {
		printf("El tercer argumento debe ser 'buscar'\n");
		return false;
	}
	if (strcmp(argv[3], "nombre") != 0 && strcmp(argv[3], "id") != 0) {
		printf("El cuarto argumento para buscar debe ser 'nombre' o 'id'\n");
		return false;
	}
	return true;
}
// ----------------------- Callback para guardar en arbol -----------------------
bool guardar_en_abb(struct pokemon *poke, void *extra)
{
	abb_t *abb = extra;
	return abb_insertar(abb, poke);
}
// ----------------------- Comparadores -----------------------
int comparador_id_pk(const void *a, const void *b)
{
	const struct pokemon *p1 = (const struct pokemon *)a;
	const struct pokemon *p2 = (const struct pokemon *)b;

	if (p1->id == p2->id)
		return 0;
	return (p1->id > p2->id) ? 1 : -1;
}

// ----------------------- Mostrar -----------------------
const char *convert_tipo_a_string(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return "DESCONOCIDO";
	}
}

void mostrar_pokemon(struct pokemon *p)
{
	if (!p)
		return;
	printf("Id: %d\n", p->id);
	printf("Nombre: %s\n", p->nombre);
	printf("Tipo: %s\n", convert_tipo_a_string(p->tipo));
	printf("Ataque: %d\n", p->ataque);
	printf("Defensa: %d\n", p->defensa);
	printf("Velocidad: %d\n\n", p->velocidad);
}

int main(int argc, char *argv[])
{
	if (!validando_params(argc, argv))
		return 0;

	struct pokemon *p = NULL;
	tp1_t *tp1 = tp1_leer_archivo(argv[1]);
	abb_t *abb = abb_crear(comparador_id_pk);
	if (!tp1 || !abb)
		return 0;

	tp1_con_cada_pokemon(tp1, guardar_en_abb, abb);

	// Buscar por nombre
	if (strcmp(argv[3], "nombre") == 0) {
	}

	// Buscar por id
	if (strcmp(argv[3], "id") == 0) {
		int id = atoi(argv[4]);
	}

	if (p)
		mostrar_pokemon(p);
	else
		printf("No se encontró el pokemon\n");

	abb_destruir(abb);
	tp1_destruir(tp1);
	return 0;
}