#include <stdio.h>
#include <conio.h>

#include "MLP.h"
CMLP MultiLayer;

#define NUM_TRAINING_SET	20
#define NUM_INPUT	3
#define NUM_OUTPUT	2

#define MAX_EPOCH	100000

void DisplayMenu();
void LearningStart();
void SaveWeight();
void loadWeight();
void test();
int main()
{
	int HiddenNodes[2] = { 5,3 };
	int numofHiddenLayer = 2;
	MultiLayer.Create(NUM_INPUT, HiddenNodes, NUM_OUTPUT, numofHiddenLayer);

	int i, j;
	char ch;

	DisplayMenu();

	while ((ch = getche()) != EOF)
	{
		switch (ch)
		{
		case '1' :
			LearningStart();
			break;
		case '2':
			SaveWeight();
			break;
		case '3':
			loadWeight();
			break;
		case '4':
			test();
			break;
		case '0':
			return 0;	
			break;
		}
		DisplayMenu();
	}

	return 0;
}

void DisplayMenu()
{
	char menu[5][64] = {"[1]학습",
						"[2]Weight 저장",
						"[3]Weight 읽기",
						"[4]test",
						"[0]종료"
	};

	for (int i = 0; i < 5; i++)
	{
		printf("\n%s", menu[i]);
	}
	printf("\n선택메뉴:");
}

void LearningStart()
{
	int i, j;

	float x[NUM_TRAINING_SET][NUM_INPUT];
	float d[NUM_TRAINING_SET][NUM_OUTPUT];


	FILE *fp = fopen("learningdata.txt", "rt");
	if (fp == NULL)
	{
		printf("\n=>학습데이터를 읽을 수 없습니다.");
		return;
	}

	for (i = 0; i < NUM_TRAINING_SET; i++)
	{
		for (j = 0; j < NUM_INPUT; j++)
		{
			fscanf(fp, "%f", &x[i][j]);
			x[i][j] /= 256;
		}
		for (j = 0; j < NUM_OUTPUT; j++)
			fscanf(fp, "%f", &d[i][j]);
	}
	fclose(fp);


	printf("*************학습 시작********************\n");
	int epoch,n;
	double MSE;
	for (epoch = 0; epoch < MAX_EPOCH; epoch++)
	{
		MSE = 0.0;
		for (n = 0; n < NUM_TRAINING_SET; n++)
		{
			for (int i = 1; i <= NUM_INPUT; i++)
				MultiLayer.pInValue[i] = x[n][i - 1];
			for (int out_no = 1; out_no <= NUM_OUTPUT; out_no++)
				MultiLayer.pCorrectOutValue[out_no] = d[n][out_no - 1];	

			MultiLayer.Forward();
			for (int out_no = 1; out_no <= NUM_OUTPUT; out_no++)
				MSE += ((MultiLayer.pCorrectOutValue[out_no] - MultiLayer.pOutValue[out_no]) *
					(MultiLayer.pCorrectOutValue[out_no] - MultiLayer.pOutValue[out_no]));

			MultiLayer.BackPropagationLearning();
		}
		MSE /= NUM_TRAINING_SET;

		printf("Epoch%d(MSE)=%.15f\n", epoch, MSE);
	}
	printf("*************학습 종료********************\n");
	printf("학번:1234567\n");
	printf("이름:홍길동\n");
}

void SaveWeight()
{
	MultiLayer.SaveWeight("weight.txt");
	printf("\n=>가중치를 저장했습니다.");
}

void loadWeight()
{
	if (MultiLayer.LoadWeight("weight.txt"))
		printf("\n=>가중치 화일을 읽었습니다.");
	else
		printf("\n=>가중치 화일을 읽을 수가 없습니다.");

}

void test()
{
	int i;
	int rgb[3];

	printf("\n=>rgb값을 입력:");
	for (i = 0; i < NUM_INPUT; i++)
		scanf("%d", &rgb[i]);

	for (i = 1; i <= NUM_INPUT; i++)
		MultiLayer.pInValue[i] = rgb[i - 1] / 256.0;

	MultiLayer.Forward();

	printf("학번:1234567\n");
	printf("이름:홍길동\n");
	printf("\n[출력]%lf,%lf,%lf=%lf,%lf", MultiLayer.pInValue[1], MultiLayer.pInValue[2], MultiLayer.pInValue[3],
		MultiLayer.pOutValue[1], MultiLayer.pOutValue[2]);
}