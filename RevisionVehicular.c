#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "RevisionVehicular.h"

// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

// Limpia el buffer de entrada para evitar problemas con fgets o scanf
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Valida que el usuario ingrese solo letras y espacios.
// Se usa para nombres o cadenas alfabéticas.
void validarCadena(const char *mensaje, char *destino, int tam) {
    int valido = 0;
    do {
        printf("%s", mensaje);
        fgets(destino, tam, stdin);                       // Lee la entrada del usuario
        destino[strcspn(destino, "\n")] = '\0';          // Elimina el salto de línea
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

    // ----------------------------
    // Validar placa: 3 letras + 4 números
    // ----------------------------
    int valido = 0;
    do {
        printf("Ingrese placa (3 letras y 4 numeros, ej: ABC1234): ");
        fgets(v->placa, sizeof(v->placa), stdin);
        v->placa[strcspn(v->placa, "\n")] = '\0';

        if (strlen(v->placa) == 7) {
            valido = 1;
            // Validar las 3 primeras letras
            for (int i = 0; i < 3; i++) {
                if (!isalpha((unsigned char)v->placa[i])) {
                    valido = 0;
                    break;
                }
            }
            // Validar los 4 números
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
    // Validar año del vehículo: 4 dígitos entre 1940 y 2025
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
                if (anioNumerico < 1940 || anioNumerico > 2025) {
                    valido = 0;
                    printf("Anio no valido, el límite está entre 1940 y 2025\n");
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
    FILE *archivo = fopen("comprobante_vehicular.txt", "w");
    if (archivo == NULL) {
        printf("Error al crear el archivo de comprobante.\n");
        return;
    }
    fprintf(archivo, "====== COMPROBANTE DE REGISTRO VEHICULAR ======\n");
    fprintf(archivo, "Placa: %s\n", v.placa);
    fprintf(archivo, "Cedula: %s\n", v.cedula);
    fprintf(archivo, "Anio: %s\n", v.anio);
    fprintf(archivo, "Tipo: %s\n", v.tipo);
    fprintf(archivo, "Avaluo: %s\n", v.avaluo);
    fprintf(archivo, "=============================================\n");
    fprintf(archivo, "Gracias por registrar su vehiculo. Este comprobante\n");
    fprintf(archivo, "constituye constancia oficial de inscripcion.\n");
    fclose(archivo);
    system("notepad comprobante_vehicular.txt"); // Abre el comprobante en el bloc de notas
}

// ============================================================
// FUNCIONES DE BÚSQUEDA Y LISTADO
// ============================================================

// Busca un vehículo por su placa en un arreglo de vehículos
// Retorna el índice si lo encuentra, -1 si no
int buscarVehiculoPorPlaca(const Vehiculo vehiculos[], int cantidad, const char* placa) {
    for (int i = 0; i < cantidad; i++) {
        if (strcmp(vehiculos[i].placa, placa) == 0) {
            return i;
        }
    }
    return -1;
}

// Lista el estado de revisiones técnicas del vehículo
void mostrarEstadoRevisiones(const Vehiculo v) {
    printf("\n--- ESTADO DE REVISIONES DEL VEHICULO %s ---\n", v.placa);
    for (int i = 0; i < NUM_REVISIONES; i++) {
        printf("Revision %d: %s\n", i + 1, v.revisiones[i] ? "Aprobada" : "No aprobada");
    }
}

// Permite registrar manualmente las revisiones técnicas (aprobadas o no)
void registrarRevisiones(int revisiones[NUM_REVISIONES]) {
    printf("\n--- REGISTRO DE REVISIONES TECNICAS ---\n");
    for (int i = 0; i < NUM_REVISIONES; i++) {
        printf("Revision %d (1=Aprobado, 0=No aprobado): ", i + 1);
        scanf("%d", &revisiones[i]);
        limpiarBuffer();
    }
    printf("Revisiones registradas correctamente.\n");
}

// Muestra todos los vehículos registrados en el arreglo
void listarVehiculos(const Vehiculo vehiculos[], int cantidad) {
    printf("\n--- LISTADO DE VEHICULOS MATRICULADOS ---\n");
    for (int i = 0; i < cantidad; i++) {
        printf("\nVehiculo %d:\n", i + 1);
        printf("Placa: %s\n", vehiculos[i].placa);
        printf("Cedula: %s\n", vehiculos[i].cedula);
        printf("Anio: %s\n", vehiculos[i].anio);
        printf("Tipo: %s\n", vehiculos[i].tipo);
        printf("Avaluo: %s\n", vehiculos[i].avaluo);
    }
}

// ============================================================
// FUNCION DE CALCULO DE MATRICULA
// ============================================================

// Calcula el valor de matrícula del vehículo considerando tipo, antigüedad y multas
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

    float antiguedad = (2025 - anio_fabricacion) * 5.0f;
    float total = base + antiguedad + multas_transito;

    if (revision_obligatoria == 0) {
        total += 50.0f; // Costo extra si no tiene revisión obligatoria
    }

    return total;
}

// ============================================================
// GUARDAR VEHICULOS EN ARCHIVO
// ============================================================

void guardarVehiculos(const Vehiculo vehiculos[], int cantidad) {

    FILE *f = fopen("vehiculos.txt", "w");

    if (f == NULL) {
        printf("Error al guardar vehiculos.\n");
        return;
    }

    for (int i = 0; i < cantidad; i++) {

        fprintf(f,
            "%s,%s,%s,%s,%s,%s,%d,%d,%d\n",

            vehiculos[i].usuario,
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

// ============================================================
// CARGAR VEHICULOS DESDE ARCHIVO
// ============================================================

void cargarVehiculos(Vehiculo vehiculos[], int *cantidad) {

    FILE *f = fopen("vehiculos.txt", "r");

    if (f == NULL) {
        return;
    }

    *cantidad = 0;

    while (fscanf(f,
        "%29[^,],%7[^,],%10[^,],%4[^,],%19[^,],%9[^,],%d,%d,%d\n",

        vehiculos[*cantidad].usuario,
        vehiculos[*cantidad].placa,
        vehiculos[*cantidad].cedula,
        vehiculos[*cantidad].anio,
        vehiculos[*cantidad].tipo,
        vehiculos[*cantidad].avaluo,

        &vehiculos[*cantidad].revisiones[0],
        &vehiculos[*cantidad].revisiones[1],
        &vehiculos[*cantidad].revisiones[2]

    ) == 9) {

        (*cantidad)++;

        if (*cantidad >= MAX_VEHICULOS) {
            break;
        }
    }

    fclose(f);
}
