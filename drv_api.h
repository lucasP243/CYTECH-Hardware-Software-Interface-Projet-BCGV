/**
 * \file        drv_api.h
 * \author      Alexis Daley
 * \version     1.0
 * \date        27/02/2022
 * \brief       This file describe the interface with driver
 * \details     It provides following functions : 
 *              - drv_open : Open connection between client and driver
 *              - drv_open : Open connection between client and driver
 *              - drv_open : Open connection between client and driver
 *              - drv_open : Open connection between client and driver
 *              Must be delivered with static library (drv_api.a) and driver executable file (driver)
 */

#ifndef DRV_API_H_
#define DRV_API_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdint.h>

#define DRV_SUCCESS 0
#define DRV_ERROR   -1

#define DRV_MAX_FRAME_SIZE      (255)
#define DRV_MAX_FRAME_READ      (8)
#define DRV_UDP_10MS_FRAME_SIZE (14)
#define DRV_UDP_20MS_FRAME_SIZE (9)

typedef struct {
    uint32_t serNum;                    /* Physical ser number */
    uint8_t frame[DRV_MAX_FRAME_SIZE];  /* Frame received/to write */
    size_t frameSize;                   /* Size of received frame */
} lns_frame_t;

/**
 * \brief Open connection with driver, and returns file descriptor of channel
 * \param   void
 * \return int32_t : File descriptor (positive value), or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_open(void);

/**
 * \brief   Read UDP 10ms frame (blocking call)
 * \details Wait for next UDP 10ms frame and returns it
 * \param       drvFd       : File descriptor of channel with driver (get with open)
 * \param[out]  udpFrame    : Structure to fill with udp frame
 * \return int32_t : DRV_SUCCESS, or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_read_udp_10ms(int32_t drvFd, uint8_t udpFrame[DRV_UDP_10MS_FRAME_SIZE]);

/**
 * \brief   Write UDP 20ms frame
 * \param       drvFd       : File descriptor of channel with driver (get with open)
 * \param       udpFrame    : Structure to write
 * \return int32_t : DRV_SUCCESS, or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_write_udp_20ms(int32_t drvFd, uint8_t udpFrame[DRV_UDP_20MS_FRAME_SIZE]);

/**
 * \brief Give all lns message received since the last call
 * \param       drvFd       : File descriptor of channel with driver (get with open)
 * \param[out]  lnsData     : Structure to fill with read messages
 * \param[out]  lnsDataLen  : Number of element in lnsData structure
 * \return int32_t : DRV_SUCCESS, or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_read_lns(int32_t drvFd, lns_frame_t lnsData[DRV_MAX_FRAME_READ], uint32_t lnsDataLen);

/**
 * \brief Write data on lns lines
 * \param   drvFd       : File descriptor of channel with driver (get with open)
 * \param   lnsData     : Pointer on structure containing messages to write on lines
 * \param   lnsDataLen  : Number of element in lnsData structure
 * \return int32_t : DRV_SUCCESS, or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_write_lns(int32_t drvFd, lns_frame_t* lnsData, uint32_t lnsDataLen);

/**
 * \brief Close connection with driver
 * \param   drvFd       : File descriptor of channel with driver (get with open)
 * \return int32_t : DRV_SUCCESS, or DRV_ERROR if an error occurs (errno is set)
 */
int32_t drv_close(int32_t drvFd);

#endif /* DRV_API_H_ */
