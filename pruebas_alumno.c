#include "pa2m.h"
#include "src/abb.h"
#include "string.h"

typedef struct persona {
	char *nombre;
	int edad;
} persona_t;

bool insertando_dinamic_en_abb(abb_t *abb, int cant)
{
	for (int i = 0; i < cant; i++) {
		int *num = malloc(sizeof(int));
		if (!num)
			false;
		*num = i;
		if (!abb_insertar(abb, num)) {
			free(num);
			return false;
		}
	}
	return true;
}
// ----------------------Comparadores de pruebas--------------------------------------------
int comparador_num(const void *num1, const void *num2)
{
	int num1_ = *(int *)num1;
	int num2_ = *(int *)num2;

	return (num1_ < num2_) ? -1 : (num1_ > num2_) ? 1 : 0;
}
int comparador_string(const void *string1, const void *string2)
{
	char *string1_ = (char *)string1;
	char *string2_ = (char *)string2;

	return strcmp(string1_, string2_);
}
int comparador_double(const void *d1, const void *d2)
{
	double a = *(double *)d1;
	double b = *(double *)d2;

	return (a < b) ? -1 : (a > b) ? 1 : 0;
}
int comparador_fecha(const void *f1, const void *f2)
{
	return strcmp((char *)f1, (char *)f2);
}
int comparador_array_suma(const void *a1, const void *a2)
{
	const int *arr1 = (int *)a1;
	const int *arr2 = (int *)a2;
	int suma1 = arr1[0] + arr1[1] + arr1[2];
	int suma2 = arr2[0] + arr2[1] + arr2[2];
	return suma1 - suma2;
}
int comparador_booleanos(const void *b1, const void *b2)
{
	return (*(int *)b1) - (*(int *)b2);
}
int comparador_persona_nombre(const void *p1, const void *p2)
{
	persona_t *per1 = (persona_t *)p1;
	persona_t *per2 = (persona_t *)p2;
	return strcmp(per1->nombre, per2->nombre);
}
int comparador_null(const void *a, const void *b)
{
	if (a == NULL && b == NULL)
		return 0; // NULL == NULL
	if (a == NULL)
		return -1; // NULL es menor que todo
	if (b == NULL)
		return 1; // todo es mayor que NULL
	return 0;
}
// -----------------------------TESTS-------------------------------------
void test_abb_crear_sin_comp()
{
	abb_t *abb = abb_crear(NULL);
	pa2m_afirmar(abb == NULL,
		     "Crear un abb pasando un comparador NULL, devuelve NULL");
}
void test_abb_crear_con_comp_num()
{
	abb_t *abb = abb_crear(comparador_num);
	pa2m_afirmar(
		abb != NULL,
		"Crea un abb correctamente cuando se le pasa un comparador de numeros");
	abb_destruir(abb);
}
void test_abb_crear_con_comp_string()
{
	abb_t *abb = abb_crear(comparador_string);
	pa2m_afirmar(
		abb != NULL,
		"Crea un abb correctamente cuando se le pasa un comparador de strings");
	abb_destruir(abb);
}
void test_abb_crear_con_comp_double()
{
	abb_t *abb = abb_crear(comparador_double);
	pa2m_afirmar(
		abb != NULL,
		"Crea un abb correctamente cuando se le pasa un comparador de double");
	abb_destruir(abb);
}
void test_abb_crear_con_comp_bool()
{
	abb_t *abb = abb_crear(comparador_booleanos);
	pa2m_afirmar(
		abb != NULL,
		"Crea un abb correctamente cuando se le pasa un comparador de booleanos");
	abb_destruir(abb);
}
void test_abb_crear_multiples_comp_personalizados()
{
	abb_t *abb = abb_crear(comparador_fecha);
	abb_t *abb2 = abb_crear(comparador_array_suma);
	pa2m_afirmar(abb && abb2,
		     "Se pueden crear ABBs con comparadores personalizados");
	abb_destruir(abb);
	abb_destruir(abb2);
}
void test_abb_crear_raiz_null()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(abb_raiz(abb) == NULL,
		     "Crear un abb inicializa el arbol con la raiz null");
	abb_destruir(abb);
}
void test_abb_crear_cantidad_correcta()
{
	abb_t *abb = abb_crear(comparador_num);
	pa2m_afirmar(abb_cantidad(abb) == 0,
		     "Un abb recien creado se inicializa con la cantidad 0");
	abb_destruir(abb);
}

void tests_abb_crear()
{
	test_abb_crear_sin_comp();
	test_abb_crear_con_comp_num();
	test_abb_crear_con_comp_string();
	test_abb_crear_con_comp_double();
	test_abb_crear_con_comp_bool();
	test_abb_crear_multiples_comp_personalizados();
	test_abb_crear_raiz_null();
	test_abb_crear_cantidad_correcta();
}
// ------------------------------------------------------------------
void test_abb_insertar_abb_null()
{
	int num = 0;
	pa2m_afirmar(abb_insertar(NULL, &num) == false,
		     "Tratar de insertar pasando un abb NULL, devuelve false");
}
void test_abb_insertar_vacio()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 1);
	pa2m_afirmar(abb_esta_vacio(abb) == false,
		     "Luego de insertar el abb deja de estar vacio");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_primer_nodo()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(insertando_dinamic_en_abb(abb, 1) == true,
		     "Se inserta correctamente el primer nodo");

	pa2m_afirmar(*(int *)abb_raiz(abb) == 0,
		     "El primer nodo insertado es la raiz del abb");
	abb_destruir_todo(abb, free);
}
void test_abb_insertar_varios_nodos()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(insertando_dinamic_en_abb(abb, 3) == true,
		     "Se insertan correctamente varios nodos");
	pa2m_afirmar(*(int *)abb_raiz(abb) == 0,
		     "Insertar varios nodos no modifica la raiz del abb");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_aumenta_cantidad()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 2);
	pa2m_afirmar(
		abb_cantidad(abb) == 2,
		"Insertar un nodo aumenta correctamente la cantidad del arbol");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_mismo_dato()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "a");

	pa2m_afirmar(abb_insertar(abb, "a") == true,
		     "Se puede insertar el mismo dato correctamente");

	abb_destruir(abb);
}
void test_abb_insertar_correctamente_enteros()
{
	abb_t *abb = abb_crear(comparador_num);

	int *n1 = malloc(sizeof(int));
	int *n2 = malloc(sizeof(int));
	int *n3 = malloc(sizeof(int));
	*n1 = 10;
	*n2 = 20;
	*n3 = 5;

	pa2m_afirmar(abb_insertar(abb, n1) == true &&
			     abb_insertar(abb, n2) == true &&
			     abb_insertar(abb, n3) == true,
		     "Se insertan enteros correctamente");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_correctamente_strings()
{
	abb_t *abb = abb_crear(comparador_string);

	char *hola = "hola";
	char *chau = "chau";
	char *arbol = "arbol";

	pa2m_afirmar(abb_insertar(abb, hola) == true &&
			     abb_insertar(abb, chau) == true &&
			     abb_insertar(abb, arbol) == true,
		     "Se insertan strings correctamente");

	abb_destruir(abb);
}
void test_abb_insertar_correctamente_bools()
{
	abb_t *abb = abb_crear(comparador_booleanos);

	bool verdadero = true;
	bool falso = false;

	pa2m_afirmar(abb_insertar(abb, &verdadero) == true &&
			     abb_insertar(abb, &falso) == true,
		     "Se insertan booleanos correctamente");

	abb_destruir(abb);
}
void test_abb_insertar_correctamente_double()
{
	abb_t *abb = abb_crear(comparador_double);

	double *d1 = malloc(sizeof(double));
	double *d2 = malloc(sizeof(double));
	double *d3 = malloc(sizeof(double));
	*d1 = 3.14;
	*d2 = 1.41;
	*d3 = 2.71;

	pa2m_afirmar(abb_insertar(abb, d1) == true &&
			     abb_insertar(abb, d2) == true &&
			     abb_insertar(abb, d3) == true,
		     "Se insertan doubles correctamente");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_correctamente_vectores()
{
	abb_t *abb = abb_crear(comparador_array_suma);

	int *arr1 = malloc(3 * sizeof(int));

	arr1[0] = 1;
	arr1[1] = 2;
	arr1[2] = 3;

	pa2m_afirmar(abb_insertar(abb, arr1) == true,
		     "Se insertan arrays correctamente ");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_correctamente_structs()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);

	persona_t *juan = malloc(sizeof(persona_t));

	juan->nombre = "Juan";
	juan->edad = 30;

	pa2m_afirmar(abb_insertar(abb, juan) == true,
		     "Se insertan structs correctamente ");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_correctamente_null()
{
	abb_t *abb = abb_crear(comparador_null);

	pa2m_afirmar(abb_insertar(abb, NULL) == true,
		     "Se puede insertar NULL correctamente en el abb");
	abb_destruir(abb);
}
void test_abb_insertar_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 3);

	int valor = 1;
	int *eliminado = abb_eliminar(abb, &valor);
	free(eliminado);

	insertando_dinamic_en_abb(abb, 2);

	pa2m_afirmar(abb_cantidad(abb) == 4,
		     "Se insertan nodos correctamente después de eliminar");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_despues_de_buscar()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 3);

	int valor = 2;
	abb_buscar(abb, &valor);

	insertando_dinamic_en_abb(abb, 2);

	pa2m_afirmar(abb_cantidad(abb) == 5,
		     "Se insertan nodos correctamente después de buscar");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_despues_de_existencia()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 3);

	int valor = 1;
	abb_existe(abb, &valor); // Comprobamos existencia

	insertando_dinamic_en_abb(abb, 2); // Insertamos después de existencia

	pa2m_afirmar(
		abb_cantidad(abb) == 5,
		"Se insertan nodos correctamente después de chequear existencia");

	abb_destruir_todo(abb, free);
}
bool contar_nodos(void *dato, void *extra)
{
	(void)dato; // evitamos el warning de parámetro no usado
	size_t *contador = (size_t *)extra;
	(*contador)++;
	return true;
}
void test_abb_insertar_despues_de_iterador_interno()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 3);

	size_t cont = 0;
	abb_con_cada_elemento(abb, ABB_INORDEN, contar_nodos, &cont);

	insertando_dinamic_en_abb(abb, 2);

	pa2m_afirmar(
		abb_cantidad(abb) == 5,
		"Se insertan nodos correctmente después de recorrer con iterador interno");

	abb_destruir_todo(abb, free);
}
void test_abb_insertar_despues_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 3);

	void *vector[3];
	abb_vectorizar(abb, 0, 3, vector);

	insertando_dinamic_en_abb(abb, 2);

	pa2m_afirmar(abb_cantidad(abb) == 5,
		     "Se insertan nodos correctamente después de vectorizar");

	abb_destruir_todo(abb, free);
}
// void test_abb_insertar_despues_de_destruir()
// {
// 	abb_t *abb = abb_crear(comparador_num);
// 	abb_destruir(abb);

