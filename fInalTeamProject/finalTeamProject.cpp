#define _CRT_SECURE_NO_WARNINGS
#include <bangtal>
using namespace bangtal;
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ANIMATION_TIME		0.1f	//�����̴� �ð� ����
#define ANIMATION_STEP		150	//�����Ҷ� �̵��ϴ� �߰��� y��ǥ
#define DELAY_TIME 0.9f	//����Ű:������ ó�� �ð� ����

SceneID start_scene, main_scene, end_scene;
ObjectID cat, silver[4], gold[4]; //�����̴� ������Ʈ(�����), ���� ����, �ݻ� ���� ����

ObjectID start_button, end_button, end_button2; //��ŸƮ ��ư, ���� ��ư

ObjectID bar;	//�Ÿ� �� ������Ʈ ����
ObjectID barCh;	//�ٸ� �����̴� ĳ���� ����
SoundID sound;	//bgm

int coin[4]; //���� ����, ��ֹ� ���� ��ġ�� ���̹Ƿ� ��ֹ��� ���������� �迭�� ����
int coin_Y[4]; // ������ Y��ǥ ����	
int coin_count; //����
char buf[30]; //������ ǥ�����ֱ� ���� �Լ�

bool ended = false; // ������ ������ �ʾ��� = �⺻��

double barCh_x = 320;	//�ٸ� �����̴� ĳ������ �ʱ� ��ǥ ����

TimerID timerRelease = createTimer(DELAY_TIME);	//����Ű ������ ó�� Ÿ�̸�(���� �ð�)
TimerID sceneTimer = createTimer(1000); //���� �� �ð� ���� Ÿ�̸�
TimerID barTimer = createTimer(0.01f);	//�ٸ� �����̴� ĳ���� Ÿ�̸� ����

int x = 200, y = 50;	//�ʱ� ��ü ��ǥ
int dx = 0, dy = 0;		//��ü �̵� ��ǥ ����

KeyState keyState = KeyState::KEY_RELEASED;	//Ű���� ���� ����

void keyboardCallback(KeyCode code, KeyState state);	//Ű���� �ݹ� �Լ�
void timerCallback(TimerID timer);	//Ÿ�̸� �ݹ� �Լ�
void mouseCallback(ObjectID object, int x, int y, MouseAction action);

//��ֹ�
ObjectID ob[4];
int ob_x[4]; //��ֹ��� x��ǥ
int ob_dx = 0, speed = 6;
int ob_check[4];//��ֹ��� �̹����� Ȯ���ϱ� ���� ����

TimerID ob_speed_timer; //������ ����ɼ��� ��ֹ��� �̵��ϴ� �ӵ��� �������� �����ϴ� Ÿ�̸�
TimerID ob_timer; //��ֹ� ��ġ, ��ֹ��� �̵��ϵ��� �ϴ� Ÿ�̸�
TimerID check_timer; //��ֹ��� ��Ű�� �浹 Ȯ���ϴ� Ÿ�̸�

//������Ʈ �����ϴ� �Լ�
ObjectID createObject(const char* name, const char* image, SceneID scene, int x, int y, bool shown) {
	ObjectID object = createObject(image);
	locateObject(object, scene, x, y);
	showObject(object);

	if (shown) {
		showObject(object);
	}
	return object;
}

//ĳ���Ͱ� ���ο� �ε������� üũ�ϴ� �Լ�.
//�ε����� ������ ������鼭 ������ 5 ����
void CoinCheck() {
	for (int j = 0; j < 4; j++)
	{
		if (x >= ob_x[j] && x < ob_x[j] + 76 && coin_Y[j] > 0) {
			if (ob_check[j] == 1 && y == 200) {
				coin_Y[j] = -200;
				locateObject(coin[j], main_scene, ob_x[j] + 32, coin_Y[j]);
				hideObject(coin[j]);
				coin_count += 5;
			}
			if (ob_check[j] == 2 && y == 350) {
				coin_Y[j] = -200;
				locateObject(coin[j], main_scene, ob_x[j] + 32, coin_Y[j]);
				hideObject(coin[j]);
				coin_count += 5;
			}
		}
	}
}
void relocate(int i) {
	locateObject(ob[i], main_scene, ob_x[i], 50);
	locateObject(coin[i], main_scene, ob_x[i] + 32, coin_Y[i]);
	showObject(ob[i]);
	showObject(coin[i]);
}

