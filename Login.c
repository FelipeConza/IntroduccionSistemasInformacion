#include <stdio.h>      // Librería para entrada/salida estándar (printf, scanf, fgets)
#include <stdlib.h>     // Librería para funciones generales (exit, system)
#include <string.h>     // Librería para manipulación de cadenas (strlen, strcmp, etc.)
#include <ctype.h>      // Librería para funciones de caracteres (isalpha, isalnum)

extern char usuarioActual[30];

// Definición de límites máximos para usuario y contraseña
#define MAX_USUARIO 1000
#define MAX_CONTRASENA 1000

// Declaración de funciones utilizadas
int registrarUsuario();
int iniciarSesion();
int validarContrasena(const char *contrasena);

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
                // Registrar usuario
                if (registrarUsuario()) {
                    printf("Registro exitoso.\n");
                }
                break;
            case 2:
                // Iniciar sesión
                if (iniciarSesion()) {
                    return 1; // Login exitoso, retorna al menú principal
                } else {
                    printf("Fallo en inicio de sesion.\n");
                }
                break;
            case 3:
                exit(0); // Salir del programa
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
        
        // Pausa para que el usuario lea mensajes antes de continuar
        printf("\nPresione Enter para continuar...");
        while (getchar() != '\n');
    }
}

// Función para registrar un nuevo usuario
int registrarUsuario() {
    char usuario[MAX_USUARIO], contrasena[MAX_CONTRASENA];
    FILE *f = fopen("usuarios.txt", "a"); // Abrir archivo de usuarios en modo añadir
    if (f == NULL) {
        printf("Error al abrir archivo de usuarios.\n");
        return 0;
    }
    
    // Pedir nombre de usuario (solo letras)
    printf("Ingrese nombre de usuario (solo letras): ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0'; // Eliminar salto de línea
    
    if (strlen(usuario) == 0) {
        printf("Usuario vacio no permitido.\n");
        fclose(f);
        return 0;
    }
    
    // Validar que solo contenga letras
    for (int i = 0; i < strlen(usuario); i++) {
        if (!isalpha(usuario[i])) {
            printf("Usuario invalido. Solo letras permitidas.\n");
            fclose(f);
            return 0;
        }
    }
    
    // Pedir contraseña hasta que sea válida
    do {
        printf("Ingrese contrasena (6 caracteres alfanumericos): ");
        fgets(contrasena, sizeof(contrasena), stdin);
        contrasena[strcspn(contrasena, "\n")] = '\0';
        
        if (!validarContrasena(contrasena)) {
            printf("Contrasena invalida. Debe tener 6 caracteres alfanumericos.\n");
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
    char linea[100];
    FILE *f = fopen("usuarios.txt", "r"); // Abrir archivo de usuarios en modo lectura
    if (f == NULL) {
        printf("Debe registrarse primero.\n");
        return 0;
    }
    
    // Pedir datos de login
    printf("Usuario: ");
    fgets(usuario, sizeof(usuario), stdin);
    usuario[strcspn(usuario, "\n")] = '\0';
    
    printf("Contrasena: ");
    fgets(contrasena, sizeof(contrasena), stdin);
    contrasena[strcspn(contrasena, "\n")] = '\0';
    
    // Leer línea por línea y comparar con los datos ingresados
    while (fgets(linea, sizeof(linea), f)) {
        sscanf(linea, "%[^,],%s", archivoUsuario, archivoContrasena); // Separar usuario y contraseña
        if (strcmp(usuario, archivoUsuario) == 0 && strcmp(contrasena, archivoContrasena) == 0) {
            strcpy(usuarioActual, usuario);
            fclose(f);
            return 1; // Login exitoso
        }
    }
    fclose(f);
    return 0; // Login fallido
}

// Función para validar que la contraseña tenga 6 caracteres alfanuméricos
int validarContrasena(const char *contrasena) {
    if (strlen(contrasena) != 6) return 0; // Debe tener exactamente 6 caracteres
    for (int i = 0; i < 6; i++) {
        if (!isalnum(contrasena[i])) return 0; // Cada carácter debe ser letra o número
    }
    return 1; // Contraseña válida
}
