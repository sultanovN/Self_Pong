#include "raylib.h"
#include <cstdint>
#include <string>
const static int s_paddle_padding = 30;
//static const char* text;
bool b_restart = true;

uint8_t left_player = 0;
uint8_t right_player = 0;

uint8_t ball_color = 0;
uint8_t paddle_color = 9;
uint8_t background_color = 0;

const uint8_t max_colors = 14u;
const Color colors[max_colors]
{
	{ 253, 249, 0, 255 }, { 255, 203, 0, 255 },
	{ 255, 161, 0, 255 }, { 255, 109, 194, 255 },
	{ 230, 41, 55, 255 }, { 190, 33, 55, 255 },
	{ 0, 228, 48, 255 }, { 0, 158, 47, 255 } ,
	{ 102, 191, 255, 255 }, { 0, 121, 241, 255 },
	{ 200, 122, 255, 255 }, { 135, 60, 190, 255 },
	{ 127, 106, 79, 255 }, { 255, 0, 255, 255 }
};
const uint8_t background_max = 4;

const Color backgroundColors[background_max]
{
	WHITE, BLACK, BEIGE, {0, 255, 255, 1}
};

Color colorChanger(uint8_t count, const Color colors[])
{
	return colors[count];
}

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
	Paddle(float x, float y, float speed, float width, float height, Color color = colorChanger(paddle_color, colors))
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

	void blockBorders()
	{
		if (m_y - m_height / 2 < 0)
			m_y = m_height / 2;
		if (m_y + m_height / 2 > GetScreenHeight())
			m_y = GetScreenHeight() - m_height / 2;
	}

	void move(Paddle &left_paddle, Paddle &right_paddle, bool &tutorial_left, bool &tutorial_right)
	{
		

		if (IsKeyDown(KEY_W))
		{
			tutorial_left = 1;
			left_paddle.m_y -= m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_S))
		{
			tutorial_left = 1;
			left_paddle.m_y += m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_UP))
		{
			tutorial_right = 1;
			right_paddle.m_y -= m_speed * GetFrameTime();
		}

		if (IsKeyDown(KEY_DOWN))
		{
			tutorial_right = 1;
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
	Ball(float x = GetScreenWidth() / 2.0f, float y = GetScreenHeight() / 2.0f, float speed_x = 150, float speed_y = 260, 
		float radius = 5, Color color = colorChanger(ball_color, colors))
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
			m_y = 0;
			m_speed_y *= -1.0f;
		}
			
		if (m_y > GetScreenHeight())
		{
			m_y = GetScreenHeight();
			m_speed_y *= -1.0f;
		}
			

		
	}

	//void winner()
	//{
	//	if (m_x < 0)
	//	{
	//		text = "Right Player Wins! Press Space To Restart";
	//		DrawText(text, GetScreenWidth()/2 - MeasureText(text, 30)/2, GetScreenHeight() / 2, 30, GRAY);
	//	}
	//	if (m_x > GetScreenWidth())
	//	{
	//		text = "Left Player Wins! Press Space To Restart";
	//		DrawText(text, GetScreenWidth()/2 - MeasureText(text, 30)/2, GetScreenHeight() / 2, 30, GRAY);
	//	}

	//}

	void isColliding(Paddle left_paddle, Paddle right_paddle)
	{
		if (CheckCollisionCircleRec(getVector(), m_radius, left_paddle.getRec()))
		{
			if (m_speed_x < 0)
			{
				m_speed_x *= -1.1f;
				m_speed_y = (m_y - left_paddle.getY()) / (left_paddle.getHeight() / 2) * m_speed_x;
			}
			
		}
			
		if (CheckCollisionCircleRec(getVector(), m_radius, right_paddle.getRec()))
		{
			if (m_speed_x > 0)
			{
				m_speed_x *= -1.1f;
				m_speed_y = (m_y - right_paddle.getY()) / (right_paddle.getHeight() / 2) * - m_speed_x;
			}
		}
	}

	void restart(Paddle &left_paddle, Paddle &right_paddle, uint8_t &ball_color, uint8_t &paddle_color) 
	{ 
		++ball_color;
		++paddle_color;
		*this = Ball{};
		left_paddle = Paddle{ s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100};
		right_paddle = Paddle{ GetScreenWidth() * 1.0f - s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100};
		b_restart = true;
	}

};

void scoreDraw(Ball ball)
{
	if (ball.getX() < 0 && b_restart)
		++right_player;         

	if (ball.getX() > GetScreenWidth() && b_restart)
		++left_player;

	std::string score = std::to_string(left_player) + ':' + std::to_string(right_player);

	DrawText(score.c_str(), GetScreenWidth() / 2 - MeasureText(score.c_str(), 30) / 2, 30, 30, GRAY);
}

void startText(bool s)
{
	if(!s)
		DrawText("Press Enter To Start", GetScreenWidth() / 2 - MeasureText("Press Enter To Start", 30) / 2, GetScreenHeight() / 2, 30, GRAY);
}
	

void tutorial(bool tutorial_left, bool tutorial_right)
{
	if (!tutorial_left)
		DrawText("W\nS", 30, GetScreenHeight() - 80, 20, GRAY);
	if (!tutorial_right)
		DrawText("UP\nDOWN", GetScreenWidth() - 80, GetScreenHeight() - 80, 20, GRAY);
}



int main()
{
	#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	InitWindow(800, 600, "Pong");

	SetWindowState(FLAG_VSYNC_HINT);

	

	Ball ball{};
	Paddle leftPaddle{ s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100};
	Paddle rightPaddle{ GetScreenWidth() * 1.0f - s_paddle_padding, GetScreenHeight() / 2.0f, 200, 10, 100};
	
	bool tutorial_left = 0;
	bool tutorial_right = 0;
	bool start = 0;

	while (!WindowShouldClose())
	{
		//Checking is start key pressed
		if(IsKeyPressed(KEY_ENTER))
			start = 1;

		//changing colors
		if (ball_color == max_colors - 1)
			ball_color = 0;
		if (paddle_color == max_colors - 1)
			paddle_color = 0;
		if (background_color == background_max - 1)
			background_color = 0;



		if (start)
		{
			
			leftPaddle.move(leftPaddle, rightPaddle, tutorial_left, tutorial_right);
			leftPaddle.blockBorders();
			rightPaddle.blockBorders();
			ball.move();
			ball.isColliding(leftPaddle, rightPaddle);
			scoreDraw(ball);
			if (ball.getX() < 0 || ball.getX() > GetScreenWidth())
				b_restart = false;
		}
		

		

		if (!b_restart)
		{
			++background_color;
			ball.restart(leftPaddle, rightPaddle, ball_color, paddle_color);
		}
			

		BeginDrawing();
		ClearBackground(colorChanger(background_color, backgroundColors));
		

		startText(start);
		DrawText("Press Esc To Exit", 0, 10, 20, DARKGRAY);
		ball.draw();
		tutorial(tutorial_left, tutorial_right);

		leftPaddle.draw();
		rightPaddle.draw();

		EndDrawing();
	}

	CloseWindow();

	return 0;
}