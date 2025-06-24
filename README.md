# Counter Strike 2D - Grupo 2

_Taller de Programación - Cátedra Veiga - 2025_

---

## Integrantes

- Luciana Larrosa - 110476
- Mora Mujica - 111223
- Carolina Racedo - 110550
- Luciano Serra - 110694

---

## Requisitos y dependencias

Se recomienda el uso de **Ubuntu 24.04** para asegurar la compatibilidad con todas las librerías requeridas.

A continuación, se detallan las dependencias necesarias y los comandos sugeridos para su instalación:

### YAML

El proyecto utiliza la librería [`yaml-cpp`](https://github.com/jbeder/yaml-cpp), la cual es descargada automáticamente mediante CMake. No se requiere instalación manual.

### SDL2 y SDL2pp

Para el cliente, se requiere SDL2 y sus módulos. Aunque estas librerías se gestionan automáticamente con FetchContent, es necesario instalar las dependencias base del sistema:

- `libsdl2-dev`
- `libsdl2-image-dev`
- `libsdl2-mixer-dev`
- `libsdl2-ttf-dev`
- `libsdl2-gfx-dev`

El wrapper en C++ SDL2pp también es gestionado automáticamente por CMake.

### Qt

Para compilar el cliente y/o el editor, se requieren las librerías de Qt. Se recomienda utilizar Qt6:

- `qt6-base-dev`
- `qt6-base-dev-tools`
- `qt6-multimedia-dev`

CMake detectará automáticamente la versión disponible en el sistema.

### Instalación de dependencias

El proyecto provee un instalador (y desinstalador) para gestionar las dependencias del mismo.

Para instalar las dependencias necesarias, ejecutar el siguiente comando desde la raíz del repositorio:

```sh
make install
```

Para desinstalarlas:

```sh
make uninstall
```

---

## Limpieza de archivos de compilación

Para eliminar todos los archivos generados durante el proceso de compilación:

```sh
make clean
```

Esto eliminará el directorio build/ y todo su contenido.

---

## Compilación

Para compilar el proyecto, ejecutar el siguiente comando desde la raíz del repositorio:

```sh
make compile-debug
```

---

## Ejecución de tests

Una vez compilado el proyecto, se pueden ejecutar los tests automáticos con:

```sh
make run-tests
```

Esto ejecutará las pruebas del protocolo de comunicación tanto del servidor como del cliente.

---

## Compilación Completa

El siguiente comando realiza una compilación limpia del proyecto y ejecuta automáticamente todos los tests:

```sh
make
```

Este comando ejecuta, en orden:

- `make clean`
- `make compile-debug`
- `make run-tests`

---

## Ejecución

### Editor (CS2D Editor)

Para ejecutar el editor, posicionarse en la carpeta `build` y utilizar el siguiente comando:

```sh
./taller_editor
```

### Juego (CS2D)

Para ejecutar el juego, se recomienda utilizar dos terminales diferentes desde la carpeta `build`:

- **Servidor**:  
  Ejecutar el servidor indicando la ruta al archivo de configuración YAML:

  ```sh
  ./taller_server <ruta_al_yaml>
  ```
  Ejemplo utilizando el archivo de configuración proporcionado:

  ```sh
  ./taller_server ../config/config2.yaml
  ```

- **Cliente**:  
  En otra terminal, ejecutar el cliente:

  ```sh
  ./taller_client
  ```

---

## Créditos

En el presente proyecto se utilizan las implementaciones de las clases `Socket`, `ResolverError`, `LibError`, `Thread` y `Queue<T>` provistas por la cátedra.

### Repositorios

El código fuente se encuentra en los siguientes repositorios de GitHub:

- [Hands-On Sockets](https://github.com/eldipa/hands-on-sockets-in-cpp)
- [Hands-On Threads](https://github.com/eldipa/hands-on-threads)

---

## Licencia

GPL v2
