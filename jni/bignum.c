#include "xj_linux_common.h" 
#include "bignum.h" 

typedef struct _Node    // 定义一个双向链表用来存贮结果
{
	char data;     // 数据*定义为字符的原因：要显示负号
	struct _Node *next;    // 尾指针
	struct _Node *ahead;   // 首指针
} Node;

//返回值说明:0是alongblong ; 2是along=blong
int abigerb(char *numa, char *numb) //比较两个数最高位那一个大
{
	int along=(int)strlen(numa); //标记数字a的长度；
	int blong=(int)strlen(numb); //标记数字b的长度；
	char *pna = numa; //指向数A的最高位，
	char *pnb = numb; //指向数B的最高位，
	if (along>blong) return 1;
	if (along==blong)
	{
		while(*pna) //比较两个数那一个大
		{
			if(*pna>*pnb)return 1;
			else if(*pna<*pnb)return 0 ;
			else if(*pna==*pnb){pna++;pnb++;} //1111与1112 
		}
		return 2; //这表明找到最后了还是全相等;
	}
	return 0 ;
}

char *findend(char *str)
{
	int len = strlen(str);
	char *p;
	if(len == 0)
	{
		return null;
	}
	p = &str[len-1];
	return p;
}
/*--------------------------------------------------------------------------
 *函数名称： 大数加法
 *函数过程：
 * 1 比较两个数那一个长
 * 2 以长的作为循环次数
 * 3 对应项相加 进位存贮直到下高位相加用
 * 4 直到循环结束
 * 5 没设计负数相加
 *入口参数：numa，numb，result字符串
 *出口参数：无
 *编辑环境：winSP2 + VC2003 + C++
 *--------------------------------------------------------------------------*/
void addition(char *numa, char *numb,char *result) // 计算两大数之和
{
	char *pna = findend(numa); // 指向numa的一个指针。point numa pna 指向乘数的最低位，
	char *pnb = findend(numb); //指向numb的一个指针 //pnb 指向被乘数的最低位，
	int along=(int)strlen(numa); //标记数字a的长度；
	int blong=(int)strlen(numb); //标记数字b的长度；
	int times = 0; // 标致要计算多少次。
	int carry=0,temp_result = 0; //存贮进位 和临时结果的
	Node *head, // 用于存贮头指针
		 *pstart, // 用于存贮计算时的首指针
		 *pnew; //作于申请新结点 
	head = pstart = (struct _Node *)malloc(sizeof(Node)); //初始化首结点和头结点。
	pstart -> data = 0;
	pstart -> next = NULL;
	pstart -> ahead = NULL; 

	if (abigerb(numa ,numb)>=1)
		times = (int)strlen(numa); //比较两个字符串长度，以大的作为循环次数
	else
	{
		times = (int)strlen(numb);
		pna = findend(numb); //交换指针
		pnb = findend(numa);
		along=(int)strlen(numb); //标记数字a的长度；
		blong=(int)strlen(numa); //标记数字b的长度；
	}
	while ((times-- && (times>=0))|| carry != 0)
	{ 
		if(!pstart->next) //如果当前为空结点，则申请新结点
		{
			pnew = (struct _Node *)malloc(sizeof(Node));
			pnew -> data = 0;
			pnew -> next = NULL;
			pnew -> ahead = pstart;
			pstart -> next = pnew;
		}
		temp_result =(pstart->data +(*pna-48)+(*pnb-48)+carry) ; //自身的值+新值+进位 作为当前的新值

		pstart -> data = temp_result%10; //存贮个位
		carry = temp_result/10; //存贮进位
		pstart = pstart -> next; //结点移动
		blong--;
		if(blong>0)pnb--; //指针移向被加数高位
		else *pnb=48; //之后相减就变为了0不作任何运算；
		pna--; //加数指针移动，
	}
	pstart =head; //寻找链表的结尾点
	while(pstart->next != 0)
	{
		pstart->data += 48; //!!<<<因为我们的输出是字符。所以再此加上48>>>> 逆顺输出
		pstart = pstart->next ;
	}

	int tip = 0; //转为字符串用
	pstart = pstart->ahead ; //找有效字
	//cout<<"\n结果是 : ";
	while(pstart != 0) //输出正序的结果；
	{
		result[tip++] = pstart->data; 
		//cout< data;
		pstart = pstart->ahead ;
	}
	result[tip] = '\0';

	pstart =head; //释放空间
	while(pstart->next != 0)
	{
		pnew = pstart->next ;
		free(pstart) ;
		pstart =pnew; 
	}
	return ;
}


