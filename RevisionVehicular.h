#ifndef REVISION_VEHICULAR_H
#define REVISION_VEHICULAR_H

// Definición de constantes para tamaños de campos
#define MAX_PLACA 10       // Tamaño máximo del string de placa
#define MAX_TIPO 20        // Tamaño máximo del tipo de vehículo
#define MAX_AVALUO 20      // Tamaño máximo del string del avalúo
#define MAX_ANIO 5         // Tamaño máximo del año (4 dígitos + '\0')
#define MAX_CEDULA 11      // Tamaño máximo de la cédula (10 dígitos + '\0')
#define NUM_REVISIONES 3   // Número de revisiones técnicas que se registran
#define MAX_VEHICULOS 100  // Máximo de vehículos que se pueden registrar

// Estructura que representa un vehículo
typedef struct {
    char placa[8];                    // Placa del vehículo (3 letras + 4 números)
    char cedula[11];                  // Cédula del propietario
    char anio[5];                     // Año del vehículo
    char tipo[20];                    // Tipo de vehículo: pesado, liviano, moto
    char avaluo[10];                  // Valor del avalúo del vehículo
    int revisiones[NUM_REVISIONES];   // Estado de revisiones técnicas: 1=aprobada, 0=no aprobada
} Vehiculo;

// Funciones para registrar y validar un vehículo
void validarMatricula(Vehiculo *v); 

// Función que calcula el valor de matrícula basado en tipo, año, multas, etc.
float calcular_matricula_vehicular(
    const char tipo_vehiculo[], 
    int cilindraje, 
    int anio_fabricacion,
    const char provincia[], 
    float es_ecologico, 
    float revision_obligatoria,
    float multas_transito, 
    int mes_pago, 
    int ultimo_digito_placa
);

// Funciones relacionadas con revisiones técnicas
void registrarRevisiones(int revisiones[NUM_REVISIONES]);
void mostrarEstadoRevisiones(const Vehiculo v);
void listarVehiculos(const Vehiculo vehiculos[], int cantidad);
int buscarVehiculoPorPlaca(const Vehiculo vehiculos[], int cantidad, const char* placa);

// Funciones auxiliares
void limpiarBuffer();                    // Limpia buffer de entrada para evitar lecturas residuales
void generarComprobanteTXT(const Vehiculo v);  // Genera un archivo de comprobante de registro

// Función de login y registro de usuarios
int pantallaLogin();

#endif	