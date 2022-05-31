# Pràctica 4:  
#  _SISTEMAS OPERATIVOS EN TIEMPO REAL_

### Introducció:
_En aquesta pràctica introduim el concepte de "RTOs" (Real-Time Operating System), tracta sobre les tasques simultànies del processador, això proporciona que pugui relitzar tasques d'una manera més ràpida i eficient, sense haver d'esperar que acabi una per relitzar la següent._
# Exercici 1:

  
  
## Codi utilitzat:

```cpp
#include <Arduino.h>

void anotherTask( void * parameter );



void setup()
{
Serial.begin(112500);
/* we create a new task here */
xTaskCreate(
anotherTask, /* Task function. */
"another Task", /* name of task. */
10000, /* Stack size of task */
NULL, /* parameter of the task */
1, /* priority of the task */
NULL); /* Task handle to keep track of created task */
}
/* the forever loop() function is invoked by Arduino ESP32 loopTask */
void loop()
{
Serial.println("this is ESP32 Task");
delay(1000);
}
/* this function will be invoked when additionalTask was created */
void anotherTask( void * parameter )
{
/* loop forever */
for(;;)
{
Serial.println("this is another Task");
delay(1000);
}
/* delete a task when finish,
this will never happen because this is infinity loop */
vTaskDelete( NULL );
}

```
## Sortida del Port Sèrie:
  
  Després d'haver pujat el programa a la ESP32, per pantalla es mostren els missatges "this is ESP32 Task" i "This is another Task".
  El missatge "this is ESP32 Task" es mostra des del loop del programa i l'altre des d'un void "anotherTask" que executa un bucle for inifinit. Els dos missatges es mostren a la vegada ja que al estar en bucles sense un final, són infinits.

  ![](pantalla.jpg)


## Funcionament 

Primerament creem la tasca que utilitzarem dins del "void setup()" utilitzant el xTaskCreate:
```cpp
void setup()
{
Serial.begin(112500);

xTaskCreate(
    anotherTask, 
    "another Task", 
    10000, 
    NULL, 
    1, 
    NULL); 
}
```
> anotherTask és el nom del subprograma a cridar, "another Task" és el nom del programa, 10000 és el tamany en bytes, NULL és el paramentre a passar, 1 és un paramentre que dona la prioritat de la tasca (va de 1 a 24), NULL és l'identificador de la tasca

Aleshores tenim el "void loop()" en el qual fem el print de "this is ESP32 Task" i afegim un delay de 1000ms 
```cpp
void loop()
{
Serial.println("this is ESP32 Task");
delay(1000);
}
```
Finalment hem de crear el subprograma que cridem des de la tasca, el qual l'anomenenm "anotherTask", el qual conté un for(;;), és a dir un bucle infinit, que va imprimint per pantalla "This is another Task" i afegeix un delay de 1000ms
```cpp
void anotherTask( void * parameter )
{

    for(;;)
    {
    Serial.println("this is another Task");
    delay(1000);
    }
vTaskDelete( NULL );
}
```
El programa finalment anirà fent prints per pantalla mostrant els misatges esmentats anteriorment, com que els dos es mostren infinitament al ser bucles sense sortida, els missatges s'intercalaran en el temps que s'executi el delay de 1000ms que tenen a cada bucle. 