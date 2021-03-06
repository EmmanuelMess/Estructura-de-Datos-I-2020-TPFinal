#include "tests.h"
#include <assert.h>
#include <malloc.h>
#include "avl/arbol_intervalos_extras.h"
#include "avl/arbol_intervalos.h"
#include "trie/trie.h"

/**
 * Son tests, algunos son para ValGrind, otros para testear funcionalidad
 */

void test_trie() {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, (Rango) {.a = 15, .b = 15});
  arbolintervalos_insertar(arbol, (Rango) {.a = -5, .b = 0});
  arbolintervalos_insertar(arbol, (Rango) {.a = 0, .b = 10});
  arbolintervalos_insertar(arbol, (Rango) {.a = 8, .b = 10});
  arbolintervalos_insertar(arbol, (Rango) {.a = 10, .b = 15});

  Trie *trie = trie_crear();
  trie_agregar(trie, L"hola", arbol);
  assert(trie_obtener(trie, L"hola"));
  assert(!trie_obtener(trie, L"hol"));
  trie_destruir(trie);
}

ArbolIntervalos * crear_arbol_intervalo(Rango rango) {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, rango);
  return arbol;
}

ArbolIntervalos * crear_arbol_elemento(int elem) {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  arbolintervalos_insertar(arbol, (Rango) {.a = elem, .b = elem});
  return arbol;
}

void unir_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_union(arbolA, arbolB));
}

void intersectar_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_interseccion(arbolA, arbolB));
}

void resta_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA, wchar_t* aliasB) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  ArbolIntervalos * arbolB = trie_obtener(trie, aliasB);
  trie_agregar(trie, alias, arbolintervalos_resta(arbolA, arbolB));
}

void complemento_en_trie(Trie * trie, wchar_t * alias, wchar_t * aliasA) {
  ArbolIntervalos * arbolA = trie_obtener(trie, aliasA);
  trie_agregar(trie, alias, arbolintervalos_complemento(arbolA));
}

void uniones_tests_completo() {
  ArbolIntervalos * arbolPares = arbolintervalos_crear();

  const int inicio = -100000;
  const int final = 100000;

  for (int i = inicio; i <= final; i += 2) {
    arbolintervalos_insertar(arbolPares, (Rango) {.a= i, .b = i});
  }

  ArbolIntervalos * arbolImpares = arbolintervalos_crear();

  for (int i = inicio+1; i <= final; i += 2) {
    arbolintervalos_insertar(arbolImpares, (Rango) {.a= i, .b = i});
  }

  ArbolIntervalos * todo = arbolintervalos_union(arbolImpares, arbolPares);
  arbolintervalos_destruir(arbolPares);
  arbolintervalos_destruir(arbolImpares);

  assert(todo->arbolAvlNodo->rango.a == inicio);
  assert(todo->arbolAvlNodo->rango.b == final);
  assert(todo->arbolAvlNodo->derecha == NULL);
  assert(todo->arbolAvlNodo->izquierda == NULL);

  arbolintervalos_destruir(todo);
}

void uniones_tests() {
  ArbolIntervalos * arbol1 = crear_arbol_elemento(15);
  ArbolIntervalos * arbol2 = crear_arbol_elemento(17);
  ArbolIntervalos * arbol3 = crear_arbol_elemento(19);
  ArbolIntervalos * arbol4 = crear_arbol_elemento(21);
  ArbolIntervalos * arbol5 = crear_arbol_elemento(23);

  ArbolIntervalos * arbol = arbolintervalos_crear();
  ArbolIntervalos * arbolTmp1 = arbolintervalos_union(arbol, arbol1);
  ArbolIntervalos * arbolTmp2 = arbolintervalos_union(arbolTmp1, arbol2);
  ArbolIntervalos * arbolTmp3 = arbolintervalos_union(arbolTmp2, arbol3);
  ArbolIntervalos * arbolTmp4 = arbolintervalos_union(arbolTmp3, arbol4);
  ArbolIntervalos * arbolTmp5 = arbolintervalos_union(arbolTmp4, arbol5);

  arbolintervalos_destruir(arbol);
  arbolintervalos_destruir(arbol1);
  arbolintervalos_destruir(arbol2);
  arbolintervalos_destruir(arbol3);
  arbolintervalos_destruir(arbol4);
  arbolintervalos_destruir(arbol5);
  arbolintervalos_destruir(arbolTmp1);
  arbolintervalos_destruir(arbolTmp2);
  arbolintervalos_destruir(arbolTmp3);
  arbolintervalos_destruir(arbolTmp4);
  arbolintervalos_destruir(arbolTmp5);
}

Rango elem(int elemento) {
  return (Rango) {.a = elemento, .b = elemento};
}