/*--------------------------------------------------------------------------
 *函数名称： 大数减法
 *函数过程：1 比较两个数那一个长
 * 2 以长的作为循环次数
 * 3 如果两个数长度相等，从最高位开始比直到发现那一个数更大，使大项减去小项
 * 4 对应项相减 进位存贮直到下高位相加用
 * 5 每一位对应项相减时，处理三种可能的情况，a=b,ab;
 * 6 a=b时，则计算,11-12,111-112,要考虑借位
 * 7 直到循环结束
 *入口参数：numa，numb，result字符串
 *出口参数：无
 *--------------------------------------------------------------------------*/
void subtract(char *numa, char *numb,char *result)//计算减
{
	char *pna = findend(numa);//指向numa的一个指针。point numa      pna 指向减数的最低位，
	char *pnb = findend(numb);//指向numb的一个指针                 //pnb 指向被减数的最低位，
	int  along=(int)strlen(numa);//标记数字a的长度；
	int  blong=(int)strlen(numb);//标记数字b的长度；
	int  times = 0;         // 标记要计算多少次。
	int  carry=0;           //存贮借位
	int  clear0=0;          //消除结果最前面无用的'0'  13-5 = 08 的效果！！
	int  isnegative=0;      //用来加上被减数大于减数时补上最后一个负号
	Node *head,   // 用于存贮头指针
		 *pstart,  // 用于存贮计算时的首指针
		 *pnew;    //作于申请新结点
	head = pstart = (struct _Node *)malloc(sizeof(Node));//初始化首结点和头结点。
	pstart -> data = 0;
	pstart -> next = NULL;
	pstart -> ahead = NULL;
	if (abigerb(numa ,numb))
		times = strlen(numa);//比较两个字符串长度，以大的作为循环次数
	else //交换位置以降低难度
	{
		times = strlen(numb);//让数（字符串）长的减去数（字符串）短的
		pna = findend(numb);//交换指针
		pnb = findend(numa);
		along=(int)strlen(numb);//标记数字a的长度；
		blong=(int)strlen(numa);//标记数字b的长度；
		isnegative=1;//标记最后要加上负号
	}
	while ((times-- && (times>=0))|| carry != 0)//carry != 0 说没有借位时
	{
		if(!pstart->next)//如果当前为空结点，则申请新结点
		{
			pnew = (struct _Node *)malloc(sizeof(Node));
			pnew -> data = 0;
			pnew -> next = NULL;
			pnew -> ahead = pstart;
			pstart -> next = pnew;
		}
		if(times<0)//如果计算完之后，借位等于1，，说明必然为负值;
		{
			pstart -> data  = -3 ;//让它等于负号 '-'//-3来源于负号与0相差3。。                break;
		}
		else
		{
			if ( *pna == *pnb )//减数等于被减数时。结果等于直截相减的结果；并置借位为0
			{
				if(carry==0)
					pstart -> data = (*pna-48)-(*pnb-48); //111-11的情况
				else
				{
					pstart->data = (*pna-48)-(*pnb-48)+10 -carry;//1121-1112
					carry=1;
				}
			}
			if( *pna > *pnb )//减数大于被减数时。结果等于直截相减的结果；并置借位为0
			{
				pstart -> data = (*pna-48)-(*pnb-48)-carry;  //存贮个位
				carry=0;
			}
			else if( *pna < *pnb )//说明被减数大于减数，让结果加10，相当于借位 (carry)为1
			{
				if(times>0)
					pstart->data = (*pna-48)-(*pnb-48)+10 -carry;//13-5的情况作为新值
				else
				{
					pstart->data = (*pnb-48)-(*pna-48) -carry; //3-5  作为当前的新值
				}
				carry=1;
			}
		}
		pstart = pstart -> next;          //结点移动
		blong--;
		if(blong>0)pnb--;//指针移向被减数高位
		else *pnb=48;//之后相减就变为了0不作任何运算，其实可以优化的。但代码会长！而且还需要重新开结点。所以放弃；
		pna--;//被数指针移动，
	}
	if(isnegative==1)////加上负号处理。增加一长度并置为负号
	{
		pnew = (struct _Node *)malloc(sizeof(Node));
		pnew -> data = 0;
		pnew -> next = NULL;
		pnew -> ahead = pstart;
		pstart -> next = pnew;
		pstart->data=-3;//因为寻找链表的结尾点要统一加48。又因为‘-’是45。所以等于‘-3’
	}
	pstart =head;//寻找链表的结尾点
	while(pstart->next != 0)
	{
		pstart->data += 48;//!!<<<因为我们的输出是字符。所以再此加上48>>>>  逆顺输出
		pstart = pstart->next ;
	}
	int tip = 0;//转为字符串用
	clear0=0;//  消除结果最前面无用的'0'  13-5 = 08 的效果  ..>>修改字符串的首指针
	pstart = pstart->ahead ;//找有效字
	while(pstart != 0)//输出正序的结果；
	{
		if (clear0==0 && ((int)pstart->data)==48&&pstart->ahead!=0)//  消除结果最前面无用的'0'
			;//不输出任何东西
		else
			result[tip++] = pstart->data;
		if(((int)pstart->data)!=48&&((int)pstart->data)!=45)clear0=1;//'－'号
		pstart = pstart->ahead ;
	}
	result[tip] = '\0';
	pstart =head;  //释放空间
	while(pstart->next != 0)
	{
		pnew = pstart->next ;
		free(pstart);
		pstart =pnew;
	}
	return ;
}


