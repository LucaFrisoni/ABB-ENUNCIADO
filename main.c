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

bool validando_func(tp1_t *tp1, abb_t *abb)
{
	if (tp1 && !abb) {
		tp1_destruir(tp1);
		return false;
	}
	if (!tp1 && abb) {
		abb_destruir(abb);
		return false;
	}
	if (!tp1 && !abb)
		return false;
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
// ----------------------- Funciones Iterador Interno -----------------------
bool buscando_por_nombre(void *a, void *extra)
{
	struct pokemon *pk_en_abb = (struct pokemon *)a;

	void **param = (void **)extra;

	char *nombre_buscado = (char *)param[0];
	struct pokemon **pk = (struct pokemon **)param[1];

	if (strcmp(pk_en_abb->nombre, nombre_buscado) == 0) {
		*pk = pk_en_abb; // => pk ya no apunta a NULL, sino al pk_en_abb
		return false; // Salimos de la iteracion
	}
	return true;
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

	if (!validando_func(tp1, abb))
		return 0;

	tp1_con_cada_pokemon(tp1, guardar_en_abb, abb);

	// Buscar por nombre
	if (strcmp(argv[3], "nombre") == 0) {
		char *nombre_buscado = argv[4];
		void *extra[2] = { nombre_buscado, &p };
		abb_con_cada_elemento(
			abb, 0, buscando_por_nombre,
			extra); //O(n) si o si en el peor de los casos
	}

	// Buscar por id
	if (strcmp(argv[3], "id") == 0) {
		//Inicializo un pokemon con todo NULL o 0
		struct pokemon poke_aux = { 0 };
		poke_aux.id = atoi(argv[4]);

		p = abb_buscar(
			abb,
			&poke_aux); // O(logn) porque estan ordenados por id
	}

	if (p)
		mostrar_pokemon(p);
	else
		printf("No se encontró el pokemon\n");

	abb_destruir(abb);
	tp1_destruir(tp1); //Este ya libera los pokemones
	return 0;
}