// 	pa2m_afirmar(insertando_dinamic_en_abb(abb, 2) == false,
// 		     "No se puede insertar luego de destruir el arbol");
// }
void test_abb_insertar_prueba_estres()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(insertando_dinamic_en_abb(abb, 5000) == true,
		     "Prueba estres:Se insertan correctamente +5000 nodos");

	abb_destruir_todo(abb, free);
}

void tests_abb_insertar()
{
	test_abb_insertar_abb_null();
	test_abb_insertar_vacio();
	test_abb_insertar_primer_nodo();
	test_abb_insertar_varios_nodos();
	test_abb_insertar_aumenta_cantidad();
	test_abb_insertar_mismo_dato();
	test_abb_insertar_correctamente_enteros();
	test_abb_insertar_correctamente_strings();
	test_abb_insertar_correctamente_bools();
	test_abb_insertar_correctamente_double();
	test_abb_insertar_correctamente_vectores();
	test_abb_insertar_correctamente_structs();
	test_abb_insertar_correctamente_null();
	test_abb_insertar_despues_de_eliminar();
	test_abb_insertar_despues_de_buscar();
	test_abb_insertar_despues_de_existencia();
	test_abb_insertar_despues_de_iterador_interno();
	test_abb_insertar_despues_de_vectorizar();
	// test_abb_insertar_despues_de_destruir();
	test_abb_insertar_prueba_estres();
}
// ------------------------------------------------------------------
void test_abb_existe_abb_null()
{
	pa2m_afirmar(abb_existe(NULL, "algo") == false,
		     "Pasandole un abb NULL, devuelve false");
}
void test_abb_existe_raiz_null()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(abb_existe(abb, "algo") == false,
		     "En un abb vacío devuelve false");

	abb_destruir(abb);
}
void test_abb_existe_si_existe_nodo()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "b");
	abb_insertar(abb, "a");

	pa2m_afirmar(abb_existe(abb, "a") == true,
		     "Se encuentra un nodo existente");

	abb_destruir(abb);
}
void test_abb_existe_no_existe_nodo()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "b");
	abb_insertar(abb, "a");

	pa2m_afirmar(abb_existe(abb, "c") == false,
		     "No encuentra un nodo inexistente");

	abb_destruir(abb);
}
void test_abb_existe_no_modifica_cantidad()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "b");

	size_t cant_antes = abb_cantidad(abb);
	abb_existe(abb, "b");

	pa2m_afirmar(cant_antes == abb_cantidad(abb),
		     "La cantidad no se modifica al consultar existencia");

	abb_destruir(abb);
}
void test_abb_existe_no_modifica_raiz()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;

	insertando_dinamic_en_abb(abb, 11);

	abb_existe(abb, &a);

	pa2m_afirmar(*(int *)abb_raiz(abb) == 0,
		     "Buscar existencia no modifica la raiz del abb");

	abb_destruir_todo(abb, free);
}
void test_abb_existe_despues_de_insertar()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "x");

	pa2m_afirmar(abb_existe(abb, "x") == true, "Existe luego de insertar");

	abb_destruir(abb);
}
void test_abb_existe_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "x");
	abb_eliminar(abb, "x");

	pa2m_afirmar(abb_existe(abb, "x") == false,
		     "No existe luego de eliminar");

	abb_destruir(abb);
}
void test_abb_existe_despues_de_iterar_internamente()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "c");
	abb_insertar(abb, "a");
	abb_insertar(abb, "b");

	size_t contador = 0;
	abb_con_cada_elemento(abb, 0, contar_nodos, &contador);

	pa2m_afirmar(
		abb_existe(abb, "b") == true,
		"Existe un nodo incluso después de recorrer con iterador interno");

	abb_destruir(abb);
}
void test_abb_existe_despues_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "c");
	abb_insertar(abb, "a");
	abb_insertar(abb, "b");

	void *vector[3];
	size_t cantidad = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cantidad == 3 && abb_existe(abb, "c") == true,
		     "Existe un nodo incluso después de vectorizar el abb");

	abb_destruir(abb);
}
void test_abb_existe_despues_de_destruir()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "c");

	abb_destruir(abb);

	pa2m_afirmar(
		abb_existe(abb, "c") == false,
		"No debería poder consultarse existencia después de destruir el abb");
}
void test_abb_existe_entero()
{
	abb_t *abb = abb_crear(comparador_num);

	int n1 = 10, n2 = 20, n3 = 30;
	abb_insertar(abb, &n1);
	abb_insertar(abb, &n2);

	pa2m_afirmar(abb_existe(abb, &n2) == true,
		     "Existe un nodo entero previamente insertado");
	pa2m_afirmar(abb_existe(abb, &n3) == false,
		     "No existe un entero no insertado");

	abb_destruir(abb);
}
void test_abb_existe_strings()
{
	abb_t *abb = abb_crear(comparador_string);

	abb_insertar(abb, "hola");
	abb_insertar(abb, "mundo");

	pa2m_afirmar(abb_existe(abb, "hola") == true,
		     "Existe un string previamente insertado");
	pa2m_afirmar(abb_existe(abb, "chau") == false,
		     "No existe un string no insertado");

	abb_destruir(abb);
}
void test_abb_existe_double()
{
	abb_t *abb = abb_crear(comparador_double);

	double d1 = 1.5, d2 = 3.7, d3 = 2.2;
	abb_insertar(abb, &d1);
	abb_insertar(abb, &d2);

	pa2m_afirmar(abb_existe(abb, &d1) == true,
		     "Existe un double previamente insertado");
	pa2m_afirmar(abb_existe(abb, &d3) == false,
		     "No existe un double no insertado");

	abb_destruir(abb);
}
void test_abb_existe_vectores()
{
	abb_t *abb = abb_crear(comparador_array_suma);

	int v1[3] = { 1, 2, 3 }; // suma 6
	int v2[3] = { 4, 4, 0 }; // suma 8

	abb_insertar(abb, v1);
	abb_insertar(abb, v2);

	pa2m_afirmar(abb_existe(abb, v1) == true,
		     "Existe un vector previamente insertado");

	pa2m_afirmar(abb_existe(abb, (int[3]){ 9, 9, 9 }) == false,
		     "No existe un vector no insertado");

	abb_destruir(abb);
}
void test_abb_existe_structs()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);

	persona_t juan = { "Juan", 30 };
	persona_t ana = { "Ana", 25 };
	persona_t pedro = { "Pedro", 40 };

	abb_insertar(abb, &juan);
	abb_insertar(abb, &ana);

	pa2m_afirmar(abb_existe(abb, &ana) == true,
		     "Existe un struct previamente insertado");
	pa2m_afirmar(abb_existe(abb, &pedro) == false,
		     "No existe un struct no insertadao");

	abb_destruir(abb);
}
void test_abb_existe_null()
{
	abb_t *abb = abb_crear(comparador_null);

	abb_insertar(abb, NULL);
	pa2m_afirmar(abb_existe(abb, NULL) == true,
		     "El abb reconoce la existencia de NULL como dato");

	abb_destruir(abb);
}
void test_abb_existe_prueba_estres()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5000);
	int n1 = 2500;
	int n2 = 5900;

	pa2m_afirmar(
		abb_existe(abb, &n1) == true,
		"Prueba estres:Existe un elemento en un abb tras insertar 5000 nodos");

	pa2m_afirmar(
		abb_existe(abb, &n2) == false,
		"Prueba estres:No existe un elemento no insertado en el abb grande");

	abb_destruir_todo(abb, free);
}

