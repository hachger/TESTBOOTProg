/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_cdc_if.c
  * @version        : v2.0_Cube
  * @brief          : Usb device for Virtual Com Port.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_if.h"

/* USER CODE BEGIN INCLUDE */

/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device library.
  * @{
  */

/** @addtogroup USBD_CDC_IF
  * @{
  */

/** @defgroup USBD_CDC_IF_Private_TypesDefinitions USBD_CDC_IF_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */
typedef struct cmd{
	char name[9];
	uint8_t (*ptrFun)(char *parameter);
	struct cmd *nextCmd;
} _sCmd;

typedef void (*ReceiveFun)(uint8_t *, uint32_t);
/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Defines USBD_CDC_IF_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */
#define SIZESTRFUNPARAM		32
/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Macros USBD_CDC_IF_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_Variables USBD_CDC_IF_Private_Variables
  * @brief Private variables.
  * @{
  */
/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/** Received data over USB are stored in this buffer      */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/** Data to send over USB CDC are stored in this buffer   */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* USER CODE BEGIN PRIVATE_VARIABLES */
static ReceiveFun aReceiveFun = NULL;
static _sCmd *cmds = NULL;
static _sCmd *aux;
static uint8_t bufAux[256];
static char strFunParam[SIZESTRFUNPARAM] = {0};

uint8_t lineCodingBuf[7];

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Exported_Variables USBD_CDC_IF_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CDC_IF_Private_FunctionPrototypes USBD_CDC_IF_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CDC_Init_FS(void);
static int8_t CDC_DeInit_FS(void);
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t CDC_Receive_FS(uint8_t* pbuf, uint32_t *Len);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
static uint8_t CMDHelp(char *parameter);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_CDC_ItfTypeDef USBD_Interface_fops_FS =
{
  CDC_Init_FS,
  CDC_DeInit_FS,
  CDC_Control_FS,
  CDC_Receive_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the CDC media low layer over the FS USB IP
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Init_FS(void)
{
  /* USER CODE BEGIN 3 */
  /* Set Application Buffers */
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  DeInitializes the CDC media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_DeInit_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  /* USER CODE BEGIN 5 */
  switch(cmd)
  {
    case CDC_SEND_ENCAPSULATED_COMMAND:

    break;

    case CDC_GET_ENCAPSULATED_RESPONSE:

    break;

    case CDC_SET_COMM_FEATURE:

    break;

    case CDC_GET_COMM_FEATURE:

    break;

    case CDC_CLEAR_COMM_FEATURE:

    break;

  /*******************************************************************************/
  /* Line Coding Structure                                                       */
  /*-----------------------------------------------------------------------------*/
  /* Offset | Field       | Size | Value  | Description                          */
  /* 0      | dwDTERate   |   4  | Number |Data terminal rate, in bits per second*/
  /* 4      | bCharFormat |   1  | Number | Stop bits                            */
  /*                                        0 - 1 Stop bit                       */
  /*                                        1 - 1.5 Stop bits                    */
  /*                                        2 - 2 Stop bits                      */
  /* 5      | bParityType |  1   | Number | Parity                               */
  /*                                        0 - None                             */
  /*                                        1 - Odd                              */
  /*                                        2 - Even                             */
  /*                                        3 - Mark                             */
  /*                                        4 - Space                            */
  /* 6      | bDataBits  |   1   | Number Data bits (5, 6, 7, 8 or 16).          */
  /*******************************************************************************/
    case CDC_SET_LINE_CODING:
    	lineCodingBuf[0] = pbuf[0];
    	lineCodingBuf[1] = pbuf[1];
    	lineCodingBuf[2] = pbuf[2];
    	lineCodingBuf[3] = pbuf[3];
    	lineCodingBuf[4] = pbuf[4];
    	lineCodingBuf[5] = pbuf[5];
    	lineCodingBuf[6] = pbuf[6];

    break;

    case CDC_GET_LINE_CODING:
    	pbuf[0] = lineCodingBuf[0];
    	pbuf[1] = lineCodingBuf[1];
    	pbuf[2] = lineCodingBuf[2];
    	pbuf[3] = lineCodingBuf[3];
    	pbuf[4] = lineCodingBuf[4];
    	pbuf[5] = lineCodingBuf[5];
    	pbuf[6] = lineCodingBuf[6];
    break;

    case CDC_SET_CONTROL_LINE_STATE:

    break;

    case CDC_SEND_BREAK:

    break;

  default:
    break;
  }

  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Data received over USB OUT endpoint are sent over CDC interface
  *         through this function.
  *
  *         @note
  *         This function will issue a NAK packet on any OUT packet received on
  *         USB endpoint until exiting this function. If you exit this function
  *         before transfer is complete on CDC interface (ie. using DMA controller)
  *         it will result in receiving more data while previous ones are still
  *         not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
  /* USER CODE BEGIN 6 */
	if(aReceiveFun != NULL)
		aReceiveFun(Buf, *Len);

	if(cmds != NULL){
		uint8_t i = 0;
		uint8_t param = 0;

		for(uint32_t j=0; j<*Len; j++){
			strFunParam[i] = Buf[j];
			if(param){
				if(strFunParam[i] == '\r'){
					if(aux->ptrFun(strFunParam)){
						strcpy((char *)bufAux, "\nOK\n");
						CDC_Transmit_FS(bufAux, 4);
					}
					else{
						strcpy((char *)bufAux, "\nERROR\n");
						CDC_Transmit_FS(bufAux, 7);
					}
					i = 255;
				}
			}
			else{
				if(strFunParam[i]==' ' || strFunParam[i]=='\r'){
					strFunParam[i] = '\0';
					aux = cmds;
					while(aux){
						if(strncmp(aux->name, strFunParam, 9) == 0)
							break;
						aux = aux->nextCmd;
					}
					i = 255;
					param = 0;
					if(aux == NULL)
						CMDHelp(NULL);
					else{
						if(Buf[j] == ' ')
							param = 1;
						else{
							aux->ptrFun(NULL);
							strcpy((char *)bufAux, "\nOK\n");
							CDC_Transmit_FS(bufAux, 4);
						}
					}
				}
			}
			i++;
			if(i == SIZESTRFUNPARAM){
				i = 0;
				param = 0;
			}
		}
	}

  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
  return (USBD_OK);
  /* USER CODE END 6 */
}

/**
  * @brief  CDC_Transmit_FS
  *         Data to send over USB IN endpoint are sent over CDC interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be sent
  * @param  Len: Number of data to be sent (in bytes)
  * @retval USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  /* USER CODE BEGIN 7 */
  USBD_CDC_HandleTypeDef *hcdc = (USBD_CDC_HandleTypeDef*)hUsbDeviceFS.pClassData;
  if (hcdc->TxState != 0){
    return USBD_BUSY;
  }
  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  /* USER CODE END 7 */
  return result;
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
void CDC_Attach_Receive_FS(void (*pFun)(uint8_t* Buf, uint32_t Len)){
	aReceiveFun = pFun;
}

uint8_t CDC_CMD_Activate(){
	if(cmds != NULL)
		return 1;

	aux = (_sCmd *)malloc(sizeof(_sCmd));

	if(aux == NULL)
		return 0;

	strcpy(aux->name, "?");
	aux->ptrFun = CMDHelp;
	aux->nextCmd = NULL;

	cmds = aux;

	return 1;
}

void CDC_CMD_Deactivate(){
	while(cmds){
		aux = cmds->nextCmd;
		free(cmds);
		cmds = aux;
	}

	cmds = NULL;
}

uint8_t CDC_CMD_State(){
	if(cmds == NULL)
		return 0;
	return 1;
}

uint8_t CDC_CMD_Add(const char *cmdName, uint8_t (*ptrCMDFun)(char *parameter)){
	aux = (_sCmd *)malloc(sizeof(_sCmd));

	if(aux == NULL)
		return 0;

	strncpy(aux->name, cmdName, 8);
	aux->name[8] = '\0';
	aux->ptrFun = ptrCMDFun;
	aux->nextCmd = cmds;

	cmds = aux;

	return 1;
}

static uint8_t CMDHelp(char *parameter){
	UNUSED(parameter);

	uint8_t i = 0;
	uint8_t j;

	aux = cmds;
	while(aux){
		j = 0;
		while(aux->name[j]){
			bufAux[i++] = aux->name[j];
			j++;
		}
		bufAux[i++] = '\n';
		aux = aux->nextCmd;
	}

	CDC_Transmit_FS(bufAux, i);

	return 1;
}

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */
