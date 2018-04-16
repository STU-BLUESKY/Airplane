#include "stm32f10x.h"

#define DATA_LEN (32)
#define MODE_MAX (4)



#define BATTERY_MAX (66)

extern u8 CurMode;
extern u8 NextMode;

typedef struct node*  BinNodePoniter;
typedef struct node
{
 u8 ord; 
 struct node *parent,*last_sibling;
 struct node *first_child,*next_sibling;
}BinNode;


void chooseMode(void);
void mode_2(void);		//ģʽ����
void mode_6(void);		//��������
void mode_8(void);		//���͵���
void mode_9(void);		//���յ���
void mode_11(void);		//��̬�ǽ���
void mode_13(void);		//��������ʾ


void changeMode(void);
void changeMode_1(void);
void changeMode_2(void);
void changeMode_3(void);
void changeMode_8(void);
void changeMode_9(void);
void changeMode_11(void);
void changeMode_13(void);








