/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

/******************************************************************************
 * NOTE 1:  This project provides two demo applications.  A simple blinky
 * style project, and a more comprehensive test and demo application.  The
 * mainCREATE_SIMPLE_BLINKY_DEMO_ONLY setting in main.c is used to select
 * between the two.  See the notes on using mainCREATE_SIMPLE_BLINKY_DEMO_ONLY
 * in main.c.  This file implements the simply blinky style version.
 *
 * NOTE 2:  This file only contains the source code that is specific to the
 * basic demo.  Generic functions, such FreeRTOS hook functions, and functions
 * required to configure the hardware are defined in main.c.
 ******************************************************************************
 *
 * main_blinky() creates one queue, and two tasks.  It then starts the
 * scheduler.
 *
 * The Queue Send Task:
 * The queue send task is implemented by the prvQueueSendTask() function in
 * this file.  prvQueueSendTask() sits in a loop that causes it to repeatedly
 * block for 200 milliseconds, before sending the value 100 to the queue that
 * was created within main_blinky().  Once the value is sent, the task loops
 * back around to block for another 200 milliseconds...and so on.
 *
 * The Queue Receive Task:
 * The queue receive task is implemented by the prvQueueReceiveTask() function
 * in this file.  prvQueueReceiveTask() sits in a loop where it repeatedly
 * blocks on attempts to read data from the queue that was created within
 * main_blinky().  When data is received, the task checks the value of the
 * data, and if the value equals the expected 100, toggles an LED.  The 'block
 * time' parameter passed to the queue receive function specifies that the
 * task should be held in the Blocked state indefinitely to wait for data to
 * be available on the queue.  The queue receive task will only leave the
 * Blocked state when the queue send task writes to the queue.  As the queue
 * send task writes to the queue every 200 milliseconds, the queue receive
 * task leaves the Blocked state every 200 milliseconds, and therefore toggles
 * the LED every 200 milliseconds.
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* Standard demo includes. */
#include "partest.h"
#include <driverlib.h>

/* Priorities at which the tasks are created. */
#define mainQUEUE_RECEIVE_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2 )
#define mainQUEUE_SEND_TASK_PRIORITY        ( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue.  The 200ms value is converted
to ticks using the portTICK_PERIOD_MS constant. */
#define mainQUEUE_SEND_FREQUENCY_MS         ( pdMS_TO_TICKS( 200 ) )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH                    ( 1 )

/* The LED toggled by the Rx task. */
#define mainTASK_LED                        ( 1 )

/*-----------------------------------------------------------*/

/*
 * Called by main when mainCREATE_SIMPLE_BLINKY_DEMO_ONLY is set to 1 in
 * main.c.
 */
void blinky2( void );

/*
 * The tasks as described in the comments at the top of this file.
 */
//static void prvQueueReceiveTask( void *pvParameters );
//static void prvQueueSendTask( void *pvParameters );

static void ledToggleTask(void *pvParameters);
static void timerTask(void *pvParameters);


/*-----------------------------------------------------------*/

/* The queue used by both tasks. */
static QueueHandle_t xQueue = NULL;

volatile bool usiButton1, critical;

SemaphoreHandle_t semaKnop, semaTimer;

/*-----------------------------------------------------------*/

