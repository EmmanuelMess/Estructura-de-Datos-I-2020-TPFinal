#include <stdbool.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include "trie/trie.h"
#include "parseador.h"
#include "avl/deque.h"
#include "avl/arbol_intervalos_extras.h"
#include "debug.h"

/**
 * Dados dos punteros en el mismo array de caracteres,
 * devuelve el que mas adelante este
 */
wchar_t* max_puntero(wchar_t* a, wchar_t* b) {
  return a > b? a: b;
}

/**
 * Dado un arbol de intervalos,
 * que cumple con las condiciones detalladas en arbol_intervalos.h,
 * Imprime la lista de intervalos disjuntos contenidos en el.
 *
 * Aviso: Ningun orden particular es usado.
 */
void imprimir_intervalos(ArbolIntervalos *arbol) {
  if (arbol->arbolAvlNodo == NULL) {
    wprintf(L"{}\n");
    return;
  }

  Deque *deque = deque_crear();

  deque_push_front(deque, arbol->arbolAvlNodo);

  wprintf(L"{");

  while (!deque_vacio(deque)) {
    ArbolIntervalosNodo *nodo = deque_pop_front(deque);

    if (nodo->izquierda)
      deque_push_front(deque, nodo->izquierda);

    if (nodo->derecha)
      deque_push_front(deque, nodo->derecha);

    if(nodo->rango.a == nodo->rango.b) wprintf(L"%d", nodo->rango.a);
    else wprintf(L"%d:%d", nodo->rango.a, nodo->rango.b);

    if (!deque_vacio(deque)) wprintf(L",");
  }

  wprintf(L"}\n");

  deque_destruir(deque);
}


/**
 * NOMBRE
 *   Interprete
 * SINOPSIS
 *   Interprete
 * DESCRIPCION
 *   Implementacion simple de un interprete capaz de hacer operaciones de
 *   asignacion de conjuntos:
 *     <alias> = {<aliasInterno>: k <= <aliasInterno> <= k1}
 *     <alias> = {k, k1, k2, ...}
 *   Union:
 *     <alias> = <alias1> | <alias2>
 *   Interseccion:
 *     <alias> = <alias1> & <alias2>
 *   Resta:
 *     <alias> = <alias1> - <alias2>
 *   Complemento:
 *     <alias> = ~<alias1>
 *   Impresion:
 *     imprimir <alias>
 *   Salida:
 *     salir
 * AUTOR
 *   Facundo Emmanuel Messulam
 */
int main(int argc, char *argv[]) {
  debug_main(argc, argv);

  if (fwide(stdout, 1) <= 0)
    puts("No se pudo crear la salida correctamente, es posible que no ande el imprimir\n");

  char *locale = "es_ES.utf8";

  if (setlocale(LC_ALL, locale) == NULL)
    wprintf(L"Error: no tiene el locale %s instalado, tildes y otros no van a funcionar\n",
      locale);
  else
    wprintf(L"Pruebe tildes y eñes!\n");

  Trie *trie = trie_crear();
  bool sigue = true;

  wprintf(L"Tamaño maximo de una linea: 1023 caracteres\n");

  while (sigue) {
    wprintf(L"> ");
    wchar_t *entrada = malloc(1024 * sizeof(wchar_t));
    entrada = fgetws(entrada, 1024, stdin);
    if (entrada != NULL) {
      remover_espacios(entrada);

      Metadatos metadatos = parser(entrada);

      if (metadatos.error) {
        wprintf(L"Error de parseo: %.5ls\n",
                max_puntero(entrada, metadatos.posError - 2));

        if (metadatos.error == METADATOS_ERROR_ARG_IMPRIMIR_NUM)
          wprintf(L"Uso: imprimir <alias>!\n");

        sigue = true;
      } else if (metadatos.salir)
        sigue = false;
      else if (metadatos.imprimir) {
        wchar_t *alias = entrada + wcslen(L"imprimir");
        ArbolIntervalos *arbol = trie_obtener(trie, alias);

        if (arbol == NULL) {
          wprintf(L"Alias no existe: %s\n", alias);
        } else {
          imprimir_intervalos(arbol);
          debug0(arbol);
        }
      } else if (metadatos.union_ || metadatos.interseccion ||
                 metadatos.resta) {
        wchar_t alias[metadatos.largoAlias + 1];
        wchar_t aliasA[metadatos.largoOperando1 + 1];
        wchar_t aliasB[metadatos.largoOperando2 + 1];
        procesar_operacion(metadatos, entrada, alias, aliasA, aliasB);

        ArbolIntervalos *arbolA = trie_obtener(trie, aliasA);
        ArbolIntervalos *arbolB = trie_obtener(trie, aliasB);

        if (arbolA == NULL || arbolB == NULL) {
          if (arbolA == NULL) wprintf(L"Alias no existe: %s\n", aliasA);
          if (arbolB == NULL) wprintf(L"Alias no existe: %s\n", aliasB);
        } else {
          ArbolIntervalos *resultado;
          if (metadatos.union_)
            resultado = arbolintervalos_union(arbolA, arbolB);
          else if (metadatos.interseccion)
            resultado = arbolintervalos_interseccion(arbolA, arbolB);
          else if (metadatos.resta)
            resultado = arbolintervalos_resta(arbolA, arbolB);
          else assert(false);

          if (trie_obtener(trie, alias) != NULL)
            arbolintervalos_destruir(trie_obtener(trie, alias));

          trie_agregar(trie, alias, resultado);
        }
      } else if (metadatos.complemento) {
        wchar_t alias[metadatos.largoAlias + 1];
        wchar_t aliasA[metadatos.largoOperando1];
        procesar_operacion(metadatos, entrada, alias, aliasA, NULL);

        ArbolIntervalos *arbol = trie_obtener(trie, aliasA);

        if (arbol == NULL) {
          wprintf(L"Alias no existe: %s\n", aliasA);
        } else {
          ArbolIntervalos *resultado = arbolintervalos_complemento(arbol);

          if (trie_obtener(trie, alias) != NULL)
            arbolintervalos_destruir(trie_obtener(trie, alias));

          trie_agregar(trie, alias, resultado);
        }
      } else {
        wchar_t alias[metadatos.largoAlias + 1];
        ArbolIntervalos *arbol = arbolintervalos_crear();

        if (metadatos.esExtension) {
          int enteros[metadatos.cantidadDeEnteros +
                      1];//std C no permite arrays automaticos de largo 0
          procesar_asignacion_extension(metadatos, entrada, alias, enteros);

          for (int i = 0; i < metadatos.cantidadDeEnteros; ++i)
            arbolintervalos_insertar(arbol,
                                     (Rango) {.a = enteros[i], .b = enteros[i]});

          debug1(alias, metadatos, enteros);
        } else {
          Rango rango;

          procesar_asignacion_compresion(metadatos, entrada, alias, &rango);

          arbolintervalos_insertar(arbol, rango);

          debug2(alias, rango);
        }

        trie_agregar(trie, alias, arbol);

        debug3(alias, trie);
      }
    }

    free(entrada);
  }

  trie_destruir(trie);
}
