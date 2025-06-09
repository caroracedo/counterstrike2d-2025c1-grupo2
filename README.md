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

Se recomienda el uso de **Ubuntu 24.04** para asegurar la compatibilidad con todas las bibliotecas requeridas.

A continuación, se detallan las dependencias necesarias y los comandos sugeridos para su instalación:

### YAML

El proyecto utiliza la biblioteca [`yaml-cpp`](https://github.com/jbeder/yaml-cpp), la cual es descargada automáticamente mediante CMake. No se requiere instalación manual.

### SDL2 y SDL2pp

Para el cliente, se requiere SDL2 y sus módulos. Aunque estas bibliotecas se gestionan automáticamente con FetchContent, es necesario instalar las dependencias base del sistema:

```sh
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
```

El wrapper en C++ SDL2pp también es gestionado automáticamente por CMake.

### Qt

Para compilar el cliente y/o el editor, se requieren las bibliotecas de Qt. Se recomienda utilizar Qt6:

```sh
sudo apt-get install qt6-base-dev qt6-base-dev-tools
```

CMake detectará automáticamente la versión disponible en el sistema.

---

## Compilación

Para compilar el proyecto, ejecutar el siguiente comando desde la raíz del repositorio:

```sh
make compile-debug
```

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
  En otra terminal, ejecutar el cliente indicando el hostname y el puerto definidos en el archivo YAML:

  ```sh
  ./taller_client <hostname> <puerto>
  ```
  Ejemplo:

  ```sh
  ./taller_client localhost 1500
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
