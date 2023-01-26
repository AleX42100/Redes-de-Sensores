

Proyecto Redes de Sensores Electrónicos



Introducción:


Este proyecto forma parte de la asignatura de redes de sensores electrónicos. Partiendo de una placa TTGO, incrustando entre otros un ESP32 y una pantalla OLED. También tenemos a nuestra disposición una unidad de medición inercial, modelo MPU9250, con un giroscopio, un acelerómetro y un magnetómetro, cada uno en 3 ejes. El objetivo inicial de este proyecto era poder identificar la actividad física realizada, entre la posición estática, caminar, correr y montar en bicicleta. Al final, no se incluyó el ciclismo. 
Además, podría haber sido interesante integrar el aprendizaje automático, pero tras el análisis mediante Excel, es relativamente sencillo identificar las actividades físicas únicamente mediante un umbral.


Extracción:


Este código incluye todas las bibliotecas necesarias para el proyecto. Empezamos declarando todos los parámetros que se utilizarán, luego inicializamos las funcionalidades que se utilizarán (WiFi, sensor, puerto serie, configuración de los registros del sensor, la hora, el servidor FTP). 
Luego, en el loop(), declaramos un objeto que permitirá construir el documento Json, que será enviado directamente al PC (que aloja un servidor Filezilla) vía WiFi. A continuación, realizamos las adquisiciones del sensor: una cada 0,1s durante 2s. Todos estos valores se almacenan, se formatean y se envían por FTP. El LED de a bordo parpadea cuando estamos a punto de enviar el archivo json. Además, tras el análisis en Excel, es posible determinar la actividad de forma muy sencilla a partir de la media de las mediciones realizadas. 
A continuación, el resultado se muestra en la pantalla OLED. El sensor se coloca mediante una banda elástica (para evitar movimientos no deseados) justo por encima de la rodilla.


Analysis:

El tratamiento de los datos se realiza a través de Excel, se recuperan una serie de muestras de mí quieto, caminando o corriendo. Trazamos las curvas y observamos lo siguiente:



 
 
 
(grafías obtenidas con una decena de los registros de cada actividad)




Aunque la idea inicial era poder aplicar el aprendizaje automático para determinar la actividad, se puede ver que es relativamente sencillo hacerlo estableciendo umbrales medios: 
-	Si la media <50: no nos movemos
-	Si la media es >50 y <150: la persona debe caminar definitivamente
-	Si la media >150: la persona debe correr definitivamente




Diagrama de funciones del codigo:


 



