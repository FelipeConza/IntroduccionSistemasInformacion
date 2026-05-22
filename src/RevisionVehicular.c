#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RevisionVehicular.h"

// Variable externa para saber qué usuario inició sesión
extern char usuarioActual[30];

// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

// Limpia el buffer de entrada para evitar problemas con fgets o scanf
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Valida que el usuario ingrese solo letras y espacios.
void validarCadena(const char *mensaje, char *destino, int tam) {
    int valido = 0;
    do {
        printf("%s", mensaje);
        fgets(destino, tam, stdin);
        destino[strcspn(destino, "\n")] = '\0';
        valido = 1;
        for (int i = 0; destino[i] != '\0'; i++) {
            if (!isalpha((unsigned char)destino[i]) && destino[i] != ' ') {
                valido = 0;
                printf("Entrada invalida. Solo letras permitidas.\n");
                break;
            }
        }
    } while (!valido);
}

// Valida que el usuario ingrese solo números y que tenga 4 dígitos
void validarNumero(const char *mensaje, char *destino, int tam) {
    int valido = 0;
    do {
        printf("%s", mensaje);
        fgets(destino, tam, stdin);
        destino[strcspn(destino, "\n")] = '\0';
        valido = 1;
        for (int i = 0; destino[i] != '\0'; i++) {
            if (!isdigit((unsigned char)destino[i])) {
                valido = 0;
                break;
            }
        }
        if (valido && strlen(destino) != 4) {
            valido = 0;
            printf("Entrada invalida. Debe tener exactamente 4 digitos.\n");
        } else if (!valido) {
            printf("Entrada invalida. Solo numeros permitidos.\n");
        }
    } while (!valido);
}

// ============================================================
// FUNCIONES DE REGISTRO DE VEHICULO
// ============================================================

