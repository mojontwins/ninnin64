20190714
========

Como siempre, estoy empezando este diario demasiado tarde. 

Primero, por gusto, cogí el AGNES 0.1 que llevaba el primer intento de Cheril Perils y me puse a limpiarlo y a optimizarlo. Así conseguí que este juego pasase de ocupar casi los 32K del NROM a quedar justo por debajo de 16K.

Luego pensé que molaría seguir el juego como algo sencillo que jugara Luca, pero luego se me pasó, y pensé mejor en hacer una suerte de Ninjajar Lost Levels con fases sencillas.

AGNES 0.1 R (de remozado) es un motor muy sencillo pero también es muy pequeño. Por ahora Nin Nin (que es como he llamado al juego) tiene tres fases y estoy algo por debajo de los 16K - sin música en condiciones.

[ ] Me reservaré entre 1.5 y 2Kb para músicas simples.
[ ] Tengo que corregir la colisión cuando hay tile 8 sobre tile 4 y se salta desde abajo.
[ ] Tengo que ver si ahorraría aún más usando el modelo de enemigos con puntero y número variable de enemigos por pantalla, ya que no suelo poner muchos enemigos.

Quiero hacer algún castillo que tenga algo de chicha, lo que hacía en el original con scripts. Para AGNES 0.1 R he diseñado un sistema sencillo de hooks parecido al code injections de AGNES 1 y 2, que funciona más o menos como los puntos de ejecución del scripting. Para ciertas cosas creo que producirá resultados más compacts, sobre todo teniendo en cuenta que estoy escribiendo rutinas en ensamble y reaprovachando código a saco.

Voy a añadir ahora un sistema sencillo para poder usar los bits de enfasis de color fácilmente.

También había pensado en meter monococos pero meter cocos me va a hacer engordar el código una barbaridad, ya que ahora incluso he evitado añadir división, módulo y multiplicaciones desde el runtime.

Sin embargo sí que quiero añadir más cosas que me den variedad al juego. Por ahora, sobre el AGNES 0.1 original, AGNES 0.1 R tiene:

- Arenas movedizas.
- Tiles resbalosos.
- Tiles cuenta (al hacer la pantalla se van contando para luego hacer cosas desde los *hooks*; en este juego se usan para abrir puertas con pulsadores).
- Hitter.
- Breakables.

Voy a hacer un bosque denoche para seguir añadiendo fases antes de introducir nuevos elementos.

20190715
========

He pasado a ensamble todo el módulo del breakable ganando algunos bytes y ciclos. Y ahora voy a comparar los resultados del módulo de enemigos comprimidos vs descomprimidos *para este juego*.

- enems0 107 vs 240 (44%)
- enems1 141 vs 288 (48%)
- enems2 135 vs 288 (46%)

Vaya, el resultado es acojonante. Creo que me cambio de compresor. Tendré que idear una forma de integrarlo en el motor super minimalista.

¡Cambio hecho! Otro porrón de bytes más ahorrados, y lo que queda, porque estamos a una media de 700 bytes para el mapa y 140 para los enemigos, por fase :-D

~~ 

He metido cocos (por probar) en pocos bytes - unos cocos muy tontos, que se disparan sólo en una dirección ortogonal. Es para implementar el tío de la nube pesao que disparaba, por ejemplo.

Pero quiero implementar que los lineales también disparen en horizontal.

Ahora uso muy pocos bits del tipo y del atributo, de forma que podría hacer esto:

Tipo: AAAABBBB
Attr: CCCCDDDD

Y usar BBBB y DDDD para lo que uso ahora todo el byte, teniendo el resto de los bits para definir comportamientos.

Voy por lo pronto a meter los ANDs necesarios antes de añadir nada más.

Voy a poner que si el tipo AAAA == 1, el enemigo dispare cuando te tenga a tiro, horizontalmente, y si AAAA == 2 lo mismo, pero verticalmente.

20190716
========

Obviamente, no he pensado en que tenía que definir una frecuencia de disparo. La defina como la defina, necesitaré un contador. Voy a ver con qué arrays "libres" puedo contar: ¡mira! tenía `en_ct` apañado para los pursuers de tipo 7 y no lo tenía "restringido", así que está ahí. Guay.

La frecuencia me vale con que sea fija, no quiero engordar más el motor.

Ahora parece que los lanza y tal, pero los cocos se mueren tras un frame. Seguramente porque escribí el código muy rápido y fue directamente en ensamble :-D

Como siempre he puesto todas las comparaciones al revés :')

~

