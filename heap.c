#include <stdio.h>
#include <stdlib.h>

typedef struct {				// Struct para almacenar Heap de Minimos
	unsigned int *values;		// Valores a almacenar en el Heap en Array
	unsigned short size;		// Tamano total del Heap (notar que en la primera
} HeapMin;						// posicion se deja siempre un valor vacio)

/*****
* void inicializar
******
* Crea array del espacio necesario segun la cantidad de terminos + 1
******
* Input:
* HeapMin * base : puntero donde se inicializa el Heap
* unsigned short tam : cantidad de valores a introducir
******
* Returns:
* void
*****/
void inicializar(HeapMin *heap, unsigned short tam){
	heap->values = (unsigned int *)calloc(tam+1, sizeof(unsigned int));		// Inicializar valores con 0
	heap->size = 0;
}

/*****
* void flotar
******
* Intercambia posiciones de los valores en el heap flotandolo asegurandose
* de mantener el orden de Heap de Minimos
******
* Input:
* HeapMin * heap : puntero al Heap en el cual se esta trabajando
* unsigned short pos_hijo : posicion actual del valor que se desea flotar
******
* Returns:
* void
*****/
void flotar(HeapMin *heap, unsigned short pos_hijo){
	if (pos_hijo == 1){						// Caso base al llegar a la raiz
		return;
	}
	unsigned short padre;					// Calcular posicion del padre
	if (pos_hijo % 2 == 0){					// dependiendo de si es par o impar
		padre = pos_hijo/2;
	}
	else {
		padre = (pos_hijo-1)/2;
	}
	if (heap->values[pos_hijo] < heap->values[padre]){	// Intercambiar valores
		unsigned int aux = heap->values[pos_hijo];		// del flote
		heap->values[pos_hijo] = heap->values[padre];
		heap->values[padre] = aux;
		flotar(heap, padre);	// Intentar flotar la nueva posicion de ser posible
	}
}

/*****
* void insertHeap
******
* Inserta valor en la primera posicion vacia del Heap, de ser necesario flota este valor
******
* Input:
* HeapMin * heap : puntero que representa el Heap en el que se trabaja
* unsigned int value : valor a insertar en el Heap
******
* Returns:
* void
*****/
void insertHeap(HeapMin *heap, unsigned int value){
	heap->values[++heap->size] = value;				// Actualizar tamano, asignar valor
	flotar(heap, heap->size);						// Intentar flotar de ser necesario en esa posicion

}

/*****
* void freeHeap
******
* Libera memoria usada por el Heap
******
* Input:
* HeapMin * heap : puntero del Heap que se desea liberar
******
* Returns:
* void
*****/
void freeHeap(HeapMin *heap){
	free(heap->values);				// Liberar array de valores
	free(heap);						// Liberar Heap
}

/*****
* void Hundir
******
* Hunde el valor de la posición introducida para mantener el heap minimo
******
* Input:
* HeapMin * heap : puntero al Heap en el cual se esta trabajando
* int posicion : posicion actual del nodo que se va a hundir
******
* Returns:
* void
*****/
void Hundir(HeapMin *heap, int posicion){
	unsigned int izq=0, der=0;
	int pos=0;
	if((posicion*2) <= heap->size){
		izq = heap->values[posicion*2];
	}
	if(((posicion*2)+1) <= heap->size){
		der = heap->values[(posicion*2)+1];
	}
	if(izq > 0){
		if(der > 0){
			if(izq < heap->values[posicion]){
				if(izq < der){
					unsigned int aux = heap->values[posicion];
					heap->values[posicion] = izq;
					heap->values[posicion*2] = aux;
					pos = posicion*2;
				}
				else{
					unsigned int aux = heap->values[posicion];
					heap->values[posicion] = der;
					heap->values[(posicion*2)+1] = aux;
					pos = (posicion*2)+1;
				}
			}
			if(der<heap->values[posicion]){
				unsigned int aux = heap->values[posicion];
				heap->values[posicion] = der;
				heap->values[(posicion*2)+1] = aux;
				pos = (posicion*2)+1;
			}
		}
		if(izq < heap->values[posicion]){
			unsigned int aux = heap->values[posicion];
			heap->values[posicion] = izq;
			heap->values[posicion*2] = aux;
			pos = posicion*2;
		}
	}
	if(pos==0){
		return;
	}
	Hundir(heap, pos);
}

/*****
* unsigned int SacarValor
******
* Intercambia la raiz del heap por el ultimo valor y llama a la funcion Hundir para dejar el heap como minimo,
* luego sacar el valor minimo del heap como un unsigned int
******
* Input:
* HeapMin * heap : puntero al Heap en el cual se esta trabajando
******
* Returns:
* unsigned int, retorna el valor minimo del heap
*****/
unsigned int SacarValor(HeapMin *heap){
	unsigned int aux = heap->values[1];
	heap->values[1]= heap->values[(heap->size)--];
	Hundir(heap, 1);
	return aux;
}

int main (){
	unsigned int i, valor, suma, costo_actual;												// Main de prueba
	unsigned short total;
	printf("Cuantos numeros quiere sumar?: ");
	scanf("%hu",&total);
	while (total!=0) {
		HeapMin *colap = (HeapMin *) malloc(sizeof(HeapMin));		// Reserva memoria para el HeapMin
		inicializar(colap, total);					// Inicia el heap,con valores y tamanno en 0 
		i=0;
		while (i < total) {
			printf("Ingrese numero a sumar: ");
			scanf("%u", &valor);
			insertHeap(colap, valor);				// Inserta en el heap colap el valor leído de la consola
			i++;
		}
		i = suma = 0;
		valor = SacarValor(colap);					// Asignar valor inicial
		costo_actual = valor;
		while (i < total-1) {						//Iteración que suma los costos de las sumas.
			valor = SacarValor(colap);
			costo_actual += valor;
			suma += costo_actual;
			i++;
		}
		printf("El menor costo de la suma es: %u\n", suma);
		freeHeap(colap);
		printf("Cuantos numeros quiere sumar?: ");
		scanf("%hu", &total);
	}
	return 1;
}