// Permite registrar un vehículo, solicitando y validando todos sus datos
void validarMatricula(Vehiculo *v) {
    printf("\n--- MENU DE REGISTRO DE VEHICULO ---\n");

    // Asignar el usuario dueño automáticamente desde la sesión activa
    strcpy(v->usuario, usuarioActual);

    // ----------------------------
    // Validar placa: 3 letras + 4 números
    // ----------------------------
    int valido = 0;
    do {
        printf("Ingrese placa (3 letras y 4 numeros, ej: ABC1234): ");
        fgets(v->placa, sizeof(v->placa), stdin);
        v->placa[strcspn(v->placa, "\n")] = '\0';

        // Forzar mayúsculas en la placa
        for(int i = 0; v->placa[i] != '\0'; i++) v->placa[i] = toupper((unsigned char)v->placa[i]);

        if (strlen(v->placa) == 7) {
            valido = 1;
            for (int i = 0; i < 3; i++) {
                if (!isalpha((unsigned char)v->placa[i])) {
                    valido = 0;
                    break;
                }
            }
            for (int i = 3; i < 7; i++) {
                if (!isdigit((unsigned char)v->placa[i])) {
                    valido = 0;
                    break;
                }
            }
        }
        if (!valido) printf("Placa invalida. Debe tener 3 letras seguidas de 4 numeros.\n");
    } while (!valido);

    // ----------------------------
    // Validar cédula: exactamente 10 dígitos
    // ----------------------------
    valido = 0;
    char tempCedula[20];
    do {
        printf("Ingrese su cedula (10 digitos): ");
        fgets(tempCedula, sizeof(tempCedula), stdin);
        tempCedula[strcspn(tempCedula, "\n")] = '\0';

        if (strlen(tempCedula) == 10) {
            valido = 1;
            for (int i = 0; i < 10; i++) {
                if (!isdigit((unsigned char)tempCedula[i])) {
                    valido = 0;
                    break;
                }
            }
        }

        if (!valido) {
            printf("Cedula invalida. Debe tener exactamente 10 digitos numericos.\n");
        } else {
            strncpy(v->cedula, tempCedula, 10);
            v->cedula[10] = '\0';
        }
    } while (!valido);

    // ----------------------------
    // Validar año del vehículo: 4 dígitos entre 1940 y 2026
    // ----------------------------
    valido = 0;
    char tempAnio[20];
    int anioNumerico = 0;
    do {
        printf("Ingrese anio del vehiculo (4 digitos): ");
        fgets(tempAnio, sizeof(tempAnio), stdin);
        tempAnio[strcspn(tempAnio, "\n")] = '\0';

        if (strlen(tempAnio) == 4) {
            valido = 1;
            for (int i = 0; i < 4; i++) {
                if (!isdigit((unsigned char)tempAnio[i])) {
                    valido = 0;
                    break;
                }
            }
            if (valido) {
                anioNumerico = atoi(tempAnio);
                if (anioNumerico < 1940 || anioNumerico > 2026) {
                    valido = 0;
                    printf("Anio no valido, el limite esta entre 1940 y 2026\n");
                }
            }
        }
        if (!valido) {
            printf("Anio invalido. Debe tener exactamente 4 digitos numericos.\n");
        } else {
            strncpy(v->anio, tempAnio, 4);
            v->anio[4] = '\0';
        }
    } while (!valido);

    // ----------------------------
    // Seleccionar tipo de vehículo
    // ----------------------------
    printf("\nSeleccione el tipo de vehiculo:\n1. Pesado\n2. Liviano\n3. Moto\nDigite una opcion: ");
    char inputTipo[10];
    int opcionTipo = 0;
    do {
        fgets(inputTipo, sizeof(inputTipo), stdin);
        if (sscanf(inputTipo, "%d", &opcionTipo) != 1) {
            opcionTipo = 0;
        }
        if (opcionTipo >= 1 && opcionTipo <= 3) {
            switch (opcionTipo) {
                case 1: strcpy(v->tipo, "pesado"); break;
                case 2: strcpy(v->tipo, "liviano"); break;
                case 3: strcpy(v->tipo, "moto"); break;
            }
        } else {
            printf("Opcion invalida. Debe ser 1, 2 o 3.\n");
        }
    } while (opcionTipo < 1 || opcionTipo > 3);

    // ----------------------------
    // Validar avalúo: entre 4 y 6 dígitos
    // ----------------------------
    valido = 0;
    char tempAvaluo[20];
    do {
        printf("Ingrese avaluo del vehiculo (4 a 6 digitos): ");
        fgets(tempAvaluo, sizeof(tempAvaluo), stdin);
        tempAvaluo[strcspn(tempAvaluo, "\n")] = '\0';
        int longitud = strlen(tempAvaluo);
        valido = 1;
        for (int i = 0; i < longitud; i++) {
            if (!isdigit((unsigned char)tempAvaluo[i])) {
                valido = 0;
                break;
            }
        }
        if (longitud < 4 || longitud > 6) {
            valido = 0;
        }
        if (!valido) {
            printf("Cantidad invalida, ingrese un valor de entre 4 a 6 cifras numericas\n");
        } else {
            strncpy(v->avaluo, tempAvaluo, sizeof(v->avaluo));
            v->avaluo[sizeof(v->avaluo) - 1] = '\0';
        }
    } while (!valido);

    // Inicializar revisiones en 0 (no aprobadas)
    for (int i = 0; i < NUM_REVISIONES; i++) {
        v->revisiones[i] = 0;
    }

    printf("Vehiculo registrado correctamente.\n");

    // Pregunta al usuario si desea generar comprobante en TXT
    char opcionComprobante[10];
    printf("¿Desea imprimir el comprobante de registro? (si/no): ");
    fgets(opcionComprobante, sizeof(opcionComprobante), stdin);
    opcionComprobante[strcspn(opcionComprobante, "\n")] = '\0';
    if (strcasecmp(opcionComprobante, "si") == 0) {
        generarComprobanteTXT(*v);
    }
}

// ============================================================
// FUNCIONES DE COMPROBANTE
// ============================================================

