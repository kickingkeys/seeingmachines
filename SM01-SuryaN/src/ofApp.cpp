#include "ofApp.h"

void ofApp::setup() {
    ofSetFrameRate(30);
    
    aliveColor.setHex(0xED682B);
    deadColor.setHex(0x577455);
    
    grid.allocate(gridWidth, gridHeight, OF_IMAGE_GRAYSCALE);
    resetGrid(true);
}

void ofApp::resetGrid(bool randomize) {
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            grid.setColor(x, y, randomize ? (ofRandom(1) < 0.3 ? ofColor(255) : ofColor(0)) : ofColor(0));
        }
    }
    grid.update();
}

void ofApp::update() {
    if(isPaused) return;
    
    ofImage nextGrid;
    nextGrid.allocate(gridWidth, gridHeight, OF_IMAGE_GRAYSCALE);
    
    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            int neighbors = countNeighbors(x, y);
            bool isAlive = grid.getColor(x, y).getBrightness() > 127;
            
            if(isAlive) {
                if(neighbors < 2 || neighbors > 3)
                    nextGrid.setColor(x, y, ofColor(0));
                else
                    nextGrid.setColor(x, y, ofColor(255));
            } else {
                if(neighbors == 3)
                    nextGrid.setColor(x, y, ofColor(255));
                else
                    nextGrid.setColor(x, y, ofColor(0));
            }
        }
    }
    nextGrid.update();
    grid = nextGrid;
}

void ofApp::draw() {
    ofBackground(0);
    
    // Draw instructions
    ofSetColor(255, 255, 255, 191);
    ofDrawBitmapString("Keys:", 20, 30);
    ofDrawBitmapString("Space - Pause/Resume", 20, 50);
    ofDrawBitmapString("R - Reset Random", 20, 70);
    ofDrawBitmapString("C - Clear Grid", 20, 90);
   // ofDrawBitmapString("Click - Toggle Cell", 20, 110);
    
    // Calculate isometric grid dimensions
    float isoWidth = (gridWidth + gridHeight) * cellSize;
    float isoHeight = (gridWidth + gridHeight) * cellSize * 0.5;
    
    // Center grid with offset for isometric projection
    float centerX = ofGetWidth()/2;
    float centerY = ofGetHeight()/2;
    
    ofPushMatrix();
//    ofTranslate(centerX - isoWidth/4, centerY - isoHeight/4);
    ofTranslate(centerX, centerY - isoHeight/4);

    for(int x = 0; x < gridWidth; x++) {
        for(int y = 0; y < gridHeight; y++) {
            drawIsometricCell(x, y);
        }
    }
    ofPopMatrix();
}

void ofApp::drawIsometricCell(int x, int y) {
    float isoX = (x - y) * cellSize;
    float isoY = (x + y) * cellSize * 0.5;
    
    if(grid.getColor(x, y).getBrightness() > 127) {
        ofSetColor(aliveColor);
        float inset = 0.2;
        
        ofBeginShape();
        ofVertex(isoX - cellSize * (1-inset), isoY);
        ofVertex(isoX, isoY - cellSize * 0.5 * (1-inset));
        ofVertex(isoX + cellSize * (1-inset), isoY);
        ofVertex(isoX, isoY + cellSize * 0.5 * (1-inset));
        ofEndShape(true);
    }
    
    ofSetColor(deadColor);
    if(x < gridWidth-1) {
        ofDrawLine(isoX, isoY, isoX + cellSize, isoY + cellSize * 0.5);
    }
    if(y < gridHeight-1) {
        ofDrawLine(isoX, isoY, isoX - cellSize, isoY + cellSize * 0.5);
    }
    
    ofDrawCircle(isoX, isoY, 2);
}

int ofApp::countNeighbors(int x, int y) {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if(i == 0 && j == 0) continue;
            int nx = x + i;
            int ny = y + j;
            if(nx >= 0 && nx < gridWidth && ny >= 0 && ny < gridHeight) {
                if(grid.getColor(nx, ny).getBrightness() > 127) count++;
            }
        }
    }
    return count;
}

void ofApp::keyPressed(int key) {
    switch(key) {
        case ' ':
            isPaused = !isPaused;
            break;
        case 'r':
            resetGrid(true);
            break;
        case 'c':
            resetGrid(false);
            break;
    }
}

glm::vec2 ofApp::screenToGrid(float screenX, float screenY) {
    float centerX = ofGetWidth()/2;
    float centerY = ofGetHeight()/4;
    
    screenX -= centerX;
    screenY -= centerY;
    
    float cartX = (2 * screenY + screenX) / (2 * cellSize);
    float cartY = (2 * screenY - screenX) / (2 * cellSize);
    
    return glm::vec2(round(cartX), round(cartY));
}

void ofApp::mousePressed(int x, int y) {
    glm::vec2 gridPos = screenToGrid(x, y);
    int gridX = gridPos.x;
    int gridY = gridPos.y;
    
    if(gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
        bool currentState = grid.getColor(gridX, gridY).getBrightness() > 127;
        grid.setColor(gridX, gridY, currentState ? ofColor(0) : ofColor(255));
        grid.update();
    }
}
