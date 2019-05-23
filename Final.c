/*---------------------------------------------+
| Nombre: Final.c                              |
| Sinopsis:El programa muestra un menu con     |
|   las distintias opciones para manipular     |
|   y leer ficheros con datos formateados.     |
| Organización: Universidad de Burgos          |
| Autor:                                       |
|   Diego, González Román                      |
| Fecha: 08-04-2018                            |
| Versión: v1.0                                |
| Información del copyright del programa:      |
|   Licencia pública MIT 2019                  |
+---------------------------------------------*/


/*Directivas al precompilador*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define NOMFICHERO "breast-cancer.txt"
#define NOMFICHERO2 "breast-cancer2.txt"

char class[2][25] = {"no-recurrence-events", "recurrence-events"};
char age[9][10] = {"10-19", "20-29", "30-39", "40-49", "50-59", "60-69", "70-79", "80-89", "90-99"};
char menopause[3][10] = {"lt40", "ge40", "premeno"};
char tumor[12][10] = {"0-4", "5-9", "10-14", "15-19", "20-24", "25-29", "30-34", "35-39", "40-44", "45-49", "50-54", "54-59"};
char invnodes[13][10] = {"0-2", "3-5", "6-8", "9-11", "12-14", "15-17", "18-20", "21-23", "24-26", "27-29", "30-32", "33-35", "36-39"};
char nodecaps[3][10] = {"yes", "no", "?"};
char degmalin[3][10] = {"1", "2", "3"};
char breast[2][5] = {"left", "right"};
char breastquad[6][20] = {"left-up", "left-low", "right-up", "right-low", "central", "?"};
char irradiat[2][10] = {"yes", "no"};
char add[2][10] = {"Si", "No"};

//Vector de registros
struct CANCER{
  char clase[25], menopause[10], invNodeCaps[4], breast[8], breastQuad[10], irradiat[4];
  int ageMin, ageMax, tumorSizeMin, tumorSizeMax, invNodesMin, invNodesMax, degMalig;
};

/* Declaraciones forward de las funciones*/
int solicitarOpcionMenu();
void seleccionarOpcion(int);
int contarCaracteres(FILE*);
void distribuirTamanos(FILE*);
int buscarTumor(char *);
int insertarFila(FILE*);
int contarFilas(FILE*);
int limpiarBuffer();
int menuClass();
int menuAge();
int menuMenopause();
int menuTumor();
int menuInvnoves();
int menuNodecaps();
int menuDegmalin();
int menuBreast();
int menuBreastquad();
int menuIrradiat();
int menuAdd();
int eliminarDuplicados(FILE*, FILE*);
int actualizarRiesgo(struct CANCER[], int, int);
int calcularTamanoMedio(struct CANCER[],  int);
void escribirBinario(struct CANCER lista[], FILE *f, int numFilas);
int cargarRegistros(struct CANCER[], FILE *);
void quitarPrimerCaracter(char[]);
struct CANCER copia(struct CANCER);

/*Programa Principal*/

int main(){
  int op;
  do{
    op = solicitarOpcionMenu();
    seleccionarOpcion(op);
  }while(op != 0);
}

/*-------------------------------------------------- clean_stdin -----
|  Función clean_stdin
|
|  Objetivo:  Limpia caracteres que se encuentren pendientes en
|      la entrada standard (stdin).
|
|  Parameteros:  Nada.
|
|  Devuelve:
|      int 1 -- Valor constante.
|
*-------------------------------------------------------------------*/
int clean_stdin(){
  while(getchar() != '\n');
  return 1;
}

/*------------------------------------------- solicitarOpcionMenu -----
|  Función solicitarOpcionMenu
|
|  Objetivo:  Muestra un menu en pantalla y espera un número
|      entre los valores 0 y 5, correspondientes a las
|      distintas funcionalidades del programa.
|
|  Parameteros:  Nada.
|
|  Devuelve:
|      int option -- Número elegido y comprobado.
|
*-------------------------------------------------------------------*/