Ahora hay una subida de dificultad bastante heavy en la fase 3 (la cueva), y la parte inferior de la fase 2 tiene saltos muy difíciles. Voy a rebajar la dificultad de esta parte de la fase 2 y luego intercalar un par de fases más sencillas antes de la cueva.

Pantallas de nadal
------------------

Como no puedo hacer arte, voy a ver cómo meterle minimalmente la posibilidad de nadar. En AGNES se mantiene una variable `vertical_engine_type` para poder alternar entre `ENGINE_TYPE_JUMP`, `ENGINE_TYPE_SWIM`, y otros, que se pueden activar a la vez. El tipo por defecto se elige siguiendo una sencilla ley de "el primero que coincida si hay varios".

Luego, en `player_move`:

- Si `vertical_engine_type != ENGINE_TYPE_SWIM`, aplicamos `PLAYER_G`.
- Antes de la colisión, en ese sitio donde el motor genital lee el mando, si `vertical_engine_type == ENGINE_TYPE_SWIM` hacemos la natación usando el mando:

```c
	#ifdef PLAYER_SWIMS
		// Controller 

		if (vertical_engine_type == ENGINE_TYPE_SWIM) {
			if (!(pad0 & (PAD_DOWN|PAD_A))) {
				pvy -= PLAYER_AY_SWIM >> 1;
			} else {
				if (pad0 & (PAD_DOWN|PAD_A)) {
					pvy += PLAYER_AY_SWIM;
				}
		
				// Limit
				if (pvy > PLAYER_VY_SWIM_MAX) {
					pvy = PLAYER_VY_SWIM_MAX;
				}
			}
			if (pvy < -PLAYER_VY_SWIM_MAX) {
				pvy = -PLAYER_VY_SWIM_MAX;
			}
		}
	#endif
```

- Finalmente, la parte del salto está guardada por un `vertical_engine_type == ENGINE_TYPE_JUMP`.

Voy a ver de integrar esto aunque aún no tenga para probarlo :-)

~

Arogas entregagas. Ahora sólo hace falta meterle los sprites. Había dejado sitio para poder jugar con cheril pero meh. Esto es mejor.

20190719
========

Pensando en cosas para un futurible castillo con puzzles y trampas que se medio hagan solas y no tenga que añadir toneladas de código:

- Springs, salen pinchos y tal, pero de una forma mucho más sencilla: marcar un tile con un beh especial, y si lo tocas aparece un pincho en ese tile. Pero es que se me están acabando los behs; luego lo resuelvo.

- Tiles que se rompen andando sobre ellos. Ya tengo tiles que se rompen, pero solo con puñetazos. ¿Cómo los marco para que además se rompan andando sobre ellos? ¿Apaño por # de tile resuelto en el master metatileset? ¿Cual uso, que ya no tengo? ¿Cuál descarto para meter este?

- La trampa esa del suelo que desaparece la pongo, aunque la tenga que programar a pelarcio.

- Puertas que se abren activando baldosas ya lo tengo, pero ¿en un orden concreto? ¿Cómo?

- Botón que active o haga aparecer una plataforma movil.

- Steady shooters - los tengo, básicamente, de rebote, pero ahora mismo la frecuencia del disparo es fija y quizá tendría que inventar una historia para poderla definir. ¡Remember que tengo el medio byte más significativo del atributo sin usar! Puedo usarlo para definir una frecuencia, tomando la "por defecto" si vale 0, y así no tengo que cambiar nada de lo que ya tengo hecho. Genius! Thing is, tendré que guardar el valor en algún sitio. ¿Dónde? `en_maxct`.

Voy a empezar a documentar todas estas cosas nonstandard

20190721
========

Voy a ver de escribir en los hooks un manejador estandar que me sirva para los puzzles de pulsar baldosas en orden. La idea es definir el orden usando algo del propio mapa, y que el manejador "cuente" automáticamente, para luego ejecutar "algo".

Qué vago todo, no?

Lo más sencillo es contar tiles de un número o de un tipo, en vertical, situados sobre la baldosa, e ir llevando un contador. Si el número de tiles contados es uno más que la cuenta actual, incrementar la cuenta. Si no, reiniciar. Y además  hay que pulsar los que el motor haya contado.

Lo primero es hacer que cuente los nuevos tiles "carita sonriente" además de los "tiles rayo".

20190724
========

Me temo que ha llegado el momento de meter otros tipos de enemigos, en modo hiper minimal, o esto va a empezar a repetirse y pronto.

Las gyrosaws de MK2 eran especialmente pequeñas. Y un fanty minimal me servirá para algunos puzzles en los castillos. También una boya que dispare en las ocho direcciones de forma secuencial, pero para eso tengo que ampliar los arrays de direcciones y modificar los disparadores de cocos actuales. Voy a empezar haciendo eso.

