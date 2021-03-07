/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,ɽ��Ƽ�
 *     All rights reserved.
 *     �������ۣ�ɽ����̳ http://www.vcan123.com
 *
 *     ��ע�������⣬�����������ݰ�Ȩ����ɽ��Ƽ����У�δ����������������ҵ��;��
 *     �޸�����ʱ���뱣��ɽ��Ƽ��İ�Ȩ������
 *
 * @file       main.c
 * @brief      ɽ��K60 ƽ̨������
 * @author     ɽ��Ƽ�
 * @version    v5.0
 * @date       2013-08-28
 */

#include "common.h"
#include "include.h"


uint8_t     usb_com_rx_len = 0;
uint8_t     rx_buf[64];

/*!
 *  @brief      main����
 *  @since      v5.0
 *  @note       ɽ�� USB ���⴮�� ����ʵ��
                ע�⣬��û���� �жϽ��գ������������̫�죬���п��ܻᶪʧ
 */
void  main(void)
{
    usb_com_init();                                 //��ʼUSBΪ ���⴮��ģʽ

    usb_enum_wait();                                //�ȴ� PC ö��

    while(1)
    {
        CDC_Engine();                               //USB_CDC ���洦��

        usb_com_rx_len = usb_com_rx(rx_buf);        //��ѯ���ݽ���
        if(usb_com_rx_len > 0)
        {
            usb_com_tx(rx_buf, usb_com_rx_len);     //��������
            //usb_com_rx_len = 0;
        }
    }
}
