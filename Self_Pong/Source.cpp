#include "raylib.h"
#include <cstdint>
#include <string>
const static int s_paddle_padding = 30;
static const char* text;
Sound kick; 
uint16_t left_player = 0;
uint16_t right_player = 0;

class Paddle
{
private:
	float m_x;
	float m_y;
	float m_speed;
	float m_width;
	float m_height;
	Color m_color;
public:
	Paddle(float x, float y, float speed, float width, float height, Color color)
		: m_x{x}, m_y{y}, m_speed{speed}, m_width{width}, m_height{height}, m_color{color}
	{

	}

	Rectangle getRec() const { return { m_x - m_width / 2, m_y - m_height / 2, m_width, m_height }; }

	void draw()
	{
		DrawRectangleRec(getRec(), m_color);
	}

	float getY()const { return m_y; }

	float getHeight()const { return m_height; }

	void move(Paddle &left_paddle, Paddle &right_paddle)
	{
		if (IsKeyDown(KEY_W))
		{
			left_paddle.m_y -= m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_S))
		{
			left_paddle.m_y += m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP))
		{
			right_paddle.m_y -= m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_DOWN))
		{
			right_paddle.m_y += m_speed * GetFrameTime();
		}

	}
};

class Ball
{
private:
	float m_x;
	float m_y;
	float m_speed_x;
	float m_speed_y;
	float m_radius;
	Color m_color;
public:
	Ball(float x, float y, float speed_x, float speed_y, float radius, Color color)
		: m_x{x}, m_y{y}, m_speed_x{speed_x}, m_speed_y{speed_y}, m_radius{radius}, m_color{color}
	{

	}

	Vector2 getVector()const { return Vector2{ m_x, m_y }; }

	float getRadius()const { return m_radius; }

	float getX()const { return m_x; }

	void draw()
	{
		DrawCircle(m_x, m_y, m_radius, m_color);
	}

	void move()
	{
		m_x += m_speed_x * GetFrameTime();
		m_y += m_speed_y * GetFrameTime();

		if (m_y < 0)
		{
			PlaySound(kick);
			m_speed_y *= -1.0f;
		}
			
		if (m_y > GetScreenHeight())
		{
			PlaySound(kick);
			m_speed_y *= -1.0f;
		}
			

		
	}

	void winner()
	{
		if (m_x < 0)
		{
			text = "Right Player Wins! Press Space To Restart";
			DrawText(text, GetScreenWidth()/2 - MeasureText(text, 30)/2, GetScreenHeight() / 2, 30, WHITE);
		}
		if (m_x > GetScreenWidth())
		{
			text = "Left Player Wins! Press Space To Restart";
			DrawText(text, GetScreenWidth()/2 - MeasureText(text, 30)/2, GetScreenHeight() / 2, 30, WHITE);
		}

	}

	void isColliding(Paddle left_paddle, Paddle right_paddle)
	{
		if (CheckCollisionCircleRec(getVector(), m_radius, left_paddle.getRec()))
		{
			if (m_speed_x < 0)
			{
				PlaySound(kick);
				m_speed_x *= -1.2f;
				m_speed_y = (m_y - left_paddle.getY()) / (left_paddle.getHeight() / 3) * m_speed_x;
			}
			
		}
			
		if (CheckCollisionCircleRec(getVector(), m_radius, right_paddle.getRec()))
		{
			if (m_speed_x > 0)
			{
				PlaySound(kick);
				m_speed_x *= -1.2f;
				m_speed_y = (m_y - right_paddle.getY()) / (right_paddle.getHeight() / 3) * - m_speed_x;
			}
		}
	}

	void restart() 
	{ 
		*this = Ball{ GetScreenWidth() / 2.0f , GetScreenHeight() / 2.0f, 50, 200, 5, ORANGE };
		text = nullptr;
	}

};

void scoreDraw(Ball ball)
{
	if (ball.getX() < 0 && text == nullptr)
		++right_player;         

	if (ball.getX() > GetScreenWidth() && text == nullptr)
		++left_player;

	std::string score = std::to_string(left_player) + ':' + std::to_string(right_player);

	DrawText(score.c_str(), GetScreenWidth() / 2 - MeasureText(score.c_str(), 30) / 2, 30, 30, WHITE);
}

void startText(bool s)
{
	if(!s)
		DrawText("Press Enter To Start", GetScreenWidth() / 2 - MeasureText("Press Enter To Start", 30) / 2, GetScreenHeight() / 2, 30, WHITE);
}
	

int main()
{
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	InitWindow(800, 600, "Pong");
	InitAudioDevice();
	kick = LoadSound("Self_Pong\Self_Pong\kick.wav");

	SetWindowState(FLAG_VSYNC_HINT);

	Ball ball{ GetScreenWidth() / 2.0f , GetScreenHeight() / 2.0f, 100, 200, 5, ORANGE };
	Paddle leftPaddle{ s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100, BLUE };
	Paddle rightPaddle{ GetScreenWidth() * 1.0f - s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100, BLUE };
	
	bool start = 0;

	while (!WindowShouldClose())
	{
		if(IsKeyPressed(KEY_ENTER))
			start = 1;

		if (start)
		{
			
			leftPaddle.move(leftPaddle, rightPaddle);
			ball.move();
			ball.isColliding(leftPaddle, rightPaddle);
			scoreDraw(ball);

			ball.winner();
		}
		

		

		if (IsKeyPressed(KEY_SPACE) && text != nullptr)
			ball.restart();

		BeginDrawing();
		ClearBackground(BLACK);
		

		startText(start);
		DrawText("Press Esc To Exit", 0, 10, 20, DARKGRAY);
		ball.draw();
		

		leftPaddle.draw();
		rightPaddle.draw();

		EndDrawing();
	}

	UnloadSound(kick);
	CloseAudioDevice();
	CloseWindow();

	return 0;
}