El Gyroshooter dispara en una nueva dirección cada vez que lo hace, y para añadirlos me ha bastado con:

```c
	#ifdef ENABLE_GYROSHOOTER
		case 0x70:
			__asm__ ("ldx %v", gpit);
			__asm__ ("inc %v, x", en_cur_d);
			__asm__ ("lda %v, x", en_cur_d);
			__asm__ ("and #7");
			__asm__ ("sta %v, x", en_cur_d);
			__asm__ ("sta %v", _t);
			break;
	#endif
```

Voy a ponerlos en esta segunda fase del agua y a probar.

20190729
========

Empecé también el castillo y pulí algunas cosas - pero ahora me doy cuenta de que necesito más granularidad en las velocidades de los fantis. Ahora se pueden poner a 0.5, 1 o 2 píxels por frame, y ya 1 es demasiado rápido.

Creo que usaré otra forma de especificar las velocidades, a modo de divisor, poniendo 0, 1, 3, 7 para 1, 0.5, 0.25 o 0.125 píxels por segundo. Lo puedo guardar en `en_status`, por ejemplo. O en `en_mx`. Pero eso me ocupará levemente más código.

~~

OK! Ya tengo los fanties modificados funcionando, además con su propio *life gauge*. Hay que hostiarlos 3 veces para hacerlos desaparecer, por ahora.

Ahora que ya van, es el momento de pasarlos a ensable a ver si gano algo de la implementación, aunque será poca cosa . . .

~~

Me acabo de dar cuenta de que aún no he usado el suelo resbaloso. Lo dejaré para la siguiente fase. Y tengo que recordar que quiero hacer una lava pit como la del original.

20190730
========

¿Cómo podría implementar una gota con lo que tengo de los lineales? Básicamente, cuando se llegue a los limites, en vez de dar la vuelta, hay que reiniciar a (x1, y1).

Creo que echo más lineas codificando para reaprovechar que implementandolo en unas lineas de ASM:

```c
		__asm__ ("ldx %v", gpit);

		__asm__ ("lda %v", _en_x);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_mx);
		__asm__ ("sta %v", _en_x);

		__asm__ ("cmp %v, x", en_x2);
		__asm__ ("bne %g", enems_droparrow_horz_done);

	enems_droparrow_horz_reset:
		__asm__ ("lda %v, x", en_x1);
		__asm__ ("sta %v", _en_x);

	enems_droparrow_horz_done:

		__asm__ ("lda %v", _en_y);
		__asm__ ("clc");
		__asm__ ("adc %v", _en_my);
		__asm__ ("sta %v", _en_y);

		__asm__ ("cmp %v, x", en_y2);
		__asm__ ("bne %g", enems_droparrow_vert_done);

	enems_droparrow_vert_reset:
		__asm__ ("lda %v, x", en_y1);
		__asm__ ("sta %v", _en_y);

	enems_droparrow_vert_done:
```

Me sirve para flechas (horizontales) y gotas. Ahora mi problema radica en como codificar qué sprite se va a utilizar. Lo más sencillo es asignar uno en una constante, pero eso me limita mucho la utilización ¿Y si quiero usar gotas y flechas a la vez?

Puedo hacer: 
	- Type 8: flecha, y el sprite se decide con `ARROW_BASE_SPRITE` y se le suma 1 si mx < 0.
	- Type 9: drop, y el sprite es `DROP_BASE_SPRITE`. 

Estos valores son *absolutos*.

~~

En previsión de futuro y de cosas que hay que hacer que precisen actualizar cosas de pantalla y tal, creo que va siendo tiempo de abandonar definitivamente el actualizador viejo (modificado) de neslib1 y poner el nuevo. Y esto debería estar haciéndolo también para la rama principal de AGNES, o al menos para la v2... Pero ahora no, que no me apetece.

Voy a hacer un diff entre neslibs y estudiar los cambios; luego tendré que modificar el motor para operar con el formato nuevo (aunque debería funcionar out of the box con lo que hay, aunque esté desaprovechando las posibilidades).

20190823
========

Design - la fase de la lava. A ver, para pintar la lava me he preparado dos ristras de datos. La primera, `lava_top`, puede usarse directamente añadiéndole el prefijo de la ubicación a una secuencia de actualización. La segunda, `lava_body`, contiene 64 bytes puestos al azar (de entre los patrones de la lava) y está pensada para que se añadan 32 bytes de estos 64 empezando desde `(rand8 () & 0x1f)`.

El tema es ver como hacer esto.

Veamos, si la lava está activa va a tener una "altura" de lava. Pongamos que la fase tiene 10 pantallas de alto, podemos expresar la altura en patrones. El valor representará donde está pintado el "borde de la lava".

