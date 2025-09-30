# RETO 4
### Colas de mensajes para comunicación entre tareas
Objetivo: Entender cómo las tareas se comunican de manera ordenada.

Descripción:

Crear una tarea de lectura de sensor (por ejemplo, temperatura con un DHT11).
Esta tarea envía los valores a través de una cola a otra tarea.
La tarea de procesamiento recibe los datos de la cola y:
Los muestra en Serial.
Enciende un LED si la temperatura pasa cierto umbral.
Implementar además una tarea de "monitoreo" que corra en paralelo y muestre un "sistema en funcionamiento".
## Explicación del codigo
Este codigo se realizo de la siguiente manera:
### prueba