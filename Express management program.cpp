#pragma warning(disable:4996);

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

const char Courier_Account[20] = "1";
const char Courier_Password[20] = "1";

typedef struct node
{
	int type;
	int number;
	bool state;
	bool retention;
	time_t put_time;
	char takecode[20];
	char telephone_number[20];
	struct node* next;
}node, * pnode;
pnode creat()
{
	pnode head, rear, t;
	int i = 0;

	head = (pnode)malloc(sizeof(node));
	t = head;

	while (i++ < 10)
	{
		rear = (pnode)malloc(sizeof(node));
		rear->type = (i - 1) % 3 + 1;
		rear->number = i;
		rear->state = true;
		rear->retention = true;
		rear->put_time = 0;
		strcpy(rear->takecode, "0");
		strcpy(rear->telephone_number, "0");
		t->next = rear;
		t = rear;
	}

	t->next = NULL;

	return head;
}
bool Check_Courier(char* courier_account, char* courier_password)
{
	if (strcmp(Courier_Account, courier_account) == 0 && strcmp(Courier_Password, courier_password) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void Staff_Take_Delivery(pnode head)
{
	pnode t = head->next;

	printf("\t\t快递员取快递\n\n");
	printf("滞留快递的快递格口已全部打开，请快递员重新放入快递\n\n");
	printf("已重新放入所有滞留快递\n\n");

	while (t != NULL)
	{
		if (t->retention == true && t->state == false)
		{
			SYSTEMTIME m_time;
			GetLocalTime(&m_time);
			char szDateTime[20] = { 0 };
			sprintf_s(szDateTime, "%02d%02d%02d%02d%02d%02d%04d", m_time.wYear, m_time.wMonth,
				m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, m_time.wMilliseconds);

			t->put_time = time(NULL);
			strcpy(t->takecode, szDateTime);

			printf("%d号格口的新取件码为：%s\n", t->number, t->takecode);
		}

		Sleep(rand()%10);
		t = t->next;
	}
}
void traverse(pnode head)
{
	pnode t = head->next;
	int a = 0, b = 0, c = 0;
	bool operation = false;

	while (t != NULL)
	{
		char type[20], state[20];
		int get_time = t->put_time;
		time_t now_time = time(NULL);

		if (t->type == 1)
			strcpy(type, "小格口");
		else if (t->type == 2)
			strcpy(type, "中格口");
		else if(t->type == 3)
			strcpy(type, "大格口");

		if (t->state == true)
			strcpy(state, "可使用");
		else
			strcpy(state, "不可使用");

		if (t->state == true)
		{
			if (t->type == 1)
				a++;
			else if (t->type == 2)
				b++;
			else if (t->type == 3)
				c++;
		}

		printf("格口类型：%s\t格口编号：%d\t格口状态：%s", type, t->number, state);

		if (now_time - get_time >= 24*60*60 && t->state == false)
		{
			printf(" 滞留");
			operation = true;
			t->retention = true;
		}

		printf("\n");

		t = t->next;
	}

	printf("\n小格口剩余：%d个\t\t中格口剩余：%d个\t\t大格口剩余：%d个\n", a, b, c);
	
	if (operation)
	{
		int flag = 0;

		printf("\n有滞留的快递，是否取出重新放入（1：是 2：不是）");
		scanf("%d", &flag);

		if (flag == 1)
		{
			system("cls");
			Staff_Take_Delivery(head);		
		}
	}
}
void Query_Box(pnode head)
{
	printf("\t\t\t快递格口使用情况\n\n");
	traverse(head);
	printf("\n输入回车退出此界面");
	getchar();
	getchar();
}
void Send_Express(pnode head)
{
	char telephone_number[20] = { 0 };
	int type = 0;
	bool operation = false;

	printf("\t   派送快递\n\n");
	printf("输入用户手机号：");
	scanf("%s", &telephone_number);
	printf("\n格口类别:");
	printf("\t1：小格口\t2：中格口\t3：大格口\n\n");
	printf("选择您需要的格口类别：");
	scanf("%d", &type);

	pnode t = head->next;

	while (t != NULL)
	{
		if (t->state == true && t->type == type)
		{
			SYSTEMTIME m_time;
			GetLocalTime(&m_time);
			char szDateTime[20] = { 0 };
			sprintf_s(szDateTime, "%02d%02d%02d%02d%02d%02d%04d", m_time.wYear, m_time.wMonth,
				m_time.wDay, m_time.wHour, m_time.wMinute, m_time.wSecond, m_time.wMilliseconds);

			t->state = false;
			t->put_time = time(NULL);
			strcpy(t->takecode, szDateTime);
			strcpy(t->telephone_number, telephone_number);
			operation = true;
			
			printf("\n取件码为：%s\n", t->takecode);

			break;
		}

		t = t->next;
	}

	if (operation == true) 
	{
		printf("\n已成功放入快递，输入回车退出此界面");
	}
	else
	{
		if (type == 1)
		{
			printf("\n无空闲的小格口，输入回车退出此界面");
		}
		else if (type == 2)
		{
			printf("\n无空闲的中格口，输入回车退出此界面");
		}
		else if (type == 3)
		{
			printf("\n无空闲的大格口，输入回车退出此界面");
		}
		else
		{
			printf("\n无该类型的格口，输入回车退出此界面");
		}
	}

	getchar();
	getchar();
}
void Check_Delivery(char* telephone_number, pnode head)
{
	pnode t = head->next;

	while (t != NULL)
	{
		if (strcmp(t->telephone_number, telephone_number) == 0)
		{
			printf("您还有快递未取\n\n");
			break;
		}

		t = t->next;
	}
}
void Take_Delivery(pnode head)
{
	while (true)
	{
		pnode t = head->next;
		bool operation = false;
		char takecode[20], telephone_number[20];

		printf("\t用户取快递\n\n");
		printf("请输入取件码（输入quit退出）：");
		scanf("%s", &takecode);

		if (strcmp(takecode, "quit") == 0)
			break;

		while (t != NULL)
		{
			if (strcmp(t->takecode, takecode) == 0)
			{
				printf("\n快递格口已打开，快递格口编号为%d\n\n", t->number);

				operation = true;
				strcpy(telephone_number, t->telephone_number);
				t->state = true;
				memset(t->takecode, '\0', sizeof(t->takecode));
				memset(t->telephone_number, '\0', sizeof(t->telephone_number));

				Check_Delivery(telephone_number, head);

				printf("输入回车退出此界面");
				getchar();
				getchar();	
				system("cls");
				break;
			}

			t = t->next;
		}

		if (!operation)
		{
			system("cls");
			printf("暂无此取件码，请等候三秒后重新输入\n");
			Sleep(3000);
			system("cls");
		}
	}
}
int Write_To_Flie(pnode head)
{
	pnode t = head->next;
	FILE* write1 = fopen("data1.txt", "w");
	FILE* write2 = fopen("data2.txt", "w");

	if (write1 == NULL || write2 == NULL)
	{
		return 0;
	}

	while (t != NULL)
	{
		fprintf(write1, "%d %d %d %d %d\n", t->type, t->number, t->state, t->retention, t->put_time);
		fprintf(write2, "%s %s\n", t->takecode, t->telephone_number);

		t = t->next;
	}

	fclose(write1);
	fclose(write2);
}
pnode Read_The_File()
{
	time_t put_time;
	int type, number, state, retention;
	char takecode[20], telephone_number[20];

	pnode head = creat();
	pnode t1 = head->next;
	pnode t2 = head->next;
	FILE* data1 = fopen("data1.txt", "r");
	FILE* data2 = fopen("data2.txt", "r");

	if (data1 == NULL || data2 == NULL)
	{
		return head;
	}

	while (!feof(data1))
	{
		fscanf(data1, "%d %d %d %d %d\n", &type, &number, &state, &retention, &put_time);
		
		t1->type = type;
		t1->number = number;
		t1->state = state;
		t1->retention = retention;
		t1->put_time = put_time;
		//printf("%d ", t1->put_time);
		t1 = t1->next;
	}
	
	while (!feof(data2))
	{
		fscanf(data2, "%s %s\n", &takecode, &telephone_number);

		strcpy(t2->takecode, takecode);
		strcpy(t2->telephone_number, telephone_number);

		t2 = t2->next;
	}

	fclose(data1);
	fclose(data2);

	return head;
}
int main()
{
	pnode head = Read_The_File();

	while (true)
	{
		int t;

		printf("\t\t\t智能快递柜管理程序\n\n");
		printf("1：快递员登录\t\t2：用户取快递\t\t3：退出\n\n");
		printf("选择功能：");

		scanf("%d", &t);

		system("cls");

		if (t == 1)
		{
			char courier_account[20], courier_password[20];

			printf("\t快递员登录\n\n");
			printf("输入快递员账号：");
			scanf("%s", &courier_account);
			printf("输入快递员密码：");
			scanf("%s", &courier_password);

			system("cls");
			if (Check_Courier(courier_account, courier_password))
			{
				while (true)
				{
					printf("\t\t\t快递员功能\n\n");
					printf("1：查询快递格口使用情况\t\t2：派送快递\t\t3:退出\n\n");
					printf("选择功能：");

					int t1;

					scanf("%d", &t1);

					if (t1 == 1)
					{
						system("cls");
						Query_Box(head);
					}
					else if (t1 == 2)
					{
						system("cls");
						Send_Express(head);
					}
					else if (t1 == 3)
					{
						system("cls");
						break;
					}
					else
					{
						system("cls");
						printf("暂无此功能，请等候三秒后重新输入\n");
						Sleep(3000);
						system("cls");
					}

					system("cls");
				}
			}
			else
			{
				printf("不存在此快递员账号（敲击回车返回主菜单）");
				getchar();
				getchar();
				system("cls");
			}
		}
		else if (t == 2)
		{
			while (true)
			{
				printf("\t     用户功能\n\n");
				printf("1：取快递\t\t2：退出\n\n");
				printf("选择功能：");

				int t2;

				scanf("%d", &t2);

				if (t2 == 1)
				{
					system("cls");
					Take_Delivery(head);
				}
				else if (t2 == 2)
				{
					system("cls");
					break;
				}
				else
				{
					system("cls");
					printf("暂无此功能，请等候三秒后重新输入\n");
					Sleep(3000);
					system("cls");
				}

				system("cls");
			}
		}
		else if (t == 3)
		{
			printf("欢迎您再次使用！\n");
			break;
		}
		else
		{
			printf("暂无此功能，请等候三秒后重新输入\n");
			Sleep(3000);
			system("cls");
		}
	}

	Write_To_Flie(head);

	return 0;
} 
