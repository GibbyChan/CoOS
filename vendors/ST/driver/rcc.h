/*
 * @Author: Gibby
 * @Date: 2025-07-08 23:48:47
 * @FilePath: \CoOS\vendors\ST\driver\rcc.h
 * @LastEditors: Gibby
 * @LastEditTime: 2025-07-08 23:52:29
 * @Description: 
 */

struct rcc {
  volatile uint32_t CR, CFGR, CIR, APB2RSTR, APB1RSTR, AHBENR, APB2ENR, APB1ENR, BDCR, CSR;
};
#define RCC ((struct rcc *) 0x40021000)
