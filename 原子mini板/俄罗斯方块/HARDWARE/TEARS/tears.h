#ifndef _TEARS_H__
#define _TEARS_H__	

# include "stm32f10x_conf.h"
# include "sys.h"

# define Draw  1
# define Erase 0

# define down  2
# define right 1
# define left  0

# define Pass_Score 2000

typedef struct
{
	u32 level;
	u32 score;
}_Game;

typedef struct        //�Ƿ��ܣ���/��/�£��ƶ��ı�־
{
	u32 Left_Flag;
	u32 Right_Flag;
	u32 Down_Flag;
}_Move_Flag;

typedef struct        //ÿ����״�߽���������
{
	u32 x;
	u32 y;
}Position;

typedef struct       //��������һ���µ���״�ĸ�������
{
	u32 x;
	u32 y;
	u32 Num;
	u32 Types;
	u32 color;
}_Shape;

//��Ϸ���ݴ�����
extern _Game *User_Date(void);
extern void Show_UserDate(_Game *date);
extern void Update_UserLevel(_Game *date);

//ͼ��λ��װ�أ���ͼ�Ͳ�������
static void Load_Block1(Position * p_blcok,u32 x1,u32 y1,int x_move,int xi_times,int y_move,int yi_moves);
static void Load_Block2(Position * p_blcok,u32 x1,u32 y1,int x_move,int xi_times,int y_move,int yi_moves);

extern Position *Shape0(u32 x1,u32 y1,u32 types);
extern Position *Shape1(u32 x1,u32 y1,u32 types);
extern Position *Shape2(u32 x1,u32 y1,u32 types);
extern Position *Shape3(u32 x1,u32 y1,u32 types);
extern Position *Shape4(u32 x1,u32 y1,u32 types);
extern Position *Shape5(u32 x1,u32 y1,u32 types);
extern Position *Shape6(u32 x1,u32 y1,u32 types);

static void Draw_OneBlock(u32 x,u32 y,u32 color);
static void Draw_OneShape(Position *p_block,u32 color);
extern Position* LOad_Cur_Block(_Shape *New);
extern void Shape_Inform(_Shape *New,u32 Draw_OR_Erase);
extern void Dis_NextShape(u32 num,u32 types,u32 Draw_OR_Erase);

//�Ա������ʼ��
extern void Int_Block(void);   
//����Ϸ����
extern void Game_Interface(void);
//��Ϸ��ʼѡ����
extern void Select_StartSpeed(void);

//�߽�����ȷ���Ѿ��ж��Ƿ����ƶ�
extern _Move_Flag *Is_Flag(void);
extern u32 Read(Position next_block);
extern Position *Load_NextBlock(Position *p,u32 *s,u32 lines,u32 dir);
extern void Load_NextAndSetflag(_Shape *New, u32 (*s)[4],u32 (*lines)[3]);
extern void SetMove_Flag(Position *next_block,u32 lines,u32 dir);

//ͼ����������
extern void Clear_Shape(_Shape *New);
extern u16 Array_size(Position *p,u8 clear_lines);

//ͼ���ƶ�����
extern void Shape_Down_Move(_Shape *New);

//ʱ��������--����Ϸ��level����
extern u32 Set_level(_Game *date);

//��Ϸ�������ֺ���
extern void Gmae_Over(u32 Over_FLag);
//ͼ��λ�ý�������-�������������е�ͼ��
extern Position *Position_Correct(_Shape *New);

void LEFT_Key_FunCation(void);  //���ư����Ĺ���
void RIGHT_Key_FunCation(void);   //���ư����Ĺ���
void A_Key_FunCation(void);		  //A��--�任��
void B_Key_FunCation(void);		  //B��--�任��״

#endif
