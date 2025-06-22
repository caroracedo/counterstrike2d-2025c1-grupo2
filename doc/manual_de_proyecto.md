# Manual de Proyecto

<!-- Este manual es una documentación corta para indicar en que trabajó cada integrante del equipo, como se organizaron por semana y cuanto difirió del plan inicial.

Indiquen que herramientas usaron (IDEs, linters) y que documentación usaron para aprender sobre las tecnologías.

Hay algo que crean que debería darse en Taller? Indiquenlos también.

Cuales fueron los puntos más problemáticos? Pudieron llegar con todo? Hay errores conocidos? Si volvieran hacer el proyecto, que cambiarían (a nivel código o a nivel organizacional). -->

En el presente Manual de Proyecto se detallará la organización del grupo, los problemas enfrentados, las herramientas utilizadas y una breve conclusión que engloba los aspectos a mejorar.

## Organización y problemáticas

Ante la presentación del TP teníamos en claro que era necesario trabajar intensamente en el proyecto; por eso no tardamos en establecer las respectivas responsabilidades:

- Luciano Serra se encargaría de la parte gráfica con SDL2.
- Carolina Racedo y Mora Mujica se encargarían de la lógica del juego y de la arquitectura cliente-servidor.
- Luciana Larrosa se encargaría del editor y el lobby con Qt.

Y así comenzamos. Sabíamos, y nuestro corrector nos hizo notar inmediatamente, que lo más importante era ir integrando a medida que avanzáramos. Además, los nuevos _features_ que implementáramos (en todos los niveles) funcionarían como una buena métrica de progreso. Por lo tanto, guiándonos principalmente por esta idea, a lo largo del desarrollo fuimos implementando cada funcionalidad de forma sincronizada (esta semana este feature, la próxima estos, y así sucesivamente).

La primera semana fue más una exploración de parte de todos los niveles. A nivel cliente-servidor nos enfrentamos ante problemas de integración y de dependencias. Ante esta situación, decidimos reorganizarnos:

- Luciano Serra se encargaría de la parte gráfica con SDL2.
- Mora Mujica se encargaría de la lógica del juego.
- Carolina Racedo se encargaría de la arquitectura cliente-servidor.
- Luciana Larrosa se encargaría del editor y el lobby con Qt.

Este cambio fue un antes y un después, ya que nos permitió **testear en capas**. Por un lado, testeábamos la lógica con un programa sencillo aislado; a partir de eso, testeábamos la arquitectura cliente-servidor utilizando un **Mock Gráfico**, que funcionaba únicamente con entrada y salida estándar. Finalmente, testeábamos la gráfica reemplazando el Mock Gráfico por la gráfica en sí. Esto al principio fue extremadamente útil, aunque, por razones obvias, no perduró en el tiempo y el Mock quedó en desuso.

A nivel cliente-servidor, nos llevó aproximadamente 3 semanas implementar el movimiento y las colisiones. Mientras tanto, el editor y el lobby continuaban en desarrollo.

Para el primer mes ya contábamos con el movimiento, los disparos, las colisiones, entre otras funcionalidades, lo cual nos permitió incorporar los mapas desarrollados por el editor, así como el lobby.

Para la primer entrega, nos encontramos con un juego bastante sólido, que carecía de drops, pero que era funcional e integrado.

Es por ello que, luego de la demo de la primer entrega, nos pusimos manos a la obra para implementar todos los aspectos restantes con la metodología habitual.

<!-- Acá iría lo que estuvimos haciendo hasta la entrega final -->

## Herramientas utilizadas

Utilizamos principalmente Visual Studio Code como IDE, ya que nos resultó muy útil para trabajar en conjunto y corregir errores de forma rápida y colectiva. 

Como linters, recurrimos a los que habíamos utilizado en trabajos anteriores de la cátedra: CPPLint y CPPCheck. Además, fueron instalados en los hooks antes de cada commit. Utilizamos el CMake y el Makefile provisto por la cátedra, incorporando todas las dependencias adicionales necesarias para el proyecto.

Para entender las dependencias, nos basamos tanto en los tutoriales y clases provistos por la cátedra como en algunos tutoriales de Youtube y en información de la web.

## Conclusión

Para finalizar este Manual de Proyecto, queríamos destacar lo importante que fue para nosotros la organización y el trabajo conjunto, así como el manejo sincrónico de las tareas. La división del trabajo por módulos fue algo esencial.

Además, es necesario aclarar que el proyecto requirió una gran dedicación, ya que se trató de un trabajo extenso y demandante.

Aun así, nos resultó muy entretenido, lo que ayudó a que el proceso fuera más llevadero.