int solicitarOpcionMenu(){
  int opcion, leido;
  char enter;
  printf(" ____________________________________\n");
  printf("|                                    |\n");
  printf("|      Selecciona una opción         |\n");
  printf("|____________________________________|\n");
  printf("|                                    |\n");
  printf("|  0. Salir                          |\n");
  printf("|  1. Numero total de caracteres     |\n");
  printf("|  2. Tamaño medio del tumor         |\n");
  printf("|  3. Añadir filas                   |\n");
  printf("|  4. Eliminar duplicados            |\n");
  printf("|  5. Pacientes en alto riesgo       |\n");
  printf("|                                    |\n");
  printf("|____________________________________|\n");

  do{
    printf("Introduce una opción:  \n");
    leido = scanf("%d%c", &opcion, &enter);
  }while(opcion < 0 || opcion > 5);
  return opcion;
}

/*--------------------------------------------- seleccionaOpcion -----
|  Subrutina seleccionaOpcion
|
|  Objetivo:  Realiza distintas funciones dependiendo del
|      paramentro de entrada.
|
|  Parameteros:
|      int option -- número de opción elegida.
|
|  Devuelve:  Nada
|
*-------------------------------------------------------------------*/

void seleccionarOpcion(int opcion){
  FILE *breastCancer;
  FILE *breastCancer2;
  int duplicados;

  switch(opcion){
    case 0:
      break;
    case 1: //Numero total de caracteres
      breastCancer = fopen(NOMFICHERO, "r");
      printf("En total hay %d caracteres\n", contarCaracteres(breastCancer));
      fclose(breastCancer);
      break;
    case 2://tamaño medio del tumor
      breastCancer = fopen(NOMFICHERO, "r");
      printf("Distribucion por edad y tamaños");
      distribuirTamanos(breastCancer);
      fclose(breastCancer);
      break;
    case 3://Añadir filas
      breastCancer = fopen(NOMFICHERO, "a");
      insertarFila(breastCancer);
      printf("\nAñadir filas\n");
      fclose(breastCancer);
    break;
    case 4://Eliminar duplicados
      breastCancer = fopen(NOMFICHERO, "r");
      breastCancer2 = fopen(NOMFICHERO2, "w");
      duplicados = eliminarDuplicados(breastCancer, breastCancer2);
      printf("\nSe han encontrado %d duplicados\n", duplicados);
      fclose(breastCancer);
      fclose(breastCancer2);
    break;
    case 5://Pacientes alto riesgo
      breastCancer = fopen(NOMFICHERO, "r");
      struct CANCER lista[300];
      int numFilas = cargarRegistros(lista, breastCancer);
      breastCancer = fopen(NOMFICHERO, "r");
      int media = calcularTamanoMedio(lista, numFilas);
      int numAltoRiesgo;
      numAltoRiesgo = actualizarRiesgo(lista, media, numFilas);
      printf("El tamaño medio del tumor es: %d\n",media );
      printf("Se han grabado %d registros de alto riesgo\n",numAltoRiesgo );
      fclose(breastCancer);
    break;
  }
}

/*----------------------------------------------- contarCaracteres ----
|  Función contarCaracteres
|
|  Objetivo:  Cuenta el número de caracteres en el fichero.
|
|  Parameteros:
|      FILE *breastCancer -- Archivo a ser leido.
|
|  Devuelve:  Nada.
|
*-------------------------------------------------------------------*/

int contarCaracteres(FILE *breastCancer){
  int numChar = 0;
  fgetc(breastCancer);
  while(!feof(breastCancer)){
      ++numChar;
      fgetc(breastCancer);
  }
  return numChar;
}

/*--------------------------------------------- distribuirTamanos -----
|  Función distribuirTamanos
|
|  Objetivo:  nos  mostrará  el  porcentaje  de  pacientes  que
|  existen en cada grupo de tamaños de tumor
|
|  Parameteros:
|      FILE *f -- Archivo a ser leido.
|
|  Devuelve:
|      int rowNum -- número de filas.
|
*-------------------------------------------------------------------*/