void check() {
	ended = true;
	enterScene(start_scene); //��ֹ��� ������ �ٽ� �ʱ� ȭ������
	stopTimer(ob_speed_timer);
	stopTimer(ob_timer);
	stopTimer(barTimer);
	speed = 5;
	sprintf(buf, "Score : %d", coin_count);
	showMessage(buf);
	stopTimer(check_timer);
}

void keyboardCallback(KeyCode code, KeyState state)	//����Ű ���������� ���� ����Ű:������ ����->Ÿ�̸ӿ��� ó��
{
	if (code == KeyCode::KEY_SPACE && state == KeyState::KEY_PRESSED) {
		if (y > 50 + ANIMATION_STEP) return;	//2�� ������ �����ϰ� �����
		y += ANIMATION_STEP;
		locateObject(cat, main_scene, x, y);
		stopTimer(timerRelease);	//������ ����� Ÿ�̸� ���
		setTimer(timerRelease, DELAY_TIME);	//����Ű ������ Ÿ�̸� �ٽ� ����
		startTimer(timerRelease);	//Ÿ�̸� ����
	}
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action)
{
	if (object == start_button) { //���� ��ư�� ������ Ÿ�̸� ����
		coin_count = 0; // ���� �ʱ�ȭ
		ended = false; // ������ ������ ���� �ʱ�ȭ
		playSound(sound);
		enterScene(main_scene);
		for (int i = 0; i < 4; i++) {
			setObjectImage(ob[i], "images\\ob_low.png");
			coin_Y[i] = 200;
			ob_check[i] = 1;
			ob_x[i] = 500 + 500 * i;
			showObject(coin[i]);
			locateObject(ob[i], main_scene, ob_x[i], 50);
			locateObject(coin[i], main_scene, ob_x[i] + 32, 200);
		}
		barCh_x = 320;
		locateObject(barCh, main_scene, barCh_x, 680);
		startTimer(ob_timer);
		startTimer(check_timer);
		startTimer(ob_speed_timer);
		startTimer(barTimer);
	}
	else if (object == end_button) { //���� ��ư�� ������ Ÿ�̸� ����
		endGame();
	}
}