void blinky2( void )
{
    /* Create the queue. */
    xQueue = xQueueCreate( mainQUEUE_LENGTH, sizeof( bool ) );

    if( xQueue != NULL )
    {
        semaKnop = xSemaphoreCreateBinary();
        semaTimer = xSemaphoreCreateBinary();

        // configure timer
        TB0CTL |= TBCLR;
        TB0CTL = (TB0CTL | 0x0100 | ID__2 | TBIE); // ACLK/2, continuous mode, interrupt enabled and timer cleared.
        //TB0CCTL0 |= CCIE; // interrupt enable
        TB0CTL |= MC__CONTINUOUS;

        xTaskCreate( ledToggleTask,               /* The function that implements the task. */
                    "ledToggle",                               /* The text name assigned to the task - for debug only as it is not used by the kernel. */
                    configMINIMAL_STACK_SIZE,           /* The size of the stack to allocate to the task. */
                    NULL,                               /* The parameter passed to the task - not used in this case. */
                    1,                                  /* The priority assigned to the task. */
                    NULL );                             /* The task handle is not required, so NULL is passed. */
        xTaskCreate( timerTask, "timerTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL );



        /* Start the tasks and timer running. */
        vTaskStartScheduler();
    }

    /* If all is well, the scheduler will now be running, and the following
    line will never be reached.  If the following line does execute, then
    there was insufficient FreeRTOS heap memory available for the Idle and/or
    timer tasks to be created.  See the memory management section on the
    FreeRTOS web site for more details on the FreeRTOS heap
    http://www.freertos.org/a00111.html. */
    for( ;; );
}
/*-----------------------------------------------------------*/

static void ledToggleTask (void *pvParameters)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    for(;;)
    {
        //bool usiButton1 = GPIO_getInputPinValue ( GPIO_PORT_P1, GPIO_PIN1 );
        //xQueueReceiveFromISR( xQueue, &toggle, portMAX_DELAY );

        if ( xSemaphoreTake(semaKnop, portMAX_DELAY) )
        {
            GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );                 // Toggle LED on interrupt input
            //usiButton1 = 0;
            //toggle = 0;
        }
        vTaskDelay(100);
    }
    vTaskDelete(NULL);
}


static void timerTask(void *pvParamters)
{
    for(;;)
    {
        if ( xSemaphoreTake(semaTimer, portMAX_DELAY) )
        {
            GPIO_toggleOutputOnPin( GPIO_PORT_P4, GPIO_PIN6 );
        }
        vTaskDelay(100);
    }
}

/*-----------------------------------------------------------*/

#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void)
{
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
        case P1IV_NONE:   break;                               // None
      case P1IV_P1IFG0:                                      // Pin 0
          __no_operation();
             break;
       case (P1IV_P1IFG1):                                   // Pin 1 (button 2)
            xSemaphoreGiveFromISR(semaKnop, NULL);
            break;
       case P1IV_P1IFG2:                                       // Pin 2
            __no_operation();
            break;
       case P1IV_P1IFG3:                                       // Pin 3
            __no_operation();
            break;
       case P1IV_P1IFG4:                                       // Pin 4
            __no_operation();
            break;
       case P1IV_P1IFG5:                                       // Pin 5
            __no_operation();
            break;
       case P1IV_P1IFG6:                                       // Pin 6
            __no_operation();
            break;
       case P1IV_P1IFG7:                                       // Pin 7
            __no_operation();
            break;
       default:   _never_executed();
    }
}


#pragma vector=TIMER0_B0_VECTOR
__interrupt void timer1_ISR (void)
{
    //**************************************************************************
    // 4. Timer ISR and vector
    //**************************************************************************
    switch( __even_in_range( TB0IV, TB0IV_TBIFG )) {
     case TB0IV_NONE: break;                 // (0x00) None
     case TB0IV_TBCCR1:                      // (0x02) CCR1 IFG
         _no_operation();

           break;
     case TB0IV_TBCCR2:                      // (0x04) CCR2 IFG
          _no_operation();
           break;
     case TB0IV_TBCCR3: break;                    // (0x06) Reserved
     case TB0IV_TBCCR4: break;                    // (0x08) Reserved
     case TB0IV_TBCCR5: break;                    // (0x0A) Reserved
     case TB0IV_TBCCR6: break;                    // (0x0C) Reserved
     case TB0IV_TBIFG:                       // (0x0E) TA1IFG - TAR overflow
          // Toggle LED2 (Green) LED on/off
         xSemaphoreGiveFromISR(semaTimer, NULL);

          break;
     default: _never_executed();
    }
}