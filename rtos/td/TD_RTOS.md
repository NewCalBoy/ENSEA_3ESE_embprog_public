# TD RTOS

## 1/ Tâches

```c
#include <stdio.h>
#include <FreeRTOS.h>
#define STACK_DEPTH 256
#define BASE_PRIO 1
int x = 0;
void codeTache(void* num) {
	int j;
	for (j = 0 ; j < 10 ; j++) {
		x++;
		printf("tache %d, j = %d, x = %d\r\n", (int)num, j, x);
		vTaskDelay(1);
	}
	vTaskDelete(NULL);
}
/* Tâche de priorité BASE_PRIO */
void init(void* param) {
	int i;
	TaskHandle_t xHandle = NULL;
	for (i = 0 ; i < 4 ; i++) {
		xTaskCreate(codeTache, "", STACK_SIZE, (void *) i, BASE_PRIO+i+1, &xHandle);
		printf("Creation de la tache %d\r\n", i);
	}
	vTaskDelete(NULL);
}
```

__1. Donner les 12 premières lignes qui s'affichent à l'écran__
```

```
__2. La fonction codeTache est-elle réentrante ?__
```

```
___

## 3/ Boîtes aux lettres

```c
#define BASE_PRIO 5
#define STACK_SIZE 1000
#define NBR_MAX 3
#define LONG_MAX 100
QueueHandle_t maBoite;
void tache2 (void* p) {
	char messBuf[LONG_MAX];
	int i;
	printf("t2: alive\r\n");
	for (i = 0 ; i < 5 ; i++) {
		xQueueReceive(maBoite, (void *)messBuf, portMAX_DELAY);
		printf("t2: R %s\r\n", messBuf);
	}
	printf("t2 out\r\n");
	vTaskDelete(NULL);
}
void tache1(void* p) {
	char msg[] = "msg 00";
	int i;
	printf("t1: alive\r\n");
	maBoite = xQueueCreate(NBR_MAX, LONG_MAX);
	printf("t1: created Queue\r\n");
	for (i = 0 ; i < 5 ; i++) {
		sprintf(msg, "msg %02d", i);
		printf("t1: S %s\r\n", msg);
		xQueueSend(maBoite, (const void *) msg, portMAX_DELAY);
	}
	printf("t1 out\r\n");
	vTaskDelete(NULL);
}
void init(void* p) {
	TaskHandle_t xHandle1 = NULL;
	TaskHandle_t xHandle2 = NULL;
	xTaskCreate(tache1, "Tache1", STACK_SIZE, NULL, BASE_PRIO-1, &xHandle1);
	xTaskCreate(tache2, "Tache2", STACK_SIZE, NULL, BASE_PRIO-2, &xHandle2);
	vTaskDelete(NULL);
}
```

__1) La tâche liée à la fonction init() est crée et le scheduleur est démarré. Qu’est-ce qui s’affiche
à l’écran ?__

```
t1: alive
t1: created Queue
t1: S msg 00
t1: S msg 01
t1: S msg 02
t1: S msg 03
t2: alive
t1: S msg 04
t2: R msg 00
t1 out
t2: R msg 01
t2: R msg 02
t2: R msg 03
t2: R msg 04
t2 out
```

___

## 2/ Sémaphores
```c

```
__4. Quelle doit être la priorité de taskSync ?__
```
Minimum 3 car doit être prioritaire
```