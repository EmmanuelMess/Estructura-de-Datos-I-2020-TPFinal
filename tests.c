#include "tests.h"
#include <assert.h>
#include "tests.h"
#include "avl/arbol_intervalos_extras.h"
#include "avl/arbol_intervalos.h"
#include "trie/trie.h"

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

void unir(ArbolIntervalos ** dest, ArbolIntervalos * src) {
  ArbolIntervalos * arbol = arbolintervalos_union(*dest, src);
  *dest = arbol;
}

void uniones_tests_completo() {
  ArbolIntervalos * arbol = arbolintervalos_crear();
  for (int i = -2000; i < 2000; i += 2) {
    ArbolIntervalos * arbol1 = crear_arbol_elemento(i);
    unir(&arbol, arbol1);
    arbolintervalos_destruir(arbol1);
  }
}


void uniones_tests() {
  ArbolIntervalos * arbol1 = crear_arbol_elemento(15);
  ArbolIntervalos * arbol2 = crear_arbol_elemento(17);
  ArbolIntervalos * arbol3 = crear_arbol_elemento(19);
  ArbolIntervalos * arbol4 = crear_arbol_elemento(21);
  ArbolIntervalos * arbol5 = crear_arbol_elemento(23);

  ArbolIntervalos * arbol = arbolintervalos_crear();
  unir(&arbol, arbol1);
  unir(&arbol, arbol2);
  unir(&arbol, arbol3);
  unir(&arbol, arbol4);
  unir(&arbol, arbol5);

  arbolintervalos_destruir(arbol1);
  arbolintervalos_destruir(arbol2);
  arbolintervalos_destruir(arbol3);
  arbolintervalos_destruir(arbol4);
  arbolintervalos_destruir(arbol5);
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

void main_tests() {
  //basicos arbol
  agregar_tests();

  //extras arbol
  uniones_tests();
  uniones_tests_completo();

  //trie
  test_trie();

  //completo
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
  assert(!resultado->arbolAvlNode);

  resultado = trie_obtener(trie, L"c");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 0, .b = 0})));

  resultado = trie_obtener(trie, L"d");
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = INT_MIN, .b = -1})));
  assert(!inexistente(arbolintervalos_intersectar(resultado, (Rango) {.a = 11, .b = INT_MAX})));

  trie_destruir(trie);


}