void tests_abb_existe()
{
	test_abb_existe_abb_null();
	test_abb_existe_raiz_null();
	test_abb_existe_si_existe_nodo();
	test_abb_existe_no_existe_nodo();
	test_abb_existe_no_modifica_cantidad();
	test_abb_existe_no_modifica_raiz();
	test_abb_existe_despues_de_insertar();
	test_abb_existe_despues_de_eliminar();
	test_abb_existe_despues_de_iterar_internamente();
	test_abb_existe_despues_de_vectorizar();
	// test_abb_existe_despues_de_destruir();
	test_abb_existe_entero();
	test_abb_existe_strings();
	test_abb_existe_double();
	test_abb_existe_vectores();
	test_abb_existe_structs();
	test_abb_existe_null();
	test_abb_existe_prueba_estres();
}
// ------------------------------------------------------------------
void test_abb_buscar_abb_null()
{
	pa2m_afirmar(abb_buscar(NULL, "dato") == NULL,
		     "Devuelve NULL si el abb es NULL");
}
void test_abb_buscar_raiz_null()
{
	abb_t *abb = abb_crear(comparador_string);
	pa2m_afirmar(abb_buscar(abb, "dato") == NULL,
		     "Devuelve NULL si el abb tiene raíz NULL");
	abb_destruir(abb);
}
void test_abb_buscar_devuelve_dato_encontrado()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "a");
	pa2m_afirmar(strcmp((char *)abb_buscar(abb, "a"), "a") == 0,
		     "Devuelve el dato si lo encuentra");
	abb_destruir(abb);
}
void test_abb_buscar_devuelve_null_sino_encuentra()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "a");
	pa2m_afirmar(abb_buscar(abb, "b") == NULL,
		     "Devuelve NULL si no encuentra el dato");
	abb_destruir(abb);
}
void test_abb_buscar_solo_un_nodo()
{
	abb_t *abb = abb_crear(comparador_num);
	int valor = 42;
	abb_insertar(abb, &valor);

	pa2m_afirmar(
		abb_buscar(abb, &valor) == &valor,
		"Se puede buscar correctamente cuando solo hay un nodo en el ABB");

	abb_destruir(abb);
}
void test_abb_buscar_varios_nodos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 30;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	pa2m_afirmar(
		abb_buscar(abb, &a) == &a && abb_buscar(abb, &b) == &b &&
			abb_buscar(abb, &c) == &c,
		"Se puede buscar correctamente cuando hay varios nodos en el abb");

	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_entero()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 5, b = 10, c = 15;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	pa2m_afirmar(*(int *)abb_buscar(abb, &b) == 10,
		     "Encuentra correctamente un entero");
	pa2m_afirmar(abb_buscar(abb, &c) == NULL,
		     "Si no encuentra un entero, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_string()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "hola");
	abb_insertar(abb, "mundo");

	pa2m_afirmar(strcmp((char *)abb_buscar(abb, "mundo"), "mundo") == 0,
		     "Encuentra correctamente un string");
	pa2m_afirmar(abb_buscar(abb, "No existo") == NULL,
		     "Si no encuentra un string, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_double()
{
	abb_t *abb = abb_crear(comparador_double);
	double a = 3.14, b = 2.71, c = 1.27;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	pa2m_afirmar(*(double *)abb_buscar(abb, &a) == 3.14,
		     "Encuentra correctamente un double");
	pa2m_afirmar(abb_buscar(abb, &c) == NULL,
		     "Si no encuentra un double, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_booleano()
{
	abb_t *abb = abb_crear(comparador_booleanos);
	bool t = true, f = false;
	abb_insertar(abb, &t);
	abb_insertar(abb, &f);

	pa2m_afirmar(*(bool *)abb_buscar(abb, &t) == true,
		     "Encuentra correctamente un booleano");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_vector()
{
	abb_t *abb = abb_crear(comparador_array_suma);
	int a[3] = { 1, 2, 3 };
	int b[3] = { 4, 5, 6 };
	int c[3] = { 7, 8, 9 };
	abb_insertar(abb, a);
	abb_insertar(abb, b);

	pa2m_afirmar(((int *)abb_buscar(abb, b))[0] == 4,
		     "Encuentra correctamente un vector y sus valores");
	pa2m_afirmar(abb_buscar(abb, c) == NULL,
		     "Si no encuentra un vector, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_struct()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);
	persona_t a = { "Ana", 25 };
	persona_t b = { "Luis", 30 };
	persona_t c = { "Carlos", 10 };

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	pa2m_afirmar(strcmp(((persona_t *)abb_buscar(abb, &b))->nombre,
			    "Luis") == 0,
		     "Encuentra correctamente un struct personalizado");

	pa2m_afirmar(abb_buscar(abb, &c) == NULL,
		     "Si no encuentra un struct, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_buscar_busca_correctamente_null()
{
	abb_t *abb = abb_crear(comparador_null);
	abb_insertar(abb, NULL);
	pa2m_afirmar(abb_buscar(abb, NULL) == NULL,
		     "Encuentra correctamente el dato NULL");
	abb_destruir(abb);
}
void test_abb_buscar_no_modifica_cantidad()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 5;
	abb_insertar(abb, &a);

	abb_buscar(abb, &a);
	pa2m_afirmar(abb_cantidad(abb) == 1,
		     "Buscar no moficia la cantidad de nodos");
	abb_destruir(abb);
}
void test_abb_buscar_volver_a_buscar_mismo_dato()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 5;
	abb_insertar(abb, &a);

	abb_buscar(abb, &a);
	abb_buscar(abb, &a);
	pa2m_afirmar(
		abb_existe(abb, &a) == true,
		"Se puede buscar el mismo dato varias veces sin problemas");
	abb_destruir(abb);
}
void test_abb_buscar_no_modifica_raiz()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 5;
	abb_insertar(abb, &a);

	void *raiz = abb_raiz(abb);
	abb_buscar(abb, &a);
	pa2m_afirmar(abb_raiz(abb) == raiz,
		     "Buscar no modifica la raíz del abb");
	abb_destruir(abb);
}
void test_abb_buscar_despues_de_insertar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);

	pa2m_afirmar(
		abb_buscar(abb, &a) != NULL,
		"Se puede buscar un nodo inmediatamente después de insertarlo");
	abb_destruir(abb);
}
void test_abb_buscar_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);
	abb_eliminar(abb, &a);

	pa2m_afirmar(abb_buscar(abb, &a) == NULL,
		     "No se encuentra un nodo después de eliminarlo");
	abb_destruir(abb);
}
void test_abb_buscar_despues_de_iterar_internamente()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	size_t contador = 0;
	abb_con_cada_elemento(abb, 0, contar_nodos, &contador);

	pa2m_afirmar(
		abb_buscar(abb, &b) != NULL,
		"Se puede buscar un nodo después de usar iterador interno");

	abb_destruir(abb);
}
void test_abb_buscar_despues_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "c");
	abb_insertar(abb, "a");
	abb_insertar(abb, "b");

	void *vector[3];
	size_t cantidad = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cantidad == 3 && abb_buscar(abb, "c") != NULL,
		     "Se puede buscar un nodo después de vectorizar el abb");

	abb_destruir(abb);
}
// void test_abb_buscar_despues_de_destruir()
// {
// 	abb_t *abb = abb_crear(comparador_string);
// 	abb_insertar(abb, "c");