void distribuirTamanos(FILE *f){
  #define FORMATO "%*[^,],%5[^,],%*[^,],%5[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^\n]"

  char ageBuscada[20], tamano[20], edad[20];
  float veces[]={0,0,0,0,0,0,0,0,0,0,0,0};
  float contador = 0;
  int op, pos;
  op = menuAge();
  strcpy(ageBuscada,age[op]);
  fscanf(f, FORMATO, edad, tamano);
  while(!feof(f)){
      if(strcmp(ageBuscada, edad) == 0){
        pos = buscarTumor(tamano);
        veces[pos]++;
        contador++;
      }
  fscanf(f, FORMATO, edad, tamano);
  }
  printf("Tamaño\tPorcentaje\n");
  for(int i = 0; i < 12; i++){
    printf("%s\t%.2f%%\n",tumor[i], veces[i]/contador*100);
  }
}


/*----------------------------------------------- buscarTumor ----
|  Función buscarTumor
|
|  Objetivo:  Busca el tamaña del tumor por cada fila
|
|  Parameteros:
|      Char buscado
|
|  Devuelve:  pos
|
*-------------------------------------------------------------------*/

int buscarTumor(char *buscado){
  int pos;
  for(int i = 0; i < 11; i++){
    if(strcmp(buscado,tumor[i]) == 0){
      pos = i;
    }
  }
  return pos;
}

/*----------------------------------------------- menuClass ----
|  Función menuClass
|
|  Objetivo:  Crea un menu para la clase de cancer y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuClass(){
  int op;
  printf("\n\nClase:\n");
  for(int i = 0; i < 2; i++){
    printf("%d. %s\n",i, class[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 1);
  return op;
  }

  /*----------------------------------------------- menuAge ----
  |  Función menuAge
  |
  |  Objetivo:  Crea un menu para la edad del paciente y te pide
  |  una opcion
  |
  |  Parameteros:
  |      Nada
  |
  |  Devuelve:  op
  |
  *-------------------------------------------------------------------*/

  int menuAge(){
    int op;
    printf("\n\nEdad:\n");
    for(int i = 0; i < 9; i++){
      printf("%d. %s\n",i, age[i]);
    }
    do{
      printf("Elige una opcion: \n");
      scanf("%d",&op);
    }while(op <  0 || op > 9);
    return op;
  }

  /*----------------------------------------------- menuMenopause ----
  |  Función menuMenopause
  |
  |  Objetivo:  Crea un menu para el tipo de menopausia y te pide
  |  una opcion
  |
  |  Parameteros:
  |      Nada
  |
  |  Devuelve:  op
  |
  *-------------------------------------------------------------------*/


  int menuMenopause(){
    int op;
    printf("\n\nMenopause:\n");
    for(int i = 0; i < 3; i++){
      printf("%d.  %s\n",i, menopause[i]);
    }
    do{
      printf("Elige una opcion: \n");
      scanf("%d",&op);
    }while(op <  0 || op > 2);
    return op;
  }

  /*----------------------------------------------- menuTumor ----
  |  Función menuTumor
  |
  |  Objetivo:  Crea un menu para el tamaño del tumor y te pide
  |  una opcion
  |
  |  Parameteros:
  |      Nada
  |
  |  Devuelve:  op
  |
  *-------------------------------------------------------------------*/

