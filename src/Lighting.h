#ifndef LIGHTING_H
#define LIGHTING_H

class Lighting {
public:
    Lighting();
    
    void init();
    void apply();
    void toggle();
    bool isEnabled() const { return enabled; }
    const float* getLightPos(int index) const;
    void drawFixtures() const;
    
private:
    bool enabled;
};

#endif // LIGHTING_H

