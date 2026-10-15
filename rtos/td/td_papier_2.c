/* Exercice 1 */
void task1(void * p) {
    for(;;) {
        process1(); // dure 4ms
        vTaskDelay(4);
    }
}
void task2(void * p) {
    for(;;) {
        process2(); // dure 2ms
        vTaskDelay(8);
    }
}

/* Exercice 3 */
void taskSync (void * p) {
    int i = 0;
    for (;;) {
        if (i % 8 == 0) {
            // Débloquer t1 (donner sem1) toutes les 8ms
            xSemaphoreGive(sem1);
        }
        if (i % 10 == 0) {
            // Débloquer t2 (donner sem2) toutes les 10ms
            xSemaphoreGive(sem2);
        }

        i++;
        vTaskDelay(1);
    }
}

void task1(void * p) {
    for(;;) {
        // Débloquer toutes les 8ms
        xSemaphoreTake(sem1, portMAX_DELAY);
        process1(); // dure 4ms
    }
}
void task2(void * p) {
    for(;;) {
        // Débloquer toutes les 10ms
        xSemaphoreTake(sem2, portMAX_DELAY);
        process2(); // dure 2ms
    }
}

/* Exercice 7 */
void taskSync (void * p) {
    int i = 0;
    for (;;) {
        if (i % 8 == 0) {
            // Débloquer t1 (donner sem1) toutes les 8ms
            xSemaphoreGive(sem1);
        }
        if (i % 10 == 0) {
            // Débloquer t2 (donner sem2) toutes les 10ms
            xSemaphoreGive(sem2);
        }

        i++;
        vTaskDelay(1);
    }
}

void task1(void * p) {
    for(;;) {
        // Débloquer toutes les 8ms
        xSemaphoreTake(sem1, portMAX_DELAY);

        xSemaphoreTake(mutex, portMAX_DELAY);
        process1(); // dure 4ms
        xSemaphoreGive(mutex);
    }
}
void task2(void * p) {
    for(;;) {
        // Débloquer toutes les 10ms
        xSemaphoreTake(sem2, portMAX_DELAY);

        xSemaphoreTake(mutex, portMAX_DELAY);
        process2(); // dure 2ms
        xSemaphoreGive(mutex);
    }
}
