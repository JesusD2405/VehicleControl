#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

				 /* ESTRUCTURA ARBOL */

typedef struct arbol
{
	char placa[7];
	char cedula[40];
	char nombre[40];
	char marca [40];
	char modelo[40];
	char anno[5];
	char color[15];
	char serial[15];
	char correo[40];

	struct arbol *izq;
	struct arbol *der;

}ARBOL;

				/* DECLARACION DE FUNCIONES */

void gotoxy(short x, short y);
void cargar_barra();                      		// Carga la Barra
void menu ();                             		// Menu Principal
ARBOL* crear_nodo();                      		// Funcion que crea un nodo
void agregar(ARBOL* &p);
char placa(char *cadena);                		// Convierte Las Letras Minusculas en Mayusculas
bool verificar(ARBOL* &p, char *cadena);  		// Verifica Que La Placa Ingresada No Exista En El Registro
bool insertar(ARBOL* &p, char *placa);  		// Inserta Un Nodo Al Arbol
void pedir_datos(ARBOL* &p, char *placa);       // Pide Los Datos Para el Registro
bool verificar_correo(char *correo);			// Verifica Que Tenga EL Signo Arroba "@"
void mostrar_registro(ARBOL *p);				// Se Muestra El Registro De las Placas En El Arbol En Postorden
void borrar_registro (ARBOL* &p);               // Borra Todo El Arbol
ARBOL* eliminar (ARBOL* &p);					    // Se Verifica Que Registro Se Va A Eliminar
ARBOL* eliminar_registro(ARBOL* &p, char *placa);	// Se Elimina El Registro (NODO) Del Arbol
char guardar_registro(FILE *registro, ARBOL* &p);	// Guardamos El Arbol En El Archivo
bool buscar(ARBOL* &p, char *dato, int resp);  		// Busca El Parámetro Ingresado En El Arbol
void menu_buscar(ARBOL* &p);						// Sub-Menú De Busquedad, Se Solicita El Parametro Específico A Buscar
void mostrar_datos(ARBOL *p);						// Muestra Los Datos Del Registro
void modificar(ARBOL* &p);							// Verifica Que La Placa Exista, Para Modificar El Registro En El Nodo
ARBOL* modificar_datos(ARBOL* &p, char *placa);		// Busca La Placa En El Árbol Y Modifica El Registro En El Nodo
void cargar_archivo (FILE *registro, ARBOL* &p, char *placa);   // Carga El Archivo Al Arbol

				/* FUNCIONES */