/*--------------------------------------------------------------------------
 *函数名称： 大数乘法
 *函数过程：1 输入两个大数作为字符串
 * 2 作一个双向链表
 * 3 两个指针分别指向数字字符串的最低位
 * 4 以第一个数的最低的一个位乘以第二个数的所有项存于链表中
 * 5 链表首指针移
 * 6 重复4，5依次从最低位乘到最高位
 * 7 乘完后因为最低位是链表首，最后一位是链表尾。所以在逆顺输出链表。
 * 4 直到循环结束
 *入口参数：numa，numb，result字符串, 注意，入参会被修改
 *出口参数：无
 *--------------------------------------------------------------------------*/
void multiply(char *numa, char *numb ,char *result)//用来储结果的)//计算乘积
{
	char *pna = findend(numa);//指向numa的一个指针。point numa      pna 指向乘数的最低位，
	char *pnb = findend(numb);//指向numb的一个指针                 //pnb 指向被乘数的最低位，
	int along=(int)strlen(numa);//标记数字a的长度；
	int blong=(int)strlen(numb);//标记数字b的长度；
	int  carry=0,temp_result = 0;//存贮进位  和临时结果的
	Node *head,   // 用于存贮头指针
		 *pstart,  // 用于存贮计算时的首指针
		 *pnew,    //作于申请新结点
		 *pgo;     //作为每计算完一行时，回到下一行起始节点用,移位标致来用
	head = pstart = (struct _Node *)malloc(sizeof(Node));//初始化首结点和头结点。
	pstart -> data = 0;
	pstart -> next = NULL;
	pstart -> ahead = NULL;
	while (along--)
	{
		pgo = pstart;//保存进位点
		blong = (int)strlen(numb);//初始化长度
		pnb = findend(numb); //初始化指针
		while ((blong-- && (blong>=0))|| carry != 0)
		{
			if(!pstart->next)//如果当前为空结点，则申请新结点
			{
				pnew = (struct _Node *)malloc(sizeof(Node));
				pnew -> data = 0;
				pnew -> next = NULL;
				pnew -> ahead = pstart;
				pstart -> next = pnew;
			}
			if(blong<0)temp_result = carry ;//处理只有进位的情况
			else temp_result =(pstart->data+(*pna-48)*(*pnb-48)+carry);//自身值+新值+进位作为新值
			pstart -> data = temp_result%10;  //存贮个位
			carry = temp_result/10;           //存贮进位
			pstart = pstart -> next;          //结点移动
			pnb--; //指针移向被乘数高位
		}
		pstart = pgo->next; //前进一个位置；
		pna--;   //指针移向乘数高位
	}
	pstart =head;//寻找链表的结尾点
	while(pstart->next != 0)
	{
		pstart->data += 48;//!!<<<因为我们的输出是字符。所以再此加上48>>>>  逆顺输出
		pstart = pstart->next ;
	}
	int tip = 0;//转为字符串用
	pstart = pstart->ahead ;//找有效字
	while(pstart != 0)//输出正序的结果；
	{
		result[tip++] = pstart->data;
		pstart = pstart->ahead ;
	}
	result[tip] = '\0';
	pstart =head;  //释放空间
	while(pstart->next != 0)
	{
		pnew = pstart->next ;
		free(pstart);
		pstart =pnew;
	}
	return ;
}