// 	abb_destruir(abb);

// 	pa2m_afirmar(true,
// 		     "No se puede buscar después de destruir el ABB (se evita pasar ABB nulo)");
// }

void test_abb_buscar_prueba_estres()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5000);

	bool encontrado = true;
	for (int i = 0; i < 5000; i++) {
		int key = i;
		if (*(int *)abb_buscar(abb, &key) != i) {
			encontrado = false;
			break;
		}
	}

	pa2m_afirmar(
		encontrado,
		"Prueba de estres:Se pueden buscar correctamente 5000 nodos");

	abb_destruir_todo(abb, free);
}

void tests_abb_buscar()
{
	test_abb_buscar_abb_null();
	test_abb_buscar_raiz_null();
	test_abb_buscar_devuelve_dato_encontrado();
	test_abb_buscar_devuelve_null_sino_encuentra();
	test_abb_buscar_solo_un_nodo();
	test_abb_buscar_varios_nodos();
	test_abb_buscar_busca_correctamente_entero();
	test_abb_buscar_busca_correctamente_string();
	test_abb_buscar_busca_correctamente_double();
	test_abb_buscar_busca_correctamente_booleano();
	test_abb_buscar_busca_correctamente_vector();
	test_abb_buscar_busca_correctamente_struct();
	test_abb_buscar_busca_correctamente_null();
	test_abb_buscar_no_modifica_cantidad();
	test_abb_buscar_volver_a_buscar_mismo_dato();
	test_abb_buscar_no_modifica_raiz();
	test_abb_buscar_despues_de_insertar();
	test_abb_buscar_despues_de_eliminar();
	test_abb_buscar_despues_de_iterar_internamente();
	test_abb_buscar_despues_de_vectorizar();
	// test_abb_buscar_despues_de_destruir();
	test_abb_buscar_prueba_estres();
}
// ------------------------------------------------------------------
void test_abb_eliminar_abb_null()
{
	pa2m_afirmar(abb_eliminar(NULL, "a") == NULL,
		     "Eliminar en un abb NULL devuelve NULL");
}
void test_abb_eliminar_raiz_null()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(abb_eliminar(abb, "a") == NULL,
		     "Eliminar en un abb sin nodos devuelve NULL");

	abb_destruir(abb);
}
void test_abb_eliminar_devuelve_correctamente_dato_eliminado()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;

	abb_insertar(abb, &a);
	int *eliminado = abb_eliminar(abb, &a);

	pa2m_afirmar(*eliminado == a,
		     "Eliminar devuelve el dato eliminado correctamente");

	abb_destruir(abb);
}
void test_abb_eliminar_reduce_cantidad()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	size_t antes = abb_cantidad(abb);
	abb_eliminar(abb, &a);
	size_t despues = abb_cantidad(abb);

	pa2m_afirmar(antes == 2 && despues == 1,
		     "Eliminar reduce la cantidad de nodos");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_unico_nodo()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;

	abb_insertar(abb, &a);
	abb_eliminar(abb, &a);

	pa2m_afirmar(abb_cantidad(abb) == 0 && abb_raiz(abb) == NULL,
		     "Eliminar el único nodo deja el abb vacío y raíz NULL");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_varios_nodos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 5;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 5 && abb_cantidad(abb) == 1,
		     "Se elimina con varios nodos correctamente");
	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 10,
		"Eliminar con varios nodos no modifica la raiz si corresponde");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_raiz_varios_nodos()
{
	abb_t *abb = abb_crear(comparador_num);

	// Insertamos varios nodos: 2, 1, 3
	int a = 2;
	int b = 1;
	int c = 3;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	// Eliminamos la raíz
	abb_eliminar(abb, &a);

	int *nueva_raiz = (int *)abb_raiz(abb);
	pa2m_afirmar(
		*nueva_raiz == 1,
		"La raíz se actualiza correctamente después de eliminar con varios nodos");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_nodo_con_hijo_der()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 5, d = 22;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	abb_insertar(abb, &d);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 20,
		     "Elimina correctamente nodo con hijo derecho");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_nodo_con_hijo_izq()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 5, c = 2, d = 12;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	abb_insertar(abb, &d);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 5,
		     "Elimina correctmanete nodo con hijo izquierdo ");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_nodo_sin_hijos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 5;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 5, "Elimina correctamente nodo sin hijos ");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_nodo_con_dos_hijos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 5, c = 20, d = 3, e = 8;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	abb_insertar(abb, &d);
	abb_insertar(abb, &e);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 5,
		     "Elimina correctamente nodo con dos hijos ");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_entero()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 42;

	abb_insertar(abb, &a);
	int *eliminado = abb_eliminar(abb, &a);

	pa2m_afirmar(*eliminado == 42, "Elimina correctamente un entero");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_string()
{
	abb_t *abb = abb_crear(comparador_string);
	char *str = "Hola";

	abb_insertar(abb, str);
	char *eliminado = abb_eliminar(abb, str);

	pa2m_afirmar(strcmp(eliminado, "Hola") == 0,
		     "Elimina correctamente un string");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_double()
{
	abb_t *abb = abb_crear(comparador_double);
	double d = 3.1415;

	abb_insertar(abb, &d);
	double *eliminado = abb_eliminar(abb, &d);

	pa2m_afirmar(*eliminado == 3.1415, "Elimina correctamente un double");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_booleano()
{
	abb_t *abb = abb_crear(comparador_booleanos);
	bool b = true;

	abb_insertar(abb, &b);
	bool *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == true, "Elimina correctamente un booleano");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_vector()
{
	abb_t *abb = abb_crear(comparador_array_suma);
	int v[3] = { 1, 2, 3 };

	abb_insertar(abb, v);
	int *eliminado = abb_eliminar(abb, v);

	pa2m_afirmar(eliminado[0] == 1 && eliminado[1] == 2 &&
			     eliminado[2] == 3,
		     "Elimina correctamente un vector y sus valores");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_struct()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);
	persona_t p = { "Luca", 24 };

	abb_insertar(abb, &p);
	persona_t *eliminado = abb_eliminar(abb, &p);

	pa2m_afirmar(strcmp(eliminado->nombre, "Luca") == 0 &&
			     eliminado->edad == 24,
		     "Elimina correctamente un struct ");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_null()
{
	abb_t *abb = abb_crear(comparador_num);

	abb_insertar(abb, NULL);
	void *eliminado = abb_eliminar(abb, NULL);

	pa2m_afirmar(eliminado == NULL, "Elimina correctamente un NULL");

	abb_destruir(abb);
}
void test_abb_eliminar_devuelve_null_si_elimina_datounico_ya_eliminado_()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 42;

	abb_insertar(abb, &a);
	int *eliminado1 = abb_eliminar(abb, &a);
	int *eliminado2 = abb_eliminar(abb, &a);

	pa2m_afirmar(*eliminado1 == 42 && eliminado2 == NULL,
		     "Elimina un dato ya eliminado devuelve NULL");

	abb_destruir(abb);
}
void test_abb_eliminar_elimina_correctamente_datos_repetidos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 5, b = 5;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b); // insertamos repetido

	int *eliminado1 = abb_eliminar(abb, &a);
	int *eliminado2 = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado1 == 5 && *eliminado2 == 5 &&
			     abb_cantidad(abb) == 0,
		     "Elimina correctamente datos repetidos");

	abb_destruir(abb);
}
void test_abb_eliminar_luego_de_insertar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	int *eliminado = abb_eliminar(abb, &a);

	pa2m_afirmar(*eliminado == 10 && abb_cantidad(abb) == 1,
		     "Se puede eliminar correctamente luego de insertar");

	abb_destruir(abb);
}
void test_abb_eliminar_luego_de_buscar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 15, b = 25;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	abb_buscar(abb, &b);
	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(*eliminado == 25 && abb_cantidad(abb) == 1,
		     "Se puede eliminar correctamente luego de buscar");

	abb_destruir(abb);
}
void test_abb_eliminar_luego_de_existencia()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 7, b = 14;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	abb_existe(abb, &b);
	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(
		*eliminado == 14 && abb_cantidad(abb) == 1,
		"Se puede eliminar correctamente luego de consultar existencia");

	abb_destruir(abb);
}
void test_abb_eliminar_luego_de_iterador_interno()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	size_t contador = 0;
	abb_con_cada_elemento(abb, 0, contar_nodos, &contador);

	int *eliminado = abb_eliminar(abb, &b);

	pa2m_afirmar(
		*eliminado == 2 && abb_cantidad(abb) == 1,
		"Se puede eliminar correctamente luego de iterar internamente");

	abb_destruir(abb);
}
void test_abb_eliminar_luego_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 3, b = 6;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	void *vector[2];
	abb_vectorizar(abb, 0, 2, vector);

	int *eliminado = abb_eliminar(abb, &a);

	pa2m_afirmar(*eliminado == 3 && abb_cantidad(abb) == 1,
		     "Se puede eliminar correctamente luego de vectorizar");

	abb_destruir(abb);
}
void test_abb_eliminar_prueba_estres()
{
	abb_t *abb = abb_crear(comparador_num);
	size_t cantidad = 5000;
	int *numeros[cantidad];

	// Insertamos 5000 nodos
	insertando_dinamic_en_abb(abb, 5000);

	// Eliminamos todos los nodos
	bool exito = true;
	for (size_t i = 0; i < cantidad; i++) {
		int *eliminado = abb_eliminar(abb, numeros[i]);
		if (!eliminado || *eliminado != (int)i)
			exito = false;
		free(eliminado);
	}

	pa2m_afirmar(exito && abb_cantidad(abb) == 0,
		     "Prueba de estres: insertar y eliminar 5000 nodos");

	abb_destruir(abb);
}