void agregar_tests() {
  ArbolIntervalos * arbol = crear_arbol_elemento(15);
  arbolintervalos_insertar(arbol, elem(17));
  arbolintervalos_insertar(arbol, elem(19));
  arbolintervalos_insertar(arbol, elem(21));
  arbolintervalos_insertar(arbol, elem(23));

  arbolintervalos_destruir(arbol);
}

void eliminar_tests() {
  ArbolIntervalos * arbol = crear_arbol_elemento(15);
  arbolintervalos_insertar(arbol, elem(17));
  arbolintervalos_insertar(arbol, elem(19));
  arbolintervalos_insertar(arbol, elem(21));
  arbolintervalos_insertar(arbol, elem(23));

  arbolintervalos_eliminar(arbol, elem(21));
  arbolintervalos_eliminar(arbol, elem(17));
  arbolintervalos_eliminar(arbol, elem(19));
  arbolintervalos_eliminar(arbol, elem(23));
  arbolintervalos_eliminar(arbol, elem(15));
  assert(arbol->arbolAvlNodo == NULL);

  arbolintervalos_destruir(arbol);
}

void test_arbol_trie() {
  ArbolIntervalos * arbol1 = crear_arbol_intervalo((Rango) {.a = 15, .b = 15});
  ArbolIntervalos * arbol2 = crear_arbol_intervalo((Rango) {.a = -5, .b = 0});
  ArbolIntervalos * arbol3 = crear_arbol_intervalo((Rango) {.a = 0, .b = 10});
  ArbolIntervalos * arbol4 = crear_arbol_intervalo((Rango) {.a = 8, .b = 10});
  ArbolIntervalos * arbol5 = crear_arbol_intervalo((Rango) {.a = 10, .b = 15});
  ArbolIntervalos * resultado;

  Trie *trie = trie_crear();
  trie_agregar(trie, L"hola1", arbol1);
  trie_agregar(trie, L"hola2", arbol2);
  trie_agregar(trie, L"hola3", arbol3);
  trie_agregar(trie, L"hola4", arbol4);
  trie_agregar(trie, L"hola5", arbol5);

  unir_en_trie(trie, L"a", L"hola1", L"hola2");
  intersectar_en_trie(trie, L"b", L"hola1", L"hola2");
  intersectar_en_trie(trie, L"c", L"hola2", L"hola3");
  complemento_en_trie(trie, L"d", L"hola5");

  resultado = trie_obtener(trie, L"a");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 15, .b = 15})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = -5, .b = 0})));

  resultado = trie_obtener(trie, L"b");
  assert(!resultado->arbolAvlNodo);

  resultado = trie_obtener(trie, L"c");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 0, .b = 0})));

  resultado = trie_obtener(trie, L"d");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = INT_MIN, .b = -1})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 11, .b = INT_MAX})));

  trie_destruir(trie);
}

bool arbol_vacio(ArbolIntervalos * arbol) {
  return arbol->arbolAvlNodo == NULL;
}