void generarComprobanteTXT(const Vehiculo v) {
    FILE *archivo = fopen("data/comprobante_vehicular.txt", "w"); // Ruta data/
    if (archivo == NULL) {
        printf("Error al crear el archivo de comprobante.\n");
        return;
    }
    fprintf(archivo, "====== COMPROBANTE DE REGISTRO VEHICULAR ======\n");
    fprintf(archivo, "Duenio (Usuario): %s\n", v.usuario);
    fprintf(archivo, "Placa: %s\n", v.placa);
    fprintf(archivo, "Cedula: %s\n", v.cedula);
    fprintf(archivo, "Anio: %s\n", v.anio);
    fprintf(archivo, "Tipo: %s\n", v.tipo);
    fprintf(archivo, "Avaluo: %s\n", v.avaluo);
    fprintf(archivo, "=============================================\n");
    fclose(archivo);
    system("notepad data/comprobante_vehicular.txt"); 
}

// Busca directamente en el archivo TXT para soportar bases de datos gigantescas de Big Data
int buscarVehiculoPorPlaca(const Vehiculo vehiculos[], int cantidad, const char* placaBuscar) {
    FILE *f = fopen("data/vehiculos.txt", "r"); // Ruta data/
    if (f == NULL) {
        return -1;
    }

    char linea[250];
    char user[60], placa[15], cedula[15], anio[10], tipo[20], avaluo[15];
    int r1, r2, r3;
    int index = 0;

    char placaUpper[15];
    strcpy(placaUpper, placaBuscar);
    for(int i=0; placaUpper[i]; i++) placaUpper[i] = toupper((unsigned char)placaUpper[i]);

    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = '\0';
        if (sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d",
                   user, placa, cedula, anio, tipo, avaluo, &r1, &r2, &r3) == 9) {
            
            if (strcmp(placa, placaUpper) == 0) {
                printf("\n--- DATOS DEL VEHICULO ENCONTRADO ---\n");
                printf("Duenio: %s\n", user);
                printf("Placa: %s\n", placa);
                printf("Cedula: %s\n", cedula);
                printf("Anio: %s\n", anio);
                printf("Tipo: %s\n", tipo);
                printf("Avaluo: %s\n", avaluo);
                printf("Revisiones: [%s] [%s] [%s]\n", 
                       r1 ? "Aprobada" : "No aprobada",
                       r2 ? "Aprobada" : "No aprobada",
                       r3 ? "Aprobada" : "No aprobada");
                fclose(f);
                return index; // Retorna una posición simulada válida
            }
        }
        index++;
    }

    fclose(f);
    return -1; 
}

// Lista el estado de revisiones del vehículo
void mostrarEstadoRevisiones(const Vehiculo v) {
    printf("\n--- ESTADO DE REVISIONES DEL VEHICULO %s ---\n", v.placa);
    for (int i = 0; i < NUM_REVISIONES; i++) {
        printf("Revision %d: %s\n", i + 1, v.revisiones[i] ? "Aprobada" : "No aprobada");
    }
}

// Permite registrar las revisiones técnicas
void registrarRevisiones(int revisiones[NUM_REVISIONES]) {
    printf("\n--- REGISTRO DE REVISIONES TECNICAS ---\n");
    for (int i = 0; i < NUM_REVISIONES; i++) {
        printf("Revision %d (1=Aprobado, 0=No aprobado): ", i + 1);
        if (scanf("%d", &revisiones[i]) != 1) {
            revisiones[i] = 0;
        }
        limpiarBuffer();
    }
    printf("Revisiones registradas internamente de manera correcta.\n");
}