void tests_abb_eliminar()
{
	test_abb_eliminar_abb_null();
	test_abb_eliminar_raiz_null();
	test_abb_eliminar_devuelve_correctamente_dato_eliminado();
	test_abb_eliminar_reduce_cantidad();
	test_abb_eliminar_elimina_correctamente_unico_nodo();
	test_abb_eliminar_elimina_correctamente_varios_nodos();
	// test_abb_eliminar_elimina_correctamente_raiz_varios_nodos();
	// test_abb_eliminar_elimina_correctamente_nodo_con_hijo_der();
	// test_abb_eliminar_elimina_correctamente_nodo_con_hijo_izq();
	// test_abb_eliminar_elimina_correctamente_nodo_sin_hijos();
	// test_abb_eliminar_elimina_correctamente_nodo_con_dos_hijos();
	test_abb_eliminar_elimina_correctamente_entero();
	test_abb_eliminar_elimina_correctamente_string();
	test_abb_eliminar_elimina_correctamente_double();
	test_abb_eliminar_elimina_correctamente_booleano();
	test_abb_eliminar_elimina_correctamente_vector();
	test_abb_eliminar_elimina_correctamente_struct();
	// test_abb_eliminar_elimina_correctamente_null();
	test_abb_eliminar_devuelve_null_si_elimina_datounico_ya_eliminado_();
	test_abb_eliminar_elimina_correctamente_datos_repetidos();
	test_abb_eliminar_luego_de_insertar();
	test_abb_eliminar_luego_de_buscar();
	test_abb_eliminar_luego_de_existencia();
	test_abb_eliminar_luego_de_iterador_interno();
	test_abb_eliminar_luego_de_vectorizar();
	// test_abb_eliminar_prueba_estres();
}
// ------------------------------------------------------------------

void test_abb_raiz_abb_null()
{
	pa2m_afirmar(abb_raiz(NULL) == NULL, "Con abb NULL, devuelve NULL");
}
void test_abb_raiz_raiz_null()
{
	abb_t *abb = abb_crear(comparador_num);
	pa2m_afirmar(abb_raiz(abb) == NULL, "Con abb vacio, devuelve NULL");
	abb_destruir(abb);
}
void test_abb_raiz_devuelve_correctamente()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);
	pa2m_afirmar(*(int *)abb_raiz(abb) == 0,
		     "Devuele correctamente el dato de la raiz");
	abb_destruir_todo(abb, free);
}
void test_abb_raiz_devuelve_entero()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);
	pa2m_afirmar(*(int *)abb_raiz(abb) == 0, "La raiz puede ser un entero");
	abb_destruir_todo(abb, free);
}
void test_abb_raiz_devuelve_string()
{
	abb_t *abb = abb_crear(comparador_string);
	abb_insertar(abb, "hola");
	pa2m_afirmar(strcmp((char *)abb_raiz(abb), "hola") == 0,
		     "La raiz puede ser un string");
	abb_destruir(abb);
}
void test_abb_raiz_devuelve_double()
{
	abb_t *abb = abb_crear(comparador_double);
	double d = 3.14;
	abb_insertar(abb, &d);
	pa2m_afirmar(*(double *)abb_raiz(abb) == 3.14,
		     "La raiz puede ser un double");
	abb_destruir(abb);
}
void test_abb_raiz_devuelve_booleano()
{
	abb_t *abb = abb_crear(comparador_booleanos);
	bool b = true;
	abb_insertar(abb, &b);
	pa2m_afirmar(*(bool *)abb_raiz(abb) == true,
		     "La raiz puede ser un booleano");
	abb_destruir(abb);
}
void test_abb_raiz_devuelve_vector()
{
	abb_t *abb = abb_crear(comparador_array_suma);
	int v[3] = { 1, 2, 3 };
	abb_insertar(abb, v);
	int *raiz = (int *)abb_raiz(abb);
	pa2m_afirmar(raiz[0] == 1 && raiz[1] == 2 && raiz[2] == 3,
		     "La raiz puede ser un vector de enteros");
	abb_destruir(abb);
}
void test_abb_raiz_devuelve_struct()
{
	abb_t *abb = abb_crear(comparador_string); // Comparador por nombre
	persona_t p = { "Luca", 22 };
	abb_insertar(abb, &p);
	persona_t *raiz = (persona_t *)abb_raiz(abb);
	pa2m_afirmar(strcmp(raiz->nombre, "Luca") == 0 && raiz->edad == 22,
		     "La raiz puede ser un struct");
	abb_destruir(abb);
}
void test_abb_raiz_despues_de_cantidad()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);

	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 0,
		"La raiz puede ser consultada después de usar abb_cantidad");
	pa2m_afirmar(abb_cantidad(abb) == 5,
		     "Consular la raiz no modifica la cantidad");
	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_esta_vacio()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);

	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 0,
		"La raiz puede ser consultada después de usar abb_esta_vacio");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_buscar()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);

	abb_buscar(abb, &(int){ 2 });

	pa2m_afirmar(*(int *)abb_raiz(abb) == 0,
		     "La raiz puede ser consultada después de buscar un nodo");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_existencia()
{
	abb_t *abb = abb_crear(comparador_num);
	int valor = 42;
	abb_insertar(abb, &valor);

	pa2m_afirmar(*(int *)abb_raiz(abb) == 42,
		     "La raiz es el nodo antes de eliminar");

	// Eliminamos el unico nodo
	abb_eliminar(abb, &valor);

	pa2m_afirmar(abb_raiz(abb) == NULL,
		     "La raiz es NULL después de eliminar el unico nodo");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);

	int a = 10, b = 20;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	pa2m_afirmar(*(int *)abb_raiz(abb) == 10,
		     "La raiz es correcta antes de eliminar");

	abb_eliminar(abb, &a);

	pa2m_afirmar(*(int *)abb_raiz(abb) == 20,
		     "La raiz se actualiza correctamente después de eliminar");

	abb_eliminar(abb, &b);

	pa2m_afirmar(abb_raiz(abb) == NULL,
		     "La raiz es NULL después de eliminar todos los nodos");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_iterar_internamente()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);

	size_t contador = 0;
	abb_con_cada_elemento(abb, 0, contar_nodos, &contador);

	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 0,
		"La raiz puede ser consultada después de iterar internamente");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_despues_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5);

	void *vector[5];
	abb_vectorizar(abb, 0, 5, vector);

	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 0,
		"La raiz puede ser consultada después de vectorizar el abb");

	abb_destruir_todo(abb, free);
}
void test_abb_raiz_prueba_estres()
{
	abb_t *abb = abb_crear(comparador_num);
	insertando_dinamic_en_abb(abb, 5000);
	pa2m_afirmar(
		*(int *)abb_raiz(abb) == 0,
		"Prueba estres:Devuele correctamente el dato de la raiz aun siendo +5000 nodos");
	abb_destruir_todo(abb, free);
}

