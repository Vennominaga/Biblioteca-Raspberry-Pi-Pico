#ifndef ALPHABET_DATA_H
#define ALPHABET_DATA_H

#include "inc/MatrizRGBPI.h" // Para a definição da estrutura Letter

/**
 * @file alphabet.h
 * @brief Cabeçalho para o dicionário de caracteres (A-Z e espaço) utilizado pela matriz de LEDs.
 * 
 * Este arquivo declara o array `alphabet`, que contém as representações gráficas das letras
 * em formato de matriz 5x5, e a constante `ALPHABET_COUNT` com o tamanho do dicionário.
 * As definições são implementadas em `alphabet.c`.
 */

/**
 * @brief Array contendo as letras do alfabeto (A-Z) e espaço, cada uma com sua matriz de pixels.
 * @details Cada entrada é uma estrutura `Letter` com:
 *   - character: O caractere (ex: 'A', 'B', ' ').
 *   - matrix: Matriz 5x5x3 onde valores não nulos indicam pixels ativos (a cor é definida em tempo de execução).
 */
extern const Letter alphabet[];

/**
 * @brief Tamanho do array `alphabet` (número de caracteres disponíveis).
 */
extern const size_t ALPHABET_COUNT;

#endif // ALPHABET_DATA_H
