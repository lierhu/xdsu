/**
 * @file bignum.h
 * @brief 大数运算：加、减、乘、除、幂
 * @author hjc@xdja.com
 * @version 1.0
 * @date 2014-08-11
 */
#ifndef _BIGNUM_H_
#define _BIGNUM_H_

/**
 * @brief 大数相加
 * 函数过程：
 * 1 比较两个数那一个长
 * 2 以长的作为循环次数
 * 3 对应项相加 进位存贮直到下高位相加用
 * 4 直到循环结束
 * 5 没设计负数相加
 *
 * @param[in] numa 被加数，传参前需要复制，应为会被修改
 * @param[in] numb 加数，传参前需要复制，应为会被修改
 * @param[out] result 相加结果
 *
 */
void addition(char *numa, char *numb,char *result); // 计算两大数之和


/**
 * @brief 大数减法
 * 函数过程：
 * 1 比较两个数那一个长
 * 2 以长的作为循环次数
 * 3 如果两个数长度相等，从最高位开始比直到发现那一个数更大，使大项减去小项
 * 4 对应项相减 进位存贮直到下高位相加用
 * 5 每一位对应项相减时，处理三种可能的情况，a=b,ab;
 * 6 a=b时，则计算,11-12,111-112,要考虑借位
 * 7 直到循环结束		
 *
 * @param[in] numa 被减数，传参前需要复制，应为会被修改
 * @param[in] numb 减数，传参前需要复制，应为会被修改
 * @param[out] result 相减结果
 */
void subtract(char *numa, char *numb,char *result);//计算减


/**
 * @brief 大数乘法
 * 函数过程：
 * 1 输入两个大数作为字符串
 * 2 作一个双向链表
 * 3 两个指针分别指向数字字符串的最低位
 * 4 以第一个数的最低的一个位乘以第二个数的所有项存于链表中
 * 5 链表首指针移
 * 6 重复4，5依次从最低位乘到最高位
 * 7 乘完后因为最低位是链表首，最后一位是链表尾。所以在逆顺输出链表。
 * 4 直到循环结束
 *
 * @param[in] numa 被乘数
 * @param[in] numb 乘数
 * @param[out] result 积
 */
void multiply(char *numa, char *numb ,char *result);//用来储结果的)//计算乘积

/**
 * @brief 大数除法 
 * 函数过程：
 * 1 用指针指向除数的最高位，保存临时字符串； tempstr[a++] = pna
 * 2 如果临时字符串大于被除数，则相减。结果等于余数
 * if(tempstr>numb)tempstr = tempstr - numb
 * 3 如果小于被除数且指针到头，余数 等于 临时字符串
 *
 * @param[in] numa 被除数
 * @param[in] numb 除数
 * @param[out] result 商
 * @param[out] remainder 余数
 */
void divide( char *numa, char *numb,char *result,char *remainder);//计算除法

/**
 * @brief 大数幂
 * 函数想法：
 * 1 B自减1直到，，作为循环控制变量.保存结果； 
 * 2 结果初值1 每次乘以A
 * 3 重复1、2步骤到结束.
 *
 * @param[in] numa 指数
 * @param[in] numb 幂数
 * @param[out] result 结果
 */
void power (char *numa, char *numb,char *result); //计算幂
#endif