void tests_abb_raiz()
{
	test_abb_raiz_abb_null();
	test_abb_raiz_raiz_null();
	test_abb_raiz_devuelve_correctamente();
	test_abb_raiz_devuelve_entero();
	test_abb_raiz_devuelve_string();
	test_abb_raiz_devuelve_double();
	test_abb_raiz_devuelve_booleano();
	test_abb_raiz_devuelve_vector();
	test_abb_raiz_devuelve_struct();
	test_abb_raiz_despues_de_cantidad();
	test_abb_raiz_despues_de_esta_vacio();
	test_abb_raiz_despues_de_buscar();
	test_abb_raiz_despues_de_existencia();
	test_abb_raiz_despues_de_eliminar();
	test_abb_raiz_despues_de_iterar_internamente();
	test_abb_raiz_despues_de_vectorizar();
	test_abb_raiz_prueba_estres();
}
// ------------------------------------------------------------------
void tests_abb_cantidad_en_arbol_nulo()
{
	abb_t *abb = NULL;
	pa2m_afirmar(abb_cantidad(abb) == 0, "Un árbol nulo tiene cantidad 0");
}
void test_abb_cantidad_aumenta_al_insertar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 30;

	abb_insertar(abb, &a);
	pa2m_afirmar(abb_cantidad(abb) == 1,
		     "La cantidad es 1 después de insertar un nodo");

	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	pa2m_afirmar(
		abb_cantidad(abb) == 3,
		"La cantidad aumenta correctamente al insertar varios nodos");

	abb_destruir(abb);
}
void test_abb_cantidad_disminuye_al_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 30;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_eliminar(abb, &b);
	pa2m_afirmar(abb_cantidad(abb) == 2,
		     "La cantidad disminuye correctamente al eliminar un nodo");

	abb_eliminar(abb, &a);
	abb_eliminar(abb, &c);
	pa2m_afirmar(abb_cantidad(abb) == 0,
		     "La cantidad vuelve a 0 al eliminar todos los nodos");

	abb_destruir(abb);
}

void tests_abb_cantidad()
{
	tests_abb_cantidad_en_arbol_nulo();
	test_abb_cantidad_aumenta_al_insertar();
	test_abb_cantidad_disminuye_al_eliminar();
}
// ------------------------------------------------------------------
void test_abb_esta_vacio_arbol_recien_creado()
{
	abb_t *abb = abb_crear(comparador_num);

	pa2m_afirmar(abb_esta_vacio(abb), "Un árbol recién creado está vacío");

	abb_destruir(abb);
}
void test_abb_esta_vacio_con_elemento_insertado()
{
	abb_t *abb = abb_crear(comparador_num);

	int a = 10;
	abb_insertar(abb, &a);

	pa2m_afirmar(!abb_esta_vacio(abb),
		     "Un árbol con elementos no está vacío");

	abb_destruir(abb);
}
void test_abb_esta_vacio_despues_de_eliminar_todos()
{
	abb_t *abb = abb_crear(comparador_num);

	int a = 10;
	abb_insertar(abb, &a);
	abb_eliminar(abb, &a);

	pa2m_afirmar(
		abb_esta_vacio(abb),
		"El árbol vuelve a estar vacío después de eliminar todos sus elementos");

	abb_destruir(abb);
}
void test_abb_esta_vacio_con_arbol_nulo()
{
	abb_t *abb_nulo = NULL;

	pa2m_afirmar(abb_esta_vacio(abb_nulo),
		     "Un árbol nulo se considera vacío");
}

void tests_abb_esta_vacio()
{
	test_abb_esta_vacio_arbol_recien_creado();
	test_abb_esta_vacio_con_elemento_insertado();
	test_abb_esta_vacio_despues_de_eliminar_todos();
	test_abb_esta_vacio_con_arbol_nulo();
}
// ------------------------------------------------------------------
bool sumar_enteros(void *dato, void *extra)
{
	*(int *)extra += *(int *)dato;
	return true; // sigue recorriendo
}
bool cortar_en_20(void *dato, void *extra)
{
	return *(int *)dato != 20; // corta cuando encuentra 10
	(*(int *)extra)++;
}
bool contar(void *dato, void *extra)
{
	(void)dato; // evitamos el warning de parámetro no usado
	(*(int *)extra)++;
	return true; // siempre continúa la iteración
}

