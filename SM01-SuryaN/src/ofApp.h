//ofApp.h
#pragma once
#include "ofMain.h"

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    void mousePressed(int x, int y);
    glm::vec2 screenToGrid(float x, float y);
    
    ofImage grid;
    int gridWidth = 20;
    int gridHeight = 20;
    // adjusted to 30 to fit screen
    float cellSize = 30;
    ofColor aliveColor;
    ofColor deadColor;
    bool isPaused = false;
    
    void drawIsometricCell(int x, int y);
    int countNeighbors(int x, int y);
    void resetGrid(bool randomize = true);
};
