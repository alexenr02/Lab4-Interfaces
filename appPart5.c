/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

/*****************************************
 * Buffer required for reading and sending
 * data over CDC
 *****************************************/
 uint8_t APP_MAKE_BUFFER_DMA_READY readBuffer[64];

/**********************
 * Switch Prompt.
 ***********************/
const uint8_t __attribute__((aligned(16))) switchPrompt[] = "\r\nPUSH BUTTON PRESSED";
uint8_t APP_MAKE_BUFFER_DMA_READY miString[] = "                                 ";

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

//********************** LRSG v

#define TRANS_COUNT 10
#define EDO_COUNT 17
/*
char chr=0;
int acum1=0;
int acum2=0;
int res=0;

*/

char chr = 0;
int contador;
float acum1 = 0;
float acum2 = 0;
float num1acum = 0;
float acum3 = 0;
float acum4 = 0;
float temp = 0;
float num2acum = 0;
float res = 0;
int potencia = 0;
bool banderaNegativo1 = false;
bool banderaNegativo2 = false;
enum Oper{Suma,Resta,Mult,Div};
enum Oper oper;



int edo=0;
int edoAnt=0;
int trans=0;
int miPrintf_flag=0;
int miStringCont=0;
char auxString[] = "                                 ";

					//0-->Inválida
					//6-->Digito
					//7-->Operador
int chrTrans[TRANS_COUNT]=
					{ 0,'(',')','=',  8, 27,'.', 6 , 7 ,'~' };
