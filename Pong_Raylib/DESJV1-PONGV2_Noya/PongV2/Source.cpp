#include "raylib.h"

enum GAMESTATE {
	menu,
	gamePvP,
	gameAI,
	end
}state;



int hits = 0;
bool showPowerUp;

Rectangle powerUp;

float ballSpeedX = 7.0f;
float ballSpeedY = 6.0f;

bool powerOnScreen = false;
bool powerOnScreen2 = false;
bool powerActive = false;
bool power2Active = false;
bool nextPower = false;
bool changeControl = false;

int main(void)
{

	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1280;
	const int screenHeight = 720;

	int verticalRandom = GetRandomValue(355.0f, 365.0f);
	int horizontalRandom = GetRandomValue(635.0f, 645.0f);

	int powerCapsuleL = 400;
	int powerCapsuleR = 700;
	int powerCapsuleUp = 50;
	int powerCapsuleDown = 670;
	

	InitAudioDevice();

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	Texture2D texture = LoadTexture("img/cancha.jpg");
	Texture2D texture1 = LoadTexture("img/download.png");
	Texture2D texture2 = LoadTexture("img/download2.png");
	Texture2D texture3 = LoadTexture("img/pelota.png");
	Sound fxWav = LoadSound("sound/oof.wav");
	Sound fxHit = LoadSound("sound/hit.wav");
	Music mainMenu = LoadMusicStream("sound/badguy.ogg");
	Music mainLoop = LoadMusicStream("sound/1942.ogg");
	Music win = LoadMusicStream("sound/win.ogg");

	DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
	DrawTexture(texture1, screenWidth / 20, screenHeight / 2 - 40, BLACK);
	DrawTexture(texture2, screenWidth - 100, screenHeight / 2 - 40, BLACK);
	DrawTexture(texture3, screenWidth / 2 - 50, screenHeight / 2 - 80, WHITE);
	
	// pelota
	Vector2 ballPosition = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	Vector2 ballSpeed = { ballSpeedX, ballSpeedY };
	Vector2 powerUpV;
	Vector2 powerUpV2;
	int ballRadius = 10;
	int powerRadius = 20;
	//rectangulos
	Rectangle rec1;
	rec1.x = screenWidth / 15;
	rec1.y = screenHeight / 2 - 40;
	rec1.width = 20;
	rec1.height = 150;
	Rectangle rec2;
	rec2.x = screenWidth - 100;
	rec2.y = screenHeight / 2 - 40;
	rec2.width = 20;
	rec2.height = 150;


	//Variables
	int puntaje1 = 0;
	int puntaje2 = 0;
	int timer = 0;
	int timerGoal = 60 * 5; // esto es porque cada 60frames se cuenta un segundo
	int timerGoalOut = 60 * 3;
	int timerGoalDissapear = 60 * 7;

	powerUp.height = 50;
	powerUp.width = 50;
	
	PlayMusicStream(mainMenu);
	SetMusicVolume(mainMenu, 0.75f);

	SetTargetFPS(60);
	int framesCounter = 0;

	bool collisions = false;
	bool empezarAJugar = false;
	bool cambiarColores = false;
	bool gol = false;
	bool controles = false;

	Color color1 = BLACK;
	Color color2 = BLACK;
	Color colorPelota = BLACK;
	int randColor = 0;

	//gamestate
	while (!WindowShouldClose())
	{
		int numPowerUpHeight = GetRandomValue(powerCapsuleUp, powerCapsuleDown);
		int numPowerUpWidth = GetRandomValue(powerCapsuleL, powerCapsuleR);
		switch (state)
		{
		case menu:
			while (!WindowShouldClose() && state == menu)
			{
				UpdateMusicStream(mainMenu);
				BeginDrawing();
				ClearBackground(BLACK);
				DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);

				DrawText("Controles", 600, 10, 20, WHITE);
				DrawText("Jugador 1: W para subir , S para bajar", 450, 40, 20, WHITE);
				DrawText("Jugador 2: UP para subir, Down para bajar", 430, 80, 20, WHITE);

				DrawText("Presiona Enter para empezar a jugar", 450, 250, 20, WHITE);
				DrawText("Presiona Space jugar contra la IA", 450, 280, 20, WHITE);
				DrawText("Presiona Scape para salir", 520, 340, 20, WHITE);

				if (IsKeyDown(KEY_ENTER))
				{
					state = gamePvP;
					StopMusicStream(mainMenu);
				}
				if (IsKeyDown(KEY_SPACE))
				{
					state = gameAI;
					StopMusicStream(mainMenu);
				}
				

				EndDrawing();
			}
			break;

		case gamePvP:
			while (!WindowShouldClose() && state == gamePvP)    // Detect window close button or ESC key
			{
				// Update
				//----------------------------------------------------------------------------------
				PlayMusicStream(mainLoop);
				UpdateMusicStream(mainLoop);
				SetMusicVolume(mainLoop, 0.2f);

				if (nextPower == false)
				{
					if (timer == timerGoal && powerActive == false)
					{
						timer = 0;
						powerOnScreen = true;
					}
					if (powerActive == true)
					{
						if (timer == timerGoalOut)
						{
							timer = 0;
							powerActive = false;
							nextPower = true;
						}
						ballSpeed.x *= 1.002f;
						ballSpeed.y *= 1.002f;
					}
				}
				else
				{
					if (timer == timerGoal && power2Active == false)
					{
						timer = 0;
						powerOnScreen2 = true;
					}

					if (power2Active == true)
					{
						changeControl = true;
						if (timer = timerGoalOut)
						{
							timer = 0;
							power2Active = false;

							nextPower = false;
						}
					}
				}

				if (changeControl == false)
				{
				//player1
				if (IsKeyDown(KEY_W)) rec1.y -= 7.0f;

				if (IsKeyDown(KEY_S)) rec1.y += 7.0f;

				//player2
				if (IsKeyDown(KEY_UP)) rec2.y -= 7.0f;
				if (IsKeyDown(KEY_DOWN)) rec2.y += 7.0f;
				}
				else
				{

					if (IsKeyDown(KEY_W)) rec1.y -= 15.0f;

					if (IsKeyDown(KEY_S)) rec1.y += 15.0f;

					if (IsKeyDown(KEY_UP)) rec2.y -= 15.0f;

					if (IsKeyDown(KEY_DOWN)) rec2.y += 15.0f;
				}

				//limite del nivel
				if (rec1.y < 0)
					rec1.y = 0;
				if (rec1.y + 150 > screenHeight)
					rec1.y = screenHeight - 150;

				if (rec2.y < 0)
					rec2.y = 0;
				if (rec2.y + 150 > screenHeight)
					rec2.y = screenHeight - 150;

				//movimiento / rebote
				ballPosition.x += ballSpeed.x;
				ballPosition.y += ballSpeed.y;

				if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius))
				{
					ballSpeed.y *= -1.0f;
					PlaySound(fxHit);
				}
				else
					framesCounter++;

				//colisiones
				if (CheckCollisionCircleRec(ballPosition, 10.0f, rec1))
				{
					ballSpeed.x *= -1.0f;
					PlaySound(fxHit);
				}
				else if (CheckCollisionCircleRec(ballPosition, 10.0f, rec2))
				{
					ballSpeed.x *= -1.0f;
					PlaySound(fxHit);
				}
			

			
				//power ups
				powerUpV.x = numPowerUpWidth;
				powerUpV.y = numPowerUpHeight;
				powerUpV2.x = numPowerUpWidth;
				powerUpV2.y = numPowerUpHeight;


				// TODO: Update your variables here
				//----------------------------------------------------------------------------------

				if (ballPosition.x >= screenWidth)
				{
					ballPosition.x = (float)screenWidth / 2;
					ballPosition.y = (float)screenHeight / 2;
					ballSpeed.x = 0;
					ballSpeed.y = 0;
					timer = 0;
					gol = true;
					puntaje1++;
					PlaySound(fxWav);
				}

				if (ballPosition.x <= 0)
				{
					ballPosition.x = (float)screenWidth / 2;
					ballPosition.y = (float)screenHeight / 2;
					ballSpeed.x = 0;
					ballSpeed.y = 0;
					puntaje2++;
					timer = 0;
					gol = true;
					PlaySound(fxWav);
				}

				if (IsKeyDown(KEY_SPACE) && gol == true)
				{
					int random = GetRandomValue(1, 2);
					ballSpeed.x = ballSpeedX;
					ballSpeed.y = ballSpeedY;
					if (random == 1)
					{
						ballSpeed.x *= -1;
						ballSpeed.y *= -1;

						ballPosition.y += ballSpeed.y;
						ballPosition.x += ballSpeed.x;
					}
					else if (random == 2)
					{
						ballPosition.x -= ballSpeed.x;
						ballPosition.y -= ballSpeed.y;
					}
					gol = false;
				}

				if (puntaje1 == 5 || puntaje2 == 5)
				{
					state = end;
					StopMusicStream(mainLoop);

				}
				

					
			
				timer++;

				// Draw
				//----------------------------------------------------------------------------------
				BeginDrawing();

				ClearBackground(BLACK);
				
				DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
				
				DrawText("Score", 585, 1, 20, GREEN);
				DrawText(TextFormat("Player 1: %i", puntaje1), 50, 50, 20, GREEN);
				DrawText(TextFormat("Player 2: %i", puntaje2), 1100, 50, 20, GREEN);

				DrawTexture(texture3, ballPosition.x - 14, ballPosition.y - 14, WHITE);
				
				DrawTexture(texture1, rec1.x - 110, rec1.y - 83 ,WHITE);
				DrawTexture(texture2, rec2.x - 110, rec2.y - 83, WHITE);

				if (powerOnScreen == true )
				{
					DrawCircleV(powerUpV, powerRadius, GREEN);
					if (timer == timerGoal)
					{
						timer = 0;
						powerOnScreen = false;
						nextPower = true;
					}
					if (CheckCollisionCircles(ballPosition, ballRadius, powerUpV, powerRadius))
					{
						timer = 0;
						powerActive = true;
						powerOnScreen = false;
					}
				}
				if (powerOnScreen2 == true)
				{
					DrawCircleV(powerUpV2, powerRadius, BLUE);
					if (timer == timerGoal)
					{
						timer = 0;
						powerOnScreen2 = false;
						nextPower = false;
					}
					if (CheckCollisionCircles(ballPosition, ballRadius, powerUpV2, powerRadius))
					{
						timer = 0;
						power2Active = true;
						powerOnScreen2 = false;
					}
				}
				EndDrawing();
				//----------------------------------------------------------------------------------
			}
			break;

		case gameAI:
			while (!WindowShouldClose() && state == gameAI)
			{
				PlayMusicStream(mainLoop);
				UpdateMusicStream(mainLoop);
				SetMusicVolume(mainLoop, 0.2f);

				if (IsKeyDown(KEY_W)) rec1.y -= 7.0f;

				if (IsKeyDown(KEY_S)) rec1.y += 7.0f;

				rec2.y = ballPosition.y - 75;

				if (rec1.y < 0)
					rec1.y = 0;
				if (rec1.y + 150 > screenHeight)
					rec1.y = screenHeight - 150;

				if (rec2.y < 0)
					rec2.y = 0;
				if (rec2.y + 150 > screenHeight)
					rec2.y = screenHeight - 150;

				//movimiento / rebote
				ballPosition.x += ballSpeed.x;
				ballPosition.y += ballSpeed.y;

				if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || (ballPosition.y <= ballRadius))
				{
					ballSpeed.y *= -1.0f;
					PlaySound(fxHit);
				}
				else
					framesCounter++;

				//colisiones
				if (CheckCollisionCircleRec(ballPosition, 10.0f, rec1))
				{
					ballSpeed.x *= -1.0f;
					PlaySound(fxHit);
				}
				else if (CheckCollisionCircleRec(ballPosition, 10.0f, rec2))
				{
					ballSpeed.x *= -1.0f;
					PlaySound(fxHit);
				}
				powerUpV.x = numPowerUpWidth;
				powerUpV.y = numPowerUpHeight;

				// TODO: Update your variables here
				//----------------------------------------------------------------------------------

				if (ballPosition.x >= screenWidth)
				{
					ballPosition.x = (float)screenWidth / 2;
					ballPosition.y = (float)screenHeight / 2;
					ballSpeed.x = 0;
					ballSpeed.y = 0;
					gol = true;
					timer = 0;
					puntaje1++;
					PlaySound(fxWav);
				}

				if (ballPosition.x <= 0)
				{
					ballPosition.x = (float)screenWidth / 2;
					ballPosition.y = (float)screenHeight / 2;
					ballSpeed.x = 0;
					ballSpeed.y = 0;
					timer = 0;
					puntaje2++;
					gol = true;
					PlaySound(fxWav);
				}

				if (IsKeyDown(KEY_SPACE) && gol == true)
				{
					int random = GetRandomValue(1, 2);
					ballSpeed.x = ballSpeedX;
					ballSpeed.y = ballSpeedY;
					if (random == 1)
					{
						ballSpeed.x *= -1;
						ballSpeed.y *= -1;

						ballPosition.y += ballSpeed.y;
						ballPosition.x += ballSpeed.x;
					}
					else if (random == 2)
					{
						ballPosition.x -= ballSpeed.x;
						ballPosition.y -= ballSpeed.y;
					}
					gol = false;
				}

				if (puntaje1 == 5 || puntaje2 == 5)
				{
					state = end;
					StopMusicStream(mainLoop);
				}

				if (timer == timerGoal && powerActive == false)
				{
					timer = 0;
					powerOnScreen = true;
				}
				if (powerActive == true)
				{
					if (timer == timerGoalOut)
					{
						timer = 0;
						powerActive = false;
					}
					ballSpeed.x *= 1.002f;
					ballSpeed.y *= 1.002f;
				}


				timer++;

				// Draw
				//----------------------------------------------------------------------------------
				BeginDrawing();

				ClearBackground(BLACK);

				DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);

				DrawText("Score", 585, 1, 20, GREEN);
				DrawText(TextFormat("Player 1: %i", puntaje1), 50, 50, 20, GREEN);
				DrawText(TextFormat("Player 2: %i", puntaje2), 1100, 50, 20, GREEN);

				DrawTexture(texture3, ballPosition.x - 14, ballPosition.y - 14, WHITE);

				DrawTexture(texture1, rec1.x - 110, rec1.y - 83, WHITE);
				DrawTexture(texture2, rec2.x - 110, rec2.y - 83, WHITE);
				if (powerOnScreen == true)
				{
					DrawCircleV(powerUpV, powerRadius, GREEN);
					if (timer == timerGoal)
					{
						timer = 0;
						powerOnScreen = false;
					}
					if (CheckCollisionCircles(ballPosition, ballRadius, powerUpV, powerRadius))
					{
						timer = 0;
						powerActive = true;
						powerOnScreen = false;
					}
				}

				EndDrawing();
			}
			break;


		case end:
			while (!WindowShouldClose() && state == end)
			{
				PlayMusicStream(win);
				UpdateMusicStream(win);
				SetMusicVolume(win, 0.2f);

				if (puntaje1 >= 5)
				{
					BeginDrawing();
					ClearBackground(BLACK);
					DrawText("El Jugador 1 Gana!", 470, 150, 40, WHITE);
					DrawText("Presiona Enter para jugar PVP", 490, 250, 20, WHITE);
					DrawText("Presiona Space para jugar contra la IA", 450, 350, 20, WHITE);
					DrawText("Presiona Scape para Salir", 520, 450, 20, WHITE);;

					if (IsKeyPressed(KEY_ENTER))
					{
						puntaje1 = 0;
						state = gamePvP;

					}
					if (IsKeyPressed(KEY_ENTER))
					{
						puntaje1 = 0;
						state = gameAI;
					}
					EndDrawing();
				}
				if (puntaje2 >= 5)
				{
					BeginDrawing();
					ClearBackground(BLACK);
					DrawTexture(texture, screenWidth / 2 - texture.width / 2, screenHeight / 2 - texture.height / 2, WHITE);
					DrawText("El Jugador 2 Gana!", 470, 150, 40, WHITE);
					DrawText("Presiona Enter para jugar PVP", 490, 250, 20, WHITE);
					DrawText("Presiona Space para jugar contra la IA", 450, 350, 20, WHITE);
					DrawText("Presiona Scape para Salir", 520, 450, 20, WHITE);

					if (IsKeyPressed(KEY_ENTER))
					{
						puntaje2 = 0;
						state = gamePvP;
						StopMusicStream(win);

					}
					if (IsKeyPressed(KEY_SPACE))
					{
						puntaje2 = 0;
						state = gameAI;
						StopMusicStream(win);
					}

					EndDrawing();
				}
			}
			break;
		}
	}
	UnloadMusicStream(win);
	UnloadMusicStream(mainMenu);
	UnloadMusicStream(mainLoop);
	UnloadSound(fxHit);
	UnloadSound(fxWav);
	UnloadTexture(texture3);
	UnloadTexture(texture2);
	UnloadTexture(texture1);
	UnloadTexture(texture);
	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}


