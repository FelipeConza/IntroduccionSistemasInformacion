#include <stdio.h>      // Librería para entrada y salida estándar (printf, scanf, fgets, etc.)
#include <stdlib.h>     // Librería para funciones de utilidad general (exit, atoi, malloc, etc.)
#include <string.h>     // Librería para manejo de cadenas (strlen, strcpy, strcmp, etc.)
#include <ctype.h>      // Librería para funciones de manipulación de caracteres (isdigit, toupper, etc.)
#include "RevisionVehicular.h" // Archivo de cabecera propio que contiene estructuras y funciones relacionadas con vehículos

// Declaración de función para mostrar la pantalla de login
int pantallaLogin();

// Arreglo global de vehículos y contador de cantidad de vehículos registrados
Vehiculo vehiculos[MAX_VEHICULOS];
int cantidadVehiculos = 0;

// Declaración de funciones principales del programa
void menuPrincipal();
void mostrarVehiculo(const Vehiculo v);

//Esto hace que cada vehículo pertenezca a un usuario
char usuarioActual[30];

int main() {
    cargarVehiculos(vehiculos, &cantidadVehiculos);
    
    // Bucle infinito para que el programa siga corriendo hasta que se salga explícitamente
    while (1) {
        // Si el login es exitoso, se llama al menú principal
        if (pantallaLogin()) {
            menuPrincipal();
        }
    }
    return 0; 
}

// Función para mostrar los datos de un vehículo específico
void mostrarVehiculo(const Vehiculo v) {
    printf("\n--- DATOS DEL VEHICULO ---\n");
    printf("Placa: %s\n", v.placa);
    printf("Cedula: %s\n", v.cedula);
    printf("Anio: %s\n", v.anio);
    printf("Tipo: %s\n", v.tipo);
    printf("Avaluo: %s\n", v.avaluo);
}

