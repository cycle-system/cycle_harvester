/*!
 * @file bluetooth.h
 * @brief Header file for bluetooth module in ChibiosRT.
 *
 * @addtogroup BLUETOOTH
 * @{
 */
#ifndef BLUETOOTH_H_INCLUDED
#define BLUETOOTH_H_INCLUDED

#include <hal.h>
#include <sys/queue.h>
#include <stdlib.h>


#if HAL_USE_BLUETOOTH || defined(__DOXYGEN__) || 1

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Bluetooth configuration options
 * @{
 */
/**
 * @brief   Default bit rate.
 * @details Configuration parameter, this is the baud rate selected for the
 *          default configuration.
 */
#if !defined(BLUETOOTH_DEFAULT_BITRATE) || defined(__DOXYGEN__)
#define BLUETOOTH_DEFAULT_BITRATE      38400
#endif
/**
 * @brief   Maximum module name length.
 * @details Configuration parameter, this is the maximum name length for the bluetooth module.
 */
#if !defined(BLUETOOTH_MAX_NAME_LENGTH) || defined(__DOXYGEN__)
#define BLUETOOTH_MAX_NAME_LENGTH 32
#endif
/**
 * @brief   Maximum module pin length.
 * @details Configuration parameter, this is the maximum pin code length for the bluetooth module.
 */
#if !defined(BLUETOOTH_MAX_PINCODE_LENGTH) || defined(__DOXYGEN__)
#define BLUETOOTH_MAX_PINCODE_LENGTH 4
#endif
/**
 * @brief   Input buffer size.
 * @details Configuration parameter, this is the input buffer size
 */
#if !defined(BLUETOOTH_INPUT_BUFFER_LENGTH) || defined(__DOXYGEN__)
#define BLUETOOTH_INPUT_BUFFER_SIZE 128
#endif
/**
 * @brief   Output buffer size.
 * @details Configuration parameter, this is the output buffer size
 */
#if !defined(BLUETOOTH_OUTPUT_BUFFER_LENGTH) || defined(__DOXYGEN__)
#define BLUETOOTH_OUTPUT_BUFFER_SIZE 128
#endif

/** @} */

/*===========================================================================*/
/* Forward declarations                                                      */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief Communication standard bit rates.
 *
 *  Using these instead of writing the bit rate to a 32 bit int should help with 8 and 16 bit compatibility and portability
 */
enum btbitrate_t{
  b1200 = 0,
  b2400 = 1,
  b4800 = 2,
  b9600 = 3,
  b19200 = 4,
  b38400 = 5,
  b57600 = 6,
  b115200 = 7
};

/**
 * @brief Available bluetooth modules to use
 *
 * The "nomodule" entry is reserved, so there is no 0 in the enum
 */
enum btmodule_t{
  nomodule = 0,     //this should not be used
  hc05 = 1
};




/**
 * @brief BluetoothDriver configuration struct.
 *
 *  This struct contains multiple pointers to different bluetooth module config structs.
 *  Only one should be non-NULL. The unused ones shuld be set to NULL.
 *  If the unused pointers are a problem, #ifdef modulname method could be used to remove at coompile time
 *  The usedmodule variable shows which config pointer to use.
 *
 */
struct BluetoothConfig{
    char name[BLUETOOTH_MAX_NAME_LENGTH+1];
    char pincode[BLUETOOTH_MAX_PINCODE_LENGTH+1];
    enum btbitrate_t baudrate;
    thread_t *sendThread;
    thread_t *recieveThread;
    enum btmodule_t usedmodule;

    //config pointers from here
    struct hc05_config_t *myhc05config;

    //config pointers end here

};

/**
 * @brief BluetoothDriver object
 */
struct BluetoothDriver{
    struct BluetoothDeviceVMT *vmt;
    struct BluetoothConfig *config;
    input_queue_t *btInputQueue;
    output_queue_t *btOutputQueue;
    int driverIsReady;
    int commSleepTimeMs;
};


/**
 * @brief BluetoothDriver virtual methods table.
 */

struct BluetoothDeviceVMT {
    int (*sendBuffer)(struct BluetoothDriver *instance, char *buffer, int bufferlength);
    int (*sendByte)(struct BluetoothDriver *instance, int mybyte);
    int (*canRecieve)(struct BluetoothDriver *instance);
    int (*readBuffer)(struct BluetoothDriver *instance, char *buffer, int maxlength);
    int (*setPinCode)(struct BluetoothDriver *instance, char *pin, int pinlength);
    int (*setName)(struct BluetoothDriver *instance, char *newname, int namelength);
    int (*open)(struct BluetoothDriver *instance, struct BluetoothConfig *config);
    int (*close)(struct BluetoothDriver *instance);
    int (*resetModuleSettings) (struct BluetoothDriver * instance);
};







/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
int btSend(struct BluetoothDriver *instance, char *buffer, int bufferlength);
int btSendByte(struct BluetoothDriver *instance, int mybyte);
int btCanRecieve(struct BluetoothDriver *instance);
int btRead(struct BluetoothDriver *instance, char *buffer, int maxlen);
int btOpen(struct BluetoothDriver *instance, struct BluetoothConfig *config);
int btClose(struct BluetoothDriver *instance);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_BLUETOOTH */

#endif // BLUETOOTH_H_INCLUDED
/** @} */
