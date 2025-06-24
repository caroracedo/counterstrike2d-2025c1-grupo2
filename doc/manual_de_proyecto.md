# Manual de Proyecto

<!-- Este manual es una documentación corta para indicar en que trabajó cada integrante del equipo, como se organizaron por semana y cuanto difirió del plan inicial.

Indiquen que herramientas usaron (IDEs, linters) y que documentación usaron para aprender sobre las tecnologías.

Hay algo que crean que debería darse en Taller? Indiquenlos también.

Cuales fueron los puntos más problemáticos? Pudieron llegar con todo? Hay errores conocidos? Si volvieran hacer el proyecto, que cambiarían (a nivel código o a nivel organizacional). -->

En el presente Manual de Proyecto se detalla la organización del grupo, los problemas enfrentados, las herramientas utilizadas y una breve conclusión que engloba los aspectos a mejorar.

## Organización y problemáticas

Ante la presentación del TP tuvimos en claro que el proyecto requería una dedicación intensa; por ello, rápidamente definimos las responsabilidades iniciales:

- Luciano Serra se encargaría de la parte gráfica con SDL2.
- Carolina Racedo y Mora Mujica se encargarían de la lógica del juego y de la arquitectura cliente-servidor.
- Luciana Larrosa se encargaría del editor y el lobby con Qt.

Y así comenzamos. Sabíamos, y nuestro corrector nos hizo notar inmediatamente, que lo más importante era ir integrando a medida que avanzáramos. Además, los nuevos _features_ que implementáramos (en todos los niveles) funcionarían como una buena métrica de avance. Por lo tanto, guiándonos principalmente por esta idea, a lo largo del desarrollo fuimos implementando cada funcionalidad de forma sincronizada (esta semana este feature, la próxima estos, y así sucesivamente).

La primera semana fue principalmente exploratoria en todos los niveles. A nivel cliente-servidor nos enfrentamos ante problemas de integración y de dependencias. Ante esta situación, decidimos reorganizar las tareas de la siguiente forma:

- Luciano Serra se encargaría de la parte gráfica con SDL2.
- Mora Mujica se encargaría de la lógica del juego.
- Carolina Racedo se encargaría de la arquitectura cliente-servidor.
- Luciana Larrosa se encargaría del editor y el lobby con Qt.

Esta nueva organización nos permitió implementar una estrategia de **testing en capas**. Primero testeábamos la lógica en forma aislada con un programa sencillo; luego, la arquitectura cliente-servidor con un _mock gráfico_ que trabajaba por entrada/salida estándar; finalmente, la gráfica reemplazando el mock por la gráfica en sí. Esta estrategia fue muy útil al comienzo, aunque por razones obvias, no perduró en el tiempo y el mock quedó en desuso.

A nivel cliente-servidor, nos llevó aproximadamente tres semanas implementar el movimiento y las colisiones. Mientras tanto, el editor y el lobby continuaban en desarrollo.

Para el primer mes ya contábamos con el movimiento, los disparos, las colisiones, y otras funcionalidades, lo que nos permitió incorporar los mapas desarrollados por el editor y el lobby.

Para la primer entrega, nos encontramos con un juego que aún no contaba con todas las funcionalidades previstas, pero que era funcional e integrado.

Tras la primer entrega, retomamos el trabajo con la metodología habitual para implementar todos los aspectos restantes. También refactorizamos el código, solucionamos errores, escribimos la documentación en conjunto y desarrollamos el instalador de dependencias. Además, Luciana Larrosa y Carolina Racedo implementaron los tests del protocolo utilizando Google Test.

## Herramientas utilizadas

Utilizamos principalmente **Visual Studio Code** como IDE, ya que nos resultó muy útil para trabajar en conjunto y corregir errores de forma rápida y colectiva. 

Como linters, recurrimos a los que habíamos utilizado en trabajos anteriores de la cátedra: **Clang-Format**, **CPPLint** y **CPPCheck**. Además, fueron instalados en los hooks para ser ejecutados antes de cada commit. Utilizamos el **CMake** y el **Makefile** provisto por la cátedra, incorporando todas las dependencias adicionales necesarias para el proyecto.

Para entender las dependencias, nos basamos en las clases y tutoriales provistos por la cátedra, así como en recursos externos (información de la web y algunos tutoriales de YouTube).

## Conclusión

Para finalizar este Manual de Proyecto, queríamos destacar lo importante que fue para nosotros la **organización** y el **trabajo en equipo**, así como el **manejo sincrónico de las tareas**. La división modular del trabajo fue un aprendizaje clave que nos permitió avanzar de manera sostenida.

El proyecto requirió una gran dedicación por parte de todo el equipo, y nos dejó aprendizajes tanto técnicos como organizativos.