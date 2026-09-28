#ifndef SCULPTURE_H
#define SCULPTURE_H

class Sculpture {
public:
    Sculpture(float x, float y, float z, int type);
    
    void draw() const;
    void drawShadow(const float lightPos[4]) const;
    void update(float deltaTime);

private:
    float x, y, z;
    int type;
    float rotationAngle;

    void drawGeometry(bool isShadow) const;
};

#endif // SCULPTURE_H
