#include <stdio.h>      // Librería para entrada/salida estándar (printf, scanf, fgets)
#include <stdlib.h>     // Librería para funciones generales (exit, system)
#include <string.h>     // Librería para manipulación de cadenas (strlen, strcmp, etc.)
#include <ctype.h>      // Librería para funciones de caracteres (isalpha, isalnum, toupper)

extern char usuarioActual[30];

// Ajustamos los límites de los búferes de lectura para que no desborden la pila de la memoria
#define MAX_USUARIO 60
#define MAX_CONTRASENA 60

// Declaración de funciones utilizadas
int registrarUsuario();
int iniciarSesion();
int validarContrasena(const char *contrasena);
void limpiarBuffer();

// Función principal de login
int pantallaLogin() {
    int opcion;
    char input[10];  // Buffer para leer opción del usuario
    
    while (1) {
        system("cls");  // Limpia la pantalla (Windows)
        printf("===== BIENVENIDO AL SISTEMA DE MATRICULACION =====\n");
        printf("1. Registrarse\n");
        printf("2. Iniciar sesion\n");
        printf("3. Salir\n");
        printf("Seleccione una opcion: ");
        
        // Leer opción del usuario y validar que sea un número
        fgets(input, sizeof(input), stdin);
        if (sscanf(input, "%d", &opcion) != 1) opcion = 0; // Si no es número, se asigna 0
        
        switch(opcion) {
            case 1:
                if (registrarUsuario()) {
                    printf("\n[OK] Registro exitoso.\n");
                }
                break;
            case 2:
                if (iniciarSesion()) {
                    return 1; // Login exitoso, retorna al menú principal
                } else {
                    printf("\n[ERROR] Usuario o contrasena incorrectos.\n");
                }
                break;
            case 3:
                exit(0); // Salir del programa
            default:
                printf("\nOpcion invalida. Intente de nuevo.\n");
        }
        
        // Pausa limpia para que el usuario lea los mensajes
        printf("\nPresione Enter para continuar...");
        limpiarBuffer();
    }
}

// Función para registrar un nuevo usuario
int registrarUsuario() {
    char usuario[MAX_USUARIO], contrasena[MAX_CONTRASENA];
    FILE *f = fopen("data/usuarios.txt", "a"); // Agregamos la ruta data/
    if (f == NULL) {
        printf("Error al abrir archivo de usuarios.\n");
        return 0;
    }
    
    // Pedir nombre de usuario y convertirlo a MAYÚSCULAS para que coincida con tu BD
    printf("Ingrese nombre de usuario (solo letras y numeros): ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0'; // Eliminar salto de línea
    
    if (strlen(usuario) == 0) {
        printf("Usuario vacio no permitido.\n");
        fclose(f);
        return 0;
    }
    
    // Validar y transformar a mayúsculas
    for (int i = 0; i < strlen(usuario); i++) {
        usuario[i] = toupper((unsigned char)usuario[i]);
    }
    
    // Pedir contraseña asegurando estrictamente el rango de 6 a 12 caracteres alfanuméricos
    do {
        printf("Ingrese contrasena (6-12 caracteres alfanumericos): ");
        fgets(contrasena, sizeof(contrasena), stdin);
        contrasena[strcspn(contrasena, "\n")] = '\0';
        
        if (!validarContrasena(contrasena)) {
            printf("[ERROR] Contrasena invalida. Intente otra vez.\n");
        }
    } while (!validarContrasena(contrasena));
    
    // Guardar usuario y contraseña en archivo
    fprintf(f, "%s,%s\n", usuario, contrasena);
    fclose(f);
    return 1;
}

// Función para iniciar sesión
int iniciarSesion() {
    char usuario[MAX_USUARIO], contrasena[MAX_CONTRASENA];
    char archivoUsuario[MAX_USUARIO], archivoContrasena[MAX_CONTRASENA];
    char linea[150];
    FILE *f = fopen("data/usuarios.txt", "r"); // Agregamos la ruta data/
    if (f == NULL) {
        printf("Debe registrarse primero.\n");
        return 0;
    }
    
    // Pedir datos de login
    printf("Usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';
    
    // Convertir entrada a mayúsculas para evitar problemas de Case Sensitivity
    for (int i = 0; i < strlen(usuario); i++) {
        usuario[i] = toupper((unsigned char)usuario[i]);
    }
    
    printf("Contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';
    
    // Leer línea por línea y comparar con los datos ingresados
    while (fgets(linea, sizeof(linea), f)) {
        // Remover el salto de línea que lee fgets de la fila del archivo
        linea[strcspn(linea, "\n")] = '\0';
        
        if (sscanf(linea, "%[^,],%s", archivoUsuario, archivoContrasena) == 2) {
            if (strcmp(usuario, archivoUsuario) == 0 && strcmp(contrasena, archivoContrasena) == 0) {
                strcpy(usuarioActual, usuario);
                fclose(f);
                return 1; // Login exitoso
            }
        }
    }
    fclose(f);
    return 0; // Login fallido
}

// Función para validar que la contraseña tenga de 6 a 12 caracteres alfanuméricos
int validarContrasena(const char *contrasena) {
    int longitud = strlen(contrasena);
    
    // 1. Verifica la regla de longitud de 6 a 12
    if (longitud < 6 || longitud > 12) {
        return 0; 
    }
    
    // 2. Verifica que cada carácter sea alfanumérico
    for (int i = 0; i < longitud; i++) {
        if (!isalnum((unsigned char)contrasena[i])) {
            return 0; 
        }
    }
    
    return 1; 
}