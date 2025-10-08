compilar:
	gcc -g -Wall -Wextra main.c src/*.c -o programa
compilar_t:
	gcc -g -Wall -Wextra pruebas_alumno.c src/*.c -o tests
valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./programa assets/pokemones.csv buscar nombre Pikachu
valgrind_t:
	valgrind --leak-check=full --show-leak-kinds=all ./tests