Al entrar en una pantalla hay que ver si la lava está en la pantalla: Hay que dividir esa "altura del borde de la lava" entre 24. Me busco una rutina en ASM que lo haga en 0.2 de las que vi hace un tiempo y listo:

También necesito el módulo, porque tendré que pintar el borde y el resto de la lava, para lo cual igualmente puedo usar `lava_top` y `lava_body` con la pantalla apagada.

Aparte de esto, la lava debe subir. Cada ¿segundo?, se decrementa uno de la posición del borde de la lava, y se pinta el borde de la lava en su sitio y una franja de cuerpo de lava cubriendo el borde anterior: creo que en VBLANK hay tiempo de sobra para esto. Obviamente me ayudo de una variable de la altura *en esta pantalla* que he precalculado debidamente en el proceso anterior.

Si resulta que el pry de ninjajar excede el precalculado de la altura de la lava en esta pantalla * 8 - 16, entonces resulta que nos ha pillado la lava.

Más o menos eso. Voy a buscar las rutinas de división y modulo.

### División entre 24

```s
	;Divide by 24 by Omegamatrix
	;15 bytes, 27 cycles
		lsr
		lsr
		lsr
		sta _rda
		lsr
		lsr
		adc _rda
		ror
		lsr
		adc _rda
		ror
		lsr

		sta _rda
```

Para implementar el módulo me temo que tendré que coger el resultado `r` y hacer `v - ((r << 4) + (r << 3))`, o sea, restar del valor original el resultado por 24.

```
		lda _rda
		asl a
		asl a
		asl a
		sta _rdb
		asl a
		clc
		adc _rdb
		sta _rdb

		lda _lava_height_absolute
		sec
		sbc _rdb
		sta _lava_height_on_screen
```

Pero antes tengo que montar la fase en mappy, de 10 pantallas de alto, y no se si tengo demasiadas ganas rait nau.

oOo

20190826
========

Veamos, puedo usar el tileset, qué se yo, de los pantanos para esto. O quizá mejor el de las fases de nadar? La última fase que hice fue un pantano, así que mejor usamos otra... La de la playa pepinona, pues.

10 pantallas de alto. El tema consiste en que hay que activar un touchplate en la pantalla de abajo del todo que abre la puerta de la pantalla de arriba, pero además activa la lava. Voy a hacer la fase primero, programar el touchplate (necesitaré un flag) y luego ya si eso ya.

Para ahorrar toneladas de memoria, vamos a hacer que el custom renderer ponga tiles 5 (22) en los extremos antes de hacer nada (y dejamos que luego se procese normalmente para meter el tile 23 random). Así que tengo que hacer de cuenta que la fase va a tener rocas en los laterales si no hay nada (tile 0).

20200216
========

Vamos ver si terminamos esto para la compo. La idea es pasarlo a UNROM64 y luego ver todo lo que se puede hacer. Para organizarme bien los tiestos primero voy a ver cuanto ocupa la OGT de Davidian.

Estas son las duraciones de las canciones:

Output format: CA65, all songs in single file
Input format: FamiTracker export
Header:     159
Instrument: 250
Sub song 0: 265
Sub song 1: 69
Sub song 2: 1604
Sub song 3: 1086
Sub song 4: 910
Sub song 5: 1061
Sub song 6: 666
Sub song 7: 1100
Sub song 8: 70
Sub song 9: 674
Sub song 10: 22

Total data size: 7936 bytes

Son menos de 8K. Si comprimo los gráficos podrían compartir banco con holgura. Entonces quedaría así:

* bank0 - música y graficos
* bank1 - level data
* bank2 - extra routines
* bank3 (fixed) main routines

Encasquetar el level data en el banco 1 se refiere a meter pantallas y enemigos, por los que las funciones que cargan las pantallas deberán estar en el banco fijo. En bank2 meteré cosas más accesoras como las pantallas fijas y mierdas que no necesiten usar datos de nivel directamente.

Lo primero será pues hacer un nuevo `/dev` y empezar a organizar lo que hay en bancos, poner bien la paginación y ver si todo tira. Y a partir de ahí, terminar el juego y pulirlo.

Orden de las fases
==================

0 (bosque fácil)
1 (desierto fácil)
4 (pantano fácil)
3 (nadal)

2 (cueva)
5 (bosque denoche)
9 (lava 1)
6 (nadal 2)

8 (pantano chungo)
A (lava 2)
B (hielo)
7 (castillo 1)

~

FIx
===

[ ] ¿Dónde ha ido el hud?
[ ] Hacer más permisiva la colisión con el coco