void gotoxy(short x, short y)
{
	COORD a;
	a.X=x;
	a.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

void cargar_barra()
{
	int i;

	gotoxy(37,10);
	printf("Cargando..");

	/* Esquinas Superiores */
	gotoxy(29,11);
	printf("\xc9");
	gotoxy(53,11);
	printf("\xbb");

	/* Lineas Verticales */
	gotoxy(29,12);
	printf("\xba");
	gotoxy(53,12);
	printf("\xba");

	/* Esquinas Inferiores */
	gotoxy(29,13);
	printf("\xc8");
	gotoxy(53,13);
	printf("\xbc");

	/*Bordes Horizontales, Superior e Inferior*/
	for(i=30; i<53; i++)
	{
		gotoxy(i,11);
		printf("\xcd");
		gotoxy(i,13);
		printf("\xcd");
	}

	/* Barra */
	for(i=30; i<53; i++)
	{
		gotoxy(i,12);
		printf("%c", 177);
		Sleep(500);
	}

	fflush(stdin); // Para Vaciar El Bufer Del Teclado
	gotoxy(24,10); printf("El Registro Se Cargo Completamente!");
	gotoxy(20,20); getch();
}

void menu ()
{
	ARBOL *p= NULL;
	FILE *registro;
	int resp;
	char placa[7];

	cargar_archivo(registro, p, placa);

	do
	{
		system("cls");
		fflush(stdin);

		printf("\t\t Registro Y Control De Vehiculos \n");
		printf("\n\t Menu: \n\n");
		printf("\t 1. Agregar \n");
		printf("\t 2. Buscar \n");
		printf("\t 3. Modificar \n");
		printf("\t 4. Eliminar \n");
		printf("\t 5. Mostrar Registro \n");
		printf("\t 6. Borrar Registro \n");
		printf("\t 7. Salir \n");
		printf("\t Opcion: ");
		resp=getch();

		switch(resp)
		{
		case '1':
			agregar(p);                 // Agregar Registro
			break;
		case '2':
			menu_buscar(p);             // Buscar Registro
			break;
		case '3':
			modificar(p);               // Modificar Registro
			break;
		case '4':
			eliminar(p);                // Eliminar Registro
			break;
		case '5':
			system("cls");
			fflush (stdin);
            printf("\t Placa Del Vehiculo: ");
            printf("\t\t Nombre Del Propietario: \n\n");
			mostrar_registro(p);  	    // Mostrar Registro
			getch();
			break;
		case '6':
			borrar_registro(p); 	    // Borrar Registro
			break;
		case '7':
			guardar_registro(registro, p);
			resp= 0;                    // Salir
			break;
		default:
			fflush(stdin);
			printf("\t Ha Introducido Una Opcion Invalida!");
			getch();
			break;
		}
	}
	while(resp!=0);
}

ARBOL* crear_nodo()
{
	ARBOL *nuevo;

	nuevo= (ARBOL*) malloc(sizeof(ARBOL));

	if (nuevo)
	{
		strcpy (nuevo->placa," ");
		strcpy (nuevo->cedula," ");
		strcpy (nuevo->nombre," ");
		strcpy (nuevo->marca," ");
		strcpy (nuevo->modelo," ");
		strcpy (nuevo->color," ");
		strcpy (nuevo->serial," ");
		strcpy (nuevo->correo," ");
		strcpy (nuevo->anno," ");

		nuevo->izq= NULL;
		nuevo->der= NULL;
	}

	return nuevo;
}

void agregar(ARBOL* &p)
{
	char cadena[7];

	system("cls");

	printf("\t Ingrese La Placa Del Vehiculo: \n\t");
	fgets (cadena,7,stdin);

	placa(cadena);

	if (verificar(p, cadena)==false)
		insertar (p, cadena);
	else
	{
		fflush(stdin);
		gotoxy(5,1); printf("%c", 158);
		printf("\n\n\t La Placa Del Vehiculo Ya Existe.. No se puede Agregar!\n");
		getch();
	}
}

char placa(char *cadena)
{
	if (*cadena=='\0')
		return 0;

	if (*cadena>= 97 && *cadena<= 122)   // Si Hay Una Letra Minuscula La Convierte En MayÃºscula
		*cadena= toupper(*cadena);

	return placa(++cadena);
}

bool verificar (ARBOL* &p, char *cadena)
{
	if (!p)
		return false;

	if (strcmp(p->placa,cadena)==0) // Si Las Placas Son Iguales
		return true;

	if (*cadena<=p->placa[0])
		return verificar(p->izq, cadena);
	else
		return verificar(p->der, cadena);
}

bool insertar (ARBOL* &p, char *placa)
{
	if (!p)
	{
		p= crear_nodo();

		if (p)
		{
			pedir_datos (p, placa);
			return true;
		}
		else
			return false;
	}

	if (*placa<=p->placa[0])
		return insertar (p->izq, placa);
	else
		return insertar (p->der, placa);
}

void pedir_datos (ARBOL* &p, char *placa)
{
	FILE *registro;
	bool correo= false;

	system ("cls");

	strcpy (p->placa,placa); // Guardamos La Placa Del Vehiculo

	printf("\n\t *Datos Personales*\n\n");

	printf("\t Ingrese La Cedula: ");
	fgets(p->cedula,40,stdin);

	printf("\n\t Ingrese El Nombre: ");
	fgets(p->nombre,40,stdin);

	printf("\n\t Ingrese El Correo Electronico: ");

	do
	{
		gotoxy(40,7); fgets(p->correo,40,stdin);

		if(verificar_correo(p->correo)==false)  // Se Verifica Que Tenga El Signo Arroba "@"
		{
			gotoxy(40,7); printf(" Debe tener el signo arroba (@) ");
			getch();
			gotoxy(40,7); printf("                                      ");
		}
		else
			correo= true;

	}
	while(correo==false);

	printf("\n\n\t *Caracteristicas Del Vehiculo*\n\n");

	printf("\t Ingrese La Marca: ");
	fgets(p->marca,40,stdin);

	printf("\n\t Ingrese El Modelo: ");
	fgets(p->modelo,40,stdin);

	printf("\n\t Ingrese El Anno: ");
	fgets(p->anno,5,stdin);

    fflush(stdin);
	printf("\n\t Ingrese El Color: ");
	fgets(p->color,15,stdin);

	printf("\n\t Ingrese El Serial Del Motor: ");
	fgets(p->serial,15,stdin);
}

bool verificar_correo(char *correo)
{
	if(*correo=='\0')
		return false;

	if(*correo==64)             // Si Tiene El Signo Arroba "@"
		return true;

	verificar_correo(++correo);
}

void mostrar_registro(ARBOL *p)
{
	if (!p)
		return;

	mostrar_registro (p->izq);

	printf("%s---------> %s",p->placa,p->nombre);

	mostrar_registro(p->der);
}

void borrar_registro (ARBOL* &p)
{
	if (!p)
	{
		system("cls");
		fflush(stdin);
		printf("\t Registro Borrado..\n");
		getch();
		return;
	}

	borrar_registro (p->izq);

	borrar_registro(p->der);

	free(p);
}

ARBOL* eliminar (ARBOL* &p)
{
	char placa[10];
	system ("cls");

	printf("\t\t *Eliminar Registro* \n\n");

	fflush(stdin);
	printf("\t Ingrese La Placa Del Vehiculo: ");
	gets(placa);

	if (verificar(p, placa)==true){ // Si La Placa Existe
		// Se Muestra El Registro Y Se Verifica Si Se Desea Eliminar
		eliminar_registro(p, placa);
		return p;
	}
	else
	{
		fflush(stdin);
		printf("\n\n La Placa No Existe En El Registro..! \n");
		getch();
		return 0;
	}
}

ARBOL* eliminar_registro (ARBOL* &p, char *placa)
{
	ARBOL *aux;

	if (!p)
		return 0;

    if (strcmp(p->placa, placa)==0){ //Si Las Placas Son Iguales
       if (!p->izq && !p->der){ 	//Si El Nodo Es Una Hoja
          free(p);
          return 0;
       }
       if (!p->izq){ //SI EL SUBARBOL IZQUIERDO ESTÁ VACÍO
          aux= p->der; //SE GUARDA EL DERECHO
          free(p);
          return aux;
       }
       if (!p->der){ //SI EL SUBARBOL DERECH ESTÁ VACÍO
          aux= p->izq;
          free(p);
       return aux;
       }

	if (*placa<=p->placa[0])
		return eliminar_registro(p, placa);
	else
		return eliminar_registro(p, placa);
  }
}

char guardar_registro(FILE *registro, ARBOL* &p)
{
	if (p){

		registro= fopen("Registro De Vehiculos.dat","wb+");


		fwrite(p->placa, sizeof p->placa, 1, registro);
		fwrite(p->cedula, sizeof p->cedula, 1, registro);
		fwrite(p->nombre, sizeof p->nombre, 1, registro);
		fwrite(p->marca, sizeof p->marca, 1, registro);
		fwrite(p->modelo, sizeof p->modelo, 1, registro);
		fwrite(p->anno, sizeof p->anno, 1, registro);
		fwrite(p->color, sizeof p->color, 1, registro);
		fwrite(p->serial, sizeof p->serial, 1, registro);
		fwrite(p->correo, sizeof p->correo, 1, registro);

		guardar_registro(registro, p->izq);
		guardar_registro(registro, p->der);

    	fclose(registro);
    }else{
    	return 0;
    }
}

void menu_buscar(ARBOL* &p){

int resp; // Respuesta Y Busquedad
char dato [40], cadena [40];
bool correo;

 do{
        system("cls");
        fflush(stdin);

        printf("\t\t Buscar por: \n\n");
        printf("\t 1. Placa del vehiculo \n");
        printf("\t 2. Cedula del propietario \n");
        printf("\t 3. Nombre del propietario \n");
        printf("\t 4. Marca del vehiculo \n");
        printf("\t 5. Modelo del vehiculo \n");
        printf("\t 6. Anno del vehiculo \n");
        printf("\t 7. Color del vehiculo \n");
        printf("\t 8. Serial del motor \n");
        printf("\t 9. Direccion de email \n");
        printf("\t x. Abandonar busqueda \n");
        printf("\t Opcion: ");
        resp=getch();
    	system("cls");

    switch(resp){
            case '1':  printf("\t Ingrese placa: ");     // Buscar por  placa
                       fgets(dato,7, stdin);
                       strcpy(cadena,dato);
                       placa (cadena);
                       buscar(p, cadena, resp);
                break;
            case '2':  printf("\t Ingrese cedula: ");   // Buscar por cedula del propietario
                       fgets(dato,40, stdin);
                       buscar(p, dato, resp);
                break;
            case '3':  printf("\t Ingrese nombre: ");   // Buscar por Nombre de propietario
                      fgets(dato,40, stdin);
                      buscar(p, dato, resp);
		        break;
            case '4':  printf("\t Ingrese marca: ");   // Buscar por Marca del vehiculo
                      fgets(dato,40, stdin);
                      buscar(p, dato, resp);
                break;
            case '5':  printf("\t Ingrese modelo: ");   // Buscar por Modelo del vehiculo
                      fgets(dato,40, stdin);
                      buscar(p, dato, resp);
                break;
            case '6':  printf("\t Ingrese anno: ");   // Buscar por Anno del vehiculo
                      fgets(dato,5, stdin);
                      buscar(p, dato, resp);
                break;
            case '7':  printf("\t Ingrese color: ");    // Buscar por Color del vehiculo
                       fgets(dato,10, stdin);
                       buscar(p, dato, resp);
                break;
            case '8':  printf("\t Ingrese serial: ");    // Buscar por Serial de motor
                       fgets(dato,15, stdin);
                       buscar(p, dato, resp);
                break;
            case '9':  printf("\t Ingrese correo: ");    // Buscar por Direccion de e-mail
						do
						{
							gotoxy(25,0); fgets(dato,40,stdin);

								if(verificar_correo(dato)==false)  // Se Verifica Que Tenga El Signo Arroba "@"
								{
									gotoxy(25,0); printf(" Debe tener el signo arroba (@) ");
									getch();
									gotoxy(25,0); printf("                                      ");
								}
								else
								correo= true;

						}while(correo==false);

                      buscar(p, dato, resp);
                break;
            case 'x':  resp= 0; // Salir
                break;
            default:  fflush(stdin);
                      printf("\t Ha Introducido Una Opcion Invalida!");
                      getch();
			break;
    }

  }while(resp!=0);
}


bool buscar(ARBOL* &p, char *dato, int resp){

  if(!p)
    return false;

    switch(resp){
            case '1': if(strcmp(p->placa, dato)==0)      // Buscar por  placa
                      {
                      mostrar_datos(p);
                      return true;
                      }
                break;
            case '2': if(strcmp(p->cedula, dato)==0)    // Buscar por cedula del propietario
                      {
                      mostrar_datos(p);
                      return true;
                      }
                break;
            case '3': if(strcmp(p->nombre, dato)==0)    // Buscar por Nombre de propietario
                      {
                      mostrar_datos(p);
                      }
                break;
            case '4':if( strcmp(p->marca, dato)==0)    // Buscar por Marca del vehiculo
                      {
                      mostrar_datos(p);
                      }
                break;
            case '5': if(strcmp(p->modelo, dato)==0)    // Buscar por Modelo del vehiculo
                      {
                      mostrar_datos(p);
                      }
                break;
            case '6': if(strcmp(p->anno, dato)==0)    // Buscar por Anno del vehiculo
                      {
                      mostrar_datos(p);
                      }
                break;
            case '7': if(strcmp(p->color, dato)==0)    // Buscar por Color del vehiculo
                      {
                      mostrar_datos(p);
                      }
                break;
            case '8': if(strcmp(p->serial, dato)==0)    // Buscar por Serial de motor
                      {
                      mostrar_datos(p);
                      return true;
                      }
                break;
            case '9': if(strcmp(p->correo, dato)==0)    // Buscar por Direccion de e-mail
                      {
                      mostrar_datos(p);
                      return true;
                      }
                break;
    }

	if(p->izq)
     return buscar(p->izq, dato, resp);

	if(p->der)
    return buscar(p->der, dato, resp);
}

void mostrar_datos(ARBOL *p)
{
  system("cls");
  printf("\t|------------------------------------------------------|\n");
  printf("\t\t .-*Informacion Del Registro*-. \n\n");

  printf("\n\t\t *Datos Del Propietario Del Vehiculo*\n\n");

  printf("\t Cedula: %s \n", p->cedula);

  printf("\t Nombre: %s \n", p->nombre);

  printf("\t Correo Electronico: %s \n", p->correo);
  printf("\t|------------------------------------------------------|\n");
  printf("\n\t\t *Caracteristicas Del Vehiculo*\n\n");

  printf("\t Marca: %s \n", p->marca);

  printf("\t Modelo: %s \n", p->modelo);

  printf("\t Anno: %s \n \n", p->anno);

  printf("\t Color: %s \n", p->color);

  printf("\t El Serial Del Motor: %s", p->serial);
  printf("\t|------------------------------------------------------|\n");

  fflush(stdin);
  getch();
}

void modificar(ARBOL* &p){

	char cadena[10];

	system("cls");
	printf("\t Ingrese La Placa Del Vehiculo: \n\t");
	fgets(cadena,10,stdin);

    placa(cadena);

	if(verificar(p,cadena)==true)
        modificar_datos(p, cadena);
	else
	{
		fflush(stdin);
		gotoxy(5,1); printf("%c", 158);
		printf("\n\n\t La Placa Del Vehiculo No Existe.! No se puede Modificar!\n");
		getch();
	}
}

ARBOL* modificar_datos(ARBOL* &p, char *placa){

	if (!p)
		return 0;

	if (strcmp(p->placa,placa)==0) // Si Las Placas Son Iguales
		pedir_datos(p,placa);

	if (*placa<=p->placa[0])
		return modificar_datos(p->izq, placa);
	else
		return modificar_datos(p->der, placa);
}

void cargar_archivo (FILE *registro, ARBOL* &p, char *placa){

	registro= fopen("Registro De Vehiculos.dat","rb");
	rewind (registro);

	if (registro)
	{
		while(!feof(registro)){

        	p= crear_nodo();

        	fread(&placa, sizeof placa, 1, registro);

        	if(verificar(p, placa))
        	{
        		strcpy(p->placa, placa);
            	fread(&p->cedula, sizeof p->cedula, 1, registro);
				fread(&p->nombre, sizeof p->nombre, 1, registro);
				fread(&p->marca, sizeof p->marca, 1, registro);
				fread(&p->modelo, sizeof p->modelo, 1, registro);
 				fread(&p->anno, sizeof p->anno, 1, registro);
				fread(&p->color, sizeof p->color, 1, registro);
				fread(&p->serial, sizeof p->serial, 1, registro);
				fread(&p->correo, sizeof p->correo, 1, registro);

    		}
    	fclose(registro);
		}
	}
	else
		return;
}

int main()
{
	system ("Color 3F");
	cargar_barra();
	menu();

	return 0;
}
