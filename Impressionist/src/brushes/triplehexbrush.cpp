#include "tripleHexbrush.h"
#include <paintview.h>
#include <math.h>
TripleHexBrush::TripleHexBrush(const std::string& name) :
    Brush(name)
{
}

void TripleHexBrush::BrushBegin(const glm::vec2 pos) {
    BrushMove(pos);
}

void TripleHexBrush::BrushMove(const glm::vec2 pos) {
    // Set the color
    int size = GetSize();
    // Points to draw
    glm::vec2 centers[3];
    GLfloat angle = GetAngle() * M_PI / 180;
    centers[0] = {pos.x + (sin(angle)*size), pos.y + (-cos(angle)*size)};
    centers[1] = {pos.x + (cos(angle)*size*sin(M_PI/3)-sin(angle)*size*0.5f), pos.y+(sin(angle)*size*sin(M_PI/3)+cos(angle)*size*0.5f)};
    centers[2] = {pos.x + (-cos(angle)*size*sin(M_PI/3)+sin(angle)*size*0.5f), pos.y+(sin(angle)*size*sin(M_PI/3)+cos(angle)*size*0.5f)};

    for(int i = 0; i < 3; i++ ){

        std::vector<GLfloat> hex(16);
        UseColor(GetColor(centers[i]));
        hex[0] = centers[i].x;
        hex[1] = centers[i].y;
        for(int j = 2; j < 16; j += 2){
            GLfloat angle = (GLfloat) ((j - 2)/2) * 60 * M_PI/180 + M_PI / 6;
            hex[j] = centers[i].x + size * cos(angle);
            hex[j + 1] = centers[i].y + size * sin(angle);
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * hex.size(), hex.data(), GL_STREAM_DRAW);

        // Set the size of the points and draw them
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

    }

}
void TripleHexBrush::BrushEnd(const glm::vec2 pos) {

}