/*--------------------------------------------------------------------------
 *函数名称： 大数除法 
 *函数想法：1 用指针指向除数的最高位，保存临时字符串； tempstr[a++] = pna
 * 2 如果临时字符串大于被除数，则相减。结果等于余数
 * if(tempstr>numb)tempstr = tempstr - numb
 * 3 如果小于被除数且指针到头，余数 等于 临时字符串
 * if(tempstr * 
 *入口参数：numa，numb，result,remainder字符串
 *出口参数：无
 *--------------------------------------------------------------------------*/
void divide( char *numa, char *numb,char *result,char *remainder)//计算除法
{
//	char one[]="1";//临时字符串....
//	char one2[]="1";//
//	char zero[]="0";//
	char numb2[6048];//
	char tempstr[6018]="";//临时字符串
	int  ia=0,ia2=0;//tempstr的指示器
	bool  moveon=false;//翻转牌
	char *pna = numa;//指向numa的一个指针。point numa      pna 指向减数的最低位，
//	char *pnb = findend(numb);//指向numb的一个指针                 //pnb 指向被减数的最低位，
	Node *head,   // 用于存贮头指针
		 *pstart,  // 用于存贮计算时的首指针
		 *pnew;    //作于申请新结点
	head = pstart = (struct _Node *)malloc(sizeof(Node));//初始化首结点和头结点。
	pstart -> data = 0;
	pstart -> next = NULL;
	pstart -> ahead = NULL;
	moveon = false;
	while(*pna)
	{
		if(!pstart->next)//如果当前为空结点，则申请新结点
		{
			pnew = (struct _Node *)malloc(sizeof(Node));
			pnew -> data = 0;
			pnew -> next = NULL;
			pnew -> ahead = pstart;
			pstart -> next = pnew;
		}
		ia=(int)strlen(tempstr);//取的长度
		tempstr[ia++] = *(pna++);
		tempstr[ia] ='\0'; //转换为字符串
		if(tempstr[0]=='0')//处理高位也是0的那种 如00
		{
			ia2=0;
			while(tempstr[ia2]=='0')++ia2;
			while(ia2>=1)//清除无用的0
			{
				ia=ia2-1;
				tempstr[ia]=tempstr[ia2];
				--ia2;
			}
			tempstr[++ia2]='\0';
		}
		while(abigerb(tempstr,numb)>0)//如果tempstr大于等于numb
		{
			if(tempstr[0]=='0')//处理高位也是0的那种 如00----此乃冗余代码，留做记念用
			{
				ia2=0;
				while(tempstr[ia2]=='0')++ia2;
				if(ia==ia2 )
				{
					moveon = true;
					break;
				}
			}
			strcpy(numb2,numb);
			subtract(tempstr, numb,tempstr);//A-B
			strcpy(numb,numb2);
			if(tempstr[0]=='-')//若判断的不准，变为了负数就再加上B。。
			{
				strcpy(numb2,numb);
				addition(tempstr, numb,tempstr);//A-B
				strcpy(numb,numb2);
				ia2=0; //修正之后的长度。因为加法中未做负数运算
				ia=0;  //为了消除最后的那一个负号，整体向前移动。
				while(tempstr[ia2]!='\0')++ia2;
				while(ia2>=1)//清除无用的0
				{
					tempstr[ia]=tempstr[ia+1];
					++ia;
					--ia2;
				}
				tempstr[ia]='\0';
				moveon = true;
				break;
			}
			pstart->data++ ; //结果自加
			moveon = true;
		}
		if(moveon) pstart = pstart -> next;          //结点移动
	}
	strcpy(remainder,tempstr);//存贮余数
	int tip = 0;//转为字符串用
	pstart =head;//寻找链表的结尾点
	while(pstart->next != 0)
	{
		pstart->data += 48;//!!<<<因为我们的输出是字符。所以再此加上48>>>>  逆顺输出
		result[tip++] = pstart->data;
		pstart = pstart->next ;
	}
	result[tip] = '\0';//存贮结果
	pstart =head;  //释放空间
	while(pstart->next != 0)
	{
		pnew = pstart->next ;
		free(pstart);
		pstart =pnew;
	}
	return ;
}