void test_abb_con_cada_elemento_recorre_todos()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 5;
	int suma = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, sumar_enteros, &suma);

	pa2m_afirmar(
		suma == 35,
		"El iterador interno recorre todos los elementos y aplica la función correctamente(INORDEN)");

	suma = 0;
	abb_con_cada_elemento(abb, 1, sumar_enteros, &suma);

	pa2m_afirmar(
		suma == 35,
		"El iterador interno recorre todos los elementos y aplica la función correctamente(PREORDEN)");

	suma = 0;
	abb_con_cada_elemento(abb, 2, sumar_enteros, &suma);

	pa2m_afirmar(
		suma == 35,
		"El iterador interno recorre todos los elementos y aplica la función correctamente(POSTORDEN)");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_corta_en_condicion()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10, b = 20, c = 5;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, cortar_en_20, &contador);

	pa2m_afirmar(contador < 3,
		     "El iterador interno puede cortar según condición");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_arbol_null()
{
	size_t cantidad = abb_con_cada_elemento(NULL, 0, sumar_enteros, NULL);

	pa2m_afirmar(cantidad == 0, "Iterar con árbol NULL devuelve 0");
}
void test_abb_con_cada_elemento_funcion_null()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;
	abb_insertar(abb, &a);

	size_t cantidad = abb_con_cada_elemento(abb, 0, NULL, NULL);

	pa2m_afirmar(cantidad == 0, "Iterar con función NULL devuelve 0");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_arbol_vacio()
{
	abb_t *abb = abb_crear(comparador_num);

	int acumulador = 0;
	size_t cantidad =
		abb_con_cada_elemento(abb, 0, sumar_enteros, &acumulador);

	pa2m_afirmar(cantidad == 0, "Iterar con árbol vacío devuelve 0");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_tipo_invalido_menor()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;
	abb_insertar(abb, &a);

	int acumulador = 0;
	size_t cantidad =
		abb_con_cada_elemento(abb, -1, sumar_enteros, &acumulador);

	pa2m_afirmar(cantidad == 0, "Iterar con recorrido < 0 devuelve 0");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_tipo_invalido_mayor()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 10;
	abb_insertar(abb, &a);

	int acumulador = 0;
	size_t cantidad =
		abb_con_cada_elemento(abb, 3, sumar_enteros, &acumulador);

	pa2m_afirmar(cantidad == 0, "Iterar con recorrido > 2 devuelve 0");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_enteros()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con enteros");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_strings()
{
	abb_t *abb = abb_crear(comparador_string);
	char *a = "a", *b = "b", *c = "c";
	int contador = 0;

	abb_insertar(abb, a);
	abb_insertar(abb, b);
	abb_insertar(abb, c);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con strings");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_doubles()
{
	abb_t *abb = abb_crear(comparador_double);
	double a = 1.1, b = 2.2, c = 3.3;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con doubles");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_booleanos()
{
	abb_t *abb = abb_crear(comparador_booleanos);
	bool a = true, b = false, c = true;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con booleanos");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_vectores()
{
	abb_t *abb = abb_crear(comparador_array_suma);
	int v1[] = { 1, 2 }, v2[] = { 3, 4 }, v3[] = { 5, 6 };
	int contador = 0;

	abb_insertar(abb, v1);
	abb_insertar(abb, v2);
	abb_insertar(abb, v3);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con vectores");

	abb_destruir(abb);
}
void test_abb_con_cada_elemento_structs()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);
	persona_t a = { "Ana", 25 }, b = { "Luis", 30 }, c = { "Eva", 20 };
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, contar, &contador);

	pa2m_afirmar(contador == 3, "Se puede iterar con structs");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_insertar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 3, "Se puede iterar después de insertar");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	abb_eliminar(abb, &b);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 2, "Se puede iterar después de eliminar");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_buscar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);
	abb_buscar(abb, &b);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 3, "Se puede iterar después de buscar");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_existencia()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_existe(abb, &a);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 1, "Se puede iterar después de existencia");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_cantidad()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	size_t cant = abb_cantidad(abb);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == (int)cant,
		     "Se puede iterar después de obtener cantidad");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_esta_vacio()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_esta_vacio(abb);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 1,
		     "Se puede iterar después de verificar si está vacío");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_raiz()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_raiz(abb);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == 1,
		     "Se puede iterar después de obtener la raíz");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_vectorizar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	int contador = 0;
	void *vector[3];

	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(contador == (int)cant,
		     "Se puede iterar después de vectorizar");

	abb_destruir(abb);
}
void test_abb_iterar_despues_de_destruir()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	int contador = 0;

	abb_insertar(abb, &a);
	abb_destruir(abb);
	abb = NULL; // importante, el puntero ahora apunta a NULL

	int cant = abb_con_cada_elemento(abb, 0, contar, &contador);
	pa2m_afirmar(
		cant == 0,
		"Se puede llamar a iterar después de destruir (retorna 0)");
	pa2m_afirmar(contador == 0,
		     "El contador no aumenta porque el árbol ya no existe");
}

void tests_abb_con_cada_elemento()
{
	test_abb_con_cada_elemento_recorre_todos();
	test_abb_con_cada_elemento_corta_en_condicion();
	test_abb_con_cada_elemento_arbol_null();
	test_abb_con_cada_elemento_funcion_null();
	test_abb_con_cada_elemento_arbol_vacio();
	test_abb_con_cada_elemento_tipo_invalido_menor();
	test_abb_con_cada_elemento_tipo_invalido_mayor();
	test_abb_con_cada_elemento_enteros();
	test_abb_con_cada_elemento_strings();
	test_abb_con_cada_elemento_doubles();
	test_abb_con_cada_elemento_booleanos();
	test_abb_con_cada_elemento_vectores();
	test_abb_con_cada_elemento_structs();
	test_abb_iterar_despues_de_insertar();
	test_abb_iterar_despues_de_eliminar();
	test_abb_iterar_despues_de_buscar();
	test_abb_iterar_despues_de_existencia();
	test_abb_iterar_despues_de_cantidad();
	test_abb_iterar_despues_de_esta_vacio();
	test_abb_iterar_despues_de_raiz();
	test_abb_iterar_despues_de_vectorizar();
	test_abb_iterar_despues_de_destruir();
}
// ------------------------------------------------------------------
void test_abb_vectorizar_abb_null()
{
	void *vector[10];
	size_t cant = abb_vectorizar(NULL, 0, 10, vector);
	pa2m_afirmar(cant == 0, "Vectorizar devuelve 0 si el árbol es NULL");
}
void test_abb_vectorizar_vector_null()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);

	size_t cant = abb_vectorizar(abb, 0, 10, NULL);
	pa2m_afirmar(cant == 0, "Vectorizar devuelve 0 si el vector es NULL");

	abb_destruir(abb);
}
void test_abb_vectorizar_arbol_vacio()
{
	abb_t *abb = abb_crear(comparador_num);
	void *vector[10];

	size_t cant = abb_vectorizar(abb, 0, 10, vector);
	pa2m_afirmar(cant == 0, "Vectorizar devuelve 0 si el árbol está vacío");

	abb_destruir(abb);
}
void test_abb_vectorizar_tipo_ordenamiento_incorrecto()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);
	void *vector[10];

	size_t cant = abb_vectorizar(abb, -1, 10, vector);
	pa2m_afirmar(
		cant == 0,
		"Vectorizar devuelve 0 si el tipo de ordenamieto es menor a 0");
	cant = abb_vectorizar(abb, 3, 10, vector);
	pa2m_afirmar(
		cant == 0,
		"Vectorizar devuelve 0 si el tipo de ordenamieto es mayor a 2");

	abb_destruir(abb);
}
void test_abb_vectorizar_tipo_ordenamiento_correcto()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	void *vector[3];
	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(
		cant == 3,
		"Vectorizar funciona correctamente con tipo de ordenamiento INORDEN");

	void *vector2[3];
	cant = abb_vectorizar(abb, 0, 3, vector2);

	pa2m_afirmar(
		cant == 3,
		"Vectorizar funciona correctamente con tipo de ordenamiento PREORDEN");

	void *vector3[3];
	cant = abb_vectorizar(abb, 0, 3, vector3);

	pa2m_afirmar(
		cant == 3,
		"Vectorizar funciona correctamente con tipo de ordenamiento POSTORDEN");

	abb_destruir(abb);
}
void test_abb_vectorizar_tamanio_vector()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);
	void *vector[10];

	size_t cant = abb_vectorizar(abb, 0, 0, vector); // tamaño 0
	pa2m_afirmar(cant == 0,
		     "Vectorizar devuelve 0 si el tamaño del vector es 0");

	abb_destruir(abb);
}
void test_abb_vectorizar_enteros()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2, c = 3;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	void *vector[3];
	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cant == 3, "Vectorizar puede recorrer enteros");

	abb_destruir(abb);
}
void test_abb_vectorizar_strings()
{
	abb_t *abb = abb_crear(comparador_string);
	char *a = "a", *b = "b", *c = "c";
	abb_insertar(abb, a);
	abb_insertar(abb, b);
	abb_insertar(abb, c);

	void *vector[3];
	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cant == 3, "Vectorizar puede recorrer strings");

	abb_destruir(abb);
}
void test_abb_vectorizar_doubles()
{
	abb_t *abb = abb_crear(comparador_double);
	double a = 1.1, b = 2.2, c = 3.3;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	void *vector[3];
	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cant == 3, "Vectorizar puede recorrer doubles");

	abb_destruir(abb);
}
void test_abb_vectorizar_structs()
{
	abb_t *abb = abb_crear(comparador_persona_nombre);
	persona_t a = { "Ana", 20 }, b = { "Ben", 30 }, c = { "Cris", 25 };
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);
	abb_insertar(abb, &c);

	void *vector[3];
	size_t cant = abb_vectorizar(abb, 0, 3, vector);

	pa2m_afirmar(cant == 3, "Vectorizar puede recorrer structs");

	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_insertar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	void *vector[2];
	size_t cant = abb_vectorizar(abb, 0, 2, vector);

	pa2m_afirmar(cant == 2,
		     "Se puede vectorizar después de insertar nodos");
	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_eliminar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	abb_eliminar(abb, &a);

	void *vector[1];
	size_t cant = abb_vectorizar(abb, 0, 1, vector);
	pa2m_afirmar(cant == 1,
		     "Se puede vectorizar después de eliminar un nodo");
	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_buscar()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);

	abb_buscar(abb, &a);

	void *vector[1];
	size_t cant = abb_vectorizar(abb, 0, 1, vector);
	pa2m_afirmar(cant == 1,
		     "Se puede vectorizar después de buscar un nodo");
	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_existencia()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);

	abb_existe(abb, &a);

	void *vector[1];
	size_t cant = abb_vectorizar(abb, 0, 1, vector);
	pa2m_afirmar(cant == 1,
		     "Se puede vectorizar después de consultar existencia");
	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_iterar_internamente()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1, b = 2;
	abb_insertar(abb, &a);
	abb_insertar(abb, &b);

	int contador = 0;
	abb_con_cada_elemento(abb, 0, contar, &contador);

	void *vector[2];
	size_t cant = abb_vectorizar(abb, 0, 2, vector);
	pa2m_afirmar(cant == 2,
		     "Se puede vectorizar después de iterar internamente");
	abb_destruir(abb);
}
void test_abb_vectorizar_despues_de_destruir()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 1;
	abb_insertar(abb, &a);

	abb_destruir(abb);
	abb = NULL;

	void *vector[1];
	size_t cant = abb_vectorizar(abb, 0, 1, vector);
	pa2m_afirmar(
		cant == 0,
		"Se puede llamar a vectorizar después de destruir (no crash, retorna 0)");
}