// Función que muestra el menú principal y permite interactuar con el sistema
void menuPrincipal() {
    int opcion;                // Variable para almacenar la opción del usuario
    char input[10];            // Buffer para leer la entrada del usuario
    char placaBuscada[8];      // Buffer para buscar un vehículo por placa
    
    do {
        // Mostrar las opciones del menú
        printf("\n===== SISTEMA DE MATRICULACION VEHICULAR =====\n");
        printf("1. Registrar vehiculo\n");
        printf("2. Registrar revisiones tecnicas\n");
        printf("3. Calcular valor matricula\n");
        printf("4. Mostrar estado de revisiones\n");
        printf("5. Buscar vehiculo por placa\n");
        printf("6. Listar vehiculos matriculados\n");
        printf("7. Generar comprobante de registro\n");
        printf("8. Cerrar sesion\n");
        printf("9. Salir completamente\n");
        printf("Seleccione una opcion: ");
        
        // Leer opción del usuario y validar que sea un número
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &opcion) != 1) {
            opcion = 0; // Si no es un número, asigna 0 para mostrar mensaje de opción no válida
        }
        
        switch (opcion) {
        case 1:
            // Registrar un nuevo vehículo
            if (cantidadVehiculos < MAX_VEHICULOS) {
                validarMatricula(&vehiculos[cantidadVehiculos]); // Llama a función para llenar los datos del vehículo
                strcpy(vehiculos[cantidadVehiculos].usuario, usuarioActual);
                
                // Optimizamos para que guarde el auto creado individualmente y no duplique el archivo masivo
                guardarVehiculos(&vehiculos[cantidadVehiculos], 1);
                cantidadVehiculos++; // Incrementa el contador de vehículos
            } else {
                printf("No se puede registrar mas vehiculos.\n");
            }
            break;
        case 2:
            // Registrar revisiones técnicas de un vehículo existente
            printf("Ingrese placa del vehiculo: ");
            fgets(placaBuscada, sizeof(placaBuscada), stdin);
            placaBuscada[strcspn(placaBuscada, "\n")] = '\0'; // Eliminar salto de línea
            {
                int idx = buscarVehiculoPorPlaca(vehiculos, cantidadVehiculos, placaBuscada);
                if (idx != -1) {
                    registrarRevisiones(vehiculos[idx].revisiones); // Llamada a función de registrar revisiones
                } else {
                    printf("Vehiculo no encontrado.\n");
                }
            }
            break;
        case 3:
            // Calcular valor de matrícula de un vehículo
            printf("Ingrese placa del vehiculo para calcular matricula: ");
            fgets(placaBuscada, sizeof(placaBuscada), stdin);
            placaBuscada[strcspn(placaBuscada, "\n")] = '\0';
            {
                int index = buscarVehiculoPorPlaca(vehiculos, cantidadVehiculos, placaBuscada);
                if (index != -1) {
                    Vehiculo v = vehiculos[index];
                    // Llama a la función que calcula la matrícula (pasando varios parámetros, algunos fijos)
                    float total = calcular_matricula_vehicular(
                        v.tipo, 1600, atoi(v.anio), "Quito", 0.0f, 1.0f, 40.0f, 4, v.placa[6] - '0');
                    printf("Total matricula calculado: $%.2f\n", total);
                } else {
                    printf("Vehiculo no encontrado.\n");
                }
            }
            break;
        case 4:
            // Mostrar estado de revisiones de un vehículo
            printf("Ingrese placa del vehiculo: ");
            fgets(placaBuscada, sizeof(placaBuscada), stdin);
            placaBuscada[strcspn(placaBuscada, "\n")] = '\0';
            {
                int idy = buscarVehiculoPorPlaca(vehiculos, cantidadVehiculos, placaBuscada);
                if (idy != -1) {
                    mostrarEstadoRevisiones(vehiculos[idy]);
                } else {
                    printf("Vehiculo no encontrado.\n");
                }
            }
            break;
        case 5:
            // Buscar vehículo por placa y mostrar datos
            printf("Ingrese placa del vehiculo a buscar: ");
            fgets(placaBuscada, sizeof(placaBuscada), stdin);
            placaBuscada[strcspn(placaBuscada, "\n")] = '\0';
            {
                int encontrado = buscarVehiculoPorPlaca(vehiculos, cantidadVehiculos, placaBuscada);
                if (encontrado != -1) {
                    // Muestra los datos si fue encontrado dinámicamente
                    mostrarVehiculo(vehiculos[encontrado]);
                } else {
                    printf("Vehiculo no encontrado.\n");
                }
            }
            break;
        case 6:
            // Listar todos los vehículos registrados
            listarVehiculos(vehiculos, cantidadVehiculos);
            break;
        case 7:
            // Generar comprobante de registro de un vehículo
            printf("Ingrese placa del vehiculo para generar comprobante: ");
            fgets(placaBuscada, sizeof(placaBuscada), stdin);
            placaBuscada[strcspn(placaBuscada, "\n")] = '\0';
            {
                int idx = buscarVehiculoPorPlaca(vehiculos, cantidadVehiculos, placaBuscada);
                if (idx != -1) {
                    generarComprobanteTXT(vehiculos[idx]);
                    printf("Comprobante generado correctamente.\n");
                } else {
                    printf("Vehiculo no encontrado.\n");
                }
            }
            break;
        case 8:
            // Cerrar sesión y volver al login
            printf("Cerrando sesion...\n");
            return; 
        case 9:
            // Salir completamente del programa
            printf("Gracias por usar el sistema.\n");
            exit(0);
        default:
            printf("Opcion no valida. Debe ser un numero entre 1 y 9.\n");
        }
        
        // Pausa para que el usuario pueda leer los mensajes antes de continuar
        if (opcion != 9 && opcion != 8) {
            printf("\nPresione Enter para continuar...");
            while (getchar() != '\n'); // Espera a que se presione Enter
        }
    } while (opcion != 9); // Repite el menú hasta que se decida salir
}