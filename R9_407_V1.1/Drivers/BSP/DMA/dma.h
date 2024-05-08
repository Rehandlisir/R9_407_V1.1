/**
 ****************************************************************************************************
 * @file        dma.h
 * @author      R9�Ŷ�
 * @version     V1.0
 * @date        2021-10-18
 * @brief       DMA ��������
 * @license     Copyright (c) 2020-2032, ����ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ҽ��
 *����ҽ�ƿƼ����޹�˾
 * ����ҽ�ƿƼ����޹�˾
 * ����ҽ�ƿƼ����޹�˾
 * ����ҽ�ƿƼ����޹�˾
 *
 * �޸�˵��
 * ����ҽ�ƿƼ����޹�˾
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __DMA_H
#define	__DMA_H

#include "./SYSTEM/sys/sys.h"

void dma_map_config(DMA_Stream_TypeDef* DMAx_CHx,uint32_t src,uint32_t dest);                 /* �ڴ�ӳ�������ʼ������ */
void dma_usart1_tx_config(uint32_t mar);                        /* ����1 TX DMA��ʼ�� */
void dma_enable(DMA_Stream_TypeDef *dmax_chy, uint16_t ndtr); /* ʹ��һ��DMA���� */
void dma_basic_config(DMA_Stream_TypeDef *dmax_chy,  uint32_t par, uint32_t mar);  /* DMA�������� */
#endif






























