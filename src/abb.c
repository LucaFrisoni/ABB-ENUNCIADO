#include "abb.h"
#include "abb_estructura_privada.h"

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
	if (!comparador)
		return NULL;

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb)
		return NULL;

	abb->comparador = comparador;

	return abb;
}
//-----------------------------------------------------------------------------------------------
bool abb_insertar_rec(abb_t *abb, nodo_t **nodo_actual, void *dato,
		      int (*comparador)(const void *, const void *))
{
	if (!*nodo_actual) {
		nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
		if (!nuevo_nodo)
			return false;

		nuevo_nodo->dato = dato;

		*nodo_actual = nuevo_nodo;
		return true;
	}
	int cmp = comparador(dato, (*nodo_actual)->dato);

	if (cmp > 0) {
		return abb_insertar_rec(abb, &(*nodo_actual)->der, dato,
					comparador);
	} else {
		return abb_insertar_rec(abb, &(*nodo_actual)->izq, dato,
					comparador);
	}
}

bool abb_insertar(abb_t *abb, void *dato)
{
	//Ver como manejar caso que se le pasa un dato que es distinto al del abb
	if (!abb)
		return false;

	if (abb_insertar_rec(abb, &abb->raiz, dato, abb->comparador)) {
		abb->cantidad++;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------------------------
nodo_t *abb_buscar_nodo(nodo_t *nodo_actual, void *dato,
			int (*comparador)(const void *, const void *))
{
	if (!nodo_actual)
		return NULL;

	int cmp = comparador(dato, nodo_actual->dato);

	if (cmp == 0)
		return nodo_actual;
	else if (cmp > 0)
		return abb_buscar_nodo(nodo_actual->der, dato, comparador);
	else
		return abb_buscar_nodo(nodo_actual->izq, dato, comparador);
}

bool abb_existe(abb_t *abb, void *dato)
{
	if (!abb || !abb->raiz)
		return false;
	return abb_buscar_nodo(abb->raiz, dato, abb->comparador) != NULL;
}
//-----------------------------------------------------------------------------------------------
void *abb_buscar(abb_t *abb, void *dato)
{
	if (!abb || !abb->raiz)
		return NULL;
	nodo_t *nodo = abb_buscar_nodo(abb->raiz, dato, abb->comparador);
	return nodo ? nodo->dato : NULL;
}
//-----------------------------------------------------------------------------------------------
nodo_t *reemplazar_por_predecesor(nodo_t *nodo)
{
	nodo_t *predecesor = nodo->izq;
	nodo_t *padre_aux = NULL;

	while (predecesor->der != NULL) {
		padre_aux = predecesor;
		predecesor = predecesor->der;
	}

	if (padre_aux){
		padre_aux->der = predecesor->izq;
		predecesor->izq = nodo->izq;
	}
		
	predecesor->der = nodo->der;

	return predecesor;
}

nodo_t *reemplazar_por_hijo(nodo_t *nodo)
{
	return nodo->izq != NULL ? nodo->izq : nodo->der;
}

void *eliminando_nodo(abb_t *abb, nodo_t **nodo_actual)
{
	void *dato_eliminado = (*nodo_actual)->dato;
	nodo_t *nuevo_nodo = NULL;

	if ((*nodo_actual)->izq && (*nodo_actual)->der) {
		//Caso dos hijos
		nuevo_nodo = reemplazar_por_predecesor(*nodo_actual);


	} else if ((*nodo_actual)->izq || (*nodo_actual)->der) {
		//Caso un hijo
		nuevo_nodo = reemplazar_por_hijo(*nodo_actual);
	}

	free(*nodo_actual);

	*nodo_actual = nuevo_nodo;
	abb->cantidad--;

	return dato_eliminado;
}

void *abb_eliminar_recu(abb_t *abb, nodo_t **nodo_actual, void *dato,
			int (*comparador)(const void *, const void *))
{
	if (!nodo_actual || !*nodo_actual)
		return NULL;

	int cmp = comparador(dato, (*nodo_actual)->dato);

	if (cmp == 0) {
		return eliminando_nodo(abb, nodo_actual);
	} else if (cmp > 0) {
		return abb_eliminar_recu(abb, &(*nodo_actual)->der, dato,
					 comparador);
	} else {
		return abb_eliminar_recu(abb, &(*nodo_actual)->izq, dato,
					 comparador);
	}
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	if (!abb || !abb->raiz)
		return NULL;

	return abb_eliminar_recu(abb, &abb->raiz, dato, abb->comparador);
}
//-----------------------------------------------------------------------------------------------
void *abb_raiz(abb_t *abb)
{
	if (!abb || !abb->raiz)
		return NULL;
	return abb->raiz->dato;
}
//-----------------------------------------------------------------------------------------------
size_t abb_cantidad(abb_t *abb)
{
	if (!abb)
		return 0;

	return abb->cantidad;
}
//-----------------------------------------------------------------------------------------------
bool abb_esta_vacio(abb_t *abb)
{
	if (!abb)
		return true;

	return abb->raiz == NULL;
}
//-----------------------------------------------------------------------------------------------
size_t abb_in_orden(nodo_t *nodo_actual, bool (*f)(void *, void *), void *extra)
{
	if (!nodo_actual)
		return 0;

	size_t cont = 0;

	cont += abb_in_orden(nodo_actual->izq, f, extra);

	if (!f(nodo_actual->dato, extra))
		return cont + 1;

	cont += 1;

	cont += abb_in_orden(nodo_actual->der, f, extra);

	return cont;
}

size_t abb_pre_orden(nodo_t *nodo_actual, bool (*f)(void *, void *),
		     void *extra)
{
	if (!nodo_actual)
		return 0;

	size_t cont = 0;

	if (!f(nodo_actual->dato, extra))
		return 1; // corto al tocar este nodo

	cont += 1;

	cont += abb_pre_orden(nodo_actual->izq, f, extra);
	cont += abb_pre_orden(nodo_actual->der, f, extra);

	return cont;
}

size_t abb_post_orden(nodo_t *nodo_actual, bool (*f)(void *, void *),
		      void *extra)
{
	if (!nodo_actual)
		return 0;

	size_t cont = 0;

	cont += abb_post_orden(nodo_actual->izq, f, extra);
	cont += abb_post_orden(nodo_actual->der, f, extra);

	if (!f(nodo_actual->dato, extra))
		return cont + 1;

	cont += 1;
	return cont;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	if (!abb || !f || !abb->raiz || tipo_recorrido < 0 ||
	    tipo_recorrido > 2)
		return 0;

	if (tipo_recorrido == 0) {
		return abb_in_orden(abb->raiz, f, extra);
	} else if (tipo_recorrido == 1) {
		return abb_pre_orden(abb->raiz, f, extra);
	} else {
		return abb_post_orden(abb->raiz, f, extra);
	}
}
//-----------------------------------------------------------------------------------------------
bool copiar_a_vector(void *dato, void *extra)
{
	//Casteamos el puntero a un array de punteros void
	void **param = (void **)extra;

	//Casteamos cada elemento del array
	void **vector = (void **)param[0];
	size_t *pos = (size_t *)param[1];
	size_t *max = (size_t *)param[2];

	if (*pos >= *max)
		return false; // detener recorrido si se llena

	vector[*pos] = dato;
	(*pos)++;
	return true;
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	if (!abb || !abb->raiz || !cant || tipo_recorrido < 0 ||
	    tipo_recorrido > 3 || !vector)
		return 0;

	size_t pos = 0;
	size_t max = cant;
	void *extra[3] = { vector, &pos, &max };

	abb_con_cada_elemento(abb, tipo_recorrido, copiar_a_vector, extra);

	return pos;
	return 0;
}
//-----------------------------------------------------------------------------------------------
void abb_destruir(abb_t *abb)
{
	abb_destruir_todo(abb, NULL);
}
//-----------------------------------------------------------------------------------------------
void abb_destruir_nodo(nodo_t *nodo, void (*destructor)(void *))
{
	if (!nodo)
		return;

	abb_destruir_nodo(nodo->izq, destructor);
	abb_destruir_nodo(nodo->der, destructor);

	if (destructor)
		destructor(nodo->dato);

	free(nodo);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;

	abb_destruir_nodo(abb->raiz, destructor);
	free(abb);
}