int menuTumor(){
  int op;
  printf("\n\nTamaño del tumor:\n");
  for(int i = 0; i < 12; i++){
    printf("%d.  %s\n",i, tumor[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 11);
  return op;
}

/*----------------------------------------------- menuInvnoves ----
|  Función menuInvnoves
|
|  Objetivo:  Crea un menu para los invnodes y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuInvnoves(){
  int op;
  printf("\n\nNodos inversos:\n");
  for(int i = 0; i < 13; i++){
    printf("%d.  %s\n",i, invnodes[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 12);
  return op;
}

/*----------------------------------------------- menuNodecaps ----
|  Función menuNodecaps
|
|  Objetivo:  Crea un menu para los nodecaps y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuNodecaps(){
  int op;
  printf("\n\nCapas del nodo:\n");
  for(int i = 0; i < 3; i++){
    printf("%d.  %s\n",i, nodecaps[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 2);
  return op;
}

/*----------------------------------------------- menuDegmalin ----
|  Función menuDegmalin
|
|  Objetivo:  Crea un menu para la clase de cancer y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuDegmalin(){
  int op;
  printf("\n\nGrado de maligno:\n");
  for(int i = 0; i < 3; i++){
    printf("%d.  %s\n",i, degmalin[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 2);
  return op;
}

/*----------------------------------------------- menuBreast ----
|  Función menuBreast
|
|  Objetivo:  Crea un menu para el pecho y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuBreast(){
  int op;
  printf("\n\nPecho:\n");
  for(int i = 0; i < 2; i++){
    printf("%d.  %s\n",i, breast[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 1);
  return op;
}

/*-------------------------------------------------menuBreastquad ----
|  Función menuBreastquad
|
|  Objetivo:  Crea un menu para la posicion del tumor y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuBreastquad(){
  int op;
  printf("\n\nLugar del pecho:\n");
  for(int i = 0; i < 6; i++){
    printf("%d.  %s\n",i, breastquad[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 5);
  return op;
}

/*----------------------------------------------- menuIrradiat ----
|  Función menuIrradiat
|
|  Objetivo:  Crea un menu para si es irradiat o no y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuIrradiat(){
  int op;
  printf("\n\nIrradia:\n");
  for(int i = 0; i < 2; i++){
    printf("%d.  %s\n",i, irradiat[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 1);
  return op;
}

/*----------------------------------------------- menuAdd ----
|  Función menuAdd
|
|  Objetivo:  Crea un menu para añadir una fila y te pide
|  una opcion
|
|  Parameteros:
|      Nada
|
|  Devuelve:  op
|
*-------------------------------------------------------------------*/

int menuAdd(){
  int op;
  printf("\n\n¿Quieres añadir otra fila?\n");
  for(int i = 0; i < 2; i++){
    printf("%d.  %s\n",i, add[i]);
  }
  do{
    printf("Elige una opcion: \n");
    scanf("%d",&op);
  }while(op <  0 || op > 1);
  return op;
}

/*----------------------------------------------- insertarFila ----
|  Función insertarFila
|
|  Objetivo: inserta una fila añadiendo cada apartado correspondiente
|
|  Parameteros:
|      FILE *f
|
|  Devuelve:  0
|
*-------------------------------------------------------------------*/

int insertarFila(FILE* f){
  int op;
  char linea[80] = "";
  //1
  do{
  strcpy(linea,"");
  int idClass = menuClass();
  strcat(linea,class[idClass]);// añado el dato a la linea
  printf("%s\n",linea);
  //2
  int idAge = menuAge();
  strcat(linea,",");// añado el dato a la linea
  strcat(linea,age[idAge]);// añado el dato a la linea
  printf("%s\n",linea);
  //3
  int idMenopause = menuMenopause();
  strcat(linea,",");
  strcat(linea,menopause[idMenopause]);
  printf("%s\n", linea);
  //4
  int idTumor = menuTumor();
  strcat(linea,","); // añado la coma al dato de la 4 columna
  strcat(linea,tumor[idTumor]);// añado el dato a la linea
  printf("%s\n",linea);
  //5
  int idInvnodes = menuInvnoves();
  strcat(linea,","); // añado la coma al dato de la 5 columna
  strcat(linea,invnodes[idInvnodes]);// añado el dato a la linea
  printf("%s\n",linea);
  //6
  int idNodecaps = menuNodecaps();
  strcat(linea,","); // añado la coma al dato de la 6 columna
  strcat(linea,nodecaps[idNodecaps]);// añado el dato a la linea
  printf("%s\n",linea);
  //7
  int idDegmalin = menuDegmalin();
  strcat(linea,","); // añado la coma al dato de la 7 columna
  strcat(linea,degmalin[idDegmalin]);// añado el dato a la linea
  printf("%s\n",linea);
  //8
  int idBreast = menuBreast();
  strcat(linea,","); // añado la coma al dato de la 8 columna
  strcat(linea,breast[idBreast]);// añado el dato a la linea
  printf("%s\n",linea);
  //9
  int idBreastquad = menuBreastquad();
  strcat(linea,","); // añado la coma al dato de la 8 columna
  strcat(linea,breastquad[idBreastquad]);// añado el dato a la linea
  printf("%s\n",linea);
  //10
  int idIrradiat = menuIrradiat();
  strcat(linea,","); // añado la coma al dato de la 9 columna
  strcat(linea,irradiat[idIrradiat]);// añado el dato a la linea
  printf("%s\n",linea);
  strcat(linea,"\n");
  fputs(linea,f);
  op = menuAdd();

}while(op == 0);
  return 0;
}

/*----------------------------------------------- eliminarDuplicados ----
|  Función eliminarDuplicados
|
|  Objetivo:  Eliminacion de filas repetidos y guarda en un nuevo archivo
|
|  Parameteros:
|      FILE *f, FILE *g
|
|  Devuelve:  cont - cont2 - 1
|
*-------------------------------------------------------------------*/

int eliminarDuplicados(FILE* f, FILE* g){
  char lineas[300][100];
  char lineas2 [300][100];
  int cont = 0;
  while(!feof(f)){
    fscanf(f, "%s\n", lineas[cont]); //el contador empieza en 0 y cuando se le un numero aumenta el contador coincidiendo el nº de repetidas con el numero del contador, no con el indice de la lista
    cont++;
  }
  int cont2 = -1;
  for(int i = 0; i < cont; i++){
    int encontrado = -1;
    for(int j = i + 1; j < cont && encontrado==-1; j++){
      if(strcmp(lineas[i],lineas[j]) == 0){ //comparo el valor del indice i con el valor de las posiciones j
        encontrado = j;
      }
    }
    if(encontrado == -1){
      cont2++;
      strcpy(lineas2[cont2],lineas[i]); // copia el valor de lineas[i] a lineas2 en la posicion cont2
      //cont2++;
    }
  }
      //grabamos en el fichero 2 lineas2
  for(int i = 0; i < cont2; i++){
    fputs(lineas2[i],g);
  }
  return cont - cont2 - 1;
}

/*----------------------------------------------- calcularTamanoMedio ----
|  Función calcularTamanoMedio
|
|  Objetivo:  calcula  el  tamaño  medio  del  tumor  en  todo  el
|  fichero  de entrada
|
|  Parameteros:
|      struct CANCER lista[],int numFilas
|
|  Devuelve:  media
|
*-------------------------------------------------------------------*/

int calcularTamanoMedio(struct CANCER lista[],int numFilas){
	int suma=0;
	int media;
	for(int i=0;i<numFilas;i++){
		suma=suma+lista[i].tumorSizeMin+2;
	}
	// sale mal el primer dato, para buscar se puede comprobar solo la primera letra: n o r
	media = suma/numFilas;
	return media;
}

/*----------------------------------------------- cargarRegistros ----
|  Función cargarRegistros
|
|  Objetivo:  Carga un fichero de texto (entrada o sin filas repetidas) en el
|  vector de registros
|
|  Parameteros:
|      struct CANCER lista[],FILE *f
|
|  Devuelve:  i-1
|
*-------------------------------------------------------------------*/

int cargarRegistros(struct CANCER filas[],FILE *f){
	#define FORMATO2  "%30[^,],%d-%d,%10[^,],%d-%d,%d-%d,%4[^,],%d,%8[^,],%10[^,],%4[^\n]"
	int i = 0;
	fscanf(f, FORMATO2, filas[i].clase, &filas[i].ageMin, &filas[i].ageMax, filas[i].menopause, &filas[i].tumorSizeMin, &filas[i].tumorSizeMax, &filas[i].invNodesMin, &filas[i].invNodesMax, filas[i].invNodeCaps, &filas[i].degMalig, filas[i].breast, filas[i].breastQuad, filas[i].irradiat);
	quitarPrimerCaracter(filas[i].clase);
	i++;
  	while(!feof(f)) {
		fscanf(f, FORMATO2, filas[i].clase, &filas[i].ageMin, &filas[i].ageMax, filas[i].menopause, &filas[i].tumorSizeMin, &filas[i].tumorSizeMax, &filas[i].invNodesMin, &filas[i].invNodesMax, filas[i].invNodeCaps, &filas[i].degMalig, filas[i].breast, filas[i].breastQuad, filas[i].irradiat);
		quitarPrimerCaracter(filas[i].clase);
		i++;
  	}
	return i-1;
}

/*-----------------------------------------------quitarPrimerCaracter ----
|  Función quitarPrimerCaracter
|
|  Objetivo:  quita el primer caracter de la primera opcion ya que daba Error
|  en calcularTamanoMedio de este modo funciona
|
|  Parameteros:
|      char texto[]
|
|  Devuelve:  nada
|
*-------------------------------------------------------------------*/

void quitarPrimerCaracter(char texto[]){
	size_t longitud = strlen(texto);
   	for(int i=0;i<longitud;i++){
		*(texto+i)=*(texto+i+1);
	}
}

/*----------------------------------------------- actualizarRiesgo ----
|  Función actualizarRiesgo
|
|  Objetivo:  aquellas filas con alto riesgo de Cancer se mandan al fichero
|  binario de salida
|
|  Parameteros:
|      struct CANCER lista[], int media, int numFilas
|
|  Devuelve:  j
|
*-------------------------------------------------------------------*/

int actualizarRiesgo(struct CANCER lista[], int media, int numFilas){
	int j=0;
  struct CANCER lista2[300];
	for(int i=0;i<numFilas;i++){
		// hay un caracter de salto al principio de lista[].clase, quitado
		if(strcmp(lista[i].irradiat,"yes")==0 && lista[i].tumorSizeMin+2>media && lista[i].clase[0]=='r'){
			lista2[j]=copia(lista[i]);
			j++;
		}
	}
  FILE *f = fopen("altoRiesgo.txt", "wb");
  escribirBinario(lista, f, j);
  fclose(f);
	return j;
}

/*----------------------------------------------- actualizarRiesgo ----
|  Función actualizarRiesgo
|
|  Objetivo:  escribe una sola fila con alto riesgo en el fichero binario
|
|  Parameteros:
|      struct CANCER lista[], FILE *f, int numFilas
|
|  Devuelve:  Nada
|
*-------------------------------------------------------------------*/

void escribirBinario(struct CANCER lista[], FILE *f, int numFilas){
  if(f!=NULL){
    for(int i=0;i<numFilas;i++){
      fwrite(&lista[i], sizeof(struct CANCER), 1, f);
    }
  }else{
    printf("Error al grabar");
  }
}

/*----------------------------------------------- copia ----
|  Función copia
|
|  Objetivo:  copia cada dato para el archivo binario
|
|  Parameteros:
|      struct CANCER d
|
|  Devuelve:  r
|
*-------------------------------------------------------------------*/

struct CANCER copia(struct CANCER d){
	struct CANCER r;
	strcpy(r.clase, d.clase);
	r.ageMin = d.ageMin;
	r.ageMax = d.ageMax;
	strcpy(r.menopause,d.menopause);
	r.tumorSizeMin=d.tumorSizeMin;
	r.tumorSizeMax=d.tumorSizeMax;
	r.invNodesMin=d.invNodesMin;
	r.invNodesMax=d.invNodesMax;
	strcpy(r.invNodeCaps,d.invNodeCaps);
	r.degMalig=d.degMalig;
	strcpy(r.breast,d.breast);
	strcpy(r.breastQuad,d.breastQuad);
	strcpy(r.irradiat,d.irradiat);
	return r;
}