int mtzTrans[EDO_COUNT][TRANS_COUNT]={
					{ 0, 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
					{ 1, 1 , 1 , 1 , 99, 99, 1 , 2 , 1 , 15 },
					{ 2, 2 , 2 , 2 , 99, 99, 4 , 3 , 2 , 2  },
					{ 3, 2 , 2 , 2 , 99, 99, 3 , 2 , 2 , 2  },
					{ 4, 4 , 4 , 4 , 99, 99, 4 , 5 , 4 , 4  },
					{ 5, 5 , 5 , 5 , 99, 99, 5 , 6 , 7 , 5  },
					{ 6, 5 , 5 , 5 , 99, 99, 6 , 5 , 5 , 6  },
					{ 7, 7 , 7 , 7 , 99, 99, 7 , 8 , 7 , 16 },
					{ 8, 8 , 8 , 8 , 99, 99, 10, 9 , 8 , 8  },
					{ 9, 9 , 9 , 9 , 99, 99, 9 , 8 , 9 , 9  },
					{ 10,10, 10, 10, 99, 99, 10, 11,10 , 10 },
					{ 11,11, 13, 11, 99, 99, 11, 12,11 , 11 },
					{ 12,12, 12, 12, 99, 99, 12, 11,12 , 12 },
					{ 13,13, 13, 14, 99, 99, 13, 13,13 , 13 },
					{ 14, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0  },
					{ 15,15, 15, 15, 99, 99, 2 , 2 , 2 , 15 },
					{ 16,16, 16, 16, 99, 99, 8 , 8 , 8 , 16 }

};
void miPrintf(char* s, int cont) {
    int i;
    for (i=0;i<cont;i++)
        miString[i]=s[i];
    miPrintf_flag=1;
    miStringCont=cont;
}

int calcTrans(char chr) {
	int trans=0;
	if ((chr>='0')&&(chr<='9'))	//Digito
		return(7);
	switch (chr) {
		case'+':
		case'-':
		case'*':
		case'/':
				return(8);
	}
	for (trans=9;trans>0;trans--)
		if (chr==chrTrans[trans])
			break;
	return(trans);
}

int sigEdo(int edo, int trans) {
	return(mtzTrans[edo][trans]);
}

int ejecutaEdo(int edo) {
    static int i=0;
    //static int i2=0;
    static int negativoFlag=0;
    static int digitosCont=0;
    static int auxDigitosCont=0;
    //static int aux2DigitosCont=0;
    //static int decimalesCont=0;
    static int auxRes=0;
    static float auxRes2=0;
    
	switch(edo) {
		case 0:
				break;
		case 1:
                BSP_LEDOff( APP_USB_LED_1);
                BSP_LEDOff( APP_USB_LED_2);
                BSP_LEDOff( APP_USB_LED_3);
				acum1 = 0;
                acum2 = 0;
				miPrintf(&chr,1);
				break;
		case 2:
				miPrintf(&chr,1);
				acum1*=10;
				acum1+=(chr-'0');
				break;
		case 3:
				miPrintf(&chr,1);
				acum1*=10;
				acum1+=(chr-'0');
				return(2);
                
        case 4: 
                miPrintf(&chr,1);
                break;
        case 5: 
                potencia++;
                miPrintf(&chr,1);
                acum2 *= 10;
                acum2 += (chr - '0');
                break;
        case 6: 
                potencia++;
                miPrintf(&chr,1);
                acum2 *= 10;
                acum2 += (chr - '0');
                return(5);
        
		case 7:
                BSP_LEDOn(  APP_USB_LED_1);
                BSP_LEDOff( APP_USB_LED_2);
                BSP_LEDOff( APP_USB_LED_3);
                acum2 = acum2 * (float)pow(10, -potencia);
                num1acum = acum1 + acum2;
                potencia = 0;
				miPrintf(&chr,1);
				switch (chr) {
					case'+':
							oper=Suma;
							break;
					case'-':
							oper=Resta;
							break;
					case'*':
							oper=Mult;
							break;
					case'/':
							oper=Div;
							break;
				}
				acum3 = 0;	//Preparar la entrada al estado 8
                acum4 = 0;
                contador = 0;
				break;
        
		case 8:
                BSP_LEDOff( APP_USB_LED_1);
                BSP_LEDOn(  APP_USB_LED_2);
                BSP_LEDOff( APP_USB_LED_3);
				miPrintf(&chr,1);
				acum3 *= 10;
            	acum3 += (chr - '0');
				break;
		case 9:
				miPrintf(&chr,1);
				acum3 *= 10;
                acum3 += (chr - '0');
                return(8);
        
		case 10:
                BSP_LEDOff( APP_USB_LED_1);
                BSP_LEDOff( APP_USB_LED_2);
                BSP_LEDOn(  APP_USB_LED_3);
				miPrintf(&chr,1);
				break;
        case 11:
                potencia++;
                miPrintf(&chr,1);
                acum4 *= 10;
                acum4 += (chr - '0');
                break;
        case 12:
                potencia++;
                 miPrintf(&chr,1);
                acum4 *= 10;
                acum4 += (chr - '0');
                return(11);
        case 13:
                acum4 = acum4 * (float)pow(10, -potencia);
                num2acum = acum3 + acum4;
                potencia = 0;
                miPrintf(&chr,1);
                break;
		case 14:
                BSP_LEDOn( APP_USB_LED_1);
                BSP_LEDOn( APP_USB_LED_2);
                BSP_LEDOn( APP_USB_LED_3);
                if (banderaNegativo1==true)
                {
                    num1acum = -1 * num1acum;
                }
                if (banderaNegativo2==true)
                {
                    num2acum = -1 * num2acum;
                }
				switch(oper) {
					case Suma:
                        res = num1acum + num2acum;
                        break;
                    case Resta:
                        res = num1acum - num2acum;
                        break;
                    case Mult:
                        res = num1acum * num2acum;
                        break;
                    case Div:
                        if (num2acum)
                            res = num1acum / num2acum;
                        else
                            res = -1;
                        break;
				}
				//printf("%d\n",res);
                if (res<0) {
                    negativoFlag=1;
                    res=-1*res;
                    
                } else {
                    negativoFlag=0;
                }
                auxRes=res;
                auxRes2=res;
                digitosCont=0;
                do {
                    auxRes/=10;
                    digitosCont++;
                } while(auxRes);
                i=digitosCont;
                
                auxDigitosCont=digitosCont;
                //aux2DigitosCont=digitosCont;
                do {
                    auxString[negativoFlag+i]='0'+fmod(res,10);
                    res/=10;
                } while(--i>=0);
               
                auxRes2=fmod(auxRes2,1); // quitarle los digitos enteros a una variable auxiliar que tiene guardado el resultado con decimales
                auxRes2=auxRes2*10*10*10*10; // 4  decimales
                auxRes=auxRes2; //para corregir error de sobrerredondeo
                
                auxString[0]='=';
                if (negativoFlag) {
                    auxString[1]='-';
                    auxString[auxDigitosCont+2]='.'; // + 2 por el igual y el negativo
                    i=3;
                    do {
                        auxString[auxDigitosCont+3+i]='0'+fmod(auxRes,10);
                        auxRes/=10;
                    } while(--i>=0);
                }else
                {
                    auxString[auxDigitosCont+1]='.';
                    i=3;
                     do {
                         auxString[auxDigitosCont+2+i]='0'+fmod(auxRes,10);
                         auxRes/=10;
                     } while(--i>=0);
                    
                }
                
                auxString[digitosCont+1+negativoFlag+8]=0x0A; //New Line
                auxString[digitosCont+1+negativoFlag+9]=0x0D; //Carriage Return
                miPrintf(&auxString[0],digitosCont+1+negativoFlag+10);
                banderaNegativo1 = false;
                banderaNegativo2 = false;
				return(0);
        case 15:
                miPrintf(&chr,1);
                banderaNegativo1 = true;
                break;
        case 16:
                miPrintf(&chr,1);
                banderaNegativo2 = true;
                break;
		case 99:
				//printf("\n<<<Captura cancelada>>>\n");
				return(0);	//Estado aceptor, rompe la rutina y marca estado de salida
	}
	return(edo);	//Para estados no aceptores regresar el estado ejecutado
}

//********************** LRSG 

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/************************************************
 * CDC COM1 Application Event Handler
 ************************************************/

void APP_USBDeviceCDCEventHandler
(
    USB_DEVICE_CDC_INDEX index ,
    USB_DEVICE_CDC_EVENT event ,
    void* pData,
    uintptr_t userData
)
{

    APP_DATA * appDataObject;
    appDataObject = (APP_DATA *)userData;
    USB_CDC_CONTROL_LINE_STATE * controlLineStateData;
    USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE * eventDataRead; 
    
    switch ( event )
    {
        case USB_DEVICE_CDC_EVENT_GET_LINE_CODING:

            /* This means the host wants to know the current line
             * coding. This is a control transfer request. Use the
             * USB_DEVICE_ControlSend() function to send the data to
             * host.  */

            USB_DEVICE_ControlSend(appDataObject->deviceHandle,
                    (uint8_t *)&appDataObject->getLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_LINE_CODING:

            /* This means the host wants to set the line coding.
             * This is a control transfer request. Use the
             * USB_DEVICE_ControlReceive() function to receive the
             * data from the host */

            USB_DEVICE_ControlReceive(appDataObject->deviceHandle,
                    (uint8_t *) &appDataObject->setLineCodingData, sizeof(USB_CDC_LINE_CODING));

            break;

        case USB_DEVICE_CDC_EVENT_SET_CONTROL_LINE_STATE:

            /* This means the host is setting the control line state.
             * Read the control line state. We will accept this request
             * for now. */

            controlLineStateData = (USB_CDC_CONTROL_LINE_STATE *)pData;
            appDataObject->controlLineStateData.dtr = controlLineStateData->dtr;
            appDataObject->controlLineStateData.carrier = controlLineStateData->carrier;

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);

            break;

        case USB_DEVICE_CDC_EVENT_SEND_BREAK:

            /* This means that the host is requesting that a break of the
             * specified duration be sent. Read the break duration */

            appDataObject->breakData = ((USB_DEVICE_CDC_EVENT_DATA_SEND_BREAK *)pData)->breakDuration;
            
            /* Complete the control transfer by sending a ZLP  */
            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            
            break;

        case USB_DEVICE_CDC_EVENT_READ_COMPLETE:

            /* This means that the host has sent some data*/
            eventDataRead = (USB_DEVICE_CDC_EVENT_DATA_READ_COMPLETE *)pData;
            appData.isReadComplete = true;
            appDataObject->numBytesRead = eventDataRead->length; 
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_RECEIVED:

            /* The data stage of the last control transfer is
             * complete. For now we accept all the data */

            USB_DEVICE_ControlStatus(appDataObject->deviceHandle, USB_DEVICE_CONTROL_STATUS_OK);
            break;

        case USB_DEVICE_CDC_EVENT_CONTROL_TRANSFER_DATA_SENT:

            /* This means the GET LINE CODING function data is valid. We dont
             * do much with this data in this demo. */

            break;

        case USB_DEVICE_CDC_EVENT_WRITE_COMPLETE:

            /* This means that the data write got completed. We can schedule
             * the next read. */

            appData.isWriteComplete = true;

            break;

        default:
            break;
    }
}

/*************************************************
 * Application Device Layer Event Handler
 *************************************************/

void APP_USBDeviceEventCallBack ( USB_DEVICE_EVENT event, void * eventData, uintptr_t context )
{
    uint8_t configurationValue;

    switch ( event )
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* USB device is reset or device is deconfigured.  This means that
             * USB device layer is about to deininitialize all function drivers.
             * Update LEDs indication */

            BSP_LEDOn ( APP_USB_LED_1 );
            BSP_LEDOn ( APP_USB_LED_2 );
            BSP_LEDOff( APP_USB_LED_3 );

            appData.isConfigured = false;

            break;

        case USB_DEVICE_EVENT_SOF:

            /* This event is used for switch debounce. This flag is reset
             * by the switch process routine. */
            appData.sofEventHasOccurred = true;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:

            /* Check the configuration */
            configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED *)eventData)->configurationValue;;
            if (configurationValue == 1)
            {
                /* The device is in configured state */
                BSP_LEDOff ( APP_USB_LED_1 );
                BSP_LEDOff ( APP_USB_LED_2 );
                BSP_LEDOn  ( APP_USB_LED_3 );

                /* Register the CDC Device application event handler here.
                 * Note how the appData object pointer is passed as the
                 * user data */

                USB_DEVICE_CDC_EventHandlerSet(USB_DEVICE_CDC_INDEX_0, APP_USBDeviceCDCEventHandler, (uintptr_t)&appData);

                /* Mark that set configuration is complete */
                appData.isConfigured = true;

            }
            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            /* Update LED indication */
            BSP_LEDOff ( APP_USB_LED_1 );
            BSP_LEDOn ( APP_USB_LED_2 );
            BSP_LEDOn  ( APP_USB_LED_3 );
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */
            USB_DEVICE_Attach(appData.deviceHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available any more. Detach the device. */
            USB_DEVICE_Detach(appData.deviceHandle);
            break;

        /* These events are not used in this demo */
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

/************************************************
 * Application State Reset Function
 ************************************************/

bool APP_StateReset(void)
{
    /* This function returns true if the device
     * was reset  */

    bool retVal;

    if(!appData.isConfigured)
    {
        appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
        appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
        appData.isReadComplete = true;
        appData.isWriteComplete = true;
        retVal = true;
    }
    else
    {
        retVal = false;
    }

    return(retVal);
}

/************************************************
 * Switch Procesing routine
 ************************************************/
void APP_SwitchStateProcess(void)
{
    /* Check if a key was pressed */
    if(BSP_SWITCH_STATE_PRESSED == (BSP_SwitchStateGet(APP_USB_SWITCH_1)))
    {
        if(appData.ignoreSwitchPress)
        {
            /* This measn the key press is in progress */
            if(appData.sofEventHasOccurred)
            {
                /* A timer event has occurred. Update the debounce timer */
                appData.switchDebounceTimer ++;
                appData.sofEventHasOccurred = false;
                if(appData.switchDebounceTimer == appData.debounceCount)
                {
                    /* Indicate that we have valid switch press. The switch is
                     * pressed flag will be cleared by the application tasks
                     * routine. We should be ready for the next key press.*/
                    appData.isSwitchPressed = true;
                    appData.switchDebounceTimer = 0;
                    appData.ignoreSwitchPress = false;
                }
            }
        }
        else
        {
            /* We have a fresh key press */
            appData.ignoreSwitchPress = true;
            appData.switchDebounceTimer = 0;
        }
    }
    else
    {
        /* No key press. Reset all the indicators. */
        appData.ignoreSwitchPress = false;
        appData.switchDebounceTimer = 0;
        appData.sofEventHasOccurred = false;
    }
}



// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    
    /* Device Layer Handle  */
    appData.deviceHandle = USB_DEVICE_HANDLE_INVALID;

    /* Initial get line coding state */
    appData.getLineCodingData.bDataBits = 8;
    appData.getLineCodingData.dwDTERate = 9600;
    appData.getLineCodingData.bCharFormat = 0;
    appData.getLineCodingData.bParityType = 0;

    /* Read Transfer Handle */
    appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Write Transfer Handle */
    appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;

    /* Initialize the application flags */
    appData.isReadComplete     = true;
    appData.isWriteComplete    = true;
    appData.ignoreSwitchPress  = false;
    appData.isSwitchPressed    = false;
    appData.isConfigured = false;
    appData.sofEventHasOccurred = false;

    /* Initialize all the switch related parameters */
    appData.switchDebounceTimer = 0;

    /* Assign the read buffer */
    appData.readBuffer = &readBuffer[0];
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks (void)
{
    /* Update the application state machine based
     * on the current state */
    int i; 
    /* Update the switch press */
    APP_SwitchStateProcess();

    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.deviceHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

            if(appData.deviceHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.deviceHandle, APP_USBDeviceEventCallBack, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            /* Check if the device was configured */
            if(appData.isConfigured)
            {
                /* If the device is configured then lets start reading */
                appData.state = APP_STATE_SCHEDULE_READ;
            }
            break;

        case APP_STATE_SCHEDULE_READ:

            if(APP_StateReset())
            {
                break;
            }

            /* If a read is complete, then schedule a read
             * else wait for the current read to complete */

            appData.state = APP_STATE_WAIT_FOR_READ_COMPLETE;
            if(appData.isReadComplete)
            {
                appData.isReadComplete = false;
                appData.readTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
                USB_DEVICE_CDC_Read (USB_DEVICE_CDC_INDEX_0, &appData.readTransferHandle, appData.readBuffer, APP_READ_BUFFER_SIZE);
                if(appData.readTransferHandle == USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID)
                {
                    appData.state = APP_STATE_ERROR;
                    break;
                }
            }

            break;

        case APP_STATE_WAIT_FOR_READ_COMPLETE:
        case APP_STATE_CHECK_SWITCH_PRESSED:

            if(APP_StateReset())
            {
                break;
            }

            /* Check if a character was received or a switch was pressed.
             * The isReadComplete flag gets updated in the CDC event handler. */

            if( appData.isReadComplete|| appData.isSwitchPressed)
            {
                 appData.state = APP_STATE_SCHEDULE_WRITE;
            }

            break;

        case APP_STATE_SCHEDULE_WRITE:

            if(APP_StateReset())
            {
                break;
            }

            /* Setup the write */

            appData.writeTransferHandle = USB_DEVICE_CDC_TRANSFER_HANDLE_INVALID;
            appData.isWriteComplete = true;
            appData.state = APP_STATE_WAIT_FOR_WRITE_COMPLETE;

            if(appData.isSwitchPressed)
            {
                /* If the switch was pressed, then send the switch prompt*/
                appData.isSwitchPressed = false;
                USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0, &appData.writeTransferHandle,
                     switchPrompt, 23, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
            }
            else
            {
                /* Else echo each received character by adding 1 */
                for(i=0; i<appData.numBytesRead; i++)
                {
                    if((appData.readBuffer[i] != 0x0A) && (appData.readBuffer[i] != 0x0D))
                    {
                        //appData.readBuffer[i] = appData.readBuffer[i] + 1;
                        chr=appData.readBuffer[i];
                        trans=calcTrans(chr);	//Calcular la transición según la entrada del teclado
                        if (trans) {			//Validar por transición valida (la transición 0 es inválida)
                            edoAnt=edo;					//Guardar el estado anterior
                            edo=sigEdo(edoAnt,trans);	//Calcular el siguiente estado
                            if (edoAnt!=edo)			//Solo si hay cambio de estado hay que ...
                                edo=ejecutaEdo(edo);	// ... ejecutar el nuevo estado y asignar estado de continuidad
                        }
                    }
                }
                if (miPrintf_flag) {
                    USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0, &appData.writeTransferHandle,
                                            miString, miStringCont, USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);
                    miPrintf_flag=0;
                    miStringCont=0;
                }
/*                USB_DEVICE_CDC_Write(USB_DEVICE_CDC_INDEX_0,
                        &appData.writeTransferHandle,
                        appData.readBuffer, appData.numBytesRead,
                        USB_DEVICE_CDC_TRANSFER_FLAGS_DATA_COMPLETE);*/
            }

            break;

        case APP_STATE_WAIT_FOR_WRITE_COMPLETE:

            if(APP_StateReset())
            {
                break;
            }

            /* Check if a character was sent. The isWriteComplete
             * flag gets updated in the CDC event handler */

            if(appData.isWriteComplete)
            {
                appData.state = APP_STATE_SCHEDULE_READ;

            }

            break;

        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}

 

/*******************************************************************************
 End of File
 */