/*--------------------------------------------------------------------------
 *函数名称： 大数幂
 *函数想法：1 B自减1直到，，作为循环控制变量.保存结果； 
 * 2 结果初值1 每次乘以A
 * 3 重复1、2步骤到结束. 
 *入口参数：numa，numb，result字符串
 *出口参数：无
 *--------------------------------------------------------------------------*/
void power (char *numa, char *numb,char *result) //计算幂
{ char one[]="1"; //临时字符串....
	char one2[]="1"; 
	char zero[]="0"; 
	char numb2[6048]; 
	char remainder[6048]; 
	strcpy(result,one); //给结果初值为1
	strcpy(numb2,numb);

	subtract(numb,one ,remainder); //B自减1
	strcpy(numb,numb2);

	strcpy(numb2,numb);
	multiply(result,numa,result ); //A自己乘自己
	strcpy(numb,numb2); 

	while(strcmp(remainder,zero)>0) //A大于相等B时 就是numb不等于0时
	{
		strcpy(numb2,numb);
		multiply(result,numa,result ); //A自己乘自己
		strcpy(numb,numb2); 
		strcpy(one,one2);
		subtract(remainder,one ,remainder); //B减一
	}

	return ;
}

#if 0

int main(int argc, char **argv)
{
	char a[128] = {0};
	char b[128] = {0};
	char r[256] = {0};
	char remain[256] = {0};
	char r1[256] = {0};

	if(argc < 3)
	{
		printf("params is too small!\n");
		return -1;
	}

   strcpy(a, argv[1]);
   strcpy(b, argv[2]);
   
//   strcpy(a, "3");
//   strcpy(b, "23");

//   printf("a = %s\n", a);
   addition(a, b, r);
   printf("a = %s, b = %s, r = %s \n", a, b, r);
   

//   strcpy(a, "3");
//   strcpy(b, "3");

   strcpy(a, argv[1]);
   strcpy(b, argv[2]);
   subtract(a, b, r);
   printf("a = %s, b = %s, r = %s \n", a, b, r);


//   strcpy(a, "2");
//   strcpy(b, "3");

   strcpy(a, argv[1]);
   strcpy(b, argv[2]);
   multiply(a, b, r);
   printf("a = %s, b = %s, r = %s \n", a, b, r);

//   strcpy(a, "2");
//   strcpy(b, "3");

//   divide2(a, b, r, remain);
//   printf("a = %s, b = %s, r = %s, remain = %s \n", a, b, r, remain);

//   strcpy(a, "68");
//   strcpy(b, "43");

   strcpy(a, argv[1]);
   strcpy(b, argv[2]);
   divide2(a, b, r1, remain);
   printf("a = %s, b =  %s, r1 = %s, remain = %s \n",a, b, r1, remain);

   strcpy(a, argv[1]);
   strcpy(b, argv[2]);
   power(a, b, r);
   printf("a = %s, b = %s, r = %s \n", a, b, r);
    

   return 0;
}

#endif