// Muestra de manera secuencial todos los vehículos que pertenecen al archivo físico y pasaron revisión
void listarVehiculos(const Vehiculo vehiculos[], int cantidad) {
    FILE *f = fopen("data/vehiculos.txt", "r"); // Ruta data/
    if (f == NULL) {
        printf("No existen vehiculos registrados en el archivo.\n");
        return;
    }

    char linea[250];
    char user[60], placa[15], cedula[15], anio[10], tipo[20], avaluo[15];
    int r1, r2, r3;
    int contador = 1;

    printf("\n--- LISTADO DE VEHICULOS MATRICULADOS OFICIALES ---\n");
    while (fgets(linea, sizeof(linea), f)) {
        linea[strcspn(linea, "\n")] = '\0';
        if (sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d",
                   user, placa, cedula, anio, tipo, avaluo, &r1, &r2, &r3) == 9) {
            
            // FILTRO EXIGIDO POR TU PROFESOR: Solo muestra si pasó alguna revisión (r1, r2 o r3)
            if (r1 == 1 || r2 == 1 || r3 == 1) {
                printf("\nVehiculo %d:\n", contador++);
                printf("  Usuario asignado: %s\n", user);
                printf("  Placa: %s\n", placa);
                printf("  Cedula: %s\n", cedula);
                printf("  Anio: %s\n", anio);
                printf("  Tipo: %s\n", tipo);
                printf("  Avaluo: %s\n", avaluo);
                printf("  Revisiones: %d, %d, %d\n", r1, r2, r3);
            }
        }
    }
    printf("\n[INFO] Total de vehiculos matriculados listados: %d\n", contador - 1);
    fclose(f);
}

// FUNCION DE CALCULO DE MATRICULA
float calcular_matricula_vehicular(const char tipo_vehiculo[], int cilindraje, int anio_fabricacion,
                                   const char provincia[], float es_ecologico, float revision_obligatoria,
                                   float multas_transito, int mes_pago, int ultimo_digito_placa) {
    float base = 0.0f;
    if (strcmp(tipo_vehiculo, "pesado") == 0) {
        base = 200.0f;
    } else if (strcmp(tipo_vehiculo, "liviano") == 0) {
        base = 25.0f;
    } else if (strcmp(tipo_vehiculo, "moto") == 0) {
        base = 25.0f;
    }

    float antiguedad = (2026 - anio_fabricacion) * 5.0f;
    float total = base + antiguedad + multas_transito;

    if (revision_obligatoria == 0) {
        total += 50.0f; 
    }

    return total;
}

// GUARDAR VEHICULOS EN ARCHIVO 
void guardarVehiculos(const Vehiculo vehiculos[], int cantidad) {
    FILE *f = fopen("data/vehiculos.txt", "a"); // Ruta data/
    if (f == NULL) {
        printf("Error al abrir el archivo para guardar.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        fprintf(f, "%s,%s,%s,%s,%s,%s,%d,%d,%d\n",
            strlen(vehiculos[i].usuario) > 0 ? vehiculos[i].usuario : usuarioActual,
            vehiculos[i].placa,
            vehiculos[i].cedula,
            vehiculos[i].anio,
            vehiculos[i].tipo,
            vehiculos[i].avaluo,
            vehiculos[i].revisiones[0],
            vehiculos[i].revisiones[1],
            vehiculos[i].revisiones[2]
        );
    }
    fclose(f);
}

// CARGAR VEHICULOS DESDE ARCHIVO
void cargarVehiculos(Vehiculo vehiculos[], int *cantidad) {
    FILE *f = fopen("data/vehiculos.txt", "r"); // Ruta data/
    if (f == NULL) {
        *cantidad = 0;
        return;
    }

    char linea[250];
    *cantidad = 0;

    while (fgets(linea, sizeof(linea), f) && *cantidad < MAX_VEHICULOS) {
        linea[strcspn(linea, "\n")] = '\0';
        if (sscanf(linea, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d,%d",
                   vehiculos[*cantidad].usuario,
                   vehiculos[*cantidad].placa,
                   vehiculos[*cantidad].cedula,
                   vehiculos[*cantidad].anio,
                   vehiculos[*cantidad].tipo,
                   vehiculos[*cantidad].avaluo,
                   &vehiculos[*cantidad].revisiones[0],
                   &vehiculos[*cantidad].revisiones[1],
                   &vehiculos[*cantidad].revisiones[2]) == 9) {
            (*cantidad)++;
        }
    }
    fclose(f);
}