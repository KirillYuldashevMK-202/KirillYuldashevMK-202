#include <iostream>
#include <raylib.h>
#include <thread>
#include <math.h>

#define screenWidth 1200
#define screenHeight 960

class Planet{
public:
    float rAngle;
    int P_size;
    Color P_color;
    Planet(int size, int speed,int place, Color color){
        this->P_size = size;
        this->P_speed = speed;
        this->P_place = place;
        this->P_color = color;
        this->rAngle = 0.0f;
    }
    void Draw(){
        while(true){
            float distance = 80.0f * this->P_place;
            float rSpeed = 0.1f * this->P_speed;
            this->rAngle += rSpeed;
            this->circleX = screenWidth / 2 + distance * cos(rAngle * DEG2RAD);
            this->circleY = screenHeight / 2 + distance * sin(rAngle * DEG2RAD);
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }
    float GetX() const { return circleX; }
    float GetY() const { return circleY; }
    ~Planet(){}
private:
    float circleX;
    float circleY;
    int P_speed;
    int P_place;
};

int main () {
    Planet yupiter(10,10,1,BLUE);
    Planet venera(20,15,2,WHITE);
    Planet mars(30,12,3,RED);
    Planet neptun(35,7,4,PURPLE);
    Planet earth(25,10,5,GREEN);

    InitWindow(screenWidth, screenHeight, "Planet");
    SetTargetFPS(60);

    std::thread t1(&Planet::Draw, &yupiter);
    std::thread t2(&Planet::Draw, &venera);
    std::thread t3(&Planet::Draw, &mars);
    std::thread t4(&Planet::Draw, &neptun);
    std::thread t5(&Planet::Draw, &earth);

    while (WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(screenWidth/2,screenHeight/2,50, YELLOW);
        DrawCircle(yupiter.GetX(), yupiter.GetY(), yupiter.P_size, yupiter.P_color);
        DrawCircle(venera.GetX(), venera.GetY(), venera.P_size, venera.P_color);
        DrawCircle(mars.GetX(), mars.GetY(),mars.P_size, mars.P_color);
        DrawCircle(neptun.GetX(), neptun.GetY(), neptun.P_size, neptun.P_color);
        DrawCircle(earth.GetX(),earth.GetY(), earth.P_size, earth.P_color);

        EndDrawing();
    }

    t1.join();t2.join();t3.join();t4.join();t5.join();

    CloseWindow();
    return 0;
}