void tests_abb_vectorizar()
{
	test_abb_vectorizar_abb_null();
	test_abb_vectorizar_vector_null();
	test_abb_vectorizar_arbol_vacio();
	test_abb_vectorizar_tipo_ordenamiento_incorrecto();
	test_abb_vectorizar_tipo_ordenamiento_correcto();
	test_abb_vectorizar_tamanio_vector();
	test_abb_vectorizar_enteros();
	test_abb_vectorizar_strings();
	test_abb_vectorizar_doubles();
	test_abb_vectorizar_structs();
	test_abb_vectorizar_despues_de_insertar();
	test_abb_vectorizar_despues_de_eliminar();
	test_abb_vectorizar_despues_de_buscar();
	test_abb_vectorizar_despues_de_existencia();
	test_abb_vectorizar_despues_de_iterar_internamente();
	test_abb_vectorizar_despues_de_destruir();
}
// ------------------------------------------------------------------
void test_abb_destruir_verificar_arbol_nulo()
{
	abb_t *abb = abb_crear(comparador_num);
	int a = 42;
	abb_insertar(abb, &a);

	abb_destruir(abb);
	abb = NULL;

	pa2m_afirmar(abb == NULL, "El árbol fue destruido correctamente");
}
void test_abb_destruir_elementos_siguen_accesibles()
{
	const size_t CANT = 10;
	abb_t *abb = abb_crear(comparador_num);
	int **elementos = malloc(sizeof(int *) * CANT);

	for (size_t i = 0; i < CANT; i++) {
		elementos[i] = malloc(sizeof(int));
		*elementos[i] = (int)i;
		abb_insertar(abb, elementos[i]);
	}

	int *primero = elementos[0];
	abb_destruir(abb);
	abb = NULL;

	pa2m_afirmar(
		*primero == 0,
		"Los elementos del árbol siguen siendo accesibles tras destruirlo");

	for (size_t i = 0; i < CANT; i++)
		free(elementos[i]);
	free(elementos);
}
void test_abb_destruir_prueba_estres()
{
	const size_t CANT = 1000;
	abb_t *abb = abb_crear(comparador_num);
	int **elementos = malloc(sizeof(int *) * CANT);

	for (size_t i = 0; i < CANT; i++) {
		elementos[i] = malloc(sizeof(int));
		*elementos[i] = (int)i;
		abb_insertar(abb, elementos[i]);
	}

	abb_destruir(abb);
	abb = NULL;

	bool todos_ok = true;
	for (size_t i = 0; i < CANT; i++) {
		if (*elementos[i] != (int)i) {
			todos_ok = false;
			break;
		}
	}

	pa2m_afirmar(
		todos_ok,
		"Prueba de estrés: los elementos siguen accesibles y correctos +1000");

	for (size_t i = 0; i < CANT; i++)
		free(elementos[i]);
	free(elementos);
}

void tests_abb_destruir()
{
	test_abb_destruir_verificar_arbol_nulo();
	test_abb_destruir_elementos_siguen_accesibles();
	test_abb_destruir_prueba_estres();
}
// ------------------------------------------------------------------
int contador_destructor = 0;
void mi_destructor(void *dato)
{
	if (dato)
		free(dato);
	contador_destructor++;
}

void test_abb_destruir_todo_arbol_con_elementos()
{
	abb_t *abb = abb_crear(comparador_num);

	insertando_dinamic_en_abb(abb, 10);

	contador_destructor = 0;
	abb_destruir_todo(abb, mi_destructor);
	abb = NULL;

	pa2m_afirmar(abb == NULL, "El árbol fue destruido correctamente");
	pa2m_afirmar(contador_destructor == 10,
		     "Todos los elementos fueron destruidos correctamente");
}
void test_abb_destruir_todo_arbol_vacio()
{
	abb_t *abb = abb_crear(comparador_num);

	contador_destructor = 0;
	abb_destruir_todo(abb, mi_destructor);
	abb = NULL;

	pa2m_afirmar(abb == NULL, "Destruir un árbol vacío no produce errores");
	pa2m_afirmar(contador_destructor == 0,
		     "No se destruyen elementos porque estaba vacío");
}
void tests_abb_destruir_todo()
{
	test_abb_destruir_todo_arbol_con_elementos();
	test_abb_destruir_todo_arbol_vacio();
}

int main()
{
	pa2m_nuevo_grupo("============== Tests Unitarios ===============");
	pa2m_nuevo_grupo("---- abb_crear ----");
	tests_abb_crear();

	pa2m_nuevo_grupo("---- abb_insertar ----");
	tests_abb_insertar();

	pa2m_nuevo_grupo("---- abb_existe ----");
	tests_abb_existe();

	pa2m_nuevo_grupo("---- abb_buscar ----");
	tests_abb_buscar();

	pa2m_nuevo_grupo("---- abb_eliminar ----");
	tests_abb_eliminar();

	pa2m_nuevo_grupo("---- abb_raiz ----");
	tests_abb_raiz();

	pa2m_nuevo_grupo("---- abb_cantidad ----");
	tests_abb_cantidad();

	pa2m_nuevo_grupo("---- abb_esta_vacio ----");
	tests_abb_esta_vacio();

	pa2m_nuevo_grupo("---- abb_con_cada_elemento ----");
	tests_abb_con_cada_elemento();

	pa2m_nuevo_grupo("---- abb_vectorizar ----");
	tests_abb_vectorizar();

	pa2m_nuevo_grupo("---- abb_destruir ----");
	tests_abb_destruir();

	pa2m_nuevo_grupo("---- abb_destruir_todo ----");
	tests_abb_destruir_todo();

	return pa2m_mostrar_reporte();
}