void timerCallback(TimerID timer)
{
	if (timer == timerRelease)
	{
		y = 50;
		locateObject(cat, main_scene, x, y);	//�����̴� ��ü �ٽ� ���ƿ��� �����
	}

	if (timer == barTimer) {	//�ٸ� �����̴� �Լ�
		hideObject(barCh);
		barCh_x = barCh_x + 0.076;
		locateObject(barCh, main_scene, barCh_x, 680);
		showObject(barCh);

		if (barCh_x > 930) {
			showMessage("Puang Run Complete!!");
			barCh_x = 320;
			stopSound(sound);
			enterScene(end_scene);
		}
		setTimer(barTimer, 0.01f);	//�ٸ� �����̴� �ð� ����
		startTimer(barTimer);	//�� Ÿ�̸� ����
	}

	if (timer == ob_timer) {
		if (y == 50 + ANIMATION_STEP or y == 50 + ANIMATION_STEP * 2) {
			CoinCheck();
		}
		for (int i = 0; i < 4; i++) {
			if (ob_x[i] >= -100) {
				ob_x[i] -= speed;
				locateObject(ob[i], main_scene, ob_x[i], 50);
				if (coin_Y[i] > 0) {
					locateObject(coin[i], main_scene, ob_x[i] + 32, coin_Y[i]);
				}
			}
			else if (ob_x[i] < -100) {
				if (i == 0) {
					ob_x[i] = 1800;
					int n = rand() % 2;
					switch (n) {
					case 0: setObjectImage(ob[i], "images\\ob_low.png"); ob_check[i] = 1; coin_Y[i] = 200; break;
					case 1: setObjectImage(ob[i], "images\\ob_high.png"); ob_check[i] = 2; coin_Y[i] = 350; break;
					}
					relocate(i);
				}
				else if (i == 1) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[0] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); coin_Y[i] = 200; ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[0] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); coin_Y[i] = 350;  ob_check[i] = 2; break;
					}
					relocate(i);
				}

				else if (i == 2) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[1] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); coin_Y[i] = 200;  ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[1] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); coin_Y[i] = 350; ob_check[i] = 2; break;
					}
					relocate(i);
				}
				else if (i == 3) {
					int n = rand() % 2;
					switch (n) {
					case 0: ob_x[i] = ob_x[2] + 400 + ob_dx, setObjectImage(ob[i], "images\\ob_low.png"); coin_Y[i] = 200; ob_check[i] = 1; break;
					case 1: ob_x[i] = ob_x[2] + 500 + ob_dx, setObjectImage(ob[i], "images\\ob_high.png"); coin_Y[i] = 350; ob_check[i] = 2; break;
					}
					relocate(i);
				}
			}
		}
		setTimer(ob_timer, 0.01f);
		startTimer(ob_timer);
	}

	if (timer == ob_speed_timer) {
		if (speed <= 50) {
			ob_dx += 0.2;
			speed += 0.2;
		}
		setTimer(ob_speed_timer, 5.0f);
		startTimer(ob_speed_timer);
	}

	if (timer == check_timer) {   //0.01�ʸ��� �浹 Ȯ��, check number�� �̿��ؼ� ��ֹ��� �̹����� ����, ��Ű ������ �� �߰� ��ġ������ ��� y = 50, 200�� �������� �浹 Ȯ����.
		for (int i = 0; i < 4; i++) {
			if (ob_x[i] < 279 && ob_x[i]> 94) {
				if (ob_check[i] == 1) {  // ���� ��ֹ��� ���
					if (y == 50) {
						check();
					}

				}
				else if (ob_check[i] == 2) {
					if (y == 50 || y == 200) {  //���� ��ֹ��� ���
						check();
					}
				}
			}
		}
		if (ended == false) { // ������ ������ �ʾ����� Ÿ�̸� ����, �������� Ÿ�̸� ���� X
			setTimer(check_timer, 0.01f);
			startTimer(check_timer);
		}
		else
			stopSound(sound);
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	setTimerCallback(timerCallback);
	setKeyboardCallback(keyboardCallback);
	setMouseCallback(mouseCallback);

	start_scene = createScene("���� ȭ��", "images\\start_scene.png");
	main_scene = createScene("���� ȭ��", "images\\main_scene.png");
	end_scene = createScene("���� ȭ��", "images\\end_scene.png");

	sound = createSound("Sounds/bgm.mp3");

	cat = createObject("cat", "images\\cat.png", main_scene, x, y, true);
	scaleObject(cat, 0.2f);


	for (int i = 0; i < 4; i++) {
		ob_x[i] = 500 + 500 * i;
		coin_Y[i] = 200;
		ob[i] = createObject("ob_block", "images\\ob_low.png", main_scene, ob_x[i], 0, true);
		ob_check[i] = 1;
		coin[i] = createObject("coin", "images\\silver.png", main_scene, ob_x[i] + 30, coin_Y[i], true);
		scaleObject(coin[i], .25f);
	}

	start_button = createObject("start_button", "images\\start_button.png", start_scene, 590, 80, true);
	end_button = createObject("end_button", "images\\end_button.png", start_scene, 590, 30, true);
	end_button2 = createObject("end_button", "images\\end_button.png", end_scene, 590, 50, true);
	bar = createObject("bar", "images\\bar.png", main_scene, 320, 680, true);	//��
	barCh = createObject("barCh", "images\\cat.png", main_scene, 320, 680, true);	//�ٸ� �����̴� ĳ����
	scaleObject(barCh, 0.07f);

	ob_speed_timer = createTimer(5.0f);
	ob_timer = createTimer(0.01f);
	check_timer = createTimer(0.01f);

	startGame(start_scene);
}