void test_propiedades() {
  ArbolIntervalos * A = arbolintervalos_crear();
  arbolintervalos_insertar(A, elem(0));
  arbolintervalos_insertar(A, elem(1));
  arbolintervalos_insertar(A, elem(2));
  arbolintervalos_insertar(A, elem(3));
  ArbolIntervalos * B = crear_arbol_intervalo((Rango) {.a = -3, .b = 5});
  ArbolIntervalos * C = crear_arbol_intervalo((Rango) {.a = 3, .b = 7});

  {//Colmutativa union
    ArbolIntervalos * union1 = arbolintervalos_union(A, B);
    ArbolIntervalos * union2 = arbolintervalos_union(B, A);

    assert((arbol_vacio(union1) && arbol_vacio(union2))
    || union1->arbolAvlNodo->rango.a == union2->arbolAvlNodo->rango.a);
    assert((arbol_vacio(union1) && arbol_vacio(union2))
    || union1->arbolAvlNodo->rango.b == union2->arbolAvlNodo->rango.b);

    arbolintervalos_destruir(union1);
    arbolintervalos_destruir(union2);
  }

  {//Colmutativa interseccion
    ArbolIntervalos * interseccion1 = arbolintervalos_interseccion(A, B);
    ArbolIntervalos * interseccion2 = arbolintervalos_interseccion(B, A);

    assert((arbol_vacio(interseccion1) && arbol_vacio(interseccion2))
    || interseccion1->arbolAvlNodo->rango.a == interseccion2->arbolAvlNodo->rango.a);
    assert((arbol_vacio(interseccion1) && arbol_vacio(interseccion2))
    || interseccion1->arbolAvlNodo->rango.b == interseccion2->arbolAvlNodo->rango.b);

    arbolintervalos_destruir(interseccion1);
    arbolintervalos_destruir(interseccion2);
  }

  {//Distributiva union
    ArbolIntervalos * CA = arbolintervalos_union(C, A);
    ArbolIntervalos * CB = arbolintervalos_union(C, B);
    ArbolIntervalos * AiB = arbolintervalos_interseccion(A, B);

    ArbolIntervalos * CAiB = arbolintervalos_union(C, AiB);
    ArbolIntervalos * CAiCB = arbolintervalos_interseccion(CA, CB);

    assert(CAiB->arbolAvlNodo->rango.a == CAiCB->arbolAvlNodo->rango.a);
    assert(CAiB->arbolAvlNodo->rango.b == CAiCB->arbolAvlNodo->rango.b);

    arbolintervalos_destruir(CA);
    arbolintervalos_destruir(CB);
    arbolintervalos_destruir(AiB);
    arbolintervalos_destruir(CAiB);
    arbolintervalos_destruir(CAiCB);
  }

  {//Distributiva interseccion
    ArbolIntervalos * CiA = arbolintervalos_interseccion(C, A);
    ArbolIntervalos * CiB = arbolintervalos_interseccion(C, B);
    ArbolIntervalos * AB = arbolintervalos_union(A, B);

    ArbolIntervalos * CiAB = arbolintervalos_interseccion(C, AB);
    ArbolIntervalos * CiACiB = arbolintervalos_union(CiA, CiB);

    assert(CiAB->arbolAvlNodo->rango.a == CiACiB->arbolAvlNodo->rango.a);
    assert(CiAB->arbolAvlNodo->rango.b == CiACiB->arbolAvlNodo->rango.b);

    arbolintervalos_destruir(CiA);
    arbolintervalos_destruir(CiB);
    arbolintervalos_destruir(AB);
    arbolintervalos_destruir(CiAB);
    arbolintervalos_destruir(CiACiB);
  }

  { //Interseccion como resta
    ArbolIntervalos * AiB = arbolintervalos_interseccion(A, B);
    ArbolIntervalos * r1 = arbolintervalos_resta(A, B);
    ArbolIntervalos * r2 = arbolintervalos_resta(A, r1);

    assert((arbol_vacio(AiB) && arbol_vacio(r2))
    || AiB->arbolAvlNodo->rango.a == r2->arbolAvlNodo->rango.a);
    assert((arbol_vacio(AiB) && arbol_vacio(r2))
    || AiB->arbolAvlNodo->rango.b == r2->arbolAvlNodo->rango.b);

    arbolintervalos_destruir(AiB);
    arbolintervalos_destruir(r1);
    arbolintervalos_destruir(r2);
  }

  {//De Morgan union
    ArbolIntervalos * cA = arbolintervalos_complemento(A);
    ArbolIntervalos * cB = arbolintervalos_complemento(B);
    ArbolIntervalos * cAcB = arbolintervalos_union(cA, cB);
    ArbolIntervalos * AiB = arbolintervalos_interseccion(A, B);
    ArbolIntervalos * cAiB = arbolintervalos_complemento(AiB);

    assert(cAcB->arbolAvlNodo->alto == cAiB->arbolAvlNodo->alto);

    ArbolIntervalos * resta = arbolintervalos_resta(cAcB, cAiB);

    assert(resta->arbolAvlNodo == NULL);

    arbolintervalos_destruir(cA);
    arbolintervalos_destruir(cB);
    arbolintervalos_destruir(cAcB);
    arbolintervalos_destruir(AiB);
    arbolintervalos_destruir(cAiB);
    arbolintervalos_destruir(resta);
  }

  {//De Morgan interseccion
    ArbolIntervalos * cA = arbolintervalos_complemento(A);
    ArbolIntervalos * cB = arbolintervalos_complemento(B);
    ArbolIntervalos * cAicB = arbolintervalos_interseccion(cA, cB);
    ArbolIntervalos * AB = arbolintervalos_union(A, B);
    ArbolIntervalos * cAB = arbolintervalos_complemento(AB);

    assert(cAicB->arbolAvlNodo->alto == cAB->arbolAvlNodo->alto);

    ArbolIntervalos * resta = arbolintervalos_resta(cAicB, cAB);

    assert(resta->arbolAvlNodo == NULL);

    arbolintervalos_destruir(cA);
    arbolintervalos_destruir(cB);
    arbolintervalos_destruir(cAicB);
    arbolintervalos_destruir(AB);
    arbolintervalos_destruir(cAB);
    arbolintervalos_destruir(resta);
  }

  arbolintervalos_destruir(A);
  arbolintervalos_destruir(B);
  arbolintervalos_destruir(C);
}

void main_tests() {
  //basicos arbol
  agregar_tests();
  eliminar_tests();

  //avanzados arbol
  uniones_tests();
  uniones_tests_completo();
  test_propiedades();

  //trie
  test_trie();

  //completo
  test_arbol_trie();
}
