#include "raylib.h"
#include <vector>
#include <string>


// Bubble class：負責單一泡泡

class Bubble {
private:
    Vector2 position;   // 泡泡中心位置 存x 跟 y
    float radius;       // 半徑
    float speed;        // 上升速度
    Color color;        // 顏色 //Color 是一種struct 存rgba a(透明度) 1~255
    int score;          // 點擊後可得到的分數
    bool active;        // 是否還存在

public:
    Bubble(int screenWidth, int screenHeight) {
        radius = (float)GetRandomValue(20, 50); //raylib.h函式庫

        position.x = (float)GetRandomValue((int)radius, screenWidth - (int)radius);
        position.y = (float)(screenHeight + radius);

        speed = (float)GetRandomValue(80, 180);

        // 半徑越小越難點，所以分數越高
        if (radius <= 25) score = 30;
        else if (radius <= 35) score = 20;
        else score = 10;

        Color colors[5] = { SKYBLUE, PINK, GREEN, ORANGE, PURPLE };
        color = colors[GetRandomValue(0, 4)];

        active = true;
    }

    void update(float deltaTime) {
        if (!active) return;

        position.y = position.y - speed * deltaTime;

        // 飄出畫面上方就消失
        if (position.y + radius < 0) { 
            active = false;
        }
    }

    void draw() const {
        if (!active) return;

        DrawCircleV(position, radius, color);
        DrawCircleLines((int)position.x, (int)position.y, radius, DARKBLUE);
    }

    bool isClicked(Vector2 mousePos) const { 
        if (!active) return false;
        return CheckCollisionPointCircle(mousePos, position, radius); //檢查mousePos有沒有在圓圈裡面
    }

    int pop() {
        if (!active) return 0;
        active = false;
        return score;
    }

    bool isActive() const {
        return active;
    }
};


// BubbleGame 類別：負責整個遊戲流程

class BubbleGame {
private:
    int screenWidth;
    int screenHeight;
    int totalScore;
    std::vector<Bubble> bubbles; 

    float spawnTimer;
    float spawnInterval;

public:
    BubbleGame(int width, int height) {
        screenWidth = width;
        screenHeight = height;
        totalScore = 0;
        spawnTimer = 0.0f;
        spawnInterval = 0.35f;  // 每隔幾秒生成一顆新泡泡
    }

    void update() {
        float dt = GetFrameTime();

        // 不斷生成新泡泡
        spawnTimer =spawnTimer + dt;
        if (spawnTimer >= spawnInterval) {
            bubbles.push_back(Bubble(screenWidth, screenHeight));
            spawnTimer = 0.0f;
        }

        // 更新所有泡泡位置
        for (int i = 0; i < (int)bubbles.size(); i++) {
            bubbles[i].update(dt);
        }

        // 處理滑鼠點擊
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();

            for (int i = 0; i < (int)bubbles.size(); i++) {
                if (bubbles[i].isClicked(mousePos)) {
                    totalScore =totalScore + bubbles[i].pop();
                    break;  // 一次點一下只消一顆
                }
            }
        }

        // 清掉已失效的泡泡
        for (int i = 0; i < (int)bubbles.size();) {
            if (!bubbles[i].isActive()) {
                bubbles.erase(bubbles.begin() + i);
            } else {
                i++;
            }
        }
    }

    void draw() const {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 畫出所有泡泡
        for (int i = 0; i < (int)bubbles.size(); i++) {
            bubbles[i].draw();
        }

        // 分數與說明文字
        DrawText("EX06 - Click the bubble", 20, 20, 30, DARKBLUE);
        DrawText(TextFormat("Score: %d", totalScore), 20, 65, 25, MAROON);
        DrawText("Click bubbles to earn points!", 20, 100, 20, GRAY);

        EndDrawing();
    }
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "EX06 - Click the bubble");
    SetTargetFPS(60);

    BubbleGame game(screenWidth, screenHeight);

    while (!WindowShouldClose()) {
        game.update();
        game.draw();
    }

    CloseWindow();
    return 0;
}